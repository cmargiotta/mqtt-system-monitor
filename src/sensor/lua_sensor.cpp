#include "lua_sensor.hpp"

#include <fstream>

using std::string; 
using std::ifstream;
using msm::lua_sensor;

lua_sensor::lua_sensor(const string& path):
	sensor(path.substr(path.find_last_of("/") + 1, path.find_last_of(".")))
{
	ifstream script_file (path);
	std::stringstream script_input;
	script_input << script_file.rdbuf();
	script += script_input.str(); 

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