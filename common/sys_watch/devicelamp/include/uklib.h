

#ifndef UKLIB_HEADER
#define UKLIB_HEADER

#ifdef __cplusplus
extern "C"{
#endif
typedef long UK_STATUS;
typedef struct UK_TIMESTAMP
{
	unsigned long sec;
	unsigned long nsec;
}UK_TIMESTAMP;
typedef struct UK_SENSOR
{
	double temperature;
	double humidity;
}UK_SENSOR;



typedef unsigned long u32;

typedef struct UK_FPOWER
{
	unsigned short tx_power;
	unsigned short rx_power;
}UK_FPOWER;

typedef struct tagNetStat{
	unsigned long      linkstate;
}UK_NetStat,*PUK_NetStat;

typedef struct tagEthStat{
	unsigned long linkstatus;
	unsigned long fincage;
	UK_FPOWER power;
}UK_EthStat,*PUK_EthStat;

//��չ��Ԫ�İ汾
typedef struct _ex_version_{
	unsigned long date;
	unsigned long version;
	unsigned long hasdev;
}ex_version;


#define K_PCIE_MAGIC  'K'

#define UK_GETDEVLIST  	 	     _IOR(K_PCIE_MAGIC,	 	0, char *)
#define UK_DEVSTART  		     _IO(K_PCIE_MAGIC,	 	1)
#define UK_DEVSTOP   		     _IO(K_PCIE_MAGIC,    	2)
#define UK_GETSTATINFO           _IOR(K_PCIE_MAGIC,   	3, int *)
#define UK_RDMODE			     _IOWR(K_PCIE_MAGIC,  	4, int *)
#define UK_WRMODE			     _IOWR(K_PCIE_MAGIC,  	5, int *)
#define UK_RDWRITEPTR			 _IOR(K_PCIE_MAGIC,   	6, int *)
#define UK_PCIEEXIST			 _IO(K_PCIE_MAGIC,    	7)
#define UK_ADDCNT        		 _IOWR(K_PCIE_MAGIC,	8, int *)
#define UK_READCNT	         	 _IOWR(K_PCIE_MAGIC,	9, int *)
#define UK_READNETSTAT	         _IOR(K_PCIE_MAGIC,		10, int *)
#define UK_DO_W					 _IOWR(K_PCIE_MAGIC, 	11, int *)
#define UK_DO_R  	 			 _IOWR(K_PCIE_MAGIC, 	12, int *)
#define UK_DI  	 			     _IOWR(K_PCIE_MAGIC, 	13, int *)
#define UK_LED_W  	 		     _IOWR(K_PCIE_MAGIC, 	14, int *)
#define UK_LED_R  	 		     _IOWR(K_PCIE_MAGIC, 	15, int *)
#define UK_SEC_W  	 		     _IOWR(K_PCIE_MAGIC, 	16, int *)
#define UK_SEC_R  	 		     _IOWR(K_PCIE_MAGIC, 	17, int *)
#define UK_NSEC_R  	 		     _IOWR(K_PCIE_MAGIC, 	18, int *)
#define UK_SYNCT		         _IOWR(K_PCIE_MAGIC, 	19, int *)
#define UK_IGB_STAT			     _IOWR(K_PCIE_MAGIC, 	20, int *)
#define UK_GETEVENT			     _IOWR(K_PCIE_MAGIC, 	21, int *)
#define UK_SYNC_STATUS		     _IOWR(K_PCIE_MAGIC, 	22, int *)
#define UK_RESETTIME             _IOWR(K_PCIE_MAGIC, 	23, int *)
#define UK_WDT_EN        	     _IOWR(K_PCIE_MAGIC, 	24, int *)
#define UK_WDT_DIS        	     _IOWR(K_PCIE_MAGIC, 	25, int *)	
#define UK_WDT_FEED        	     _IOWR(K_PCIE_MAGIC, 	26, int *)
#define UK_POWERSTAT        	 _IOWR(K_PCIE_MAGIC, 	27, int *)
#define UK_GPIO_SET_LED        	 _IOWR(K_PCIE_MAGIC, 	28, int *)
#define UK_GPIO_GET_LED        	 _IOWR(K_PCIE_MAGIC, 	29, int *)
#define UK_GET_BIOSSEL           _IOWR(K_PCIE_MAGIC, 	30, int *)
#define UK_SET_BIOSSEL           _IOWR(K_PCIE_MAGIC, 	31, int *)
#define UK_IGB_INTEN			 _IOWR(K_PCIE_MAGIC, 	32, int *)
#define UK_IGB_PAR				 _IOWR(K_PCIE_MAGIC, 	33, int *)
#define UK_IGB_INTDIS			 _IOWR(K_PCIE_MAGIC, 	34, int *)
#define UK_POWEROFF				 _IOWR(K_PCIE_MAGIC, 	35, int *)
#define UK_INT_ENA_W			 _IOWR(K_PCIE_MAGIC, 	36, int *)
#define UK_INT_EDGE_W			 _IOWR(K_PCIE_MAGIC, 	37, int *)
#define UK_POWERON				 _IOWR(K_PCIE_MAGIC, 	38, int *)
#define UK_RDPPS                 _IOWR(K_PCIE_MAGIC,    39, int *)
#define K_PCIE_MAXNR             40

#define UK_GPIO_MAGIC  'G'
#define UK_GPIO_SET     _IOWR(UK_GPIO_MAGIC,1, unsigned long *)


#define UK_ERR_SUCCESS       0
#define UK_ERR_NODEVS       -1
#define UK_ERR_EXISTS       -2
#define UK_ERR_NONEXISTS    -3
#define UK_ERR_NOMEM        -4
#define UK_ERR_DEVSTART     -5
#define UK_ERR_DEVSTOP      -6
#define UK_ERR_THREAD_FAIL  -7
#define UK_ERR_THREAD_EXIT  -8
#define UK_ERR_INPROGRESS   -9
#define UK_ERR_INVALIDCMD   -10
#define UK_ERR_FAIL         -11
#define UK_ERR_INVHANDLE    -12
#define UK_ERR_FILEFULL     -13
#define UK_ERR_FILESEEK     -14
#define UK_ERR_FILEMAP      -15
#define UK_ERR_INVARG       -16
#define UK_ERR_VERSION      -17

#define UK_TRUE  1
#define UK_FALSE 0
#define LED_OFF     0
#define LED_ON      1
#define LED_TOGGLE  2
#define DO_OFF   1
#define DO_ON    0



/*************************************
������	: UK_getNetList
����    	: �õ���ڵ��豸�б�
����        
pName	: �洢ʹ��":"�������豸�б��buffer
buflen	: buffer�ĳ���
����ֵ	
UK_ERR_NODEVS	: û���豸
����0��ֵ		: �豸����
 **************************************/
int UK_getNetList(char *pName,int buflen);


/*************************************
������	: UK_regNetCallback
����     	: ע��һ���豸�ص��������ú������ڹ��������ʱ���ص�
���� 
devname			: �豸��
UK_NetCallBack	: �豸�ص�������ԭ������
UK_STATUS (*UK_NetCallBack)(unsigned char *pNetBuf,int* plen,UK_STATUS status,void *pContext);
pContext		      	: Ҫ���ݸ��ص�����������
timeout			: ��ʱʱ��
����ֵ     
UK_ERR_EXISTS   : ���豸�Ѿ���ע�����
UK_ERR_SUCCESS: ע��ɹ� 
 **************************************/
typedef UK_STATUS (*UK_NetCallBack)(int totallen,UK_STATUS status,void *pContext);
UK_STATUS UK_regNetCallback(char *devname,UK_NetCallBack pFunc,void *pContext,unsigned long timeout);


/*************************************
������	: UK_unregNetCallback
����		: ȡ��ԭ����ע���豸�ص�����
���� 
devname			: �豸��
UK_NetCallBack	: Ҫȡ�����豸�ص�������ԭ������
UK_STATUS (*UK_NetCallBack)(unsigned char *pNetBuf,int* plen,UK_STATUS status,void *pContext);
pContext		      	: Ҫ���ݸ��ص�����������
����ֵ
UK_ERR_NONEXISTS	: ���豸û��ע����豸�ص�����
UK_ERR_SUCCESS	: ȡ���ɹ� 
 **************************************/
UK_STATUS UK_unregNetCallback(char *devname,UK_NetCallBack pFunc,void *pContext);


/*************************************
������	: UK_NetCaptureStart
����		: �����հ�	
����ֵ:     
-1					:  ����ʧ��
UK_ERR_SUCCESS	:  �����ɹ�
 **************************************/
UK_STATUS UK_NetCaptureStart(void);


/*************************************
������	: UK_ReadFile
����		: ���������
����
devname	: Ҫ��ȡ���豸��
buffer	: ��Ŷ��������ݵ�λ��
size		: buffer�ĳ���
return:     
0	: û��������
>0  	: ����ֵΪ���������ݳ���
 **************************************/
long  UK_ReadFile(char *devname,void * buffer,int size);


/*************************************
������	: UK_NetCaptureStop
����	 	: ֹͣ�հ�
����ֵ     
UK_ERR_SUCCESS: �رճɹ� 
 **************************************/
UK_STATUS UK_NetCaptureStop(void);


/*************************************
������	: UK_dido_setdo
����	 	: ���ÿ�����״̬
����
val	: ������״̬���ó�val(��val����DO_OFFʱ���������Ͽ�����val����DO_ONʱ���������պ�)
num	: Ҫ���õĿ���
����ֵ   
UK_ERR_SUCCESS: ���óɹ�
 **************************************/
int UK_dido_setdo(char val,char num);


/*************************************
������	: UK_read_di
����	 	: ��ȡ�����״̬
����
val	: ָ�룬�����״̬��������ָ����ڴ���
����ֵ   
UK_ERR_SUCCESS: ��ȡ�ɹ�
 **************************************/
int UK_read_di(char *val);


/*************************************
������	: UK_EnableEvent
����	 	: ʹ��/��ֹ�����жϣ�ͬʱ��ʹ�ܵ�ʱ���豸�������ػ����½�����Ч
����
ena		: 0:��ֹ��1:ʹ��
edge	: 0:��������Ч��1:��������Ч
����ֵ   
UK_ERR_SUCCESS: �����ɹ�
 **************************************/
int UK_EnableEvent(int ena,int edge);


/*************************************
������	: UK_CheckDIEvent
����	 	: ��������ֱ�������жϷ������߳�ʱ
����
event	: �洢�ж����͵�buffer
timeout	: ��ʱʱ��
����ֵ   
UK_ERR_SUCCESS: �ɹ�
 **************************************/
int UK_CheckDIEvent(void * event,int timeout);


/*************************************
������	: UK_UsrLed
����	 	: �����Զ����
����
bit		:0~7 ��ʾָ�������ĵƣ� -1 ��ʾ���е�
status	:�� bitΪ 0~7ʱ�� status ������ֵLED_ON��ʾ�򿪣� LED_OFF��ʾ�رգ�
LED_TOGGLE ��ʾȡ������ bit Ϊ-1 ʱ��ÿһ�� bit ��ʾ��Ӧ��״̬
����ֵ
UK_ERR_INVARG 		: �Ƿ�����
UK_ERR_SUCCESS	: �ɹ�
 **************************************/
UK_STATUS UK_UsrLed(unsigned int bit,unsigned long status);

#define LED_FAULT 0
#define LED_RUN   1
/*************************************
������  : UK_UsrLed_GPIO
����        : ������GPIO���Ƶ������Զ����
����
bit     :
LED_FAULT	���ϵ�
LED_RUN		���е�
status  :
LED_ON��ʾ��
LED_OFF��ʾ�ر�
����ֵ
UK_ERR_INVARG       : �Ƿ�����
UK_ERR_SUCCESS  : �ɹ�
 **************************************/
UK_STATUS UK_UsrLed_GPIO(int bit,unsigned long status);



/*************************************
������	: IRIGB_DeviceOpen
����	 	: ��B���豸
����ֵ
����0��ֵ��ʾ�ļ����
����0��ֵ��ʾ��ʧ��
 **************************************/
int IRIGB_DeviceOpen();


/*************************************
������	: IRIGB_DeviceClose
����	 	: �ر�B���豸
����
fd	: ͨ��IRIGB_DeviceOpen��ȡ���ļ����
 **************************************/
void IRIGB_DeviceClose(int fd);


/*************************************
������	: IRIGB_EnableDevice
����	 	: ʹ��B���豸
����
fd	: ͨ��IRIGB_DeviceOpen��ȡ���ļ����
 **************************************/
int IRIGB_EnableDevice(int fd);


/*************************************
������	: IRIGB_DisableDevice
����	 	: ��ֹB���豸
����
fd	: ͨ��IRIGB_DeviceOpen��ȡ���ļ����
 **************************************/
int IRIGB_DisableDevice(int fd);


/*************************************
������	: IRIGB_SyncTime
����	 	: ��ȡ��ʱ״̬
����
fd	: ͨ��IRIGB_DeviceOpen��ȡ���ļ����
����ֵ
��ʱ�Ķ�ʱ״̬
 **************************************/
int IRIGB_SyncTime(int fd);

/*************************************
������	: IRIGB_GetPTPState
����	 	: ��ȡ�ź�״̬
����
fd	: ͨ��IRIGB_DeviceOpen��ȡ���ļ����
����ֵ
��ʱ���ź�״̬
 **************************************/
int IRIGB_GetPTPState(int fd);


/*************************************
������	: IRIGB_GetTime
����	 	: �Ӷ�ʱ��fpga�л�ȡ��ʱ�ľ�ȷʱ��
����
fd	: ͨ��IRIGB_DeviceOpen��ȡ���ļ����
����ֵ
��ʱ�ľ�ȷʱ��
 **************************************/
UK_TIMESTAMP IRIGB_GetTime(int fd);

/*************************************
������	: IRIGB_SetTime
����	 	: ���ö�ʱfpga��ʱ��
����
fd	: ͨ��IRIGB_DeviceOpen��ȡ���ļ����
sec	: Ҫ���õ�ʱ��
����ֵ
UK_ERR_FAIL		: ����ʧ��
UK_ERR_SUCCESS 	: ���óɹ�
 **************************************/
UK_STATUS IRIGB_SetTime(int fd,unsigned long sec);


/*************************************
������	: IRIGB_ResetTime
����	 	: �Զ�ʱ�����˹���λ
����
fd	: ͨ��IRIGB_DeviceOpen��ȡ���ļ����
����ֵ
UK_ERR_FAIL		: ����ʧ��
UK_ERR_SUCCESS 	: ���óɹ�
 **************************************/
UK_STATUS IRIGB_ResetTime(int fd);


//add by caowei
//time 5s 10s 15s 20s 25s 30s
/*************************************
������	: UK_Wdt_Enable
����	 	: ʹ�ܿ��Ź������ó�ʱʱ��
����
time	: ���Ź��ĳ�ʱʱ��
����ֵ
UK_ERR_FAIL		: ����ʧ��
UK_ERR_SUCCESS 	: ���óɹ�
 **************************************/
UK_STATUS UK_Wdt_Enable(unsigned int time);


/*************************************
������	: UK_Wdt_Disable
����	 	: ��ֹ���Ź�
����ֵ
UK_ERR_FAIL		: ����ʧ��
UK_ERR_SUCCESS 	: ���óɹ�
 **************************************/
UK_STATUS UK_Wdt_Disable();


/*************************************
������	: UK_Wdt_FeedDog
����	 	: ι��
����ֵ
UK_ERR_FAIL		: ι��ʧ��
UK_ERR_SUCCESS 	: ι���ɹ�
 **************************************/
UK_STATUS UK_Wdt_FeedDog();


/*************************************
������	: UK_getOptions
����	 	: ��ȡ��ڵ�ͳ����Ϣ(��ڵ�����״̬�͹⹦��)
����		
idx			: ��ں�
pBuf		: �洢��ȡ������Ϣ���ڴ浥Ԫ
plen			: pBuf�ĳ���
����ֵ
UK_ERR_FAIL	 	: ��ȡʧ��
UK_ERR_SUCCESS  	: ��ȡ�ɹ�

 **************************************/
UK_STATUS UK_getOptions(int idx,unsigned char *pBuf,int *plen);


/*************************************
������	: UK_Read_Ethstat
����	 	: ��ȡ��ڵ�ͳ����Ϣ(����ֻ�й�ڵ�����״̬)
����		
idx			: ��ں�
����ֵ
UK_NetStat	 	: ��ȡ����״ֵ̬

 **************************************/
UK_NetStat UK_Read_Ethstat(int idx);


/*************************************
������	: UK_Read_Digital_Sensor
����	 	: ��ȡ��ʪ�ȴ�������ֵ
����		
sensor		: UK_SENSOR���͵�ָ�룬�������ص�ʱ����ʪ��ֵ
����ֵ
UK_ERR_FAIL	 	: ��ȡʧ��
UK_ERR_SUCCESS  	: ��ȡ�ɹ�

 **************************************/
UK_STATUS UK_Read_Digital_Sensor(UK_SENSOR * sensor);


/*************************************
������	: UKPower_regCallback
����     	: ע��һ���ص��������ú��������豸���������»ص�
���� 
fd					: �ļ����
UKPower_CallBack	: �ص�������ԭ������
UK_STATUS (*UKPower_CallBack)(void *pContext);
pContext		      	: Ҫ���ݸ��ص�����������
����ֵ     
UK_ERR_EXISTS   : ���豸�Ѿ���ע�����
UK_ERR_SUCCESS	: ע��ɹ� 
 **************************************/
typedef UK_STATUS (*UKPower_CallBack)(void *pContext);
UK_STATUS UKPower_regCallback(int fd,UKPower_CallBack pFunc,void *pContext);

/*************************************
������  : 	UK_ReadPowerState
����        : ��ȡ��ǰ�ĵ�Դ״̬ 
����ֵ     
-1   		: ��ȡʧ�� 
0,1,2,3  	: ��Դ״̬ 
**************************************/ 
int UK_ReadPowerState();

/*************************************
������	: UKPower_unregCallback
����		: ȡ��ԭ��ע��Ļص�����
���� 
fd					: �ļ����
����ֵ
UK_ERR_NONEXISTS	: ���豸û��ע����豸�ص�����
UK_ERR_SUCCESS		: ȡ���ɹ� 
 **************************************/
UK_STATUS UKPower_unregCallback(int fd);


/*************************************
������	: UK_Read_Ex_Version
����		: ȡ����չ��İ汾��
���� 
index					: ��չ�ں�
val						: ����������չ���FPGA�汾��Ϣ
����ֵ
UK_ERR_NONEXISTS	 	: û�������չ���FPGA�汾��Ϣ
UK_ERR_FAIL			: �������ĵ�ַΪNULL
UK_ERR_SUCCESS 	 	: ȡ�óɹ� 
 **************************************/
UK_STATUS UK_Read_Ex_Version(int index,ex_version * val);


UK_STATUS UK_Read_BIOSSEL(int fd,int * sel);

UK_STATUS UK_Write_BIOSSEL(int fd,int sel);

int IRIGB_Parity(int fd, int en);

UK_STATUS UK_WriteMode(unsigned long serial,unsigned long trsconf);

UK_STATUS UK_ReadMode(unsigned long * serial,unsigned long * trsconf);

UK_STATUS UK_ReadPPSTIME(int * t);

UK_STATUS UK_SetGPIO(int value);


#ifdef __cplusplus
}
#endif
#endif
