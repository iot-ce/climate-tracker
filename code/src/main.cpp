#include <ArduinoHttpClient.h>
#include <WiFi.h>

// ESP32 pin GIOP36 (ADC0)
#define CARBON_SENSOR_PIN 36 

#include <dht11.h>

/////// WiFi Settings ///////
char ssid[] = WIFI_SSID
char pass[] = WIFI_PASSWORD;

// server address
char serverAddress[] = "SERVER_ADDRESS";  
int port = 9090;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

// temperature in Celsius
float tempC; 

// temperature in Fahrenheit
float tempF; 

int DHT11Temp = 0;
int DHT11Hum = 0;
const int DHT11pin = 23;

dht11 DHT;


void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  DHT.read(DHT11pin);
  DHT11Temp = DHT.temperature;
   DHT11Hum = DHT.humidity;

   int CarbonValue = analogRead(CARBON_SENSOR_PIN);
  
   // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
   Serial.print("Current Temp: " + DHT11Temp);
  Serial.println("making POST request");
  String postData = "{temperature:";
  String postData01 = "}";
  postData = postData + DHT11Temp + postData01;

   Serial.print("Current Hum: " + DHT11Hum);
  Serial.println("making POST request");
  String postHumidityData = "{humidity:";
  String postData02 = "}";
  postHumidityData = postHumidityData + DHT11Hum + postData02;

  Serial.print("Current Carbon: " + CarbonValue);
  Serial.println("making POST request");
  String postCarbonData = "{Carbon:";
  String postData03 = "}";
  postCarbonData = postCarbonData + CarbonValue + postData03;

  Serial.print("Current JSON BODY: " + postData);
  Serial.println("Current JSON BODY: " + postHumidityData);
  Serial.println("Current JSON BODY: " + postCarbonData);

  
  client.beginRequest();
  client.post("/api/v1/rk5Q0SyD34umcFIBcdGn/telemetry");
  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/json");
  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postData.length());
  client.sendHeader("X-CUSTOM-HEADER", "custom_value");
  client.endRequest();
  client.write((const byte*)postData.c_str(), postData.length());
  // note: the above line can also be achieved with the simpler line below:
  //client.print(postData);

   client.beginRequest();
  client.post("/api/v1/rk5Q0SyD34umcFIBcdGn/telemetry");
  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/json");
  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postHumidityData.length());
  client.sendHeader("X-CUSTOM-HEADER", "custom_value");
  client.endRequest();
  client.write((const byte*)postHumidityData.c_str(), postHumidityData.length());

 client.beginRequest();
  client.post("/api/v1/rk5Q0SyD34umcFIBcdGn/telemetry");
  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/json");
  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postCarbonData.length());
  client.sendHeader("X-CUSTOM-HEADER", "custom_value");
  client.endRequest();
  client.write((const byte*)postCarbonData.c_str(), postCarbonData.length());
 
  Serial.print("POST Status code: ");
  Serial.println(statusCode);
  Serial.print("POST Response: ");
  Serial.println(response);

  Serial.println("Wait five seconds");
  delay(5000);
}