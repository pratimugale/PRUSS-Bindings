# Multi-Channel Waveform Generator.

## Current Progress: 8-channel

Output Videos:
1. [https://www.youtube.com/watch?v=0COg9I88yyQ](https://www.youtube.com/watch?v=0COg9I88yyQ): Sine waves being at a 180 degree phase difference<br>
2. [://www.youtube.com/watch?v=WIY_7yH4kKo](https://www.youtube.com/watch?v=WIY_7yH4kKo): y = sin(x) and y = x periodic waveform

The sample values of two waveforms are interweaved together in the PRU SRAM.
PRU0 - Creates different waveforms on different channels using PWM
PRU1 - For sample values transfer.
