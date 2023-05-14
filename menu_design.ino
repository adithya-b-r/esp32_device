//OTA
#include <WiFi.h>
//#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

//oled
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//DHT
#include <DHT.h>

//Clock
#include <time.h>

const char* ssid = "freedata";
const char* password = "freedata";

AsyncWebServer server(80);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHT_PIN 15  //dht pin

byte lstate = LOW;
byte lbtn = LOW;
int buzzer;
int up, down, enter, back;
int selected = 0;
int entered = -1;

int buzzerPin = 25; //buzzer pin
int laserPin = 2; //laser pin
DHT dht(DHT_PIN, DHT11);  //declare

unsigned long lastTime = 0;

//Declaration of functions.
void menu_head();
void initmenu();
void wifimenu();
void clockmenu();
void buzzermenu();
void lasermenu();
void weathermenu();
void sysmenu();
void wifimenu();

void displaymenu(void) {
  int up = digitalRead(32);  
  int down = digitalRead(13); 
  int enter = digitalRead(4); 
  int back = digitalRead(12); 

  if (up == LOW) {
    if (selected == 0)
      selected = 4;
    else
      selected = selected - 1;
    if (entered != 0)
      delay(200);
  }
  else if (down == LOW) {
    if (selected > 3)
      selected = 0;
    else
      selected = selected + 1;

    if (entered != 0 )
      delay(200);
  }
  else if (enter == LOW) {
    entered = selected;
  }
  else if (back == LOW) {
    entered = -1;
  }
  const char *options[5] = {
    "Clock",
    "Buzzer",
    "Laser",
    "Weather",
    "System"
  };

  if (entered == -1) {
    initmenu();
    menu_head("Main Menu");
    for (int i = 0; i < 5; i++) {
      if (i == selected) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.println("> " + String(options[i]));
      } else if (i != selected) {
        display.setTextColor(SSD1306_WHITE);
        display.println(options[i]);
      }
    }
  }
  switch (entered) {
    case 0:
      clockmenu();
      break;
    case 1:
      buzzermenu();
      break;
    case 2:
      lasermenu();
      break;
    case 3:
      weathermenu();
      break;
    case 4:
      sysmenu();
      break;
  }
  display.display();
}

void setup(void) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    break;
  }

  //Time
  configTime(5.5 * 3600, 0, "pool.ntp.org");
  setenv("TZ", "IST-5:30", 1);
  tzset();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", "śHi! This is a sample response");
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();

  pinMode(32, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(2, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //SSD1306 allocation failed
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  eye();
  delay(200);
}

void loop(void) {
  displaymenu();
}

void initmenu() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
}

void clockmenu() {
  initmenu();
  menu_head("CLOCK");

  // Get the current time
  time_t now = time(nullptr);
  tm *t = localtime(&now);

  display.println("\n");

  display.setTextSize(2);
  int hour = (t->tm_hour > 12) ? (t->tm_hour - 12) : ((t->tm_hour == 0) ? 12 : t->tm_hour);
  //display.print(" ");
  display.print(" " + String(hour) + ":");
  //display.print(':');
  if (t->tm_min < 10) display.print('0'); // Add leading zero for single digit minutes
  display.print(String(t->tm_min) + ":");
  //display.print(':');
  if (t->tm_sec < 10) display.print('0'); // Add leading zero for single digit seconds
  display.print(String(t->tm_sec));
  display.setTextSize(1);
  display.print(String((t->tm_hour >= 12) ? " PM" : " AM"));
  //display.print((t->tm_hour >= 12) ? " PM" : " AM");

  delay(1000);  // wait for 1 sec
}

void lasermenu() {
  initmenu();
  menu_head("LASER");
  byte btn = digitalRead(14);

  if (btn != lbtn) {
    lbtn = btn;
    if (btn == LOW) {
      lstate = (lstate == HIGH) ? LOW : HIGH;
      digitalWrite(laserPin, lstate);
    }
  }

  if (lstate == 1)
    display.println("Status : On");
  else
    display.println("Status : Off");
}

void buzzermenu() {
  initmenu();
  menu_head("BUZZER");
  int buzzer = digitalRead(14);

  if (buzzer == LOW) {
    digitalWrite(buzzerPin, HIGH);
    display.println("Status : On");
  } else {
    digitalWrite(buzzerPin, LOW);
    display.println("Status : Off");
  }
}

void weathermenu() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum) || temp < 0 || hum > 101) {
    return;
  }

  initmenu();
  menu_head("WEATHER");
  display.println("");
  display.println("Temperature : " + String(temp) + "C");
  display.println("");
  display.println("Humidity : " + String(hum) + "%");
  delay(2000);
}

class sysinfo {
  public:
    void uptime() {
      unsigned long currentTime = millis();
      if (currentTime - lastTime > 1000) {
        lastTime = currentTime;
        display.print("Uptime : "+String(currentTime / 1000)+" sec");
        //display.print(currentTime / 1000);
        //display.print(" sec");
        delay(1000);
      }
    }
    void ip() {
      display.print("IP : ");
      if (WiFi.status() == WL_CONNECTED) {
        display.print(WiFi.localIP());
      } else {
        display.print("Connection Error");
      }
    }
    void con_wifi() {
      display.print("SSID : ");
      if (WiFi.status() != WL_CONNECTED) {
        display.print("Disconnected");
      } else {
        display.print(ssid);
      }
    }
};

void sysmenu() {
  sysinfo sys;

  initmenu();
  menu_head("SYSTEM");

  sys.ip();
  display.println("\n");
  sys.con_wifi();
  display.println("\n");
  sys.uptime();
}

void eye() {
  display.fillRoundRect(10, 10, 30, 50, 2, WHITE);
  display.fillRoundRect(88, 10, 30, 50, 2, WHITE);
  display.display();
  delay(1000);

  display.fillRoundRect(10, 10, 30, 50, 2, BLACK);
  display.fillRoundRect(88, 10, 30, 50, 2, BLACK);
  display.display();
  delay(300);
}

void menu_head(String mname) {
  display.setCursor(0, 5);
  display.setCursor((SCREEN_WIDTH - 6 * mname.length()) / 2, 0); //Used to center text (%formula%)
  display.println(mname);
  display.drawLine(0, 10, 127, 10, WHITE);
  display.println("");
}
