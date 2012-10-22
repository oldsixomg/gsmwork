/* ----------------------------------------------------------------------------
*         WUHOME ��˾�ṩ֧��
* ----------------------------------------------------------------------------
* ��Ȩ��2012 WUHOME��˾
*
* ��˾��������Ȩ��
*
*
*
*APP��Ҫ�ǽ�����Ϣ����Ӧ����Ҫ��һЩ����ִ�еĺ�����������򽻵��ĺ���������ֱ�ӵ��������ײ��ִ�к�����
*
*GSM������������G��ģ��ĸ��ִ���
*
* ----------------------------------------------------------------------------
*/


//�궨��


//ͷ�ļ�
#include "os.h"

extern void app_auto_cmd_cmti(char * ptr, int len);
extern void app_auto_cmd_clip(char * ptr, int len);
extern void APP_gsm_send(void * ptr, int len);
extern void app_sync_cmd_cmgr(char * ptr, int len);
//�����ϱ�����
typedef struct _AT_ARRY{
  unsigned char cmd[19];
  unsigned char flags;
  void (*func)(char *,int);
}AT_ARRY;

//��������
//��������
//ͬ��ָ�����
AT_ARRY app_sync_cmd[] =
{
  {"OK",0,0},
  {"+CMGD",0,0},
  {"ERROR",0,0},
  {"+CMGR",0,0},
  {"+CMGS",0,0},
  {"+CMGW",0,0},
  {"+CNMI",0,0}
};

//�첽ָ�����
unsigned char app_nsync_cmd[][20] =
{
  "+CMS ERROR",
  "+CLIP",
  "RING",
  "+CRING",
  "+CMTI",
  "+CMT",
  "+CBM",
  "+CDS"
};

AT_ARRY app_auto_cmd[10] =
{
  {"^SYSSTART",0,0},
  {"+CMTI",0,app_auto_cmd_cmti},
  {"+CLIP",0,app_auto_cmd_clip},
  {"+CMT",0,0},
  {"+CDS",0,0},
  {"+CBM",0,0},
  {"RING",0,0}
};

//�ȴ�״̬����
typedef enum
{
  APP_GSM_WAIT_SYNC = 0,
  APP_GSM_WAIT_NSYNC,
  APP_GSM_WAIT_AUTO,
}APP_GSM_WORK_STATE;

APP_GSM_WORK_STATE app_gsm_work_state = APP_GSM_WAIT_AUTO;
//��������
//��������

char *strcpy(char *str1,char *str2){
     char *temp=str1;
    while(*str2!='\0')
    {
    *temp++ = *str2++;
    }
     *temp='\0';       //�����ʲô�ã�������ɾ����ҲûӰ��
    return str1;
}

/*
��������APP_gsm
��������
����ֵ����
������FLASH��ʼ��
*/
void APP_gsm(void)
{
  app_gsm_work_state = APP_GSM_WAIT_AUTO;
  strcpy(app_auto_cmd[0].cmd,"^SYSSTART");
  strcpy(app_auto_cmd[1].cmd,"+CMTI");
  strcpy(app_auto_cmd[2].cmd,"+CLIP");
  app_auto_cmd[1].func = app_auto_cmd_cmti;
  app_auto_cmd[2].func = app_auto_cmd_clip;
  
  strcpy(app_sync_cmd[0].cmd,"OK");
  strcpy(app_sync_cmd[1].cmd,"+CMGR");
  strcpy(app_sync_cmd[2].cmd,"+CMS");
  app_sync_cmd[1].func = app_sync_cmd_cmgr;
}

char *strstr1(const char *s1, const char *s2)
{
  int n;
  if (*s2)
  {
    while (*s1)
    {
      for (n=0; *(s1 + n) == *(s2 + n); n++)
      {
        if (!*(s2 + n + 1))
          return (char *)s1;
      }
      s1++;
    }
    return 0;
  }
  else
    return (char *)s1;
}

void APP_gsm_match_auto_at(void * ptr, int len)
{
  char * strp ;
  for (int j=0;j<2;j++)
  {
    strp = strstr1(ptr,&app_auto_cmd[j].cmd[0]);
    if (strp != 0)
    {
      app_auto_cmd[j].func(strp,len);
      return ;
    }
  }
  
}

