# autovelox
Stupid program for Arduino. For halloween I wanted to be a speed camera so here we are.

## Features

- Distance measurement with an HC-SR04 ultrasonic sensor
- Double white LED flash and buzzer "click" when an object is closer than the threshold
- Cooldown between flashes to avoid repeated triggers
- 16x2 LCD showing photo count and current distance
- Green LED when the system is active, red LED when it is off
- Push button to toggle the system on and off
- Serial monitor output for debugging

## Hardware

| Component | Quantity |
|---|---|
| Arduino Uno R3 (or compatible) | 1 |
| HC-SR04 ultrasonic sensor | 1 |
| 16x2 LCD (HD44780, parallel) | 1 |
| Potentiometer (10k, for LCD contrast) | 1 |
| White LED | 2 |
| Green LED | 1 |
| Red LED | 1 |
| 220 Ω resistor | 5 (4 LEDs + LCD backlight) |
| Active buzzer | 1 |
| Push button | 1 |
| Breadboard and jumper wires | - |
| USB cable | 1 |

## How It Works

1. The system starts in the active state (green LED on).
2. The ultrasonic sensor measures the distance every ~100 ms.
3. When an object is closer than `SOGLIA_CM` (default 100 cm) and the cooldown (`COOLDOWN_MS`, default 1.5 s) has passed, the system:
   - fires a double flash with both white LEDs,
   - plays a short beep on the buzzer,
   - increments the photo counter and shows it on the LCD.
4. Pressing the button toggles the system on and off. When off, the red LED lights up and the LCD shows "Sistema spento" (the system is off).

## Configuration

You can tweak these constants at the top of the sketch:

| Constant | Default | Description |
|---|---|---|
| `SOGLIA_CM` | `100` | Trigger distance in cm. Keep it between 2 and 400 cm (the sensor's reliable range). |
| `COOLDOWN_MS` | `1500` | Minimum time between two flashes, in milliseconds. |

## Wiring

### Ultrasonic sensor (HC-SR04)

| Sensor pin | Arduino |
|---|---|
| VCC | 5V |
| TRIG | Pin 9 |
| ECHO | Pin 10 |
| GND | GND |

### LCD 1602 (parallel)

| LCD pin | Arduino |
|---|---|
| VSS | GND |
| VDD | 5V |
| V0 | Center pin of potentiometer (other pins to 5V and GND) |
| RS | Pin 12 |
| E | Pin 11 |
| D4 | Pin 5 |
| D5 | Pin 4 |
| D6 | Pin 3 |
| D7 | Pin 6 |
| RW | GND |
| A (backlight +) | 5V through 220 Ω |
| K (backlight -) | GND |

### LEDs

Each LED has its own 220 Ω resistor. The longer leg is the anode (+), which connects to the resistor. The cathode (-) goes to GND.

| LED | Arduino pin |
|---|---|
| Flash 1 (white) | Pin 8 |
| Flash 2 (white) | A0 |
| Active (green) | A1 |
| Standby (red) | A2 |

### Other components

| Component | Arduino pin |
|---|---|
| Buzzer | Pin 7 |
| Push button | Pin 2 (other leg to GND, uses internal pull-up) |

## Possible Improvements

- Add an I2C adapter to the LCD to save pins
- Store the photo counter in EEPROM so it survives a reset
- Add a 3D-printed or cardboard enclosure with a window for the flash
- Use a shift register (74HC595) to drive a row of LEDs
