# MAARINA Alert — Wiring Guide

**MCU:** ESP32-S3-WROOM-1  
**All voltages:** 3.3V logic unless noted  

---

## ESP32-S3 Pin Assignment Overview

| GPIO | Function | Connected To |
|------|----------|-------------|
| GPIO1 | I2C SDA | MPU6050 SDA |
| GPIO2 | I2C SCL | MPU6050 SCL |
| GPIO4 | I2S WS (LRCLK) | INMP441 WS, MAX98357 LRC |
| GPIO5 | I2S SCK (BCLK) | INMP441 SCK, MAX98357 BCLK |
| GPIO6 | I2S SD IN | INMP441 SD |
| GPIO7 | I2S SD OUT | MAX98357 DIN |
| GPIO8 | UART TX → SIM | SIM7670G RX |
| GPIO9 | UART RX ← SIM | SIM7670G TX |
| GPIO10 | SIM PWR KEY | SIM7670G PWRKEY |
| GPIO11 | SIM RESET | SIM7670G RESET |
| GPIO12 | SOS Button | Button → GND (pull-up) |
| GPIO13 | Vibration Motor | NPN transistor base |
| GPIO14 | LED Red | 220Ω → LED → GND |
| GPIO15 | LED Green | 220Ω → LED → GND |
| GPIO16 | Battery ADC | Voltage divider (100k/100k) |
| GND | Ground | Common ground |
| 3V3 | 3.3V rail | AMS1117 output |

---

## 1. MPU6050 (Fall Detection — I2C)

```
MPU6050          ESP32-S3
─────────        ──────────
VCC         ──▶  3V3
GND         ──▶  GND
SDA         ──▶  GPIO1
SCL         ──▶  GPIO2
INT         ──▶  (optional: GPIO3 for interrupt-driven wake)
AD0         ──▶  GND  (I2C address = 0x68)
```

**Notes:**
- Add 4.7kΩ pull-up resistors on SDA and SCL lines to 3.3V
- MPU6050 can wake ESP32-S3 from deep sleep via INT pin
- I2C address: 0x68 (AD0 low) or 0x69 (AD0 high)

---

## 2. SIM7670G (4G LTE — UART)

```
SIM7670G         ESP32-S3 / Power
─────────        ─────────────────
VCC (4.0V)  ──▶  MT3608 output (boost 3.7V→4.2V)
GND         ──▶  Common GND
RX          ──▶  GPIO8 (ESP32 TX)
TX          ──▶  GPIO9 (ESP32 RX)
PWRKEY      ──▶  GPIO10 (pull LOW 1s to power on)
RESET       ──▶  GPIO11 (active LOW)
RING        ──▶  (optional: GPIO3 for incoming call wake)
```

**Nano SIM slot:**
```
SIM7670G SIM pins → Nano SIM card holder (push-push type)
```

**Notes:**
- SIM7670G requires 3.4–4.4V, peak current 2A — do NOT power from 3.3V rail
- Use MT3608 boost converter: LiPo (3.7V) → 4.2V regulated
- Add 100µF + 10µF capacitors on SIM7670G VCC for current spikes
- UART baud rate: 115200 default
- Power on sequence: hold PWRKEY LOW for 1 second

---

## 3. INMP441 (Microphone — I2S)

```
INMP441          ESP32-S3
─────────        ──────────
VDD         ──▶  3V3
GND         ──▶  GND
WS          ──▶  GPIO4  (Word Select / LRCLK)
SCK         ──▶  GPIO5  (Bit Clock)
SD          ──▶  GPIO6  (Serial Data OUT → ESP32 IN)
L/R         ──▶  GND    (Left channel, address = 0)
```

**Notes:**
- INMP441 outputs left-justified 24-bit I2S data
- L/R pin: GND = left channel, 3.3V = right channel
- Sampling rate: 16kHz recommended for voice
- Shared I2S bus with MAX98357 (WS + SCK lines shared)

