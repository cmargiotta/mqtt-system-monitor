local content

local run = io.popen
local results = run("df -h / | tail -n1  | awk '{ print $5 }' | sed 's/%//g'") 
for r in results:lines()
do
	content = r
end
results:close()

sensor.value = string.format("%.1f", content)
sensor.debug_message = ""
sensor.name = "Root FS usage"
sensor.id = "root_fs_usage"
sensor.unit = "%"
sensor.class = "None"