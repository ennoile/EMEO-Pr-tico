#include <Arduino.h>

// Pinos e variáveis
const int ledBluePin = 1;       // Pino do LED azul
const int ledRedPin = 3;        // Pino do LED vermelho
bool ledState = false;          // Estado atual do LED azul

const int buttonPin = 4;        // Pino do botão 1
const int buttonPin2 = 8;       // Pino do botão 2

bool button1Toggled = false;    // Indica se o botão 1 controla o recurso
bool button2Toggled = false;    // Indica se o botão 2 controla o recurso

bool lock = false;              // Recurso bloqueado (exclusão mútua)

// Estado anterior dos botões (para detectar flancos)
bool lastButtonState1 = false;
bool lastButtonState2 = false;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);

  digitalWrite(ledBluePin, LOW);  // Inicializa o LED azul apagado
  digitalWrite(ledRedPin, LOW);   // Inicializa o LED vermelho apagado
  Serial.begin(9600);             // Inicia comunicação serial para mensagens
},

void loop() {
  // Leitura dos estados dos botões
  bool buttonState1 = digitalRead(buttonPin);
  bool buttonState2 = digitalRead(buttonPin2);

  // Verifica o botão 1
  if (buttonState1 && !lastButtonState1) { // Botão 1 pressionado
    if (!lock || button1Toggled) {         // Se o recurso estiver livre ou controlado pelo botão 1
      lock = !lock;                        // Alterna o lock (libera ou adquire o recurso)
      ledState = lock;                     // Atualiza o estado do LED com base no lock
      digitalWrite(ledBluePin, ledState);  // Atualiza o LED azul fisicamente
      digitalWrite(ledRedPin, !ledState); // Atualiza o LED vermelho fisicamente (se lock=true, vermelho apaga)
      
      button1Toggled = lock;               // Define o botão 1 como controlador (ou libera)
      button2Toggled = false;              // Reseta o controle do botão 2
      Serial.print(lock ? "BTN1-L;" : "BTN1-U;");
    } else {
      // Recurso ocupado por outro botão
      Serial.print("BTN1-B;");
    }
  }

  // Verifica o botão 2
  if (buttonState2 && !lastButtonState2) { // Botão 2 pressionado
    if (!lock || button2Toggled) {         // Se o recurso estiver livre ou controlado pelo botão 2
      lock = !lock;                        // Alterna o lock (libera ou adquire o recurso)
      ledState = lock;                     // Atualiza o estado do LED com base no lock
      digitalWrite(ledBluePin, ledState);  // Atualiza o LED azul fisicamente
      digitalWrite(ledRedPin, !ledState); // Atualiza o LED vermelho fisicamente (se lock=true, vermelho apaga)
      
      button2Toggled = lock;               // Define o botão 2 como controlador (ou libera)
      button1Toggled = false;              // Reseta o controle do botão 1
      Serial.print(lock ? "BTN2-L;" : "BTN2-U;");
    } else {
      // Recurso ocupado por outro botão
      Serial.print("BTN2-B;");
    }
  }

  // Atualiza os estados anteriores dos botões
  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
}