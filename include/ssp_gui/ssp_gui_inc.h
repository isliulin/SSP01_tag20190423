/**
 *
 * �� �� �� : ssp_gui_inc.h
 * �������� : 2015-7-8 11:02
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SSPͼ���˻��Ի����򣨹���վ�����ƽ̨�ӿ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-8	�ۿ�������ļ�
 *
 **/
#ifndef SSP_GUI_INC_H
#define SSP_GUI_INC_H

// #ifdef SSP_GUI_LIB
// #ifdef WIN32
// #ifdef NDEBUG
// #pragma comment(lib,"sbase_qtgui.lib")
// //#pragma comment(lib,"ssp_gui.lib")
// #else
// #pragma comment(lib,"sbase_qtguid.lib")
// //#pragma comment(lib,"ssp_guid.lib")
// #endif
// #endif
// #define SSP_GUI_EXPORT __declspec(dllexport)
// 
// #else//SSP_GUI_LIB
// 
// #define SSP_GUI_EXPORT __declspec(dllimport)
// 
// #endif//SSP_GUI_LIB


#ifdef WIN32
  #ifdef _DEBUG
    #pragma comment(lib,"sbased.lib")
    #pragma comment(lib,"sbase_mdbd.lib")
    #ifdef SSP_DBUSED_MYSQL
      #pragma comment(lib,"sbase_mysqld.lib")
    #endif
    #ifdef SSP_DBUSED_PGSQL
      #pragma comment(lib,"sbase_pgsqld.lib")
    #endif
    #ifdef SSP_DBUSED_ORACLE
      #pragma comment(lib,"sbase_oracled.lib")
    #endif
    #pragma comment(lib,"sbase_zipd.lib")
    #pragma comment(lib,"sbase_qtguid.lib")
    #pragma comment(lib,"ssp_based.lib")
  #else//_DEBUG
    #pragma comment(lib,"sbase.lib")
    #pragma comment(lib,"sbase_mdb.lib")
    #ifdef SSP_DBUSED_MYSQL
      #pragma comment(lib,"sbase_mysql.lib")
    #endif
    #ifdef SSP_DBUSED_PGSQL
      #pragma comment(lib,"sbase_pgsql.lib")
    #endif
    #ifdef SSP_DBUSED_ORACLE
      #pragma comment(lib,"sbase_oracle.lib")
    #endif
    #pragma comment(lib,"sbase_zip.lib")
    #pragma comment(lib,"sbase_qtgui.lib")
    #pragma comment(lib,"ssp_base.lib")
  #endif//_DEBUG
  
  #ifdef SSP_GUI_LIB_STATIC  //��̬�⣬����Ҫ����������
    #define SSP_GUI_EXPORT
  #else //��̬�⣬������̬����Ϊ��������������Ϊ��������
    #ifdef  SSP_GUI_EXPORT_DLL
      #define SSP_GUI_EXPORT __declspec(dllexport)
    #else //SSP_GUI_EXPORT_DLL
      #define SSP_GUI_EXPORT __declspec(dllimport)
      #ifdef _DEBUG
        #pragma comment(lib,"ssp_guid.lib")
      #else//_DEBUG
        #pragma comment(lib,"ssp_gui.lib")
      #endif//_DEBUG
    #endif//SBASE_EXPORT_DLL
  #endif//SBASE_LIB_STATIC
#else//WIN32
#define SSP_GUI_EXPORT
#endif//WIN32

#endif // SSP_GUI_INC_H
