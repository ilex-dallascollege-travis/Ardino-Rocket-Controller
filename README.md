# Ardino-Rocket-Controller

# Arduino Pressure Regulator & Rocket Launch Countdown

## Overview
This Arduino program regulates a compressor output until a target pressure (psi) is reached, then initiates a rocket launch countdown sequence. It is designed for the Tinkercad circuit featuring a target pressure potentiometer (`A0`), simulated pressure sensor potentiometer, indicator LEDs, 16x2 LCD display, and keypad[span_1](start_span)[span_1](end_span).

## Number Input Rule
- **Without `0x` prefix:** Treated as **binary** (base 2), e.g., `1010` = `10`.
- **With `0x` (or `0X`) prefix:** Treated as **hexadecimal** (base 16), e.g., `0x0A` = `10`.
- Type the number into the Serial Monitor (set it to 9600 baud). If you don't type one, the `A0` knob sets the target instead.


## Workflow
1. Accept/read the target pressure (from `A0` potentiometer and/or numeric input using the binary/hex rule)[span_2](start_span)[span_2](end_span).
2. Monitor current pressure and keep the compressor active until target pressure is reached[span_3](start_span)[span_3](end_span).
3. Stop compressor regulation and run the rocket launch countdown on the LCD/LEDs, triggering the launch indicator at completion[span_4](start_span)[span_4](end_span).

