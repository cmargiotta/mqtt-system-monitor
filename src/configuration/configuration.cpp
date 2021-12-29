#include "configuration.hpp"

#include <iostream>
#include <exception>
#include <yaml-cpp/yaml.h>

using std::cout;
using std::string;
using std::vector;
using msm::configuration; 

configuration::configuration(const string& path)
{
	YAML::Node yaml = YAML::LoadFile(path);

	cout << "Loading config file.\n";

	auto lua_sensors = yaml["lua-sensors"];
	auto sensor_files = yaml["sensor-files"];
	auto binary_sensor_files = yaml["binary-sensor-files"];

	auto period = yaml["update-period"];
	auto broker = yaml["mqtt-broker"];

	if (!broker)
	{
		throw std::runtime_error("MQTT broker setting is required.");
	}
	
	data.mqtt_broker = broker.as<string>(); 

	if (lua_sensors)
	{
		cout << "Lua sensor files setting found.\n";

		data.lua_sensors = lua_sensors.as<vector<string>>();
	}
	else
	{
		cout << "No lua sensor files set.\n";
	}

	if (sensor_files)
	{
		cout << "Sensor files setting found.\n";

		data.string_file_sensors = sensor_files.as<vector<string>>();
	}
	else
	{
		cout << "No sensor files set.\n";
	}

	if (binary_sensor_files)
	{
		cout << "Binary sensor files setting found.\n";

		data.binary_file_sensors = binary_sensor_files.as<vector<string>>();
	}
	else
	{
		cout << "No binary sensor files set.\n";
	}

	if (period)
	{
		data.update_period = period.as<decltype(data.update_period)>(); 
	}
}

const configuration::data_* configuration::operator->()
{
	return &data;
}