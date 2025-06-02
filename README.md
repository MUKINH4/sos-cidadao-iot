# 🚨 SOS+ Cidadão – Estação de Emergência Comunitária

Sistema IoT desenvolvido com ESP32 para auxiliar moradores de áreas de risco em situações de desastres naturais, como enchentes e incêndios. O dispositivo monitora variáveis ambientais em tempo real e publica alertas via MQTT para visualização em um painel Node-RED.

---

## 👨‍💻 Integrantes

- Samuel Heitor – RM 556731
- Lucas Nicolini - RM 557613
- Renan Olivi - RM 557680

---

## 📦 Tecnologias Utilizadas

- **ESP32 (via Wokwi)**: microcontrolador central com conectividade Wi-Fi.
- **Node-RED**: visualização de dados e lógica de controle no painel.
- **MQTT (HiveMQ)**: protocolo leve para comunicação entre dispositivo e servidor.
- **Wokwi**: simulação online completa do circuito e sensores.

---

## 📚 Bibliotecas Utilizadas

### No Arduino (ESP32)
- `WiFi.h`: conecta à rede Wi-Fi simulada.
- `PubSubClient.h`: responsável por enviar mensagens MQTT.
- `ArduinoJson.h`: empacota dados dos sensores em JSON.

### No Node-RED
- `node-red-dashboard`: cria interface visual (gauges, textos).
- `mqtt in`, `json`, `function`, `ui_gauge`, `ui_text`: fluxos principais do painel.

---

## 🧩 Funcionalidades do Dispositivo

| Função                    | Componente           | Descrição                                                                 |
|---------------------------|----------------------|---------------------------------------------------------------------------|
| Medição de chuva          | Potenciômetro        | Simula sensor de chuva via leitura analógica                             |
| Detecção de fumaça        | Sensor MQ-2          | Detecta presença de fumaça ou gás                                         |
| Medição de alagamento     | HC-SR04              | Mede o nível da água em centímetros                                       |
| Alerta visual             | LED RGB (anodo)      | Vermelho (alerta), verde (normal)                                         |
| Alerta sonoro             | Buzzer               | Emite som durante situações críticas                                      |
| Botão manual              | Push-button          | Alterna entre modo automático e modo silencioso (manual)                 |
| Envio de dados            | MQTT                 | Publica JSON com status de sensores em tempo real                         |

---

## 🖥️ Exemplo do Dashboard

![Dashboard](./docs/dashboard.jpg)

---

## 🌐 Estrutura MQTT

- **Broker:** `broker.hivemq.com`
- **Porta:** `1883`
- **Tópico publicado:** `sos/emergencia`
- **Formato enviado:**
```json
{
  "chuva": true,
  "agua": 23,
  "fumaca": false,
  "botao": false
}
