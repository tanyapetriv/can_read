#ifndef CAN_H
#define CAN_H

#include <global.h>
#include <Canbus.h>
#include <mcp2515_defs.h>
#include <mcp2515.h>
#include <defaults.h>
#include <Arduino.h>



int can_setup();
unsigned int can_read(unsigned int adr);

#endif
