/**
 * \file
 *         Sensor for assessment
 *         Final version of sensor file
 * \author
 *         Cian Feldshtein
 *	       cian.feldshtein@mycit.ie
 */

#include "contiki.h"
#include "sht11-sensor.h"
#include "dev/light-sensor.h"
#include "net/rime/rime.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/

PROCESS(sensor_information_process, "Sensor Node");

AUTOSTART_PROCESSES(&sensor_information_process);

/*---------------------------------------------------------------------------*/
//stores 3 temp values at a time
static int values[3];

static struct unicast_conn unicast;
static const struct unicast_callbacks unicast_callbacks = {};

static struct broadcast_conn broadcast;


static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{

	packetbuf_clear();
	
	int val = 0;
	
	int i;
	//add the array
	for(i = 0; i < 3; i++){

		val = val + values[i];
	
	}

	//divided sum value by number of values for average
	val = val / 3;
	
	//convert our int average value to char array
	char value[5];
	sprintf(value, "%d", val);


	packetbuf_copyfrom(value, 5);
	//Test output of temperature and light for the sensor. Values are the average
	printf("SENSOR VALUES -- temp: %d\n", val);		
		
      	unicast_send(&unicast, from);


	
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};

//Define the process code
PROCESS_THREAD(sensor_information_process, ev, data)
{
	
	//etimer to tick down to read temperature
	static struct etimer etimer;

	//Variable to store temperature reading
	static int valTemp;

	//closes connections when process exits
	PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
	PROCESS_EXITHANDLER(unicast_close(&unicast);)

	PROCESS_BEGIN();
	
	//Open connections
	broadcast_open(&broadcast, 123, &broadcast_call);
	unicast_open(&unicast, 134, &unicast_callbacks);

	//populating array with 0 values
	int i;
	for(i = 0; i < 3; i++){

		values[i] = 0;
	}
	

	while(1){
		

		//Set etimer
		etimer_set(&etimer, 500);
		
		//Wait until timer is up to proceed
		PROCESS_WAIT_UNTIL(etimer_expired(&etimer));

		//Activate the sensors
		SENSORS_ACTIVATE(light_sensor);
		SENSORS_ACTIVATE(sht11_sensor);

		//Reading temperature
		valTemp = ((sht11_sensor.value(SHT11_SENSOR_TEMP) / 10) - 396) / 10;	

		//move all values 1 to the left of array and fill last position with new temp value
		for(i = 0; i < 3; i++){

			if(i +1 < 3){

				values[i] = values[i +1];
				
				// last index int the array is populated with the new temp reading
				if(i+1 == 2){
				
					values[i + 1] = valTemp; 
				}

			}

		}
	
		//Reset etimer
		etimer_reset(&etimer);

		//Deactivate sensors
		SENSORS_DEACTIVATE(light_sensor);
		SENSORS_DEACTIVATE(sht11_sensor);

	}

	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
