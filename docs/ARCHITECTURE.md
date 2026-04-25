# MAARINA Alert — System Architecture

---

## Overview

MAARINA Alert is a wearable SOS device that detects falls, listens for distress, and connects elderly users to family via 4G voice calls and SMS — with no subscription, no app dependency, and no cloud lock-in.

---

## Hardware Block Diagram

```
                        ┌─────────────────────────────────────┐
                        │           MAARINA DEVICE             │
                        │                                      │
  ┌──────────┐  I2C     │  ┌─────────────┐                    │
  │ MPU6050  │──────────┼─▶│             │                    │
  │ (IMU)    │          │  │             │   UART   ┌────────┐│
  └──────────┘          │  │  ESP32-S3   │◀────────▶│SIM7670G││
                        │  │  (Main MCU) │          │(4G LTE)││
  ┌──────────┐  I2S     │  │             │          └────┬───┘│
  │ INMP441  │──────────┼─▶│             │               │    │
  │ (Mic)    │          │  │             │         Nano SIM    │
  └──────────┘          │  │             │                    │
                        │  │             │   I2S    ┌────────┐│
  ┌──────────┐  GPIO    │  │             │─────────▶│MAX98357││
  │SOS Button│──────────┼─▶│             │          │  + Spkr││
  └──────────┘          │  └──────┬──────┘          └────────┘│
                        │         │                            │
  ┌──────────┐  GPIO    │         │ GPIO                       │
  │Vibe Motor│◀─────────┼─────────┤                            │
  └──────────┘          │         │                            │
                        │  ┌──────▼──────┐                    │
  ┌──────────┐          │  │  LEDs (R/G) │                    │
  │ LiPo     │──────────┼─▶│  Power Mgmt │                    │
  │TP4056+   │  3.7V    │  │  TP4056     │                    │
  │MT3608    │          │  │  MT3608     │                    │
  └──────────┘          │  └─────────────┘                    │
                        └─────────────────────────────────────┘
```

---

## Software Module Map

```
firmware/
├── main/
│   ├── main.c                  # Entry point, task scheduler
│   ├── fall_detection/
│   │   ├── fall_detector.c     # MPU6050 read + threshold logic
│   │   └── fall_detector.h
│   ├── sos/
│   │   ├── sos_manager.c       # SOS trigger, escalation logic
│   │   └── sos_manager.h
│   ├── comms/
│   │   ├── sim7670g.c          # AT command driver for SIM7670G
│   │   ├── sms.c               # SMS send/receive
│   │   ├── voice_call.c        # Dial, answer, hang up
│   │   └── comms.h
│   ├── audio/
│   │   ├── mic_capture.c       # INMP441 I2S read
│   │   ├── speaker_play.c      # MAX98357 I2S write
│   │   └── audio.h
│   ├── power/
│   │   ├── power_manager.c     # Sleep, wake, battery monitor
│   │   └── power_manager.h
│   ├── storage/
│   │   ├── nvs_config.c        # NVS: phone numbers, settings
│   │   └── nvs_config.h
│   └── ui/
│       ├── led.c               # LED patterns
│       ├── haptic.c            # Vibration motor
│       └── ui.h
```

---

## State Machine

```
                    ┌─────────────┐
              ┌────▶│    IDLE     │◀────────────────┐
              │     │  (low power)│                  │
              │     └──────┬──────┘                  │
              │            │                         │
              │     Fall detected OR                 │
              │     SOS button pressed               │
              │            │                         │
              │     ┌──────▼──────┐                  │
              │     │  SOS_ALERT  │                  │
              │     │  (vibrate + │                  │
              │     │   beep 10s) │                  │
              │     └──────┬──────┘                  │
              │            │                         │
              │     No cancel in 10s                 │
              │            │                         │
              │     ┌──────▼──────┐                  │
              │     │  CALLING    │                  │
              │     │ (dial #1    │                  │
              │     │  caregiver) │                  │
              │     └──────┬──────┘                  │
              │            │                         │
              │     ┌──────▼──────┐                  │
              │     │  IN_CALL    │                  │
              │     │ (2-way audio│                  │
              │     │  + SMS sent)│                  │
              │     └──────┬──────┘                  │
              │            │                         │
              │     Call ended                       │
              │            │                         │
              └────────────┘
                       │
              Cancel pressed in 10s
                       │
                ┌──────▼──────┐
                │  CANCELLED  │
                │  (log + idle)│
                └─────────────┘
```

---

## Communication Protocol

### SMS Format (on SOS trigger)
```
MAARINA SOS: [Name] may need help.
Location context: [last known / GPS if added later]
Time: [HH:MM DD/MM/YYYY]
Call her now. — MAARINA
```

### AT Command Flow (SIM7670G)
```
1. AT               → OK (module alive)
2. AT+CREG?         → +CREG: 0,1 (registered on network)
3. AT+CSQ           → signal strength
4. AT+CMGF=1        → SMS text mode
5. AT+CMGS="number" → send SMS
6. ATD+91XXXXXXXXXX → dial voice call
7. AT+CHUP          → hang up
```

---

## Power Budget

| State | Current Draw | Duration | Notes |
|-------|-------------|----------|-------|
| Deep sleep | ~10µA | Most of time | MPU6050 motion wake |
| Idle (active) | ~30mA | Periodic poll | 1Hz IMU read |
| SOS alert | ~80mA | ~10s | Vibe + LED + SIM warmup |
| Voice call | ~200mA | 2–5 min | SIM7670G active |
| Charging | ~500mA | ~1.5hr | TP4056 |

**Battery life estimate:** 500mAh → ~3 days standby, ~2hr continuous call

---

## Design Principles

1. **Offline-first** — works without internet, only needs SIM
2. **No subscription** — uses standard voice/SMS
3. **No app dependency** — works from day 1 with any phone
4. **Fail-safe** — hardware SOS button always works regardless of firmware state
5. **India-native** — BSNL/Airtel/Jio compatible, India pricing
