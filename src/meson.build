subdir('mqtt_wrapper')
subdir('sensor')
subdir('configuration')

subdir('daemon')

executable('msm', 
			dependencies: [daemon], 
			sources: ['main.cpp'], 
			install: true, 
			install_dir: '/usr/bin/')