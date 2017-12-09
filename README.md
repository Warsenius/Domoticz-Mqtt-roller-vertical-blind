# Domoticz-Mqtt-roller-vertical-blind
wifi controlled window blinds using mqtt controlled by domoticz

This is my automated window blinds project that lets Domoticz open and close my vertical blinds.
A steppermotor is controlled by a Wemos D1 mini which receives its commands from Domoticz using MQTT over wifi.

A MQTT server is needed for Domoticz and the Wemos board to connect to.
To set this up please see the the [Domoticz wiki](https://www.domoticz.com/wiki/MQTT)


The LamellenStudie_Wemos file is uploaded to the Wemos board.


In domoticz a Virtual switch is created (named LamellenStudie in this code) of type Blinds Percentage.
This switch is assigned to a floorplan(example) and to a roomplan(Lamellen).
Domoticz will then send out any any changes done to the Virtual switch to the MQTT topic domoticz/out/example/Lamellen where the Wemos board will subscribe to.


## Connecting the parts

I am using the following parts:
⋅⋅* Wemos D1 mini
⋅⋅* EasyDriver - Stepper Motor Driver
⋅⋅* 12 to 5 volt converter [Fine](https://blog.yavilevich.com/2017/03/efficient-dc-12v-to-5v-conversion-for-low-power-electronics-evaluation-of-six-modules/)
⋅⋅* 2.1 DC power socket
⋅⋅* 17HS2408 4-lead Nema 17 Steppermotor
⋅⋅* N/O reed switch (normal open)
⋅⋅* [Parametric ball chain pulley](https://www.thingiverse.com/thing:3147)
⋅⋅* 3D printed enclosure
⋅⋅* Tesa powerstrip (to mount enclosure to wall)

The Easydriver board has a build in 12 to 5 volt converter, however it gets really warm and using the Fine converter lowered to idle power usage from 0.11 to 0.04 ampere (at 12volt)
The reed switch is optional, it works as a stop limit using a tiny magnet on the ball chain.

