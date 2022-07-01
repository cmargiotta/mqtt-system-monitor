#ifndef MQTT_MQTT
#define MQTT_MQTT

#include <chrono>
#include <string>
#include <unordered_map>

#include <mqtt/async_client.h>

namespace msm
{
    class mqtt_wrapper
    {
        private:
            ::mqtt::async_client client_;

            std::unordered_map<std::string, ::mqtt::topic> topics_cache;

            static constexpr auto PERIOD            = std::chrono::seconds(5);
            static constexpr int  MAX_BUFFERED_MSGS = 120;// 120 * 5sec => 10min off-line buffering
            static constexpr int  DEFAULT_QOS       = 0;

        public:
            mqtt_wrapper(const std::string& address, const std::string& client_id);

            void publish(const std::string& topic, const std::string& message);
    };
}// namespace msm

#endif /* MQTT_MQTT */
