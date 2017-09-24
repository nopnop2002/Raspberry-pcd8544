# pcd8544_rpi
pcd8544(NOKIA5110) Command Line Tool for Raspberry Pi / Orange Pi

You can operate from command line.  

Command line parameters:  
+1 String : String for #1 line(with External 4X8 Font)  
+2 String : String for #2 line(with External 4X8 Font)  
+3 String : String for #3 line(with External 4X8 Font)  
+4 String : String for #4 line(with External 4X8 Font)  
+5 String : String for #5 line(with External 4X8 Font)  
+6 String : String for #6 line(with External 4X8 Font)  
+a String : String for #1 line(with Internal 6X8 Font)  
+b String : String for #2 line(with Internal 6X8 Font)  
+c String : String for #3 line(with Internal 6X8 Font)  
+d String : String for #4 line(with Internal 6X8 Font)  
+e String : String for #5 line(with Internal 6X8 Font)  
+f String : String for #6 line(with Internal 6X8 Font)  
-1 : delete #1 line  
-2 : delete #2 line  
-3 : delete #3 line  
-4 : delete #4 line  
-5 : delete #5 line  
-6 : delete #6 line  
+L   : Scroll Up 1Line  
-L   : Scroll Down 1Line  
P1 n : Set start colum n to line#1  
P2 n : Set start colum n to line#2  
P3 n : Set start colum n to line#3  
P4 n : Set start colum n to line#4  
P5 n : Set start colum n to line#5  
P6 n : Set start colum n to line#6  
r  : remove all string  
s  : show display  

You can use within script.  
#!/bin/bash  
./nokia r  
./nokia +1 "ABCDEFG"  
./nokia +2 "abcdefg"  
./nokia +3 "1234567"  
./nokia +d "ABCDEFG"  
./nokia +e "abcdefg"  
./nokia +f "1234567"  
sudo ./nokia s  

---

Wire connection

|NOKIA510|---|RPi/OPi||
|:-:|:-:|:-:|:-:|
|1 RST|---|GPIO23|(Pin#16)|
|2 CE|---|SPI CE0|(Pin#24)|
|3 DC|---|GPIO24|(PIn#18)|
|4 DIN|---|SPI MOSI|(Pin#19)|
|5 CLK|---|SPI SCLK|(Pin#23)|
|6 VCC|---|3.3V||
|7 BL|---|3.3V/GND(*)||
|8 GND|---|GND||

(*)There are both module of GND and 3.3V.  
Blue module supplies a backlight pin with 3.3V, a backlight lights up.   
Red module connects a backlight pin to GND, a backlight lights up.   

---

Install  
git clone https://github.com/nopnop2002/pcd8544_rpi   
cd pcd8544_rpi/  
cc -o nokia nokia.c fontx.c pcd8544.c -lwiringPi  
bash ./test.sh  

---
Line#1-#3 : External 4X8 Font   
Line#5-#6 : Internal 6X8 Font   

![pcd8544-11](https://cloud.githubusercontent.com/assets/6020549/25205698/ee9c2584-259e-11e7-9205-8c5fa0f88c0f.JPG)

![pcd8544-12](https://cloud.githubusercontent.com/assets/6020549/25205703/f1b3f3e6-259e-11e7-8367-b39dec00e196.JPG)

