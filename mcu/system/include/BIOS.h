/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : BIOS.h  
 Version   : DS203_SYS Ver 1.5x                                 Author : bure
*******************************************************************************/
#ifndef __BIOS_H
#define __BIOS_H

#include "stm32f10x_lib.h"

//=================== Ӳ���汾��ض��� ===================

  #define HDW_VERSION       "Hardware Ver 2.6  "

//==================== Flash �ռ���� ====================

#define BIN_BASE                ((u32)(0x0802C000)) // Size < 68KB  
#define PRM_BASE                BIN_BASE + 68*1024  // Size =  2KB   
#define INF_BASE                BIN_BASE + 70*1024  // Size < 10KB   
#define APP4_BASE               ((u32)(0x08024000)) // Size = 32KB  
#define APP3_BASE               ((u32)(0x0801C000)) // Size = 32KB  
#define APP2_BASE               ((u32)(0x08014000)) // Size = 32KB  
#define APP1_BASE               ((u32)(0x0800C000)) // Size = 32KB  
#define SYS_BASE                ((u32)(0x08004000)) // Size = 32KB   
#define DFU_BASE                ((u32)(0x08000000)) // Size = 16KB 

//====================== ���� Set �� Object �� Value ���� ======================

#define CH_A_OFFSET       0     // Aͨ����ֱλ��  Value = 0~200
#define CH_B_OFFSET       1     // Bͨ����ֱλ��  Value = 0~200
#define BACKLIGHT         2     // ��������       Value = 0~100
#define BEEP_VOLUME       3     // ����������     Value = 0~100
#define BETTERY_DT        4     // ��ص�ѹ���   Value = 1: ����
#define ADC_MODE          5     // ADC����ģʽ    Value = 1/0
#define FIFO_CLR          6     // FIFOָ�븴λ   Value = 1/0: W_PTR/R_PTR
#define T_BASE_PSC        7     
#define T_BASE_ARR        8     
#define CH_A_COUPLE       9     //                Value = 1/0: AC/DC
#define CH_A_RANGE       10     //                Value = 0~5
#define CH_B_COUPLE      11     //                Value = 1/0: AC/DC
#define CH_B_RANGE       12     //                Value = 0~5
#define ANALOG_ARR       13     // ģ�������Ƶϵ��
#define ANALOG_PTR       14     // ģ�����������ָ��
#define ANALOG_CNT       15     // ģ�����ÿ���ڵ���
#define DIGTAL_PSC       16     // �������Ԥ��Ƶϵ��
#define DIGTAL_ARR       17     // ���������Ƶϵ��
#define DIGTAL_CCR       18     // �������ռ�ձ�ϵ��
#define KEY_IF_RST       19     // ��ʱ�жϱ�־��λ
#define STANDBY          20     // ���뽵���ĵȴ�״̬
#define FPGA_RST         31     // FPGA ��λ

#define TRIGG_MODE       32+0  // ����ģʽ
#define V_THRESHOLD      32+1  // ��ѹ��������
#define T_THRESHOLD      32+2  // �����ȴ���ʱ������
#define ADC_CTRL         32+4  // ADC ����״̬����
#define A_POSITION       32+5  // ��ǰCH_A�����λ��
#define B_POSITION       32+6  // ��ǰCH_B�����λ��
#define REG_ADDR         32+7  // �õ�ַ������FPGA����һ��Ĵ��������ݱ�MCU����

//==================== ���� Set �� TRIGG_MODE �� Value ����=====================
/*
CH_A Trigger source & kind select => 
0x00: by Negedge;   0x01: by Posedge;   0x02: by low level; 0x03: by high level
0x04: TL < Delta_T; 0x05: TL > Delta_T; 0x06: TH < Delta_T; 0x07: TH > Delta_T;

CH_B Trigger source & kind select =>
0x08: by Negedge;   0x09: by Posedge;   0x0A: by low level; 0x0B: by high level
0x0C: TL < Delta_T; 0x0D: TL > Delta_T; 0x0E: TH < Delta_T; 0x0F: TH > Delta_T;

CH_C Trigger source & kind select =>
0x10: by Negedge;   0x11: by Posedge;   0x12: by low level; 0x13: by high level
0x04: TL < Delta_T; 0x05: TL > Delta_T; 0x06: TH < Delta_T; 0x07: TH > Delta_T;

CH_D Trigger source & kind select =>
0x18: by Negedge;   0x19: by Posedge;   0x1A: by low level; 0x1B: by high level
0x1C: TL < Delta_T; 0x1D: TL > Delta_T; 0x1E: TH < Delta_T; 0x1F: TH > Delta_T;

0x20~0xFF  =>  Unconditional trigger
*/
#define UNCONDITION       0x20        // ��������������        

