# mrubyc-esp-idf
An example of mruby/c on ESP-IDF

## Description

I tested the latest mruby/c (Dec 27, 2024) on ESP-IDF v5.3.2, and the target is esp32c3. The original `hal/esp32/hal.c` caused errors related to the timer function, so I modified it with ChatGPT o1 and o1-mini support.

The arrangement of code is based on the following repositorie.
https://github.com/mruby-esp32/mrubyc-esp32

mruby/c code is placed in `components/mrubyc/mrubyc` as a git submodule. Because `hal/esp32`  is required as the `hal` subdirectory in mruby/c's `src,` I wrote automation logic in top-level `CMakeLists.txt,` which creates a symbolic link from `hal/esp32` to `src/hal.`

Also, automation logic for converting ruby code to bytecode is in top-level `CMakeLists.txt`

There are three type codes in `main/main.c`; these are activated macros  `SAMPLE_TASK_WITW_NO_MRBC_SCHEDULER,` `SAMPLE_TASK_WITW_MRBC_SCHEDULER,` and `SAMPLE_EXEC_IN_MAIN.`

When you try the `SAMPLE_TASK_WITW_NO_MRBC_SCHEDULER`  code, the `sleep 1` code in `resource/hello.rb` should be commented out because the `sleep` object can't be used without the mruby/c scheduler.

## Installation instructions

1. Install ESP-IDF v5.3.2 and configure for esp32c3 target.

1. Execute `export.sh` in ESP-IDF to enable `idf.py.`

1. Clone this repository.

1. To build the binary, execute `idf.py build` in the top-level directory of this repository.

1. Attach your ESP32C3 device to your computer.

1. To flash binary to the device, execute `idf.py -p <port> -b <baud rate> flash.`

1. To see the response from this app, execute `idf.py -p <port> -b <baud rate> monitor.`