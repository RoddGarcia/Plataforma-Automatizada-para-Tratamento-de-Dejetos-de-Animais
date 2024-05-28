#include <EspMQTTClient.h>

// Definicoes e constantes
char SSIDName[] = "Wokwi-GUEST"; // nome da rede WiFi
char SSIDPass[] = ""; // senha da rede WiFI

char BrokerURL[] = "broker.hivemq.com"; // URL do broker MQTT
char BrokerUserName[] = ""; // nome do usuario para autenticar no broker MQTT
char BrokerPassword[] = ""; // senha para autenticar no broker MQTT
char MQTTClientName[] = "mqtt-mack-sub"; // nome do cliente MQTT
int BrokerPort = 1883; // porta do broker MQTT

String TopicoPrefixo = "TESTMACK1165744"; // prefixo do topico
String Topico_01 = TopicoPrefixo + "/Temperatura"; // nome do topico 01
String Topico_02 = TopicoPrefixo + "/Umidade"; // nome do topico 02
String Topico_03 = TopicoPrefixo + "/Peso"; // nome do topico 03
String Topico_04 = TopicoPrefixo + "/Acionar"; // nome do topico 04

// Variaveis globais e objetos
EspMQTTClient clienteMQTT(SSIDName, SSIDPass, BrokerURL, BrokerUserName, BrokerPassword, MQTTClientName, BrokerPort); // inicializa o cliente MQTT

const int RELAY_PIN = 13; // terminal do relé
bool releAtivado = false;
unsigned long tempoReleAtivado = 0; // Variável para armazenar o tempo da última ativação do relé

void onConnectionEstablished() {
  // Subscreve nos topicos e mostra a(s) mensagem(ns) recebida(s) no monitor serial
  clienteMQTT.subscribe(Topico_01.c_str(), [](const String & topic, const String & payload) {
    Serial.println("Mensagem recebida no tópico: " + topic + ", payload: " + payload);
  });

  clienteMQTT.subscribe(Topico_02.c_str(), [](const String & topic, const String & payload) {
    Serial.println("Mensagem recebida no tópico: " + topic + ", payload: " + payload);
  });

  clienteMQTT.subscribe(Topico_03.c_str(), [](const String & topic, const String & payload) {
    Serial.println("Mensagem recebida no tópico: " + topic + ", payload: " + payload);
  });

  clienteMQTT.subscribe(Topico_04.c_str(), [](const String & topic, const String & payload) {
    Serial.println("Mensagem recebida no tópico: " + topic + ", payload: " + payload);
    if (payload == "true") {
      digitalWrite(RELAY_PIN, HIGH); // Ativar o relé
      Serial.println("Relé ativado");
      tempoReleAtivado = millis(); // Armazena o tempo da ativação do relé
      releAtivado = true;
    } else if (payload == "false") {
      digitalWrite(RELAY_PIN, LOW); // Desativar o relé
      Serial.println("Relé desativado");
      releAtivado = false;
    }
  });
}

void configurarRele() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Inicialmente desativa o relé
}

void setup() {
  Serial.begin(9600);
  clienteMQTT.enableDebuggingMessages();
  clienteMQTT.enableHTTPWebUpdater();
  clienteMQTT.enableLastWillMessage(Topico_01.c_str(), "Desconectado!");

  configurarRele(); // Configurações do relé

  Serial.println("Iniciando conexão WiFi e MQTT...");
}

void loop() {
  clienteMQTT.loop(); // Função necessária para manter a conexão com o broker MQTT ativa e coletar as mensagens recebidas

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

  // Verifica se o relé está ativado e se 7 segundos se passaram desde a última ativação
  if (releAtivado && millis() - tempoReleAtivado >= 7000) {
    digitalWrite(RELAY_PIN, LOW); // Desativar o relé
    Serial.println("Relé desativado após 7 segundos");
    clienteMQTT.publish(Topico_04, "false"); // Publicar mensagem "false" no tópico /Acionar
    releAtivado = false; // Atualizar o estado do relé
  }

  delay(2000); // Intervalo entre verificações
}
