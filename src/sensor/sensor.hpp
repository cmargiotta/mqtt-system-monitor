#ifndef SENSOR_SENSOR_HPP
#define SENSOR_SENSOR_HPP

#include <string>

namespace msm
{
	class sensor
	{
		private:
			std::string name; 

		public:
			sensor(const std::string& name);
			virtual ~sensor() = default;

			const std::string& get_name();

			virtual const std::string& get_value() = 0;
			virtual const std::string& get_debug_message(); 
	};
}

#endif /* SENSOR_SENSOR_HPP */
