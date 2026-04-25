# MAARINA Alert — Hardware Bill of Materials

**Target Price:** Rs 2,499 (assembled)  
**Form Factor:** Wristband / pendant wearable  
**Connectivity:** 4G LTE (SIM7670G) + BLE 5.0  
**Power:** 500mAh LiPo, USB-C charging  

---

## Core Components

| # | Component | Description | Qty | Unit Price (Rs) | Total (Rs) | Buy Link |
|---|-----------|-------------|-----|-----------------|------------|----------|
| 1 | ESP32-S3-WROOM-1 | Main MCU — dual-core 240MHz, WiFi, BLE 5.0 | 1 | 320 | 320 | [Robu.in](https://robu.in) |
| 2 | SIM7670G | 4G LTE Cat-1 module — SMS, voice, data | 1 | 950 | 950 | [Evelta.com](https://evelta.com) |
| 3 | MPU6050 | 6-axis IMU — fall detection (accel + gyro) | 1 | 80 | 80 | [Robu.in](https://robu.in) |
| 4 | INMP441 | I2S MEMS microphone — voice capture | 1 | 120 | 120 | [Robu.in](https://robu.in) |
| 5 | MAX98357A | I2S audio amplifier — speaker output | 1 | 150 | 150 | [Evelta.com](https://evelta.com) |
| 6 | LiPo 500mAh 3.7V | Rechargeable battery | 1 | 180 | 180 | [Robu.in](https://robu.in) |
| 7 | TP4056 Module | LiPo charger with protection (USB-C) | 1 | 30 | 30 | [Robu.in](https://robu.in) |
| 8 | MT3608 Boost | 3.7V → 5V boost converter for SIM7670G | 1 | 25 | 25 | [Robu.in](https://robu.in) |
| 9 | AMS1117-3.3 | 3.3V LDO regulator for ESP32 + sensors | 1 | 10 | 10 | [Robu.in](https://robu.in) |
| 10 | SOS Button | Tactile push button, 6mm, waterproof rated | 1 | 15 | 15 | [Robu.in](https://robu.in) |
| 11 | LED — Red | SOS / alert indicator | 1 | 2 | 2 | Local |
| 12 | LED — Green | Power / status indicator | 1 | 2 | 2 | Local |
| 13 | Vibration Motor | Haptic feedback — 10mm coin type | 1 | 35 | 35 | [Robu.in](https://robu.in) |
| 14 | Small Speaker | 8Ω 1W — voice playback | 1 | 40 | 40 | [Robu.in](https://robu.in) |
| 15 | Nano SIM Slot | Push-push type surface mount | 1 | 20 | 20 | [Evelta.com](https://evelta.com) |
| 16 | PCB (custom) | 2-layer, 50x30mm, JLCPCB | 5pcs | 60 | 12 per unit | [JLCPCB](https://jlcpcb.com) |
| 17 | Enclosure | 3D printed PLA, wristband mount | 1 | 80 | 80 | Local print |
| 18 | Misc (caps, resistors, wires) | Passives | — | 30 | 30 | Local |

---

## Total BOM Cost

| Category | Cost (Rs) |
|----------|-----------|
| Core Electronics | 2,059 |
| PCB | 12 |
| Enclosure | 80 |
| Misc | 30 |
| **Total** | **~2,181** |
| Assembly + margin | ~318 |
| **Retail Target** | **Rs 2,499** |

---

## Supplier Summary

| Supplier | Website | Notes |
|----------|---------|-------|
| Robu.in | https://robu.in | Best for ESP32, sensors, LiPo |
| Evelta.com | https://evelta.com | SIM modules, audio ICs |
| Amazon India | https://amazon.in | Backup source, faster shipping |
| JLCPCB | https://jlcpcb.com | PCB fabrication, SMT assembly |
| Fab India 3D | Local | Enclosure printing |

---

## JLCPCB Ordering Notes

- Board size: 50mm × 30mm
- Layers: 2
- Finish: HASL (lead-free)
- Minimum order: 5 pcs
- Estimated cost: Rs 300 for 5 boards (~Rs 60/unit)
- Turnaround: 7–10 days to India

---

## Component Versions / Alternatives

| Component | Primary | Alternative |
|-----------|---------|-------------|
| MCU | ESP32-S3-WROOM-1 | ESP32-S3-DevKitC-1 (for prototyping) |
| 4G Module | SIM7670G | A7670E, SIM7600G |
| IMU | MPU6050 | LSM6DS3, ICM-42688-P |
| Microphone | INMP441 | MSM261S4030H0R |
| Amp | MAX98357A | PAM8403 (analog fallback) |
