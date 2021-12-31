local open = io.open

local file = open("/proc/meminfo", "r") 
local total = string.match(file:read(), "%d+") --first line

local memory = total/1000000

sensor.value = string.format("%.2f", memory)
sensor.debug_message = ""
sensor.name = "Total emory"
sensor.id = "total_mem"
sensor.unit = "GB"
sensor.class = "None"