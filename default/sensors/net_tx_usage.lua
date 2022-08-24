local content

local run = io.popen
local results = run("cat /proc/net/dev | grep 'eth0:' | awk '{ print $10 }'")
for r in results:lines()
do
	content = r
end
results:close()

content = content/1073741824

sensor.value = string.format("%.3f", content)
sensor.debug_message = ""
sensor.name = "Network TX usage"
sensor.id = "net_tx_usage"
sensor.unit = "GiB"
sensor.class = "None"
