# MAARINA Alert — 8 Phase Build Roadmap

---

## Phase Overview

| Phase | Name | Goal | Duration |
|-------|------|------|----------|
| 1 | Foundation | Dev environment, ESP32-S3 blink, I2C scan | Week 1 |
| 2 | Fall Detection | MPU6050 read, fall threshold algorithm | Week 2 |
| 3 | SOS Trigger | Button + fall → SOS state machine | Week 3 |
| 4 | 4G Comms | SIM7670G AT commands, SMS, voice call | Week 4–5 |
| 5 | Audio | INMP441 mic + MAX98357 speaker, 2-way audio | Week 6 |
| 6 | Power Management | Deep sleep, battery monitor, wake sources | Week 7 |
| 7 | Integration | All modules together, field testing | Week 8–9 |
| 8 | Production | PCB, enclosure, documentation, open source release | Week 10–12 |

---

## Phase 1 — Foundation

**Goal:** Working ESP32-S3 dev environment, I2C scan confirms MPU6050 detected.

**Deliverables:**
- ESP-IDF installed and working
- ESP32-S3 blink LED on GPIO14
- I2C scan showing MPU6050 at address 0x68
- UART monitor working at 115200 baud
- Git repo initialized and pushed

**Claude Prompt:**
```
I'm building MAARINA — a wearable SOS device on ESP32-S3.
Help me set up ESP-IDF v5.x on macOS, create a new project,
blink an LED on GPIO14, and run an I2C scanner to verify
MPU6050 is detected at address 0x68. Show me the complete
CMakeLists.txt, main.c, and how to flash via USB.
```

---

## Phase 2 — Fall Detection

**Goal:** Read MPU6050 accelerometer, detect fall events with tuned threshold.

**Deliverables:**
- MPU6050 driver reading accel X/Y/Z at 100Hz
- Fall detection algorithm (sudden acceleration spike → freefall → impact)
- Serial output showing fall detected / no fall
- Threshold tunable via NVS config
- Unit tests for fall algorithm

**Algorithm Overview:**
```
1. Read accel magnitude: |a| = sqrt(ax² + ay² + az²)
2. Freefall detect: |a| < 0.3g for >100ms
3. Impact detect: |a| > 2.5g within 500ms after freefall
4. Confirm: sustained low movement for 2s after impact
5. Trigger SOS if all 3 conditions met
```

**Claude Prompt:**
```
For MAARINA (ESP32-S3 + MPU6050 on I2C GPIO1/GPIO2):
Write a fall detection driver in C using ESP-IDF.
Read accelerometer at 100Hz, implement a 3-stage algorithm:
freefall detection (|a| < 0.3g), impact detection (|a| > 2.5g),
and post-fall stillness confirmation (2 seconds).
Include a FreeRTOS task, configurable thresholds stored in NVS,
and a callback function triggered on confirmed fall.
```

---

## Phase 3 — SOS Trigger

**Goal:** Complete SOS state machine — fall OR button press triggers SOS flow with 10s cancel window.

**Deliverables:**
- SOS state machine (IDLE → SOS_ALERT → CALLING → IN_CALL → IDLE)
- 10 second cancel window with button press cancel
- LED patterns for each state
- Vibration motor feedback
- SOS event logged to NVS with timestamp

**Claude Prompt:**
```
For MAARINA (ESP32-S3), implement a SOS state machine in C with ESP-IDF FreeRTOS.
States: IDLE, SOS_ALERT, CALLING, IN_CALL, CANCELLED.
Transitions:
- Fall detected OR button press → SOS_ALERT
- 10s timeout with no cancel → CALLING
- Button press during SOS_ALERT → CANCELLED → IDLE
- Call connected → IN_CALL
- Call ended → IDLE
Include LED patterns (red blink = alert, solid = in call, green = idle),
vibration motor feedback, and NVS event logging.
```

---

## Phase 4 — 4G Communications

**Goal:** SIM7670G sends SMS and makes voice call on SOS trigger.

**Deliverables:**
- SIM7670G power-on sequence via PWRKEY
- AT command driver with timeout and retry
- SMS send to configured caregiver number
- Voice call dial, detect connect/busy/no-answer
- Fallback: try up to 3 numbers in sequence
- Phone numbers stored in NVS

**Claude Prompt:**
```
For MAARINA (ESP32-S3 + SIM7670G on UART GPIO8/GPIO9 at 115200 baud):
Write a complete AT command driver in C using ESP-IDF.
Implement:
1. Power-on sequence (PWRKEY GPIO10 LOW for 1s, wait for AT OK)
2. Network registration check (AT+CREG?)
3. SMS send function (AT+CMGF=1 text mode)
4. Voice call dial (ATD+91XXXXXXXXXX)
5. Call status detection (CONNECT, BUSY, NO ANSWER)
6. Sequential fallback: try up to 3 phone numbers stored in NVS
7. SMS message format: "MAARINA SOS: [name] needs help. Time: [time]"
Include error handling and AT response parsing.
```

---

## Phase 5 — Audio

