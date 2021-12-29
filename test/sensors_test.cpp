#include <string>
#include <catch2/catch.hpp>

#include "sensor/lua_sensor.hpp"
#include "sensor/string_file_sensor.hpp"
#include "sensor/binary_file_sensor.hpp"

using std::string;
using msm::lua_sensor;
using msm::string_file_sensor;
using msm::binary_file_sensor;

SCENARIO("Sensor interaction with LUA scripts")
{
	GIVEN("A LUA script producing sensor values")
	{
		const string script("sensor.value = \"15\"\nsensor.debug_message = \"test\"");
		lua_sensor sensor (script); 

		WHEN("A new value is requested")
		{
			auto value = sensor.get_value();

			THEN("It is correct")
			{
				REQUIRE(value == "15");
			}
		}

		WHEN("The debug message is requested")
		{
			sensor.get_value(); //Forcing update, this instance is different than the previous test
			auto debug_message = sensor.get_debug_message();

			THEN("It is correct")
			{
				REQUIRE(debug_message == "test");
			}
		}
	}
}

SCENARIO("Sensor interaction with human readable files")
{
	GIVEN("A file with sensor data on it")
	{
		const string path("fake_string_sensor");
		string_file_sensor sensor(path);

		WHEN("A new value is requested")
		{
			auto value = sensor.get_value();

			THEN("It is correct")
			{
				REQUIRE(value == "1234");
			}
		}

		WHEN("The debug message is requested")
		{
			sensor.get_value(); //Forcing update, this instance is different than the previous test
			auto debug_message = sensor.get_debug_message();

			THEN("It is empty")
			{
				REQUIRE(debug_message == "");
			}
		}
	}

	GIVEN("A wrong sensor file path")
	{
		const string path("wrong_path");

		WHEN("The sensor manager is initialized for it")
		{
			THEN("An exception is thrown")
			{
				REQUIRE_THROWS(new string_file_sensor(path));
			}
		}
	}
}

SCENARIO("Sensor interaction with human binary files")
{
	GIVEN("A file with sensor data on it")
	{
		const string path("fake_binary_sensor");
		binary_file_sensor<uint8_t> sensor(path);

		WHEN("A new value is requested")
		{
			auto value = sensor.get_value();

			THEN("It is correct")
			{
				REQUIRE(value == "255");
			}
		}

		WHEN("The debug message is requested")
		{
			sensor.get_value(); //Forcing update, this instance is different than the previous test
			auto debug_message = sensor.get_debug_message();

			THEN("It is empty")
			{
				REQUIRE(debug_message == "");
			}
		}
	}

	GIVEN("A wrong sensor file path")
	{
		const string path("wrong_path");

		WHEN("The sensor manager is initialized for it")
		{
			THEN("An exception is thrown")
			{
				REQUIRE_THROWS(new binary_file_sensor<uint8_t>(path));
			}
		}
	}
}