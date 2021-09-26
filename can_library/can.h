#ifndef CAN_H
#define CAN_H

#include <global.h>
#include <Canbus.h>
#include <mcp2515_defs.h>
#include <mcp2515.h>
#include <defaults.h>
#include <Arduino.h>

//unsigned int can_values[100];

#define ADDR_BRAKE_PRESSURE_REAR  0
#define ADDR_BRAKE_PRESSURE_FRONT 1

#define ADDR_BRAKE_TEMP_FRONT_LEFT  2
#define ADDR_BRAKE_TEMP_FRONT_RIGHT 3
#define ADDR_BRAKE_TEMP_REAR_LEFT   4
#define ADDR_BRAKE_TEMP_REAR_RIGHT  5

#define ADDR_WHEEL_SPEED_FRONT_LEFT  10
#define ADDR_WHEEL_SPEED_FRONT_RIGHT 11
#define ADDR_WHEEL_SPEED_REAR_LEFT   12
#define ADDR_WHEEL_SPEED_REAR_RIGHT  13

#define ADDR_OIL_PRESSURE 20

#define ADDR_ENGINE_OIL_TEMP 21
#define ADDR_BAT_V 22

#define ADDR_NEUTRAL_GEAR_SWITCH 30

#define ADDR_THROTTLE_POSITION     31

#define ADDR_GEAR 32

#define ADDR_LAMBDA1 40
#define ADDR_LAMBDA2 41

#define ADDR_ENGINE_SPEED          50
#define ADDR_MANIFOLD_AIR_PRESSURE 51
#define ADDR_MANIFOLD_AIR_TEMP     52

#define ADDR_COOLANT_TEMP 53

#define ADDR_CAR_STATUS_BIT 54

int can_setup();
float can_read(unsigned int adr);
void can_getvalue();
void can_setready();
bool can_ready();
#endif