**Goal:** 2-way audio during SOS call — mic captures voice, speaker plays caller audio.

**Deliverables:**
- INMP441 I2S mic capture at 16kHz
- MAX98357 I2S speaker playback
- Audio routed through SIM7670G during voice call
- Pre-recorded voice prompt: "Help is on the way, stay calm"
- Mic mute during playback (half-duplex management)

**Claude Prompt:**
```
For MAARINA (ESP32-S3):
Implement I2S audio using ESP-IDF for:
1. INMP441 microphone on I2S0 (WS=GPIO4, SCK=GPIO5, SD=GPIO6) at 16kHz 16-bit mono
2. MAX98357A speaker on I2S1 (LRC=GPIO4, BCLK=GPIO5, DIN=GPIO7)
3. Audio routing to SIM7670G for 2-way voice call (PCM audio via UART or DAC)
4. Play a short WAV prompt stored in SPIFFS on SOS trigger
5. Half-duplex control: mute mic while speaker plays
Show the I2S config structs, DMA buffer setup, and FreeRTOS task structure.
```

---

## Phase 6 — Power Management

**Goal:** Maximize battery life — deep sleep between events, smart wake sources.

**Deliverables:**
- Deep sleep with MPU6050 motion interrupt wake (GPIO3)
- Battery voltage ADC read on GPIO16
- Low battery warning (vibrate + LED at <20%)
- Shutdown at <5% battery with NVS state save
- Wake-on-button (SOS button as RTC GPIO wake source)
- Sleep current target: <50µA

**Claude Prompt:**
```
For MAARINA (ESP32-S3 + LiPo 500mAh):
Implement power management using ESP-IDF:
1. Configure ESP32-S3 deep sleep with two wake sources:
   - RTC GPIO wake on SOS button (GPIO12, active LOW)
   - EXT1 wake on MPU6050 interrupt (GPIO3, active HIGH)
2. Battery voltage read via ADC on GPIO16 (100k/100k divider, 3.3V ref)
3. Battery level: 100%=4.2V, 20%=3.6V, 5%=3.4V
4. Low battery: vibrate 3 times + red LED slow blink
5. Critical battery: save state to NVS, shut down gracefully
6. On wake: restore state, log wake reason via esp_sleep_get_wakeup_cause()
```

---

## Phase 7 — Integration

**Goal:** All modules working together — real-world SOS test with family member as caregiver.

**Deliverables:**
- Full integration test: drop device → SMS received in <15s → call connects
- 3 caregiver numbers configured via BLE provisioning app (or USB serial)
- Field test: 50+ fall simulations, <2% false positives
- Battery life test: >48 hours standby
- SIM card tested on Airtel/Jio/BSNL

**Claude Prompt:**
```
For MAARINA — help me write an integration test suite in Python
that connects to the device via USB serial and:
1. Sends simulated fall events
2. Verifies SOS state machine transitions
3. Confirms SMS delivery (via Twilio webhook or manual check)
4. Logs test results to CSV
5. Reports pass/fail for each of the 50 test cases in TESTING_PROTOCOL.md
Also help me write a BLE provisioning flow to configure caregiver phone numbers
without needing a dedicated app — using a simple BLE serial terminal.
```

---

## Phase 8 — Production

**Goal:** Custom PCB, 3D printed enclosure, open source release, first 10 units to pilot families.

**Deliverables:**
- KiCad schematic and PCB layout (50x30mm, 2-layer)
- Gerbers ordered from JLCPCB
- 3D printed enclosure (PLA, wristband mount, OpenSCAD source)
- Firmware OTA update via SIM7670G (HTTPS)
- GitHub release v1.0.0 with all documentation
- 10 pilot units assembled and shipped to elderly women in Chennai/Mumbai
- MAARINA website live

**Claude Prompt:**
```
For MAARINA — help me:
1. Write a KiCad schematic in netlist format for the full MAARINA circuit
   (ESP32-S3-WROOM-1, SIM7670G, MPU6050, INMP441, MAX98357, TP4056, MT3608, AMS1117)
2. Generate JLCPCB-compatible BOM and CPL files for SMT assembly
3. Write an OpenSCAD model for a 65x35x12mm enclosure with:
   - USB-C cutout
   - SOS button hole (6mm)
   - Speaker grille (3x3 holes)
   - Wristband mounts (22mm standard strap)
4. Implement OTA firmware update over HTTPS using ESP-IDF esp_https_ota
5. Write the GitHub release checklist and open source license recommendation
```

---

## Milestone Summary

| Milestone | Target | Success Criteria |
|-----------|--------|-----------------|
| M1 | End Week 1 | MPU6050 detected, LED blinking |
| M2 | End Week 2 | Fall detected in 95% of drop tests |
| M3 | End Week 3 | SOS state machine verified |
| M4 | End Week 5 | SMS sent, call connected on real SIM |
| M5 | End Week 6 | 2-way audio working in call |
| M6 | End Week 7 | 48hr battery life confirmed |
| M7 | End Week 9 | 50-test integration suite passing |
| M8 | End Week 12 | 10 pilot units shipped |
