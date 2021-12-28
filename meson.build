project('mqtt_system_monitor', 'cpp', default_options: ['cpp_std=c++17'])

cmake = import('cmake')
cpp = meson.get_compiler('cpp')

# Paho MQTT prerequisite
message('Running Paho MQTT prerequisites')
paho_pre = run_command('/usr/bin/bash', 'subprojects/paho_mqtt/install_paho_mqtt_c.sh')
message(paho_pre.stdout().strip())
message('Errors:')
message(paho_pre.stderr().strip())

# Subprojects options
paho_mqtt_options = cmake.subproject_options()
paho_mqtt_options.add_cmake_defines({'PAHO_BUILD_STATIC': true})

# Cmake subprojects
paho_mqtt_pro = cmake.subproject('paho_mqtt', options: paho_mqtt_options)
yaml_pro = cmake.subproject('yaml_cpp')

# Meson subprojects
sol2_proj = subproject('sol2')
catch2_proj = subproject('catch2')

# Dependencies
paho_mqtt = paho_mqtt_pro.dependency('paho-mqttpp3-static')
yaml = yaml_pro.dependency('yaml_cpp')
sol2 = sol2_proj.get_variable('sol2_dep')
catch2 = catch2_proj.get_variable('catch2_dep')

subdir('src')
subdir('test')