# mrubyc-esp32/main/CMakeLists.txt

idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES mrubyc nvs_flash
)

target_include_directories(${COMPONENT_LIB} PRIVATE
    ${CMAKE_BINARY_DIR}
)