# Waterplant 🪴

## About
This is a Cloud service school project with ambition to enlarge my home automation without using Home Assistant. In this project I will use an ESP82 with the help of a soil sensor to send mqtt messages to AWS IoT Core and store it in DynamoDB, I will then visualize my data with Python Flask web application/server to see when I need to water my avocado plant 🥑.

## Overview
My first thought was to use an ESP8266-01 with a 3V, 230mAh battery (CR2032) and enable deep sleep mode which only uses ~20uA (according to ESP8266 datasheet). But I realized pretty soon that i had to solder the tiny tiny GPIO 16 on ESP8266EX chip with ESP8266-01 reset pin to use deep sleep mode. And also A0 to use analog input. (reference below)
![esp-01-deepsleep](reference/deepsleep_pin.png)
So I bought a Wemos D1 Mini v3 instead because of good price, good size and it has A0, RST & the 16 pin so I could use the soil sensor and deep sleep to save battery. I calculated that with the CT2032 battery it would last for 59 days if ESP only sent moist mqtt message 3 times a day. But the Wemos D1 Mini returns ```rst cause:5``` when waking up from deep sleep mode and after some research I found it to be either because of bad soldering or bad wiring, and I had both. So instead of batteries I use power from USB (because battery would only last like 3 hours without deep sleep) and have an internal timer that sends mqtt messages every half hour instead of three times a day.