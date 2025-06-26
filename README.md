# SmartRemoteESP32

Turn your ESP32 into a versatile smart-home hub: send IR commands to lights and TVs, wake your PC over LAN, and control everything via a secure, mobile-friendly web interface.


---

## Table of Contents

1. [Features](#features)
2. [Parts & Modules](#parts--modules)
3. [Wiring Diagram](#wiring-diagram)
4. [Software Setup](#software-setup)

   * [ESP32 Board Support](#esp32-board-support)
   * [Library Installation](#library-installation)
5. [Getting Started](#getting-started)

   * [Customize & Upload](#customize--upload)
   * [Capturing IR Codes](#capturing-ir-codes)
6. [Web Interface](#web-interface)
7. [Remote Access (Port Forwarding)](#remote-access-port-forwarding)
8. [Security Notes](#security-notes)
9. [Repository Structure](#repository-structure)
10. [License & Contribution](#license--contribution)

---

## Features

* **IR Control**: Send custom IR commands to lights, TVs, air conditioners, and more.
* **Wake-on-LAN**: Power on your PC remotely via Magic Packet.
* **Web UI**: Secure, mobile-friendly interface to control devices from any browser.
* **Extensible**: Easily add more buttons or integrate with home automation platforms.

## Parts & Modules

| Component             | Quantity | Notes                                              |
| --------------------- | -------- | -------------------------------------------------- |
| ESP32 Dev Board       | 1        | e.g. ESP32-WROOM-32, LilyGO T8 ESP32-S2 V1.1       |
| IR Transmitter Module | 1        | 5 V/VCC, GND, DAT (driven by ESP32 GPIO)           |
| 220 Ω resistor        | 1        | In series with IR LED (omit if module has onboard) |
| Breadboard & jumpers  | —        | For prototyping                                    |
| USB-C/Micro cable     | 1        | To program ESP32                                   |

> **Tip**: If your IR Tx module already includes a resistor, you can skip the external 220 Ω.

## Wiring Diagram

```text
ESP32 3V3  ──► IR Transmitter VCC
ESP32 GND  ──► IR Transmitter GND
ESP32 GPIO13 (D13) ──► IR Transmitter DAT
```

## Software Setup

You'll need Arduino IDE (1.8.19 or newer) and a few libraries.

### ESP32 Board Support

1. Open **File → Preferences**
2. Add this URL under **Additional Boards Manager URLs**:

   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Go to **Tools → Board → Boards Manager**, search for **esp32**, and click **Install**.

### Library Installation

Install the following via **Sketch → Include Library → Manage Libraries...**:

* **ESPAsyncWebServer**
* **AsyncTCP**
* **IRremoteESP8266**
* **WiFiUdp** (built-in)

## Getting Started

1. **Clone the Repository**

   ```bash
   git clone https://github.com/yourusername/SmartRemoteESP32.git
   cd SmartRemoteESP32
   ```

2. **Open the Sketch**

   * File → Open → `SmartRemoteESP32.ino`

### Customize & Upload

At the top of `SmartRemoteESP32.ino`, update your network and credentials:

```cpp
const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* loginUser = "admin";
const char* loginPass = "admin";
// MAC address of PC for Wake-on-LAN
byte mac[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
// (Optional) Replace these with your captured IR pulse arrays:
uint16_t raw_on[]  = { /* … */ };
uint16_t raw_off[] = { /* … */ };
uint16_t raw_tv[]  = { /* … */ };
```

* **Tools → Board**: Select **ESP32 Dev Module** (or your variant)
* Choose the correct COM port
* Click **Upload**

### Capturing IR Codes

Use a Flipper Zero or an IR receiver circuit to record raw IR pulse lengths.

1. Point your remote at the receiver
2. Capture pulses (e.g. with Flipper Zero Infrared → Capture)
3. Copy the printed durations into the `raw_…[]` arrays above

## Web Interface

1. After reboot, open **Serial Monitor** at 115200 baud
2. Note the IP address (e.g. `192.168.1.42`)
3. In your browser, navigate to:

   ```
   http://<ESP_IP>/
   ```
4. Log in with your **admin** credentials
5. Control devices:

   * **Light ON / OFF**
   * **TV POWER**
   * **Wake PC** (Magic Packet)

## Remote Access (Port Forwarding)

> **Warning**: Exposing your hub to the internet has security risks.

1. Log in to your router’s admin panel
2. Find **Port Forwarding** / **Virtual Server**
3. Forward **TCP port 80** to your ESP32’s local IP (e.g. `192.168.1.42`)
4. Save and reboot your router
5. Access remotely via:

   ```
   http://<YourPublicIP>/
   ```

* Use a dynamic DNS service if your ISP IP changes
* Consider an HTTPS proxy or VPN for secure access

## Security Notes

* **Change default passwords** immediately
* **Disable port forwarding** when not in use
* **Use VPN or HTTPS proxy** for remote control

## Repository Structure

```
/
├── Photos/               # Device and module images
├── SmartRemoteESP32.ino  # Main Arduino sketch
└── README.md             # This documentation
```

## License & Contribution

This project is released under the **MIT License**. Contributions, issues, and pull requests are welcome!

---

*Created by Velislav Haygarov*
