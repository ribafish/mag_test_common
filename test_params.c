/*
 * test_params.c
 *
 *  Created on: Jul 6, 2018
 *      Author: gksolutions
 */

#include <string.h>
#include "stdbool.h"
#include "stdint.h"
#include "sdk_common.h"
#include "nrf.h"
#include "nrf_ble_gatt.h"
#include "nrf_error.h"
#include "ble.h"
#include "ble_gattc.h"
#include "ble_advdata.h"
#include "ble_err.h"
#include "ble_srv_common.h"
#include "ble_gap.h"
#include "ble_gatt.h"
#include "app_error.h"
#include "app_error_weak.h"


#include "test_params.h"
#include "ble_stack.h"
#include "utils.h"

// Debug header files
#include "debug.h"
#include "app_error.h"

#ifdef DEBUG
#undef DEBUG
#endif

#define DEBUG	1
#define debug_line(...)  do { if (DEBUG>0) { debug_line_global(__VA_ARGS__); debug_global("\n"); }} while (0)
#define debug_error(...)  do { if (DEBUG>0) { debug_errorline_global(__VA_ARGS__); debug_global("\n"); }} while (0)
#define debug_L2(...)  do { if (DEBUG>1) { debug_line_global(__VA_ARGS__); debug_global("\n"); }} while (0)
#define debug_data(...)  do { if (DEBUG>0) { debug_global(__VA_ARGS__); }} while (0)


#define LL_HEADER_LEN             4


void gatt_mtu_set(uint16_t att_mtu);
void conn_evt_len_ext_set(bool status);
void data_len_ext_set(test_params_t *params);
void tx_power_set(int8_t tx_power);

const char * ble_version_str[] = {"BLE 4.1", "BLE 4.2", "BLE 5 High Speed", "BLE 5 Long Range"};
const char * test_case_str[] = {"NULL", "Write", "Write no rsp", "Read", "Notify", "L2CAP Write", "L2CAP Read"};

/* Connection parameters requested for connection. */
static ble_gap_conn_params_t m_conn_param =
{
    .min_conn_interval = (uint16_t)CONN_INTERVAL_MIN,   // Minimum connection interval.
    .max_conn_interval = (uint16_t)CONN_INTERVAL_MAX,   // Maximum connection interval.
    .slave_latency     = (uint16_t)SLAVE_LATENCY,       // Slave latency.
    .conn_sup_timeout  = (uint16_t)CONN_SUP_TIMEOUT     // Supervisory timeout.
};

void gatt_mtu_set(uint16_t att_mtu)
{
    ret_code_t err_code;
    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, att_mtu);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_ble_gatt_att_mtu_central_set(&m_gatt, att_mtu);
    APP_ERROR_CHECK(err_code);
}


void conn_evt_len_ext_set(bool status)
{
    ret_code_t err_code;
    ble_opt_t  opt;

    memset(&opt, 0x00, sizeof(opt));
    opt.common_opt.conn_evt_ext.enable = status ? 1 : 0;

    err_code = sd_ble_opt_set(BLE_COMMON_OPT_CONN_EVT_EXT, &opt);
    debug_line("Setting connection event extension to %d: %x", opt.common_opt.conn_evt_ext.enable, err_code);
    APP_ERROR_CHECK(err_code);
}


void data_len_ext_set(test_params_t *params)
{
    ret_code_t err_code;

	uint16_t pdu_size = params->data_len_ext_enabled ?
        (params->att_mtu + LL_HEADER_LEN) : BLE_GATT_ATT_MTU_DEFAULT;

	if(pdu_size > NRF_BLE_GATT_MAX_MTU_SIZE)
	{
		pdu_size = NRF_BLE_GATT_MAX_MTU_SIZE;
	}

    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, pdu_size);

    debug_line("Setting DLE to %u: %x", pdu_size, err_code);
    APP_ERROR_CHECK(err_code);
}

void tx_power_set(int8_t tx_power)
{
	uint32_t err_code;
	err_code = sd_ble_gap_tx_power_set(tx_power);
	APP_ERROR_CHECK(err_code);
}

