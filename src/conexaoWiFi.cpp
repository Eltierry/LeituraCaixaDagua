#include <Arduino.h>
#include <WiFi.h>

const char* SSID = "AP92";                // SSID / nome da rede WiFi que deseja se conectar
const char* PASSWORD = "9876543210";   // Senha da rede WiFi que deseja se conectar

void conectaWiFi() 
{

  if (WiFi.status() == WL_CONNECTED) 
  {
    return;
  }
        
  Serial.print("Conectando-se na rede: ");
  Serial.print(SSID);
  Serial.println("  Aguarde!");

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(SSID);  
  Serial.print("  IP obtido: ");
  Serial.println(WiFi.localIP()); 
}