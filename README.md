# Mapper-ToF-with-VIO

An open source project to make a 3D point cloud with a ToF camera using VIO to map of a room. Preferably relatively low-cost.
It probably wont work but ehhh if it works it works. Most of my work on the Nios II is based on what I did in my masters degree and with Quartus.
Probably will try to put it on a drone if it works who knows.  
This project is mainly to familiarize myself with Verilog/SystemVerilog, Quartus and FGPA overall and git.  
/!\ Note to self : remember to deactivate Memory integrity to enable Quartus USB-driver and let OpenCore open before and while opening the bit stream /!\

## Project objectives

- Reading in real time the IMU / Mono camera with a ToF camera
- Synchronizing and data handling by the DE10 Lite FPGA
- Transfering data from FPGA to PC to handle the VIO and map in real time
- Generating the fused and interpolated 3D point cloud

Example of 3D could point from a ToF LiDAR taken from the internet
![alt text](images/LiDARexample.png)

## Project Architectures

[IMU + mono + ToF] -> [Nios II / FPGA] -> [PC]

- **DE10-Lite / FPGA (Verilog)**: sensor acquisition, timestamping, transmission
- **Nios II processor (C++)**: IMU + Mono camera + ToF + I2C + UART + timer drivers  
- **PC (Python)**: VIO processing and 3D point cloud construction and interpolation

### DE10 Lite Expension Header

For I2C I use the GPIO0 as SDA, the GPIO1 as SCL, the GPIO2 as SDA OE (output enable) and the GPIO 3 for SCL OE  
For UART I use the GPIO34 as RxD and the GPIO35 as TxD
![alt text](images/ExpansionHeader.png)

## Folder Structure

PS : for the Quartus folder I only put the most important files

Mapper-ToF-with-VIO  
┣ Documentation --> Doc folder  
┃ ┗ DE10_Lite_User_Manual.pdf  
┣ FPGA --> **Quartus project**  
┃ ┣ Hardware_Qsys  
┃ ┃ ┣ synthesis  
┃ ┃ ┃ ┗ Hardware_Qsys.qip -> Import NIOS II on the schematic  
┃ ┃ ┗ Hardware_Qsys.bsf  
┃ ┣ software -> **Code for the Nios II**  
┃ ┃ ┣ Software_Mapper  
┃ ┃ ┃ ┣ Software_Mapper  
┃ ┃ ┣ Software_Mapper_bsp  
┃ ┃ ┃ ┣ Software_Mapper  
┃ ┣ DE10_LITE_Golden_Top.v   
┃ ┣ Hardware_Mapper.qpf  
┃ ┣ Hardware_Mapper.qsf  
┃ ┣ Hardware_Qsys.qsys -> Qsys for NIOS II  
┃ ┣ Hardware_Qsys.sopcinfo -> Base file for software  
┣ Images  
┃ ┗ LiDARexample.png  
┣ PC --> **VIO & Mapping**  
┃ ┗  
┣ Software_Mapper --> Workspace of Eclipse for Quartus (non important)  
┣ .gitignore  
┣ LICENSE  
┣ README.md  
┗ todo.md  
