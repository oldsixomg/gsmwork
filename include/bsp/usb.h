/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;

        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

AT91SAMxUSB.H  file

���ߣ�����ȦȦ
��������: 2008.06.27
�޸�����: 2009.03.08
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/


#ifndef USB_H
#define USB_H

#define GET_STATUS         0
#define CLEAR_FEATURE      1
#define SET_FEATURE        3
#define SET_ADDRESS        5
#define GET_DESCRIPTOR     6
#define SET_DESCRIPTOR     7
#define GET_CONFIGURATION  8
#define SET_CONFIGURATION  9
#define GET_INTERFACE      10
#define SET_INTERFACE      11
#define SYNCH_FRAME        12

#define DEVICE_DESCRIPTOR         1
#define CONFIGURATION_DESCRIPTOR  2
#define STRING_DESCRIPTOR         3
#define INTERFACE_DESCRIPTOR      4
#define ENDPOINT_DESCRIPTOR       5
#define REPORT_DESCRIPTOR         0x22

#define SET_IDLE 0x0A

#define GET_LINE_CODING         0x21
#define SERIAL_STATE            0x20
#define SET_LINE_CODING         0x20
#define SET_CONTROL_LINE_STATE  0x22
#define SEND_BREAK              0x23


//��������
void UsbDisconnect(void); //USB�Ͽ�����
void UsbConnect(void);    //USB����
void UsbChipInit(void);
void UsbChipResetEndpoint(void);
void UsbChipSetConfig(unsigned char Value);
void UsbChipWriteAddress(unsigned char Addr);
void UsbChipSetAddressStatus(unsigned char Value);
int  UsbChipIsSetup(unsigned char Endp);
unsigned char UsbChipReadEndpointBuffer(unsigned char Endp, unsigned char Len, unsigned char *Buf);
unsigned char bsp_usb_send_endpoint_buff(unsigned char Endp, unsigned char Len, unsigned char *Buf);
void UsbChipClearBuffer(unsigned char Endp);
void UsbChipAcknowledgeSetup(unsigned char Endp);
void UsbChipSetAddress(unsigned char Addr);
void UsbChipSetEndpointEnable(unsigned char Enable);
void SetCsr(int endpoint, unsigned int flags);
void ClrCsr(int endpoint, unsigned int flags);

void UsbBusSuspend(void); //���߹����жϴ���
void UsbBusReset(void);   //���߸�λ�жϴ���
void UsbEp0Out(void);     //�˵�0����жϴ���
void UsbEp0In(void);      //�˵�0�����жϴ���
void UsbEp1Out(void);     //�˵�1����жϴ���
void UsbEp1In(void);      //�˵�1�����жϴ���
void UsbEp2Out(void);     //�˵�2����жϴ���
void UsbEp2In(void);      //�˵�2�����жϴ���
void UsbEp3In(void);      //�˵�3�����жϴ���

extern unsigned char ConfigValue;  //��ǰ����ֵ
extern unsigned char Ep1InIsBusy;  //�˵�1�����Ƿ�æ
extern unsigned char Ep3InIsBusy;  //�˵�3�����Ƿ�æ
extern unsigned long Count;     //�����˶��ٴα���ļ�����

#endif