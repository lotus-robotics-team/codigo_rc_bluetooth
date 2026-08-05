// Arquivo principal do projeto

#include <stdio.h>
#include <cmath>

#include "eletronica/config.hpp"
#include "locomocao.hpp"
#include "bluetooth.hpp"
#include "macros.hpp"
#include "parametros/chihiro.hpp"
#include "parametros/hockey.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main(void) {
   Controle::init(); // Inicializa o mutex da classe estática do controle

   /*Hardwares/periféricos internos */
   Sistema::initFlashNVS(); // Inicializa a memória flash não volátil

   /* Hardwares/periféricos externos */
   led.begin();            // Inicializa o LED da placa
   motorDireito.begin();   // Inicializa o MCPWM do motor direito
   motorEsquerdo.begin();  // Inicializa o MCPWM do motor esquerdo

   // Criando tarefa do bluetooth para rodar no núcleo 0
   xTaskCreatePinnedToCore(
      Bluetooth::taskBluetooth,
      "task_bluetooth",
      8192,
      nullptr,
      5,
      nullptr,
      0 // Bluetooth rodando no núcleo 0
   );

   // Variáveis para debouncings
   bool estadoArmadoAnterior = false;
   bool modoAgressivoAnterior = false;

   while (true) {
      Estados comando = Controle::getEstados();

      // Lógica para ocorrer quando há troca de estado do robô (armado ou desarmado) para evitar poluir o ciclo do loop
      if (comando.roboArmado != estadoArmadoAnterior) {
         uni_hid_device_t* dispositivoControle = Bluetooth::getDevice();

         if (dispositivoControle != nullptr) {
            if (comando.roboArmado) {
               Controle::mudarCorControle(dispositivoControle, 160, 32, 240);
            }

            else {
               Controle::mudarCorControle(dispositivoControle, 0, 255, 0);
               Controle::vibrarControle(dispositivoControle, 100, 80);
            }
         }

         estadoArmadoAnterior = comando.roboArmado;
      }

      if (comando.roboArmado) {
         if (std::abs(comando.velocidadeAngular) > Controle::DEADZONE || std::abs(comando.velocidadeLinear) > Controle::DEADZONE) {
            mixagem(comando.velocidadeLinear, comando.velocidadeAngular, Parametros::percentualVelocidade);
         }

         /* COMANDOS DE COMBO DE BOTÕES */

         else if (comando.zigZagAntiHorario) {
            Macros::zigZagAntiHorario();
         }

         else if (comando.zigZagHorario) {
            Macros::zigZagHorario();
         }

         /* COMANDO DE UM BOTÃO SÓ */

         else if (comando.fullFrente > Controle::DEADZONE) {
            Macros::fullFrente(comando.fullFrente);
         }

         else {
            pararRobo();
         }

         if (comando.modoAgressivo && !modoAgressivoAnterior) {
            if (Parametros::percentualVelocidade == Parametros::velocidadePadrao) {
               Parametros::percentualVelocidade = Parametros::velocidadeAgressiva;
               Controle::vibrarControle(Bluetooth::getDevice(), 150, 200);
            }

            else {
               Parametros::percentualVelocidade = Parametros::velocidadePadrao;
               Controle::vibrarControle(Bluetooth::getDevice(), 300, 200);
            }
         }

         modoAgressivoAnterior = comando.modoAgressivo;
      }

      else {
         pararRobo();
      }

      vTaskDelay(pdMS_TO_TICKS(10));
   }
}