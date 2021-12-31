local open = io.open

local file = open("/proc/meminfo", "r") 
local total = string.match(file:read(), "%d+") --first line
file:read()
local avail = string.match(file:read(), "%d+") --third line

local usage = 100 - (100*avail/total)

sensor.value = string.format("%.2f", usage)
sensor.debug_message = ""
sensor.name = "Memory usage"
sensor.id = "mem_usage"
sensor.unit = "%"
sensor.class = "None"