/**
 *
 * �� �� �� : main.cpp
 * �������� : 2015-8-1 13:31
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ���Ź�������(��Ӳ�����Ź�)
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-1	�ۿ�������ļ�
 *
 **/

//////////////////////////////////////////////////////////////////////////
// History
// 1.0.0	2015-08-01  �ۿ�����Ź�����
//////////////////////////////////////////////////////////////////////////

#include "WatchApplication.h"

CWatchApplication g_app;

void test()
{
	SString str;
	SStringList list;
	int p,pid,i,ps=SApi::GetSystemProcess(list);
	for(i=0;i<ps;i++)
	{
		str = *list[i];
		printf("%s\n",str.data());
	}
	printf("\n\n");
	getchar();
}

int main(int argc, char* argv[])
{
	//test();
	g_app.SetModuleDesc("���Ź�����");
	g_app.SetVersion("1.0.0");
	g_app.Run(argc,argv,__DATE__,__TIME__);

	return 0;
}
