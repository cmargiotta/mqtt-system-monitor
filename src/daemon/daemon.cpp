#include "daemon.hpp"

#include <fstream>
#include <iostream>
#include <thread>

#include "sensor/sensor.hpp"

using msm::daemon;
using std::cout;
using std::ifstream;
using std::string;

daemon::daemon(const string& path)
    : config(path),
      mqtt(config->mqtt_broker, config->client_id),
      prefix(config->prefix + "/" + config->client_id),
      update_period(config->update_period)
{
    sensors.reserve(config->sensors.size());

    for (const string& s: config->sensors)
    {
        string path("/etc/msm/sensors/");

        if (s[0] == '/')
        {
            // Absolute path given
            sensors.push_back(std::make_unique<sensor>(s));
        }
        else
        {
            // Default sensor given
            if (s.substr(s.size() - 4) != ".lua")
            {
                // Extension needed
                sensors.push_back(std::make_unique<sensor>(path + s + ".lua"));
            }
            else
            {
                sensors.push_back(std::make_unique<sensor>(path + s));
            }
        }
    }

    if (config->homeassistant)
    {
        homeassistant_register();
    }

    // Publishing ON state
    notify_state(true);
}

void daemon::homeassistant_register_state()
{
    std::stringstream json, topic;

    topic << "homeassistant/binary_sensor/" << config->client_id << "_state/config";

    json << "{\"name\": \"" << config->client_id << " State\","
         << "\"class\": \"connectivity\","
         << "\"device\": {\"name\": \"" << config->client_id << "\", "
         << "\"model\": \"" << config->client_id << "\", "
         << "\"identifiers\": \"" << config->client_id << "\"}, "
         << "\"expire_after\": " << HOME_ASSISTANT_EXPIRE_AFTER << ", "
         << "\"state_topic\": \"" << prefix << "/state\","
         << "\"unique_id\": \"" << config->client_id << "_state\"}";

    mqtt.publish(topic.str(), json.str());
}

void daemon::homeassistant_register_sensor(msm::sensor::sensor_& data)
{
    std::stringstream json, topic;

    topic << "homeassistant/sensor/" << config->client_id << '_' << data.id << "/config";

    json << "{\"name\": \"" << config->client_id << ' ' << data.name << "\", "
         << "\"class\": \"" << data.class_ << "\",";

    if (data.unit.size() != 0)
    {
        json << "\"unit_of_measurement\": \"" << data.unit << "\",";
    }

    json << "\"device\": {\"name\": \"" << config->client_id << "\", "
         << "\"model\": \"" << config->client_id << "\", "
         << "\"identifiers\": \"" << config->client_id << "\"}, "
         << "\"expire_after\": " << HOME_ASSISTANT_EXPIRE_AFTER << ", "
         << "\"state_topic\": \"" << prefix << '/' << data.class_ << '/' << data.id << "\","
         << "\"unique_id\": \"" << config->client_id << '_' << data.id << "\"}";

    mqtt.publish(topic.str(), json.str());
}

void daemon::homeassistant_register()
{
    homeassistant_register_state();

    for (auto& sensor: sensors)
    {
        auto data = sensor->get_data();

        homeassistant_register_sensor(data);
    }
}

void daemon::notify_state(bool state)
{
    auto topic = prefix + "/state";
    mqtt.publish(topic, state ? "ON" : "OFF");
}

daemon::~daemon()
{
    notify_state(false);
}

void daemon::run()
{
    unsigned cycle_count              = 0;
    bool     homeassistant_reregister = false;

    while (true)
    {
        notify_state(true);

        if (cycle_count >= config->update_period / HOME_ASSISTANT_EXPIRE_AFTER && config->homeassistant)
        {
            homeassistant_register_state();

            homeassistant_reregister = true;
            cycle_count              = 0;
        }

        for (auto& sensor: sensors)
        {
            auto data = sensor->get_data();
            mqtt.publish(prefix + "/" + data.class_ + "/" + data.id, data.value);

            if (!data.debug_message.empty())
            {
                auto debug_topic = prefix + "/" + data.class_ + "/" + data.id + "-debug";
                mqtt.publish(debug_topic, data.debug_message);
            }

            if (homeassistant_reregister)
            {
                homeassistant_register_sensor(data);
            }
        }

        if (homeassistant_reregister)
        {
            homeassistant_reregister = false;
        }

        std::this_thread::sleep_for(update_period);

        cycle_count++;
    }
}