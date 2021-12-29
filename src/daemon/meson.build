daemon_lib = static_library(
	'daemon', 
	[
		'daemon.cpp'
	],
	dependencies: [sensor, mqtt_wrapper, configuration]
)

daemon = declare_dependency(
	link_with: daemon_lib,
	include_directories: ['..'],
	dependencies: [mqtt_wrapper, sensor]
)