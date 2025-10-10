# Mapper-ToF-with-VIO
An open source project to make a 3D point cloud with a ToF camera using VIO to map of a room
Preferably relatively low-cost

## Project objectives
-   Reading in real time the IMU / Mono camera with a ToF camera
-   Synchronizing and data handling by the DE10 Lite FPGA
-   Transfering data from FPGA to PC to handle the VIO
-   Generating the fused and interpolated 3D point cloud

## Project Architectures
┌────────────┐ ┌─────┐
| Mono + IMU | | ToF |
└─────┬──────┘ └──┬──┘     
      |   "COMM"  |
      ▼           ▼
┌──────────────────────┐
|   DE10 Lite (FPGA)   |
| - Reading IMU / Mono |
| - Reading ToF        |  
| - Timestamping       |
| - UART Out           |
└─────────┬────────────┘
          │ UART
          ▼
┌──────────────────────┐
│         PC           │
│ - Data Parsing       │
│ - VIO (IMU + Cam/ToF)│
│ - Pos Estimation     │
│ - 3D Reconstruction  │
│ - Visualization      │
└──────────────────────┘

- **DE10-Lite / FPGA**: sensor acquisition, timestamping, transmission  
- **Nios II softcore**: IMU + + ToF + UART + timer drivers  
- **PC (Python)**: VIO processing and point cloud construction

## Folder Structure

📦Mapper-ToF-with-VIO
┣ 📂documentation --> Doc folder
┃ ┗
┣ 📂FPGA --> NIOS II source code
┣ 📂PC -->