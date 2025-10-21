# Mapper-ToF-with-VIO

An open source project to make a 3D point cloud with a ToF camera using VIO to map a room. Preferably relatively low-cost.
It probably wont work but ehhh if it works it works. Most of my work on the Nios II is based on what I did in my masters degree and with Quartus.
Probably will try to put it on a drone if it works and if I find how to send data over the air to my PC.  
This project is mainly to familiarize myself with Verilog/SystemVerilog, Quartus and FGPA overall and git.  
/!\ Note to self : remember to deactivate Memory integrity to enable Quartus USB-driver and let OpenCore open before uploading the bit stream /!\

## Project objectives

- Reading in real time the IMU with a ToF camera
- Synchronizing and data handling by the DE10 Lite FPGA
- Transfering data from FPGA to PC to handle the VIO and map in real time
- Generating the fused and interpolated 3D point cloud

Example of 3D cloud point from a ToF LiDAR taken from the internet
![alt text](images/LiDARexample.png)

## Project Architectures

[IMU + ToF] -> [Nios II / FPGA] -> [PC]

- **DE10-Lite / FPGA (Verilog)**: sensor acquisition, timestamping, transmission  
- **Nios II processor (C++)**: IMU + ToF + I2C + UART + timer drivers  
- **PC (Python)**: VIO processing and 3D point cloud construction and interpolation  

### DE10 Lite Expansion Header

For I2C I use the GPIO0 as SDA and GPIO1 as SCL
For UART I use the GPIO34 as RxD and the GPIO35 as TxD for the LiDAR and GPIO32 as RxD and GPIO33 as TxD for the PC
![alt text](images/ExpansionHeader.png)

### LED Representation on FPGA

To be able to debug and add a bit of a feedback on what's happening I added an LED HMI

- LED 9 = I2C init success
- LED 8 = IMU init success
- LED 7 = LiDAR init success
- LED 6-1 = not defined yet
- LED 0 = 2Hz heartbeat

### Debug mode

To be able to activate the debug mode you need to put all switches up before starting the FPGA (before uploading code or before the soft reset with PB).

### Command needed on Nios2 Command Shell to upload code

For those commands you need to do a cd /cygdrive/c/Users/YourUsername/Mapper-ToF-with-VIO/FPGA_NIOS_Mapper/software/Software_NiosII or Software_NiosII_bsp.
You also need to be on a Nios2 Command Shell.

- **make clean** -> clean folder you are currently on (example : on this project on Software_NiosII or Software_NiosII_bsp)  
- **make all** -> build folder you are currently on only use on Software_NiosII or Software_NiosII_bsp  
- **nios2-download Software_NiosII.elf --go** -> Upload the program on the DE10 Lite only use on Software_NiosII  

## Folder Structure

PS : for the Quartus folder I only put the most important files

Mapper-ToF-with-VIO  
┣ **Documentation --> Doc folder**  
┃ ┣ DE10_Lite_User_Manual.pdf  
┃ ┣ MPU-6000-Datasheet.pdf  
┃ ┣ TFmini-Plus-A02_Product_Manual_EN.pdf  
┃ ┗ ug_embedded_ip.pdf  
┣ **FPGA_NIOS_Mapper --> Quartus project**  
┃ ┣ Hardware_Qsys  
┃ ┃ ┣ synthesis  
┃ ┃ ┃ ┗ Hardware_Qsys.qip -> Used to import NIOS II on the schematic  
┃ ┃ ┗ Hardware_Qsys.bsf  
┃ ┣ **software --> Code for the Nios II**  
┃ ┃ ┣ Software_NiosII  
┃ ┃ ┃ ┣ include  
┃ ┃ ┃ ┃ ┣ drivers  
┃ ┃ ┃ ┃ ┃ ┣ hex.h  
┃ ┃ ┃ ┃ ┃ ┣ i2c.h  
┃ ┃ ┃ ┃ ┃ ┗ uart.h  
┃ ┃ ┃ ┃ ┣ ISR  
┃ ┃ ┃ ┃ ┃ ┗ timerISR.h  
┃ ┃ ┃ ┃ ┣ sensors  
┃ ┃ ┃ ┃ ┃ ┣ LiDAR.h  
┃ ┃ ┃ ┃ ┃ ┗ IMU.h  
┃ ┃ ┃ ┣ src  
┃ ┃ ┃ ┃ ┣ drivers  
┃ ┃ ┃ ┃ ┃ ┣ hex.c  
┃ ┃ ┃ ┃ ┃ ┣ i2c.c  
┃ ┃ ┃ ┃ ┃ ┗ uart.c  
┃ ┃ ┃ ┃ ┣ ISR  
┃ ┃ ┃ ┃ ┃ ┗ timerISR.c  
┃ ┃ ┃ ┃ ┣ sensors  
┃ ┃ ┃ ┃ ┃ ┣ LiDAR.c  
┃ ┃ ┃ ┃ ┃ ┗ IMU.c  
┃ ┃ ┃ ┃ ┗ main.c  
┃ ┃ ┃ ┗ Makefile  
┃ ┃ ┗ Software_NiosII_bsp  
┃ ┃ ┃ ┣ Makefile  
┃ ┃ ┃ ┗ system.h  
┃ ┣ DE10_LITE_Golden_Top.v  
┃ ┣ Hardware_Mapper.qpf  
┃ ┣ Hardware_Mapper.qsf  
┃ ┣ Hardware_Qsys.qsys -> Qsys for NIOS II  
┃ ┣ Hardware_Qsys.sopcinfo -> Base file for software  
┣ Images  
┃ ┗ LiDARexample.png  
┣ **PC --> VIO & Mapping**  
┃ ┗  
┣ .gitignore  
┣ LICENSE  
┣ README.md  
┗ todo.md  
