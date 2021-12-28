#include "string_file_sensor.hpp"

using std::string;
using msm::string_file_sensor;

string_file_sensor::string_file_sensor(const string& path):
	data(path)
{}

const string& string_file_sensor::get_value()
{
	last_value.clear();
	data >> last_value;

	return last_value;
}