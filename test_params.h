/*
 * test_params.h
 *
 *  Created on: Jul 6, 2018
 *      Author: gksolutions
 */

#ifndef TEST_PARAMS_H_
#define TEST_PARAMS_H_

#include "stdint.h"
#include "stdbool.h"

#define TEST_READ_NOTIFY_STRING "Test data"

typedef enum {
	TEST_NULL = 0,
	TEST_BLE_WRITE = 1,
	TEST_BLE_WRITE_NO_RSP = 2,
	TEST_BLE_READ = 3,
	TEST_BLE_NOTIFY = 4,
//	TEST_L2CAP_WRITE = 5,
//	TEST_L2CAP_READ = 6,
} test_case_t;

typedef enum {
	BLE_4_1 = 0,	// BLE 4.1 and 4.0
	BLE_4_2 = 1,	// BLE 4.2
	BLE_5_HS = 2,	// BLE 5.0 High Speed
	BLE_5_LR = 3,	// BLE 5.0 Long Range
} test_ble_version_t;

typedef struct
{
	test_case_t test_case;
    uint16_t att_mtu;                   // GATT ATT MTU, in bytes.
    float	 conn_interval;             // Connection interval in ms
    bool     data_len_ext_enabled;      // Data length extension status.
    bool     conn_evt_len_ext_enabled;  // Connection event length extension status.
	uint8_t  rxtx_phy;					//PHY used (125Kbps, 1Mbps or 2Mbps)
	int8_t 	 tx_power;					//output power
	uint8_t  link_budget;				//link budget (output power minus sensitivity)
	uint32_t transfer_data_size;		//transfer data size in bytes
	test_ble_version_t ble_version;
} test_params_t;

extern const char * ble_version_str[];
extern const char * test_case_str[];

void test_params_set_all(test_params_t *params);

void test_params_load(test_params_t * params, test_ble_version_t ble_version, test_case_t test_case);

void test_params_serialize(test_params_t * params_in, uint8_t * data_out, uint8_t * datalen_out);
void test_params_deserialize(uint8_t * data_in, uint8_t datalen_in, test_params_t * params_out);
void test_params_print(test_params_t * params);
void test_params_build_data(test_params_t * params, uint32_t offset, uint8_t * data, uint8_t * datalen);
void test_params_confirm_data(test_params_t * params, uint32_t offset, uint8_t * data, uint8_t datalen);



#endif /* TEST_PARAMS_H_ */
