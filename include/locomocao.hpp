/**
 * @file locomocao.hpp
 * @brief Este arquivo contém os códigos necessários para controlar a locomação do robô
 */

#pragma once

#include <cstdint>
#include <cmath>

#include "eletronica/config.hpp"
#include "controle.hpp"
#include "parametros/chihiro.hpp"
#include "parametros/hockey.hpp"

void mixagem(std::int32_t &velocidadeLinear, std::int32_t &velocidadeAngular, float &percentualVelocidade) {
    /* Mixagem simples */
    std::int32_t velocidadeMotorEsquerdo = velocidadeLinear + velocidadeAngular;
    std::int32_t velocidadeMotorDireito  = velocidadeLinear - velocidadeAngular;

    std::int32_t max_val = std::max(std::abs(velocidadeMotorEsquerdo), std::abs(velocidadeMotorDireito));

    if (max_val > HBridgePWM::getValorMaximoPotencia()) {
        velocidadeMotorEsquerdo = (velocidadeMotorEsquerdo * HBridgePWM::getValorMaximoPotencia()) / max_val;
        velocidadeMotorDireito  = (velocidadeMotorDireito * HBridgePWM::getValorMaximoPotencia()) / max_val;
    }

    motorEsquerdo.setPotencia(static_cast<int32_t>(percentualVelocidade * velocidadeMotorEsquerdo));
    motorDireito.setPotencia(static_cast<int32_t>(percentualVelocidade * velocidadeMotorDireito));
}

inline void pararRobo() {
    motorDireito.freiar();
    motorEsquerdo.freiar();
}