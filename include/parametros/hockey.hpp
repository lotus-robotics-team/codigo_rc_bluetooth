#ifdef HOCKEY

#pragma once

namespace Parametros {
    #ifdef ANA
        constexpr float velocidadePadrao = 0.8f;
        constexpr float velocidadeAgressiva = 1.f;
        constexpr float velocidadeToretto = 0.55f;

        /* MACROS */
        constexpr int tempoFreioDeMao = 60;

        constexpr int tempoGiro45Horario      = 45;
        constexpr int tempoGiro45AntiHorario  = 45;
        constexpr int tempoGiro90Horario      = 80;
        constexpr int tempoGiro90AntiHorario  = 80;
        constexpr int tempoGiro180Horario     = 200;
        constexpr int tempoGiro180AntiHorario = 200;

        constexpr int tempoFrenteZigZagHorario     = 100;
        constexpr int tempoFrenteZigZagAntiHorario = 40;
        constexpr int tempoOrtogonalZigZagHorario  = 120;
        constexpr int tempoOrtogonalZigZagAntiHorario = 120;

    #elif defined FROES
        constexpr float velocidadePadrao = 0.8f;
        constexpr float velocidadeAgressiva = 1.f;
        constexpr float velocidadeToretto = 0.55f;

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
        constexpr int tempoOrtogonalZigZagHorario  = 120;
        constexpr int tempoOrtogonalZigZagAntiHorario = 120;

    #elif defined CERQUEIRA
        constexpr float velocidadePadrao = 0.8f;
        constexpr float velocidadeAgressiva = 1.f;
        constexpr float velocidadeToretto = 0.55f;

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
        constexpr int tempoOrtogonalZigZagHorario  = 120;
        constexpr int tempoOrtogonalZigZagAntiHorario = 120;

    #else // default

        constexpr float velocidadePadrao = 0.8f;
        constexpr float velocidadeAgressiva = 1.f;
        constexpr float velocidadeToretto = 0.55f;

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
        constexpr int tempoOrtogonalZigZagHorario  = 120;
        constexpr int tempoOrtogonalZigZagAntiHorario = 120;

    #endif // Condicional ANA || FROES || CERQUEIRA || default

    float percentualVelocidade = velocidadePadrao;
}
#endif // HOCKEY