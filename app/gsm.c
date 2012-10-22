/* ----------------------------------------------------------------------------
*         WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*
*
*APP主要是接受消息再响应，主要是一些被动执行的函数，跟任务打交道的函数，不能直接调用驱动底层的执行函数。
*
*GSM用来处理来着G网模块的各种处理
*
* ----------------------------------------------------------------------------
*/


//宏定义


//头文件
#include "os.h"

extern void app_auto_cmd_cmti(char * ptr, int len);
extern void app_auto_cmd_clip(char * ptr, int len);
extern void APP_gsm_send(void * ptr, int len);
extern void app_sync_cmd_cmgr(char * ptr, int len);
//主动上报回码
typedef struct _AT_ARRY{
  unsigned char cmd[19];
  unsigned char flags;
  void (*func)(char *,int);
}AT_ARRY;

//变量引用
//变量定义
//同步指令回码
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

//异步指令回码
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

//等待状态定义
typedef enum
{
  APP_GSM_WAIT_SYNC = 0,
  APP_GSM_WAIT_NSYNC,
  APP_GSM_WAIT_AUTO,
}APP_GSM_WORK_STATE;

APP_GSM_WORK_STATE app_gsm_work_state = APP_GSM_WAIT_AUTO;
//函数引用
//函数定义

char *strcpy(char *str1,char *str2){
     char *temp=str1;
    while(*str2!='\0')
    {
    *temp++ = *str2++;
    }
     *temp='\0';       //这个有什么用？好像我删除掉也没影响
    return str1;
}

