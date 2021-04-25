/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 * Description: Ethernet Interface Skeleton
 * Author: none
 * Create: 2013
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/dhcp.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/raw.h"
#include "netif/etharp.h"
#include "netif/driverif.h"

#if LWIP_PLC
#include "netif/driverif_plc.h"
#endif

#if LWIP_IEEE802154
#include "netif/driverif_ieee802154.h"
#endif

#if LWIP_6LOWPAN && ETH_6LOWPAN
#include "netif/lowpan6.h"
#endif

LWIP_STATIC err_t
driverif_output(struct netif *netif, struct pbuf *p)
{
  LWIP_DEBUGF(DRIVERIF_DEBUG, (("driverif_output : going to send packet. netif 0x%p, pbuf 0x%p, "
                              "packet_length %"U16_F"\n"), (void *)netif, (void *)p, p->tot_len));
#if PF_PKT_SUPPORT
  if (all_pkt_raw_pcbs != NULL) {
    p->flags = (u16_t)(p->flags & ~(PBUF_FLAG_LLMCAST | PBUF_FLAG_LLBCAST | PBUF_FLAG_HOST));
    p->flags |= PBUF_FLAG_OUTGOING;
    raw_pkt_input(p, netif, NULL);
  }
#endif

#if ETH_PAD_SIZE
  (void)pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
  if (netif->drv_send != NULL) {
    netif->drv_send(netif, p);
  }

#if ETH_PAD_SIZE
  (void)pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
  MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
  LINK_STATS_INC(link.xmit);

  return ERR_OK;
}

#if !PF_PKT_SUPPORT
static void
driverif_input_internal(struct netif *netif, struct pbuf *p)
{
  err_t ret = ERR_VAL;
  u16_t ethhdr_type;
  struct eth_hdr *ethhdr = NULL;
  ethhdr = (struct eth_hdr *)p->payload;
  ethhdr_type = ntohs(ethhdr->type);

  switch (ethhdr_type) {
    /* IP or ARP or 6lowpan packet? */
    case ETHTYPE_IP:
    case ETHTYPE_IPV6:
    case ETHTYPE_ARP:
    case ETHTYPE_6LOWPAN:
      LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input : received packet of type %"U16_F"\n", ethhdr_type));
      /* full packet send to tcpip_thread to process */
      if (netif->input != NULL) {
        ret = netif->input(p, netif);
      }

      if (ret != ERR_OK) {
        LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input: IP input error\n"));
        (void)pbuf_free(p);
        LINK_STATS_INC(link.drop);
        LINK_STATS_INC(link.link_rx_drop);
        if (ret == ERR_MEM) {
          MIB2_STATS_NETIF_INC(netif, ifinoverruns);
          LINK_STATS_INC(link.link_rx_overrun);
        }
      } else {
        LINK_STATS_INC(link.recv);
      }
      break;

    default:
      LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input : received packet is of unsupported type %"U16_F"\n", ethhdr_type));
      (void)pbuf_free(p);
      LINK_STATS_INC(link.drop);
      LINK_STATS_INC(link.link_rx_drop);
      break;
  }
}
#endif

/*
 * This function should be called by network driver to pass the input packet to LwIP.
 * Before calling this API, driver has to keep the packet in pbuf structure. Driver has to
 * call pbuf_alloc() with type as PBUF_RAM to create pbuf structure. Then driver
 * has to pass the pbuf structure to this API. This will add the pbuf into the TCPIP thread.
 * Once this packet is processed by TCPIP thread, pbuf will be freed. Driver is not required to
 * free the pbuf.
 *
 * @param netif the lwip network interface structure for this driverif
 * @param p packet in pbuf structure format
 */
void
driverif_input(struct netif *netif, struct pbuf *p)
{
#if PF_PKT_SUPPORT
#if (DRIVERIF_DEBUG & LWIP_DBG_OFF)
  u16_t ethhdr_type;
  struct eth_hdr *ethhdr = NULL;
#endif
#endif

  LWIP_ERROR("driverif_input : invalid arguments", ((netif != NULL) && (p != NULL)), return);

  LWIP_DEBUGF(DRIVERIF_DEBUG, (("driverif_input : going to receive input packet. netif 0x%p, pbuf 0x%p, "
                               "packet_length %"U16_F"\n"), (void *)netif, (void *)p, p->tot_len));

  /* points to packet payload, which starts with an Ethernet header */
  MIB2_STATS_NETIF_ADD(netif, ifinoctets, p->tot_len);

#if PF_PKT_SUPPORT
#if ((DRIVERIF_DEBUG & LWIP_DBG_OFF) != 0)
  ethhdr = (struct eth_hdr *)p->payload;
  ethhdr_type = ntohs(ethhdr->type);
  LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input : received packet of type %"U16_F" netif->input=%p\n", ethhdr_type,
                               netif->input));
