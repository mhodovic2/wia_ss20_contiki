#include "contiki.h"
/* TODO: complete imports, based on used "libraries/apps" */
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uiplib.h"

#include <stdio.h> /* For printf() */

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* TODO: define IP-addresses (and ports?) to be used */
/* e.g. light_switch address */
#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

static struct simple_udp_connection udp_conn;

/*---------------------------------------------------------------------------*/
PROCESS(light_switch_process, "Light switch process");
AUTOSTART_PROCESSES(&light_switch_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(light_switch_process, ev, data)
{
  static struct etimer timer;
  static char str[32];
  static unsigned count;
  uip_ip6addr_t dest_ipaddr;
  uiplib_ipaddrconv("fd00::c30c:0:0:5", &dest_ipaddr);

  PROCESS_BEGIN();

  /* set up connection to light actuator, e.g. UDP */
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
		  UDP_SERVER_PORT, NULL);

  /* register any callbacks if necessary */

  /* Setup a periodic timer that expires after 10 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 10);

  while(1) {
   
    /* TODO: Send message to light actuator */
	  if(NETSTACK_ROUTING.node_is_reachable()) {
		  LOG_INFO("Sending request %u to ", count);
		  LOG_INFO_6ADDR(&dest_ipaddr);
		  LOG_INFO("\n");
		  snprintf(str, sizeof(str), "%d", count);
		  simple_udp_sendto(&udp_conn, str, strlen(str), &dest_ipaddr);
	  }

    /* Wait for the periodic timer to expire and then restart the timer. */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    etimer_reset(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
