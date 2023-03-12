use serde::Serialize;

pub const DEFAULT_EXPIRE_AFTER: u64 = 60;

#[derive(Serialize)]
pub struct DeviceDescriptor {
    pub name: String,
    pub model: String,
    pub identifiers: String,
}

#[derive(Serialize)]
pub struct Descriptor {
    pub name: String,
    pub class: String,
    pub unit_of_measurement: Option<String>,
    pub device: DeviceDescriptor,
    pub expire_after: u64,
    pub state_topic: String,
    pub unique_id: String,
}
