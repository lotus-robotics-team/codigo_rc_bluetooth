cmake_minimum_required(VERSION 3.16.0)

set(BLUEPAD32_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/components/bluepad32)

set(EXTRA_COMPONENT_DIRS 
    ${BLUEPAD32_ROOT}/src/components
    ${BLUEPAD32_ROOT}/external/btstack/port/esp32/components
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(codigo_rc_ps5)