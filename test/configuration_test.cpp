#include <string>
#include <vector>
#include <catch2/catch.hpp>

#include "configuration/configuration.hpp"

using std::string;
using std::vector;
using msm::configuration; 
using Catch::Matchers::UnorderedEquals;

SCENARIO("config.yml loading")
{
	GIVEN("A yaml config")
	{
		const string path("config_test.yml");

		WHEN("The config file is parsed")
		{
			configuration config(path);

			THEN("Data is correctly read")
			{
				REQUIRE_THAT(config->lua_sensors, 
								UnorderedEquals(vector<string>{"lua1", "lua2"}));
				
				REQUIRE_THAT(config->string_file_sensors, 
								UnorderedEquals(vector<string>{"sensor1", "sensor2"}));

				REQUIRE_THAT(config->binary_file_sensors, 
								UnorderedEquals(vector<string>{"bin1", "bin2"}));

				REQUIRE(config->mqtt_broker == "localhost");
				REQUIRE(config->update_period == 60);
			}
		}
	}

	GIVEN("A yaml config without a broker set")
	{
		const string path("config_test_no_broker.yml");

		WHEN("The config file is parsed")
		{
			THEN("An exception is thrown")
			{
				REQUIRE_THROWS(new configuration(path)); 
			}
		}
	}
}