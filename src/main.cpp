#include <Arduino.h>
#include <WiFi.h>
// #include <WebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define pinBotao1 2 // D6

static String resultado;

WiFiClient wifiClient;

// MQTT Server
const char *BROKER_MQTT = "test.mosquitto.org"; // URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883;                         // Porta do Broker MQTT
// const char* user1 = "FsetCloud";
// const char* password1 = "123456";

#define ID_MQTT "EspCaixa01/"    // Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior.
#define TOPIC_PUBLISH "Caixa01/" // Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
PubSubClient MQTT(wifiClient);   // Instancia o Cliente MQTT passando o objeto espClient

// Declaração das Funções
void enviaValores();
String trigPulse();     // Função que gera o pulso de trigger
void mantemConexoes();  // Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi();     // Faz conexão com WiFi
void conectaMQTT();     // Faz conexão com Broker MQTT
void enviaPacoteMqtt(); //

void setup()
{
  pinMode(pinBotao1, INPUT_PULLUP);
  Serial.begin(9600);
  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

void loop()
{
  mantemConexoes();
  enviaValores();
  MQTT.loop();
  resultado = trigPulse();
}

void mantemConexoes()
{
  if (!MQTT.connected())
  {
    conectaMQTT();
  }
  else
  {
    enviaPacoteMqtt();
  }
  conectaWiFi(); // se não há conexão com o WiFI, a conexão é refeita
}

void conectaMQTT()
{

  while (!MQTT.connected())
  {
    Serial.print("Conectando ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) // if (MQTT.connect(ID_MQTT, user1, password1))
    {
      Serial.println("Conectado ao Broker com sucesso!");
    }
    else
    {
      Serial.println("Noo foi possivel se conectar ao broker.");
      Serial.println("Nova tentativa de conexao em 10s");
      delay(10000);
    }
  }
}

void enviaValores()
{
  static bool estadoBotao1 = HIGH;
  static bool estadoBotao1Ant = HIGH;
  static unsigned long debounceBotao1;

  estadoBotao1 = digitalRead(pinBotao1);
  if ((millis() - debounceBotao1) > 30) // Elimina efeito Bouncing
  {
    if (!estadoBotao1 && estadoBotao1Ant)
    {
      Serial.println("Botao1 APERTADO. Payload enviado.");

      debounceBotao1 = millis();
    }
    else if (estadoBotao1 && !estadoBotao1Ant)
    {
      Serial.println("Botao1 SOLTO. Payload enviado.");
      debounceBotao1 = millis();
    }
  }
  estadoBotao1Ant = estadoBotao1;
}

void enviaPacoteMqtt()
{
  static unsigned long delayEnvia;
  if ((millis() - delayEnvia) > 2000)
  {
    int tamanho = resultado.length();
    char charBuf[tamanho];
    resultado.toCharArray(charBuf, tamanho);
    boolean rc = MQTT.publish("Caixa01/", charBuf);
    delayEnvia = millis();
    Serial.println(resultado);
  }
}