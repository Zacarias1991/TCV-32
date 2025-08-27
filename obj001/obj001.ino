

#include <Servo.h>
#include <ArduinoFFT.h>

const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";
const char* servidorReconocimiento = "http://TU_SERVIDOR/transcribir";
const char* servidorAudioBase = "http://TU_SERVIDOR/audio/";

#define A1 30
#define A2 31
#define A3 29

#define B1 26
#define B2 25
#define B3 33

#define MICRO_ADC 0
#define SAMPLES 64 
#define SAMPLING_FREQ 4000
double vReal[1];
double vImag[1];


#define SERVO_PIN 34

#define DAC_OUT 32

void setup() {
  Serial.begin(115200);

  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);

  servo.attach(SERVO_PIN);
  audio.setPinout(DAC_OUT, 26, -1);
  audio.setVolume(10);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConectado a WiFi");
}

void loop() {
// Leer SAMPLES del micrófono
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = analogRead(MIC_PIN); // Leer valor ADC
    vImag[i] = 0; // Parte imaginaria 0
    delayMicroseconds(1000000 / SAMPLING_FREQ); // Mantener frecuencia de muestreo
  }

  // Aplicar FFT
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  // Imprimir los resultados
  Serial.println("Frecuencias:");
  for (int i = 0; i < SAMPLES / 2; i++) {
    double frequency = i * ((double)SAMPLING_FREQ / SAMPLES);
    Serial.print(frequency);
    Serial.print(" Hz: ");
    Serial.println(vReal[i]);
  }
  
  delay(500);
}
  }