//===================== ���� Set �� ADC_CTRL �� Value ���� =====================

#define ADC_DN            0        // ADC����
#define ADC_EN            1        // ADC����

//===================== ���� Set �� ADC_MODE �� Value ���� =====================
 
#define SEPARATE          0        // ADC��������ģʽ
#define INTERLACE         1        // ADC�������ģʽ

//======================== FPGA���ݶ�д�����ź�H_L���� =========================

#define LOW               0
#define HIGH              1

//===================== ���� Set �� FIFO_CLR �� Value ���� =====================

#define R_PTR             0        // FIFO����ַָ��
#define W_PTR             1        // FIFOд��ַָ��

//==================== ���� Set_ �� CH_X_COUPLE �� Value ���� ==================

#define DC                0
#define AC                1

//======================== ���� Get �е� Object ���� ===========================

#define FIFO_DIGIT     0        // 16bits FIFO digital data
#define FIFO_EMPTY     1        // FIFO empty flag: 1 = empty
#define FIFO_START     2        // FIFO start flag: 1 = start
#define FIFO_FULL      3        // FIFO full flag: 1 = Full
#define KEY_STATUS     4        // Current keys status
#define USB_POWER      5        // USB power status: 1 = Power ON
#define V_BATTERY      6        // Battery voltage (mV)
#define VERTICAL       7        // ��ֱͨ������ָ��
#define HORIZONTAL     8        // ˮƽͨ������ָ��
#define GLOBAL         9        // ��������ָ��
#define TRIGGER        10       // ����ͨ������ָ��
#define FPGA_OK        11       // FPGA ���óɹ�
#define CHARGE         12       // ��س��״̬
#define HDWVER         13       // �豸Ӳ���汾�� 
#define DFUVER         14       // DFU����ģ��汾�� 
#define SYSVER         15       // SYS����ģ��汾�� 
#define FPGAVER        16       // FPGA���ó���汾�� 

#define ADC_DATA       32+0     // 0~7:ADC_CH_A 8~15:ADC_CH_B 16~17:CH_C&CH_D 
#define PRE_SAMPLING   32+1     // 0~15:Pre-sampling depth
#define ALL_SAMPLING   32+2     // 0~15:Total sampling depth
#define CH_A_MIN_MAX   32+3     // 0~7:VMIN 8~15:VMAX 
#define CH_A_V_SUM     32+4     // 0~15:CH_A voltage sum
#define CH_A_V_SSQ     32+5     // 0~15:CH_A voltage sum of squares
#define CH_A_NEDGE     32+6     // 0~15:CH_A number of edge
#define CH_A_FREQ      32+7     // 0~15:CH_A frequence
#define CH_A_PHW_MAX   32+8     // 0~15:CH_A pulse high width MAX
#define CH_A_PHW_MIN   32+9     // 0~15:CH_A pulse high width MIN
#define CH_A_PLW_MAX   32+10    // 0~15:CH_A pulse low width MAX
#define CH_A_PLW_MIN   32+11    // 0~15:CH_A pulse low width MIN

//======================= ģ��ͨ��(˥��/����)��λ�����ź� ======================

  #define _50MV             0
  #define _100MV            1
  #define _200MV            2
  #define _500MV            3
  #define _1V               4
  #define _2V               5
  #define _5V               6
  #define _10V              7
  #define CH_A              8
  #define CH_B              8

//============================= ϵͳ������IO�Ŷ��� =============================

#define LCD_nRST_LOW()    GPIOD_BRR  = GPIO_Pin_11
#define LCD_nRST_HIGH()   GPIOD_BSRR = GPIO_Pin_11
#define LCD_RS_LOW()      GPIOD_BRR  = GPIO_Pin_12
#define LCD_RS_HIGH()     GPIOD_BSRR = GPIO_Pin_12

