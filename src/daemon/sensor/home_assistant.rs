use serde::Serialize;

pub const DEFAULT_EXPIRE_AFTER: u32 = 60;

#[derive(Serialize)]
pub struct DeviceDescriptor {
    name: String,
    model: String,
    identifiers: String,
}

#[derive(Serialize)]
pub struct Descriptor {
    name: String,
    class: String,
    device: DeviceDescriptor,
    expire_after: u32,
    state_topic: String,
    unique_id: String,
}

pub fn build_json(data: &Descriptor) -> Result<String, serde_json::Error> {
    serde_json::to_string(&data)
}
