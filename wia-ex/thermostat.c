#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "dev/leds.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

static struct simple_udp_connection udp_conn;
bool turned_on=false;

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(thermostat_process, "Thermostat process");
AUTOSTART_PROCESSES(&thermostat_process);
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
//  LOG_INFO("Received request '%.*s' from ", datalen, (char *) data);
//  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("\n"); 

  LOG_INFO("I received a %d", *data);
  /* TODO: receive/poll for value from temperature sensor */
  if (*data < 20) {
	  if (!turned_on) {
	  	LOG_INFO("Turning on the heater");
	  	leds_on(LEDS_RED);
		turned_on = true;
	  }
  } else if (*data > 23) {
	  if (turned_on) {
	  	LOG_INFO("Turning off the heater");
	  	leds_off(LEDS_RED);
		turned_on = false;
	  }
  } 

  /* TODO: implement (simple) thermostat logic */
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(thermostat_process, ev, data)
{
  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
