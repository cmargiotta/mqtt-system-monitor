local open = io.open

local file = open("/proc/meminfo", "r") 
local free = string.match(file:read(), "%d+") --second line

local memory = free/1000000

sensor.value = string.format("%.2f", memory)
sensor.debug_message = ""
sensor.name = "Available memory"
sensor.id = "available_mem"
sensor.unit = "GB"
sensor.class = "None"