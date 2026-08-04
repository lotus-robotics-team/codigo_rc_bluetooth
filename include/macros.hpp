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
        HBridgePWM &motorEsquerdo;
        HBridgePWM &motorDireito;

        bool delay(int tempo) {
            tempo *= 1000;
            unsigned long tempoAtual = esp_timer_get_time();

            while (esp_timer_get_time() - tempoAtual <= tempo) {
                if ((std::abs(estados.velocidadeLinear) > 0) || (std::abs(estados.velocidadeAngular) > 0)) {
                    return true; // Interrompe o delay se houver entrada do controle
                }
            }

            return false; // Delay concluído sem interrupção
        }

        void freioDeMao() {
            motorEsquerdo.freiar();
            motorDireito.freiar();

            if (delay(tempoFreioDeMao)) {
                return;
            }
        }

        void giro(bool sentidoHorario, int tempo) {
            if (sentidoHorario) {
                motorEsquerdo.setPotencia(motorEsquerdo.getValorMaximoPotencia());
                motorDireito.setPotencia(-motorDireito.getValorMinimoPotencia());
            }

            else {
                motorEsquerdo.setPotencia(-motorEsquerdo.getValorMinimoPotencia());
                motorDireito.setPotencia(motorDireito.getValorMaximoPotencia());
            }

            if (delay(tempo)) {
                return;
            }

            freioDeMao();
        }

    public:

        Macros(HBridgePWM &motorEsquerdo, HBridgePWM &motorDireito) : motorEsquerdo(motorEsquerdo), motorDireito(motorDireito) {}

        void zigZagHorario() {
            giro(false, tempoGiro45AntiHorario);

            motorEsquerdo.setPotencia(motorEsquerdo.getValorMaximoPotencia());
            motorDireito.setPotencia(motorDireito.getValorMaximoPotencia());

            if (delay(tempoFrenteZigZagHorario)) {
                return;
            }

            freioDeMao();

            giro(true, tempoGiro90Horario);

            motorEsquerdo.setPotencia(motorEsquerdo.getValorMaximoPotencia());
            motorDireito.setPotencia(motorDireito.getValorMaximoPotencia());

            if (delay(tempoFrenteZigZagHorario)) {
                return;
            }
        }

        void zigZagAntiHorario() {
            giro(true, tempoGiro45Horario);

            motorEsquerdo.setPotencia(motorEsquerdo.getValorMaximoPotencia());
            motorDireito.setPotencia(motorDireito.getValorMaximoPotencia());

            if (delay(tempoFrenteZigZagAntiHorario)) {
                return;
            }

            freioDeMao();

            giro(false, tempoGiro90AntiHorario);

            motorEsquerdo.setPotencia(motorEsquerdo.getValorMaximoPotencia());
            motorDireito.setPotencia(motorDireito.getValorMaximoPotencia());

            if (delay(tempoFrenteZigZagAntiHorario)) {
                return;
            }
        }
};