# Arduino Pressure Regulator & Rocket Launch Countdown

## Overview
This Arduino program regulates a compressor output until a target pressure (PSI) is reached, then initiates a rocket launch countdown sequence. It is designed for a Tinkercad circuit featuring:
- Target pressure potentiometer (`A0`)
- Simulated pressure sensor potentiometer
- Indicator LEDs
- 16x2 LCD display
- Keypad

## Number Input Rule
- **Without `0x` prefix:** Treated as **binary** (base 2) – e.g., `1010` = `10` PSI.
- **With `0x` (or `0X`) prefix:** Treated as **hexadecimal** (base 16) – e.g., `0x0A` = `10` PSI.
- Type the number into the Serial Monitor (set it to 9600 baud). If you don't type one, the `A0` knob sets the target instead.

## Workflow
1. **Set Target Pressure:** Accept and read the target pressure from the `A0` potentiometer and/or keypad numeric input (following the binary/hex rule).
2. **Pressurize:** Monitor current pressure and keep the compressor active until the target pressure is reached.
3. **Launch Sequence:** Stop compressor regulation and run the rocket launch countdown on the LCD and LEDs, triggering the launch indicator upon completion.
