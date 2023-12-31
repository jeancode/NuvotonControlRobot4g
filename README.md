# NK-N9H30

## Sample video
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/eU1D-Tij6EA/0.jpg)](https://www.youtube.com/watch?v=eU1D-Tij6EA)

## Video of the operation of the robot with touch control
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/QvbEQ5q8GA8/0.jpg)](https://www.youtube.com/watch?v=QvbEQ5q8GA8)


## instructions for preparation

### install nuvoton board in rt-thread IDE

![NA](./n1.PNG)

### Create a project for the nuvorton board

![NA](./n2.PNG)

### Copy the applications folder from the repository to your project folder

![NA](./n3.PNG)


### compile the code and follow the link to find out how to upload the firmware

![NA](./n5.PNG)

[more information to upload your code to the board](https://github.com/jeancode/Ovni_NuvotonN9h30)




### Run the tcp server remember to change the url to your online server
![NA](./n6.PNG)


### Use plaformio to upload the code to esp32
![NA](./n7.PNG)

### the esp32 arduino code is compatible with Rtduino as well as the 4g connection module
![NA](./n8.PNG)


### the pins refer to the control of the dives and the 4g module

![NA](./n9.PNG)

## 1.installation instructions
Nuvoton offers HMI platforms which are embedded with Nuvoton N9H MPU.  The N9H series with ARM926EJ-S core can operate at up to 300 MHz and can drive up to 1024x768 pixels in parallel port. It integrated TFT LCD controller and 2D graphics accelerator, up to 16 million colors (24-bit) LCD screen output, and provides high resolution and high chroma to deliver gorgeous display effects. It embedded up to 64 MB DDR memory, along with ample hardware storage and computing space for excellent design flexibility.

[![NK-N9H30](https://i.imgur.com/B04MCCf.png "NK-N9H30")](https://i.imgur.com/B04MCCf.png "NK-N9H30")



### 1.1 MPU specification
|  | Features |
| -- | -- |
| Part NO. | N9H30F61IEC(or N9H30F63IEC) (LQFP216 pin MCP package with DDR (64 MB) |
| CPU ARCH. | 32-bit ARM926EJ-S |
| Operation frequency | 300 MHz |
| Embedded SDRAM size | Built-in 64MB |
| Crypto engine |  AES, DES,HMAC and SHA crypto accelerator |
| RMII interface |  10/100 Mbps x2 |
| USB 2.0 |  High Speed Host/Device x1 |
| Audio |  Mono microphone / Stereo headphone |
| Extern storage |  32MB SPI-NOR Flash |
| SD card slot |  SD |

**Notice: Please remember to select corresponding Part NO in NuWriter.**

### 1.2 Interface
| Interface |
| -- |
| Two RJ45 Ethernet |
| An USB 2.0 HS Dual role(Host/Device) port |
| A microSD slot |
| A 3.5mm Audio connector |
| An ICE connector |

### 1.3 On-board devices
| Device | Description | Driver supporting status |
| -- | -- | -- |
|Ethernet PHY | IP101GR | Supported |
|Keypad | K[1, 6] | Supported |
|LEDs |  | Supported |
|TFT-LCD panel | 7" inch 24b RGB  | Supported |
|Touch panel | 7" inch resistive | Supported |
|Audio Codec | NAU8822, Supports MIC and earphone | Supported |
|USB Device | VCOM + MStorage | Supported |
|USB Host | MStorage | Supported |
|SPI NOR flash | W25Q256JVEQ (32 MB) | Supported |
