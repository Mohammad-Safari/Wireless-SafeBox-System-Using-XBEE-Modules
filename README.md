# Wireless-SafeBox-System-Using-XBEE-Modules

## Definition

Consider a bank that has three vaults. In order to ensure the security of these rooms, a password is required for each one Login is determined, and people are only allowed to enter after entering the correct password.
In this project, the board Arduino Mega 2560 and Xbee module is used to do the necessary implementations.

Project goals:
- Connecting two or more remote boards by Xbee module
- Write and read in EEPROM
- Use of Virtual Serial Port Driver

Required tools:
- Virtual Terminal or (LCD to display the status of each room)
- (keypad to enter the password)
- (Arduino Mega 2560 room management and central bank management)
- (EEPROM storing and retrieving login passwords)
- (Arduino IDE, management of peripheral devices by the board)
  
## Introducing Xbee and Zigbee

In Zigbee technology, it is designed in such a way that digital radio signals with low power (low power) in personal networks (PAN) is distributed with a small range and the bandwidth obtained is also low.
In exchange for loss of bandwidth, The transmission band and coverage range has an affordable price and consumes little energy.
Zigbee wireless network is cheaper than other wireless networks such as WiFi and Bluetooth 
and Its useful in networks in which data transmission is at a low rate and low energy consumption is required.
On the other hand, Zigbee compared to WiFi and Bluetooth has a lower data transfer speed.

**Zigbee** is a standard protocol for wireless networks and **Xbee** is a module that uses different protocols such as WiFi and Zigbee.
In this project, we will use Xbee / Xbee-PRO module and Zigbee protocol.


## Virtual Serial Port Driver

The Xbee module library is required and it must be added to Proteus.

Then the two Xbee modules should be connected using the Virtual Terminal.
In order to do this, Virtual Serial Port Driver is used and two different ports for two modules are considered.
[Here](https://www.theengineeringprojects.com/2016/01/xbee-library-proteus.html) is a guiding article about how it is done.

## Implementation

Consider a bank that has three vaults. To enter any of these rooms, the password must be entered first. There are the following rules for entering each room:

  1. The code of each door consists of five characters (each room has one door).
  2. If the password of a door is entered correctly, that door will remain open for a certain period of time (for example, 5 seconds), and then it will be closed (while the door is open, people can enter it without entering the password).
  3. If the password of one of the doors is entered incorrectly, all people will be directed out of all the rooms and all the doors will be locked.
  4. A maximum of three people can be present in each room at the same time (pay attention to people leaving the room and update the room status accordingly).
     
Note that password information must be stored by the central server, and only the head of the bank will be allowed to change passwords.
To save the passwords in the central part of the bank, the EEPROM connection to a board is used so that the passwords are not lost after the simulation is finished, and also the bank manager be able to change and set the password. 
Obviously, to access the EEPROM, a password is required to be entered so that only the bank administrator be allowed to enter and change passwords.
The rooms are not connected to each other, and each room only communicates with the central administration.
For communication between each room and central management, strictly just wireless communication and Xbee module are possible to use.
