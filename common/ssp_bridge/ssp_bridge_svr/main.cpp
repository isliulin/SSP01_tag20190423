/**
 *
 * �� �� �� : main.cpp
 * �������� : 2018-10-12 16:36
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �Žӷ���������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2018-10-12	�ۿ�������ļ�
 *
 **/

#include "BridgeSvrApp.h"
#define MOD_VERSION "1.0.0"

#ifndef MOD_DATE
#define MOD_DATE "2018-10-12"
#endif
#ifndef MOD_TIME
#define MOD_TIME "21:41:00"
#endif

int main(int argc, char* argv[])
{
	CBridgeSvrApp g_app;
	g_app.SetModuleDesc("�Žӷ���������");
	g_app.SetVersion(MOD_VERSION);
	g_app.Run(argc,argv,MOD_DATE,MOD_TIME);

	g_app.Stop();
	return 0;
}
