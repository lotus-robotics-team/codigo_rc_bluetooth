// Arquivo principal do projeto

#include <stdio.h>

#include "locomocao.hpp"
#include "eletronica/config.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_check.h"
#include "nvs_flash.h"

extern "C" void app_main(void) {
   Sistema::initFlashNVS(); // Inicializa a memória flash não volátil


   motorDireito.begin();   // Inicializa o MCPWM do motor direito
   motorEsquerdo.begin();  // Inicializa o MCPWM do motor esquerdo

   while (true) {

      vTaskDelay(pdMS_TO_TICKS(10));
   }
}