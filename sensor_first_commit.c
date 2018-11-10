/**
 * \file
 *         Sensor for Assesment
 *         Commit 1: Setup temperature reading
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

static int temp;
static int count;

//Define the process code
PROCESS_THREAD(sensor_information_process, ev, data)
{
	
	//etimer to tick down to read temperature
	static struct etimer etimer;

	//Variable to store temperature reading
	static int valTemp;

	PROCESS_BEGIN();
	

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

		//temp is a combination of current temp value and temperature read in. Added together and divided by a counter to get the average
		temp = temp + valTemp;

		//Count increments by 1 for every temperature read
		count = count + 1;
		
		//Reset etimer
		etimer_reset(&etimer);

		//Deactivate sensors
		SENSORS_DEACTIVATE(light_sensor);
		SENSORS_DEACTIVATE(sht11_sensor);

	}

	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
