#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <sys/stat.h>

// Internal TEE functions (not exposed in public header, but used by
// slp_manager) In a real scenario, these would be interacting with /dev/tee0 or
// similar

const std::string SECURE_STORAGE_FILE = ".slp_secure_storage.bin";

namespace SLP_Internal {

// Set secure file permissions (owner read/write only)
void SetSecurePermissions(const std::string &filename) {
#ifndef _WIN32
  // On Unix-like systems, set permissions to 0600 (owner read/write only)
  if (chmod(filename.c_str(), S_IRUSR | S_IWUSR) != 0) {
    std::cerr << "TEE Warning: Failed to set secure permissions on storage file."
              << std::endl;
  }
#endif
  // On Windows, file permissions work differently and default ACLs are
  // generally secure for the current user
}

uint64_t ReadCounter() {
  std::ifstream infile(SECURE_STORAGE_FILE);
  uint64_t counter = 0;
  
  if (infile.is_open()) {
    infile >> counter;
    // Validate that the read operation succeeded
    if (infile.fail()) {
      std::cerr << "TEE Warning: Failed to read counter from secure storage. "
                << "Reinitializing to 0." << std::endl;
      infile.close();
      // Reinitialize the file with 0
      std::ofstream outfile(SECURE_STORAGE_FILE);
      if (outfile.is_open()) {
        outfile << 0;
        outfile.close();
        SetSecurePermissions(SECURE_STORAGE_FILE);
      }
      return 0;
    }
    infile.close();
  } else {
    // File doesn't exist, initialize it
    std::ofstream outfile(SECURE_STORAGE_FILE);
    if (outfile.is_open()) {
      outfile << 0;
      outfile.close();
      SetSecurePermissions(SECURE_STORAGE_FILE);
    } else {
      std::cerr << "TEE Error: Failed to initialize secure storage."
                << std::endl;
    }
  }
  
  return counter;
}

void IncrementCounter() {
  uint64_t current = ReadCounter();
  
  // Check for potential overflow
  if (current == std::numeric_limits<uint64_t>::max()) {
    std::cerr << "TEE Error: Counter overflow - maximum value reached."
              << std::endl;
    // In real hardware, this would be a critical security event
    return;
  }
  
  uint64_t next = current + 1;

  std::ofstream outfile(SECURE_STORAGE_FILE);
  if (outfile.is_open()) {
    outfile << next;
    outfile.close();
    SetSecurePermissions(SECURE_STORAGE_FILE);
  } else {
    std::cerr << "TEE Error: Critical failure accessing secure storage."
              << std::endl;
    // In real hardware, this would be a tamper event
  }
}
} // namespace SLP_Internal
