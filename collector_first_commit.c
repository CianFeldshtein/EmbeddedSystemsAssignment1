/**
 * \file
 *         Collector for assesment project
 *         Commit 1: Setup broadcast
 * \author
 *         Cian Feldshtein
 *	   	   cian.feldshtein@mycit.ie
 */

#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/

//Declare the process
PROCESS(broadcast_process, "Broadcast");

//Make the process start when the module is loaded
AUTOSTART_PROCESSES(&broadcast_process);

/*---------------------------------------------------------------------------*/


//Connections
static struct broadcast_conn broadcast;


static const struct broadcast_callbacks broadcast_call = {};

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(broadcast_process, ev, data)
{
	
	// etimer, used to take temperature and light readings as set times
	static struct etimer etimer;

	//On process exit, close both broadcast and unicast connections
 	PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

 	PROCESS_BEGIN();
	
	//Open connections
 	broadcast_open(&broadcast, 123, &broadcast_call);	

  	while(1) {

    	//Set time for etimer
		etimer_set(&etimer, 2200);
		

    		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etimer));

		    //Packet that will be sent to sensors
	    	packetbuf_copyfrom("SensorInformation", 18);

		    //Broadcast packet to all sensors
	    	broadcast_send(&broadcast);
		
		    //Print message that packet has been sent
	    	printf("broadcast message sent\n");
		
		//Reset etimer
		etimer_reset(&etimer);
		
 	}

  	PROCESS_END();
}
/*---------------------------------------------------------------------------*/

