
# STM32F103VCT6-LQFP100

 * http://www.st.com/en/microcontrollers/stm32f103vc.html
 * Power: 3.3V
 * Frequency: 8MHz crystal

## Memory map

### RAM

 * 48kB SRAM in total
 * 0x20000000 - 0x2000C000

### Flash

 * 256kB in total
 * 0x08000000 - 0x08040000

### Sections

 * 0x08000000 - 0x08003FFF: Bootloader
 * 0x08004000 - 0x0800BFFF: System
 * 0x0800C000 - 0x08013FFF: Application #1
 * 0x08014000 - 0x0801BFFF: Application #2
 * 0x0801C000 - 0x08023FFF: Application #3
 * 0x08024000 - 0x0802BFFF: Application #4
 * 0x0802C000 - 0x0803C9F8: FPGA bitstream (68.088 bytes)
 * 0x0803C9F8 - 0x0803FFFF: unused(?)