void APP_gsm_match_sync_at(void * ptr, int len)
{
    char * strp;
  for (int j=0;j<3;j++)
  {
    strp = strstr1(ptr,app_sync_cmd[j].cmd);
    if (strp != 0)
    {
      app_sync_cmd[j].func(strp,len);
      return ;
    }
  }
  
}

void app_auto_cmd_clip(char * ptr, int len)
{
  char num[13];
  char bptr = ptr;
  //+CLIP:num,type[...]
  if (*bptr ! = ':')
    return ;
  
  bptr++;
  if ((*bptr == '+') && (bptr[1] == '8') && (bptr[2] == '6'))
  {
    bptr += 3;
  }
  
  for (int i =0;i<len;i++)
  {
    if (bptr[i] == ',')
    {
      num[i] = 0;
      //ʶ�𵽺�����
      mc_set_sys_num(num,i);
    }  
    num[i] = bptr[i];
  }
}

void app_auto_cmd_cmti(char * ptr, int len)
{
      APP_gsm_send("AT+CMGR=1\r\n",12); //���Ͷ�ȡ����
      app_gsm_work_state = APP_GSM_WAIT_SYNC;//����ͬ������
  //ƥ���������ȡ����
  for (int i=0;i<len;i++)
  {
    if (ptr[i] == ',')
    {
      ptr += i;
      
    }
  }
}

void app_sync_cmd_cmgr(char * ptr, int len)
{
  int index;
  //ƥ���������ȡ����
  for (int i=0;i<len;i++)
  {
    if (ptr[i] == ',')
    {
      ptr += i;
      
      //APP_gsm_send("AT+CMGR="+*ptr); //���Ͷ�ȡ����
      app_gsm_work_state = APP_GSM_WAIT_SYNC;//����ͬ������
    }
  }
}


void APP_gsm_send(void * ptr, int len)
{
  unsigned char * bptr = (unsigned char *)ptr;
  DSM_ITEM *item = dsm_alloc_item();
    item->stream_id = 0;
    for (int i=0;i<len;i++)
    {
      item->data_ptr[i] = bptr[i];
    }
    item->used = len;
    TS_sio_transmit(item);
  
}

void APP_gsm_recv(void * ptr, int len)
{
  unsigned char * b_ptr = (unsigned char *)ptr;
  
  switch (app_gsm_work_state)
  {
  case APP_GSM_WAIT_SYNC:
    //Ŀǰֻ��������ϱ��Ͷ�ȡ���Ϳ���ָ��
    APP_gsm_match_sync_at(ptr,len);
    
    break;
    
  case APP_GSM_WAIT_NSYNC:
    
    break;
    
  case APP_GSM_WAIT_AUTO: //�����ϱ�״̬��ƥ��ָ��
    //Ŀǰֻ��������ϱ��Ͷ�ȡ���Ϳ���ָ��
    APP_gsm_match_auto_at(ptr,len);
    
    break;
  }
}


typedef struct {
  char SCA[16]; // ����Ϣ�������ĺ���(SMSC��ַ)
  char TPA[16]; // Ŀ������ظ�����(TP-DA��TP-RA)
  char TP_PID; // �û���ϢЭ���ʶ(TP-PID)
  char TP_DCS; // �û���Ϣ���뷽ʽ(TP-DCS)
  char TP_SCTS[16]; // ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
  char TP_UD[160]; // ���յ��Ķ�������
  //char TP_UDS[160]; // Ҫ���͵Ķ�������
} SM_PARAM;

int String2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
  for (int i = 0; i < nSrcLength; i += 2)
  {
    // �����4λ
    if ((*pSrc >= '0') && (*pSrc <= '9'))
    {
      *pDst = (*pSrc - '0') << 4;
    }
    else
    {
      *pDst = (*pSrc - 'A' + 10) << 4;
    }
    
    pSrc++;
    
    // �����4λ
    if ((*pSrc>='0') && (*pSrc<='9'))
    {
      *pDst |= *pSrc - '0';
    }
    else
    {
      *pDst |= *pSrc - 'A' + 10;
    }
    
    pSrc++;
    pDst++;
  }
  
  // ����Ŀ�����ݳ���
  return (nSrcLength / 2);
}

// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// ����: pSrc - Դ����ָ��
//       nSrcLength - Դ���ݳ���
// ���: pDst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int Bytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
  const char tab[]="0123456789ABCDEF";    // 0x0-0xf���ַ����ұ�
  
  for (int i = 0; i < nSrcLength; i++)
  {
    *pDst++ = tab[*pSrc >> 4];        // �����4λ
    *pDst++ = tab[*pSrc & 0x0f];    // �����4λ
    pSrc++;
  }
  
  // ����ַ����Ӹ�������
  *pDst = '\0';
  
  // ����Ŀ���ַ�������
  return (nSrcLength * 2);
}

