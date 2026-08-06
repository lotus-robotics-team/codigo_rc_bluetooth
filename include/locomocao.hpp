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

    std::int32_t diferencaVelocidade = std::abs(std::abs(velocidadeAngular) - std::abs(velocidadeLinear));

        if (velocidadeMotorEsquerdo < 0) {
            velocidadeMotorEsquerdo -= diferencaVelocidade;
        }

        else {
            velocidadeMotorDireito += diferencaVelocidade;
        }

        if (velocidadeMotorDireito < 0) {
            velocidadeMotorDireito -= diferencaVelocidade;
        }

        else {
            velocidadeMotorEsquerdo += diferencaVelocidade;
        }

    motorEsquerdo.setPotencia(static_cast<int32_t>(percentualVelocidade * velocidadeMotorEsquerdo));
    motorDireito.setPotencia(static_cast<int32_t>(percentualVelocidade * velocidadeMotorDireito));
}

inline void pararRobo() {
    motorDireito.freiar();
    motorEsquerdo.freiar();
}