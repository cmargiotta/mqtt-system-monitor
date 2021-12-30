local open = io.open

local file = open("/proc/meminfo", "r") 
local total = string.match(file:read(), "%d+") --first line
local free = string.match(file:read(), "%d+") --second line

local usage = 100 - (100*free/total)

sensor.value = string.format("%.2f", usage)
sensor.debug_message = ""
sensor.name = "Memory usage"
sensor.id = "mem_usage"
sensor.unit = "%"
sensor.class = "None"