---

## 4. MAX98357A (Speaker Amplifier — I2S)

```
MAX98357A        ESP32-S3
─────────        ──────────
VDD         ──▶  3V3
GND         ──▶  GND
LRC (WS)    ──▶  GPIO4  (shared with INMP441)
BCLK        ──▶  GPIO5  (shared with INMP441)
DIN         ──▶  GPIO7  (Serial Data IN ← ESP32 OUT)
SD          ──▶  3V3    (enable — tie HIGH to enable amp)
GAIN        ──▶  leave floating (9dB gain default)
```

**Speaker connection:**
```
MAX98357A OUT+  ──▶  Speaker +
MAX98357A OUT-  ──▶  Speaker -
Speaker: 8Ω, 1W minimum
```

**Notes:**
- SD pin: HIGH = enabled, LOW = shutdown (use for power saving)
- Do NOT connect OUT+ or OUT- to GND — fully differential output
- Add small RC filter if audio has high-frequency noise

---

## 5. LiPo Battery + TP4056 + Power Distribution

```
LiPo Battery
    │
    ▼
TP4056 (charger + protection)
    │ BAT+ / BAT-
    ├──────────────────▶ MT3608 IN+ (boost for SIM7670G → 4.2V)
    │
    └──────────────────▶ AMS1117-3.3 IN (→ 3.3V for ESP32 + sensors)

USB-C ──▶ TP4056 VIN (5V charging input)
```

**TP4056 Connections:**
```
TP4056       
────────
VIN (5V) ──▶ USB-C 5V
GND      ──▶ Common GND
BAT+     ──▶ LiPo +
BAT-     ──▶ LiPo - (= GND)
OUT+     ──▶ MT3608 IN+ AND AMS1117 IN
CHRG LED ──▶ (optional: 1kΩ → LED indicator)
```

**Battery voltage monitor:**
```
BAT+ ──▶ 100kΩ ──▶ GPIO16 (ADC)
                └──▶ 100kΩ ──▶ GND
```
(Voltage divider: reads 0–2.1V on ADC for 0–4.2V battery)

---

## 6. SOS Button

```
3V3 ──▶ internal pull-up (ESP32 INPUT_PULLUP)
         │
       GPIO12
         │
      [BUTTON]
         │
        GND
```

**Notes:**
- Configure GPIO12 as INPUT_PULLUP in firmware
- Detect LOW = button pressed
- Add 100nF capacitor across button for debounce

---

## 7. Vibration Motor

```
GPIO13 ──▶ 1kΩ ──▶ NPN transistor (2N2222) Base
                    Collector ──▶ Motor +
                    Emitter  ──▶ GND
Motor - ──▶ GND
3V3    ──▶ Motor + (via transistor collector)
           Add flyback diode (1N4148) across motor terminals
```

---

## 8. LEDs

```
GPIO14 ──▶ 220Ω ──▶ Red LED anode ──▶ LED cathode ──▶ GND
GPIO15 ──▶ 220Ω ──▶ Green LED anode ──▶ LED cathode ──▶ GND
```

---

## Full Wiring Checklist

- [ ] MPU6050 powered and I2C pull-ups in place
- [ ] SIM7670G on boosted supply with bulk capacitors
- [ ] SIM card inserted and seated
- [ ] INMP441 L/R pin tied to GND
- [ ] MAX98357 SD pin tied HIGH
- [ ] Shared I2S bus (GPIO4, GPIO5) connected to both mic and amp
- [ ] TP4056 BAT pins to LiPo
- [ ] AMS1117 output decoupled (10µF + 100nF)
- [ ] Battery ADC voltage divider in place
- [ ] SOS button pull-up active, 100nF debounce cap
- [ ] Motor flyback diode fitted
- [ ] All grounds tied together (common GND)
- [ ] USB-C connected to TP4056 VIN for charging
