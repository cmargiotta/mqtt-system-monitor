local open = io.open

local file = open("/proc/meminfo", "r") 
local total = string.match(file:read(), "%d+") --first line
file:read()
local avail = string.match(file:read(), "%d+") --third line

local memory = (total - avail)/1000000

sensor.value = string.format("%.2f", memory)
sensor.debug_message = ""
sensor.name = "Occupied memory"
sensor.id = "occupied_mem"
sensor.unit = "GB"
sensor.class = "None"