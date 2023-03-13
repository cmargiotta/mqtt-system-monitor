pub mod home_assistant;
pub mod message;

use log::*;
use mlua::{Lua, Result, UserData};
use serde::Serialize;

use self::home_assistant::DEFAULT_EXPIRE_AFTER;

#[derive(Serialize, Default, Clone, Debug)]
struct SensorData {
    name: String,
    id: String,
    class: String,
    unit: Option<String>,

    value: String,
    debug_message: String,
}

impl UserData for SensorData {}

pub struct Sensor {
    data: SensorData,
    script: Box<dyn Fn() -> Result<SensorData>>,
    prefix: String,
    client_id: String,
}

impl Sensor {
    pub fn new(name: String, code: String, prefix: &String, client_id: &String) -> Sensor {
        let lua = Lua::new();

        lua.globals()
            .set(
                "sensor",
                lua.create_table().expect("This lua instruction must work"),
            )
            .expect("This lua instruction must work");

        let script = Box::new(move || {
            match lua.load(&code).exec() {
                Err(error) => error!("Runtime error in sensor {name}: {error}"),
                _ => {}
            }

            let sensors: mlua::Table = lua.globals().get("sensor")?;

            Ok(SensorData {
                name: sensors.get("name")?,
                id: sensors.get("id")?,
                class: sensors.get("class")?,
                unit: sensors.get("unit").unwrap_or(Default::default()),

                value: sensors.get("value")?,
                debug_message: sensors.get("debug_message")?,
            })
        }) as Box<dyn Fn() -> Result<SensorData>>;

        Sensor {
            data: Default::default(),
            script,
            prefix: prefix.clone(),
            client_id: client_id.clone(),
        }
    }

    fn get_topic(self: &Sensor) -> String {
        format!(
            "{}/{}/{}/{}",
            self.prefix, self.client_id, self.data.class, self.data.id
        )
    }

    pub fn exec(self: &mut Sensor) -> () {
        self.data = match (self.script)() {
            Ok(data) => data,
            Err(_) => Default::default(),
        };
    }

    pub fn get_homeassistant_descriptor(self: &Sensor) -> message::Message {
        let mut name = self.client_id.clone();
        name.push(' ');
        name.push_str(&self.data.name);

        let descriptor = home_assistant::Descriptor {
            name,
            class: self.data.class.clone(),
            unit_of_measurement: self.data.unit.clone(),
            device: home_assistant::DeviceDescriptor {
                name: self.data.name.clone(),
                model: self.client_id.clone(),
                identifiers: self.client_id.clone(),
            },
            expire_after: DEFAULT_EXPIRE_AFTER,
            state_topic: self.get_topic(),
            unique_id: format!("{}_{}", self.client_id, self.data.id),
        };

        message::Message {
            topic: format!(
                "homeassistant/sensor/{}_{}/config",
                self.client_id, self.data.id
            ),
            value: match serde_json::to_string(&descriptor) {
                Ok(value) => value,
                Err(error) => panic!("Cannot build home assistant sensor descriptor: {error}"),
            },
        }
    }

    pub fn get_message(self: &Sensor) -> message::Message {
        message::Message {
            topic: self.get_topic(),
            value: self.data.value.clone(),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn lua_sensor() {
        let code = String::from(
            r#"
                sensor.value = "test"
                sensor.debug_message = ""
                sensor.name = "Total memory"
                sensor.id = "total_mem"
                sensor.unit = "GB"
                sensor.class = "None"
            "#,
        );

        let mut sensor = Sensor::new(
            String::from("test"),
            code,
            &String::from("prefix"),
            &String::from("id"),
        );

        sensor.exec();

        let message = sensor.get_message();

        assert_eq!("prefix/id/None/total_mem", message.topic);
        assert_eq!("test", message.value);
    }
}
