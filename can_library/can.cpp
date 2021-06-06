#include <global.h>
#include <Canbus.h>
#include <mcp2515_defs.h>
#include <mcp2515.h>
#include <defaults.h>
#include <can.h>
#include <Arduino.h>

float can_values[100];
bool canReady = 0;
// hi
int can_setup(){

  // Initialise MCP2515 CAN controller
  // Return 1 if success 0 if fail
  int result = mcp2515_init(0);
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(2), can_setready, FALLING);
  for (int i = 0; i < 100; i++) {
	  can_values[i]=420;
  }
  return result;
}

unsigned int _combined_data(uint8_t data[], size_t firstIndex)
{
    return (data[firstIndex] << 8) + data[firstIndex + 1];
}

void can_getvalue() {
 
    // Read Message
    tCAN r_message;
      if (mcp2515_get_message(&r_message)) 
      {
        switch(r_message.id) 
        { 
          case 0x65D: // brake temp front left, front right, rear left, rear right
            // brake temp front left
            can_values[ADDR_BRAKE_TEMP_FRONT_LEFT] = (float)_combined_data(r_message.data, 0)/100.0; //units: degrees C
			can_values[ADDR_BRAKE_TEMP_FRONT_RIGHT] = (float)_combined_data(r_message.data, 2)/100.0;
			can_values[ADDR_BRAKE_TEMP_REAR_LEFT] = (float)_combined_data(r_message.data, 4)/100.0;
			can_values[ADDR_BRAKE_TEMP_REAR_RIGHT] = (float)_combined_data(r_message.data, 6)/100.0;
			
          case 0x655: // brake pressure front, brake pressure rear, coolant pressure, power steering pressure           
            can_values[ADDR_BRAKE_PRESSURE_REAR] = (float)_combined_data(r_message.data, 2)/100.0; //units: Bar
			can_values[ADDR_BRAKE_PRESSURE_FRONT] = (float)_combined_data(r_message.data, 0)/100.0;

          case 0x648: // wheel speed front left, front right, back left, back right
			can_values[ADDR_WHEEL_SPEED_FRONT_LEFT] = (float)_combined_data(r_message.data, 0)*0.0621; //units: mph
			can_values[ADDR_WHEEL_SPEED_FRONT_RIGHT] = (float)_combined_data(r_message.data, 2)*0.0621;
			can_values[ADDR_WHEEL_SPEED_REAR_LEFT] = (float)_combined_data(r_message.data, 4)*0.0621;
			can_values[ADDR_WHEEL_SPEED_REAR_RIGHT] = (float)_combined_data(r_message.data, 6)*0.0621;
            
          case 0x640: // engine speed, throttle position, manifold air pressure, manifold air temperature
            // engine speed
			unsigned int RPM = _combined_data(r_message.data, 0);
			can_values[ADDR_ENGINE_SPEED] = (RPM != 0 && (RPM > 530 || RPM < 500))?RPM:can_values[50];//units: RPM
			can_values[ADDR_MANIFOLD_AIR_PRESSURE] = (float)_combined_data(r_message.data, 2)/10.0; //units: kPa
			can_values[ADDR_MANIFOLD_AIR_TEMP] = (float)_combined_data(r_message.data, 4)/10.0;//units: degrees C
			can_values[ADDR_THROTTLE_POSITION] = (float)_combined_data(r_message.data, 6)/10.0;//units: percent
			
		  case 0x641:
			can_values[ADDR_LAMBDA1] = (float)_combined_data(r_message.data, 2)/100.0; //units: Lambda

          case 0x649: 
			can_values[ADDR_ENGINE_OIL_TEMP] = r_message.data[1] + 40; //units: degrees C
			can_values[ADDR_COOLANT_TEMP] = r_message.data[0] + 40; //units: degrees C
		  case 0x110:
			int batV = r_message.data[6];
			can_values[ADDR_BAT_V] = (batV == 0)?(float)can_values[ADDR_BAT_V]/10.0:batV; //units: V

          case 0x644:
			can_values[ADDR_OIL_PRESSURE] = (float)_combined_data(r_message.data, 6)/10.0; //units: kPa
    
          case 0x64E: 
			can_values[ADDR_NEUTRAL_GEAR_SWITCH] = (r_message.data[3] & 0b10000000)/0b10000000; //bit 31
            
          case 0x64D:
			can_values[ADDR_GEAR] = r_message.data[6] & 0b00001111; //4 bits, 52-56
		  case 0x631:
			can_values[ADDR_CAR_STATUS_BIT] = r_message.data[0] & 0b00000001;
        }
      }
}

float can_read(unsigned int addr) {
	return can_values[addr];
}

void can_setready() {
	canReady = 1;
}

bool can_ready() {
	return canReady;
}