configuration_lib = static_library(
	'configuration', 
	[
		'configuration.cpp'
	],
	dependencies: [yaml]
)

configuration = declare_dependency(
	link_with: configuration_lib,
	include_directories: ['..']
)