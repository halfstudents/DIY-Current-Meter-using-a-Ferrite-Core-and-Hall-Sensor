# DIY-Current-Meter-using-a-Ferrite-Core-and-Hall-Sensor
A simple isolated current meter for measuring higher currents with good resolution based on hall effect sensor.
Here is the full article live on hackster: https://www.hackster.io/sainisagar7294/diy-current-meter-using-a-ferrite-core-and-hall-sensor-4a91aa

[Order PCBs from JLCPCB](https://jlcpcb.com/?from=audrey3)

I have used too many current and power measurement meters in the past. And I always want to build a better version of the previous one. Most of my work kept revolving around making measuring and test equipment, in the hope of someday creating a revolution in the testing world.  The most common method is using a shunt resistor, but it introduces voltage drop and power loss in the circuit.
I built a DC ammeter using a toroidal ferrite core and a Hall Effect sensor (WSH130NL). The idea is, when current flows through a wire wound around a toroidal core, it creates a magnetic field inside the core. For AC measurements we can just use a secondary winding and get a pretty good step down transformer producing an effective voltage which can be converted into current values. But with a DC steady signal we have to measure the magnetic flux in a different manner.

![Image](https://github.com/user-attachments/assets/6649c7b9-c5e2-4b1e-9363-2ba841a90ef8)

By cutting a gap in the toroidal core and placing a Hall Effect sensor in that gap.  With this we can measure the magnetic flux density, which is directly proportional to the current flowing through the windings. I know this is the same thing which is used in clamp meters and that’s how the idea comes to my mind. Overall now with this approach we get an isolated, non-invasive current measurement. And by choosing the right number of windings, you can adjust the sensitivity and resolution of your ammeter to suit your needs. The same can be converted later with the help of a PCB provided by JLCPCB. As I got my MCU dev board fabricated from there in a $2 price.

**How Does It Work?**

The working principle is based on Ampere's Law and the Hall Effect. Let me break it down step by step. When current passes through a conductor wound around a toroidal ferrite core, it generates a magnetic field inside the core. The key takeaway here is that magnetic flux density (Tesla) is directly proportional to both number of turns and  current. So if we keep N fixed, the magnetic flux density becomes a linear function of the current.

The ferrite has a very high permeability, which means the magnetic field is mostly confined inside the core. To measure it, we need to introduce a small air gap in the toroidal core. When we cut a gap in the core, the magnetic flux that was flowing through the ferrite now has to cross this air gap. By placing a Hall Effect sensor right in this air gap, the sensor sits in the path of the magnetic flux. The Hall Effect sensor generates a voltage proportional to the magnetic field passing through it.

**WSH130NL Sensor:**
Better the sensor more linear will be the output with good resolution. I am using this breadkout module board which is available in the same price of the sensor.

Wide supply voltage range (2.4V-26V)

Temperature compensation

Small TO-92S package

Fast response - sub-microsecond switching times

Low power consumption - only 2mA typical

The sensor includes on-chip temperature compensation, so as temperature changes can shift the magnetic characteristics of both the ferrite core and the sensor itself.

Components Required:

WSH130NL Hall Effect Sensor

Toroidal Ferrite Core

Enamelled Copper Wire

10K Ohm Resistor

0.1uF Capacitor

Arduino Uno/Nano (To get digital readings)

16x2 LCD Display I2C (To get digital readings)

Breadboard & Jumper Wires

Connecting Wires

Hacksaw / Dremel

Sandpaper

Super Glue / Epoxy

**Building the DC Ammeter - Step by Ste**p


The toroidal ferrite core is the heart of this ammeter. You can salvage one from an old power supply, a common-mode choke, or buy one online. The size doesn't matter too much, but a larger core gives you more room to wind wires.

**Cutting the Air Gap:**

![Image](https://github.com/user-attachments/assets/68d47dbc-60ee-484e-87e8-b9ee29fdbcb6)

This is the most critical and tricky step. We need to cut a small gap in the toroid where the Hall sensor will sit. Let me tell you what I did, I just took a plier and broke it into two pieces. This may be non ideal but I can later glue the parts to fix it. You can use a Dremel with a thin cutting disc or a fine hacksaw blade. Cut slowly and steadily. After cutting, use fine sandpaper to smooth both cut faces. The smoother the surfaces, the smaller the effective air gap, which means better flux coupling to the sensor. The air gap should be as small as practically possible while still fitting the sensor. A larger gap means more flux leakage and lower sensitivity.

**Winding the Toroid:**

I used 10 turns of 24 AWG enamelled copper wire on a ferrite core.The number of windings (N) acts as a multiplier for the magnetic field. More windings mean a stronger magnetic field for the same current. Higher output voltage from the Hall sensor for the same current.nBetter resolution because small changes in current produce larger changes in voltage. Easier ADC conversion because the voltage swing is larger. For example, if 10 turns at 1A gives you 0.5V output, then 20 turns at 1A might give you 1.0V output. The higher voltage is easier to read with an Arduino ADC and gives you finer measurement steps.

![Image](https://github.com/user-attachments/assets/2c93b39e-e774-4055-b38f-e636f050e657)

However, too many windings can saturate the ferrite core at higher currents, and the core has a limited physical size. So you need to balance the number of windings with your expected current range.

**Placing the Hall Effect Sensor**
You can place the sensor, make sure it is flat in the middle of the gap, then secure it using hot glue and some tape. The orientation decides the polarity, but it doesn’t matter even in this case because the steady state sensor readings are common mode by VDD/2. We will talk about it in the measurements section. Wire connections are really simple:

![Image](https://github.com/user-attachments/assets/6115227c-a8d9-4dd3-8a11-188f633b7f5e)

Pin 1 (VDD) : 5V supply

Pin 2 (GND)

Pin 3 (VOUT)

**Taking First Reading:**

The sensor output pin produces a voltage proportional to the current flowing through the windings. Now to measure this voltage either can use a multimeter and then convert the readings to current values manually. I did it by flowing a constant current from my lab bench power supply and then writing down the voltage at 1A, 2A and 3A. Which gives me an idea how the readings change.

For example at steady state, when no current is flowing the output voltage is VDD/2 + offset. When the current starts flowing it changes by some value, decreasing or increasing based on how the sensor is placed and in what direction current is flowing. Which means it is capable of measuring bidirectional current.

**Steady State:**

![Image](https://github.com/user-attachments/assets/b0a39bcb-6421-44b1-ad22-d2f4481a1b6d)


**At 1A Current:**

![Image](https://github.com/user-attachments/assets/2a2f7ae2-0919-4654-93b9-6c2a2e4b15c7)


As you can see, the output is linear,  the voltage increases proportionally with current. This is exactly what we expected from the theory. The linearity is key because it means we can use a simple linear equation to convert voltage back to current or just do it in excel by measuring slope.
The other method is to use an Arduino and get rid of all the conversions, Still you need to calibrate it which is explained in the section below.

**How to Calibrate:**
1. Upload the code to your Arduino with SENSITIVITY set to 0 initially
2. With no current flowing, note the voltage reading  this is your V_OFFSET.
3. Pass a known current (use a bench supply and a reference multimeter) of say 1A through the windings, and note the voltage.
4. Pass another known current of 3A, note the voltage
5. Calculate sensitivity:
SENSITIVITY = (V_at_3A - V_at_1A) / (3.0 - 1.0)
6. Update the V_OFFSET and SENSITIVITY defines in the code
7. Re-upload and verify against known current values
   
**Working:**

**At 1A:**

![Image](https://github.com/user-attachments/assets/2a2f7ae2-0919-4654-93b9-6c2a2e4b15c7)

**At 2A:**

![Image](https://github.com/user-attachments/assets/23955064-4cef-4296-881a-01c088f2f363)

**At: 3A:**

![Image](https://github.com/user-attachments/assets/e591187f-45b7-45d8-b856-c01e9a694edc)

The Arduino Uno has a 10-bit ADC, which means it can resolve 1024 discrete voltage levels across its 0-5V reference range:
Resolution = 5V / 1024 = 4.88 mV per step

So if your sensor gives 0.5V per Ampere (sensitivity), the ADC can resolve:

Current resolution = 4.88mV / 50mV per A = 0.0976 A per step (about 100mA)
