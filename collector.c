/**
 * \file
 *         Collector for assesment project
 *         Final version of collector file
 * \author
 *         Cian Feldshtein
 *	       cian.feldshtein@mycit.ie
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
static struct unicast_conn unicast;

static const struct broadcast_callbacks broadcast_call = {};

recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{
	
	printf("unicast message received from %d.%d: '%s'\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
	
	
}
//Execute recv_uc if unicast signal is received
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(broadcast_process, ev, data)
{
	
	//etimer, used to take temperature readings as set times
	static struct etimer etimer;

	//On process exit, close both broadcast and unicast connections
 	PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
	PROCESS_EXITHANDLER(broadcast_close(&unicast);)

 	PROCESS_BEGIN();
	
	//Open connections
 	broadcast_open(&broadcast, 123, &broadcast_call);
	unicast_open(&unicast, 134, &unicast_callbacks);	

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

