#include <DHTesp.h>
#include <EspMQTTClient.h>
#include "HX711.h"

// Definicoes e constantes
char SSIDName[] = "Wokwi-GUEST"; // nome da rede WiFi
char SSIDPass[] = ""; // senha da rede WiFI

const int DHT_PIN = 15; // terminal do sensor de temperatura e umidade
const int RELAY_PIN = 13; // terminal do relé
int BOTAO_PIN = 23; 

char BrokerURL[] = "broker.hivemq.com"; // URL do broker MQTT
char BrokerUserName[] = ""; // nome do usuario para autenticar no broker MQTT
char BrokerPassword[] = ""; // senha para autenticar no broker MQTT
char MQTTClientName[] = "mqtt-mack-pub"; // nome do cliente MQTT
int BrokerPort = 1883; // porta do broker MQTT

String TopicoPrefixo = "TESTMACK1165744"; // prefixo do topico
String Topico_01 = TopicoPrefixo+"/Temperatura"; // nome do topico 01
String Topico_02 = TopicoPrefixo+"/Umidade"; // nome do topico 02
String Topico_03 = TopicoPrefixo+"/Peso"; // nome do topico 03

// Variaveis globais e objetos
DHTesp dhtSensor; // instancia o objeto dhtSensor a partir da classa DHTesp
EspMQTTClient clienteMQTT(SSIDName, SSIDPass, BrokerURL, BrokerUserName, BrokerPassword, MQTTClientName, BrokerPort); // inicializa o cliente MQTT

// Definicoes da balanca HX711
#define DOUT  18 // HX711 DATA OUT
#define CLK   5 // HX711 SCK IN

HX711 balanca; // define instancia balança HX711
float calibration_factor = 42130; // fator de calibração para teste inicial

// Este prototipo de funcao deve estar sempre presente
void onConnectionEstablished() {
  // Função chamada quando a conexão MQTT é estabelecida
}

void enviarDados() {
  // Obter dados do sensor DHT22
  TempAndHumidity temp_umid = dhtSensor.getTempAndHumidity();
  
  // Verificar se a leitura do sensor DHT22 é válida
  if (isnan(temp_umid.temperature) || isnan(temp_umid.humidity)) {
    Serial.println("Falha na leitura do sensor DHT22");
  } else {
    // Debug: Exibir temperatura e umidade no console
    Serial.print("Temperatura: ");
    Serial.print(temp_umid.temperature, 2);
    Serial.println("°C");
    Serial.print("Umidade: ");
    Serial.print(temp_umid.humidity, 1);
    Serial.println("%");

    // Publicar dados de temperatura e umidade no MQTT
    clienteMQTT.publish(Topico_01, String(temp_umid.temperature, 2) + "°C");
    clienteMQTT.publish(Topico_02, String(temp_umid.humidity, 1) + "%");
  }

  // Obter peso da balança
  balanca.set_scale(calibration_factor);
  float weight = balanca.get_units(3);

  // Verificar se a leitura da balança HX711 é válida
  if (isnan(weight)) {
    Serial.println("Falha na leitura da balança HX711");
  } else {
    // Debug: Exibir peso no console
    Serial.print("Peso: ");
    Serial.print(weight * 100);
    Serial.println(" kg");

    // Publicar dados de peso no MQTT
    clienteMQTT.publish(Topico_03, String(weight * 100, 2) + " kg");
  }

  // Ativar relé se responder à condição
  if (temp_umid.humidity >= 10) {
  // if (temp_umid.temperature >= 50 && (weight * 100) >= 10) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Relé ativado");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Relé desativado");
  }
}

void setup() {
  // Configuração inicial
  Serial.begin(9600);
  Serial.println("Inicializando...");

  pinMode(BOTAO_PIN, INPUT);

  // Inicializa o sensor de temperatura e umidade
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  Serial.println("Sensor DHT22 inicializado");

  // Inicializa a balança
  balanca.begin(DOUT, CLK);
  Serial.println("Balança HX711 inicializada");

  balanca.set_scale();
  zeraBalanca();

  // Configurações do cliente MQTT
  clienteMQTT.enableDebuggingMessages();
  Serial.println("Cliente MQTT configurado");

  // Configura o pino do relé como saída
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Inicialmente desativa o relé

  Serial.println("HX711 - Calibracao da Balanca");
  Serial.println("Remova o peso da balanca");
  Serial.println("Depois que as leituras começarem, coloque um peso conhecido sobre a Balanca");
  Serial.println("Pressione a,s,d,f para aumentar Fator de Calibração por 10,100,1000,10000 respectivamente");
  Serial.println("Pressione z,x,c,v para diminuir Fator de Calibração por 10,100,1000,10000 respectivamente");
  Serial.println("Após leitura correta do peso, pressione t para TARA(zerar)");
}

void loop() {
  // Loop principal
  clienteMQTT.loop(); // Função necessária para manter a conexão com o broker MQTT ativa e coletar as mensagens recebidas
  enviarDados(); // Publica os dados no broker MQTT

  if (digitalRead(BOTAO_PIN) == HIGH) {
    digitalWrite(RELAY_PIN, HIGH); // Ativar o relé
    Serial.println("Botão pressionado - Relé ativado");
    delay(1000); // Delay para evitar detecção múltipla de pressionamento
  }

  // Verifica o status da conexão WiFi e MQTT
  if (clienteMQTT.isWifiConnected()) {
    Serial.println("Conectado ao WiFi!");
  } else {
    Serial.println("Nao conectado ao WiFi!");
  }

  if (clienteMQTT.isMqttConnected()) {
    Serial.println("Conectado ao broker MQTT!");
  } else {
    Serial.println("Nao conectado ao broker MQTT!");
  }

  Serial.println("Nome do cliente: " + String(clienteMQTT.getMqttClientName())
    + " / Broker MQTT: " + String(clienteMQTT.getMqttServerIp())
    + " / Porta: " + String(clienteMQTT.getMqttServerPort())
  );

  // Ajuste do fator de calibração da balança
  if (Serial.available()) {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 10;
    else if (temp == 's')
      calibration_factor += 100;
    else if (temp == 'x')
      calibration_factor -= 100;
    else if (temp == 'd')
      calibration_factor += 1000;
    else if (temp == 'c')
      calibration_factor -= 1000;
    else if (temp == 'f')
      calibration_factor += 10000;
    else if (temp == 'v')
      calibration_factor -= 10000;
    else if (temp == 't')
      zeraBalanca();
  }

  delay(5000); // Intervalo entre as leituras e publicações
}

void zeraBalanca() {
  balanca.tare(); // Zera a balança
  Serial.println("Balança Zerada");
}
