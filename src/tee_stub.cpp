#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

// Internal TEE functions (not exposed in public header, but used by
// slp_manager) In a real scenario, these would be interacting with /dev/tee0 or
// similar

const std::string SECURE_STORAGE_FILE = ".slp_secure_storage.bin";

namespace SLP_Internal {

bool FileExists(const std::string &name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

uint64_t ReadCounter() {
  if (!FileExists(SECURE_STORAGE_FILE)) {
    // Initialize if not exists
    std::ofstream outfile(SECURE_STORAGE_FILE);
    if (outfile.is_open()) {
      outfile << 0;
      outfile.close();
    }
    return 0;
  }

  std::ifstream infile(SECURE_STORAGE_FILE);
  uint64_t counter = 0;
  if (infile.is_open()) {
    infile >> counter;
    infile.close();
  }
  return counter;
}

void IncrementCounter() {
  uint64_t current = ReadCounter();
  uint64_t next = current + 1;

  std::ofstream outfile(SECURE_STORAGE_FILE);
  if (outfile.is_open()) {
    outfile << next;
    outfile.close();
  } else {
    std::cerr << "TEE Error: Critical failure accessing secure storage."
              << std::endl;
    // In real hardware, this would be a tamper event
  }
}
} // namespace SLP_Internal
