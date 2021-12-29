test_main = static_library('test_main', sources: ['catch_main.cpp'], dependencies: [catch2])

sensors_test = executable(
	'sensors_test', 
	'sensors_test.cpp', 
	link_with: test_main, 
	dependencies: [catch2, sensor]
)

configuration_test = executable(
	'configuration_test', 
	'configuration_test.cpp', 
	link_with: test_main, 
	dependencies: [catch2, configuration]
)

test(
	'Sensors manager test', 
	sensors_test,
	workdir: meson.current_source_dir()
)

test(
	'Config manager test', 
	configuration_test,
	workdir: meson.current_source_dir()
)