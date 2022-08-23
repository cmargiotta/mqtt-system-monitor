build:
	docker build -t mqttsm .

start:
	docker run -d -v $PWD/../config:/etc/msm:ro -name mqtt-system-monitor mqttsm