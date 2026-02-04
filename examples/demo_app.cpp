#include "slp_core.h"
#include <chrono>
#include <iostream>
#include <thread>

// Simulated network verification function
void verify_dormancy(const std::string &token) {
  // In a real app, this sends the token to the server
  std::cout << "  [Network] Verifying token: " << token << "..." << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  if (token.find("SLP-TOKEN") != std::string::npos) {
    std::cout << "Verification Result: [Success]" << std::endl;
  } else {
    std::cout << "Verification Result: [Fail]" << std::endl;
  }
}

int main() {
  std::cout << "=== GreetMe SLP SDK Demo ===" << std::endl;

  // Initialize the SLPManager
  SLP::Context secure_ctx = SLP::Initialize();

  // Print Current Hardware Counter
  std::cout << "Current Hardware Counter: [" << secure_ctx.GetCounter() << "]"
            << std::endl;

  // Simulate a Trigger (e.g. GPS Lock)
  SLP::Trigger trigger = {.type = SLP::SLP_TRIGGER_GPS_GEOFENCE,
                          .timestamp = 123456789.0};

  std::cout << "Generating Proof of Physical Work..." << std::endl;
  try {
    std::string token = secure_ctx.GenerateToken(trigger);

    // Call verifty_dormancy (simulating network check)
    verify_dormancy(token);

    // Print New Hardware Counter
    std::cout << "New Hardware Counter: [" << secure_ctx.GetCounter() << "]"
              << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
