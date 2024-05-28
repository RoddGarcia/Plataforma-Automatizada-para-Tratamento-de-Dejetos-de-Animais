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
String Topico_04 = TopicoPrefixo+"/Acionar"; 

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
  clienteMQTT.subscribe(Topico_04.c_str(), [](const String & topic, const String & payload) {
    Serial.println("Mensagem recebida no tópico: " + topic + ", payload: " + payload);
    if (payload == "true") {
      digitalWrite(RELAY_PIN, HIGH);
    } else if (payload == "false") {
      digitalWrite(RELAY_PIN, LOW);
    }
  });

  clienteMQTT.publish(Topico_04.c_str(), "false", true); // Publicar o valor inicial false no tópico /Acionar
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
}

void setup() {
  // Configuração inicial
  Serial.begin(9600);
  Serial.println("Inicializando...");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Inicialmente desativa o relé

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
}

void loop() {
  // Loop principal
  clienteMQTT.loop(); // Função necessária para manter a conexão com o broker MQTT ativa e coletar as mensagens recebidas
  enviarDados(); // Publica os dados no broker MQTT

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