#define FIFO_SCK_LOW()    GPIOA_BRR  = GPIO_Pin_6
#define FIFO_SCK_HIGH()   GPIOA_BSRR = GPIO_Pin_6
#define FIFO_SDA_LOW()    GPIOA_BRR  = GPIO_Pin_7
#define FIFO_SDA_HIGH()   GPIOA_BSRR = GPIO_Pin_7

#define FIFO_CLRW_LOW()   GPIOB_BRR  = GPIO_Pin_1
#define FIFO_CLRW_HIGH()  GPIOB_BSRR = GPIO_Pin_1
#define FIFO_MODE_LOW()   GPIOC_BRR  = GPIO_Pin_3
#define FIFO_MODE_HIGH()  GPIOC_BSRR = GPIO_Pin_3
#define FIFO_CLRR_LOW()   GPIOC_BRR  = GPIO_Pin_4
#define FIFO_CLRR_HIGH()  GPIOC_BSRR = GPIO_Pin_4
#define FIFO_H_L_LOW()    GPIOC_BRR  = GPIO_Pin_5
#define FIFO_H_L_HIGH()   GPIOC_BSRR = GPIO_Pin_5

#define SPI_SS_LOW()      GPIOB_BRR  = GPIO_Pin_10
#define SPI_SS_HIGH()     GPIOB_BSRR = GPIO_Pin_10
#define SPI_CLK_LOW()     GPIOB_BRR  = GPIO_Pin_11
#define SPI_CLK_HIGH()    GPIOB_BSRR = GPIO_Pin_11
#define SPI_MOSI_LOW()    GPIOB_BRR  = GPIO_Pin_12
#define SPI_MOSI_HIGH()   GPIOB_BSRR = GPIO_Pin_12
#define SPI_CRST_LOW()    GPIOB_BRR  = GPIO_Pin_14
#define SPI_CRST_HIGH()   GPIOB_BSRR = GPIO_Pin_14

#define CHRG_ST          !((GPIOB_IDR & 0x0100)>>8)
#define USB_PWR_ON        ((GPIOD_IDR & 0x0004)>>2)
#define FPGA_CFG_OK       (GPIOB_IDR >>15)

#define KEY1_OFF          ((GPIOA_IDR & 0x4000)!=0)
#define KEY2_OFF          ((GPIOC_IDR & 0x0400)!=0)
#define KEY3_OFF          ((GPIOC_IDR & 0x0200)!=0)
#define KEY4_OFF          ((GPIOC_IDR & 0x0100)!=0)
#define KEY1_ON           ((GPIOA_IDR & 0x4000)==0)
#define KEY2_ON           ((GPIOC_IDR & 0x0400)==0)
#define KEY3_ON           ((GPIOC_IDR & 0x0200)==0)
#define KEY4_ON           ((GPIOC_IDR & 0x0100)==0)

#define USB_EN()          GPIOA_CRH = 0x888BBBBB

#define STB_DN()    GPIOD_BSRR = GPIO_Pin_13; TIM1_CR1 = 0x0085 
#define STB_EN()    GPIOD_BRR  = GPIO_Pin_13; TIM1_CR1 = 0x0084 

//================= ģ��ͨ�������źŶ��� =================

  #define DC_1()        GPIOE_BSRR = GPIO_Pin_0
  #define AC_1()        GPIOE_BRR  = GPIO_Pin_0
  #define DC_2()        GPIOE_BSRR = GPIO_Pin_5
  #define AC_2()        GPIOE_BRR  = GPIO_Pin_5
  #define Ax0_OFF()     GPIOE_BRR  = GPIO_Pin_4
  #define Ax0_ON()      GPIOE_BSRR = GPIO_Pin_4
  #define Ax1_OFF()     GPIOB_BRR  = GPIO_Pin_9
  #define Ax1_ON()      GPIOB_BSRR = GPIO_Pin_9
  #define Ax2_LOW()     GPIOE_BRR  = GPIO_Pin_2
  #define Ax2_HIGH()    GPIOE_BSRR = GPIO_Pin_2
  #define Ax5_LOW()     GPIOE_BRR  = GPIO_Pin_3
  #define Ax5_HIGH()    GPIOE_BSRR = GPIO_Pin_3
