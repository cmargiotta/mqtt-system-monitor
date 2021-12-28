#include "sensor.hpp"

using msm::sensor;
using std::string; 

const string& sensor::get_debug_message()
{
	static const string empty_message;

	return empty_message;
}