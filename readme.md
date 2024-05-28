# Desenvolvimento de uma Plataforma Sustentável para Tratamento Automatizado de Dejetos de Animais

**Caio Leão Ribeiro, Gustavo Henrique Fernandes Vieira, Matheus Barbosa Ferrari, Rodrigo Garcia de Carvalho, Wallace Santana**  
**Universidade Presbiteriana Mackenzie (UPM)**  
Rua da Consolação, 930 Consolação, São Paulo - SP, 01302-907 - Brasil  
{10401002@mackenzista.br, 10400715@mackenzista.br, 10400837@mackenzista.br, 10119867@mackenzista.br, 1165744@mackenzie.br}

## Abstract

Properly treated animal waste management is a growing concern. This project proposes a platform for the automated treatment of animal waste with the help of Arduino, sensors, and actuators in public places. The platform not only prevents waste in public places but also treats its appropriate disposal, integrating responsible water management, which is advocated by the UN Sustainable Development Goals. The system uses sensors to detect the presence of animals and their waste, activating actuators to clean and control the drainage system. This multidisciplinary approach seeks to develop an innovative and efficient solution applicable in different contexts, contributing to a cleaner and more sustainable environment.

## Resumo

A gestão de resíduos animais tratada da maneira adequada é uma preocupação crescente. Este projeto propõe uma plataforma para o tratamento automatizado de dejetos de animais com o auxílio do Arduino, sensores e atuadores em lugares públicos. A plataforma não apenas evita os dejetos em lugares públicos, mas trata a sua destinação adequada, além de integrar a gestão responsável da água, sendo um dos tópicos defendidos pelos Objetivos de Desenvolvimento Sustentável da ONU. O sistema utiliza sensores para detectar a presença de animais e dos seus dejetos, acionando atuadores para a limpeza e controle do sistema de escoamento. Esta abordagem multidisciplinar busca desenvolver uma solução inovadora e eficiente aplicável em diversos contextos, contribuindo para um ambiente mais limpo e sustentável.

## 1. Introdução

A crescente preocupação com a gestão adequada de resíduos animais tem impulsionado a busca por soluções inovadoras e sustentáveis. Nesse contexto, a criação de uma plataforma para o tratamento automatizado de dejetos de animais surge como uma resposta promissora para mitigar os impactos ambientais e facilitar a vida dos proprietários de animais de estimação. Este projeto visa explorar e implementar uma solução que não apenas promova a limpeza eficiente das áreas contaminadas, mas também contribua para a preservação dos recursos naturais.

A plataforma proposta baseia-se em tecnologias acessíveis, como o Arduino, para criar um sistema automatizado capaz de identificar e limpar os dejetos de forma rápida e eficaz (Monk, 2016). Além disso, busca-se integrar o uso responsável da água e a destinação adequada dos resíduos para o sistema de esgoto, minimizando assim o impacto ambiental e promovendo a sustentabilidade (Li & Yang, 2020).

Em 2015, a ONU criou um plano de ação para um desenvolvimento sustentável, considerando várias áreas diferentes para inovação. A Agenda 2030 traz consigo 17 objetivos com o propósito de concretizar os direitos humanos de todos e alcançar a igualdade de gênero. Além dessas tarefas, este movimento busca aprimorar a gestão sustentável dos recursos naturais para proporcionar aos seres vivos um melhor ambiente (Sun et al., 2021).

Dentre os 17 Objetivos de Desenvolvimento Sustentável (ODS) da ONU, destaca-se a relevância direta para a Meta 6, que visa garantir a disponibilidade e gestão sustentável da água e saneamento para todos. Além disso, o projeto contribui indiretamente para outras metas, como a Meta 11 (Cidades e Comunidades Sustentáveis) e a Meta 12 (Consumo e Produção Responsáveis), ao promover práticas mais sustentáveis no manejo de resíduos (Zhu, Wang & Xie, 2018).

O projeto tem como objetivo preencher a lacuna de gestão de resíduos animais com o auxílio de tecnologias modernas no que diz respeito a soluções automatizadas. Para embasar nossas abordagens e decisões de projeto, faremos referência a estudos prévios relacionados à gestão de resíduos animais, tecnologias de automação e sistemas sustentáveis (Li & Yang, 2020). Além disso, buscaremos orientação em trabalhos acadêmicos e práticas recomendadas na área de desenvolvimento sustentável.

Ao adotar uma abordagem multidisciplinar, integrando conhecimentos de engenharia, ciência ambiental e design, esperamos desenvolver uma plataforma inovadora e eficiente que possa ser aplicada em diversos contextos, desde residências até espaços públicos, contribuindo assim para um futuro mais limpo e sustentável (Sun et al., 2021).

## 2. Materiais e Métodos

### 2.1 Componentes Utilizados

Serão utilizados os seguintes componentes para a montagem do sistema automatizado:

- Sensor de Peso (HX711)
- Módulo ESP 32
- Atuadores (Módulo Relé)
- Sistema de escoamento (tubos e válvulas)
- Fonte de alimentação
- Sensor de temperatura e umidade (DHT22)

### 2.2 Programação do Arduino

A programação do Arduino será realizada utilizando a linguagem C++ e consiste em:

- Leitura dos sensores
- Acionamento dos atuadores
- Cálculo de comparação de pesos
- Controle do sistema de escoamento

### 2.3 Metodologia

