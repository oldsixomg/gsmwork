/* ----------------------------------------------------------------------------
*         WUHOME ��˾�ṩ֧��
* ----------------------------------------------------------------------------
* ��Ȩ��2012 WUHOME��˾
*
* ��˾��������Ȩ��
*
*
*
*
*
*
*
* ----------------------------------------------------------------------------
*/

//�궨��
#define GLOBE_RST_KEY (1<<29)
#define GLOBE_SET_KEY (1<<30)

#define GLOBE_SET_GPIO_IN(x)    AT91F_PIO_CfgInput(AT91C_BASE_PIOA,x)
#define GLOBE_GPIO_INT_EN(x)    AT91F_PIO_InterruptEnable(AT91C_BASE_PIOA,x)
#define GLOBE_GET_KEY()         AT91F_PIO_GetInput(AT91C_BASE_PIOA)
#define OS_REPORT_KEY_EVENT(x)  phe_put_cmd(x, 0);



/*
��������DR_key_init
��������
����ֵ����
������������ʼ��
*/
void DR_key_init();


/*
��������DR_key_isr_cb
��������
����ֵ����
�����������жϴ�����
*/
void DR_key_isr_cb();

/*
��������DR_key_isr_cb
��������
����ֵ����
�����������жϴ�����
*/
unsigned int DR_key_input_check(unsigned int num);

/*
��������DR_set_cb
��������
����ֵ����
�����������жϴ�����
*/
void DR_set_cb(void);

/*
��������DR_rst_cb
��������
����ֵ����
�����������жϴ�����
*/
void DR_rst_cb(void);