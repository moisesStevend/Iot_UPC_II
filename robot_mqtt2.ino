#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define  PWM1 D5
#define  PWM2 D6
#define  DA_1   D1
#define  DA_2   D2
#define  DB_1   D3
#define  DB_2   D4

const char* ssid = "HUAWEI-1605";
const char* password = "75985170";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int pwm=500;

void setup() {
  pinMode(PWM1, OUTPUT);     
  pinMode(PWM2, OUTPUT);
  pinMode(DA_1, OUTPUT);
  pinMode(DA_2, OUTPUT);
  pinMode(DB_1, OUTPUT);
  pinMode(DB_2, OUTPUT);

  analogWrite(PWM1,0);
  analogWrite(PWM2,0);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void avanzar() {
  analogWrite(PWM1,pwm);
  analogWrite(PWM2,pwm);
  digitalWrite(DA_1, HIGH);
  digitalWrite(DA_2, LOW);
  digitalWrite(DB_1, HIGH);
  digitalWrite(DB_2, LOW);
}

void retroceder() {
  analogWrite(PWM1,pwm);
  analogWrite(PWM2,pwm);
  digitalWrite(DA_1, LOW);
  digitalWrite(DA_2, HIGH);
  digitalWrite(DB_1, LOW);
  digitalWrite(DB_2, HIGH);
}

void derecha(){
  analogWrite(PWM1,pwm);
  analogWrite(PWM2,pwm);
  digitalWrite(DA_1, HIGH);
  digitalWrite(DA_2, LOW);
  digitalWrite(DB_1, LOW);
  digitalWrite(DA_1, HIGH);
  
  }

void izquierda(){
  analogWrite(PWM1,pwm);
  analogWrite(PWM2,pwm);
  digitalWrite(DA_1, LOW);
  digitalWrite(DA_2, HIGH);
  digitalWrite(DB_1, HIGH);
  digitalWrite(DA_2, LOW);
  
  }

void  stop(){
    analogWrite(PWM1,0);
    analogWrite(PWM2,0);
  }

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == 'A') {
    avanzar();
  } else if ((char)payload[0] == 'R'){
    retroceder();
  } else if ((char)payload[0] == 'I'){
    izquierda();
  } else if ((char)payload[0] == 'D'){
    derecha();
  } else if ((char)payload[0] == 'S'){
    stop();
  }
  delay(200);
  stop();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Ubuntu16_MATE")) {
      Serial.println("connected");
      client.publish("outTopic", "hello world");
      client.subscribe("robot/labotec/1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
