#
# components/mrubyc/CMakeLists.txt
#
file(GLOB_RECURSE MRUBYC_CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/mrubyc/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mrubyc/hal/esp32/*.c
)

set(MRUBYC_SOURCES
    ${MRUBYC_CORE_SOURCES}
    ${MRUBYC_HAL_ESP32_SOURCES}
)

idf_component_register(
    SRCS ${MRUBYC_SOURCES}
    INCLUDE_DIRS
        "mrubyc/src"
        "mrubyc/hal/esp32"
    PRIV_REQUIRES
        driver
)
