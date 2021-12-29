subdir('mqtt_wrapper')
subdir('sensor')
subdir('configuration')

subdir('daemon')

executable('mqtt_system_monitor', 
			dependencies: [daemon], 
			sources: ['main.cpp'])