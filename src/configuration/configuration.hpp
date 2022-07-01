#ifndef CONFIGURATION_CONFIGURATION
#define CONFIGURATION_CONFIGURATION

#include <string>
#include <vector>

namespace msm
{
    class configuration
    {
        private:
            struct data_
            {
                    std::vector<std::string> sensors;

                    unsigned update_period = 10;
                    bool     homeassistant = false;

                    std::string mqtt_broker;
                    std::string client_id;
                    std::string prefix = "mqtt-system-monitor";
            };

            data_ data;

        public:
            explicit configuration(const std::string& path);
            ~configuration() = default;

            const data_* operator->();
    };
}// namespace msm

#endif /* CONFIGURATION_CONFIGURATION */
