local content

local run = io.popen
local results = run("uname -r")
for r in results:lines()
do
	content = r
end
results:close()

sensor.value = tostring(content)
sensor.debug_message = ""
sensor.name = "Linux kernel"
sensor.id = "linux_kernel"
sensor.unit = ""
sensor.class = "None"
