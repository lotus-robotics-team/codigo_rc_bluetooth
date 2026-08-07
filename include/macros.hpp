#pragma once

#include <cstdint>
#include <cmath>

#include "eletronica/config.hpp"
#include "parametros/chihiro.hpp"
#include "parametros/hockey.hpp"
#include "controle.hpp"

#include <esp_timer.h>

class Macros {
    private:
        static bool delay(std::int64_t tempo) {
            tempo = static_cast<std::int64_t>(tempo * 1000);
            std::int64_t tempoAtual = esp_timer_get_time();

            while (esp_timer_get_time() - tempoAtual <= tempo) {
                if ((std::abs(Controle::getVelocidadeLinear()) > 0) || (std::abs(Controle::getVelocidadeAngular()) > 0)) {
                    return true; // Interrompe o delay se houver entrada do controle
                }

                vTaskDelay(pdMS_TO_TICKS(1));
            }

            return false; // Delay concluído sem interrupção
        }

        static void freioDeMao() {
            motorEsquerdo.freiar();
            motorDireito.freiar();

            if (delay(Parametros::tempoFreioDeMao)) {
                return;
            }
        }

        static void giro(bool sentidoHorario, int tempo) {
            if (sentidoHorario) {
                motorEsquerdo.setPotencia(motorEsquerdo.getValorMaximoPotencia());
                motorDireito.setPotencia(motorDireito.getValorMinimoPotencia());
            }

            else {
                motorEsquerdo.setPotencia(motorEsquerdo.getValorMinimoPotencia());
                motorDireito.setPotencia(motorDireito.getValorMaximoPotencia());
            }

            if (delay(tempo)) {
                return;
            }

            freioDeMao();
        }

    public:
        static void fullFrente(int32_t velocidade) {
            velocidade = std::clamp(velocidade, static_cast<int32_t>(0), HBridgePWM::getValorMaximoPotencia());

            motorEsquerdo.setPotencia(velocidade);
            motorDireito.setPotencia(velocidade);
        }

        static void zigZagHorario() {
            giro(false, Parametros::tempoGiro45AntiHorario);

            motorEsquerdo.setPotencia(700);
            motorDireito.setPotencia(700);

            if (delay(Parametros::tempoFrenteZigZagHorario)) {
                return;
            }

            freioDeMao();

            giro(true, Parametros::tempoGiro90Horario);

            motorEsquerdo.setPotencia(700);
            motorDireito.setPotencia(700);

            if (delay(Parametros::tempoOrtogonalZigZagHorario)) {
                return;
            }
        }

        static void zigZagAntiHorario() {
            giro(true, Parametros::tempoGiro45Horario);

            motorEsquerdo.setPotencia(700);
            motorDireito.setPotencia(700);

            if (delay(Parametros::tempoFrenteZigZagAntiHorario)) {
                return;
            }

            freioDeMao();

            giro(false, Parametros::tempoGiro90AntiHorario);

            motorEsquerdo.setPotencia(700);
            motorDireito.setPotencia(700);

            if (delay(Parametros::tempoOrtogonalZigZagAntiHorario)) {
                return;
            }
        }
};