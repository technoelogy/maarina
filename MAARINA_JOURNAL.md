# MAARINA — Project Journal & Master Tracker

> *"Maarina is my mother's blessing. My mother's love is my superpower."*
> — Tron, Founder

**Started:** 19th April 2026
**Founder:** Tron (GitHub: @technoelogy / Twitter: @technoelog)
**For:** Martha. 30th October 2024. Always.

## What MAARINA Is

MAARINA is an open-source wearable SOS device for elderly people.
One press calls 5 family contacts, sends GPS location via SMS,
enables two-way voice. Passive fall detection. No subscription.
Target price Rs2,499. Built on ESP32-S3 + 4G LTE.

MAARINA = Maa (mother) + Rina (Sanskrit: sacred debt owed to parents)

## Session Log

### Session 1 — 19th April 2026
- Named the project MAARINA after Martha
- Built 12 project documents
- Created folder structure at ~/maarina/
- First git commit: "feat: MAARINA initial commit — For Martha. 30th October 2024."
- Pushed to GitHub: https://github.com/technoelogy/maarina
- Registered maarina.com and maarina.care
- Session ended: ~4:00 AM IST

### Session 2 — 22nd April 2026
- Installed Pandoc for PDF conversion
- Recreated MAARINA_JOURNAL.md
- Where I stopped: converting MD files to PDF for printing

## Next 3 Things
1. Run: find . -name "*.md" | while read f; do pandoc "$f" -o "${f%.md}.pdf"; done
2. Print all PDFs — build your MAARINA binder
3. Order hardware from HARDWARE_BOM.md

## The Build Phases
Phase 0 — Arduino IDE setup
Phase 1 — SOS button + GSM call
Phase 2 — SMS
Phase 3 — Fall detection
Phase 4 — GPS in SMS
Phase 5 — Two-way voice
Phase 6 — Multi-contact sequence
Phase 7 — Power optimisation
Phase 8 — Enclosure + field test

### Session 3 — 31st July 2026
- Restarted after long gap; re-oriented on hardware setup
- Confirmed correct board: A7670E (SIM7670G parked for GPS phase)
- Located and connected antenna to MAIN port
- Wired SOS button: black clip to IO3, green clip to GND
- Uploaded dial_test.ino successfully
- Modem not responding to AT commands
- Contacted Waveshare support; they confirmed:
  - UART pins: ESP32 GPIO17 = A7670 TX, GPIO18 = A7670 RX
  - PWRKEY control: GPIO21 (HIGH = ON)
- Added GPIO21 power-on code; modem still not responding
- Sent follow-up to Waveshare with photo and detailed status
- Where I stopped: Waiting on Waveshare reply about jumper switch location

