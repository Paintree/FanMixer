FROM ubuntu

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
build-essential \
cmake \
make \
gcc-arm-none-eabi \
gdb-multiarch \
openocd

# Set the working directory
WORKDIR /usr/src/fan_mixer
