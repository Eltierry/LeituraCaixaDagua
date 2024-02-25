#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#define trig 18 // Pino 9 do Arduino será a saída de trigger
#define echo 19 // Pino 8 do Arduino será a entrada de echo
float pulse;    // Variável que armazena o tempo de duração do echo
double dist_cm; // Variável que armazena o valor da distância em centímetros

StaticJsonDocument<200> doc;
static unsigned long delayEnvia;

String trigPulse()
{
    char out[128];

    char msg_out1[20];
    char msg_out2[20];
    digitalWrite(trig, HIGH); // Pulso de trigger em nível alto
    delayMicroseconds(10);    // duração de 10 micro segundos
    digitalWrite(trig, LOW);  // Pulso de trigge em nível baixo

    pulse = pulseIn(echo, HIGH, 200000); // Mede o tempo em que o pino de echo fica em nível alto
    //dist_cm = pulse / 58.82;             // Valor da distância em centímetros

    // dtostrf(dist_cm, 2, 2, msg_out1);
    //  MQTT.publish(TOPIC_PUBLISH, msg_out1);

    // double nivelPorCento = (double)(map((long)(dist_cm * 100), 6600, 2300, 010, 10000)) / 100; // Cada número foi adicionado dois dígitos para ter as casas decimais
    doc["distanciaCaixa01"] = pulse / 58.82;
    doc["nivelCaixa01"] = (double)(map((long)(dist_cm * 100), 6600, 2300, 010, 10000)) / 100;
    
    serializeJson(doc, out);
    return out;
}