Para iniciar o experimento, utilizaremos a programação com o Arduino para a criação da lógica geral do projeto, que consiste em configurar dois sensores: o primeiro, um sensor de peso (HX711), que enviará à API o valor do peso; e um sensor DHT22, que enviará temperatura e umidade. A ideia principal destes sensores é de detectar mudanças no calor emitido pelos dejetos liberados pelos animais. Quando um animal se move dentro do alcance do sensor, a temperatura ambiente ao redor dele muda, e o sensor pode detectar essa mudança (Arduino, n.d.). O segundo sensor consiste em um sensor de peso para detectar a presença de animais e seus dejetos separadamente. O sensor mede o peso da plataforma, e o Arduino calcula as diferenças de peso para determinar quando um animal está presente, quando ele excreta e quando apenas os dejetos estão na plataforma. Isso pode ser útil em fazendas, casas com animais de estimação e estudos de vida selvagem.

O sistema começa inicializando todas as variáveis necessárias e calibrando o sensor de peso para obter a leitura inicial da plataforma sem nenhum objeto sobre ela. No loop principal, o sistema realiza as seguintes etapas continuamente:

1. Ele lê os sensores conectados. Isso inclui a leitura do sensor de peso para obter o peso atual da plataforma e o sensor de temperatura e umidade.
2. Após a leitura, o sistema verifica se houve a diferença de temperatura e peso de um momento para o outro, e caso esta condição seja positiva, será ativado o Relé, que enviará uma carga de energia para ativar a bomba de aquário e liberar o fluxo d’água para fazer os dejetos irem em direção ao ralo.

Com base nas leituras dos sensores, o sistema determina o estado atual da plataforma: se houver apenas o animal, se o animal estiver presente com excrementos ou se houver apenas excrementos (quando o animal saiu).

![Imagem1](https://github.com/RoddGarcia/Plataforma-Automatizada-para-Tratamento-de-Dejetos-de-Animais/assets/85592905/b6af74a9-8aec-457a-8cb4-07329ffee688)

Na Figura 1, exemplificamos com dois retângulos coloridos representando os sensores. O retângulo à esquerda, de cor verde, representa onde ficará o ESP32 e o sensor de peso, e o retângulo à direita representa os sensores de temperatura e umidade. Eles serão protegidos com uma caixa de metal para que o sistema de escoamento não afete o desempenho do Arduino.

No espaço em branco, estará posicionado uma caixa com água e uma bomba de aquário próxima do retângulo verde, que será ativada quando o Relé for ativado, enviando sinais de energia para ela.

No desenvolvimento, utilizamos dois Arduinos, sendo um dedicado à recepção de dados dos sensores e o outro responsável pelo controle do atuador. O primeiro Arduino está equipado com o sensor de peso HX711 e o sensor de temperatura e umidade DHT22. Este Arduino mede o peso, a temperatura e a umidade da plataforma, que é projetada para escorrer a umidade para um recipiente onde será testada. Os dados coletados pelos sensores são então encaminhados para um ESP32 através do Node-RED. No ESP32, um atuador é responsável por transmitir energia para uma bomba de aquário, representada pelo BCC.

No Arduino de sensores, há um LED azul que acende para indicar que as informações estão sendo enviadas. No outro Arduino, há um LED vermelho que acende para indicar que a informação foi recebida e está sendo executada. No Node-RED, uma configuração compara os dados recebidos com os dados da API. Se os dados recebidos forem maiores do que os dados da API, uma mensagem é enviada para o atuador, que acende o LED vermelho por 7 segundos. Durante esse tempo, a transmissão de energia é passada para a bomba d'água, que opera por 7 segundos antes de ser desligada.

## 3. Resultados

Os resultados foram obtidos através de testes realizados nas plataformas Wokwi, Red Node e Grafana, já que não foram adquiridas as ferramentas utilizadas fisicamente. O projeto em si mostrou ser funcional. Porém, devido à falta de material para ser testado fisicamente e em um ambiente de testes onde tragam diferentes resultados para sabermos se ele realmente funcionará na vida real e se será funcional.

## 4. Conclusões

Pode-se concluir que o projeto apresenta uma boa opção para aqueles que planejam evitar a presença de excrementos em um ambiente público. 
Os resultados obtidos indicam que a plataforma automatizada para tratamento de dejetos de animais é eficaz e funcional. O projeto mostrou ser eficiente porém não foi possível visualizar na vida real, em um ambiente de teste, para saber se ele realmente funcionará e será benéfico. 
As principais vantagens do projeto incluem a automação do processo de tratamento de dejetos, a precisão na detecção e atuação, e a integração com tecnologias acessíveis e de baixo custo. Além disso, promove a sustentabilidade ao gerenciar recursos naturais de forma eficiente.

## 5. Referências
- Arduino. (n.d.). Ultrasonic Sensor - HC-SR04. Retrieved from https://www.arduino.cc/en/Tutorial/UltrasoundSensor
- Li, Y., & Yang, L. (2020). Smart animal waste management system based on IoT and edge computing. 2020 IEEE 6th International Conference on Computer and Communications (ICCC), 2361-2366.
- Monk, S. (2016). Programming Arduino: Getting Started with Sketches. New York: McGraw-Hill Education.
- Zhu, Z., Wang, W., & Xie, L. (2018). An intelligent animal waste disposal system based on Internet of Things. 2018 37th Chinese Control Conference (CCC), 6946-6950.

Links:

Aqui estão os links em Markdown:

- [Vídeo de apresentação no YouTube](https://www.youtube.com/watch?v=fEfWyhlYuFs)
- [Wokwi Publisher](https://wokwi.com/projects/398954874165918721)
- [Wokwi Subscriber](https://wokwi.com/projects/399073607167111169)
