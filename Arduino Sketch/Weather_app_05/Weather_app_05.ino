#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <Arduino_JSON.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define DHTPIN 19
#define DHTTYPE DHT11
#define LED_GRN_PIN 12
#define LED_RED_PIN 13
#define LED_BLUE_PIN 14
#define API_KEY "AIzaSyCgnEdZXauMbo61T168dZ8Py_8VHgwXST8"
#define DATABASE_URL "https://simple-weather-station-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define SERVER_IP "192.168.231.172"
#define WIFI_SSID "note8pro"
#define WIFI_PASSWORD "wenaskara45"


DHT dht11(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
String URL = "http://" + String(SERVER_IP) + "/dht11_project/test_data.php";

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String location = "Boralesgamuwa";

int temperature = 0;
int humidity = 0;
int pressure = 0;
bool sensorOn = false;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  pinMode(LED_GRN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);

  dht11.begin();
  bmp.begin(0x76);

  Serial.begin(115200);
  connectWiFi();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase Signup ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  Serial.println("Start --------------------------------------------------------------");
  checkState("OnOff");
  checkState("Configuration");

  if (sensorOn) {
    digitalWrite(LED_GRN_PIN, HIGH);
    digitalWrite(LED_RED_PIN, LOW);

    if (WiFi.status() != WL_CONNECTED) {
      connectWiFi();
    }

    LoadDHT11Data();
    LoadBMP280Data();
    Blink();

    String postData = "temperature=" + String(temperature) +
                      "&humidity=" + String(humidity) +
                      "&pressure=" + String(pressure);

    HTTPClient http;
    http.begin(URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST(postData);
    String payload = http.getString();

    Serial.print("URL :");
    Serial.println(URL);
    Serial.print("Data:");
    Serial.println(postData);
    Serial.print("httpCode :");
    Serial.println(httpCode);
    Serial.print("payload :");
    Serial.println(payload);
    Serial.println("----------------------------------");

    float fbTemperature = dht11.readTemperature();
    float fbHumidity = dht11.readHumidity();
    float fbPressure = bmp.readPressure() / 100;

    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
      Blink();
      sendDataPrevMillis = millis();

      if (Firebase.RTDB.setInt(&fbdo, "DHT_11/Temperature", fbTemperature) &&
          Firebase.RTDB.setFloat(&fbdo, "DHT_11/Humidity", fbHumidity) &&
          Firebase.RTDB.setFloat(&fbdo, "DHT_11/Pressure", fbPressure) &&
          Firebase.RTDB.setString(&fbdo, "DHT_11/location", location)) {
        Serial.println("Firebase Data Sent Successfully");
      } else {
        Serial.println("Failed to Send Data to Firebase");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }
    Serial.println("End -----------------------------------------------------------------");
  } else {
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_GRN_PIN, LOW);
  }
}

void checkState(String stateType) {
  String postData = "";
  String payload = "";
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    int httpCode;
    postData = "id=1";
    payload = "";

    Serial.println();
    Serial.println("---------------getdata.php");
    if (stateType == "OnOff") {
      http.begin("http://" + String(SERVER_IP) + "/dht11_project/onoffstate.php");
    }
    if (stateType == "Configuration") {
      http.begin("http://" + String(SERVER_IP) + "/dht11_project/configstate.php");
    }
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    httpCode = http.POST(postData);
    payload = http.getString();

    Serial.print("httpCode : ");
    Serial.println(httpCode);
    Serial.print("payload  : ");
    Serial.println(payload);

    http.end();
    Serial.println("---------------");

    JSONVar myObject = JSON.parse(payload);

    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      Serial.println("---------------");
      return;
    }

    if (stateType == "OnOff") {
      if (myObject.hasOwnProperty("State")) {
        Serial.print("Sensor State = ");
        Serial.println(myObject["State"]);
      }

      if (strcmp(myObject["State"], "1") == 0) {
        sensorOn = true;
        Serial.println("Sensor ON");
      }
      if (strcmp(myObject["State"], "0") == 0) {
        sensorOn = false;
        Serial.println("Sensor OFF");
      }
    }

    if (stateType == "Configuration") {
      if (myObject.hasOwnProperty("Location")) {
        Serial.print("Location State = ");
        Serial.println(myObject["Location"]);
        location = String(static_cast<const char*>(myObject["Location"]));
      }
    }
  }
  delay(1000);
}

void Blink() {
  digitalWrite(LED_BLUE_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_BLUE_PIN, LOW);
  delay(1000);
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected to: ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void LoadBMP280Data() {
  pressure = bmp.readPressure() / 100;

  if (isnan(pressure)) {
    Serial.println("Failed to read data from BMP280 sensor!");
    pressure = 0;
  }

  Serial.printf("Pressure: %d hPa\n", pressure);
}

void LoadDHT11Data() {
  temperature = dht11.readTemperature();
  humidity = dht11.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read data from DHT11 sensor!");
    temperature = 0;
    humidity = 0;
  }

  Serial.printf("Temperature: %d °C\n", temperature);
  Serial.printf("Humidity: %d %%\n", humidity);
}


