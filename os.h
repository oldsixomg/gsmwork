/* ----------------------------------------------------------------------------
*         WUHOME 公司提供支持
* ----------------------------------------------------------------------------
* 版权：2012 WUHOME公司
*
* 公司保留所有权力
*
*
*
*
*
*
*
* ----------------------------------------------------------------------------
*/

//宏定义
#define DSM_ITEM_MAX_SIZE 32
#define DSM_ITEM_MAX_CNT  20

typedef struct _DSM_ITEM{
struct _DSM_ITEM * next;
unsigned int stream_id;
unsigned int used;
unsigned char data_ptr[DSM_ITEM_MAX_SIZE];
}DSM_ITEM;

typedef struct _OS_CMD{
unsigned int cmd_id;
void *data_p;
struct _OS_CMD* next;
}OS_CMD;


typedef struct{
OS_CMD * head;
OS_CMD * last;
}OS_CMD_LIST;


extern OS_CMD_LIST phe_cmd_alloc;
extern OS_CMD_LIST sio_cmd_alloc;
extern OS_CMD_LIST efs_cmd_alloc;

/*
函数名：put_cmd
参数：无
返回值：无
描述：把命令加入队列
*/
void phe_put_cmd(unsigned int cmd,void *ptr);
void sio_put_cmd(unsigned int cmd,void *ptr);


OS_CMD * q_get(OS_CMD_LIST *alloc_list);

void os_init(void);
DSM_ITEM * dsm_alloc_item(void);
unsigned char str_include(char *str1,char *str2);
int str_to_int(char *str1);