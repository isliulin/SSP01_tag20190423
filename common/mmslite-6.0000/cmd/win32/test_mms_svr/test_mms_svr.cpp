// test_mms_clt.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SMmsServer.h"
#include "SLog.h"
//#include "vld.h"
  
class CMyMmsSvr : public SMmsServer
{
public:
	CMyMmsSvr()
	{
		m_map.setAutoDelete(true);
	};
	virtual ~CMyMmsSvr(){};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ӳ��Ҷ�ӽڵ��û�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-30 8:55
	// ����˵��:  @sIedLdName��ʾIED+LD����
	//         :  @sLeafPath��ʾҶ�ӽڵ�·��
	//         :  @ppLeafMap��ʾҶ�ӽڵ���û�ӳ��ָ��ĵ�ַ��ӳ��ָ��Ϊ�գ��û�������Ƿ���Ҫӳ�䣬��Ҫʱ����stuLeafMap������дָ�룬MMS�����ڲ��������ͷ�
	// �� �� ֵ:  true��ʾ�ɹ�ӳ�䣬false��ʾ�޷�ӳ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnMapLeafToUser(char* sIedLdName, char* sLeafPath, stuLeafMap **ppLeafMap)
	{
		if(strstr(sLeafPath,"stVal") != NULL)
		{
			*ppLeafMap = new stuLeafMap();
			m_map.append(*ppLeafMap);
			return true;
		}
		return false;
	}
	//virtual bool OnReadLeafValue(const char* sIedLdName, const char* sLeafPath, stuLeafMap *pLeafMap, void *mvluRdVaCtrl, int reason = 0){return false;};
	SPtrList<stuLeafMap> m_map;
};

void test_mmssvr()
{

}

void* ThreadRpt(void* lp)
{
	CMyMmsSvr *pSvr = (CMyMmsSvr*)lp;
	unsigned long pos;
	int stVal = 0;
	while(1)
	{
		stVal++;
		if(stVal > 1)
			stVal = 0;
		stuLeafMap *pMap = pSvr->m_map.FetchFirst(pos);
		while(pMap)
		{
			pSvr->NewReportPush(pMap,stVal);	
			pMap = pSvr->m_map.FetchNext(pos);
		}
		pSvr->NewReportProc();
		SApi::UsSleep(3000000);
	}
}

