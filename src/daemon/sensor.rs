pub mod home_assistant;

use mlua::{Lua, Result, UserData};
use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Default, Clone, Debug)]
struct SensorData {
    name: String,
    id: String,
    class: String,
    unit: String,

    value: String,
    debug_message: String,
}

impl UserData for SensorData {}

pub struct Sensor {
    data: SensorData,
    script: Box<dyn Fn() -> Result<SensorData>>,
    topic: String
}

impl Sensor {
    pub fn new(code: String, prefix: &String, client_id: &String) -> Sensor {
        let lua = Lua::new();

        lua.globals()
            .set(
                "sensor",
                lua.create_table().expect("This lua instruction must work"),
            )
            .expect("This lua instruction must work");

        let script = Box::new(move || {
            lua.load(&code).exec().expect("Runtime error in sensor");

            let sensors: mlua::Table = lua.globals().get("sensor")?;

            Ok(SensorData {
                name: sensors.get("name")?,
                id: sensors.get("id")?,
                class: sensors.get("class")?,
                unit: sensors.get("unit")?,

                value: sensors.get("value")?,
                debug_message: sensors.get("debug_message")?,
            })
        }) as Box<dyn Fn() -> Result<SensorData>>;

        Sensor {
            data: Default::default(),
            script,
            topic: format!("{prefix}/{client_id}/{self.data.class}/{self.data.id}")
        }
    }

    pub fn exec(self: &mut Sensor) -> () {
        self.data = match (self.script)() {
            Ok(data) => data,
            Err(_) => Default::default(),
        };
    }

    pub fn get_homeassistant_descriptor(self: &Sensor, client_id: &String) -> String {
        let mut name = client_id.clone();
        name.push(' ');
        name.push_str(&self.data.name);

        let mut state_topic =

        let descriptor = home_assistant::Descriptor {
            name,
            class: self.data.class,
            device: home_assistant::DeviceDescriptor {
                name: self.data.name,
                model: *client_id,
                identifiers: *client_id,
            },
            expire_after: 0,
            state_topic: "",
            unique_id: "",
        };
    }
}
