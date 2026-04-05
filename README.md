STM32 CAN Bootloader (UDS / ISO-TP / Bare-Metal)

*** Overview:
  - This project implements a CAN-based firmware update system for STM32F103C8T6 using automotive communication standards.
  - The system enables firmware flashing from a PC to the target MCU over CAN bus, via a custom gateway built on Arduino Nano.
  => Focus: Full-stack embedded development — from low-level drivers to high-level automotive protocols.

*** System Architecture:
        PC (Python Tool)
              │
           UART/USB
              │
        Arduino Nano (CAN Gateway)
              │
        MCP2515 (CAN Controller) + MCP2551 (Transceiver)
              │
            CAN Bus
              │
        STM32F103C8T6 (Bootloader + Application)

*** Components:
    - PC Host: Python-based firmware uploader
    - CAN Gateway
       + Arduino Nano
       + MCP2515 (SPI-based CAN controller)
       + MCP2551
    - Target MCU
       + STM32F103C8T6 running custom bootloader

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
