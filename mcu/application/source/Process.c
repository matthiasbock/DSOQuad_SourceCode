/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
 File Name : Process.c       
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/
#include "../../application/include/Process.h"

#include "../../app/include/BIOS.h"
#include "../../app/include/Draw.h"
#include "../../application/include/BIOS.h"
#include "../../application/include/File.h"
#include "../../application/include/Function.h"
#include "../../application/include/Interrupt.h"
#include "../../application/include/Menu.h"

u16 TaS, TbS, TcS, TdS;            // �����ۼ�
u16 PaS, PbS, PcS, PdS;            // �����ۼ�
u16 TaN, TbN, TcN, TdN;            // ���ڼ���
u8  a_Mid_H, a_Mid_L;
u8  b_Mid_H, b_Mid_L;

s8  Kab;                                     // ģ��ͨ�����ƽ��У��ϵ��
u32 a_Avg, b_Avg, a_Ssq, b_Ssq;              // ƽ��ֵ�ۼ�,ƽ�����ۼ�
u8  a_Max, b_Max, a_Min, b_Min;              // ԭʼ���ֵ,ԭʼ��Сֵ
s16 Posi_412, Posi_41, Posi_42, Posi_4_2, Posi_4F1, Posi_4F2, Posi_4F3, Posi_4F4;
s16 c_Max, d_Max, A_Posi, B_Posi;
u8  Full=1, Interlace;
u16 JumpCnt;

uc16 Wait[27]= {1000, 500, 200, 100, 50, 20, 10, 5, 2, 2,   
                2,      2,   2,   2,  2,  2,  2, 2, 2, 2,    
                2,      2,   2,   2,  2,  2,  2 };
                     
Y_attr *Y_Attr; 
X_attr *X_Attr; 
G_attr *G_Attr; 
T_attr *T_Attr; 

u32 DataBuf[4096];
u8  TrackBuff  [X_SIZE * 4];         // ���߹켣���棺i+0,i+1,i+2,i+3,�ֱ���1��4�Ź켣����

