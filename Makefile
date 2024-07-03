# This makefile is created for just simplify building and cleaning by CMake

IMAGE_NAME    = FanController.hex
BUILD_DIR    ?= $(shell pwd)/build
PACKAGES_DIR ?= ${BUILD_DIR}/packages
GREEN		  = \033[1;32m

.PHONY: build cmake all test

all: build

cmake:
	cmake . -B${BUILD_DIR} -DBUILD_DIR:STRING=${BUILD_DIR}

build: cmake
	${MAKE} -C ${BUILD_DIR}
	@echo "${GREEN}Built successfully!"

clean:
	rm -rf ${BUILD_DIR}

flash:
	openocd \
		-f interface/stlink.cfg \
		-f target/stm32f1x.cfg \
		-c "program ${BUILD_DIR}/${IMAGE_NAME} verify reset exit"

deps:
	sudo apt install gcc-arm-none-eabi gcc g++ cmake openocd