int main(int argc, _TCHAR* argv[])
{
	SLog::startLogToConsole();
	SLog::setLogLevel(SLog::LOG_DEBUG);
	LOGDEBUG("Into main");
	SLog::startLogToConsole();
	SLog::setLogLevel(SLog::LOG_DEBUG);
	CMyMmsSvr svr;
	svr.SetRootPath("d:\\");
	//SString scd = "sac.scd";
	//SString scd = "110kV���±�-20100319.scd";//"uk.scd";
	SString scd = "Z:\\Software_Platform\\common\\sim_iec61850\\bin\\rms.scd";//"uk.scd";
	scd="Z:\\Software_Platform\\common\\sim_iec61850\\bin\\uk.scd";
	scd="Z:\\Software_Platform\\common\\sim_iec61850\\conf\\1.scd";//Z:\Software_Platform\common\sim_iec61850\conf
	scd="d:\\uk.scd";
	//SString scd = "��Ϫ���̿����汾111207.scd";//"uk.scd";
#if 1
	printf("begin to add by scd... \n");

	/*
	����SCL 1 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=1-PT2201,AP=S1)���Ϊ:0, ��ʱ:612.000000ms
	����SCL 2 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=2-CL2201,AP=S1)���Ϊ:0, ��ʱ:721.000000ms
	����SCL 3 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=3-CL2202,AP=S1)���Ϊ:0, ��ʱ:775.000000ms
	����SCL 4 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=4-PL2201,AP=S1)���Ϊ:0, ��ʱ:1035.000000ms
	����SCL 5 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=5-PL2202,AP=S1)���Ϊ:0, ��ʱ:1083.000000ms
	����SCL 6 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=6-PF2201,AP=S1)���Ϊ:0, ��ʱ:2201.000000ms
	����SCL 7 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=7-PM2201,AP=S1)���Ϊ:0, ��ʱ:1405.000000ms
	����SCL 8 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=8-PM2201,AP=G1)���Ϊ:0, ��ʱ:1174.000000ms
	����SCL 9 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=9-CT2201,AP=S1)���Ϊ:0, ��ʱ:1390.000000ms
	����SCL 10 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=10-PL1101,AP=S1)���Ϊ:0, ��ʱ:1738.000000ms
	����SCL 11 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=11-PL1102,AP=S1)���Ϊ:0, ��ʱ:1902.000000ms
	����SCL 12 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=12-PL1001,AP=S1)���Ϊ:0, ��ʱ:1257.000000ms
	����SCL 13 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=13-PL1002,AP=S1)���Ϊ:0, ��ʱ:1210.000000ms
	����SCL 14 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=14-PC1001,AP=S1)���Ϊ:0, ��ʱ:1133.000000ms
	����SCL 15 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=15-PC1002,AP=S1)���Ϊ:0, ��ʱ:1152.000000ms
	����SCL 16 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=16-PS1001,AP=S1)���Ϊ:0, ��ʱ:1013.000000ms
	����SCL 17 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=17-PS1002,AP=S1)���Ϊ:0, ��ʱ:963.000000ms
	����SCL 18 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=18-ECFL,AP=S1)���Ϊ:0, ��ʱ:898.000000ms
	����SCL 19 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=19-HJJC,AP=S1)���Ϊ:0, ��ʱ:931.000000ms
	����SCL 20 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=20-XDC,AP=S1)���Ϊ:0, ��ʱ:946.000000ms
	����SCL 21 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=21-YTHDY,AP=S1)���Ϊ:0, ��ʱ:895.000000ms
	����SCL 22 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=22-GHJ,AP=S1)���Ϊ:3, ��ʱ:403.000000ms
	IED:22-GHJ/S1ģ�ͼ���ʧ��!
	����SCL 23 (file=Z:\Software_Platform\common\sim_iec61850\conf\1.scd,IED=23-GPS,AP=S1)���Ϊ:3, ��ʱ:269.000000ms
	IED:23-GPS/S1ģ�ͼ���ʧ��!

	*/
	svr.AddIedByScd(scd);
	/*
	svr.AddIed(stuMmsServerIed::New(scd,1,"PT2201","S1"));
 	svr.AddIed(stuMmsServerIed::New(scd,1,"GHJ","S1"));
 	svr.AddIed(stuMmsServerIed::New(scd,1,"GPS","S1"));
	*/
	printf("ieds=%d\n",svr.m_Ieds.count());
#else
	SXmlConfig xml;
	if(!xml.ReadConfig(scd))
	{
		printf("open scl file:%s error!\n",scd.data());
		return 0;
	}
	SBaseConfig *pCommunication = xml.SearchChild("Communication");
	if(pCommunication != NULL)
	{
		int cnt = 0;
		unsigned long pos,pos2;
		SBaseConfig *pSubNetwork = pCommunication->GetChildPtr()->FetchFirst(pos);
		while(pSubNetwork)
		{
			if(pSubNetwork->GetAttribute("type").find("MMS") >= 0)
			{
				SBaseConfig *pConnectedAP = pSubNetwork->GetChildPtr()->FetchFirst(pos2);
				while(pConnectedAP)
				{
					svr.AddIed(stuMmsServerIed::New(scd,0,pConnectedAP->GetAttribute("iedName"),pConnectedAP->GetAttribute("apName")));
					printf("cnt=%d %s %s\n",++cnt,pConnectedAP->GetAttribute("iedName").data(),pConnectedAP->GetAttribute("apName").data());
					pConnectedAP = pSubNetwork->GetChildPtr()->FetchNext(pos2);
				}
			}
			pSubNetwork = pCommunication->GetChildPtr()->FetchNext(pos);
		}
	}
	xml.clear();
#endif
//  	svr.AddIed(stuMmsServerIed::New(scd,0,"CG1001","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PL1102","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PL1112","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PM1101A","S1"));
	//svr.AddIed(stuMmsServerIed::New(scd,0,"PM1101B","S1"));
//	svr.AddIed(stuMmsServerIed::New(scd,0,"PE1101","S1"));


// 	svr.AddIed(stuMmsServerIed::New(scd,0,"RMS601","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"CSC103BA","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"ZH5D","A1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PCS921GA","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PCS931GA","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"RCS974AA","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PCS977DA","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"S3QSACCB1","P1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"S3P1PCPB1","P1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"WDGL_VII","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"ISA381GA","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PRS721AA","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PRS723CA","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PRS753DA","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PRS7532A","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PRS753SA","S1"));

// 	svr.AddIed(stuMmsServerIed::New(scd,0,"PSR662UP","S1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"ZH5X01","A1"));
// 	svr.AddIed(stuMmsServerIed::New(scd,0,"RMS601","S1"));

// 	svr.AddIed(stuMmsServerIed::New(scd,0,"E1Q1SB1","S1"));
	svr.Start();
	printf("printf any key to quit!");
	SKT_CREATE_THREAD(ThreadRpt,&svr);

	while(1)
	{
		char ch = getchar();
		if(ch == '0')
			svr.SetGlobalCtrl_En(false,false,false);
		else if(ch == '1')
			svr.SetGlobalCtrl_En(true,true,true);
	}

	svr.Stop();

	SApi::UsSleep(1000000);
	SLog::WaitForLogEmpty();
	SLog::quitLog();
	return 0;
}

