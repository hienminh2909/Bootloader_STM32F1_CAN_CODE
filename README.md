# STM32 CAN Bootloader (UDS / ISO-TP / Bare-Metal): Completed an embedded systems internship at FPT SOFTWARE HCM

# OVERVIEW
<img width="1162" height="641" alt="image" src="https://github.com/user-attachments/assets/55ae311f-9330-4c1e-8891-da0f0dc09aa9" />

# SOFTWARE REQUIREMENT
<img width="1043" height="582" alt="image" src="https://github.com/user-attachments/assets/d2453378-9841-41ee-ab21-80ad0496786e" />

# SOFTWARE ARCHITECTURE
<img width="1562" height="352" alt="image" src="https://github.com/user-attachments/assets/6aa399ef-e526-41a9-9ada-702338ed2c45" />

# MEMORY
<img width="506" height="430" alt="image" src="https://github.com/user-attachments/assets/02a71930-8d7b-4b45-baf6-7456b3c22730" />

# SUD
<img width="1566" height="352" alt="image" src="https://github.com/user-attachments/assets/cdf4dc21-7415-4ecf-bb39-397b40818520" />



*** Components:
    - PC Host: Python-based firmware uploader
    - CAN Gateway
       + Arduino Nano
       + MCP2515 (SPI-based CAN controller)
       + MCP2551
    - Target MCU
       + STM32F103C8T6 running custom bootloader ***

**́́ Key Features:
- Firmware update over CAN bus
- Custom bootloader (bare-metal, register-level)
- Automotive communication stack:
   + ISO-TP (CAN Transport Protocol)
   + UDS (Unified Diagnostic Services)
 - Boot control using dedicated Flash flag region
 - Flash memory management (erase / program / verify)
 - Multi-node system (PC ↔ Gateway ↔ STM32)
 - Custom low-level drivers (no HAL / no LL)
