/*
 * ble_uuid.h
 *
 *  Created on: 6 May 2018
 *      Author: gaspe
 */

#ifndef BLE_UUID_H_
#define BLE_UUID_H_

// Base UUIDs count
#define BLE_PERIPHERAL_VS_UUID_COUNT							1

// 128-bit base UUID (little endian)
#define BLE_UUID_PERIPHERAL_BASE		{{0x01, 0xF0, 0x01, 0xF0, 0x01, 0xF0, 0x01, 0xF0, 0x01, 0xF0, 0x01, 0xF0, 0x00, 0xF0, 0x01, 0xF0}}


/** service UUIDs are marked by 1st two bytes, with first bit (MSB) always being 1 (0x8000).
 *  Characteristics UUIDs have the 1st bit (MSB) always 0 + appended the 1st two bytes of the service.
 *  Example:
 *   Service 0x8000: chars 0x0000, 0x0001
 *   Service 0x8100: chars 0x0100, 0x0101
 *   Service 0x8200: chars 0x0200, 0x0201
 */

#define BLE_SERVICES_NUM			1

#define BLE_CHARACTERISTICS_NUM		BLE_TEST_SERVICE_CHARA_NUM

#define BLE_TEST_SERVICE_CHARA_NUM	2
#define BLE_UUID_SERVICE_TEST		0x8000
#define BLE_UUID_CHARA_CONTROL		0x0000
#define BLE_UUID_CHARA_DATA			0x0001


#endif /* BLE_UUID_H_ */
