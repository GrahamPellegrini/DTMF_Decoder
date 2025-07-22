# DTMF Decoder – Microcontroller-Based Embedded System

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

The system was developed as part of the **Microcontroller Based Systems** study-unit ([CCE2014](https://www.um.edu.mt/courses/studyunit/CCE2014)) at the **University of Malta**, under the supervision of [Dr. Johann A. Briffa](https://www.um.edu.mt/profile/johannbriffa).

> ⚠️ **Credit**: This project was a **group effort** and was jointly developed by:
> - **Graham Pellegrini** – graham.pellegrini@um.edu.mt
> - **Luca Vella** – Luca.Vella.22@um.edu.mt
> - **Julian Falzon** – julian.a.falzon.19@um.edu.mt
> - **Matthew Mifsud** – matthew.mifsud.22@um.edu.mt
>
> 📜 Redistribution or reuse of this project requires **explicit consent from all listed contributors**.

---

## ⚙️ System Features

- Real-time audio signal processing using the **ADC peripheral** of an ARM Cortex-M microcontroller
- Decoding of tone-pairs as per the **ITU-T Q.23 DTMF standard**
- Display of decoded symbols on a **Hitachi HD44780-compatible LCD**
- Initialisation to a well-defined system state on power-up or reset
- Error signalling for unrecognisable or invalid tones
- Persistent configuration across sessions

---

## 🧩 Technical Breakdown

### ✅ Functional Requirements Addressed
- Real-time response to asynchronous DTMF tone input
- Use of a **digital input** (system mode/configuration switches)
- Use of **digital output** (LEDs for status/error indication)
- Connection with a **peripheral** (text-based LCD)
- Robust design against invalid input
- Use of **interrupts** and **multi-threaded architecture**

### 🛠️ Implementation Details
- Entirely written in **C/C++** using **ARM MDK** (Keil µVision)
- Core logic organised into modular components:
  - Signal acquisition via ADC
  - Frequency analysis and tone mapping
  - Character resolution
  - LCD interface module
  - System control state machine
- Thorough **documentation via DoxyGen**, with consistent inline commenting

---

## 🧪 Testing & Evaluation

- Signal decoding was tested using:
  - Clean tone samples
  - Timing-distorted inputs
  - Noise-polluted recordings
- Input signals delivered via line-in audio, mobile apps, and simulated waveforms
- LCD display behaviour validated against expected keypad sequence outputs

---

## 📁 Repository Structure (Trunk)

```
DTMF_Decoder/
├── code/                    # Source code & generated Doxygen docs
│   └── trunk/
│       ├── src/             # C/C++ implementation files
│       └── include/         # Header files
├── documentation/
│   ├── design/              # Design brief (LaTeX sources)
│   ├── agendas/             # Group meeting agendas
│   ├── minutes/             # Group meeting minutes
│   └── scaling/             # Contribution forms
├── test/                    # Experimental code (excluded from assessment)
└── feedback/                # Lecturer feedback (read-only)
```

---

## 📦 Setup

### Requirements
- **Keil MDK ARM** (v5.38a or later)
- **Embedded Artists LPC board**
- External audio signal generator (app or PC audio line-out)
- HD44780-compatible LCD

### Build Instructions
1. Clone the repository
2. Open the `.uvprojx` file in Keil µVision
3. Compile and flash the firmware to the target board
4. Provide input via the headphone line into the ADC circuit

---

## 🔒 Licensing Notice

This repository is provided for **educational and referential purposes only**. Reuse, reproduction, or redistribution of code or design materials **must receive prior written approval** from **all contributors** listed above.

If you wish to use or extend this project:
1. Please contact the repository owner and contributors individually.
2. Explain your intended use and obtain consensus.

A custom license declaration is added in the LICENSE file to reflect this clause.

---

## 🎓 Acknowledgements

- University of Malta — Department of Communications & Computer Engineering
- Dr. Johann A. Briffa — Supervisor
- ARM MDK development tools
- Keil µVision IDE
- DoxyGen for code documentation

---

## 📬 Contact

For project-related queries:
- Graham Pellegrini: graham.pellegrini@um.edu.mt
- Luca Vella: Luca.Vella.22@um.edu.mt
- Julian Falzon: julian.a.falzon.19@um.edu.mt
- Matthew Mifsud: matthew.mifsud.22@um.edu.mt
