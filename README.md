# a simple BLDC motor controller experiment
BLDC controller using the ATtiny24a microcontroller.
The motor's back-EMF is used to sense the rotor position and trigger the commutation.
A three-resistor star-network generates a virtual star-point for the motor, to measure the voltage across each phase using the analog comparator of the ATtiny24A. The comparator triggers an interrupt, which then triggers the next commutation.
This very simple approach works, but doesn't allow autonomous starting of the motor (i.e. you have to spin up the motor by hand for it tos 		start. This is because when the motor is at rest, there is no voltage inducted in the phases, thus we can't deduce the rotor position to trigger 	the commutation.

Schematic:
(https://raw.githubusercontent.com/MarcelMG/simple_BLDC_controller/master/bldc_schematic.png)
