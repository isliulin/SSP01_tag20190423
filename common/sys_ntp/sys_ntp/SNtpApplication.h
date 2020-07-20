#include "SApplication.h"
#include "SNtpFile.h"

class SNtpApplication : public SApplication 
{
public:
	SNtpApplication();
	virtual ~SNtpApplication();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������񣬿�������Ӧ�õķ��񣬸ú���������������ʵ�֣�������������ɺ���뷵��
	// ��    ��:  �߳���
	// ����ʱ��:  2019-4-1 9:03
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ���񣬹ر�����Ӧ�õķ���
	// ��    ��:  �߳���
	// ����ʱ��:  2019-4-1 9:03
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();
	virtual bool ProcessAgentMsg(WORD wMsgType,stuSpUnitAgentMsgHead *pMsgHead,SString &sHeadStr,BYTE* pBuffer=NULL,int iLength=0);
private:
	SNtpFile m_SntpFile;
};