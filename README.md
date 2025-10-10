# Mapper-ToF-with-VIO
An open source project to make a 3D point cloud with a ToF camera using VIO to map of a room
Preferably relatively low-cost

## Project objectives
-   Reading in real time the IMU / Mono camera with a ToF camera
-   Synchronizing and data handling by the DE10 Lite FPGA
-   Transfering data from FPGA to PC to handle the VIO
-   Generating the fused and interpolated 3D point cloud

## Project Architectures
┌────────────┐ ┌─────┐      <br>
| Mono + IMU | | ToF |      <br>
└─────┬──────┘ └──┬──┘      <br>  
      |   "COMM"  |         <br>
      ▼           ▼         <br>
┌──────────────────────┐    <br>
|   DE10 Lite (FPGA)   |    <br>
| - Reading IMU / Mono |    <br>
| - Reading ToF        |    <br>
| - Timestamping       |    <br>
| - UART Out           |    <br>
└─────────┬────────────┘    <br>
          │ UART            <br>
          ▼                 <br>
┌──────────────────────┐    <br>
│         PC           │    <br>
│ - Data Parsing       │    <br>
│ - VIO (IMU + Cam/ToF)│    <br>
│ - Pos Estimation     │    <br>
│ - 3D Reconstruction  │    <br>
│ - Visualization      │    <br>
└──────────────────────┘    <br>

- **DE10-Lite / FPGA**: sensor acquisition, timestamping, transmission  
- **Nios II softcore**: IMU + + ToF + UART + timer drivers  
- **PC (Python)**: VIO processing and point cloud construction

## Folder Structure

📦Mapper-ToF-with-VIO
┣ 📂documentation --> Doc folder
┃ ┗
┣ 📂FPGA --> NIOS II source code
┣ 📂PC -->