void test_params_set_all(test_params_t *params)
{
	uint32_t retval = 0;
	gatt_mtu_set(params->att_mtu);

	m_conn_param.max_conn_interval = MSEC_TO_UNITS(params->conn_interval, UNIT_1_25_MS);
	m_conn_param.min_conn_interval = MSEC_TO_UNITS(params->conn_interval, UNIT_1_25_MS);
	if (params->conn_interval > 100) {
		m_conn_param.conn_sup_timeout = (params->conn_interval * 2 + (params->conn_interval / 10))/10 + 1;		// divide by 10, as this is in 10ms units, add 1 to be at least one bigger
	} else {
		m_conn_param.conn_sup_timeout = 25;
	}
    retval = ble_stack_set_conn_param(&m_conn_param);

    if (retval != NRF_SUCCESS) {
    	debug_error("Failed setting connection parameters: %d", retval);
    }

    data_len_ext_set(params);
    conn_evt_len_ext_set(params->conn_evt_len_ext_enabled);
	tx_power_set(params->tx_power);

	switch(params->rxtx_phy)
	{
		case BLE_GAP_PHY_2MBPS:
			params->link_budget = params->tx_power + 92;
			break;
		case BLE_GAP_PHY_1MBPS:
			params->link_budget = params->tx_power + 96;
			break;
		case BLE_GAP_PHY_CODED:
			params->link_budget = params->tx_power + 103;
			break;
	}
}

void test_params_load(test_params_t * params, test_ble_version_t ble_version, test_case_t test_case) {
	params->test_case = test_case;
	params->ble_version = ble_version;
	switch(ble_version) {
	case BLE_4_1:
	    params->att_mtu                  	= 23;
	    params->conn_interval            	= 7.5f;
	    params->data_len_ext_enabled     	= false;
	    params->conn_evt_len_ext_enabled 	= false;
		params->rxtx_phy                 	= BLE_GAP_PHY_1MBPS;
		params->tx_power				  	= 4;
		params->transfer_data_size			= 100;
		params->link_budget					= 100;
		break;
	case BLE_4_2:
	    params->att_mtu                  	= 247;
	    params->conn_interval            	= 400.0f;
	    params->data_len_ext_enabled     	= true;
	    params->conn_evt_len_ext_enabled 	= true;
		params->rxtx_phy                 	= BLE_GAP_PHY_1MBPS;
		params->tx_power				  	= 4;
		params->transfer_data_size			= 512;
		params->link_budget					= 100;
		break;
	case BLE_5_HS:
	    params->att_mtu                  	= 247;
	    params->conn_interval            	= 400.0f;
	    params->data_len_ext_enabled     	= true;
	    params->conn_evt_len_ext_enabled 	= true;
		params->rxtx_phy                 	= BLE_GAP_PHY_2MBPS;
		params->tx_power				  	= 8;
		params->transfer_data_size			= 10*1024;
		params->link_budget					= 100;
		break;
	case BLE_5_LR:
	    params->att_mtu                  	= 23;
	    params->conn_interval            	= 7.5f;
	    params->data_len_ext_enabled     	= false;
	    params->conn_evt_len_ext_enabled 	= false;
		params->rxtx_phy                 	= BLE_GAP_PHY_CODED;
		params->tx_power				  	= 8;
		params->transfer_data_size			= 100;
		params->link_budget					= 111;
		break;
	}
}

void test_params_serialize(test_params_t * params_in, uint8_t * data_out, uint8_t * datalen_out) {
	*datalen_out = 0;

	if (sizeof (test_case_t) != 1) {
		debug_error("ble_version_t size not 1: %u", sizeof(test_case_t));
	}
	data_out[*datalen_out] = params_in->test_case;
	*datalen_out += 1;

	W_LE16(&data_out[*datalen_out], params_in->att_mtu);	// 2bytes
	*datalen_out += 2;

	if ((sizeof (float)) != 4) {
		debug_error("Float size not 4: %u", sizeof(float));
	}
	W_LE32(&data_out[*datalen_out], params_in->conn_interval);	// 4bytes
	*datalen_out += 4;

	data_out[*datalen_out] = params_in->data_len_ext_enabled;
	*datalen_out += 1;

	data_out[*datalen_out] = params_in->conn_evt_len_ext_enabled;
	*datalen_out += 1;

	data_out[*datalen_out] = params_in->rxtx_phy;
	*datalen_out += 1;

	data_out[*datalen_out] = params_in->tx_power;
	*datalen_out += 1;

	data_out[*datalen_out] = params_in->link_budget;
	*datalen_out += 1;

	W_LE32(&data_out[*datalen_out], params_in->transfer_data_size);	// 4bytes
	*datalen_out += 4;

	if (sizeof (test_ble_version_t) != 1) {
		debug_error("ble_version_t size not 1: %u", sizeof(test_ble_version_t));
	}
	data_out[*datalen_out] = params_in->ble_version;
	*datalen_out += 1;

}

