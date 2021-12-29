#include "sensor.hpp"

#include <fstream>

using msm::sensor;
using std::string; 
using std::ifstream;

sensor::sensor(const string& path)
{
	ifstream script_file (path);
	std::stringstream script_input;
	script_input << script_file.rdbuf();
	script += script_input.str(); 

	lua.new_usertype<sensor_>("sensor_", 
								"value", &sensor_::value, 
								"debug_message", &sensor_::debug_message,
								"class", &sensor_::class_,
								"name", &sensor_::name,
								"id", &sensor_::id,
								"unit", &sensor_::unit);

	lua.open_libraries(sol::lib::base, sol::lib::io);
}

sensor::sensor_ sensor::get_data()
{
	lua.script(script);

	return lua.get<sensor_>("sensor");;
}