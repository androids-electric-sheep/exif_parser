FROM docker.io/library/debian:bookworm as builder
RUN \
	apt update && \
	apt install -y libexif-dev build-essential valgrind
WORKDIR /build
COPY . .
RUN make
