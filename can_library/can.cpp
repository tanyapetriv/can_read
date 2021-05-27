#include <global.h>
#include <Canbus.h>
#include <mcp2515_defs.h>
#include <mcp2515.h>
#include <defaults.h>
#include <can.h>
#include <Arduino.h>

unsigned int can_values[100];
bool canReady = 0;
// hi
int can_setup(){

  // Initialise MCP2515 CAN controller
  // Return 1 if success 0 if fail
  int result = mcp2515_init(0);
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(2), can_setready, FALLING);
  for (int i = 0; i < 100; i++) {
	  can_values[i]=0;
  }
  return result;
}

unsigned int _combined_data(uint8_t data[], size_t firstIndex)
{
    return (data[firstIndex] << 8) + data[firstIndex + 1];
}

void can_getvalue() {
	canReady = 0;
    // Read Message
    tCAN r_message;
      if (mcp2515_get_message(&r_message)) 
      {
        switch(r_message.id) 
        { 
          case 0x65D: // brake temp front left, front right, rear left, rear right
            // brake temp front left
            can_values[2] = _combined_data(r_message.data, 0);
			can_values[3] = _combined_data(r_message.data, 2);
			can_values[4] = _combined_data(r_message.data, 4);
			can_values[5] = _combined_data(r_message.data, 6);
			
          case 0x655: // brake pressure front, brake pressure rear, coolant pressure, power steering pressure           
            can_values[0] = _combined_data(r_message.data, 2);
			can_values[1] = _combined_data(r_message.data, 0);

          case 0x648: // wheel speed front left, front right, back left, back right
			can_values[10] = _combined_data(r_message.data, 0);
			can_values[11] = _combined_data(r_message.data, 2);
			can_values[12] = _combined_data(r_message.data, 4);
			can_values[13] = _combined_data(r_message.data, 6);
            
          case 0x640: // engine speed, throttle position, manifold air pressure, manifold air temperature
            // engine speed
			can_values[50] = _combined_data(r_message.data, 0);
			can_values[51] = _combined_data(r_message.data, 2);
			can_values[52] = _combined_data(r_message.data, 4);
			can_values[31] = _combined_data(r_message.data, 6);

          case 0x649: 
			can_values[21] = r_message.data[1];
			can_values[53] = r_message.data[0];

          case 0x644:
			can_values[20] = _combined_data(r_message.data, 6);
    
          case 0x64E: 
			can_values[30] = r_message.data[4] & 128; // deci number for 1000 0000
            
          case 0x64D:
			can_values[30] = r_message.data[4] & 0b00001111;
        }
      }
}

unsigned int can_read(unsigned int addr) {
	return can_values[addr];
}

void can_setready() {
	canReady = 1;
}

bool can_ready() {
	return canReady;
}