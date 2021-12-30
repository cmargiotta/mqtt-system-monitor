project('mqtt_system_monitor', 'cpp', default_options: ['cpp_std=c++17'])

cmake = import('cmake')
cpp = meson.get_compiler('cpp')

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

install_data('msm.service', install_dir: '/usr/lib/systemd/system/')
install_subdir('default/sensors/', install_dir: '/etc/msm/')