// 7bit����
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ����봮ָ��
// ����: Ŀ����봮����
int Encode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
  int nSrc;        // Դ�ַ����ļ���ֵ
  int nDst;        // Ŀ����봮�ļ���ֵ
  int nChar;        // ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
  unsigned char nLeft;    // ��һ�ֽڲ��������
  
  // ����ֵ��ʼ��
  nSrc = 0;
  nDst = 0;
  
  // ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�
  // ѭ���ô�����̣�ֱ��Դ����������
  // ������鲻��8�ֽڣ�Ҳ����ȷ����
  while (nSrc < nSrcLength)
  {
    // ȡԴ�ַ����ļ���ֵ�����3λ
    nChar = nSrc & 7;
    
    // ����Դ����ÿ���ֽ�
    if(nChar == 0)
    {
      // ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��
      nLeft = *pSrc;
    }
    else
    {
      // ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�
      *pDst = (*pSrc << (8-nChar)) | nLeft;
      
      // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
      nLeft = *pSrc >> nChar;
      
      // �޸�Ŀ�괮��ָ��ͼ���ֵ
      pDst++;
      nDst++;
    }
    
    // �޸�Դ����ָ��ͼ���ֵ
    pSrc++;
    nSrc++;
  }
  
  // ����Ŀ�괮����
  return nDst;
}

// 7bit����
// ����: pSrc - Դ���봮ָ��
//       nSrcLength - Դ���봮����
// ���: pDst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int Decode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
  int nSrc;        // Դ�ַ����ļ���ֵ
  int nDst;        // Ŀ����봮�ļ���ֵ
  int nByte;        // ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
  unsigned char nLeft;    // ��һ�ֽڲ��������
  
  // ����ֵ��ʼ��
  nSrc = 0;
  nDst = 0;
  
  // �����ֽ���źͲ������ݳ�ʼ��
  nByte = 0;
  nLeft = 0;
  
  // ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
  // ѭ���ô�����̣�ֱ��Դ���ݱ�������
  // ������鲻��7�ֽڣ�Ҳ����ȷ����
  while(nSrc<nSrcLength)
  {
    // ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
    *pDst = ((*pSrc << nByte) | nLeft) & 0x7f;
    
    // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
    nLeft = *pSrc >> (7-nByte);
    
    // �޸�Ŀ�괮��ָ��ͼ���ֵ
    pDst++;
    nDst++;
    
    // �޸��ֽڼ���ֵ
    nByte++;
    
    // ����һ������һ���ֽ�
    if(nByte == 7)
    {
      // ����õ�һ��Ŀ������ֽ�
      *pDst = nLeft;
      
      // �޸�Ŀ�괮��ָ��ͼ���ֵ
      pDst++;
      nDst++;
      
      // �����ֽ���źͲ������ݳ�ʼ��
      nByte = 0;
      nLeft = 0;
    }
    
    // �޸�Դ����ָ��ͼ���ֵ
    pSrc++;
    nSrc++;
  }
  
  // ����ַ����Ӹ�������
  *pDst = '\0';
  
  // ����Ŀ�괮����
  return nDst;
}




// ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��
// �磺"8613851872468" --> "683158812764F8"
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int InvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
  int nDstLength;        // Ŀ���ַ�������
  char ch;            // ���ڱ���һ���ַ�
  
  // ���ƴ�����
  nDstLength = nSrcLength;
  
  // �����ߵ�
  for(int i=0; i<nSrcLength;i+=2)
  {
    ch = *pSrc++;        // �����ȳ��ֵ��ַ�
    *pDst++ = *pSrc++;    // ���ƺ���ֵ��ַ�
    *pDst++ = ch;        // �����ȳ��ֵ��ַ�
  }
  
  // Դ��������������
  if(nSrcLength & 1)
  {
    *(pDst-2) = 'F';    // ��'F'
    nDstLength++;        // Ŀ�괮���ȼ�1
  }
  
  // ����ַ����Ӹ�������
  *pDst = '\0';
  
  // ����Ŀ���ַ�������
  return nDstLength;
}

