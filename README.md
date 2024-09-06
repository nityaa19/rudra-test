# rudra-test
To control a Sabertooth 2x60 motor driver using serial commands from an RC transmitter via an SBUS receiver, you need to translate the SBUS channel values into commands that the Sabertooth motor driver can understand. Specifically, you want to interpolate the channel values and send them to the motor driver to make the rover move forward or backward.

Here are the steps for the problem solving:
1. Understand the Sabertooth 2x60 Serial Command Format
2.  Interpolate Channel Value
3.  Code Implementation
