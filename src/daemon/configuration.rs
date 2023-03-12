use gethostname::gethostname;
use serde::Deserialize;

#[derive(Deserialize)]
pub struct Configuration {
    #[serde(default = "Default::default")]
    pub sensors: Vec<String>,

    #[serde(default = "default_update_period")]
    pub update_period: u32,
    #[serde(default = "default_homeassistant")]
    pub homeassistant: bool,

    #[serde(rename = "mqtt-broker")]
    pub mqtt_broker: String,
    #[serde(rename = "mqtt-broker-port", default = "default_broker_port")]
    pub mqtt_broker_port: u16,
    #[serde(rename = "mqtt-user")]
    pub mqtt_user: Option<String>,
    #[serde(rename = "mqtt-password")]
    pub mqtt_password: Option<String>,
    #[serde(default = "default_client_id")]
    pub client_id: String,
    #[serde(default = "default_prefix")]
    pub prefix: String,
}

const fn default_update_period() -> u32 {
    10
}

const fn default_broker_port() -> u16 {
    1883
}

const fn default_homeassistant() -> bool {
    false
}

fn default_client_id() -> String {
    gethostname().into_string().expect("Cannot read hostname")
}

fn default_prefix() -> String {
    String::from("mqtt-system-monitor")
}

impl Configuration {
    pub fn load(path: &str) -> Result<Configuration, serde_yaml::Error> {
        let yaml: String = std::fs::read_to_string(&path)
            .expect("Should have been able to read the configuration");

        serde_yaml::from_str(&yaml)
    }
}
