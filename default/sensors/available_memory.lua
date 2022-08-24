local open = io.open
local file = open("/proc/meminfo", "r")
local total = string.match(file:read(), "%d+") --first line
local free = string.match(file:read(), "%d+") --second line
file:close()

local memory = free/1000000

sensor.value = string.format("%.1f", memory)
sensor.debug_message = ""
sensor.name = "Available memory"
sensor.id = "available_mem"
sensor.unit = "GB"
sensor.class = "None"