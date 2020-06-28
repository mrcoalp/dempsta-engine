FROM gcc:9.3

RUN apt-get update && \
        apt-get install -y cmake && \
        apt-get install -y xorg-dev && \
        apt-get install -y clang-format && \
        apt-get install -y clang-tidy
