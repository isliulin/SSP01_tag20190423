/**
 *
 * �� �� �� : ssp_gui_3d_inc.h
 * �������� : 2017-3-2 10:35
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SSP 3D ��������ͷ�ļ�
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2017-3-2	�ۿ�������ļ�
 *
 **/

#ifndef __SSP_GUI_3D_INCLUDE_H__
#define __SSP_GUI_3D_INCLUDE_H__


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
    #pragma comment(lib,"ssp_guid.lib")
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
    #pragma comment(lib,"ssp_gui.lib")
  #endif//_DEBUG

  #ifdef SSP_GUI_3D_LIB_STATIC  //��̬�⣬����Ҫ����������
    #define SSP_GUI_3D_EXPORT
  #else //��̬�⣬������̬����Ϊ��������������Ϊ��������
    #ifdef  SSP_GUI_3D_EXPORT_DLL
      #define SSP_GUI_3D_EXPORT __declspec(dllexport)
    #else //SSP_GUI_3D_EXPORT_DLL
      #define SSP_GUI_3D_EXPORT __declspec(dllimport)
      #ifdef _DEBUG
        #pragma comment(lib,"ssp_gui_3dd.lib")
      #else//_DEBUG
        #pragma comment(lib,"ssp_gui_3d.lib")
      #endif//_DEBUG
    #endif//SBASE_EXPORT_DLL
  #endif//SBASE_LIB_STATIC
#else//WIN32
  #define SSP_GUI_3D_EXPORT
#endif//WIN32

#endif//__SSP_GUI_3D_INCLUDE_H__