void test_params_deserialize(uint8_t * data_in, uint8_t datalen_in, test_params_t * params_out) {
	uint8_t offset = 0;
//	debug_line("test_params_deserialize len %d data: ", datalen_in);
//	for (uint8_t i=0; i<datalen_in; i++) {
//		debug_global("%02x ", data_in[i]);
//	}
//	debug_global("\n");

	if (sizeof (test_case_t) != 1) {
		debug_error("ble_version_t size not 1: %u", sizeof(test_case_t));
	}
	params_out->test_case = data_in[offset];
	offset += 1;

	params_out->att_mtu = R_LE16(&data_in[offset]);		// 2 bytes
	offset += 2;

	if ((sizeof (float)) != 4) {
		debug_error("Float size not 4: %u", sizeof(float));
	}
	params_out->conn_interval = R_LE32(&data_in[offset]);		// 4 bytes
	offset += 4;


	params_out->data_len_ext_enabled = data_in[offset];
	offset += 1;

	params_out->conn_evt_len_ext_enabled = data_in[offset];
	offset += 1;

	params_out->rxtx_phy = data_in[offset];
	offset += 1;

	params_out->tx_power = data_in[offset];
	offset += 1;

	params_out->link_budget = data_in[offset];
	offset += 1;


	params_out->transfer_data_size = R_LE32(&data_in[offset]);		// 4 bytes
	offset += 4;

	if (sizeof (test_ble_version_t) != 1) {
		debug_error("ble_version_t size not 1: %u", sizeof(test_ble_version_t));
	}
	params_out->ble_version = data_in[offset];
	offset += 1;

	if (offset != datalen_in) {
		debug_error("datalen in wrong (offset %d, datalen %d)", offset, datalen_in);
	}

}

void test_params_print(test_params_t * params) {
//	debug_line("Test [%x]: %s | [%x]: %s | size %d | mtu %d",
//			params->test_case, test_case_str[params->test_case],
//			params->ble_version, ble_version_str[params->ble_version],
//			params->transfer_data_size, params->att_mtu);
	debug_line("Test: %s | %s | size %d | mtu %d",
			test_case_str[params->test_case],
			ble_version_str[params->ble_version],
			params->transfer_data_size, params->att_mtu);
	debug_line("phy %x | interval %d | DLE %u | EXT %u | pwr %d | budget %d",
			params->rxtx_phy, params->conn_interval, params->data_len_ext_enabled,
			params->conn_evt_len_ext_enabled, params->tx_power, params->link_budget);
}

void test_params_build_data(test_params_t * params, uint32_t offset, uint8_t * data, uint8_t * datalen) {
	uint8_t i;
	for (i = 0; i < params->att_mtu-3 && offset < params->transfer_data_size; i++) {
		data[i] = (uint8_t) (offset & 0xFF);
		offset++;
	}
	*datalen = i;
}

void test_params_confirm_data(test_params_t * params, uint32_t offset, uint8_t * data, uint8_t datalen) {
	uint8_t i;
	if (datalen > params->att_mtu) {
		debug_error("Wrong datalen %d, MTU %d", datalen, params->att_mtu);
	}

	for (i = 0; i < datalen; i++) {
		if (data[i] != (uint8_t) (offset & 0xFF)) {
			debug_error("Wrong data[%d] = %d [%x] @ offset %d [%x]", i, data[i], data[i], offset, offset);
		}
		offset++;
	}
}

