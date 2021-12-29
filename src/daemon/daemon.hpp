#ifndef DAEMON_DAEMON
#define DAEMON_DAEMON

#include <string>
#include <vector>
#include <memory>
#include <chrono>

#include "sensor/sensor.hpp"
#include "mqtt_wrapper/mqtt_wrapper.hpp"
#include "configuration/configuration.hpp"

namespace msm
{
	class daemon
	{
		private: 
			configuration config; 
			msm::mqtt_wrapper mqtt; 
			std::vector<std::unique_ptr<sensor>> sensors; 

			std::chrono::duration<size_t> update_period;

		public:
			explicit daemon(const std::string& config_path);
			~daemon();

			void run(); 
			void notify_off_state();
	};
}

#endif /* DAEMON_DAEMON */
