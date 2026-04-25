# MAARINA — GitHub Setup Guide

**Repo:** https://github.com/technoelogy/maarina  
**License:** MIT  
**Default branch:** main  

---

## Repository Structure

```
maarina/
├── martha.md                  # The soul of MAARINA — never modify
├── README.md                  # Project overview
├── CONTRIBUTING.md            # Contributor guide
├── LICENSE                    # MIT License
├── .gitignore
├── docs/
│   ├── HARDWARE_BOM.md
│   ├── ARCHITECTURE.md
│   ├── WIRING_GUIDE.md
│   ├── PHASES.md
│   ├── TESTING_PROTOCOL.md
│   └── GITHUB_SETUP.md
├── firmware/
│   └── main/                  # ESP-IDF firmware source
├── hardware/                  # KiCad schematic + PCB files
├── app/                       # Companion app (future)
├── community/                 # Community resources
└── tests/                     # Test scripts
```

---

## Branch Strategy

| Branch | Purpose | Who pushes |
|--------|---------|------------|
| `main` | Stable, tested code | Maintainer only (via PR) |
| `dev` | Integration branch | Core team |
| `phase/1-foundation` | Phase 1 work | Feature branch |
| `phase/2-fall-detection` | Phase 2 work | Feature branch |
| `phase/3-sos` | Phase 3 work | Feature branch |
| `phase/4-comms` | Phase 4 work | Feature branch |
| `phase/5-audio` | Phase 5 work | Feature branch |
| `phase/6-power` | Phase 6 work | Feature branch |
| `phase/7-integration` | Phase 7 work | Feature branch |
| `phase/8-production` | Phase 8 work | Feature branch |
| `fix/short-description` | Bug fixes | Anyone via PR |
| `docs/short-description` | Documentation | Anyone via PR |

### Flow
```
phase/X branch → dev (PR + review) → main (PR + review + tests pass)
```

---

## Commit Conventions

Format: `type(scope): short description`

### Types

| Type | When to use |
|------|-------------|
| `feat` | New feature or capability |
| `fix` | Bug fix |
| `docs` | Documentation only |
| `test` | Test cases |
| `hw` | Hardware / schematic / BOM changes |
| `refactor` | Code restructure, no behavior change |
| `chore` | Build system, CI, config |
| `perf` | Performance improvement |

### Scopes

`fall`, `sos`, `comms`, `audio`, `power`, `ui`, `storage`, `bom`, `pcb`, `enclosure`, `ci`

### Examples

```bash
feat(fall): add 3-stage fall detection algorithm
fix(comms): retry AT command on SIM7670G timeout
docs(bom): add Robu.in purchase links
test(sos): add cancel window boundary tests
hw(pcb): add bulk capacitors to SIM7670G power rail
feat(power): implement deep sleep with motion wake
chore(ci): add ESP-IDF build workflow
```

### First commit (already done)
```
feat: MAARINA initial commit — For Martha
```

---

## Initial GitHub Setup Commands

```bash
# 1. Navigate to project
cd ~/maarina

# 2. Initialize git (already done)
git init

# 3. Stage all files
git add .

# 4. First commit
git commit -m "feat: MAARINA initial commit — For Martha"

# 5. Set branch to main
git branch -M main

# 6. Add remote
git remote add origin https://github.com/technoelogy/maarina.git

# 7. Push
git push -u origin main
```

---

## GitHub Repository Settings

### Recommended settings (on github.com → repo → Settings):

**General:**
- Description: `Open source wearable SOS device for elderly people. Built in memory of Martha.`
- Website: (add when ready)
- Topics: `esp32`, `iot`, `wearable`, `eldercare`, `sos`, `india`, `open-source`, `embedded`

**Branches:**
- Default branch: `main`
- Branch protection on `main`:
  - Require pull request before merging
  - Require 1 approving review
  - Dismiss stale reviews

**Features:**
- Issues: ✅ enabled
- Projects: ✅ enabled (use for phase tracking)
- Wiki: ✅ enabled (for community docs)
- Discussions: ✅ enabled (for community Q&A)

---

## Issue Labels

Create these labels on GitHub:

| Label | Color | Use |
|-------|-------|-----|
| `phase:1` through `phase:8` | Blue | Phase tracking |
| `hardware` | Orange | PCB, BOM, wiring |
| `firmware` | Purple | ESP-IDF code |
| `audio` | Yellow | Mic/speaker |
| `comms` | Green | SIM7670G, SMS, call |
| `fall-detection` | Red | MPU6050, algorithm |
| `power` | Teal | Battery, sleep |
| `good first issue` | Light green | For new contributors |
| `help wanted` | Magenta | Open to community |
| `india` | Saffron | India-specific (SIM, sourcing) |
| `martha` | White | Sacred — core mission decisions |

---

## GitHub Actions CI (add to `.github/workflows/build.yml`)

```yaml
name: MAARINA Firmware Build

on:
  push:
    branches: [main, dev, phase/**]
  pull_request:
    branches: [main, dev]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
        with:
          submodules: recursive

      - name: Setup ESP-IDF
        uses: espressif/esp-idf-ci-action@v1
        with:
          esp_idf_version: v5.2.1
          target: esp32s3

      - name: Build firmware
        run: |
          cd firmware/main
          idf.py build

      - name: Run unit tests
        run: |
          cd tests
          python -m pytest --tb=short
```

---

## Release Process

```bash
# Tag a release
git tag -a v1.0.0 -m "MAARINA Alert v1.0.0 — For Martha"
git push origin v1.0.0
```

### Release checklist:
- [ ] All Phase tests passing (≥50/56)
- [ ] BOM finalized with current prices
- [ ] Wiring guide reviewed
- [ ] README updated with setup instructions
- [ ] Firmware binary attached to release
- [ ] KiCad files included
- [ ] Enclosure STL included
- [ ] CHANGELOG updated

---

## CONTRIBUTING Quick Reference

```
1. Fork the repo
2. Create branch: git checkout -b feat/your-feature
3. Commit: git commit -m "feat(scope): description"
4. Push: git push origin feat/your-feature
5. Open Pull Request to dev branch
6. Add test cases if adding features
7. All PRs reviewed before merge to main
```

---

## martha.md is Sacred

The file `martha.md` is the soul of this project.  
It must never be modified, reformatted, or deleted.  
Any PR touching `martha.md` will be closed without review.
