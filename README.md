# State-Locked Protocol (SLP) SDK

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE)
[![Standard](https://img.shields.io/badge/Standard-3GPP%20Rel--20%20Draft-green.svg)](https://www.uplinklogic.tech)
[![Platform](https://img.shields.io/badge/Platform-Android%20%7C%20iOS%20%7C%20Embedded-lightgrey.svg)]()

> **The Hardware-Rooted Standard for Secure Dormancy.**
> Preventing "Vampire Drain" and Sybil Attacks in 6G & DePIN Networks.

## Overview

The **State-Locked Protocol (SLP)** is a verification standard that moves "Proof of Presence" from the software layer to the hardware logic gate.

This SDK implements **Phase 1** of the SLP Roadmap: **TEE Emulation**.
It allows mobile and IoT developers to implement "Zero-Allocation" dormancy by using the device's Trusted Execution Environment (TEE) to simulate the behavior of a dedicated Hardware Monotonic Counter.

**Key Features:**

- **Zero-Allocation Networking:** No server RAM allocated without a valid token.
- **Monotonic Integrity:** Prevents Replay Attacks using hardware-backed counters.
- **Battery Savings:** Reduces "Keep-Alive" radio wake-ups by up to 90%.

## Installation

### Android (Gradle)

```gradle
dependencies {
    implementation 'tech.uplinklogic:slp-sdk:0.1.0-alpha'
}
```

### iOS (CocoaPods)

```ruby
pod 'SLP-Core', '~> 0.1.0'
```

## Usage Example (C++)

Generating a "Proof of Physical Work" token:

```cpp
#include "slp_core.h"

// 1. Initialize the Secure Context (Binds to TEE)
SLP::Context secure_ctx = SLP::Initialize();

// 2. Define the Trigger (e.g., GPS Location Lock)
SLP::Trigger physical_trigger = {
    .type = SLP_TRIGGER_GPS_LOCK,
    .precision = SLP_PRECISION_HIGH
};

// 3. Generate the State-Locked Token
// This will FAIL if the hardware counter cannot physically increment.
try {
    std::string token = secure_ctx.GenerateToken(physical_trigger);
    Network::SendPayload(token, data);
} catch (SLP::HardwareException& e) {
    // Handle "Brick Risk" or Counter Failure
    Log::Error("Hardware integrity check failed.");
}
```

## The Architecture

The SLP SDK does not rely on software variables. It binds cryptographic signatures to a Monotonic Counter stored in the Secure Enclave.

**Wake:** Device wakes from Dormant State via Interrupt.

**Lock:** TEE reads the Monotonic Counter (C).

**Increment:** TEE forces a physical write (C+1).

**Sign:** Token is generated: Hash(Data+(C+1)+Key).

For the full 19-page technical specification, read the White Paper.

https://www.uplinklogic.tech/topic/mobile-performance/announcing-state-locked-protocol

## License

Copyright © 2026 Greetme Technologies LTD. Licensed under the Apache License, Version 2.0.
