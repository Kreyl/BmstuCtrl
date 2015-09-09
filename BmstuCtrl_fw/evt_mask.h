/*
 * evt_mask.h
 *
 *  Created on: Apr 12, 2013
 *      Author: g.kruglov
 */

#ifndef EVT_MASK_H_
#define EVT_MASK_H_

// Event masks
#define EVTMSK_PILL_CHECK       EVENT_MASK(2)
#define EVTMSK_MEASURE_TIME     EVENT_MASK(4)
#define EVTMSK_MEASUREMENT_DONE EVENT_MASK(5)
#define EVTMSK_UART_RX_POLL     EVENT_MASK(6)


/* Radio and Mesh */
#define EVTMSK_MESH_TX          EVENT_MASK(7)
#define EVTMSK_MESH_RX          EVENT_MASK(8)
#define EVTMSK_MESH_NEW_CYC     EVENT_MASK(9)
#define EVTMSK_MESH_RX_END      EVENT_MASK(10)
#define EVTMSK_MESH_PREPARE     EVENT_MASK(11)
#define EVTMSK_MESH_PKT_ENDS    EVENT_MASK(12)

#define EVTMSK_SENS_TABLE_READY EVENT_MASK(13)

#endif /* EVT_MASK_H_ */
