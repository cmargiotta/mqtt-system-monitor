local open = io.open

local file = open("/sys/class/hwmon/hwmon4/temp1_input", "r") 
local content = file:read() --first line

file:close()

local temp = tonumber(content)/1000
	
sensor.value = tostring(temp)
sensor.debug_message = "WARNING: check if the right hwmon is loaded"
sensor.name = "CPU Temperature"
sensor.id = "cpu_temp"
sensor.unit = "°C"
sensor.class = "temperature"