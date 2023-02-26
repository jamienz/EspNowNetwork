#ifndef __ESP_NOW_STRUCTURE_H__
#define __ESP_NOW_STRUCTURE_H__

#include <Arduino.h>

#define MESSAGE_ID_HEADER 0x03

#define MESSAGE_ID_DISCOVERY_REQUEST_V1 0xD0
#define MESSAGE_ID_DISCOVERY_RESPONSE_V1 0xD1

#define MESSAGE_ID_CHALLENGE_REQUEST_V1 0xDA
#define MESSAGE_ID_CHALLENGE_RESPONSE_V1 0xDB
#define MESSAGE_ID_CHALLENGE_UPDATE_RESPONSE_V1 0xDC

#pragma pack(1)

/**
 * Sent by host and nodes. This message is followed by the application message.
 */
struct EspNowMessageHeaderV1 {
  uint8_t id = MESSAGE_ID_HEADER;
  uint16_t retries = 0;
  uint32_t challenge = 0; // Challenge from [EspNowChallengeResponseV1]
};

/**
 * Sent by nodes to perform host discovery.
 */
struct EspNowDiscoveryRequestV1 {
  uint8_t id = MESSAGE_ID_DISCOVERY_REQUEST_V1;
};

/**
 * Sent by host to confirm host discovery.
 */
struct EspNowDiscoveryResponseV1 {
  uint8_t id = MESSAGE_ID_DISCOVERY_RESPONSE_V1;
};

/**
 * Sent by nodes to request the challenge to include in the [EspNowMessageHeaderV1] message
 * for message verification (protect against reply attacks).
 */
struct EspNowChallengeRequestV1 {
  uint8_t id = MESSAGE_ID_CHALLENGE_REQUEST_V1;
  uint32_t firmware_version;
};

/**
 * Sent by host in reply to a [EspNowChallengeRequestV1].
 * The challenge can only be used once.
 */
struct EspNowChallengeResponseV1 {
  uint8_t id = MESSAGE_ID_CHALLENGE_RESPONSE_V1;
  uint32_t challenge; // Should be set in [EspNowMessageHeaderV1].
};

/**
 * Sent by host in reply to a [EspNowChallengeRequestV1] when the device should update its firmware.
 */
struct EspNowChallengeDownloadResponseV1 {
  uint8_t id = MESSAGE_ID_CHALLENGE_UPDATE_RESPONSE_V1;
  char wifi_ssid[32];     // WiFi SSID that node should connect to.
  char wifi_password[64]; // WiFi password that the node should connect to.
  char url[96];           // url where to find firmware binary. Note the max file path.
  uint16_t port = 80;     // HTTP port to use.
};

#pragma pack(0)

#endif // __ESP_NOW_STRUCTURE_H__