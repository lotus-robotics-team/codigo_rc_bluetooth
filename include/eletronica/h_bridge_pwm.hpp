// Classe do driver de motor DRV8870

#pragma once

#include <cstdint>
#include <algorithm>

#include "driver/mcpwm_timer.h"
#include "driver/mcpwm_oper.h"
#include "driver/mcpwm_cmpr.h"
#include "driver/mcpwm_gen.h"
#include "esp_err.h"

class HBridgePWM {
    private:
        // Pinos de controle do driver
        const int pinoIN1;
        const int pinoIN2;

        bool invertido = false;

        mcpwm_timer_handle_t timer = nullptr;

        mcpwm_oper_handle_t oper = nullptr;

        mcpwm_cmpr_handle_t cmprIN1 = nullptr;
        mcpwm_cmpr_handle_t cmprIN2 = nullptr;

        mcpwm_gen_handle_t genIN1 = nullptr;
        mcpwm_gen_handle_t genIN2 = nullptr;

        static constexpr std::uint32_t RESOLUCAO_HZ = 25'000'000;
        static constexpr std::uint32_t PERIOD_TICKS = 1000;
        static constexpr std::uint32_t FREQUENCIA   = RESOLUCAO_HZ / PERIOD_TICKS; // 25MHz

        static constexpr std::uint32_t VALOR_MAX_POTENCIA = PERIOD_TICKS;
        static constexpr std::int32_t VALOR_MIN_POTENCIA  = -VALOR_MAX_POTENCIA;

        std::int32_t potencia = 0;

        void initTimer() {
            mcpwm_timer_config_t configTimer = {
                .group_id = 0,                            // Grupo 0 do MCPWM
                .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,   // Fonte de clock padrão (160MHz)
                .resolution_hz = RESOLUCAO_HZ,            // 1 tick = 1 segundo / RESOLUCAO_HZ
                .count_mode = MCPWM_TIMER_COUNT_MODE_UP,  // Contagem crescente
                .period_ticks = PERIOD_TICKS,             // Frequência do PWM = RESOLUCAO_HZ / PERIOD_TICKS (PERIOD_TICKS é o número de ticks para completar um período do PWM)
                .intr_priority = 0,                       // Sem prioridade de interrupção
                .flags = {}
            };
            ESP_ERROR_CHECK(mcpwm_new_timer(&configTimer, &timer));
            ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
            ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
        }

        void initOperators() {
            mcpwm_operator_config_t configOperator = {};
            configOperator.group_id = 0;

            ESP_ERROR_CHECK(mcpwm_new_operator(&configOperator, &oper));
            ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));
        }

        void initComparators() {
            mcpwm_comparator_config_t configComparator = {
                .intr_priority = 0,
                /// @attention O update compare on timer equal zero é essencial para evitar o driver queimar com pulsos errados
                .flags = {
                    .update_cmp_on_tez = true, // Atualiza o valor do comparador no início do período
                    .update_cmp_on_tep = false,
                    .update_cmp_on_sync = false
                }
            };
            ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &configComparator, &cmprIN1));
            ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &configComparator, &cmprIN2));
        }

        void initGenerators() {
            mcpwm_generator_config_t configGeneratorIN1 = {
                .gen_gpio_num = pinoIN1,
                .flags = {}
            };
            ESP_ERROR_CHECK(mcpwm_new_generator(oper, &configGeneratorIN1, &genIN1));

            mcpwm_generator_config_t configGeneratorIN2 = {
                .gen_gpio_num = pinoIN2,
                .flags = {}
            };
            ESP_ERROR_CHECK(mcpwm_new_generator(oper, &configGeneratorIN2, &genIN2));

            mcpwm_gen_timer_event_action_t actionZero = {
                .direction = MCPWM_TIMER_DIRECTION_UP,
                .event = MCPWM_TIMER_EVENT_EMPTY,
                .action = MCPWM_GEN_ACTION_HIGH
            };
            ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(genIN1, actionZero));
            ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(genIN2, actionZero));

            mcpwm_gen_compare_event_action_t actionIN1Compare = {
                .direction = MCPWM_TIMER_DIRECTION_UP,
                .comparator = cmprIN1,
                .action = MCPWM_GEN_ACTION_LOW
            };
            ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(genIN1, actionIN1Compare));

            mcpwm_gen_compare_event_action_t actionIN2Compare = {
                .direction = MCPWM_TIMER_DIRECTION_UP,
                .comparator = cmprIN2,
                .action = MCPWM_GEN_ACTION_LOW
            };
            ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(genIN2, actionIN2Compare));
        }

    public:
        HBridgePWM(int pIN1, int pIN2) 
            : pinoIN1(pIN1), pinoIN2(pIN2) {} // Construtor

        HBridgePWM(const HBridgePWM&) = delete; // Desabilita o construtor de cópia
        HBridgePWM(HBridgePWM&&)      = delete; // Desabilita o construtor de movimentação
        HBridgePWM& operator=(const HBridgePWM&) = delete; // Desabilita o operador de atribuição de cópia
        HBridgePWM& operator=(HBridgePWM&&)      = delete; // Desabilita o operador de atribuição de movimentação

        ~HBridgePWM() {
            if (genIN1)  mcpwm_del_generator(genIN1);
            if (genIN2)  mcpwm_del_generator(genIN2);
            if (cmprIN1) mcpwm_del_comparator(cmprIN1);
            if (cmprIN2) mcpwm_del_comparator(cmprIN2);
            if (oper)    mcpwm_del_operator(oper);
            if (timer) {
                mcpwm_timer_disable(timer);
                mcpwm_del_timer(timer);
            }
        }

        inline void begin() {
            initTimer();
            initOperators();
            initComparators();
            initGenerators();

            freiar();
        }

        inline void setPotencia(std::int32_t p) {
            p = std::clamp<std::int32_t>(p, VALOR_MIN_POTENCIA, VALOR_MAX_POTENCIA);

            if (invertido) p = -p;

            if (p > 0) {
                mcpwm_comparator_set_compare_value(cmprIN1, VALOR_MAX_POTENCIA - p);
                mcpwm_comparator_set_compare_value(cmprIN2, VALOR_MAX_POTENCIA);
            }

            else if (p < 0) {
                mcpwm_comparator_set_compare_value(cmprIN1, VALOR_MAX_POTENCIA);
                mcpwm_comparator_set_compare_value(cmprIN2, VALOR_MAX_POTENCIA + p);
            }

            else {
                mcpwm_comparator_set_compare_value(cmprIN1, VALOR_MAX_POTENCIA);
                mcpwm_comparator_set_compare_value(cmprIN2, VALOR_MAX_POTENCIA);
            }

            potencia = p;
        }

        inline void freiar() {
            setPotencia(0);
        }

        inline void pontoMorto() {
            mcpwm_comparator_set_compare_value(cmprIN1, 0);
            mcpwm_comparator_set_compare_value(cmprIN2, 0);
        }

        [[nodiscard]] inline std::int32_t getPotencia() const {
            return this->potencia;
        }

        [[nodiscard]] inline static std::int32_t getValorMaximoPotencia() {
            return VALOR_MAX_POTENCIA;
        }

        [[nodiscard]] inline static std::int32_t getValorMinimoPotencia() {
            return VALOR_MIN_POTENCIA;
        }
};