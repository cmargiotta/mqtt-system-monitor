mod daemon;

fn main() {
    let daemon = daemon::Daemon::new(&String::from("a.yml"));
}
