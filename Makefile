IMAGE_NAME    		= FanMixer.hex
TESTS_NAME			= FanMixerTests
BUILD_DIR     		= $(shell pwd)/build
TESTS_BUILD_DIR     = $(shell pwd)/build/tests
TESTS_DIR     		= $(shell pwd)/tests
BUILD_ENVIRONMENT   = build_environment
DOCKER_PATH			= /usr/src/fan_mixer

.PHONY: build cmake all test

all: build

.cmake:
	cmake . -B${BUILD_DIR} -DBUILD_DIR:STRING=${BUILD_DIR}

.cmake_test:
	cmake ${TESTS_DIR} -B${TESTS_BUILD_DIR}

build: .cmake
	${MAKE} -C ${BUILD_DIR}
	@echo "Built successfully!"

clean:
	rm -rf ${BUILD_DIR}

flash:
	openocd \
		-f interface/stlink.cfg \
		-f target/stm32f1x.cfg \
		-c "program ${BUILD_DIR}/${IMAGE_NAME} verify reset exit"

test: .cmake_test
	${MAKE} -C ${TESTS_BUILD_DIR}
	${TESTS_BUILD_DIR}/${TESTS_NAME}

.docker:
	docker build --tag ${BUILD_ENVIRONMENT} . 

build_docker: .docker
	docker run -v ${shell pwd}:${DOCKER_PATH} ${BUILD_ENVIRONMENT} sh -c "${MAKE} -j$(nproc) && chmod 777 -R build "


clean_docker:
	docker rmi -f ${BUILD_ENVIRONMENT}

