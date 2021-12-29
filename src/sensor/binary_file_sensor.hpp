#ifndef SENSOR_FILE_SENSOR
#define SENSOR_FILE_SENSOR

#include "sensor.hpp"

#include <string>
#include <fstream>
#include <exception>
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
				sensor(path.substr(path.find_last_of("/") + 1)),
				data(path, std::ifstream::in | std::ifstream::binary)
			{
				if (data.fail())
				{
					throw std::runtime_error("Invalid binary file sensor path.");
				}
			}
			
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
