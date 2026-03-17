# 16-bit Hardware-in-the-Loop (HIL) VLSI Validator

### Project Overview
A complete Hardware-Software Co-Design environment. This project bridges a physical **ESP32 Microcontroller** with a simulated 16-bit **Verilog RTL Core** running on a Linux host via a custom Python UART bridge. It serves as a proof-of-concept for testing digital logic before deploying to an FPGA.

### Key Features
* **16-bit RTL ALU:** Custom Verilog design supporting addition, subtraction, and bitwise logic with status flags.
* **HIL Python Bridge:** A bidirectional script using `pyserial` and `subprocess` to stream multi-byte data packets between the physical hardware and the Icarus Verilog simulator.
* **Physical Telemetry UI:** Integrated TFT display on the ESP32 to visualize real-time computation results from the virtual silicon.
* **Automated Toolchain:** Native Ubuntu workflow utilizing GTKWave for functional verification.

### System Architecture
1. **Master Controller (ESP32):** Generates 16-bit test vectors and transmits them via UART.
2. **Host Bridge (Python):** Captures serial data, injects it into the RTL testbench, and triggers the simulator.
3. **Virtual Silicon (Verilog):** Processes the data combinationally and returns the output.
4. **Validation:** Results are sent back down the serial line and verified on the hardware UI.

### Tech Stack
* **Hardware Description:** Verilog HDL
* **Simulation & Synthesis:** Icarus Verilog, GTKWave
* **Embedded Firmware:** C++, PlatformIO
* **Scripting:** Python 3

### Verification Results
*Testing confirmed across 16-bit arithmetic operations.*

| Test Case | Inputs (A, B) | Opcode | Expected Result | Actual Result | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Addition** | 2, 3 | 00 (Add) | 5 | 5 | **PASS** |
| **Subtraction** | 7, 4 | 01 (Sub) | 3 | 3 | **PASS** |
| **Bitwise AND** | 12 (1100), 10 (1010) | 10 (AND) | 8 (1000) | 8 | **PASS** |
| **16-bit Stress** | 6631, 4245 | 00 (Add) | 10876 | 10876* | **PASS** |
*(Note: Display output verified via UART bridge synchronization).*
