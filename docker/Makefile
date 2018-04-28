SRC=../
PORT=8080

.PHONY: all build run help

all: build run

build:
	docker build -t wplexserver \
				--build-arg port=${PORT} \
				 -f Dockerfile ${SRC}

run:
	docker run -d --name wplexserver_1 \
			   -p 8080:8080 wplexserver

clean:
	-docker rm -f wplexserver_1
	-docker rmi wplexserver

help:
	@cat Makefile
