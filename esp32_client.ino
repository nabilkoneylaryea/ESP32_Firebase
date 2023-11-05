#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

/*
RTDB URL: https://esp32-rtdb-tutorial-default-rtdb.firebaseio.com/
API_KEY: AIzaSyDU7HjH3cAn_2EbQXs7O6JYTRYb1u3WYu0
*/

// #define WIFI_SSID "ufdevice"
// #define WIFI_PASSWORD "gogators"
#define WIFI_SSID "nabilsphone"
#define WIFI_PASSWORD "12345678"
#define API_KEY "AIzaSyDU7HjH3cAn_2EbQXs7O6JYTRYb1u3WYu0"
#define DATABASE_URL "https://esp32-rtdb-tutorial-default-rtdb.firebaseio.com/"

#define LED1_PIN 12
#define LED2_PIN 14
#define LDR_PIN 36
#define PWMChannel 0

const int freq = 5000;
const int resolution = 8;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int ldrData = 0;
float voltage = 0.0;
int pwmValue = 0;
bool ledStatus = false;

void setup() {
  pinMode(LED2_PIN, OUTPUT);
  ledcSetup(PWMChannel, freq, resolution);
  ledcAttachPin(LED1_PIN, PWMChannel);
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if(Firebase.signUp(&config, &auth, "", "")) {
    signupOK = true;
    Serial.println("Sign Up: OK");
  }
  else {
    Serial.printf("@s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(signupOK && (millis() - sendDataPrevMillis > 30000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    if(Firebase.ready()) {
      runFirebaseOperations();
    }
  }
}

void runFirebaseOperations() {
  ldrData = analogRead(LDR_PIN);
  voltage = (float)analogReadMilliVolts(LDR_PIN) / 1000;

  if(Firebase.setInt(fbdo, "Sensor/ldr_data", ldrData)) {
    Serial.println();
    Serial.print(ldrData);
    Serial.print(" - successfully saved to: " + fbdo.dataPath());
    Serial.println(" (" + fbdo.dataType() + ")");
  }
  else {
    Serial.println("FAILED: " + fbdo.errorReason());
  }

  if(Firebase.setFloat(fbdo, "Sensor/voltage", voltage)) {
    Serial.println();
    Serial.print(voltage);
    Serial.print(" - successfully saved to: " + fbdo.dataPath());
    Serial.println(" (" + fbdo.dataType() + ")");
  }
  else {
    Serial.println("FAILED: " + fbdo.errorReason());
  }

  if(Firebase.getInt(fbdo, "LED/analog")) {
    if(fbdo.dataType() == "int") {
      pwmValue = fbdo.intData();
      Serial.println("Successful READ from " + fbdo.dataPath() + ": " + pwmValue + " (" + fbdo.dataType() + ")");
      ledcWrite(PWMChannel, pwmValue);
    }
  }
  else {
    Serial.println("FAILED: " + fbdo.errorReason());
  }

  if(Firebase.getBool(fbdo, "LED/digital")) {
    if(fbdo.dataType() == "boolean") {
      ledStatus = fbdo.boolData();
      Serial.println("Successful READ from " + fbdo.dataPath() + ": " + ledStatus + " (" + fbdo.dataType() + ")");
      digitalWrite(LED2_PIN, ledStatus);
    }
  }
  else {
    Serial.println("FAILED: " + fbdo.errorReason());
  }
}