#endif
  err_t ret = ERR_VAL;
  /* full packet send to tcpip_thread to process */
  if (netif->input != NULL) {
    ret = netif->input(p, netif);
  }
  if (ret != ERR_OK) {
    LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input: IP input error\n"));
    (void)pbuf_free(p);
    LINK_STATS_INC(link.drop);
    LINK_STATS_INC(link.link_rx_drop);
    if (ret == ERR_MEM) {
      LINK_STATS_INC(link.link_rx_overrun);
    }
  } else {
    LINK_STATS_INC(link.recv);
  }

#else
  driverif_input_internal(netif, p);
#endif /* PF_PKT_SUPPORT */

  LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input : received packet is processed\n"));
}

/*
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this driverif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM on Allocation Failure
 *         any other err_t on error
 */
err_t
driverif_init(struct netif *netif)
{
  u16_t link_layer_type;

  LWIP_ERROR("driverif_init : invalid arg netif", (netif != NULL), return ERR_ARG);

  link_layer_type = netif->link_layer_type;
  LWIP_ERROR("driverif_init : invalid link_layer_type in netif",
             ((link_layer_type == ETHERNET_DRIVER_IF) || (link_layer_type == WIFI_DRIVER_IF) ||
              (link_layer_type == PLC_DRIVER_IF) ||
              (link_layer_type == IEEE802154_DRIVER_IF)),
             return ERR_IF);

#if LWIP_PLC
  if (link_layer_type == PLC_DRIVER_IF) {
    return plcdriverif_init(netif);
  }
#endif

#if LWIP_IEEE802154
  if (link_layer_type == IEEE802154_DRIVER_IF) {
    return ieee802154driverif_init(netif);
  }
#endif

  LWIP_ERROR("driverif_init : netif hardware length is greater than maximum supported",
             (netif->hwaddr_len <= NETIF_MAX_HWADDR_LEN), return ERR_IF);

  LWIP_ERROR("driverif_init : drv_send is null", (netif->drv_send != NULL), return ERR_IF);

#if LWIP_NETIF_PROMISC
  LWIP_ERROR("driverif_init : drv_config is null", (netif->drv_config != NULL), return ERR_IF);
#endif

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  if (strncpy_s(netif->hostname, NETIF_HOSTNAME_MAX_LEN,
                LWIP_NETIF_HOSTNAME_DEFAULT, NETIF_HOSTNAME_MAX_LEN - 1) == EOK) {
    netif->hostname[NETIF_HOSTNAME_MAX_LEN - 1] = '\0';
  } else {
    LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_init: hostname %s in invalid\n", LWIP_NETIF_HOSTNAME_DEFAULT));
    netif->hostname[0] = '\0';
  }
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

  netif->output = etharp_output;
  netif->linkoutput = driverif_output;
#if LWIP_6LOWPAN && ETH_6LOWPAN
#if LWIP_ETH_6LOWPAN_ENABLE
  netif->enabled6lowpan = lwIP_TRUE;
#else
  netif->enabled6lowpan = lwIP_FALSE;
#endif
  (void)lowpan6_if_init(netif);
#endif

  if (!((netif->name[0] != '\0') && (netif->name[1] != '\0') && (netif->name[IFNAMSIZ - 1] == '\0'))) {
    if (link_layer_type == ETHERNET_DRIVER_IF) {
      (void)strncpy_s(netif->name, IFNAMSIZ, ETHERNET_IFNAME, IFNAMSIZ - 1);
    } else {
      (void)strncpy_s(netif->name, IFNAMSIZ, WIFI_IFNAME, IFNAMSIZ - 1);
    }
  }

  /* maximum transfer unit */
  netif->mtu = IP_FRAG_MAX_MTU;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP |
#if DRIVER_STATUS_CHECK
                 NETIF_FLAG_DRIVER_RDY |
#endif
#if LWIP_IGMP
                 NETIF_FLAG_IGMP |
#endif

  /**
   * @page RFC-2710 RFC-2710
   * @par Compliant Sections
   * Section 5. Node State Transition Diagram
   * @par Behavior Description
   * MLD messages are sent for multicast addresses whose scope is 2
   * (link-local), including Solicited-Node multicast addresses.\n
   * Behavior:Stack will send MLD6 report /Done to solicited node multicast address
   * if the LWIP_MLD6_ENABLE_MLD_ON_DAD is enabled. By default, this is disabled.
   */
  /* Enable sending MLD report /done for solicited address during neighbour discovery */
#if LWIP_IPV6 && LWIP_IPV6_MLD
#if LWIP_MLD6_ENABLE_MLD_ON_DAD
                 NETIF_FLAG_MLD6 |
#endif /* LWIP_MLD6_ENABLE_MLD_ON_DAD */
#endif
                 NETIF_FLAG_LINK_UP;
#if LWIP_NETIF_DEFAULT_LINK_DOWN
  netif->flags = netif->flags & (~NETIF_FLAG_LINK_UP);
#endif
#if DRIVER_STATUS_CHECK
  netif->waketime = -1;
#endif /* DRIVER_STATUS_CHECK */
  LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_init : Initialized netif 0x%p\n", (void *)netif));
  return ERR_OK;
}
