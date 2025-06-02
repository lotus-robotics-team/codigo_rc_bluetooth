// Classe do driver de motor DRV8870

#pragma once
#include <Arduino.h>

class DRV8870 {
    private:
        int pinoIN1;
        int pinoIN2;
        int canalIN1;
        int canalIN2;

        static constexpr int FREQUENCIA_PWM = 1000;
        static constexpr int RESOLUCAO_PWM = 8;
        static constexpr int POTENCIA_MAXIMA = (1 << RESOLUCAO_PWM) - 1;
        int _potencia;

    public:
        DRV8870(int _pinoIN1, int _pinoIN2, int _canalIN1, int _canalIN2) : pinoIN1(_pinoIN1), pinoIN2(_pinoIN2), canalIN1(_canalIN1), canalIN2(_canalIN2) {
            ledcAttachPin(pinoIN1, canalIN1);
            ledcAttachPin(pinoIN2, canalIN2);

            ledcSetup(canalIN1, FREQUENCIA_PWM, RESOLUCAO_PWM);
            ledcSetup(canalIN2, FREQUENCIA_PWM, RESOLUCAO_PWM);
        }

        void setVelocidade(int potencia) {
            potencia = constrain(potencia, -POTENCIA_MAXIMA, POTENCIA_MAXIMA);

            if (potencia > 0) {
                ledcWrite(canalIN1, potencia);
                ledcWrite(canalIN2, 0);
            } 
            
            else if (potencia < 0) {
                ledcWrite(canalIN1, 0);
                ledcWrite(canalIN2, abs(potencia));
            } 
            
            else {
                ledcWrite(canalIN1, 0);
                ledcWrite(canalIN2, 0);
            }

            _potencia = potencia;
        }

        void parar(bool freio = true) {
            if (freio) {
                ledcWrite(canalIN1, POTENCIA_MAXIMA);
                ledcWrite(canalIN2, POTENCIA_MAXIMA);
            }

            else {
                ledcWrite(canalIN1, 0);
                ledcWrite(canalIN2, 0);
            }
        }

        int getPotencia() {
            return _potencia;
        }
};