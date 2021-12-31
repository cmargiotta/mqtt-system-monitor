project('mqtt_system_monitor', 'cpp', default_options: ['cpp_std=c++17'])

cmake = import('cmake')
cpp = meson.get_compiler('cpp')

# Subprojects options
paho_mqtt_options = cmake.subproject_options()
paho_mqtt_options.add_cmake_defines({'PAHO_BUILD_STATIC': true})

# Cmake subprojects
yaml_pro = cmake.subproject('yaml_cpp')
paho_mqtt_pro = cmake.subproject('paho_mqtt', options: paho_mqtt_options)

# Meson subprojects
sol2_pro = subproject('sol2')
catch2_pro = subproject('catch2')

# Dependencies
paho_mqtt = paho_mqtt_pro.dependency('paho-mqttpp3-static')
sol2 = sol2_pro.get_variable('sol2_dep')
catch2 = catch2_pro.get_variable('catch2_dep')
yaml = yaml_pro.dependency('yaml_cpp')

subdir('src')
subdir('test')

install_data('msm.service', install_dir: '/usr/lib/systemd/system/')
install_subdir('default/sensors/', install_dir: '/etc/msm/')