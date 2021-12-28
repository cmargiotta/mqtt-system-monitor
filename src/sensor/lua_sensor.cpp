#include "lua_sensor.hpp"

using std::string; 
using msm::lua_sensor;

lua_sensor::lua_sensor(const string& code):
	script{string("sensor = sensor_.new()\n") + code}
{
	lua.new_usertype<sensor_>("sensor_", "value", &sensor_::value, "debug_message", &sensor_::debug_message);
}

const string& lua_sensor::get_value()
{
	lua.script(script);

	last_data = lua.get<sensor_>("sensor");

	return last_data.value;
}

const string& lua_sensor::get_debug_message()
{
	return last_data.debug_message;
}