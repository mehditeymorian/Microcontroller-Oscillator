# Microcontroller-Oscillator
A Microcontroller based Oscillator that produces a periodic, oscillating electronic signal, often a sine wave or a square wave. I used Stm32f microcontroller to complete this project. You can generate up to 8 different types of waves. Also Wave period and frequency are adjustable. You can find the complete report [here](https://github.com/mehditeymorian/Microcontroller-Oscillator/blob/master/Assets/report.pdf).

## How It Works
Microcontroller generate PWM and change its duty cycle frequently to shape the right wave. afterward, PWM goes through a LCF type T and then a LCF type L to clear the noises. Finally we have a nice and smooth wave. 😁

## Wave Types
- Sine Wave
- Square Wave
- Triangle Wave
- Sawtooth Wave
- Stair Wave (12)
- Rectified Sine Wave (14)
- Vertically Segmented Sine Wave (17)
- Sine Modulation of Square Wave (18)

![](https://github.com/mehditeymorian/Microcontroller-Oscillator/blob/master/Assets/waves-1.png)
![](https://github.com/mehditeymorian/Microcontroller-Oscillator/blob/master/Assets/waves-2.png)

![Wave presentations](https://github.com/mehditeymorian/Microcontroller-Oscillator/blob/master/Assets/wave-types.gif)

## Domain and Freuqnecy
You can adjust the domain from scale 4 to 10 which 10 being the highest and the bigest size. and the frequency is adjustable from 1hz to 20kHz.

![Wave adjustment](https://github.com/mehditeymorian/Microcontroller-Oscillator/blob/master/Assets/wave-properties.gif)

## Proteus Shematics
![](https://github.com/mehditeymorian/Microcontroller-Oscillator/blob/master/Assets/schematic.png)

### How to Run
load the hex file into the microcontroller in proteus file. set 96 and 97 for amilitude and frequency respectively in proteus file. and hit the run button.

## Contribution
Project is open for any contribution and futher development. Please feel free to contact me if you have any doubt or problem. here's [my email](mailto:mehditeymorian322@gmail.com). 🍻

## Acknowledgment
I would like to express my gratitude to amazing Fatemeh that helped me to complete this wonderful job. it wasn't possible without her.

