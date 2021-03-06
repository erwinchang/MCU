## Arduino Tutorial

* [Arduino Tutorial](http://www.ladyada.net/learn/arduino/)

## ATMEL MCU Example

- [WinAVR使用指南][2]

在Windows平台上，免費的AVR編程軟體主要有兩個：AVR Studio和WinAVR。他們都使用GCC作為編譯器。前者具有一個完整的IDE和軟體硬體，但體積臃腫；後者簡單高效，但操作較為複雜。

- USBASP win7 32bit driver 及燒錄方式

driver 請選用usbasp_driver_and_Progisp_and_AVR_fighter/usbasp/usbasp/driver

請使用Progisp燒錄

目前使用AVR_fighter會遇到Flash校驗失敗問題


- 請先下載[WinAVR-20100110][1]
或是由atmel tools目錄內下載

### ATMEL Example使用方式

- make 產生.bin

- make clean：清除bin

## 範例來源

attiny85/test1-led： [Getting started with ATtiny85 (AVR Programming)][6]

## Makefile

- ATTINY85 Makefile

 [參考來源][3]

- 其它可以參考Makefile

 [crosspack][5]

 [CrossPack-AVR][4]

--------------------------

## Ubuntu 開發環境建立

### toolchain

#### Required Tools
* avr-gcc
    * [Install avr-libc][12]
     gcc version 4.9.2 (GCC)


#### Optional Tools
* AVRDUDE
    * AVRDUDE is software for programming Atmel AVR Microcontrollers.
    * [How to Install AVRDude 6.3][10]
    * [AVR Downloader/UploaDEr][11]
    avrdude version 6.3, URL: <http://savannah.nongnu.org/projects/avrdude/>

-------------------

## USBasp

[Using USBTinyISP to program ATTiny45 and ATTiny85][13]


### 設定udev，讓avrdude不需要使用sudo

```
erwin@erwin-pc150: /etc/udev/rules.d
$ ls
10-usbasp.rules  70-persistent-net.rules  README
erwin@erwin-pc150: /etc/udev/rules.d
$ cat 10-usbasp.rules 
SUBSYSTEM=="usb", SYSFS{idVendor}=="16c0", SYSFS{idProduct}=="05dc", GROUP="adm", MODE="0666"
```

* 記得動啟才會套用設定檔
sudo restart udev


來源：
* [avrisp mk II Rechteproblem][14]
* [Run avrdude on Ubuntu without root][15]
* [averdude requires sudo for USBasp - a fix][16]


### USBasp測試

* 需要更新USBasp軟体
```
$ sudo avrdude -c USBasp  -p attiny85 -U flash:w:main.hex:i                                                                                                                              

avrdude: warning: cannot set sck period. please check for usbasp firmware update.
avrdude: error: program enable: target doesn't answer. 1 
avrdude: initialization failed, rc=-1
         Double check connections and try again, or use -F to override
         this check.


avrdude done.  Thank you.
```

來源：
* [USBasp Firmware~串口燒燒燒][17]
* [How to Update the Firmware on a USBasp V2.0][18]

-------------------

## 7688Duo

* 目前確定無法燒錄，改使用OpenWrt燒錄正常
* [Software and Tools][29]
    * OpenWrt SDK for C/C++ for Linux
    * C/C++ Toolchain for Linux
* [Beginners' Guide to OpenWrt][23]
* [Tutorials][24]
* [此處收集LinkIt Smart 7688相關][26]
    * [LinkIt Smart 7688 and MCS][27]
    * [LASS - Linkit Smart 7688 Duo][28]

注意：
* 若由Arduino燒錄失敗，記得先再OpenWrt燒錄MCU的bootload，之後就會在ubuntu下產生/dev/ttyACM2等, 再使用Arduino燒一次就會成功
* 再Arduino裡面的Serial.print()，記得開啟Serial Monitor即可查看

[Linkit Smart 7688 Duo上使用PMS3003 (G3) 空汙感測器][33]

### t1-led

```
xx/test1-led on master [?]
$ sudo make flash                                                                                                                                                                        
avrdude -cavr109 -P/dev/ttyACM1 -b57600 -p atmega32u4 -U flash:w:main.hex:i

Connecting to programmer: .avrdude: butterfly_recv(): programmer is not responding
```

### t2-led

```
avrdude -C../LinkIt/hardware/avr/0.1.5/avrdude.conf -cavr109 -P/dev/ttyACM1 -b57600 -p atmega32u4 -U flash:w:main.hex:i

Connecting to programmer: .avrdude: butterfly_recv(): programmer is not responding
```

### 改由OpenWrt燒錄正常

```
avrdude -p m32u4 -c linuxgpio -v -e -U flash:w:main.hex -U lock:w:0x0f:m
```

來源：[Linkit Smart7688 Duo Arduino Bootloader 解磚 ][20]


### 7688

![7688](http://i.imgur.com/03tnDTP.png)

![7688 Power](http://i.imgur.com/lANndOA.png)

* USB Host
    * 接頭為Micro-AB
    * 直接接到SOC的USB_DP/USB_DM
    * 提供5V_USB

* USB Power
    * 接頭為Micro-B
    * 直接到MCU的D-/D+
    * 提供5V_USB

一般只接USB_Power提供5V_USB，經XC6220B331P(1A LDO Voltage)產生3.3V
若電源不足記得再接USB Host來供電


Power Consumption

![Power Consumption](http://i.imgur.com/ysmXuMd.png)

### 燒錄 MCU bootload

* 當將USB Power插入電腦USB Port若未出現(/dev/ttyACM0)，此時需要燒錄bootload如下(使用OpenWrt內建的程式)
```
avrdude -c linuxgpio -C /etc/avrdude.conf -p m32u4 -v -e -U flash:w:/etc/arduino/Caterina-smart7688.hex
```

* 參數說明
```
-c <programmer> 由於MCU的燒錄腳位都接到SOC，因此直接控制gpio即可燒錄程式
-C <config-file> 設定相關配置
-p <partnom> 指定device
-v Verbose output
-e Perform a chip erase
-U <memtyp>:r|w|v:<filename>
```

* avrdude.conf內容如下
```
part
    id               = "m32u4";
    desc             = "ATmega32U4";
    signature        = 0x1e 0x95 0x87;
    usbpid           = 0x2ff4;
...

    memory "flash"
        paged           = yes;
        size            = 32768;
        page_size       = 128;
        num_pages       = 256;
        min_write_delay = 4500;
```

來源：
* [Update Arduino Bootloader][30]
* [EduBot][31]

### 如何取得 arduino hex檔

* 取得avrdude狀態
使用ps -ef | grep avrdude
```
xx/avrdude -C/root/.arduino15/packages/LinkIt/hardware/avr/0.1.5/avrdude.conf -q -q -patmega32u4 -cavr109 -P/dev/ttyACM0 -b57600 -D -Uflash:w:/tmp/build2842119949771062043.tmp/smart7688-t1.cpp.hex:i
```

* 產生hex，用到相關檔案
內有CDC.cpp (即USBAPI，通過arduino燒錄到MCU相關程式)
```
$ ls -lh /tmp/build2842119949771062043.tmp/
abi.cpp.d              HardwareSerial1.cpp.d  HID.cpp.d              new.cpp.d              smart7688-t1.cpp.hex   WInterrupts.c.d        wiring_pulse.c.d       WString.cpp.o
abi.cpp.o              HardwareSerial1.cpp.o  HID.cpp.o              new.cpp.o              smart7688-t1.cpp.o     WInterrupts.c.o        wiring_pulse.c.o       
buildprefs.txt         HardwareSerial2.cpp.d  hooks.c.d              Print.cpp.d            Stream.cpp.d           wiring_analog.c.d      wiring_pulse.S.o       
CDC.cpp.d              HardwareSerial2.cpp.o  hooks.c.o              Print.cpp.o            Stream.cpp.o           wiring_analog.c.o      wiring_shift.c.d       
CDC.cpp.o              HardwareSerial3.cpp.d  IPAddress.cpp.d        smart7688-t1.cpp       Tone.cpp.d             wiring.c.d             wiring_shift.c.o       
core.a                 HardwareSerial3.cpp.o  IPAddress.cpp.o        smart7688-t1.cpp.d     Tone.cpp.o             wiring.c.o             WMath.cpp.d            
HardwareSerial0.cpp.d  HardwareSerial.cpp.d   main.cpp.d             smart7688-t1.cpp.eep   USBCore.cpp.d          wiring_digital.c.d     WMath.cpp.o            
HardwareSerial0.cpp.o  HardwareSerial.cpp.o   main.cpp.o             smart7688-t1.cpp.elf   USBCore.cpp.o          wiring_digital.c.o     WString.cpp.d  
```

* 產生hex大小
```
$ ls -lh /tmp/build2842119949771062043.tmp/smart7688-t1.cpp.hex 
-rw-r--r-- 1 root root 17K Apr 19 10:16 /tmp/build2842119949771062043.tmp/smart7688-t1.cpp.hex
```

參考：[How to get hex file from arduino files][32]


### 7688 Duo Pin

![Pin](http://i.imgur.com/zk9yg20.png)

### Build Code

* [Using OpenWrt SDK to Build C/C++ Programs][25]



### 其它參考

* [LinkIt Smart 7688 Duo 之 Arduino COM埠閃退篇][19]
* [Linkit Smart7688 Duo Arduino Bootloader 解磚 ][20]
* [Using AVRDUDE from OpenWrt SDK][21]

-----------------------
##  ARDUINO UNO

### 確認版子的連接

avrdude -Cdmeavrdude.conf -v -patmega328p -carduino -P/dev/ttyACM0 -b115200

```
$ sudo avrdude -Cavrdude.conf -v -patmega328p -carduino -P/dev/ttyACM0 -b115200                                                                                                          

avrdude: Version 6.3
         Copyright (c) 2000-2005 Brian Dean, http://www.bdmicro.com/
         Copyright (c) 2007-2014 Joerg Wunsch

         System wide configuration file is "avrdude.conf"
         User configuration file is "/home/erwin/.avrduderc"
         User configuration file does not exist or is not a regular file, skipping

         Using Port                    : /dev/ttyACM0
         Using Programmer              : arduino
         Overriding Baud Rate          : 115200
         AVR Part                      : ATmega328P
...

avrdude: Device signature = 0x1e950f (probably m328p)
avrdude: safemode: hfuse reads as 0
avrdude: safemode: efuse reads as 0

avrdude: safemode: hfuse reads as 0
avrdude: safemode: efuse reads as 0
avrdude: safemode: Fuses OK (E:00, H:00, L:00)

avrdude done.  Thank you.

```

-----------------------
 ## 其它參考

 * [AVR Tutorials][7]
 * [Getting Started - AVR Microcontrollers][8]
 * [Tools and Software ATmega328P][9]


[1]:http://sourceforge.net/projects/winavr/
[2]:https://zh.wikibooks.org/zh-tw/WinAVR使用指南
[3]:https://gist.github.com/electronut/8a4c297213620958ebef
[4]:https://github.com/obdev/CrossPack-AVR/blob/master/templates/TemplateProject/firmware/Makefile
[5]:https://www.obdev.at/products/crosspack/index.html
[6]:http://electronut.in/getting-started-with-attiny85-avr-programming/
[7]:http://www.avr-tutorials.com
[8]:http://www.atmel.com/products/microcontrollers/avr/start_now.aspx
[9]:http://www.atmel.com/devices/atmega328p.aspx?tab=tools
[10]:http://ubuntuhandbook.org/index.php/2017/01/install-avrdude-6-4-ubuntu-16-04/
[11]:http://savannah.nongnu.org/projects/avrdude/
[12]:https://www.howtoinstall.co/en/ubuntu/xenial/avr-libc
[13]:http://www.arvydas.co.uk/2012/09/using-usbtinyisp-to-program-attiny45-and-attiny85/
[14]:https://forum.ubuntuusers.de/topic/arduino-avrisp-mk-ii-rechteproblem/
[15]:http://mightyohm.com/blog/2010/03/run-avrdude-without-root-privs-in-ubuntu/
[16]:http://forum.arduino.cc/index.php?topic=90581.0
[17]:https://dotblogs.com.tw/aliceyeh/2015/01/05/147946
[18]:http://blog.lincomatic.com/?p=1480
[19]:http://blog.cavedu.com/%E7%89%A9%E8%81%AF%E7%B6%B2/linkit/linkit-smart-7688-duo-%E4%B9%8B-arduino-com%E5%9F%A0%E9%96%83%E9%80%80%E7%AF%87/
[20]:http://storychen.blogspot.tw/2016/12/linkit-smart7688-duo-arduino-bootloader.html
[21]:https://docs.labs.mediatek.com/resource/linkit-smart-7688/en/tutorials/linkit-smart-7688-duo/linkit-smart-7688-duo-development-board-and-arduino-ide/updating-arduino-bootloader
[22]:https://docs.labs.mediatek.com/resource/linkit-smart-7688/en/downloads
[23]:http://wiki.openwrt.org/doc/howto/user.beginner
[24]:https://docs.labs.mediatek.com/resource/linkit-smart-7688/en/tutorials
[25]:https://docs.labs.mediatek.com/resource/linkit-smart-7688/en/tutorials/c-c++-programming/using-openwrt-sdk-to-build-c-c++-programs
[26]:https://mediatek-labs.gitbooks.io/linkit-smart-7688/content/
[27]:https://mcs.mediatek.com/7688/
[28]:https://hackpad.com/LASS-Linkit-Smart-7688-Duo-1HcedS5HQnI
[29]:https://docs.labs.mediatek.com/resource/linkit-smart-7688/en/downloads
[30]:https://mediatek-labs.gitbooks.io/linkit-smart-7688/content/duo-update-arduino-bootloader.html
[31]:https://sites.google.com/a/fssh.khc.edu.tw/edumot/7688
[32]:https://forum.arduino.cc/index.php?topic=131655.0
[33]:http://lass-net.org/2016/03/implement-pms3003-g3-with-linkit-smart-7688-duo/
