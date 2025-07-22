<h1 align="center">DTMF Decoder – Microcontroller-Based Embedded System</h1>

<p align="center">
  <a href="https://www.um.edu.mt/courses/studyunit/CCE2014">
    <img src="https://img.shields.io/badge/University%20of%20Malta-CCE2014-blue?style=for-the-badge" alt="CCE2014">
  </a>
  <a href="https://github.com/GrahamPellegrini/DTMF_Decoder">
    <img src="https://img.shields.io/badge/Project-DTMF%20Decoder-green?style=for-the-badge" alt="DTMF Decoder">
  </a>
  <a href="https://www.um.edu.mt/profile/johannbriffa">
    <img src="https://img.shields.io/badge/Supervisor-Dr.%20Johann%20Briffa-lightgrey?style=for-the-badge" alt="Supervisor">
  </a>
</p>

---

## 🔊 Overview

This project implements a **Dual-Tone Multi-Frequency (DTMF) Decoder** embedded system using an ARM Cortex-M microcontroller. The decoder interprets standard tone combinations used in telecommunication keypads and displays results in real time on an LCD.

Developed for the **Microcontroller Based Systems** study-unit ([CCE2014](https://www.um.edu.mt/courses/studyunit/CCE2014)) at the **University of Malta**, the project involved real-time audio processing, tone recognition, and hardware-software integration.

---

## ⚙️ System Features

- Real-time audio signal capture and tone detection via **ADC**
- Decoding of DTMF signals using **Goertzel algorithm**
- Keypad display output via **HD44780-compatible LCD**
- **Interrupt-based design** for responsiveness and efficiency
- Error detection and LED indication for invalid tone sequences
- Modular architecture and state machine-based control flow
- EEPROM persistence for user-selected settings

---

## 🧩 Technical Breakdown

### ✅ Functional Requirements Addressed
- Decode DTMF inputs in real-time
- Display results clearly via LCD interface
- System configuration through digital input pins
- Error detection and LED signaling
- Robust operation across varied tone quality
- Persistent configuration via EEPROM

### 🛠️ Implementation Details
- Written in **C/C++** and compiled with **ARM MDK (Keil µVision)**
- Modular source split into `drivers`, `include`, and `src` folders
- LCD handled using custom `lcd.c` driver
- DTMF decoding based on optimised Goertzel algorithm
- EEPROM used to store display mode (scrolling vs single-character)
- Clear documentation provided using **Doxygen**

### 🔍 Core Components

#### Signal Processing – `Sampler.c`
- Audio signal buffering and preprocessing
- Quiet region detection for tone separation
- Continuous sampling via ADC interrupt

#### Frequency Analysis – `Decode.c`
- Goertzel algorithm for frequency detection
- DTMF key matching
- Input validation (must detect exactly two tones)

#### Display Interface – `lcd.h/.c`
- Character display using 16x2 LCD
- Mode 0: single-character overwrite
- Mode 1: scrolling character display

#### EEPROM Logic – `eeprom.c`
- Persistent user display setting
- EEPROM map:
  - `0x00`: Display mode (0 = single, 1 = scrolling)

#### System Control – `main.c`
- State machine implementation
- Signal buffering and display coordination

---

## 🧪 Testing & Evaluation

Test procedures involved:
- **Pure tone decoding** via signal generator
- **Distorted tone testing** (overlapping, timing errors)
- **Background noise simulation**
- Visual verification using LCD and LED indicators

---

## 📁 Repository Structure

```bash
DTMF_Decoder/
├── code/
│   ├── trunk/                     # 📌 Main implementation folder
│   │   ├── src/                  # C source files
│   │   ├── include/              # Header files
│   │   ├── drivers/              # LCD, ADC, and system drivers
│   │   ├── RTE/, lst/, obj/      # Keil output and config folders
│   │   ├── group_05.uvprojx      # µVision project file
│   │   ├── doxyfile              # Doxygen config
│   ├── branches/, tags/          # Reserved SVN dirs (unused)
├── documentation/
│   ├── design/                   # 📐 LaTeX report
│   ├── agendas/, minutes/       # Project meeting logs
│   ├── scaling/                  # Contribution breakdowns
│   └── feedback/                 # Assessment feedback
├── test/                         # Experimental and discarded tests
├── Demo.mp4                      # 🎥 Video demonstration
├── LICENSE                       # 📜 Custom license
├── README.md                     # 📘 This file
```

---

## 📦 Setup

### Requirements
- Keil MDK ARM (v5.38a or later)
- ARM Cortex-M development board (e.g., LPC4088)
- HD44780-compatible LCD
- Audio tone generator (software/hardware)

### Build Instructions
```bash
1. Clone this repository
2. Launch `group_05.uvprojx` in Keil µVision
3. Connect target hardware (board, LCD, tone source)
4. Flash the firmware
5. Feed audio signal via line-in jack
```

---

## 🔒 License

This was a **collaborative group project**.

This project is shared under a **custom license agreement**. From the side of Graham Pellegrini, reuse is permitted **only if**:

- All contributors are contacted in good faith and their approval is sought.
- If no response is received from any contributor **within 1 month** of a documented attempt, consent may be assumed unless objected.

Refer to the [`LICENSE`](LICENSE) file for the full statement.

---

## 📬 Contact

> - Graham Pellegrini — graham.pellegrini@um.edu.mt
> - Luca Vella — Luca.Vella.22@um.edu.mt
> - Julian Falzon — julian.a.falzon.19@um.edu.mt
> - Matthew Mifsud — matthew.mifsud.22@um.edu.mt
