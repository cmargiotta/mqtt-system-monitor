#ifndef SENSOR_STRING_FILE_SENSOR
#define SENSOR_STRING_FILE_SENSOR

#include "sensor.hpp"

#include <string>
#include <fstream>
#include <type_traits>

namespace msm
{
	class string_file_sensor final: public sensor
	{
		private:
			std::ifstream data;
			std::string last_value;

		public:
			explicit string_file_sensor(const std::string& path);
			~string_file_sensor() = default; 

			const std::string& get_value() override;
	};
}

#endif /* SENSOR_STRING_FILE_SENSOR */
