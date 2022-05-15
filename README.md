# Alexa Finger
## Description
A device that extends Alexa's presence in the physical world by enabling it to turn standard light switches on and off.

## Design
This project is made up of these three components:

1. A raspberrypi running a simple node red flow that listens for requests from Alexa, and then turns them into requests that are passed on to an ESP8266. Communication with Alexa is provided by the node described [here.](https://flows.nodered.org/node/node-red-contrib-alexa-local)

2. A ESP8266 that translates HTTP requests into servo motor controls, and the necessary power related electronics.

3. A 3D printed rack gear topped with two "finger" grippers that are spaced apart about the width of a standard light switch, a pinion gear, and a very basic housing containing these elements and the electronics above.

#### Note:
The Alexa communication node mentioned above suggests you run node red as root since Alexa sends to port 80, this is not the way. I routed requests received on port 80 to port 8080 by doing:

```bash
$ sudo iptables -I INPUT 1 -p tcp --dport 80 -j ACCEPT
$ sudo iptables -A PREROUTING -t nat -i wlan0 -p tcp --dport 80 -j REDIRECT --to-port 8080
```
And I installed `iptables-persistent` to persist this setting between reboots.
