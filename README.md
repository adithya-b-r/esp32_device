# ESP32 Weather Station with OTA Updates

Thi project is a weather station built using an ESP32 microcontroller. It provides various features such as displaying the time, controlling a buzzer, toggling a laser, and presenting weather information. Additionally, it includes over-the-air (OTA) update functionality to easily update the firmware wirelessly.

## Dependencies

- WiFi.h
- ESPAsyncWebServer.h
- Adafruit_GFX.h
- Adafruit_SSD1306.h
- DHT.h
- time.h

Make sure to install these libraries before compiling the code.

## Hardware Requirements

- ESP32 microcontroller
- OLED display (SSD1306)
- DHT11 sensor
- Buzzer
- Laser module
- Various push buttons

## Wiring Instructions

Connect the components according to your hardware specifications. Ensure that the OLED display, DHT11 sensor, buzzer, laser module, and buttons are properly wired to the ESP32.

## Configuration

- Set the WiFi credentials in the `ssid` and `password` variables.
- Adjust pin assignments for buttons, buzzer, laser, and DHT sensor as needed.
- Customize the menu options and appearance in the `displaymenu()`, `menu_head()`, and individual menu functions.

## Usage

1. Upload the code to your ESP32.
2. Open the Serial Monitor to view the ESP32's IP address.
3. Access the web interface by navigating to the provided IP address in a web browser.
4. Use the displayed menu to interact with various features: Clock, Buzzer, Laser, Weather, and System.
5. Explore the weather information by connecting a DHT11 sensor.
6. Perform OTA updates by uploading new firmware versions via the web interface.

## OTA Update Instructions

This project utilizes the [AsyncElegantOTA](https://github.com/ayushsharma82/AsyncElegantOTA) library for OTA updates. Follow these steps to perform OTA updates:

1. Ensure your ESP32 is connected to the same WiFi network as your computer.
2. Upload the new firmware binary using the web interface.
3. The ESP32 will automatically update itself, and the new firmware will be applied.

## Additional Notes

- The project includes a simple eye animation during startup for a visual indication.
- The clock displays the current time, and the weather menu shows temperature and humidity information.
- The system menu provides details about the ESP32's IP address, WiFi connection status, and uptime.

Feel free to customize and expand upon this project to suit your specific needs!
