// Arquivo de pinagem das placas

#pragma once

#include <cstdint>

#include "driver/gpio.h"
#include "driver/ledc.h"

namespace Pino {
    #ifdef CHIHIRO

        constexpr int MOTOR_DIREITO_IN1  = 23;
        constexpr int MOTOR_DIREITO_IN2  = 22;
        constexpr int MOTOR_ESQUERDO_IN1 = 19;
        constexpr int MOTOR_ESQUERDO_IN2 = 18;

        constexpr int SERVO = 25;

        constexpr gpio_num_t LED = GPIO_NUM_26;

    #elifdef HOCKEY
    
        constexpr int MOTOR_ESQUERDO_D0 = 4;
        constexpr int MOTOR_ESQUERDO_D1 = 18;
        constexpr int MOTOR_DIREITO_D2  = 16;
        constexpr int MOTOR_DIREITO_D3  = 17;

        constexpr gpio_num_t LED = GPIO_NUM_23;

    #endif
}

namespace Canal {
    #ifdef CHIHIRO
        constexpr ledc_channel_t SERVO = LEDC_CHANNEL_0;
    #endif
}