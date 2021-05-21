#include <global.h>
#include <Canbus.h>
#include <mcp2515_defs.h>
#include <mcp2515.h>
#include <defaults.h>
#include <can.h>
#include <Arduino.h>

int can_setup(){

  // Initialise MCP2515 CAN controller
  // Return 1 if success 0 if fail
  int result = mcp2515_init(0);
  delay(1000);
  return result;
}

unsigned int _combined_data(uint8_t data[], size_t firstIndex)
{
    return (data[firstIndex] << 8) + data[firstIndex + 1];
}

unsigned int can_read(unsigned int addr) {
 
  while(1) {
    // Read Message
    tCAN r_message;
    if (mcp2515_check_message()) 
    {
      if (mcp2515_get_message(&r_message)) 
      {
        switch(r_message.id) 
        { 
          case 0x65D: // brake temp front left, front right, rear left, rear right
            // brake temp front left
            switch(addr)
            {
                case 2:  // brake temp front left
                    return _combined_data(r_message.data, 0);
                case 3:  // brake temp front right
                    return _combined_data(r_message.data, 2);
                case 4: // brake temp rear left
                    return _combined_data(r_message.data, 4);
                case 5: // brake temp rear right
                    return _combined_data(r_message.data, 6);
                    
            }
            break;
          case 0x655: // brake pressure front, brake pressure rear, coolant pressure, power steering pressure           
            
            switch(addr)
            {
                case 0: // brake pressure rear
                    return _combined_data(r_message.data, 2);
                case 1: // brake pressure front
                    return _combined_data(r_message.data, 0);          
            }

          case 0x648: // wheel speed front left, front right, back left, back right

            switch(addr)
            {
                case 10: // wheel speed front left
                    return _combined_data(r_message.data, 0);
                case 11: // wheel speed front right
                    return _combined_data(r_message.data, 2);
                case 12: // wheel speed rear left
                    return _combined_data(r_message.data, 4);
                case 13: // wheel speed rear right
                    return _combined_data(r_message.data, 6);
            }
            
          case 0x640: // engine speed, throttle position, manifold air pressure, manifold air temperature
            // engine speed
            switch(addr)
            {
                case 50: // engine speed
                    return _combined_data(r_message.data, 0);
                case 51: // manifold air pressure
                    return _combined_data(r_message.data, 2);
                case 52: // manifold air temp
                    return _combined_data(r_message.data, 4);
                case 31: // throttle position
                    return _combined_data(r_message.data, 6);
            }
            
            

          case 0x649: 
            switch(addr)
            {
                case 21: // engine oil temp
                    return r_message.data[1];
                case 53: // coolant temp
                    return r_message.data[0];
            }

          case 0x644:
            switch(addr)
            {
                case 20: // oil pressure
                    return _combined_data(r_message.data, 6);
            }
    
          case 0x64E: 
            switch(addr)
            {
                case 30: // neutral gear switch
                    return r_message.data[4] & 128; // deci number for 1000 0000

            }
            
          case 0x64D:
            switch(addr)
            {
                case 32: // gear
                    return r_message.data[4] & 0b00001111;
            }
        }
      }
    }
    else
    {
      return 0b1111111111111111;
    }
  }
}
