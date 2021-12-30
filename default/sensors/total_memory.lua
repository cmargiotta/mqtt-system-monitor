local open = io.open

local file = open("/proc/meminfo", "r") 
local total = string.match(file:read(), "%d+") --first line
local free = string.match(file:read(), "%d+") --second line

local usage = total/1000000

sensor.value = string.format("%.2f", total)
sensor.debug_message = ""
sensor.name = "Total emory"
sensor.id = "mem_usage"
sensor.unit = "GB"
sensor.class = "None"