#ifdef CHIHIRO

#pragma once

#include <cstdint>

namespace Parametros {
    constexpr float velocidadePadrao = 0.38f;
    constexpr float velocidadeAgressiva = 0.45f;

    float percentualVelocidade = velocidadePadrao;

    /* MACROS */
    constexpr int tempoFreioDeMao = 60;

    constexpr int tempoGiro45Horario      = 50;
    constexpr int tempoGiro45AntiHorario  = 50;
    constexpr int tempoGiro90Horario      = 100;
    constexpr int tempoGiro90AntiHorario  = 100;
    constexpr int tempoGiro180Horario     = 200;
    constexpr int tempoGiro180AntiHorario = 200;

    constexpr int tempoFrenteZigZagHorario     = 100;
    constexpr int tempoFrenteZigZagAntiHorario = 100;
}
#endif