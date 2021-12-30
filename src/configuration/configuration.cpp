#include "configuration.hpp"

#include <array>
#include <unistd.h>
#include <limits.h>
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

	auto lua_sensors = yaml["sensors"];

	auto period = yaml["update-period"];
	auto broker = yaml["mqtt-broker"];
	auto client_id = yaml["client-id"];
	auto prefix = yaml["mqtt-prefix"];
	auto homeassistant = yaml["homeassistant"];

	if (!broker)
	{
		throw std::runtime_error("MQTT broker setting is required.");
	}
	
	data.mqtt_broker = broker.as<string>(); 

	if (lua_sensors)
	{
		data.sensors = lua_sensors.as<vector<string>>();
	}

	if (period)
	{
		data.update_period = period.as<decltype(data.update_period)>(); 
	}

	if (prefix)
	{
		data.prefix = prefix.as<string>(); 
	}

	if (homeassistant)
	{
		data.homeassistant = homeassistant.as<bool>(); 
	}

	if (client_id && !client_id.as<string>().empty())
	{
		data.client_id = client_id.as<string>(); 
	}
	else
	{
		std::array<char, HOST_NAME_MAX> hostname;
		gethostname(hostname.data(), HOST_NAME_MAX);
		data.client_id = string(hostname.data());
		
		cout << "Client id not found, setting to: " << data.client_id << ".\n";
	}
}

const configuration::data_* configuration::operator->()
{
	return &data;
}