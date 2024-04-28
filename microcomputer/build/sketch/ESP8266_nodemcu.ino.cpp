#include <Arduino.h>
#line 1 "C:\\Users\\29853\\Documents\\Qt\\IOT_QT\\microcomputer\\ESP8266_nodemcu\\ESP8266_nodemcu.ino"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define WIFI_SSID "kang" // 替换为WiFi名称
#define WIFI_PASSWORD "1234567890" // 替换为WiFi密码
#define SERVER_URL "192.168.84.45" // 替换为服务器接收数据的URL

// DHT11
#define DHTPIN D7
#define DHTTYPE DHT11
// 光敏传感器
#define ldrPin A0
// 土壤湿度
// #define Moisture D2
#define DO D5

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

#line 22 "C:\\Users\\29853\\Documents\\Qt\\IOT_QT\\microcomputer\\ESP8266_nodemcu\\ESP8266_nodemcu.ino"
void setup();
#line 43 "C:\\Users\\29853\\Documents\\Qt\\IOT_QT\\microcomputer\\ESP8266_nodemcu\\ESP8266_nodemcu.ino"
void loop();
#line 22 "C:\\Users\\29853\\Documents\\Qt\\IOT_QT\\microcomputer\\ESP8266_nodemcu\\ESP8266_nodemcu.ino"
void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();

  pinMode(ldrPin, INPUT);
  // pinMode(Moisture, INPUT);
  pinMode(DO, INPUT);

  Serial.println("连接到WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi连接成功");
}

void loop() {
  // 读取湿度和温度值
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // 温湿度
  Serial.print("温度: ");
  Serial.println(temperature);
  Serial.print(" 湿度: ");
  Serial.println(humidity);

  // 光敏传感器
  int ldrValue = analogRead(ldrPin); //读取模拟口A5的值，存入变量中
  Serial.print("光敏：");
  Serial.println(ldrValue); //输出gmValue的值到串口监视器

  // 土壤湿度
  Serial.print("土壤湿度：");
  // Serial.print(analogRead(Moisture));
  // Serial.print("|DO=");
  Serial.println(digitalRead(DO));

  String postData = "{\"temp\": " + String(temperature);
  postData += ",\"humidity\": " + String(humidity);
  postData += ",\"light\": " + String(ldrValue);
  postData += ",\"soil_humi\": " + String(digitalRead(D0)) + "}";
  Serial.println(postData);

  // 创建HTTP请求并发送数据
  if (client.connect(SERVER_URL, 3000)) {
    HTTPClient http;  // 声明一个HTTPClient对象

    http.begin(client, "http://192.168.84.45:3000/api/common/json"); // 指定目标URL
    http.addHeader("Content-Type", "application/json"); // 设置HTTP头，指定内容类型为JSON
    // 发送POST请求
    int httpCode = http.POST(postData);

    // 检查请求是否成功
    if (httpCode > 0) {
      String payload = http.getString(); // 获取响应体
      Serial.println(httpCode);
      Serial.println(payload);
    } else {
      Serial.print("Error on HTTP request:");
      Serial.println(httpCode);
    }

    http.end(); // 结束HTTP连接
    // client.println(postData);
  }
  else
  {
    Serial.println("Error on connect");
  }

  delay(2000); // 等待2秒钟再次读取
}

