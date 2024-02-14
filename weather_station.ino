
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "ESP32_MailClient.h"
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <Wire.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define DHTTYPE DHT11
#define DHTPIN 26
#define SCLPIN 22
#define SDAPIN 21
#define MISOPIN 19
#define MOSIPIN 23
#define SCKIN 18
#define CS 2
#define REDLED 33
#define GREENLED 32

File file;
DHT Dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 Bmp;

float Temperature;
float Humidity;
float Pressure;
float Altitude;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
bool card = false;

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "WE_";
const char* password = "AaSdFc1357&@&12";

// To send Emails using Gmail on port 465 (SSL)
#define emailSenderAccount "heshamayman653@gmail.com"
#define emailSenderPassword "kdah qacv jszf ofmc"
#define smtpServer "smtp.gmail.com"
#define smtpServerPort 465
#define emailSubject "[ALERT] ESP32 Temperature"

// Default Recipient Email Address
String inputMessage = "heba65564@gmail.com";
String enableEmailChecked = "checked";
String inputMessage2 = "true";
// Default Threshold Temperature Value
String inputMessage3 = "25.0";
String lastTemperature;
String lastHumidity;
String lastPressure;
String lastAltitude;


// HTML web page to handle 3 input fields (email_input, enable_email_input, threshold_input)
const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
<html>
<head>
  <title>Email Notification with Temperature</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
.desktop1-container {
  width: 100%;
  display: flex;
  overflow: auto;
  min-height: 100vh;
  align-items: center;
  flex-direction: column;
}
.desktop1-desktop1 {
  width: 100%;
  height: 1024px;
  display: flex;
  overflow: hidden;
  position: relative;
  align-items: center;
  flex-shrink: 0;
  background-size: cover;
  justify-content: flex-end;
  background-image: url('https://zapnito.com/wp-content/uploads/2021/07/andrew-haimerl-tg5brdLWoL8-unsplash-scaled-1-1280x800.jpg');
}
.desktop1-container1 {
  top: 438px;
  left: 70px;
  width: 430px;
  height: 366px;
  display: flex;
  position: absolute;
  align-items: flex-end;
  justify-content: flex-start;
}
.desktop1-text {
  top: 0px;
  left: 44px;
  color: #ffffff;
  width: 225px;
  height: auto;
  position: absolute;
  font-size: 32px;
  font-style: Regular;
  text-align: left;
  font-family: Roboto;
  font-weight: 400;
  line-height: normal;
  font-stretch: normal;
  text-decoration: none;
}
.desktop1-text2 {
  top: 110px;
  left: 44px;
  color: #ffffff;
  width: 375px;
  height: auto;
  position: absolute;
  font-size: 32px;
  font-style: Regular;
  text-align: left;
  font-family: Roboto;
  font-weight: 400;
  line-height: normal;
  font-stretch: normal;
  text-decoration: none;
}
.desktop1-textinput {
  top: 50px;
  flex: 1;
  left: 44px;
  width: 360px;
  height: 30px;
  position: absolute;
  border-radius: 10px;
  border: 0px;
  padding: 3px;
}
.desktop1-textinput1 {
  left: 44px;
  width: 360px;
  top: 160px;
  height: 30px;
  position: absolute;
  border-radius: 10px;
  border: 0px;
  padding: 3px;
}
.desktop1-text3 {
  left: 44px;
  color: #ffffff;
  width: 387px;
  top: 220px;
  height: auto;
  position: absolute;
  font-size: 32px;
  font-style: Regular;
  text-align: left;
  font-family: Roboto;
  font-weight: 400;
  line-height: normal;
  font-stretch: normal;
  text-decoration: none;
}
.desktop1-checkbox {
  left: 40px;
  width: 50px;
  top: 270px;
  height: 40px;
  position: absolute;
}
.desktop1-container2 {
  top: 215px;
  left: 968px;
  width: 295.7375183105469px;
  height: 193.29999923706055px;
  display: flex;
  position: absolute;
  align-items: center;
  justify-content: flex-end;
}
.desktop1-text4 {
  top: 0px;
  left: 10px;
  color: rgb(255, 255, 255);
  position: absolute;
  font-size: 42px;
  font-family: Roboto;
  font-weight: 400;
  line-height: 1.15;
  text-transform: none;
  text-decoration: none;
}
.desktop1-text5 {
  top: 70px;
  left: 10px;
  color: rgb(255, 255, 255);
  position: absolute;
  font-size: 42px;
  font-family: Roboto;
  font-weight: 400;
  line-height: 1.15;
  text-transform: none;
  text-decoration: none;
}
.desktop1-text6 {
  top: 140px;
  left: 10px;
  color: rgb(255, 255, 255);
  right: -12px;
  bottom: -13px;
  position: absolute;
  font-size: 42px;
  font-family: Roboto;
  font-weight: 400;
  line-height: 1.15;
  text-transform: none;
  text-decoration: none;
}
.desktop1-button {
  left: 120px;
  color: #ffffff;
  width: 360px;
  top: 800px;
  height: 40px;
  position: absolute;
  background-color: #D2366B;
  border-radius: 10px;
  border: 0px;
  cursor:pointer;
}

