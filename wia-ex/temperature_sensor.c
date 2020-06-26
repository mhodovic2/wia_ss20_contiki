#include "contiki.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uiplib.h"

#include <stdio.h> /* For printf() */

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

static struct simple_udp_connection udp_conn;
int i = 0;
/*---------------------------------------------------------------------------*/
PROCESS(temperature_sensor_process, "Temperature sensor process");
AUTOSTART_PROCESSES(&temperature_sensor_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(temperature_sensor_process, ev, data)
{
  static struct etimer timer;
  static unsigned count[10] = {16, 19, 21, 24, 25, 20, 14, 15, 17, 21};
  static char str[32];
  uip_ip6addr_t dest_ipaddr;
  uiplib_ipaddrconv("fd00::c30c:0:0:2", &dest_ipaddr);

  PROCESS_BEGIN();

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, NULL);

  /* Setup a periodic timer that expires after 30 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 30);

  while(1) {
    /* TODO: Send message to thermostat */
    /* prefferably send semi-reasonable randomly fluctuating values */
    if(NETSTACK_ROUTING.node_is_reachable()) {
      /* Send to destination ip */
      LOG_INFO("Sending request %u to ", count[i]);
      LOG_INFO_6ADDR(&dest_ipaddr);
      LOG_INFO_("\n");

      snprintf(str, sizeof(str), "%d", count[i]);
      simple_udp_sendto(&udp_conn, &count[i], sizeof(count), &dest_ipaddr);
      if(i == 9) {
	      i=0;
      } else {
	      i++;
      }
    }

    /* Wait for the periodic timer to expire and then restart the timer. */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    etimer_reset(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
