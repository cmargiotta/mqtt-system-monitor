#include <string>
#include <catch2/catch.hpp>

#include "sensor/sensor.hpp"

using std::string;
using msm::sensor;

SCENARIO("Sensor interaction with LUA scripts")
{
	GIVEN("A LUA script producing sensor values")
	{
		const string path("sensor.lua");
		sensor sensor (path); 

		WHEN("A new value is requested")
		{
			auto data = sensor.get_data();

			THEN("The produced data is correct correct")
			{
				REQUIRE(data.class_ == "None");
				REQUIRE(data.debug_message == "test");
				REQUIRE(data.name == "Test sensor");
				REQUIRE(data.value == "15");
			}
		}
	}
}