// �����ߵ����ַ���ת��Ϊ����˳����ַ���
// �磺"683158812764F8" --> "8613851872468"
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int SerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
  int nDstLength;        // Ŀ���ַ�������
  char ch;            // ���ڱ���һ���ַ�
  
  // ���ƴ�����
  nDstLength = nSrcLength;
  
  // �����ߵ�
  for(int i=0; i<nSrcLength;i+=2)
  {
    ch = *pSrc++;        // �����ȳ��ֵ��ַ�
    *pDst++ = *pSrc++;    // ���ƺ���ֵ��ַ�
    *pDst++ = ch;        // �����ȳ��ֵ��ַ�
  }
  
  // �����ַ���'F'��
  if(*(pDst-1) == 'F')
  {
    pDst--;
    nDstLength--;        // Ŀ���ַ������ȼ�1
  }
  
  // ����ַ����Ӹ�������
  *pDst = '\0';
  
  // ����Ŀ���ַ�������
  return nDstLength;
}

// PDU���룬���ڱ��ơ����Ͷ���Ϣ
// ����: pSrc - ԴPDU����ָ��
// ���: pDst - Ŀ��PDU��ָ��
// ����: Ŀ��PDU������
int EncodePdu(const SM_PARAM* pSrc, char* pDst)
{
  int nLength;            // �ڲ��õĴ�����
  int nDstLength;            // Ŀ��PDU������
  unsigned char buf[256];    // �ڲ��õĻ�����
  
  // SMSC��ַ��Ϣ��
  nLength = strlen(pSrc->SCA);    // SMSC��ַ�ַ����ĳ���    
  buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;    // SMSC��ַ��Ϣ����
  buf[1] = 0x91;        // �̶�: �ù��ʸ�ʽ����
  nDstLength = Bytes2String(buf, pDst, 2);        // ת��2���ֽڵ�Ŀ��PDU��
  nDstLength += InvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);    // ת��SMSC���뵽Ŀ��PDU��
  
  // TPDU�λ���������Ŀ���ַ��
  nLength = strlen(pSrc->TPA);    // TP-DA��ַ�ַ����ĳ���
  buf[0] = 0x11;                    // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
  buf[1] = 0;                        // TP-MR=0
  buf[2] = (char)nLength;            // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
  buf[3] = 0x91;                    // �̶�: �ù��ʸ�ʽ����
  nDstLength += Bytes2String(buf, &pDst[nDstLength], 4);        // ת��4���ֽڵ�Ŀ��PDU��
  nDstLength += InvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);    // ת��TP-DA��Ŀ��PDU��
  
  // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
  nLength = strlen(pSrc->TP_UD);    // �û���Ϣ�ַ����ĳ���
  buf[0] = pSrc->TP_PID;            // Э���ʶ(TP-PID)
  buf[1] = pSrc->TP_DCS;            // �û���Ϣ���뷽ʽ(TP-DCS)
  buf[2] = 0;                        // ��Ч��(TP-VP)Ϊ5����
  
  // 7-bit���뷽ʽ
  buf[3] = nLength;            // ����ǰ����
  nLength = Encode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;    // ת��TP-DA��Ŀ��PDU��
  
  nDstLength += Bytes2String(buf, &pDst[nDstLength], nLength);        // ת���ö����ݵ�Ŀ��PDU��
  
  // ����Ŀ���ַ�������
  return nDstLength;
}

