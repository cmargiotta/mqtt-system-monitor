mod configuration;
mod sensor;

use configuration::Configuration;
use rumqttc::{Client, MqttOptions, QoS};
use sensor::Sensor;

const SENSOR_PATH: &'static str = "/etc/msm/sensors/";

pub struct Daemon {
    config: Configuration,
    mqtt: Client,

    sensors: Vec<Sensor>,
}

impl Daemon {
    pub fn new(path: &String) -> Result<Daemon, serde_yaml::Error> {
        let config = Configuration::load(&path)?;
        let mqtt_config = MqttOptions::new(
            &config.client_id,
            &config.mqtt_broker,
            config.mqtt_broker_port,
        );

        let (client, _) = Client::new(mqtt_config, 1);

        let mut sensors: Vec<Sensor> = Default::default();

        for sensor in &config.sensors {
            println!("Loading sensor {sensor}");
            let script = match std::fs::read_to_string(String::from(SENSOR_PATH) + sensor + ".lua")
            {
                Ok(script) => script,
                Err(error) => panic!("{error}"),
            };
            sensors.push(Sensor::new(script));
        }

        Ok(Daemon {
            config,
            mqtt: client,
            sensors,
        })
    }

    pub fn run(self: &Daemon) {
        loop {
            for sensor in &self.sensors {}
        }
    }
}
