#include "daemon.hpp"

#include <thread>
#include <fstream>
#include <iostream>

#include "sensor/lua_sensor.hpp"
#include "sensor/binary_file_sensor.hpp"
#include "sensor/string_file_sensor.hpp"

using std::cout;
using std::string;
using msm::daemon;
using std::ifstream;

daemon::daemon(const string& path):
	config(path),
	mqtt(config->mqtt_broker, config->client_id),
	update_period(config->update_period)
{
	sensors.reserve(config->string_file_sensors.size() + 
					config->binary_file_sensors.size() +
					config->lua_sensors.size());

	for (const auto& sensor: config->lua_sensors)
	{
		sensors.push_back(std::make_unique<lua_sensor>(sensor));
	}

	for (const auto& sensor: config->string_file_sensors)
	{
		sensors.push_back(std::make_unique<string_file_sensor>(sensor));
	}

	//Publishing ON state
	auto topic = config->client_id + "/state";
	mqtt.publish(topic, "ON");
}

void daemon::notify_off_state()
{
	cout << "Goodbye\n";
	auto topic = config->client_id + "/state";
	mqtt.publish(topic, "OFF");
}

daemon::~daemon()
{
	notify_off_state();
}

void daemon::run()
{
	while (true)
	{
		for (auto& sensor: sensors)
		{
			mqtt.publish(config->client_id + "/" + sensor->get_name(), sensor->get_value()); 

			if (!sensor->get_debug_message().empty())
			{
				auto debug_topic = config->client_id + "/" + sensor->get_name() + "-debug";
				mqtt.publish(debug_topic, sensor->get_debug_message());
			}
		}

		std::this_thread::sleep_for(update_period);
	}
}