/*
函数名：APP_gsm
参数：无
返回值：无
描述：FLASH初始化
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
      //识别到号码了
      mc_set_sys_num(num,i);
    }  
    num[i] = bptr[i];
  }
}

void app_auto_cmd_cmti(char * ptr, int len)
{
      APP_gsm_send("AT+CMGR=1\r\n",12); //发送读取命令
      app_gsm_work_state = APP_GSM_WAIT_SYNC;//进入同步命令
  //匹配命令后提取参数
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
  //匹配命令后提取参数
  for (int i=0;i<len;i++)
  {
    if (ptr[i] == ',')
    {
      ptr += i;
      
      //APP_gsm_send("AT+CMGR="+*ptr); //发送读取命令
      app_gsm_work_state = APP_GSM_WAIT_SYNC;//进入同步命令
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
    //目前只处理短信上报和读取，和开机指令
    APP_gsm_match_sync_at(ptr,len);
    
    break;
    
  case APP_GSM_WAIT_NSYNC:
    
    break;
    
  case APP_GSM_WAIT_AUTO: //主动上报状态，匹配指令
    //目前只处理短信上报和读取，和开机指令
    APP_gsm_match_auto_at(ptr,len);
    
    break;
  }
}


typedef struct {
  char SCA[16]; // 短消息服务中心号码(SMSC地址)
  char TPA[16]; // 目标号码或回复号码(TP-DA或TP-RA)
  char TP_PID; // 用户信息协议标识(TP-PID)
  char TP_DCS; // 用户信息编码方式(TP-DCS)
  char TP_SCTS[16]; // 服务时间戳字符串(TP_SCTS), 接收时用到
  char TP_UD[160]; // 接收到的短信内容
  //char TP_UDS[160]; // 要发送的短信内容
} SM_PARAM;

int String2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
  for (int i = 0; i < nSrcLength; i += 2)
  {
    // 输出高4位
    if ((*pSrc >= '0') && (*pSrc <= '9'))
    {
      *pDst = (*pSrc - '0') << 4;
    }
    else
    {
      *pDst = (*pSrc - 'A' + 10) << 4;
    }
    
    pSrc++;
    
    // 输出低4位
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
  
  // 返回目标数据长度
  return (nSrcLength / 2);
}

// 字节数据转换为可打印字符串
// 如：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// 输入: pSrc - 源数据指针
//       nSrcLength - 源数据长度
// 输出: pDst - 目标字符串指针
// 返回: 目标字符串长度
int Bytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
  const char tab[]="0123456789ABCDEF";    // 0x0-0xf的字符查找表
  
  for (int i = 0; i < nSrcLength; i++)
  {
    *pDst++ = tab[*pSrc >> 4];        // 输出高4位
    *pDst++ = tab[*pSrc & 0x0f];    // 输出低4位
    pSrc++;
  }
  
  // 输出字符串加个结束符
  *pDst = '\0';
  
  // 返回目标字符串长度
  return (nSrcLength * 2);
}

// 7bit编码
// 输入: pSrc - 源字符串指针
//       nSrcLength - 源字符串长度
// 输出: pDst - 目标编码串指针
// 返回: 目标编码串长度
int Encode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
  int nSrc;        // 源字符串的计数值
  int nDst;        // 目标编码串的计数值
  int nChar;        // 当前正在处理的组内字符字节的序号，范围是0-7
  unsigned char nLeft;    // 上一字节残余的数据
  
  // 计数值初始化
  nSrc = 0;
  nDst = 0;
  
  // 将源串每8个字节分为一组，压缩成7个字节
  // 循环该处理过程，直至源串被处理完
  // 如果分组不到8字节，也能正确处理
  while (nSrc < nSrcLength)
  {
    // 取源字符串的计数值的最低3位
    nChar = nSrc & 7;
    
    // 处理源串的每个字节
    if(nChar == 0)
    {
      // 组内第一个字节，只是保存起来，待处理下一个字节时使用
      nLeft = *pSrc;
    }
    else
    {
      // 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节
      *pDst = (*pSrc << (8-nChar)) | nLeft;
      
      // 将该字节剩下的左边部分，作为残余数据保存起来
      nLeft = *pSrc >> nChar;
      
      // 修改目标串的指针和计数值
      pDst++;
      nDst++;
    }
    
    // 修改源串的指针和计数值
    pSrc++;
    nSrc++;
  }
  
  // 返回目标串长度
  return nDst;
}

// 7bit解码
// 输入: pSrc - 源编码串指针
//       nSrcLength - 源编码串长度
// 输出: pDst - 目标字符串指针
// 返回: 目标字符串长度
int Decode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
  int nSrc;        // 源字符串的计数值
  int nDst;        // 目标解码串的计数值
  int nByte;        // 当前正在处理的组内字节的序号，范围是0-6
  unsigned char nLeft;    // 上一字节残余的数据
  
  // 计数值初始化
  nSrc = 0;
  nDst = 0;
  
  // 组内字节序号和残余数据初始化
  nByte = 0;
  nLeft = 0;
  
  // 将源数据每7个字节分为一组，解压缩成8个字节
  // 循环该处理过程，直至源数据被处理完
  // 如果分组不到7字节，也能正确处理
  while(nSrc<nSrcLength)
  {
    // 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节
    *pDst = ((*pSrc << nByte) | nLeft) & 0x7f;
    
    // 将该字节剩下的左边部分，作为残余数据保存起来
    nLeft = *pSrc >> (7-nByte);
    
    // 修改目标串的指针和计数值
    pDst++;
    nDst++;
    
    // 修改字节计数值
    nByte++;
    
    // 到了一组的最后一个字节
    if(nByte == 7)
    {
      // 额外得到一个目标解码字节
      *pDst = nLeft;
      
      // 修改目标串的指针和计数值
      pDst++;
      nDst++;
      
      // 组内字节序号和残余数据初始化
      nByte = 0;
      nLeft = 0;
    }
    
    // 修改源串的指针和计数值
    pSrc++;
    nSrc++;
  }
  
  // 输出字符串加个结束符
  *pDst = '\0';
  
  // 返回目标串长度
  return nDst;
}




// 正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补'F'凑成偶数
// 如："8613851872468" --> "683158812764F8"
// 输入: pSrc - 源字符串指针
//       nSrcLength - 源字符串长度
// 输出: pDst - 目标字符串指针
// 返回: 目标字符串长度
int InvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
  int nDstLength;        // 目标字符串长度
  char ch;            // 用于保存一个字符
  
  // 复制串长度
  nDstLength = nSrcLength;
  
  // 两两颠倒
  for(int i=0; i<nSrcLength;i+=2)
  {
    ch = *pSrc++;        // 保存先出现的字符
    *pDst++ = *pSrc++;    // 复制后出现的字符
    *pDst++ = ch;        // 复制先出现的字符
  }
  
  // 源串长度是奇数吗？
  if(nSrcLength & 1)
  {
    *(pDst-2) = 'F';    // 补'F'
    nDstLength++;        // 目标串长度加1
  }
  
  // 输出字符串加个结束符
  *pDst = '\0';
  
  // 返回目标字符串长度
  return nDstLength;
}

// 两两颠倒的字符串转换为正常顺序的字符串
// 如："683158812764F8" --> "8613851872468"
// 输入: pSrc - 源字符串指针
//       nSrcLength - 源字符串长度
// 输出: pDst - 目标字符串指针
// 返回: 目标字符串长度
int SerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
  int nDstLength;        // 目标字符串长度
  char ch;            // 用于保存一个字符
  
  // 复制串长度
  nDstLength = nSrcLength;
  
  // 两两颠倒
  for(int i=0; i<nSrcLength;i+=2)
  {
    ch = *pSrc++;        // 保存先出现的字符
    *pDst++ = *pSrc++;    // 复制后出现的字符
    *pDst++ = ch;        // 复制先出现的字符
  }
  
  // 最后的字符是'F'吗？
  if(*(pDst-1) == 'F')
  {
    pDst--;
    nDstLength--;        // 目标字符串长度减1
  }
  
  // 输出字符串加个结束符
  *pDst = '\0';
  
  // 返回目标字符串长度
  return nDstLength;
}

// PDU编码，用于编制、发送短消息
// 输入: pSrc - 源PDU参数指针
// 输出: pDst - 目标PDU串指针
// 返回: 目标PDU串长度
int EncodePdu(const SM_PARAM* pSrc, char* pDst)
{
  int nLength;            // 内部用的串长度
  int nDstLength;            // 目标PDU串长度
  unsigned char buf[256];    // 内部用的缓冲区
  
  // SMSC地址信息段
  nLength = strlen(pSrc->SCA);    // SMSC地址字符串的长度    
  buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;    // SMSC地址信息长度
  buf[1] = 0x91;        // 固定: 用国际格式号码
  nDstLength = Bytes2String(buf, pDst, 2);        // 转换2个字节到目标PDU串
  nDstLength += InvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);    // 转换SMSC号码到目标PDU串
  
  // TPDU段基本参数、目标地址等
  nLength = strlen(pSrc->TPA);    // TP-DA地址字符串的长度
  buf[0] = 0x11;                    // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
  buf[1] = 0;                        // TP-MR=0
  buf[2] = (char)nLength;            // 目标地址数字个数(TP-DA地址字符串真实长度)
  buf[3] = 0x91;                    // 固定: 用国际格式号码
  nDstLength += Bytes2String(buf, &pDst[nDstLength], 4);        // 转换4个字节到目标PDU串
  nDstLength += InvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);    // 转换TP-DA到目标PDU串
  
  // TPDU段协议标识、编码方式、用户信息等
  nLength = strlen(pSrc->TP_UD);    // 用户信息字符串的长度
  buf[0] = pSrc->TP_PID;            // 协议标识(TP-PID)
  buf[1] = pSrc->TP_DCS;            // 用户信息编码方式(TP-DCS)
  buf[2] = 0;                        // 有效期(TP-VP)为5分钟
  
  // 7-bit编码方式
  buf[3] = nLength;            // 编码前长度
  nLength = Encode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;    // 转换TP-DA到目标PDU串
  
  nDstLength += Bytes2String(buf, &pDst[nDstLength], nLength);        // 转换该段数据到目标PDU串
  
  // 返回目标字符串长度
  return nDstLength;
}

// PDU解码，用于接收、阅读短消息
// 输入: pSrc - 源PDU串指针
// 输出: pDst - 目标PDU参数指针
// 返回: 用户信息串长度
int DecodePdu(const char* pSrc, SM_PARAM* pDst)
{
  int nDstLength;            // 目标PDU串长度
  unsigned char tmp;        // 内部用的临时字节变量
  unsigned char buf[256];    // 内部用的缓冲区
  
  // SMSC地址信息段
  String2Bytes(pSrc, &tmp, 2);    // 取长度
  tmp = (tmp - 1) * 2;    // SMSC号码串长度
  pSrc += 4;            // 指针后移，忽略了SMSC地址格式
  SerializeNumbers(pSrc, pDst->SCA, tmp);    // 转换SMSC号码到目标PDU串
  pSrc += tmp;        // 指针后移
  
  // TPDU段基本参数
  String2Bytes(pSrc, &tmp, 2);    // 取基本参数
  pSrc += 2;        // 指针后移
  
  // 取回复号码
  String2Bytes(pSrc, &tmp, 2);    // 取长度
  if(tmp & 1) tmp += 1;    // 调整奇偶性
  pSrc += 4;            // 指针后移，忽略了回复地址(TP-RA)格式
  SerializeNumbers(pSrc, pDst->TPA, tmp);    // 取TP-RA号码
  pSrc += tmp;        // 指针后移
  
  // TPDU段协议标识、编码方式、用户信息等
  String2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);    // 取协议标识(TP-PID)
  pSrc += 2;        // 指针后移
  String2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);    // 取编码方式(TP-DCS)
  pSrc += 2;        // 指针后移
  SerializeNumbers(pSrc, pDst->TP_SCTS, 14);        // 服务时间戳字符串(TP_SCTS) 
  pSrc += 14;        // 指针后移
  String2Bytes(pSrc, &tmp, 2);    // 用户信息长度(TP-UDL)
  pSrc += 2;        // 指针后移
  // 7-bit解码
  nDstLength = String2Bytes(pSrc, buf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4);    // 格式转换
  Decode7bit(buf, pDst->TP_UD, nDstLength);    // 转换到TP-DU
  nDstLength = tmp;
  
  // 返回目标字符串长度
  return nDstLength;
}


#if 0
// 发送短消息，仅发送命令，不读取应答
// 输入: pSrc - 源PDU参数指针
int SendMessage(SM_PARAM* pSrc)
{
  int nPduLength;        // PDU串长度
  unsigned char nSmscLength;    // SMSC串长度
  int nLength;        // 串口收到的数据长度
  char cmd[16];        // 命令串
  char pdu[512];        // PDU串
  char ans[128];        // 应答串
  
  nPduLength = EncodePdu(pSrc, pdu);    // 根据PDU参数，编码PDU串
  strcat(pdu, "\x01a");        // 以Ctrl-Z结束
  
  String2Bytes(pdu, &nSmscLength, 2);    // 取PDU串中的SMSC信息长度
  nSmscLength++;        // 加上长度字节本身
  
  // 命令中的长度，不包括SMSC信息长度，以数据字节计
  sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);    // 生成命令
  
  
  WriteComm(cmd, strlen(cmd));    // 先输出命令串
  
  nLength = ReadComm(ans, 128);    // 读应答数据
  
  // 根据能否找到"\r\n> "决定成功与否
  if(nLength == 4 && strncmp(ans, "\r\n> ", 4) == 0)
  {
    return WriteComm(pdu, strlen(pdu));        // 得到肯定回答，继续输出PDU串
  }
  
  return 0;
}

// 读取短消息，仅发送命令，不读取应答
// 用+CMGL代替+CMGR，可一次性读出全部短消息
int ReadMessageList()
{
  return WriteComm("AT+CMGL\r", 8);
}

// 删除短消息，仅发送命令，不读取应答
// 输入: index - 短消息序号，1-255
int DeleteMessage(int index)
{
  char cmd[16];        // 命令串
  
  sprintf(cmd, "AT+CMGD=%d\r", index);    // 生成命令
  
  // 输出命令串
  return WriteComm(cmd, strlen(cmd));
}



// 从列表中解析出全部短消息
// 输入: pBuff - 短消息列表缓冲区
// 输出: pMsg - 短消息缓冲区
// 返回: 短消息条数
int ParseMessageList(SM_PARAM* pMsg, SM_BUFF* pBuff)
{
  int nMsg;            // 短消息计数值
  char* ptr;            // 内部用的数据指针
  
  nMsg = 0;
  ptr = pBuff->data;
  
  // 循环读取每一条短消息, 以"+CMGL:"开头
  while((ptr = strstr(ptr, "+CMGL:")) != NULL)
  {
    ptr += 6;        // 跳过"+CMGL:", 定位到序号
    sscanf(ptr, "%d", &pMsg->index);    // 读取序号
    //        TRACE("  index=%d\n",pMsg->index);
    
    ptr = strstr(ptr, "\r\n");    // 找下一行
    if (ptr != NULL)
    {
      ptr += 2;        // 跳过"\r\n", 定位到PDU
      
      gsmDecodePdu(ptr, pMsg);    // PDU串解码
      
      pMsg++;        // 准备读下一条短消息
      nMsg++;        // 短消息计数加1
    }
  }
  
  return nMsg;
}
#endif