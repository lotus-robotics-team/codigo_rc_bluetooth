/**
 * @file controle.hpp
 * @brief Este arquivo contém o código para o controle
 */

#pragma once

#include <cmath>
#include <cstdint>

#include <uni.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

struct Estados {
    bool roboArmado = false;

    std::int32_t velocidadeLinear = 0;  // Eixo Y do analógico direito
    std::int32_t velocidadeAngular = 0; // Eixo X do analógico esquerdo

    std::int32_t fullFrente = 0; // L2 - Vai para frente até a velocidade máxima, no analógico só vai até a velocidade padrão ou agressiva

    bool modoAgressivo = false;     // Select - Muda a porcentagem que multiplica a potencia enviada para o motor pelo analógico
    bool modoToretto = false;

    bool zigZagAntiHorario = false; // X + R1
    bool zigZagHorario = false;     // X + L1

    bool inverterMotorDireito = false;
    bool inverterMotorEsquerdo = false;
};

namespace Botao {
    constexpr uint16_t CROSS     = BUTTON_A;
    constexpr uint16_t BOLA      = BUTTON_B;
    constexpr uint16_t QUADRADO  = BUTTON_X;
    constexpr uint16_t TRIANGULO = BUTTON_Y;

    constexpr uint16_t CIMA     = DPAD_UP;
    constexpr uint16_t BAIXO    = DPAD_DOWN;
    constexpr uint16_t DIREITA  = DPAD_RIGHT;
    constexpr uint16_t ESQUERDA = DPAD_LEFT;

    constexpr uint16_t L1 = BUTTON_SHOULDER_L;
    constexpr uint16_t R1 = BUTTON_SHOULDER_R;

    constexpr uint8_t SELECT = MISC_BUTTON_BACK;
    constexpr uint8_t PS_    = MISC_BUTTON_SYSTEM;
}

class Controle {
    private:
        inline static Estados estadosControle;

        inline static SemaphoreHandle_t mutexControle = nullptr; // Mutex para proteger o acesso aos estados do controle

    public:
        static constexpr uint8_t DEADZONE = 50; // Valor da zona morta para os eixos analógicos

        static void init() {
            if (mutexControle == nullptr) {
                mutexControle = xSemaphoreCreateMutex();
            }
        }

        static void processarEntrada(uni_gamepad_t* gamepad) {
            if (!gamepad || !mutexControle) return;

            // Leitura dos eixos do analógico de -512 a 511
            int32_t analogicoDireitoEixoY  = -gamepad->axis_ry; // invertido para ser frente +511 e para trás -512
            int32_t analogicoEsquerdoEixoX = gamepad->axis_x;  // direita será +511 e para esquerda -512

            // Aplicando a zona morta
            if (std::abs(analogicoDireitoEixoY) < DEADZONE) analogicoDireitoEixoY = 0;
            if (std::abs(analogicoEsquerdoEixoX) < DEADZONE) analogicoEsquerdoEixoX = 0;

            analogicoDireitoEixoY  = static_cast<int32_t>((analogicoDireitoEixoY * 1000) / 512);
            analogicoEsquerdoEixoX = static_cast<int32_t>((analogicoEsquerdoEixoX * 1000) / 512);

            bool crossApertado = (gamepad->buttons & Botao::CROSS) != 0;
            bool bolaApertado = (gamepad->buttons & Botao::BOLA) != 0;
            // bool quadradoApertado = (gamepad->buttons & Botao::QUADRADO) != 0;
            bool trianguloApertado = (gamepad->buttons & Botao::TRIANGULO) != 0;

            // bool cimaApertado = (gamepad->buttons & Botao::CIMA) != 0;
            // bool baixoApertado = (gamepad->buttons & Botao::BAIXO) != 0;
            bool direitaApertado = (gamepad->buttons & Botao::DIREITA) != 0;
            bool esquerdaApertado = (gamepad->buttons & Botao::ESQUERDA) != 0;

            bool selectApertado = (gamepad->misc_buttons & Botao::SELECT) != 0;
            bool psApertado = (gamepad->misc_buttons & Botao::PS_) != 0;

            bool l1Apertado = (gamepad->buttons & Botao::L1) != 0;
            bool r1Apertado = (gamepad->buttons & Botao::R1) != 0;

            int32_t gatilhoR2 = gamepad->throttle; // valor de 0 a 1023
            gatilhoR2 = static_cast<int32_t>((gatilhoR2 * 1000) / 1023);
            if (std::abs(gatilhoR2) < 120) gatilhoR2 = 0;

            if (xSemaphoreTake(mutexControle, 0) == pdTRUE) { // tenta pegar o mutex sem bloquear
                if (psApertado && bolaApertado) {
                    estadosControle.roboArmado = true;
                }

                else if (psApertado && trianguloApertado) {
                    estadosControle.roboArmado = false;
                }

                estadosControle.velocidadeLinear = analogicoDireitoEixoY;
                estadosControle.velocidadeAngular = analogicoEsquerdoEixoX;

                estadosControle.fullFrente = gatilhoR2;

                estadosControle.modoAgressivo = selectApertado;
                estadosControle.modoToretto   = l1Apertado;

                estadosControle.zigZagAntiHorario = (crossApertado && r1Apertado);
                estadosControle.zigZagHorario     = (crossApertado && l1Apertado);

                estadosControle.inverterMotorDireito = (psApertado && direitaApertado);
                estadosControle.inverterMotorEsquerdo = (psApertado && esquerdaApertado);

                xSemaphoreGive(mutexControle); // libera o mutex
            }
        }

        static Estados getEstados() {
            Estados copiaEstadosControle;

            if (mutexControle && xSemaphoreTake(mutexControle, portMAX_DELAY) == pdTRUE) { // Espera até conseguir ler
                copiaEstadosControle = estadosControle; // Cópia dos dados atuais
                xSemaphoreGive(mutexControle);
            }

            return copiaEstadosControle;
        }

        static int32_t getVelocidadeLinear() {
            return estadosControle.velocidadeLinear;
        }

        static int32_t getVelocidadeAngular() {
            return estadosControle.velocidadeAngular;
        }

        inline static void mudarCorControle(uni_hid_device_t* device, uint8_t r, uint8_t g, uint8_t b) {
            if (device == nullptr) return;

            // Envia o comando para mudar a cor do controle
            if (device->report_parser.set_lightbar_color != nullptr) {
                device->report_parser.set_lightbar_color(device, r, g, b);
            }
        }

        inline static void vibrarControle(uni_hid_device_t* device, uint16_t duracao, uint8_t forca) {
            if (device == nullptr) return;

            if (device->report_parser.play_dual_rumble != nullptr) {
                device->report_parser.play_dual_rumble(device, 0, duracao, 0x00, forca);
            }
        }

        /// @brief Função para forçar o desarme do robô em caso de desconexão
        static void forceDesarme() {
            if (!mutexControle) return;

            if (xSemaphoreTake(mutexControle, pdMS_TO_TICKS(5)) == pdTRUE) {
                estadosControle = Estados{};
                xSemaphoreGive(mutexControle);
            }
        }
};

template <typename T = bool>
class Debouncing {
    private:
        T estadoAnterior{};

    public:
        bool apertado(T estadoAtual) {
            bool debounce = estadoAtual && !estadoAnterior;
            estadoAnterior = estadoAtual;

            return debounce;
        }
};