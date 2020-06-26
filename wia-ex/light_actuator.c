#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  0
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

static struct simple_udp_connection udp_conn;
int i = 1;

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(light_actuator_process, "Light actuator process");
AUTOSTART_PROCESSES(&light_actuator_process);
/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{

	if(*data == '1') {
		leds_toggle(LEDS_GREEN);
		if (i == 1) {
			LOG_INFO("Light turned on\n");
			i=0;
		} else {
			LOG_INFO("Light turned off\n");
			i=1;
		}
	}

}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(light_actuator_process, ev, data)
{
  PROCESS_BEGIN();
  //static struct etimer timer;

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);

  /*
  etimer_set(&timer, CLOCK_SECOND * 10);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  LOG_INFO("TURNING THE LIGHTS OFF");
  leds_off(LEDS_GREEN);
  etimer_reset(&timer);
  */

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
