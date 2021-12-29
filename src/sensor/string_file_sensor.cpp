#include "string_file_sensor.hpp"

#include <exception>

using std::string;
using msm::string_file_sensor;

string_file_sensor::string_file_sensor(const string& path):
	data(path)
{
	if (data.fail())
	{
		throw std::runtime_error("Invalid string file sensor path.");
	}
}

const string& string_file_sensor::get_value()
{
	last_value.clear();
	data >> last_value;

	return last_value;
}