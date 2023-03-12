pub(crate) mod configuration;
mod sensor;

use std::sync::atomic::{AtomicBool, Ordering};

use configuration::Configuration;
use log::*;
use rumqttc::{AsyncClient, MqttOptions, QoS};
use tokio::task;

use self::sensor::home_assistant::DEFAULT_EXPIRE_AFTER;
use self::sensor::message::Message;
use self::sensor::Sensor;

const SENSOR_PATH: &'static str = "/etc/msm/sensors/";

pub struct Daemon {
    config: Configuration,
    mqtt_config: MqttOptions,

    sensors: Vec<Sensor>,
    stop: AtomicBool,
}

impl Daemon {
    pub fn new(config: Configuration) -> Result<Daemon, serde_yaml::Error> {
        info!("Daemon for {} starting", config.client_id);

        let mqtt_config = MqttOptions::new(
            &config.client_id,
            &config.mqtt_broker,
            config.mqtt_broker_port,
        );

        info!(
            "Connecting to MQTT broker {}:{}",
            config.mqtt_broker, config.mqtt_broker_port
        );

        let mut sensors: Vec<Sensor> = Default::default();

        for sensor in &config.sensors {
            info!("Loading sensor {sensor}");
            let script = match std::fs::read_to_string(String::from(SENSOR_PATH) + sensor + ".lua")
            {
                Ok(script) => script,
                Err(error) => panic!("{error}"),
            };
            sensors.push(Sensor::new(
                sensor.clone(),
                script,
                &config.prefix,
                &config.client_id,
            ));
        }

        Ok(Daemon {
            config,
            mqtt_config,
            sensors,
            stop: AtomicBool::new(false),
        })
    }

    pub async fn run(self: &mut Daemon) {
        let mut cycles_counter = 0;
        let mut register: bool;

        let expire_cycles = DEFAULT_EXPIRE_AFTER / self.config.update_period - 1;
        let sleep_period = std::time::Duration::from_secs(self.config.update_period);

        let (client, mut eventloop) = AsyncClient::new(self.mqtt_config.clone(), 1);

        task::spawn(async move {
            while let Ok(notification) = eventloop.poll().await {
                debug!("MQTT notification received: {:?}", notification);
            }
        });

        while !self.stop.load(Ordering::Relaxed) {
            register = cycles_counter == 0 && self.config.homeassistant;

            self.notify_state(&client, true).await;

            for sensor in &mut self.sensors {
                sensor.exec();
                let message = sensor.get_message();

                Daemon::publish(&client, message).await;

                if register {
                    let message = sensor.get_homeassistant_descriptor();

                    Daemon::publish(&client, message).await;
                }
            }

            if register {
                let message = self.get_homeassistant_registration_message();

                Daemon::publish(&client, message).await;
            }

            if cycles_counter == expire_cycles {
                cycles_counter = 0;
            } else {
                cycles_counter += 1;
            }

            std::thread::sleep(sleep_period);
        }

        self.notify_state(&client, false).await;
    }

    async fn publish(client: &AsyncClient, data: sensor::message::Message) {
        debug!("Publishing {:?}", data);

        match client
            .publish(&data.topic, QoS::AtLeastOnce, false, data.value.clone())
            .await
        {
            Err(message) => error!("MQTT publish error: {}", message),
            _ => {}
        }
    }

    fn get_state_topic(self: &Daemon) -> String {
        format!("{}/{}/state", self.config.prefix, self.config.client_id)
    }

    pub async fn notify_state(self: &Daemon, client: &AsyncClient, state: bool) {
        Daemon::publish(
            client,
            Message {
                topic: self.get_state_topic(),
                value: if state {
                    String::from("ON")
                } else {
                    String::from("OFF")
                },
            },
        )
        .await
    }

    fn get_homeassistant_registration_message(self: &Daemon) -> Message {
        let descriptor = sensor::home_assistant::Descriptor {
            name: format!("{} State", self.config.client_id),
            class: "connectivity".to_string(),
            unit_of_measurement: None,
            device: sensor::home_assistant::DeviceDescriptor {
                name: self.config.client_id.clone(),
                model: self.config.client_id.clone(),
                identifiers: self.config.client_id.clone(),
            },
            expire_after: 60,
            state_topic: self.get_state_topic(),
            unique_id: self.config.client_id.clone(),
        };

        Message {
            topic: format!(
                "homeassistant/binary_sensor/{}_state/config",
                self.config.client_id
            ),
            value: match serde_json::to_string(&descriptor) {
                Ok(result) => result,
                Err(error) => {
                    error!("Cannot build Home Assistant registration message: {error}");
                    Default::default()
                }
            },
        }
    }
}

impl Drop for Daemon {
    fn drop(&mut self) {
        self.stop.store(true, Ordering::Relaxed)
    }
}
