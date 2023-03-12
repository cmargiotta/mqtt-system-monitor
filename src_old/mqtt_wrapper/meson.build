mqtt_wrapper_lib = static_library(
	'mqtt_wrapper',
	[
		'mqtt_wrapper.cpp'
	],
	dependencies: [emqtt5]
)

mqtt_wrapper = declare_dependency(
	link_with: mqtt_wrapper_lib,
	include_directories: ['..'],
	dependencies: [emqtt5]
)