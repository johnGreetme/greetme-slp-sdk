#ifndef SLP_CORE_H
#define SLP_CORE_H

#include <string>
#include <vector>

namespace SLP {

// Defined Trigger Types for Wake-Up Events
enum TriggerType {
  SLP_TRIGGER_MANUAL_INTERACTION, // User unlocked screen
  SLP_TRIGGER_GPS_GEOFENCE,       // Entered delivery zone
  SLP_TRIGGER_NFC_HANDSHAKE,      // Physical tap
  SLP_TRIGGER_VIBRATION           // Kinetic impact
};

struct Trigger {
  TriggerType type;
  double timestamp;
  std::vector<uint8_t> sensor_data;
};

class Context {
public:
  // Connects to the device TEE (TrustZone / Secure Enclave)
  static Context Initialize();

  // The Core Function: Generates the Token
  // Throws HardwareException if Monotonic Counter fails to increment.
  std::string GenerateToken(const Trigger &trigger);

  // Verifies the integrity of the hardware counter
  bool CheckIntegrity();

  // Debug/Demo Helper: Get the current hardware counter value
  uint64_t GetCounter();

private:
  Context(); // Private constructor
  uint64_t internal_counter_id;
};

} // namespace SLP

#endif // SLP_CORE_H
