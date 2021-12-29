sensor_lib = static_library(
	'sensor', 
	[
		'sensor.cpp'
	],
	dependencies: [sol2]
)

sensor = declare_dependency(
	link_with: sensor_lib,
	include_directories: ['..'],
	dependencies: [sol2]
)