/**
 * @file config.hpp
 * @brief Esse arquivo contém todas as configurações eletrônicas do código (acesso ao hardware)
 * @note Este arquivo deve ser incluído em todos os arquivos que precisarem acessar o hardware do robô e apenas ele
 */

#pragma once

#include "pinagem.hpp"
#include "h_bridge_pwm.hpp"
#include "led.hpp"

#include "nvs_flash.h"

#if defined CHIHIRO
    HBridgePWM motorDireito(
        Pino::MOTOR_DIREITO_IN1, 
        Pino::MOTOR_DIREITO_IN2
    );
    HBridgePWM motorEsquerdo(
        Pino::MOTOR_ESQUERDO_IN1, 
        Pino::MOTOR_ESQUERDO_IN2
    );

#elif defined HOCKEY
    HBridgePWM motorDireito(
        Pino::MOTOR_DIREITO_D2, 
        Pino::MOTOR_DIREITO_D3
    );
    HBridgePWM motorEsquerdo(
        Pino::MOTOR_ESQUERDO_D0,
        Pino::MOTOR_ESQUERDO_D1
    );
#endif

LED led(Pino::LED);

namespace Sistema {
    /// @brief Função para inicializar a memória flash não volátil
    inline void initFlashNVS() {
        // Inicializando a non-volatile storage (NVS)
        esp_err_t ret = nvs_flash_init();

        // Verificando se a inicialização da NVS falhou
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
                ESP_ERROR_CHECK(nvs_flash_erase()); // Limpando a NVS
                ret = nvs_flash_init(); // Tentando inicializar novamente
        }

        ESP_ERROR_CHECK(ret); // Verificando se a inicialização da NVS foi bem-sucedida
    }
}