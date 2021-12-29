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
				std::vector<std::string> string_file_sensors;
				std::vector<std::string> binary_file_sensors;
				std::vector<std::string> lua_sensors;

				unsigned update_period = 30; 
				std::string mqtt_broker; 
			};

			data_ data;

		public:
			explicit configuration(const std::string& path); 
			~configuration() = default; 

			const data_* operator->();
	};
}

#endif /* CONFIGURATION_CONFIGURATION */
