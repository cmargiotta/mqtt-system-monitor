sensor_lib = static_library(
	'sensor', 
	[
		'sensor.cpp',
		'lua_sensor.cpp',
		'string_file_sensor.cpp'
	],
	dependencies: [sol2]
)

sensor = declare_dependency(
	link_with: sensor_lib,
	include_directories: ['..'],
	dependencies: [sol2]
)