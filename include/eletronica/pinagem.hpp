// Arquivo de pinagem das placas

#pragma once

#include <cstdint>

#include "driver/gpio.h"
#include "driver/ledc.h"

namespace Pino {
    #ifdef CHIHIRO

        constexpr gpio_num_t MOTOR_DIREITO_IN1  = GPIO_NUM_18;
        constexpr gpio_num_t MOTOR_DIREITO_IN2  = GPIO_NUM_19;
        constexpr gpio_num_t MOTOR_ESQUERDO_IN1 = GPIO_NUM_22;
        constexpr gpio_num_t MOTOR_ESQUERDO_IN2 = GPIO_NUM_23;

        constexpr int SERVO = 25;

        constexpr gpio_num_t LED = GPIO_NUM_26;

    #elifdef HOCKEY
    
        constexpr gpio_num_t MOTOR_ESQUERDO_D0 = GPIO_NUM_4;
        constexpr gpio_num_t MOTOR_ESQUERDO_D1 = GPIO_NUM_18;
        constexpr gpio_num_t MOTOR_DIREITO_D2  = GPIO_NUM_16;
        constexpr gpio_num_t MOTOR_DIREITO_D3  = GPIO_NUM_17;

    #endif
}

namespace Canal {
    #ifdef CHIHIRO
        constexpr ledc_channel_t SERVO = LEDC_CHANNEL_0;
    #endif
}