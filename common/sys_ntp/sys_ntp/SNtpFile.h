#include "SApi.h"
#include "SNtp.h"
#include "sp_config_mgr.h"
class SNtpFile : public CConfigBase
{
public:
	SNtpFile();
	virtual ~SNtpFile();


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������ļ�
	// ��    ��:  �߳���
	// ����ʱ��:  2019-4-1 9:03
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Load(SString sPathFile);

	static void* ThreadStart(void *lp);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������ļ�ֵѡ���Ƿ�����ntp�ͻ��ˡ������
	// ��    ��:  �߳���
	// ����ʱ��:  2019-4-1 9:03
	// ����˵��:  
	// �� �� ֵ:  true��ʾ�����ػ����У�false��ʾ��Ҫ��ֹ����
	//////////////////////////////////////////////////////////////////////////
	bool Run(int argc, char* argv[]);

	
	SNtpClient *m_SntpClient;
	SNtpServer *m_SntpServer;
	bool m_bOpenClient;
	bool m_bOpenServer;
	int m_iTimeSecs;//��ȡʱ��������λΪ��
	SDateTime m_Now;
	SLock m_Lock;//������
};