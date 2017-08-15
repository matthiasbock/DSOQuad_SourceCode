/********************* (C) COPYRIGHT 2011 e-Design Co.,Ltd. ********************
 File Name : FAT12.c  
 Version   : DS203_SYS Ver 1.5x                                  Author : bure
*******************************************************************************/
#include "../../system/include/FAT12.h"

#include <string.h>

#include "../../system/include/ASM.h"
#include "../../system/include/BIOS.h"
#include "../../system/include/Function.h"

#define FAT_LEN      0x1800 
#define FAT1_BASE    0x00001000   // FAT1����ʼ��ַ
#define FAT2_BASE    0x00002800   // FAT2����ʼ��ַ
#define ROOT_BASE    0x00004000   // ��Ŀ¼��ʼ��ַ
#define FILE_BASE    0x00008000   // �ļ�����ʼ��ַ
#define SEC_LEN      0x200        // ��������
#define FAT1_SEC     0x0C         // FAT1������
#define FAT2_SEC     0x0C         // FAT2������

#define OK           0            // �������
#define SEC_ERR      1            // ������д����
#define FAT_ERR      2            // FAT���д����
#define OVER         3            // �������
#define NEW          4            // ��Ŀ¼��
#define END          0xFFF        // ���ӽ���

#define OW           0            // ��д(���ݴ�0��1��д)
#define RW           1            // ��д