.desktop1-button:hover {
  background-color: #FB6F9E;
}

.desktop1-text7 {
  top: 193px;
  left: 106px;
  color: rgb(255, 255, 255);
  width: auto;
  height: auto;
  position: absolute;
  font-size: 128px;
  font-style: Bold;
  text-align: left;
  font-family: Roboto;
  font-weight: 700;
  line-height: normal;
  font-stretch: normal;
  text-decoration: none;
}
  </style>
</head>
<body>
<div>
  <link href="./desktop1.css" rel="stylesheet" />
  <div class="desktop1-container">
    <div class="desktop1-desktop1">
      <div class="desktop1-container1">
        <span class="desktop1-text"><span>Email Address</span></span>
        <form action="/get">
        <span class="desktop1-text2">Temperature Threshold</span>
        <input
          type="email" name="email_input" value="%EMAIL_INPUT%" required
          class="desktop1-textinput input"
        />
        <input
          type="number" step="0.1" name="threshold_input" value="%THRESHOLD%" required
          class="desktop1-textinput1 input"
        />
        <span class="desktop1-text3">Email Notfication</span>
        <input type="checkbox" name="enable_email_input" value="true" %ENABLE_EMAIL% class="desktop1-checkbox" />
      </div>
      <div class="desktop1-container2">
        <span class="desktop1-text4">Humidity: %HUMIDITY%</span>
        <span class="desktop1-text5">Altitude: %ALTITUDE%</span>
        <span class="desktop1-text6">Pressure: %PRESSURE%</span>
      </div>
      <input type="submit" value="Submit" class="desktop1-button input"></input>
      </form>
    </div>
    <span class="desktop1-text7">%TEMPERATURE% &deg;C</span>
  </div>
</div>
</body>
</html>
)rawliteral";

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

String readTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  Temperature = Dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  if (isnan(Temperature)) {
    Serial.println("Failed to read Temperature from DHT sensor!");
    return "--";
  } else {
    Serial.println(Temperature);
    return String(Temperature);
  }
}

String readHumidity() {
  Humidity = Dht.readHumidity();
  if (isnan(Humidity)) {
    Serial.println("Failed to read Humidity from DHT sensor!");
    return "--";
  } else {
    Serial.println(Humidity);
    return String(Humidity);
  }
}

String readPressure() {
  Pressure = Bmp.readPressure();
  if (isnan(Pressure)) {
    Serial.println("Failed to read Pressure from BMP sensor!");
    return "--";
  } else {
    Serial.println(Pressure);
    return String(Pressure);
  }
}

String readAltitude() {
  Altitude = Bmp.readAltitude();
  if (isnan(Altitude)) {
    Serial.println("Failed to read Altitude from BMP sensor!");
    return "--";
  } else {
    Serial.println(Altitude);
    return String(Altitude);
  }
}
String processor(const String& var) {
  if (var == "TEMPERATURE") {
    return lastTemperature;
  }
  if (var == "HUMIDITY") {
    return lastHumidity;
  }
  if (var == "PRESSURE") {
    return lastPressure;
  }
  if (var == "ALTITUDE") {
    return lastAltitude;
  } else if (var == "EMAIL_INPUT") {
    return inputMessage;
  } else if (var == "ENABLE_EMAIL") {
    return enableEmailChecked;
  } else if (var == "THRESHOLD") {
    return inputMessage3;
  }
  return String();
}

// Flag variable to keep track if email notification was sent or not
bool emailSent = false;

const char* PARAM_INPUT_1 = "email_input";
const char* PARAM_INPUT_2 = "enable_email_input";
const char* PARAM_INPUT_3 = "threshold_input";

