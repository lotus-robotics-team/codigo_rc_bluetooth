// Classe para controle do LED

#pragma once

#include <cstdint>

#include "driver/gpio.h"
#include "esp_err.h"

class LED {
    private:
        const gpio_num_t pino;

    public:
        explicit LED(gpio_num_t p) : pino(p) {}

        // Proteção contra cópias
        LED(const LED&) = delete;
        LED& operator=(const LED&) = delete;
        LED(LED&&) = delete;
        LED& operator=(LED&&) = delete;

        /// @brief Inicializa o pino do LED
        void begin() {
            gpio_config_t config = {
                .pin_bit_mask = (1ULL << pino),
                .mode = GPIO_MODE_OUTPUT,
                .pull_up_en = GPIO_PULLUP_DISABLE,
                .pull_down_en = GPIO_PULLDOWN_DISABLE,
                .intr_type = GPIO_INTR_DISABLE
            };

            ESP_ERROR_CHECK(gpio_config(&config));
            gpio_set_level(pino, 0); // garante que será iniciado desligado
        }

        /// @brief Liga o LED
        inline void ligar() {
            gpio_set_level(pino, 1);
        }

        /// @brief Desliga o LED
        inline void desligar() {
            gpio_set_level(pino, 0);
        }

        /// @brief Alterna o estado do LED (ligado/desligado)
        inline void alternar() {
            int estadoAtual = gpio_get_level(pino);
            gpio_set_level(pino, !estadoAtual);
        }

        /**
         * @brief Método para obter o estado atual do LED
         * @return true se o LED estiver ligado, false caso contrário
         */
        [[nodiscard]] inline bool getEstado() const {
            return gpio_get_level(pino) != 0;
        }
};