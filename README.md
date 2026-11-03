# RFID-Obstacle-avoidance-Robot

**🏆 1st Place Winner - Robomanthan Hackathon**

Most basic obstacle-avoiding chassis are "always-on" the moment they get power. For this hackathon, we built a 2WD autonomous rover with an integrated hardware security layer. The drive system remains completely deadlocked until an authorized RFID card is scanned. Once authenticated, it navigates its environment using real-time ultrasonic echolocation, and instantly shuts down when a designated kill-tag is scanned. 

## Hardware Stack
* **Microcontroller:** Arduino Uno R3
* **Access Control:** MFRC522 RFID Reader (13.56 MHz)
* **Vision:** HC-SR04 Ultrasonic Distance Sensor
* **Drive System:** 
  * L298N Dual H-Bridge Motor Driver
  * 2x BO Geared DC Motors with rubber wheels
  * Front castor wheel for 3-point balance
* **Power:** 2-cell 18650 Lithium-ion battery pack (7.4V)

## Pin Mapping & Wiring

| Component | Module Pin | Arduino Uno Pin | Power Connection |
| :--- | :--- | :--- | :--- |
| **L298N Driver** | IN1, IN2, IN3, IN4 | 5, 4, 3, 2 | 12V to Battery (+), GND to Battery (-) & Arduino GND |
| **HC-SR04 Sensor**| TRIG, ECHO | 6, 7 | 5V, GND |
| **RC522 RFID** | SDA(SS), SCK, MOSI, MISO, RST | 10, 13, 11, 12, 9 | **3.3V ONLY**, GND |

> **Note:** ENA and ENB on the L298N must have their hardware jumper caps left ON for max speed output. 

## Software & Setup

1. **Clone this repository** and open the `.ino` file in the Arduino IDE.
2. **Install the correct library:** 
   * Do NOT use the newer "v2" MFRC522 library. 
   * Install the classic `MFRC522` library by *GithubCommunity / miguelbalboa* via the IDE Library Manager, or download the ZIP directly from [their repo](https://github.com/miguelbalboa/rfid).
3. **Map your specific RFID tags:**
   * Run the `DumpInfo` example sketch included with the library to find your card's Hex UID.
   * Update the `whiteCardUID[]` and `blueTagUID[]` byte arrays at the top of the main code to match your physical cards.
4. **Upload and Run.**

## Known Quirks & Hardware Gotchas (Lessons Learned)

If you are replicating this build, pay attention to these hardware constraints we ran into during development:

* **Electromagnetic Interference (EMI) crashing the SPI bus:** BO motors generate a lot of electrical noise. When the motors run, voltage drops and EMI can cause the RC522 reader to freeze. 
  * *The Fix:* Keep motor wires physically separated from the SPI jumper lines, and include `mfrc522.PCD_Init();` at the start of your `loop()` to continuously force the reader awake.
* **Strict 3.3V Logic:** The RC522 is highly sensitive. Supplying it with 5V from the Arduino will instantly and permanently burn out the module. 
* **Header Pin Integrity:** If your reader is failing to initialize (throwing a `Firmware 0x80` error in the serial monitor), check your soldering. Push-fit jumper wires on unsoldered header pins will fail under the vibration of the driving chassis. 
* **Common Grounding:** The L298N motor driver and the Arduino Uno MUST share a ground wire, otherwise the PWM/Digital signals sent to IN1-IN4 will float and the wheels will refuse to spin.

## Project showcase
* Working prototype:
https://github.com/user-attachments/assets/202b29a1-d7bd-4d69-a10a-2546b1914843
* The Robot:
<img width="1200" height="1600" alt="RFID robot" src="https://github.com/user-attachments/assets/4c0e41a5-242f-46a7-bd6b-7d62ab4287b5" />
* Top view:
<img width="1600" height="1200" alt="RFID robot top view" src="https://github.com/user-attachments/assets/66f0b6d9-3f4b-4769-a51b-b7175d018a38" />


## Contributing
Feel free to open an issue or submit a pull request if you have ideas on adding PID control for smoother turning or upgrading to an ESP32 for remote logging.
