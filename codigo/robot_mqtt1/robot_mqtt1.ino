#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define  PWM1 D1
#define  PWM2 D2
#define  DA   D3
#define  DB   D4

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
  pinMode(DA, OUTPUT);
  pinMode(DB, OUTPUT);

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
  digitalWrite(DA, HIGH);
  digitalWrite(DB, HIGH);
}

void retroceder() {
  analogWrite(PWM1,pwm);
  analogWrite(PWM2,pwm);
  digitalWrite(DA, LOW);
  digitalWrite(DB, LOW);
}

void derecha(){
  analogWrite(PWM1,pwm);
  analogWrite(PWM2,pwm);
  digitalWrite(DA, HIGH);
  digitalWrite(DB, LOW);
  
  }

void izquierda(){
  analogWrite(PWM1,pwm);
  analogWrite(PWM2,pwm);
  digitalWrite(DA, LOW);
  digitalWrite(DB, HIGH);
  
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
