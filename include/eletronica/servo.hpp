/*
Classe responsável pelo controle do servo motor
*/

#pragma once

#include <cstdint>

#include "driver/ledc.h"
#include "esp_log.h"
#include "hal/gpio_types.h"

// Classe para controlar servo motores
class Servo {
    private:
        int pino;
        ledc_channel_t canal;

        // Atributo que armazena o valor da posição atual do servo motor
        std::uint8_t posicao = 0;

        // Definição do valor da frequência do sinal PWM do servo motor
        static constexpr std::uint32_t FREQUENCIA_DO_SINAL_PWM_DO_SERVO_MOTOR = 50;

        // Definição da resolução do sinal PWM
        static constexpr std::uint8_t RESOLUCAO_DO_SINAL_PWM_DO_SERVO_MOTOR = 15;

        // Definição das porcentagens do ciclo de trabalho do sinal PWM
        // Período do sinal PWM: 20000us
        // Valor mínimo do ciclo de trabalho: 500us
        // Valor médio do ciclo de trabalho: 1450us
        // Valor máximo do ciclo de trabalho: 2400us
        static constexpr float PORCENTAGEM_MINIMA_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR = 0.025f;  // Servo motor na posição 0°
        static constexpr float PORCENTAGEM_MEDIA_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR  = 0.0725f; // Servo motor na posição 90°
        static constexpr float PORCENTAGEM_MAXIMA_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR = 0.12f;   // Servo motor na posição 180°

        static constexpr unsigned int VALOR_MAXIMO_RESOLUCAO_PWM_SERVO = (1 << RESOLUCAO_DO_SINAL_PWM_DO_SERVO_MOTOR) - 1;

        // Constantes com os valores do ciclo de trabalho do sinal PWM
        static constexpr unsigned short int VALOR_MINIMO_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR = static_cast<unsigned short int>(VALOR_MAXIMO_RESOLUCAO_PWM_SERVO * PORCENTAGEM_MINIMA_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR);
        static constexpr unsigned short int VALOR_MEDIO_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR  = static_cast<unsigned short int>(VALOR_MAXIMO_RESOLUCAO_PWM_SERVO * PORCENTAGEM_MEDIA_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR);
        static constexpr unsigned short int VALOR_MAXIMO_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR = static_cast<unsigned short int>(VALOR_MAXIMO_RESOLUCAO_PWM_SERVO * PORCENTAGEM_MAXIMA_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR);

        /**
         * @brief Método para configurar o canal e o timer do ESP32 para o controle do servo motor.
         */
        void initCanal() {
            ledc_timer_config_t ledc_timer = {
                .speed_mode      = LEDC_HIGH_SPEED_MODE,
                .duty_resolution = static_cast<ledc_timer_bit_t>(RESOLUCAO_DO_SINAL_PWM_DO_SERVO_MOTOR),
                .timer_num       = LEDC_TIMER_0,
                .freq_hz         = FREQUENCIA_DO_SINAL_PWM_DO_SERVO_MOTOR,
                .clk_cfg         = LEDC_AUTO_CLK,
                .deconfigure     = false
            };
            ledc_timer_config(&ledc_timer);

            ledc_channel_config_t ledc_channel = {
                .gpio_num   = pino,                // Pino de saída
                .speed_mode = LEDC_HIGH_SPEED_MODE,
                .channel    = LEDC_CHANNEL_0,       // Canal já remapeado
                .intr_type  = LEDC_INTR_DISABLE,    // desativa as interrupções
                .timer_sel  = LEDC_TIMER_0,         // Timer já configurado
                .duty       = 0,                    // Começa com o duty cicle em 0
                .hpoint     = 0,
                .flags      = {}
            };
            ledc_channel_config(&ledc_channel);
        }

    public:

        /**
         * @brief Método construtor que configura o servo motor.
         * 
         * @param pino Número do pino de sinal do servo motor;
         * @param canal Canal de PWM do pino de sinal do servo motor.
         */
        Servo(int p, ledc_channel_t c) : pino(p), canal(c) {}

        Servo(const Servo&) = delete;            // Desabilita o construtor de cópia
        Servo& operator=(const Servo&) = delete; // Desabilita o operador de atribuição de cópia

        /// @brief Método para iniciar o controle do servo motor, configurando o canal e o timer do ESP32.
        void begin() { 
            initCanal(); 
        }

        /**
         * @brief Método para alterar a posição do servo motor de acordo com o valor de posição passado.
         * 
         * @param posicao Valor da posição, na qual varia no intervalo [0, 180], para aplicar no servo motor.
         */
        void setPosicao(std::uint8_t pos) {
            // Atualiza o atributo da posição do servo motor com o valor da posição atual e limita o valor da posição no intervalo [0, 180]
            this->posicao = (pos > 180) ? 180 : pos;

            // Remapeia o valor da posição do servo motor para o intervalo do ciclo de trabalho do sinal PWM
            std::uint32_t diferencaPWM = VALOR_MAXIMO_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR - VALOR_MINIMO_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR;
            std::uint32_t posicaoRemapeada = VALOR_MINIMO_DO_CICLO_DE_TRABALHO_DO_SERVO_MOTOR + ((this->posicao * diferencaPWM) / 180);

            // Escreve no canal o valor remapeado
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, this->canal, posicaoRemapeada);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, this->canal);
        }

        /**
         * @brief Método para obter o valor do atributo que armazena o valor da posição atual do servo motor.
         * 
         * @return O valor da posição atual do servo motor.
         */
        std::uint8_t getPosicao() const { 
            return this->posicao; 
        }
};