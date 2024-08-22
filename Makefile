IMAGE_NAME    		= FanControllerBoard.hex
TESTS_NAME			= FanControllerTests
BUILD_DIR     		= $(shell pwd)/build
TESTS_BUILD_DIR     = $(shell pwd)/build/tests
TESTS_DIR     		= $(shell pwd)/tests

.PHONY: build cmake all test

all: build

cmake:
	cmake . -B${BUILD_DIR} -DBUILD_DIR:STRING=${BUILD_DIR}

cmake_test:
	cmake ${TESTS_DIR} -B${TESTS_BUILD_DIR}

build: cmake
	${MAKE} -C ${BUILD_DIR}
	@echo "Built successfully!"

clean:
	rm -rf ${BUILD_DIR}

flash:
	openocd \
		-f interface/stlink.cfg \
		-f target/stm32f1x.cfg \
		-c "program ${BUILD_DIR}/${IMAGE_NAME} verify reset exit"

test: cmake_test
	${MAKE} -C ${TESTS_BUILD_DIR}
	${TESTS_BUILD_DIR}/${TESTS_NAME}
