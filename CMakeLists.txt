# 必須の設定
cmake_minimum_required(VERSION 3.16)

# プロジェクトの設定
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(mrubyc-esp32 VERSION 0.1.0 LANGUAGES C CXX)

# mrbc の自動検出
find_program(MRBC_EXECUTABLE
    NAMES mrbc
    PATHS ENV PATH
    REQUIRED
    DOC "Path to the mruby compiler (mrbc)"
)

# mrb_code.h の生成
set(MRB_BUF_H "${CMAKE_BINARY_DIR}/mrbbuf.h")
set(RUBY_SCRIPT "${CMAKE_SOURCE_DIR}/main/resource/hello.rb")

add_custom_command(
    OUTPUT ${MRB_BUF_H}
    COMMAND ${MRBC_EXECUTABLE} --remove-lv -Bmrbbuf -o ${MRB_BUF_H} ${RUBY_SCRIPT}
    DEPENDS ${RUBY_SCRIPT}
    COMMENT "Generating mrb_code.h from ${RUBY_SCRIPT}"
    VERBATIM
)

add_custom_target(generate_mrb_code
    DEPENDS ${MRB_BUF_H}
)

# HAL のシンボリックリンク作成
set(HAL_SYMLINK "${CMAKE_SOURCE_DIR}/components/mrubyc/mrubyc/src/hal")
set(HAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/components/mrubyc/mrubyc/hal/esp32")

add_custom_command(
    OUTPUT ${HAL_SYMLINK}
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${HAL_SOURCE_DIR} ${HAL_SYMLINK}
    DEPENDS ${HAL_SOURCE_DIR}
    COMMENT "Creating symlink: ${HAL_SOURCE_DIR} -> ${HAL_SYMLINK}"
)

add_custom_target(create_mrubyc_symlink
    DEPENDS ${HAL_SYMLINK}
)

# シンボリックリンク作成をアプリに依存させる
add_dependencies(app create_mrubyc_symlink)
add_dependencies(app generate_mrb_code)
