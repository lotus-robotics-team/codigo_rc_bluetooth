#ifdef CHIHIRO

#pragma once

#include <cstdint>

#include "eletronica/config.hpp"
namespace Parametros {
    constexpr float velocidadePadrao = 0.4f;
    constexpr float velocidadeAgressiva = 0.45f;
    constexpr float velocidadeToretto = 0.55f;

    float percentualVelocidade = velocidadePadrao;

    /* MACROS */
    constexpr int tempoFreioDeMao = 60;

    constexpr int tempoGiro45Horario      = 40;
    constexpr int tempoGiro45AntiHorario  = 40;
    constexpr int tempoGiro90Horario      = 70;
    constexpr int tempoGiro90AntiHorario  = 70;
    constexpr int tempoGiro180Horario     = 200;
    constexpr int tempoGiro180AntiHorario = 200;

    constexpr int tempoFrenteZigZagHorario     = 155;
    constexpr int tempoFrenteZigZagAntiHorario = 155;
    constexpr int tempoOrtogonalZigZagHorario  = 180;
    constexpr int tempoOrtogonalZigZagAntiHorario = 180;
}
#endif