#ifndef SENSOR_FILE_SENSOR
#define SENSOR_FILE_SENSOR

#include "sensor.hpp"

#include <string>
#include <fstream>
#include <type_traits>

namespace msm
{
	template<typename Type>
	class binary_file_sensor final: public sensor
	{
		static_assert(std::is_trivial<Type>::value, "file sensor type must be trivial");

		private:
			std::ifstream data;
			std::string last_value;
			Type last_value_bin;

		public:
			explicit binary_file_sensor(const std::string& path):
				data(path, std::ifstream::in | std::ifstream::binary)
			{}
			~binary_file_sensor() = default; 

			const std::string& get_value() override
			{
				char* buffer = reinterpret_cast<char*>(&last_value_bin);
				data.read(buffer, sizeof(Type));

				last_value = std::to_string(last_value_bin);
				return last_value;
			}
	};
}

#endif /* SENSOR_FILE_SENSOR */
