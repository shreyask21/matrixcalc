# MatrixCalc
Simple matrix calculator with Arduino, Nokia 5110 (G)LCD (Phillips PCD8544) and a membrane keypad.

![splash](https://raw.githubusercontent.com/shreyask21/matrixcalc/master/img/s1.jpg "Splash Screen")

![Pic](https://raw.githubusercontent.com/shreyask21/matrixcalc/master/img/s2.jpg "Sample")

![Pic](https://raw.githubusercontent.com/shreyask21/matrixcalc/master/img/s3.jpg "Sample")

## Libraries:
The following Libraries are needed:

[Adafruit PCD8544](https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library)

[Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)

[Keypad](https://playground.arduino.cc/Code/Keypad/)

**You can also install them from the Arduino IDE library manager.**

![Libs](https://raw.githubusercontent.com/shreyask21/matrixcalc/master/img/s4.png "Library Manager")

## Connections:

_I have used Arduino nano for this but for other boards you can modify the pin config in the Arduino Sketch._

### Display:

| Display Pin   | Arduino Pin   |
| ------------- | ------------- |
|      RST      |       A3      |
|      CE       |       A2      |
|      DC       |       A1      |
|      DIN      |      D11      |
|      CLK      |       A5      |
|      VCC      |      3.3V     |
|      BL       |     GND/5V    |
|      GND      |      GND      |

### Keypad:

| Keypad Pin    | Arduino Pin   |
| ------------- | ------------- |
|      R1       |       D9      |
|      R2       |       D8      |
|      R3       |       D7      |
|      R4       |       D6      |
|      C1       |       D5      |
|      C2       |       D4      |
|      C3       |       D3      |
|      C4       |       D2      |

## Key Bindings:

|      Key      |    Function   |
| ------------- | ------------- |
|    0 to 9     |  Number Input/Menu Selection      |
|      *       |       Negate (After input)      |
|      #       |       Enter      |
|      C       |       Clear stored number      |