//#define Ax10_OFF()    GPIOB_BSRR = GPIO_Pin_8
//#define Ax10_ON()     GPIOB_BRR  = GPIO_Pin_8
  #define Bx0_OFF()     GPIOC_BRR  = GPIO_Pin_13
  #define Bx0_ON()      GPIOC_BSRR = GPIO_Pin_13
  #define Bx1_OFF()     GPIOE_BRR  = GPIO_Pin_6
  #define Bx1_ON()      GPIOE_BSRR = GPIO_Pin_6
  #define Bx2_LOW()     GPIOC_BRR  = GPIO_Pin_14
  #define Bx2_HIGH()    GPIOC_BSRR = GPIO_Pin_14
  #define Bx5_LOW()     GPIOC_BRR  = GPIO_Pin_15
  #define Bx5_HIGH()    GPIOC_BSRR = GPIO_Pin_15

// =======================================================��

typedef struct  // Ӳ���ۺ����� 
{
  u16 LCD_X;    // ��Ļˮƽ��ʾ����
  u16 LCD_Y;    // ��Ļ��ֱ��ʾ����
  u16 Yp_Max;   // ��ֱ��λ���ֵ     
  u16 Xp_Max;   // ˮƽ��λ���ֵ     
  u16 Tg_Num;   // ������λ���ֵ
  u16 Yv_Max;   // ��ֱλ�����ֵ 
  u16 Xt_Max;   // ˮƽλ�����ֵ 
  u16 Co_Max;   // ��Ϸ�ʽ���ֵ 
  u8  Ya_Num;   // ģ��ͨ����Ŀ
  u8  Yd_Num;   // ����ͨ����Ŀ
  u8  INSERT;   // ��ʼӦ�ò�ֵ�ĵ�λ
  u16 KpA1;     // Aͨ��λ�Ʋ���ϵ��1
  u16 KpA2;     // Aͨ��λ�Ʋ���ϵ��2
  u16 KpB1;     // Bͨ��λ�Ʋ���ϵ��1
  u16 KpB2;     // Bͨ��λ�Ʋ���ϵ��2
} G_attr ;

typedef struct  // ��ֱͨ������ 
{
  u8  STR[8];   // ��λ��ʶ�ַ���
  s16 KA1;      // Aͨ��λ�����У��ϵ��1
  u16 KA2;      // Aͨ��б�����У��ϵ��2
  s16 KB1;      // Bͨ��λ�����У��ϵ��1
  u16 KB2;      // Bͨ��б�����У��ϵ��2
  u32 SCALE;    // ��ֱͨ������ϵ��
} Y_attr ;

typedef struct  // ˮƽͨ������ 
{
  u8  STR[8];   // ��λ��ʶ�ַ���
  s16 PSC;      // Ԥ��Ƶϵ��
  u16 ARR;      // ��Ƶϵ��
  u16 CCR;      // ռ�ձ�ϵ��
  u16 KP;       // ��ֵϵ��
  u32 SCALE;    // ˮƽͨ������ϵ��
} X_attr ; 

typedef struct  // ����ͨ������ 
{
  u8  STR[8];   // ������ʽ��ʶ�ַ���
  u8  CHx;      // ����ͨ�����
  u8  CMD;      // ������ʽ������
} T_attr ; 

extern uc8  SYS_Ver[5]; 
extern u8   VerStr[8];
extern u8   Clash;

extern const G_attr G_ATTR[1];
extern const Y_attr Y_ATTR[9];
extern const X_attr X_ATTR[27];
extern const T_attr T_ATTR[32];

extern Y_attr *Y_Attr; 
extern X_attr *X_Attr; 
extern G_attr *G_Attr; 
extern T_attr *T_Attr; 

void SendByte(u8 Data); 
void Set_Param(u8 RegAddr, u8 Parameter); 

u32  Get(u8 Object, u32 Value);
u32  Set(u8 Object, u32 Value);

#endif  

/*******************************  END OF FILE  ********************************/
