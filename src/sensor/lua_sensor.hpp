#ifndef SENSOR_LUA_SENSOR
#define SENSOR_LUA_SENSOR

#include "sensor.hpp"

#include <string>
#include <sol/sol.hpp>

namespace msm
{
	class lua_sensor final: public sensor
	{
		private:
			struct sensor_
			{
				std::string value; 
				std::string debug_message;
			};

			std::string script; 
			sol::state lua;

			sensor_ last_data;

		public:
			explicit lua_sensor(const std::string& code);
			~lua_sensor() = default; 

			const std::string& get_value() override;
			const std::string& get_debug_message() override;
	};
}

#endif /* SENSOR_LUA_SENSOR */
