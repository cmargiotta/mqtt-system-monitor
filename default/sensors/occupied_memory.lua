local open = io.open

local file = open("/proc/meminfo", "r") 
local total = string.match(file:read(), "%d+") --first line
local free = string.match(file:read(), "%d+") --second line

local memory = (total - free)/1000000

sensor.value = string.format("%.2f", memory)
sensor.debug_message = ""
sensor.name = "Occupied memory"
sensor.id = "occupied_mem"
sensor.unit = "GB"
sensor.class = "None"