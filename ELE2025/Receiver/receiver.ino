#include "LoRaWan_APP.h"
#include "Arduino.h"

#define RF_FREQUENCY                      915000000
#define LORA_BANDWIDTH                    0
#define LORA_SPREADING_FACTOR             7
#define LORA_CODINGRATE                   1
#define LORA_PREAMBLE_LENGTH              8
#define LORA_SYMBOL_TIMEOUT               0
#define LORA_FIX_LENGTH_PAYLOAD_ON        false
#define LORA_IQ_INVERSION_ON              false

static RadioEvents_t RadioEvents;

bool lora_received = false;
String received_msg = "";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando receptor LoRa...");

  Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);

  RadioEvents.RxDone = OnRxDone;
  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                  LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                  LORA_SYMBOL_TIMEOUT, false, 0, false,
                  false, 0, LORA_IQ_INVERSION_ON, true);

  Radio.Rx(0);
}

void loop() {
  Radio.IrqProcess();

  if (lora_received) {
    Serial.println("📡 Dados recebidos:");
    Serial.println(received_msg);
    lora_received = false;
    Radio.Rx(0); 
  }
}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
  received_msg = "";
  for (int i = 0; i < size; i++) {
    received_msg += (char)payload[i];
  }

  Serial.print("RSSI: ");
  Serial.print(rssi);
  Serial.print(" dBm | SNR: ");
  Serial.println(snr);

  lora_received = true;
}
