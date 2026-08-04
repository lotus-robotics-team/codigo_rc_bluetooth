/// @brief Arquivo para funções do bluetooth

#include "controle.hpp"
#include "eletronica/config.hpp"

#include <uni.h>
#include <btstack.h>
#include <btstack_port_esp32.h>
#include <btstack_run_loop.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Bluetooth {
    private:
        inline static uni_hid_device_t* hidDevice = nullptr;

        static void callbackOnDeviceConnected(uni_hid_device_t* device) {
            hidDevice = device;
        }

        static void callbackOnDeviceDisconnected(uni_hid_device_t* device) {
            if (hidDevice == device) {
                hidDevice = nullptr;
            }

            led.desligar(); // Desliga o LED da placa
        }

        static void callbackOnGamepadData(uni_hid_device_t* device, uni_controller_t* controller) {
            if (device == nullptr || controller == nullptr) return;

            if (controller->klass == UNI_CONTROLLER_CLASS_GAMEPAD) {
                processarEntradaControle(&controller->gamepad);
            }
        }

        static void callbackOnInitComplete(void) {
            uni_bt_start_scanning_and_autoconnect_unsafe();
            uni_bt_allow_incoming_connections(true);
        }

        static uni_error_t callbackOnDeviceReady(uni_hid_device_t* device) {
            if (device == nullptr) return UNI_ERROR_INVALID_DEVICE;

            uni_bt_enable_new_connections_unsafe(false); // Desabilita novas conexões

            mudarCorControle(device, 0, 255, 0); // Muda a cor do controle para verde
            led.ligar();                         // Liga o LED da placa
            vibrarControle(device, 300, 128);    // Vibra por 300ms com força 128

            return UNI_ERROR_SUCCESS;
        }

        inline static uni_platform platform = {
            .name = "RoboRC",
            .init = nullptr,
            .on_init_complete = callbackOnInitComplete,
            .on_device_discovered = nullptr,
            .on_device_connected = callbackOnDeviceConnected,
            .on_device_disconnected = callbackOnDeviceDisconnected,
            .on_device_ready = callbackOnDeviceReady,
            .on_gamepad_data = nullptr,
            .on_controller_data = callbackOnGamepadData,
            .get_property = nullptr,
        };

    public:
        Bluetooth() {}

        static void taskBluetooth(void* pvParameters) {
            btstack_init(); // Inicia a stack do bluetooth

            uni_platform_set_custom(&platform); // Ativa a plataforma customizada montada

            uni_init(0, nullptr); // Inicia a máquina de estados do bluepad32

            btstack_run_loop_execute(); // Loop infinito do bluetooth

            vTaskDelete(nullptr); // Segurança (O ideal é que isso nunca chegue a acontecer)
        }
};