// Interval between sensor readings. Learn more about timers: https://RandomNerdTutorials.com/esp32-pir-motion-sensor-interrupts-timers/
unsigned long previousMillis = 0;
const long interval = 5000;

// The Email Sending data object contains config and data to send
SMTPData smtpData;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  delay(1000);
  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  Serial.begin(115200);
  while (!card) {
    if (SD.begin()) {
      Serial.println("SD Card DONE!!");
      card = true;
      digitalWrite(REDLED, LOW);

    } else {
      Serial.println("SD Card FAILED!!");
      digitalWrite(REDLED, HIGH);
    }
  }
  Dht.begin();
  Bmp.begin();
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Receive an HTTP GET request at <ESP_IP>/get?email_input=<inputMessage>&enable_email_input=<inputMessage2>&threshold_input=<inputMessage3>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    // GET email_input value on <ESP_IP>/get?email_input=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      // GET enable_email_input value on <ESP_IP>/get?enable_email_input=<inputMessage2>
      if (request->hasParam(PARAM_INPUT_2)) {
        inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
        enableEmailChecked = "checked";
      } else {
        inputMessage2 = "false";
        enableEmailChecked = "";
      }
      // GET threshold_input value on <ESP_IP>/get?threshold_input=<inputMessage3>
      if (request->hasParam(PARAM_INPUT_3)) {
        inputMessage3 = request->getParam(PARAM_INPUT_3)->value();
      }
    } else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    Serial.println(inputMessage2);
    Serial.println(inputMessage3);
    request->send(200, "text/html", "HTTP GET request sent to your ESP.<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  //if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    lastTemperature = readTemperature();
    lastHumidity = readHumidity();
    lastPressure =readPressure();
    lastAltitude =readAltitude();
    file = SD.open("/Sensors.txt", FILE_APPEND);
     Temperature = lastTemperature.toFloat();
     Humidity = lastHumidity.toFloat();
     Pressure = lastPressure.toFloat();
     Altitude = lastAltitude.toFloat();
     if (currentMillis - previousMillis1 >= 1000 && file) {
    previousMillis1 = currentMillis;
    if (digitalRead(GREENLED) == HIGH) {
      digitalWrite(GREENLED, LOW);
      digitalWrite(REDLED, LOW);
    }

    else {
      digitalWrite(GREENLED, HIGH);
      digitalWrite(REDLED, LOW);
    }
  }
 if  (!file && currentMillis - previousMillis1 >= 1000 ) {
    if (digitalRead(REDLED) == HIGH) {
      digitalWrite(REDLED, LOW);
      digitalWrite(GREENLED, LOW);
    } else {
      digitalWrite(REDLED, HIGH);
      digitalWrite(GREENLED, LOW);
    }
  }
  if (currentMillis - previousMillis2 >= interval&&file) {
    previousMillis2 = currentMillis;
    Serial.println("Writing to the File .... ");
    file.print(Temperature);
    file.print("    ");
    file.print(Humidity);
    file.print("    ");
    file.print(Pressure);
    file.print("    ");
    file.print(Altitude);
    file.print("    ");
    file.println("");
  }
  file.close();


    // Check if temperature is above threshold and if it needs to send the Email alert
    if (Temperature > inputMessage3.toFloat() && inputMessage2 == "true" && !emailSent) {
      String emailMessage = String("Temperature above threshold. Current temperature: ") + String(Temperature) + String("C");
      if (sendEmailNotification(emailMessage)) {
        Serial.println(emailMessage);
        emailSent = true;
      } else {
        Serial.println("Email failed to send");
      }
    }
    // Check if temperature is below threshold and if it needs to send the Email alert
    else if ((Temperature < inputMessage3.toFloat()) && inputMessage2 == "true" && emailSent) {
      String emailMessage = String("Temperature below threshold. Current Temperature: ") + String(Temperature) + String(" C");
      if (sendEmailNotification(emailMessage)) {
        Serial.println(emailMessage);
        emailSent = false;
      } else {
        Serial.println("Email failed to send");
      }
    }
  //}
}

bool sendEmailNotification(String emailMessage) {
  // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  // Set the sender name and Email
  smtpData.setSender("ESP32", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject(emailSubject);

  // Set the message with HTML format
  smtpData.setMessage(emailMessage, true);

  // Add recipients
  smtpData.addRecipient(inputMessage);

  smtpData.setSendCallback(sendCallback);

  // Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData)) {
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    return false;
  }
  // Clear all data from Email object to free memory
  smtpData.empty();
  return true;
}

// Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}