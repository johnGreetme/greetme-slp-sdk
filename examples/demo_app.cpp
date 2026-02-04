#include "slp_core.h"
#include <iostream>

int main() {
  std::cout << "Starting SLP SDK Demo..." << std::endl;

  // ERROR WAS HERE: Changed SLP::Initialize() to SLP::Context::Initialize()
  try {
    SLP::Context secure_ctx = SLP::Context::Initialize();

    // Simulate checking the counter
    uint64_t current_counter = secure_ctx.GetMonotonicCounter();
    std::cout << "Current Hardware Counter: " << current_counter << std::endl;

    // Verify Verification Logic
    // Note: In the previous demo, verification was a standalone function.
    // The user now calls secure_ctx.VerifyDormancy().
    // We need to implement this in the SDK or add it back as a local function
    // if the user intended that. Given "secure_ctx.VerifyDormancy()", it
    // implies a method on the Context class.
    bool is_valid = secure_ctx.VerifyDormancy();
    std::cout << "Dormancy Verification: " << (is_valid ? "PASSED" : "FAILED")
              << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "SDK Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
