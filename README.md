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

![finished product](https://github.com/Warsenius/Domoticz-Mqtt-roller-vertical-blind/blob/master/3d%20printed%20enclosure/with%20cover.jpg)

## Connecting the parts

I am using the following parts:
1. Wemos D1 mini
1. EasyDriver - Stepper Motor Driver
1. 12 to 5 volt converter [Fine](https://blog.yavilevich.com/2017/03/efficient-dc-12v-to-5v-conversion-for-low-power-electronics-evaluation-of-six-modules/)
1. 2.1 DC power socket
1. 17HS2408 4-lead Nema 17 Steppermotor
1. N/O reed switch (normal open)
1. [Parametric ball chain pulley](https://www.thingiverse.com/thing:3147)
1. [3D printed case](https://github.com/Warsenius/Domoticz-Mqtt-roller-vertical-blind/tree/master/3d%20printed%20enclosure)
1. Tesa powerstrip (to mount case to wall)

![Electronics](https://github.com/Warsenius/Domoticz-Mqtt-roller-vertical-blind/blob/master/electronics.jpg)

The Easydriver board has a build in 12 to 5 volt converter, however it gets really warm and using the Fine converter lowered to idle power usage from 0.11 to 0.04 ampere (at 12volt)

The reed switch is optional, it works as a stop limit using a tiny magnet on the ball chain.

## Follow the Sun

In domoticz i have set up the script "[script_device_blinds_follow_sun_script.lua](https://github.com/Warsenius/Domoticz-Mqtt-roller-vertical-blind/blob/master/script_device_blinds_follow_sun_script.lua)"
This will use the [angle of the sun](http://www.domoticz.com/wiki/Real-time_solar_data_without_any_hardware_sensor_:_azimuth,_Altitude,_Lux_sensor...) and [strenght of the sun](https://www.domoticz.com/wiki/Virtual_weather_devices) (pulled from Weather Underground) to turn the blinds.
It also changes the angle of the blinds if the tv\pc is on and checks if the 'Lamellen Handmatig' virtual switch is on (disabling the script)

the code include's a 'privacy' virtual switch which will close the blinds to position 2 or 14(depending on what is closest) when enabled.
