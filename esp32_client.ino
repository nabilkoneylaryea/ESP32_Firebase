#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// TODO: Define the following according to your setup
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define API_KEY ""
#define DATABASE_URL ""

// Feel free to change these pins according to your setup
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

void setup()
{
  pinMode(LED2_PIN, OUTPUT);
  ledcSetup(PWMChannel, freq, resolution);
  ledcAttachPin(LED1_PIN, PWMChannel);

  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    signupOK = true;
    Serial.println("Sign Up: OK");
  }
  else
  {
    Serial.printf("@s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (signupOK && (millis() - sendDataPrevMillis > 30000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    if (Firebase.ready())
    {
      runFirebaseOperations();
    }
  }
}

void runFirebaseOperations()
{
  ldrData = analogRead(/* TODO: Add correct pin */);
  voltage = (float)analogReadMilliVolts(/* TODO: Add correct pin */) / 1000;

  if (Firebase.setInt(fbdo, /* TODO: Add appropriate path to the LDR value */, ldrData))
  {
    Serial.println();
    Serial.print(ldrData);
    Serial.print(" - successfully saved to: " + fbdo.dataPath());
    Serial.println(" (" + fbdo.dataType() + ")");
  }
  else
  {
    Serial.println("FAILED: " + fbdo.errorReason());
  }

  if (Firebase.setFloat(fbdo, /* TODO: Add appropriate path to the LDR voltage */, voltage))
  {
    Serial.println();
    Serial.print(voltage);
    Serial.print(" - successfully saved to: " + fbdo.dataPath());
    Serial.println(" (" + fbdo.dataType() + ")");
  }
  else
  {
    Serial.println("FAILED: " + fbdo.errorReason());
  }

  if (Firebase.getInt(fbdo, /* TODO: Add appropriate path to the LED analog value */))
  {
    if (fbdo.dataType() == "int")
    {
      pwmValue = fbdo.intData();
      Serial.println("Successful READ from " + fbdo.dataPath() + ": " + pwmValue + " (" + fbdo.dataType() + ")");
      ledcWrite(PWMChannel, pwmValue);
    }
  }
  else
  {
    Serial.println("FAILED: " + fbdo.errorReason());
  }

  if (Firebase.getBool(fbdo, /* TODO: Add appropriate path to the LED digital value */))
  {
    if (fbdo.dataType() == "boolean")
    {
      ledStatus = fbdo.boolData();
      Serial.println("Successful READ from " + fbdo.dataPath() + ": " + ledStatus + " (" + fbdo.dataType() + ")");
      digitalWrite(LED2_PIN, ledStatus);
    }
  }
  else
  {
    Serial.println("FAILED: " + fbdo.errorReason());
  }
}
