#include <EspNowCrypt.h>
#include <EspNowNode.h>
#include <EspNowPreferences.h>
#include <esp_crt_bundle.h>
#include <esp_log.h>
#include <esp_sleep.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define TAG "example"

#define SLEEP_TIME_US (1000LL * 1000LL * 60LL * 1LL) // 1 minute

#define FIRMWARE_VERSION 90201

// These structs are the application messages shared across the host and node device.
#pragma pack(1)
struct MyApplicationMessage {
  uint8_t id = 0x01;
  bool open;
};
struct MySecondApplicationMessage {
  uint8_t id = 0x02;
  double temperature;
};
#pragma pack(0)

// Encyption key used for our own packet encryption (GCM).
// We are not using the esp-now encryption due to the peer limitation.
// The key should be the same for both the host and the node.
const char esp_now_encryption_key[] = "0123456789ABCDEF"; // Must be exact 16 bytes long. \0 does not count.

// Used to validate the integrity of the messages.
// The secret should be the same for both the host and the node.
const char esp_now_encryption_secret[] = "01234567"; // Must be exact 8 bytes long. \0 does not count.

unsigned long _turn_of_led_at_ms = 0;

EspNowNode::OnLog _on_log = [](const std::string message, const esp_log_level_t log_level) {
  // Callback for logging. Can be omitted.
  esp_log_write(log_level, TAG, "EspNowNode: %s", message.c_str());
};

EspNowNode::OnStatus _on_status = [](EspNowNode::Status status) {
  switch (status) {
  case EspNowNode::Status::HOST_DISCOVERY_STARTED:
    break;
  case EspNowNode::Status::HOST_DISCOVERY_SUCCESSFUL:
    break;
  case EspNowNode::Status::HOST_DISCOVERY_FAILED:
    break;
  case EspNowNode::Status::INVALID_HOST:
    break;
  case EspNowNode::Status::FIRMWARE_UPDATE_STARTED:
    break;
  case EspNowNode::Status::FIRMWARE_UPDATE_SUCCESSFUL:
    break;
  case EspNowNode::Status::FIRMWARE_UPDATE_FAILED:
    break;
  case EspNowNode::Status::FIRMWARE_UPDATE_WIFI_SETUP_FAILED:
    break;
  }
};

EspNowPreferences _esp_now_preferences;
EspNowCrypt _esp_now_crypt(esp_now_encryption_key, esp_now_encryption_secret);
EspNowNode _esp_now_node(_esp_now_crypt, _esp_now_preferences, FIRMWARE_VERSION, _on_status, _on_log,
                         esp_crt_bundle_attach);

extern "C" {
void app_main();
}

void app_main(void) {
  _esp_now_preferences.initalizeNVS();

  // Setup node, send message, and then go to sleep.
  if (_esp_now_node.setup()) {
    MySecondApplicationMessage message;
    message.temperature = 25.6;
    _esp_now_node.sendMessage(&message, sizeof(MySecondApplicationMessage));
  }

  esp_deep_sleep(SLEEP_TIME_US);
}
