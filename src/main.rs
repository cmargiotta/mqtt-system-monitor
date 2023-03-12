mod daemon;

use daemon::Daemon;

const DEFAULT_CONFIG_PATH: &str = "/etc/msm/config.yml";

#[tokio::main]
async fn main() {
    let args: Vec<String> = std::env::args().collect();

    let config_path = match args.get(1) {
        Some(path) => path.clone(),
        None => String::from(DEFAULT_CONFIG_PATH),
    };

    let config = daemon::configuration::Configuration::load(&config_path)
        .expect("Configuration is correctly loaded");

    stderrlog::new()
        .module(module_path!())
        .verbosity(config.log_verbosity)
        .init()
        .unwrap();

    let mut daemon = Daemon::new(config).expect("Daemon must be initialized");

    daemon.run().await
}