s8  Ka1[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // Aͨ����λ���У��ϵ��
s8  Kb1[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // Bͨ����λ���У��ϵ��
u16 Ka2[10] ={1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024}; // Aͨ���������У��ϵ��
u16 Kb2[10] ={1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024}; // Bͨ���������У��ϵ��
s8  Ka3[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // Aͨ����λ���У��ϵ��
s8  Kb3[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // Bͨ����λ���У��ϵ��

D_tab D_Tab[20] ={  // ���岨�����������, ����72MHz��Ƶ
//    STR      PSC     ARR       DUTY% 
  {" 10Hz ",  180-1,  40000-1,     50},
  {" 20Hz ",  180-1,  20000-1,     50},
  {" 50Hz ",  180-1,   8000-1,     50},
  {"!100Hz!", 180-1,   4000-1,     50},
  {"!200Hz!", 180-1,   2000-1,     50},
  {"!500Hz!", 180-1,    800-1,     50},
  {" 1KHz ",  180-1,    400-1,     50},
  {" 2KHz ",  180-1,    200-1,     50},
  {" 5KHz ",   18-1,    800-1,     50},
  {"!10KHz!",  18-1,    400-1,     50},
  {"!20KHz!",  18-1,    200-1,     50},
  {"!50KHz!",   1-1,   1440-1,     50},
  {"100KHz",    1-1,    720-1,     50},
  {"200KHz",    1-1,    360-1,     50},
  {"500KHz",    1-1,    144-1,     50},
  {" 1MHz ",    1-1,     72-1,     50},
  {" 2MHz ",    1-1,     36-1,     50},
  {" 4MHz ",    1-1,     18-1,     50},
  {" 6MHz ",    1-1,     12-1,     50},
  {" 8MHz ",    1-1,      9-1,     50}};

A_tab A_Tab[11] ={ // ģ�Ⲩ�����������, ����72MHz��Ƶ, ÿ����36��ϳ�
//    STR     PSC     ARR 
  {" 10Hz ",  20-1,  10000-1},
  {" 20Hz ",  20-1,   5000-1},
  {" 50Hz ",  20-1,   2000-1},
  {"!100Hz!", 20-1,   1000-1},
  {"!200Hz!", 20-1,    500-1},
  {"!500Hz!", 20-1,    200-1},
  {" 1KHz ",  20-1,    100-1},
  {" 2KHz ",  20-1,     50-1},
  {" 5KHz ",  20-1,     20-1},
  {"!10KHz!", 20-1,     10-1},
  {"!20KHz!", 20-1,      5-1}};

u16 SIN_DATA[36] =  // Sine wave data                                                                                                 //         
  {0x000,0x027,0x08E,0x130,0x209,0x311,0x441,0x58F,0x6F0,    // 90
   0x85A,0x9C0,0xB19,0xC59,0xD76,0xE68,0xF26,0xFAB,0xFF3,    // 180
   0xFFF,0xFD7,0xF70,0xECE,0xDF5,0xCED,0xBBD,0xA6F,0x90E,    // 270
   0x7A4,0x63E,0x4E5,0x3A5,0x288,0x196,0x0D8,0x053,0x00B,};  // 360   

u16 TRG_DATA[36] =  // triangle wave data
  {0x000,0x0E3,0x1C6,0x2AA,0x38D,0x471,0x554,0x638,0x71B,    // 90
   0x7FF,0x8E2,0x9C6,0xAA9,0xB8D,0xC70,0xD54,0xE37,0xF1B,    // 180
   0xFFE,0xF1B,0xE37,0xD54,0xC70,0xB8D,0xAA9,0x9C6,0x8E2,    // 270
   0x7FF,0x71B,0x638,0x554,0x471,0x38D,0x2AA,0x1C6,0x0E3};   // 360   
         
u16 SAW_DATA[36] =  // Sawtooth wave data                                                                                             //         
  {0x000,0x075,0x0EA,0x15F,0x1D4,0x249,0x2BE,0x333,0x3A8,    // 90
   0x41D,0x492,0x507,0x57C,0x5F1,0x666,0x6DB,0x750,0x7C5,    // 180
   0x83A,0x8AF,0x924,0x999,0xA0E,0xA83,0xAF8,0xB6D,0xBE2,    // 270
   0xC57,0xCCC,0xD41,0xDB6,0xE2B,0xEA0,0xF15,0xF8A,0xFFF};   // 360    


/*******************************************************************************
 App_init: ��ʾ���ڲ������ݳ�ʼ��
*******************************************************************************/
void App_init(void)
{ 
  u16 i, j;
  __Set(ADC_CTRL, EN );       
  __Set(STANDBY, DN);          // �˳�ʡ��״̬
  __Clear_Screen(BLACK);       // ����Ļ
  Delayms(20); 
  __Set(FIFO_CLR, W_PTR); 
  for(i=0; i<13; i++) for(j=0; j<4; j++) Title[i][j].Flag |= UPDAT;
  for(i=0; i<9; i++)                     Meter[i].Flag    |= UPDAT;
  if(Current != FILE) {
    Title[FILE][0].Flag &= !UPDAT;
    Title[FILE][1].Flag &= !UPDAT;
    Title[FILE][3].Flag &= !UPDAT;
  }  
  PD_Cnt = 600;
  Update = 1;                  // ���غ�ָ�ԭ���ĵ�λ����
}
/*******************************************************************************
 View_init: ��ʾ���ڲ������ݳ�ʼ��
*******************************************************************************/
void View_init(void)
{ 
  u16 i, j = 0, k = 0;
  for(i = 0; i < X_SIZE * 4; i += 4){
    TrackBuff[i] = 160;
    TrackBuff[i+1] = Get_Ref_Wave(i/4);
    if((i%160)==0) j = 1 - j;
    TrackBuff[i+2] = 60 + (j * 17);
    if((i%64)==0) k = 1 - k;
    TrackBuff[i+3] = 20 + (k * 17);
  }
}
/*******************************************************************************
 Update_Range: 
*******************************************************************************/
void Update_Range(void) 
{
  Interlace = 0;
  __Set(ADC_CTRL, EN);       
  __Set(ADC_MODE, SEPARATE);                        // Set Separate mode ((Range + 1)*25)
  __Set(CH_A_COUPLE, Title[TRACK1][COUPLE].Value);
  __Set(CH_A_RANGE,  Title[TRACK1][RANGE].Value);
  __Set(CH_A_OFFSET, ((1024 + Ka3[_A_Range])*_1_posi + 512)/1024);
//  if(_1_source == HIDE){
//    if(_T_base > 16){
//      Interlace = 1;                      
//      __Set(CH_A_RANGE, G_Attr[0].Yp_Max+1);        // Aͨ���ϲ���Bͨ�� 
//      __Set(ADC_CTRL, EN + 2);       
//      __Set(ADC_MODE, INTERLACE);                   // Set Interlace mode
//    }  
//  }
  __Set(CH_B_COUPLE, Title[TRACK2][COUPLE].Value);
  __Set(CH_B_RANGE,  Title[TRACK2][RANGE].Value);
  __Set(CH_B_OFFSET, ((1024 + Kb3[_B_Range])*_2_posi + 512)/1024);
//  if(_2_source == HIDE){
//    if(_T_base > 16){
//      Interlace = 1;                        
//      __Set(CH_B_RANGE,  G_Attr[0].Yp_Max+1);      // Bͨ���ϲ���Aͨ��
//      __Set(ADC_CTRL, EN + 2);       
//      __Set(ADC_MODE, INTERLACE);                  // Set Interlace mode
//    }
//  } 
  if(_Status == RUN) __Set(FIFO_CLR, W_PTR);       // FIFOдָ�븴λ
}
/*******************************************************************************
 Update_Base: 
*******************************************************************************/
void Update_Base(void) 
{
  u16 i;
  
  __Set(ADC_CTRL, EN);       
  if(Interlace == 0)  i = Title[T_BASE][BASE].Value;     // ��������ģʽ
  else                i = Title[T_BASE][BASE].Value + 5; // �������ģʽ
  __Set(T_BASE_PSC, X_Attr[i].PSC);
  __Set(T_BASE_ARR, X_Attr[i].ARR);
  Wait_Cnt = Wait[_T_base];
  if(_Status == RUN) __Set(FIFO_CLR, W_PTR);      // FIFOдָ�븴λ
}
/*******************************************************************************
 Update_Output: 
*******************************************************************************/
void Update_Output(void) 
{
  if(_Kind == SINE){
    __Set(ANALOG_CNT, 36);
    (*((vu32 *)(0x40020400+0x50))) = (u32)&SIN_DATA[0];
    __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
  }
  if(_Kind == SAW){
    __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
    (*((vu32 *)(0x40020400+0x50))) = (u32)&SAW_DATA[0];
    __Set(ANALOG_CNT, 36);
  }
  if(_Kind == TRIANG){
    __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
    (*((vu32 *)(0x40020400+0x50))) = (u32)&TRG_DATA[0];
    __Set(ANALOG_CNT, 36);
  }
  if(_Kind == DIGI){
    __Set(DIGTAL_PSC, D_Tab[_Frqn].PSC);
    __Set(DIGTAL_ARR, D_Tab[_Frqn].ARR);
    __Set(DIGTAL_CCR, (D_Tab[_Frqn].ARR+1)/2);
  }
}
/*******************************************************************************
 Update_Trig: 
*******************************************************************************/
void Update_Trig(void) 
{ 
  if(_T1 > _T2)  __Set(T_THRESHOLD, (_T1 - _T2));  
  else           __Set(T_THRESHOLD, (_T2 - _T1)); 
  __Set(TRIGG_MODE,  (_Tr_source<< 3)+_Tr_kind);
  if(Title[TRIGG][SOURCE].Value == TRACK1){ 
    __Set(V_THRESHOLD, (((_Vt1-Ka1[_A_Range])*1024)/Ka2[_A_Range])&0xFF); 
  }
  if(Title[TRIGG][SOURCE].Value == TRACK2){ 
    __Set(V_THRESHOLD, (((_Vt2-Kb1[_B_Range])*1024)/Kb2[_B_Range])&0xFF); 
  }
  if(_Status == RUN) __Set(FIFO_CLR, W_PTR);      // FIFOдָ�븴λ
}
/*******************************************************************************
 Process: ���㴦���������� 
*******************************************************************************/
void Process(void)
{ 
  s16 i, j = 0, k, V[8], n = 0;
  s32 Tmp;
  u8  Ch[4], C_D;
  s8  Sa = 2, Sb = 2, Sc = 2, Sd = 2; // ��ʱ״̬
  u16 Ta, Tb, Tc, Td;                 // �������
  
  PaS = 0; PbS = 0; PcS = 0; PdS = 0; 
  TaN = 0; TbN = 0; TcN = 0; TdN = 0; 

  Posi_412 = _4_posi - _1_posi - _2_posi;
  Posi_41  = _4_posi - _1_posi;
  Posi_42  = _4_posi - _2_posi;
  Posi_4_2 = _4_posi + _2_posi;
  Posi_4F1 = _4_posi - FileBuff[ 299];
  Posi_4F2 = _4_posi - FileBuff[ 599];
  Posi_4F3 = _4_posi - FileBuff[ 899];
  Posi_4F4 = _4_posi - FileBuff[1199];
  A_Posi   = ((_1_posi-Ka1[_A_Range])*1024)/Ka2[_A_Range];
  B_Posi   = ((_2_posi-Kb1[_B_Range])*1024)/Kb2[_B_Range];
  
  a_Max = A_Posi; b_Max = B_Posi; 
  a_Min = A_Posi; b_Min = B_Posi;             
  a_Avg = 2048;   b_Avg = 2048; 
  a_Ssq = 2048;   b_Ssq = 2048;      
  
  if((_3_posi + 20)>= Y_BASE+Y_SIZE)  c_Max = Y_BASE+Y_SIZE-1;
  else                                c_Max = _3_posi + 20;
  if((_4_posi + 20)>= Y_BASE+Y_SIZE)  d_Max = Y_BASE+Y_SIZE-1;
  else                                d_Max = _4_posi + 20;
  
  if(Interlace == 0){                           // ��������ģʽ
    k =((1024 -_Kp1)*150 + 512)/1024 + _X_posi.Value;  // �����ֵ����󴰿�λ�õ�����ֵ
    for(i=0; i <4096; i++){
      if((_T_base > 16)&&(_Status == RUN))  DataBuf[i] = __Read_FIFO(); // ����32λFIFO����, ����ָ��+1
      else if((__Get(FIFO_EMPTY)==0)&&(i == JumpCnt)&&(_Status == RUN)){
        JumpCnt++;
        DataBuf[i] = __Read_FIFO();             // ����32λFIFO����, ����ָ��+1
      }
      Ch[A] = (DataBuf[i] & 0xFF );              
      a_Avg += Ch[A];                           // �ۼ�Aͨ��ֱ��ƽ��ֵ              
      Tmp = Ch[A]- A_Posi;
      a_Ssq +=(Tmp * Tmp);                      // ͳ��Aͨ��ƽ����
      Ch[B] = ((DataBuf[i] >> 8) & 0xFF);       
      b_Avg += Ch[B];                           // �ۼ�Bͨ��ֱ��ƽ��ֵ
      Tmp = Ch[B]- B_Posi;
      b_Ssq +=(Tmp * Tmp);                      // ͳ��Bͨ��ƽ����
        
      if(Ch[A] > a_Max)  a_Max = Ch[A];         // ͳ��Aͨ�����ֵ  
      if(Ch[A] < a_Min)  a_Min = Ch[A];         // ͳ��Aͨ����Сֵ  
      if(Ch[B] > b_Max)  b_Max = Ch[B];         // ͳ��Bͨ�����ֵ  
      if(Ch[B] < b_Min)  b_Min = Ch[B];         // ͳ��Bͨ����Сֵ  
       
      C_D = DataBuf[i] >>16;
      if((i>1)&&(i<4094)){
        if(Sa == 2){TaS = i; Ta = i; PaS = 0;}
        if(Ch[A] > a_Mid_H){
          if(Sa == 0){ TaS = i; TaN++;} 
          Sa = 1;  
        } else { 
          if(Ch[A] < a_Mid_L) if(Sa == 1){Sa = 0; PaS += i-TaS;}
        }
        if(Sb == 2){TbS = i; Tb = i; PbS = 0;}
        if(Ch[B] > b_Mid_H){
          if(Sb == 0){ TbS = i; TbN++;} 
          Sb = 1;  
        } else { 
          if(Ch[B] < b_Mid_L) if(Sb == 1){Sb = 0; PbS += i-TbS;}
        }
        if(Sc == 2){TcS = i; Tc = i; PcS = 0;}
        if(C_D & 1){
          if(Sc == 0){ TcS = i; TcN++;} 
          Sc = 1;  
        } else {
          if(Sc == 1){Sc = 0; PcS += i-TcS;} 
        }
        if(Sd == 2){TdS = i; Td = i; PdS = 0;}
        if(C_D & 2){
          if(Sd == 0){ TdS = i; TdN++;} 
          Sd = 1;  
        } else {
          if(Sd == 1){Sd = 0; PdS += i-TdS;} 
        }
      }
      if(i >= k){                               // ָ�뵽��ָ������λ��
        V[A]  = Ka1[_A_Range] +(Ka2[_A_Range] *Ch[A]+ 512)/1024;      
        V[B]  = Kb1[_B_Range] +(Kb2[_B_Range] *Ch[B]+ 512)/1024;      //��ǰ�����ֵ
        while(j > 0 ){
          Send_Data( V[A_]+((V[A]-V[A_])*(1024 - j))/1024, //��ǰCH_A��Ĳ�ֵ
                     V[B_]+((V[B]-V[B_])*(1024 - j))/1024, //��ǰCH_B��Ĳ�ֵ
                     C_D,                                  //��ǰ������ͨ��ֵ
                     n++);
          j -= _Kp1;
          if(n >= 300){ k = 8192;  break;}
        }
        j += 1024;
        V[A_] = V[A];  V[B_] = V[B];     
      }
    }
  } 
  else {                            // �������ģʽ
    k =((1024 -_Kp2)*150 + 512)/1024 + _X_posi.Value;  // �����ֵ����󴰿�λ�õ�����ֵ
    for(i=0; i <4096; i++){
      if(_Status == RUN)  DataBuf[i] = __Read_FIFO(); // ����32λFIFO����, ����ָ��+1
      C_D    = DataBuf[i] >>16;
      if(_2_source == HIDE){                            // Bͨ���ϲ���Aͨ��ʱ
        Ch[A]  = (DataBuf[i] & 0xFF );              
        Ch[B]  = ((DataBuf[i] >> 8) & 0xFF);//+ Kab;
        Tmp    = (Ch[A]- A_Posi);
        a_Ssq += (Tmp * Tmp)/2;
        Tmp    = (Ch[B]- A_Posi);
        a_Ssq += (Tmp * Tmp)/2;                         // ͳ��ƽ����                    
      } else {                                          // Aͨ���ϲ���Bͨ��ʱ
        Ch[B]  = (DataBuf[i] & 0xFF );//- Kab;              
        Ch[A]  = ((DataBuf[i] >> 8) & 0xFF);  
        Tmp    = (Ch[A]- B_Posi);
        a_Ssq += (Tmp * Tmp)/2;
        Tmp    = (Ch[B]- B_Posi);
        a_Ssq += (Tmp * Tmp)/2;                                    // ͳ��ƽ����                    
      }
      a_Avg += (Ch[A]+Ch[B])/2;                                   // �ۼ�ֱ��ƽ��ֵ                                   
      if(Ch[A] > a_Max)  a_Max = Ch[A];          
      if(Ch[B] > a_Max)  a_Max = Ch[B];                          // ͳ�����ֵ 
      if(Ch[A] < a_Min)  a_Min = Ch[A];         
      if(Ch[B] < a_Min)  a_Min = Ch[B];                          // ͳ����Сֵ  

      if(i >= k){                 // ��1��ָ�뵽��ָ������λ��
        if(_2_source == HIDE){                            // Bͨ���ϲ���Aͨ��ʱ
          V[A] = Ka1[_A_Range] +(Ka2[_A_Range]*Ch[A]+ 512)/1024;    //���㵱ǰ��1�����ֵ
          V[B] = Ka1[_A_Range] +(Ka2[_A_Range]*Ch[B]+ 512)/1024;    //���㵱ǰ��2�����ֵ
        } else {                                          // Aͨ���ϲ���Bͨ��ʱ
          V[A] = Kb1[_B_Range] +(Kb2[_B_Range]*Ch[A]+ 512)/1024;   //���㵱ǰ��1�����ֵ
          V[B] = Kb1[_B_Range] +(Kb2[_B_Range]*Ch[B]+ 512)/1024;   //���㵱ǰ��2�����ֵ
        }
        while(j > 0 ){
          Tmp = V[B_]+((V[A]- V[B_])*(1024 - j))/1024; //��ǰ��1��Ĳ�ֵ
          Send_Data(Tmp, Tmp, C_D, n++);
          j -= _Kp2;
          if(n >= 300){ k = 8192;  break;}
        }
        j += 1024;
      }
      if(i >= k){                 // ��2��ָ�뵽��ָ������λ��
        while(j > 0 ){
          Tmp = V[A]+((V[B]- V[A])*(1024 - j))/1024;  //��ǰ��2��Ĳ�ֵ
          Send_Data(Tmp, Tmp, C_D, n++);
          j -= _Kp2;
          if(n >= 300){ k = 8192;  break;}
        }
        j += 1024;
        V[B_] = V[B];       
      }
    }
    b_Avg  = a_Avg;
    b_Ssq  = a_Ssq;
    b_Max  = a_Max;
    b_Min  = a_Min;

    if(_1_source == HIDE)  a_Avg = _1_posi*4096;   // Aͨ���ϲ���Bͨ��ʱ      
    if(_2_source == HIDE)  b_Avg = _2_posi*4096;   // Bͨ���ϲ���Aͨ��ʱ
  }

  a_Mid_H = 4 +(a_Max + a_Min)/2;
  a_Mid_L = a_Mid_H - 8;
  b_Mid_H = 4 +(b_Max + b_Min)/2;
  b_Mid_L = b_Mid_H - 8;

  TaS -= Ta; TbS -= Tb; TcS -= Tc; TdS -= Td;
  
  for(j=0; j<4; j++){                               // ������Ļ�˵�����
    TrackBuff[(  0)*4+ j] = TrackBuff[(  1)*4 + j];
    TrackBuff[(299)*4+ j] = TrackBuff[(298)*4 + j];
  }
}

void Send_Data(s16 Va, s16 Vb, u8 C_D, u16 n)  //�����ʾ����
{
  s32 Tmp, i;

  i = n*4;
  if(Va >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK1] = Y_BASE+Y_SIZE-1;
  else if(Va <= Y_BASE+1)  TrackBuff[i + TRACK1] = Y_BASE+1;
  else                     TrackBuff[i + TRACK1] = Va;
  if(Vb >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK2] = Y_BASE+Y_SIZE-1;
  else if(Vb <= Y_BASE+1)  TrackBuff[i + TRACK2] = Y_BASE+1;
  else                     TrackBuff[i + TRACK2] = Vb;
  
  if(C_D & 1)  TrackBuff[i + TRACK3] = c_Max;
  else         TrackBuff[i + TRACK3] = _3_posi;
  
  switch (_4_source){                       
  case A_add_B:
    if(Interlace == 0) Tmp = Posi_412 + Va + Vb;
    else{
      if(_1_source != HIDE) Tmp = Posi_41  + Va;
      if(_2_source != HIDE) Tmp = Posi_42  + Vb;
    } break;
  case A_sub_B:
    if(Interlace == 0) Tmp = Posi_412 + Va - Vb;
    else{
      if(_1_source != HIDE) Tmp = Posi_41  + Va;
      if(_2_source != HIDE) Tmp = Posi_4_2 - Vb;
    } break;
  case C_and_D:
    if((~C_D)& 3) Tmp = d_Max; 
    else          Tmp = _4_posi;
    break;  
  case C_or_D:
    if(C_D & 3)   Tmp = d_Max; 
    else          Tmp = _4_posi;
    break;  
  case REC_1:
    Tmp = Posi_4F1 + FileBuff[n];  
    break;
  case REC_2:
    Tmp = Posi_4F2 + FileBuff[n+300];  
    break;
  case REC_3:
    Tmp = Posi_4F3 + FileBuff[n+600];  
    break;
  case REC_4:
    Tmp = Posi_4F4 +  FileBuff[n+900];  
    break;
  default:
    if(C_D & 2)  Tmp = d_Max;
    else         Tmp = _4_posi;
  }
  if(Tmp >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK4] = Y_BASE+Y_SIZE-1;
  else if(Tmp <= Y_BASE+1)  TrackBuff[i + TRACK4] = Y_BASE+1;
  else                      TrackBuff[i + TRACK4] = Tmp;
}
/*******************************************************************************
 Synchro: ɨ��ͬ���������趨ģʽ��ʾ���� 
*******************************************************************************/
void Synchro(void)  //ɨ��ͬ����ʽ���У�AUTO��NORM��SGL��NONE��SCAN 5��ģʽ
{ 
  u16  i;

  switch (_Mode){ 
  case AUTO:
    __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  // �败������
    if(__Get(FIFO_START)!=0){
      Process();                                  // �����µ���ʾ����
      Wait_Cnt = Wait[_T_base];
    } else if(Wait_Cnt==0){
      if(JumpCnt >= 4095)  JumpCnt = 0;         
      Process();   
      Wait_Cnt = Wait[_T_base];
    } break;
  case NORM:
    __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  // �败������
    if(__Get(FIFO_START)!=0){
      Process();                                  // �����µ���ʾ����
      Wait_Cnt = Wait[_T_base];
    } else if(Wait_Cnt==0){
      for(i=0; i<4*X_SIZE; ++i)  TrackBuff[i] = 0;// ����ɵ���ʾ����
      Wait_Cnt = Wait[_T_base];
    } break;
  case SGL:
    __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  // �败������
    if(__Get(FIFO_START)!=0)  Process();          // �����µ���ʾ����
    break;
  case NONE:
  case SCAN:
    __Set(TRIGG_MODE, UNCONDITION);               // ��Ϊ����������
    Process();                                    // ���ɵ�ǰ��ʾ����
    break;
  }
  Draw_Window();                                  // ˢ����Ļ������ʾ��

  if((_Status == RUN)&&(__Get(FIFO_FULL)!=0)){    // FIFO is full
    __Set(FIFO_CLR, W_PTR);                       // FIFOдָ�븴λ
    Wait_Cnt = Wait[_T_base];
    JumpCnt =0;
    if(_Mode == SGL){
      _Status = HOLD;                             // һ֡��󣬽�����ͣ
      _State.Flag |= UPDAT;
    }
    if(_Mode == SCAN){
      for(i=0; i<X_SIZE; i++){                    // �ؽ���ǰ�켣����                   
        TrackBuff[i*4 + TRACK1] = _1_posi;
        TrackBuff[i*4 + TRACK2] = _2_posi; 
        TrackBuff[i*4 + TRACK3] = _3_posi; 
        TrackBuff[i*4 + TRACK4] = _4_posi;
      }
    }
  }    
}  
/******************************** END OF FILE *********************************/
