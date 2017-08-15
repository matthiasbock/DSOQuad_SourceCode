/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : main.c  
 Version   : DS203_SYS Ver 1.5x                                  Author : bure
*******************************************************************************/
#include "Function.h"
#include "USB_lib.h"
#include "Config.h"
#include "Ident.h"
#include "BIOS.h"
#include "LCD.h"
#include "ASM.h"
#include "USB_pwr.h"

typedef  void (*pFunc)(void);

pFunc Jump_To_App;
u32 JumpAddr;

/*******************************************************************************
  �汾�޸�˵��
SYS V1.24: �޸���144MHz�������ģʽ�µĲ�ֵϵ��(BIOS.c)
           �޸���V2.6Ӳ����·��ģ���������̵�λ(BIOS.c)
SYS V1.30: ��Licenceʶ����ת�Ƶ���DFUģ����(ASM.s,Ident.c)
           �޸���LicenceȨ�޿�������(Main.c, Ident.c)
           �޸���DFU,SYS�⺯������������÷�ʽ(ASM.s, VECTOR.c)
           ������µ�SYS�⺯��(BIOS.c)
SYS V1.31: ���ڲ�U�̵ĳ�ʼ���Ƴٵ�Licenceʶ������(Main.c,Config.c)
           �޸���U��������д����(SPI_flash.c)
           �޸���SYS�⺯������������ʼ��ַ
SYS V1.32: �޸���Set(T_BASE_ARR, Value)�����е�BUG(BIOS.c)
SYS V1.33: �����˶�LCD����оƬILI9327��R61509V�ļ��ݺ��Զ�ʶ��(LCD.c, ASM.s)
SYS V1.34: �Ӹð汾�� �ɲ���ʹ�� IAR 4.42�룵.���汾���뻷��(ASM.s)
SYS V1.35: �޸���LCD����оƬ��������ص�BUG(ASM.s)         
           �������豸Ӳ����SYS�̼���DFU�汾��Ϣ����(ASM.s, BIOS.c, Main.c)
           �����˸߾��ȵ�A,Bͨ����ֱƫ�����ù��ܺ���(BIOS.c)
SYS V1.37: �޸��˸߾��ȵ�A,Bͨ����ֱƫ�����ù��ܺ�����BUG(BIOS.c)        
           ������FPGA2.60���ϰ汾�İ汾��Ϣ����(BIOS.c)
SYS V1.40: ������U���ļ�ϵͳ��д��غ���(DOSFS.c)        
SYS V1.41: �޸��˶�BUF�ļ�ʱ��BUG(DOSFS.c)
SYS V1.42: ������Row_Fill����(ASM.s)
SYS V1.50: ��д���µ�FAT12�ļ�ϵͳ(FAT12.c, ASM.s)
           �޸���Flash��д�����ܿ���PC���Ķ�д��ͻ(Memory.c, EXT_Flash.c) 
*******************************************************************************/

uc8  SYS_Ver[5]  = "1.50"; 

/*******************************************************************************
  main : Main routine.
*******************************************************************************/
int main(void)
{ 
  u32 Licence;
  u8  NumStr[9];
  
  Set_System();
  NVIC_Config();
  GPIO_Config();
  Timer_Config();
  FSMC_Config();
  SPI_Config();

  DMA_Config();
  ADC_Config();
  DAC_Config();
  
  Beep(20, 500); 
  LCD_Initial();
  Clear_Screen(0x0000);

  Display_Logo(70,150); 
  
  Display_Str( 5*8, 90, GRN, PRN,  "Hardware Ver " );
  Display_Str(18*8, 90, GRN, PRN,  (u8*)Get(HDWVER, 0));
  Display_Str(25*8, 90, GRN, PRN, "Serial No: ");
  Word2Hex(NumStr, __Dev_SN());            
  Display_Str(36*8, 90, YEL, PRN, NumStr);      // ��ʾ��ȡ�����豸���к�

  Display_Str( 6*8, 70, GRN, PRN, "     DS203 Mini DSO SYS Ver            ");
  Display_Str(34*8, 70, GRN, PRN, (u8*)Get(SYSVER, 0)); // ��ʾSYS�̼��汾��
  
  if((KEY2_OFF)&&(KEY3_OFF)&&(KEY4_OFF)){       // �����ް�������
    Key_Buffer = 0;                             // �������״̬����ʼ�ȴ��µİ���
    Licence = Get_Lic(SYS_LIC_OFFSET);          // Get master licence
    if(__Chk_SYS(Licence)!= 1){                 // No master licence 
      Display_Str(6*8,50,GRN,PRN,     "Please input master licence: 00000000");
      Licence = Input_Lic(48+29*8, 50);         // Input Licence
      if(__Chk_SYS(Licence)!= 1){               // Licence error
        Display_Str(6*8,50,YEL,PRN,   "        Licence input error          ");
      } else {                                  // Licence ok
        if(Add_Lic(Licence, SYS_LIC_OFFSET)== 1)// Licence writen ok
          Display_Str(6*8,50,GRN,PRN, "         Licence writen ok           ");
        else  
          Display_Str(6*8,50,GRN,PRN, "        Licence writen error         ");
      }
      Display_Str(6*8,30,GRN,PRN,     "      Push any key to next step      ");
      while (Key_Buffer == 0){};
    }
    JumpAddr = *(vu32*) (APP1_BASE + 4);
    Jump_To_App = (pFunc) JumpAddr;
    __MSR_MSP(*(vu32*) APP1_BASE);
  } else if(KEY2_ON){                           // ��������K2��  
    JumpAddr = *(vu32*) (APP2_BASE + 4);
    Jump_To_App = (pFunc) JumpAddr;
    __MSR_MSP(*(vu32*) APP2_BASE);
  } else if(KEY3_ON){                           // ��������K3�� 
    JumpAddr = *(vu32*) (APP3_BASE + 4);
    Jump_To_App = (pFunc) JumpAddr;
    __MSR_MSP(*(vu32*) APP3_BASE);
  } else if(KEY4_ON){                           // ��������K4�� 
    JumpAddr = *(vu32*) (APP4_BASE + 4);
    Jump_To_App = (pFunc) JumpAddr;
    __MSR_MSP(*(vu32*) APP4_BASE);
  }
  USB_EN();
  USB_Init();  
  Jump_To_App();
}
/*********************************  END OF FILE  ******************************/
