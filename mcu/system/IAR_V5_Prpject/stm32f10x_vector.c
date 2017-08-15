/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_vector.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : STM32F10x vector table for EWARM4.x toolchain.
*                      This module performs:
*                      - Set the initial SP
*                      - Set the initial PC == __program_start,
*                      - Set the vector table entries with the exceptions ISR address,
*                     - Configure external SRAM mounted on STM3210E-EVAL board
*                       to be used as data memory (optional, to be enabled by user)
*                      After Reset the Cortex-M3 processor is in Thread mode,
*                      priority is Privileged, and the Stack is set to Main.
*******************************************************************************/
#include "../../system/include/Interrupt.h"
#include "stm32f10x_lib.h"

typedef void( *intfunc )( void );
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

#pragma language=extended
#pragma segment="CSTACK"

void __iar_program_start( void );
#pragma location = ".intvec"

//======================= STM32F10x Vector Table entries =======================
const intvec_elem __vector_table[] =
{
  { .__ptr = __sfe( "CSTACK" ) },
  __iar_program_start,
  NMIException,
  HardFaultException,
  MemManageException,
  BusFaultException,
  UsageFaultException,
  0, 0, 0, 0,            /* Reserved */ 
  SVCHandler,
  DebugMonitor,
  0,                      /* Reserved */
  PendSVC,
  SysTickHandler,
  WWDG_IRQHandler,
  PVD_IRQHandler,
  TAMPER_IRQHandler,
  RTC_IRQHandler,
  FLASH_IRQHandler,
  RCC_IRQHandler,
  EXTI0_IRQHandler,
  EXTI1_IRQHandler,
  EXTI2_IRQHandler,
  EXTI3_IRQHandler,
  EXTI4_IRQHandler,
  DMA1_Channel1_IRQHandler,
  DMA1_Channel2_IRQHandler,
  DMA1_Channel3_IRQHandler,
  DMA1_Channel4_IRQHandler,
  DMA1_Channel5_IRQHandler,
  DMA1_Channel6_IRQHandler,
  DMA1_Channel7_IRQHandler,
  ADC1_2_IRQHandler,
  USB_HP_CAN_TX_IRQHandler,
  USB_LP_CAN_RX0_IRQHandler,
  CAN_RX1_IRQHandler,
  CAN_SCE_IRQHandler,
  EXTI9_5_IRQHandler,
  TIM1_BRK_IRQHandler,
  TIM1_UP_IRQHandler,
  TIM1_TRG_COM_IRQHandler,
  TIM1_CC_IRQHandler,
  TIM2_IRQHandler,
  TIM3_IRQHandler,
  TIM4_IRQHandler,
  I2C1_EV_IRQHandler,
  I2C1_ER_IRQHandler,
  I2C2_EV_IRQHandler,
  I2C2_ER_IRQHandler,
  SPI1_IRQHandler,
  SPI2_IRQHandler,
  USART1_IRQHandler,
  USART2_IRQHandler,
  USART3_IRQHandler,
  EXTI15_10_IRQHandler,
  RTCAlarm_IRQHandler,
  USBWakeUp_IRQHandler,
  TIM8_BRK_IRQHandler,
  TIM8_UP_IRQHandler,
  TIM8_TRG_COM_IRQHandler,
  TIM8_CC_IRQHandler,
  ADC3_IRQHandler,
  FSMC_IRQHandler,
  SDIO_IRQHandler,
  TIM5_IRQHandler,
  SPI3_IRQHandler,
  UART4_IRQHandler,
  UART5_IRQHandler,
  TIM6_IRQHandler,
  TIM7_IRQHandler,
  DMA2_Channel1_IRQHandler,
  DMA2_Channel2_IRQHandler,
  DMA2_Channel3_IRQHandler,
  DMA2_Channel4_5_IRQHandler,
/*  
  __CTR_HP,
  __USB_Istr,
  __USB_Init,
  __Point_SCR,
  __LCD_SetPixl,
  __LCD_GetPixl,
  __LCD_Copy,
  __LCD_Fill,
  __Row_Copy,
  __Read_FIFO,
  __Disk_Buff_WR,
  __Disk_Buff_RD,
*/  
  
};
#ifdef DATA_IN_ExtSRAM
#pragma language=extended

__interwork int __low_level_init(void);

#pragma location="ICODE"
__interwork int __low_level_init(void)
{
   
/* FSMC Bank1 NOR/SRAM3 is used for the STM3210E-EVAL, if another Bank is 
  required, then adjust the Register Addresses*/

  /* Enable FSMC clock */
  *(vu32 *)0x40021014 = 0x00000114;
  
  /* Enable GPIOD, GPIOE, GPIOF and GPIOG clocks */  
  *(vu32 *)0x40021018 = 0x000001E0;
  
/* ---------------  SRAM Data lines, NOE and NWE configuration ---------------*/
/*----------------  SRAM Address lines configuration -------------------------*/
/*----------------  NOE and NWE configuration --------------------------------*/  
/*----------------  NE3 configuration ----------------------------------------*/
/*----------------  NBL0, NBL1 configuration ---------------------------------*/
  
  *(vu32 *)0x40011400 = 0x44BB44BB;
  *(vu32 *)0x40011404 = 0xBBBBBBBB;
  
  *(vu32 *)0x40011800 = 0xB44444BB;
  *(vu32 *)0x40011804 = 0xBBBBBBBB;
   
  *(vu32 *)0x40011C00 = 0x44BBBBBB;
  *(vu32 *)0x40011C04 = 0xBBBB4444;  

  *(vu32 *)0x40012000 = 0x44BBBBBB;
  *(vu32 *)0x40012004 = 0x44444B44;
  
/*----------------  FSMC Configuration ---------------------------------------*/  
/*----------------  Enable FSMC Bank1_SRAM Bank ------------------------------*/
  
  *(vu32 *)0xA0000010 = 0x00001011;
  *(vu32 *)0xA0000014 = 0x00000200;
  

  return (1);
}
#endif /*DATA_IN_ExtSRAM*/

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