/*******************************************************************************
 ������ҳ��(256 Bytes)  ����USB��д��ͻ���ض�
*******************************************************************************/
u8 ReadDiskData(u8* pBuffer, u32 ReadAddr, u16 Lenght)
{
  u8 n = 0;
  
  while(1){
    Clash = 0;
    __ExtFlash_PageRD(pBuffer, ReadAddr, Lenght);
    if(n++ > 6) return SEC_ERR;     // ��ʱ������
    if(Clash == 0) return OK;       // �޳�ͻ�����򷵻�
  }
}
/*******************************************************************************
 д����ҳ��(256 Bytes)  ����USB��д��ͻ����д
*******************************************************************************/
u8 ProgDiskPage(u8* pBuffer, u32 ProgAddr)
{                         
  u8   n = 0; 
  
  while(1){
    Clash = 0;
    __ExtFlash_PageWR(pBuffer, ProgAddr);
    if(n++ > 6) return SEC_ERR;     // ��ʱ������
    if(Clash == 0) return OK;       // �޳�ͻ�����򷵻�
  }
} 
/*******************************************************************************
 ������һ�����Ӵغź󷵻أ���ǰ�غű�����ָ��+1��λ��
*******************************************************************************/
u8 NextCluster(u16* pCluster)
{
  u16 FatNum;
  u32 Addr = FAT1_BASE +(*pCluster + *pCluster/2);
  
  *(pCluster+1)= *pCluster;                                   // ����ǰһ���غ�
  *pCluster = 0;
  if((*(pCluster+1) >= END)||(*(pCluster+1)< 2)) return OK;
  if(__ReadDiskData((u8*)&FatNum, Addr, 2)!= OK) return SEC_ERR;
  *pCluster= (*(pCluster+1) & 1)?(FatNum >>4):(FatNum & 0x0FFF); // ָ����һ���غ�
  return OK; 
}
/*******************************************************************************
 ���ļ�����(512 Bytes), ����ʱָ��ָ����һ���غţ���ǰ�غű�����ָ��+1��λ��
*******************************************************************************/
u8 ReadFileSec(u8* pBuffer, u16* pCluster)
{
  u32 ReadAddr = FILE_BASE + SEC_LEN*(*pCluster-2);
  
  if(__ReadDiskData(pBuffer, ReadAddr, 256)!=OK) return SEC_ERR; // ��ǰ������
  pBuffer += 256;  ReadAddr += 256;
  if(__ReadDiskData(pBuffer, ReadAddr, 256)!=OK) return SEC_ERR; // ���������
  if(NextCluster(pCluster)!=0) return FAT_ERR;                 // ȡ��һ���غ�
  return OK;
} 
/*******************************************************************************
 д�ļ�����(512 Bytes)����д��ǰFAT�����ز��ҵ�����һ���غ�
*******************************************************************************/
u8 ProgFileSec(u8* pBuffer, u16* pCluster)
{
  u16 Tmp;
  u32 ProgAddr = FILE_BASE + SEC_LEN*(*pCluster-2);
  
  if(__ProgDiskPage(pBuffer, ProgAddr)!= OK) return SEC_ERR; // дǰ������
  pBuffer += 256;  ProgAddr += 256;
  if(__ProgDiskPage(pBuffer, ProgAddr)!= OK) return SEC_ERR; // д�������
  if(NextCluster(pCluster)!=0) return FAT_ERR;                 // ȡ��һ���غ�
  Tmp = *(pCluster+1);
  if(*pCluster == 0){
    *pCluster = Tmp;
    if(SeekBlank (pBuffer, pCluster )!= OK) return OVER;
    if(SetCluster(pBuffer, pCluster )!= OK) return SEC_ERR;
  }
  return OK;
}
/*******************************************************************************
 ���ҿ��дغţ�����ʱָ��ָ����һ�����дغţ���ǰ�غű�����ָ��+1��λ��
*******************************************************************************/
u8 SeekBlank(u8* pBuffer, u16* pCluster)
{
  u16  Tmp;
  u8   Buffer[2];

  *(pCluster+1)= *pCluster;                                    // ���浱ǰ�غ�
  for(*pCluster=0; (*pCluster)<4095; (*pCluster)++){
    if(__ReadDiskData(Buffer, FAT1_BASE +(*pCluster)+(*pCluster)/2, 2)!= 0) return SEC_ERR;
    Tmp = ((*pCluster)& 1)?((*(u16*)Buffer)>>4):((*(u16*)Buffer)& 0xFFF);
    if((Tmp == 0)&&((*pCluster)!= *(pCluster+1))) return OK;
  }
  return OK;
}         
/*******************************************************************************
 ����һ�غ�д��FAT��ǰ������λ������ʱָ��ָ����һ�غţ�ָ��+1Ϊ��ǰ�غ�
*******************************************************************************/
u8 SetCluster(u8* pBuffer, u16* pCluster)
{
  u16  Offset, i, k;
  u32  SecAddr;

  i = *(pCluster+1);                    // ��ȡԭ��ǰ�غ�
  k = *pCluster;                        // ��ȡ��һ�غ�
  Offset = i + i/2;
  SecAddr = FAT1_BASE +(Offset & 0xFF00);
  Offset &=  0x00FF;
  if(__ReadDiskData(pBuffer, SecAddr, 258)!= 0) return SEC_ERR; 
  if(i & 1){
    pBuffer[Offset  ]=(pBuffer[Offset]& 0x0F)+((k <<4)& 0xF0);
    pBuffer[Offset+1]= k >>4;
  } else {
    pBuffer[Offset  ]= k & 0xFF;
    pBuffer[Offset+1]=(pBuffer[Offset+1]& 0xF0)+((k>>8)& 0x0F);
  }
  if(__ProgDiskPage(pBuffer,     SecAddr    )!= 0) return SEC_ERR;
  if(__ProgDiskPage(pBuffer+256, SecAddr+256)!= 0) return SEC_ERR;
  return OK;
}
/*******************************************************************************
 ��ģʽ���ļ��������ļ���һ���غż�Ŀ¼���ַ�� 0�غż���һ���հ�Ŀ¼���ַ
*******************************************************************************/
u8 OpenFileRd(u8* pBuffer, u8* pFileName, u16* pCluster, u32* pDirAddr)
{
  u16 i, n;
  
  *pCluster = 0;
  for(*pDirAddr=ROOT_BASE; *pDirAddr<FILE_BASE; ){
    if(__ReadDiskData(pBuffer, *pDirAddr, 256)!= OK) return SEC_ERR;
    for(n=0; n<256; n+=32){
      for(i=0; i<11; i++){
        if(pBuffer[n + i]!= 0){
          if(pBuffer[n + i]!= pFileName[i]) break;
          if(i == 10){                             // �ҵ��ļ���
            *pCluster = *(u16*)(pBuffer + n + 0x1A); // �ļ���һ���غ�
            return OK;         
          }
        } else return NEW;               // ������һ���հ�Ŀ¼��󷵻�
      }
      *pDirAddr += 32;
    }
  }
  return OVER;
}
/*******************************************************************************
 дģʽ���ļ��������ļ���һ���غż�Ŀ¼���ַ
*******************************************************************************/
u8 OpenFileWr(u8* pBuffer, u8* pFileName, u16* pCluster, u32* pDirAddr)
{
  u16 i, n;
  
  i = OpenFileRd(pBuffer, pFileName, pCluster, pDirAddr);
  if(i != NEW) return i;
  else{                                                    // ��ǰ��Ϊ�հ�Ŀ¼��
    if(SeekBlank(pBuffer, pCluster)!= OK) return OVER;     // ��FAT��������
    n =*pDirAddr & 0xFF;                                   // nΪ��ǰҳĿ¼��
    if(__ReadDiskData(pBuffer,(*pDirAddr)-n, 256)!= OK) return SEC_ERR;
    for(i=0; i<11; i++) pBuffer[n + i]= pFileName[i];      // ������Ŀ¼��
    *(u16*)(pBuffer + n + 0x1A)= *pCluster;
    if(__ProgDiskPage(pBuffer,(*pDirAddr)-n)!= OK) return SEC_ERR;
    return OK;
  }
}                
/*******************************************************************************
 �ر��ļ�����������д��FAT�����ļ�����д��Ŀ¼�����FAT1��FAT2
*******************************************************************************/
u8 CloseFile(u8* pBuffer, u32 Lenght, u16* pCluster, u32* pDirAddr)
{
  u16 n;
  
  *pCluster = 0xFFF;
  SetCluster(pBuffer, pCluster); 
  if(__ReadDiskData(pBuffer, (*pDirAddr &(~0xFF)), 256)!= OK) return SEC_ERR;
  *(u8* )(pBuffer +(*pDirAddr & 0xFF)+ 0x0B)= 0x20;
  *(u32*)(pBuffer +(*pDirAddr & 0xFF)+ 0x1C)= Lenght;
  if(__ProgDiskPage(pBuffer, (*pDirAddr &(~0xFF)))!= OK) return SEC_ERR;
  for(n=0; n<FAT1_SEC; n++){
    if(__ReadDiskData(pBuffer, FAT1_BASE+n*256, 256)!= OK) return SEC_ERR;
    if(__ProgDiskPage(pBuffer, FAT2_BASE+n*256     )!= OK) return SEC_ERR;
  } 
  return OK;
}
