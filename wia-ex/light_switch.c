#include "contiki.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uiplib.h"
#include "dev/button-sensor.h"

#include <stdio.h> 

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

static struct simple_udp_connection udp_conn;

/*---------------------------------------------------------------------------*/
PROCESS(light_switch_process, "Light switch process");
AUTOSTART_PROCESSES(&light_switch_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(light_switch_process, ev, data)
{
  static char str[32];
  static unsigned count = 1;
  uip_ip6addr_t dest_ipaddr;
  uiplib_ipaddrconv("fd00::c30c:0:0:5", &dest_ipaddr);

  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
		  UDP_SERVER_PORT, NULL);

  SENSORS_ACTIVATE(button_sensor);

  while(1) {
	
	PROCESS_WAIT_EVENT_UNTIL((ev==sensors_event) && (data == &button_sensor));
   
	if (NETSTACK_ROUTING.node_is_reachable()) {
		LOG_INFO("%u", count);
		LOG_INFO_6ADDR(&dest_ipaddr);
		LOG_INFO("\n");
		snprintf(str, sizeof(str), "%d", count);
		simple_udp_sendto(&udp_conn, str, strlen(str), &dest_ipaddr);
	}
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
