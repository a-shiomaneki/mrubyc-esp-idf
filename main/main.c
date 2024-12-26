#include <stdio.h>

#include "esp_log.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mrubyc.h"
#include "nvs_flash.h"

#include "mrbbuf.h"

// #define SAMPLE_TASK_WITW_NO_MRBC_SCHEDULER
// #define SAMPLE_TASK_WITW_MRBC_SCHEDULER
#define SAMPLE_EXEC_IN_MAIN

#define TAG1 "mrubyc_task_1"
#define TAG2 "mrubyc_task_2"
#define TAG3 "mrubyc_exec_in_main"

#if !defined(MRBC_MEMORY_SIZE)
#define MRBC_MEMORY_SIZE (1024 * 40)
#endif
static uint8_t memory_pool[MRBC_MEMORY_SIZE];

#if defined(SAMPLE_TASK_WITW_NO_MRBC_SCHEDULER)
static void mruby_task1(void *arg) {
  hal_init();
  mrbc_init_alloc(memory_pool, MRBC_MEMORY_SIZE);
  mrbc_init_global();
  mrbc_init_class();

  mrbc_vm *vm = mrbc_vm_open(NULL);
  if (vm == NULL) {
    ESP_LOGE(TAG1, "%s", "Error: VM open failed");
    goto ERROR;
  }

  ESP_LOGI(TAG1, "%s", "Loading binary...");
  if (mrbc_load_mrb(vm, mrbbuf) != 0) {
    mrbc_print_exception(&vm->exception);
    ESP_LOGE(TAG1, "%s", "Error: Illegal bytecode");
    goto ERROR;
  }

  mrbc_vm_begin(vm);
  int ret = mrbc_vm_run(vm);
  mrbc_vm_end(vm);
  mrbc_vm_close(vm);

  ESP_LOGI(TAG1, "%d", ret);

ERROR:
  for (;;) {
    ESP_LOGI(TAG1, "%s", "Looping...");
    vTaskDelay(1);
  }
}
#endif

#if defined(SAMPLE_TASK_WITW_MRBC_SCHEDULER)
static void mruby_task2(void *arg) {
  mrbc_init(memory_pool, MRBC_MEMORY_SIZE);

  if (mrbc_create_task(mrbbuf, 0) != NULL) {
    mrbc_run();
    ESP_LOGI(TAG2, "%s", "Bytecode is loaded successfully");
  } else {
    ESP_LOGE(TAG2, "%s", "Error: Illegal bytecode");
  }

  for (;;) {
    ESP_LOGI(TAG2, "%s", "Looping...");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
#endif

void app_main(void) {
  esp_log_level_set("*", ESP_LOG_VERBOSE);

  nvs_flash_init();

#if defined(SAMPLE_TASK_WITW_NO_MRBC_SCHEDULER)
  xTaskCreate(&mruby_task1, TAG1, 8192, NULL, 5, NULL);
  ESP_LOGI(TAG1, "%s", "Task started");
#endif

#if defined(SAMPLE_TASK_WITW_MRBC_SCHEDULER)
  xTaskCreate(&mruby_task2, TAG2, 8192, NULL, 5, NULL, );
  ESP_LOGI(TAG2, "%s", "Task started");
#endif

#if defined(SAMPLE_EXEC_IN_MAIN)
  mrbc_init(memory_pool, MRBC_MEMORY_SIZE);
  if (mrbc_create_task(mrbbuf, 0) != NULL) {
    mrbc_run();
  }
#endif
}