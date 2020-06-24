FROM gcc:9.3

RUN apt-get update && \
        apt-get install -y cmake && \
        apt-get install -y xorg-dev
