#ifdef HOCKEY

#pragma once

namespace Parametros {
    #ifdef ANA
        constexpr float velocidadePadrao = 0.8f;
        constexpr float velocidadeAgressiva = 1.f;

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

    #elif defined FROES
        constexpr float velocidadePadrao = 0.8f;
        constexpr float velocidadeAgressiva = 1.f;

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

    #elif defined CERQUEIRA
        constexpr float velocidadePadrao = 0.8f;
        constexpr float velocidadeAgressiva = 1.f;

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

    #else // default

        constexpr float velocidadePadrao = 0.8f;
        constexpr float velocidadeAgressiva = 1.f;

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

    #endif // Condicional ANA || FROES || CERQUEIRA || default

    float percentualVelocidade = velocidadePadrao;
}
#endif // HOCKEY