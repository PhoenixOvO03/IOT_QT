
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define WIFI_SSID "qjnu" // 替换为WiFi名称
#define WIFI_PASSWORD "" // 替换为WiFi密码

// DHT11
#define DHTPIN D7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();

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

  // 未能读取，结束该次循环
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String postData = "{\"temp\": " + String(temperature);
  postData += ",\"humidity\": " + String(humidity) + "}";
  Serial.println(postData);

  // 创建HTTP请求并发送数据
  if (client.connect("10.130.12.157", 12345)) {
    HTTPClient http;  // 声明一个HTTPClient对象

    http.begin(client, "http://10.130.12.157:12345/esp8266"); // 指定目标URL
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
  }
  else
  {
    Serial.println("Error on connect");
  }

  delay(2000); // 等待2秒钟再次读取
}
