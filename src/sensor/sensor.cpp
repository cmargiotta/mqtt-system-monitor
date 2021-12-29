#include "sensor.hpp"

using msm::sensor;
using std::string; 

sensor::sensor(const string& name_):
	name(name_)
{}

const string& sensor::get_debug_message()
{
	static const string empty_message;

	return empty_message;
}

const string& sensor::get_name()
{
	return name; 
}