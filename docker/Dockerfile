FROM alpine:latest AS mqttsm-build

RUN echo 'https://dl-cdn.alpinelinux.org/alpine/edge/main' >> /etc/apk/repositories; \
	echo 'https://dl-cdn.alpinelinux.org/alpine/edge/community' >> /etc/apk/repositories;

RUN	apk update; \
	apk add --no-cache \
	git \
	meson \
	ninja \
	build-base \
	cmake \
	openssl-dev \
	paho-mqtt-c-dev \
	readline-dev \
	lua-dev 
		
RUN	git clone https://github.com/cmargiotta/MQTT-System-Monitor --recurse

RUN cd MQTT-System-Monitor; \
	meson build; \
	ninja -C build test

FROM alpine:latest

RUN echo 'https://dl-cdn.alpinelinux.org/alpine/edge/main' >> /etc/apk/repositories; \
	echo 'https://dl-cdn.alpinelinux.org/alpine/edge/community' >> /etc/apk/repositories;

RUN	apk update; \
	apk add --no-cache \
	libstdc++ \
	paho-mqtt-c-dev \
	lua-dev 
		
COPY --from=mqttsm-build /MQTT-System-Monitor/build/src/msm /usr/bin/

VOLUME [ "/etc/msm"]
WORKDIR /etc/msm
ENTRYPOINT ["/usr/bin/msm"]