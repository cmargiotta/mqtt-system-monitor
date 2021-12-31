local open = io.open

local file = open("/proc/meminfo", "r") 
file:read()
file:read()
local avail = string.match(file:read(), "%d+") --third line

local memory = avail/1000000

sensor.value = string.format("%.2f", memory)
sensor.debug_message = ""
sensor.name = "Available memory"
sensor.id = "available_mem"
sensor.unit = "GB"
sensor.class = "None"