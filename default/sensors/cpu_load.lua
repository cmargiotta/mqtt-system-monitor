local open = io.open

local file = open("/proc/stat", "r") 
local content = file:read() --first line

file:close()
	
local fields = {}
for str in string.gmatch(content, "([^ ]+)") do
	table.insert(fields, str)
end

local user = tonumber(fields[2])
local nice = tonumber(fields[3])
local system = tonumber(fields[4])
local idle = tonumber(fields[5])
local iowait = tonumber(fields[6])
local irq = tonumber(fields[7])
local softirq = tonumber(fields[8])

local steal = 0;
local guest = 0;
local guest_nice = 0;

if (#fields > 8) then
	steal = tonumber(fields[9])
	guest = tonumber(fields[10])
	guest_nice = tonumber(fields[11])
end

local load = 100 - (idle * 100)/(user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice)

sensor.value = string.format("%.3f", load)
sensor.debug_message = ""
sensor.name = "CPU Load"
sensor.id = "cpu_load"
sensor.unit = "%"
sensor.class = "sensor"
