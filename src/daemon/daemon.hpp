#ifndef DAEMON_DAEMON
#define DAEMON_DAEMON

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "configuration/configuration.hpp"
#include "mqtt_wrapper/mqtt_wrapper.hpp"
#include "sensor/sensor.hpp"

namespace msm
{
    class daemon
    {
        private:
            configuration                        config;
            msm::mqtt_wrapper                    mqtt;
            std::vector<std::unique_ptr<sensor>> sensors;

            std::string prefix;

            std::chrono::duration<size_t> update_period;

            void homeassistant_register_sensor(sensor::sensor_& data);
            void homeassistant_register_state();
            void homeassistant_register();

            static constexpr int HOME_ASSISTANT_EXPIRE_AFTER = 60;

        public:
            explicit daemon(const std::string& config_path);
            ~daemon();

            void run();
            void notify_state(bool state = true);
    };
}// namespace msm

#endif /* DAEMON_DAEMON */
