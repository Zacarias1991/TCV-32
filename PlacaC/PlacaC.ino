#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int micPin = ÑP;      // MAX9814 salida analógica
const int chipSelect = ÑP;  // SD-nano CS
const int buzzerPin = ÑP;   // LM386 (bocina)

const int threshold = 200;  // Umbral para detectar voz

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_C3_C"); // Nombre del ESP32-C3 de C

  pinMode(micPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("Error al inicializar SD-nano");
    while (1);
  }
  Serial.println("SD-nano lista");
}

void loop() {
  int sonido = analogRead(micPin);

  if (sonido > threshold) {
    int comando = 1; // ID de comando (puede cambiar según lógica)

    // Enviar comando al robot (placa P) vía Bluetooth
    SerialBT.println("CMD:" + String(comando));

    // Reproducir audio local desde SD
    reproducirAudio(comando);

    delay(300);
  }

  // Revisar comandos entrantes de P
  if (SerialBT.available()) {
    String mensaje = SerialBT.readStringUntil('\n');
    if (mensaje.startsWith("AUDIO:")) {
      int audioID = mensaje.substring(6).toInt();
      reproducirAudio(audioID);
    }
  }
}

void reproducirAudio(int idAudio) {
  String filename = "AUDIO" + String(idAudio) + ".WAV";
  if (SD.exists(filename)) {
    Serial.println("Reproduciendo: " + filename);
    delay(500); // Simula duración del audio
  } else {
    Serial.println("Archivo no encontrado: " + filename);
  }
}
