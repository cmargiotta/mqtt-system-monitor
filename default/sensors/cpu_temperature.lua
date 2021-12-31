local open = io.open

local file = open("/sys/class/hwmon/hwmon4/temp1_input", "r") 

if file then 
    local content = file:read() --first line
    file:close()

    local temp = tonumber(content)/1000
	
    sensor.value = tostring(temp)
    sensor.debug_message = "WARNING: check if the right hwmon is loaded"
else
    sensor.value = "9999"
    sensor.debug_message = "ERROR: wrong hwmon loaded"
end

sensor.name = "CPU Temperature"
sensor.id = "cpu_temp"
sensor.unit = "°C"
sensor.class = "temperature"