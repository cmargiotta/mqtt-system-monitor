mqtt_wrapper_lib = static_library(
	'mqtt_wrapper', 
	[
		'mqtt_wrapper.cpp'
	],
	dependencies: [paho_mqtt]
)

mqtt_wrapper = declare_dependency(
	link_with: mqtt_wrapper_lib,
	include_directories: ['..'],
	dependencies: [paho_mqtt]
)