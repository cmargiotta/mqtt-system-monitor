local content

local run = io.popen
local results = run("cat /sys/devices/system/cpu/possible  | sed 's/-//g'")
for r in results:lines()
do
	content = r
end
results:close()

core = tonumber(content) + 1
	
sensor.value = string.format("%d", core)
sensor.debug_message = ""
sensor.name = "Cpu cores"
sensor.id = "cpu_cores"
sensor.unit = ""
sensor.class = "None"
