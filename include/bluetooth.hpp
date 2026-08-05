/// @brief Arquivo para funções do bluetooth

#pragma once

#include "controle.hpp"
#include "eletronica/config.hpp"

#include <uni.h>
#include <btstack.h>
#include <btstack_port_esp32.h>
#include <btstack_run_loop.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

class Bluetooth {
    private:
        inline static uni_hid_device_t* hidDevice = nullptr;

        static void callbackOnInit(int argc, const char** argv) {}
        static void callbackOnOobEvent(uni_platform_oob_event_t event, void* data) {}
        static void callbackOnGamepadDataOld(uni_hid_device_t* device, uni_gamepad_t* gamepad) {}

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
                Controle::processarEntrada(&controller->gamepad);
            }
        }

        static void callbackOnInitComplete(void) {
            uni_bt_start_scanning_and_autoconnect_unsafe();
            uni_bt_allow_incoming_connections(true);
        }

        static uni_error_t callbackOnDeviceReady(uni_hid_device_t* device) {
            if (device == nullptr) return UNI_ERROR_INVALID_DEVICE;

            uni_bt_enable_new_connections_unsafe(false); // Desabilita novas conexões

            Controle::mudarCorControle(device, 0, 255, 0); // Muda a cor do controle para verde
            led.ligar();                         // Liga o LED da placa
            Controle::vibrarControle(device, 300, 128);    // Vibra por 300ms com força 128

            return UNI_ERROR_SUCCESS;
        }

        static uni_error_t callbackOnDeviceDiscovered(bd_addr_t addr, const char* nome, uint16_t cod, uint8_t rssi) {
            // const uint8_t ENDERECO_MAC[6] = {0x4c, 0xb9, 0x9b, 0x1c, 0x44, 0xd8};

            // if (memcmp(addr, ENDERECO_MAC, 6) == 0) {
            //     return UNI_ERROR_SUCCESS;
            // }

            return UNI_ERROR_SUCCESS;
        }

        static const uni_property_t* callbackGetProperty(uni_property_idx_t idx) {
            return nullptr; 
        }

        inline static uni_platform platform = {
            .name = "RoboRC",
            .init = callbackOnInit,
            .on_init_complete = callbackOnInitComplete,
            .on_device_discovered = callbackOnDeviceDiscovered,
            .on_device_connected = callbackOnDeviceConnected,
            .on_device_disconnected = callbackOnDeviceDisconnected,
            .on_device_ready = callbackOnDeviceReady,
            .on_gamepad_data = callbackOnGamepadDataOld,
            .on_controller_data = callbackOnGamepadData,
            .get_property = callbackGetProperty,
            .on_oob_event = callbackOnOobEvent,
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

        static uni_hid_device_t* getDevice() {
            return hidDevice;
        }
};