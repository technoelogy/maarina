# MAARINA Alert — Testing Protocol

**Total test cases: 56**  
**Coverage: All 8 phases**  
**Format:** ID | Test | Input | Expected | Pass/Fail  

---

## Phase 1 — Foundation Tests (6 tests)

| ID | Test | Input / Action | Expected Result |
|----|------|---------------|-----------------|
| T01 | ESP32-S3 boots | Power on | Serial output: "MAARINA boot OK" within 3s |
| T02 | LED blink | Boot | GPIO14 (green LED) blinks 1Hz |
| T03 | I2C scan | Boot | MPU6050 detected at address 0x68 |
| T04 | NVS read/write | Write "test=1", read back | Value matches, no corruption |
| T05 | USB serial monitor | Connect USB | 115200 baud, readable output |
| T06 | Flash firmware | OTA or USB flash | Device reboots, new version string confirmed |

---

## Phase 2 — Fall Detection Tests (10 tests)

| ID | Test | Input / Action | Expected Result |
|----|------|---------------|-----------------|
| T07 | MPU6050 read | Static placement | |a| ≈ 1.0g, stable ±0.05g |
| T08 | Freefall detection | Drop device 1m | Freefall flag set within 150ms |
| T09 | Impact detection | Drop onto surface | Impact flag set, |a| > 2.5g logged |
| T10 | Post-fall stillness | Leave still after drop | Stillness confirmed within 2s |
| T11 | Full fall sequence | Drop 1m, leave still | Fall event triggered, callback fires |
| T12 | False positive — walking | Walk briskly | No fall triggered |
| T13 | False positive — stairs | Walk down stairs | No fall triggered |
| T14 | False positive — sitting down fast | Sit down quickly | No fall triggered |
| T15 | Sensitivity threshold | Adjust threshold in NVS | Fall triggers at new threshold |
| T16 | 50-drop test | Drop 50 times | ≥48 correct detections (96%+) |

---

## Phase 3 — SOS State Machine Tests (8 tests)

| ID | Test | Input / Action | Expected Result |
|----|------|---------------|-----------------|
| T17 | Fall → SOS_ALERT | Trigger fall | State = SOS_ALERT, vibrate 3x, red LED blink |
| T18 | Button → SOS_ALERT | Press SOS button | State = SOS_ALERT within 500ms |
| T19 | Cancel window timer | Enter SOS_ALERT | 10 second countdown visible on serial |
| T20 | Cancel button press | Press button within 10s | State = CANCELLED, green LED, no call |
| T21 | No cancel → CALLING | Wait 10s | State = CALLING, SIM dials |
| T22 | CALLING → IN_CALL | Call connects | State = IN_CALL, green LED solid |
| T23 | Call end → IDLE | Hang up | State = IDLE, low power |
| T24 | SOS during IN_CALL | Button press | Ignored — already in call |

---

## Phase 4 — 4G Communications Tests (10 tests)

| ID | Test | Input / Action | Expected Result |
|----|------|---------------|-----------------|
| T25 | SIM7670G power on | Boot sequence | AT OK response within 10s |
| T26 | Network registration | Insert SIM (Airtel) | AT+CREG returns registered (1 or 5) |
| T27 | Signal strength | Check CSQ | AT+CSQ returns ≥10 (−93 dBm or better) |
| T28 | SMS send | Trigger SOS | SMS received on caregiver phone within 30s |
| T29 | SMS content | Receive SMS | Message contains name, time, MAARINA signature |
| T30 | Voice call dial | Trigger call | Phone rings at caregiver number |
| T31 | Call connect | Answer call | IN_CALL state, audio path active |
| T32 | Busy fallback | First number busy | Dials second number within 5s |
| T33 | No answer fallback | No answer 30s | Dials second number, then third |
| T34 | Jio SIM test | Repeat T26–T31 with Jio SIM | All pass |

---

## Phase 5 — Audio Tests (8 tests)

| ID | Test | Input / Action | Expected Result |
|----|------|---------------|-----------------|
| T35 | Mic capture | Speak near device | I2S data non-zero, waveform visible |
| T36 | Mic audio quality | Speak "Help" clearly | Waveform intelligible, SNR > 20dB |
| T37 | Speaker playback | Trigger audio prompt | "Help is on the way, stay calm" plays audibly |
| T38 | Speaker volume | 1m distance | Audio clearly audible without distortion |
| T39 | 2-way call audio | SOS call active | Caregiver can hear device, device can hear caregiver |
| T40 | Half-duplex mute | Prompt playing | Mic muted during playback, no echo |
| T41 | Audio after sleep | Wake from deep sleep | Audio works without reinitialization |
| T42 | Concurrent I2S | Mic + speaker active | No I2S conflicts, no crashes |

---

## Phase 6 — Power Management Tests (6 tests)

| ID | Test | Input / Action | Expected Result |
|----|------|---------------|-----------------|
| T43 | Deep sleep entry | No activity 60s | Sleep current < 50µA measured |
| T44 | Button wake | Press SOS in sleep | Wake within 500ms, SOS_ALERT state |
| T45 | Motion wake | Drop device while sleeping | MPU6050 INT wakes device, fall detected |
| T46 | Battery read | Check ADC GPIO16 | Voltage within ±0.1V of multimeter reading |
| T47 | Low battery warning | Discharge to 20% | Vibrate 3x, red LED slow blink |
| T48 | Battery life standby | Full charge, standby | >48 hours before low battery warning |

---

## Phase 7 — Integration Tests (5 tests)

| ID | Test | Input / Action | Expected Result |
|----|------|---------------|-----------------|
| T49 | End-to-end SOS | Drop device, leave still | SMS + call within 20s |
| T50 | End-to-end cancel | Drop device, press cancel | No SMS, no call |
| T51 | Multi-caregiver | #1 busy, #2 busy, #3 answers | Call connects on #3, SMS sent to all 3 |
| T52 | Real-world wear test | Wear for 8 hours, normal activity | No false positives, battery > 60% |
| T53 | Network loss recovery | Remove SIM mid-operation | Device recovers, retries on SIM reinsertion |

---

## Phase 8 — Production Tests (3 tests)

| ID | Test | Input / Action | Expected Result |
|----|------|---------------|-----------------|
| T54 | PCB assembly test | Flash fresh PCB | All components respond (I2C, UART, ADC, I2S) |
| T55 | OTA firmware update | Push new firmware via HTTPS | Device updates, reboots, new version confirmed |
| T56 | 10-unit batch test | Run T01–T53 on 10 units | All 10 units pass ≥50/53 tests |

---

## Test Execution Log Template

```
Date: ____________
Firmware version: ____________
Tester: ____________
Hardware revision: ____________

| ID  | Result | Notes |
|-----|--------|-------|
| T01 | PASS   |       |
| T02 | PASS   |       |
...

Total: __ / 56 passed
```

---

## Pass Criteria

| Phase | Required Pass Rate |
|-------|--------------------|
| Phases 1–3 | 100% |
| Phase 4 (comms) | ≥90% |
| Phase 5 (audio) | ≥87% |
| Phase 6 (power) | 100% |
| Phases 7–8 | ≥90% |
| **Overall** | **≥50 / 56** |