// PDU���룬���ڽ��ա��Ķ�����Ϣ
// ����: pSrc - ԴPDU��ָ��
// ���: pDst - Ŀ��PDU����ָ��
// ����: �û���Ϣ������
int DecodePdu(const char* pSrc, SM_PARAM* pDst)
{
  int nDstLength;            // Ŀ��PDU������
  unsigned char tmp;        // �ڲ��õ���ʱ�ֽڱ���
  unsigned char buf[256];    // �ڲ��õĻ�����
  
  // SMSC��ַ��Ϣ��
  String2Bytes(pSrc, &tmp, 2);    // ȡ����
  tmp = (tmp - 1) * 2;    // SMSC���봮����
  pSrc += 4;            // ָ����ƣ�������SMSC��ַ��ʽ
  SerializeNumbers(pSrc, pDst->SCA, tmp);    // ת��SMSC���뵽Ŀ��PDU��
  pSrc += tmp;        // ָ�����
  
  // TPDU�λ�������
  String2Bytes(pSrc, &tmp, 2);    // ȡ��������
  pSrc += 2;        // ָ�����
  
  // ȡ�ظ�����
  String2Bytes(pSrc, &tmp, 2);    // ȡ����
  if(tmp & 1) tmp += 1;    // ������ż��
  pSrc += 4;            // ָ����ƣ������˻ظ���ַ(TP-RA)��ʽ
  SerializeNumbers(pSrc, pDst->TPA, tmp);    // ȡTP-RA����
  pSrc += tmp;        // ָ�����
  
  // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
  String2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);    // ȡЭ���ʶ(TP-PID)
  pSrc += 2;        // ָ�����
  String2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);    // ȡ���뷽ʽ(TP-DCS)
  pSrc += 2;        // ָ�����
  SerializeNumbers(pSrc, pDst->TP_SCTS, 14);        // ����ʱ����ַ���(TP_SCTS) 
  pSrc += 14;        // ָ�����
  String2Bytes(pSrc, &tmp, 2);    // �û���Ϣ����(TP-UDL)
  pSrc += 2;        // ָ�����
  // 7-bit����
  nDstLength = String2Bytes(pSrc, buf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4);    // ��ʽת��
  Decode7bit(buf, pDst->TP_UD, nDstLength);    // ת����TP-DU
  nDstLength = tmp;
  
  // ����Ŀ���ַ�������
  return nDstLength;
}


#if 0
// ���Ͷ���Ϣ���������������ȡӦ��
// ����: pSrc - ԴPDU����ָ��
int SendMessage(SM_PARAM* pSrc)
{
  int nPduLength;        // PDU������
  unsigned char nSmscLength;    // SMSC������
  int nLength;        // �����յ������ݳ���
  char cmd[16];        // ���
  char pdu[512];        // PDU��
  char ans[128];        // Ӧ��
  
  nPduLength = EncodePdu(pSrc, pdu);    // ����PDU����������PDU��
  strcat(pdu, "\x01a");        // ��Ctrl-Z����
  
  String2Bytes(pdu, &nSmscLength, 2);    // ȡPDU���е�SMSC��Ϣ����
  nSmscLength++;        // ���ϳ����ֽڱ���
  
  // �����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�
  sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);    // ��������
  
  
  WriteComm(cmd, strlen(cmd));    // ��������
  
  nLength = ReadComm(ans, 128);    // ��Ӧ������
  
  // �����ܷ��ҵ�"\r\n> "�����ɹ����
  if(nLength == 4 && strncmp(ans, "\r\n> ", 4) == 0)
  {
    return WriteComm(pdu, strlen(pdu));        // �õ��϶��ش𣬼������PDU��
  }
  
  return 0;
}

// ��ȡ����Ϣ���������������ȡӦ��
// ��+CMGL����+CMGR����һ���Զ���ȫ������Ϣ
int ReadMessageList()
{
  return WriteComm("AT+CMGL\r", 8);
}

// ɾ������Ϣ���������������ȡӦ��
// ����: index - ����Ϣ��ţ�1-255
int DeleteMessage(int index)
{
  char cmd[16];        // ���
  
  sprintf(cmd, "AT+CMGD=%d\r", index);    // ��������
  
  // ������
  return WriteComm(cmd, strlen(cmd));
}



// ���б��н�����ȫ������Ϣ
// ����: pBuff - ����Ϣ�б�����
// ���: pMsg - ����Ϣ������
// ����: ����Ϣ����
int ParseMessageList(SM_PARAM* pMsg, SM_BUFF* pBuff)
{
  int nMsg;            // ����Ϣ����ֵ
  char* ptr;            // �ڲ��õ�����ָ��
  
  nMsg = 0;
  ptr = pBuff->data;
  
  // ѭ����ȡÿһ������Ϣ, ��"+CMGL:"��ͷ
  while((ptr = strstr(ptr, "+CMGL:")) != NULL)
  {
    ptr += 6;        // ����"+CMGL:", ��λ�����
    sscanf(ptr, "%d", &pMsg->index);    // ��ȡ���
    //        TRACE("  index=%d\n",pMsg->index);
    
    ptr = strstr(ptr, "\r\n");    // ����һ��
    if (ptr != NULL)
    {
      ptr += 2;        // ����"\r\n", ��λ��PDU
      
      gsmDecodePdu(ptr, pMsg);    // PDU������
      
      pMsg++;        // ׼������һ������Ϣ
      nMsg++;        // ����Ϣ������1
    }
  }
  
  return nMsg;
}
#endif