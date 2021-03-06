/*
名称：数据库结构脚本(for MDB)
版本：V2.0.1
作者：邵凯田
日期：2014-05-16

History:
 Ver  Date        Author  Comments
 ---  ----------  ------  -------------------------------------------
 001  2014-05-16  邵凯田  创建脚本文件初始版本
 002  2014-09-06  邵凯田  增加表5.44（状态量触发动作表T_STATE_ACTION）
 003  2014-11-14  邵凯田  修改为自定义的内存数据库
*/


--5.1 用户组表T_USER_GROUP
create table T_User_Group
(
   grp_code char(16) NOT NULL, 
   name char(32) NOT NULL, 
   dsc char(128), 
   constraint pk_usergrp primary key(grp_code)
);

--5.2 用户表T_USER
create table T_User
(
   usr_sn int not null,
   usr_code char(16) not null,
   grp_code char(16) not null,
   name char(32) not null,
   pwd char(32) not null,
   dsc char(128),
   email char(64),
   mobile char(16),
   create_time int default 0,
   constraint pk_user primary key(usr_sn)
);
create index idx_user_code on T_User (usr_code);

--5.3 功能点定义表T_FUN_POINT
create table T_Fun_Point
(
   fun_key char(16) not null,
   p_fun_key char(16) not null,
   name char(32) not null,
   dsc char(128),
   path char(512),
   constraint pk_funpoint primary key(fun_key)
);

--5.4 用户组授权表T_USERGROUP_AUTH
create table T_UserGroup_Auth
(
   grp_code char(16) not null,
   fun_key char(16) not null,
   auth byte not null,
   constraint pk_usrgrp_auth primary key(grp_code,fun_key)
);

--5.5 用户授权表T_USER_AUTH
create table T_User_Auth
(
   usr_sn int not null,
   fun_key char(16) not null,
   auth byte,
   constraint pk_usr_auth primary key(usr_sn,fun_key)
);

--5.6 公司表T_COMPANY
create table T_Company
(
   cmp_sn int not null,
   pcmp_sn short not null,
   uri char(32),
   name char(64),
   address char(32),
   constraint pk_cmp primary key(cmp_sn)
);

--5.7 厂站表T_SUBSTATION
create table T_SubStation
(
   sub_sn short not null,
   cmp_sn short not null,
   uri char(32),
   name char(32),
   address char(32),
   state byte default 1,
   comstate byte default 0,
   vlevel short,
   join_time int,
   ip_addr_a char(24),
   ip_addr_b char(24),
   svr_port int,
   svg_file blob,
   constraint pk_sub primary key(sub_sn)
);

--5.8 交流线路表T_ACLINE
create table T_AcLine
(
   acl_sn int not null,
   peer_acl_sn int not null,
   sub_sn short,
   code short,
   uri char(32),
   name char(16),
   state byte default 1,
   vlevel short,
   conductor_b0ch float,
   conductor_bch float,
   conductor_g0ch float,
   conductor_gch float,
   conductor_length float,
   conductor_r float,
   conductor_r0 float,
   conductor_x float,
   conductor_x0 float,
   conductor_rc float,
   conductor_rc0 float,
   ct_rate float,
   phase int,
   constraint pk_acl primary key(acl_sn)
);
create index idx_acline_peer_acl_sn on T_AcLine (peer_acl_sn);
create index idx_acline_sub_sn on T_AcLine (sub_sn);

--5.9 直流线路表T_DCLINE
create table T_DcLine
(
   dcl_sn int not null,
   peer_dcl_sn int not null,
   sub_sn short not null,
   code short not null,
   uri char(32),
   name char(16),
   state byte,
   vlevel short,
   conductor_b0ch float,
   conductor_bch float,
   conductor_g0ch float,
   conductor_gch float,
   conductor_length float,
   conductor_r float,
   conductor_r0 float,
   conductor_x float,
   conductor_x0 float,
   conductor_rc float,
   conductor_rc0 float,
   ct_rate float,
   phase int,
   constraint pk_dcl primary key(dcl_sn)
);
create index idx_dcline_peer_dcl_sn on T_DcLine (peer_dcl_sn);
create index idx_dcline_sub_sn on T_DcLine (sub_sn);

--5.10 母线表T_BUSBAR
create table T_Busbar
(
   bus_sn int not null,
   sub_sn short not null,
   code short not null,
   uri char(32),
   name char(16),
   state byte default 1,
   type byte default 0,
   vlevel short,
   constraint pk_bus primary key(bus_sn)
);
create index idx_busbar_sub_sn on T_Busbar (sub_sn);

--5.11 变压器表T_TRANSFORMER
create table T_Transformer
(
   tsf_sn int not null,
   sub_sn short not null,
   code short not null,
   uri char(32),
   name char(16),
   state byte default 1,
   type byte default 0,
   vlevel short,
   vlevel_h short,
   vlevel_m short,
   vlevel_l short,
   capacity float,
   constraint pk_tsf primary key(tsf_sn)
);
create index idx_transf_sub_sn on T_Transformer (sub_sn);

--5.12 断路器表T_BREAKER
create table T_Breaker
(
   brk_sn int not null,
   sub_sn short not null,
   code short not null,
   uri char(32),
   name char(16),
   state byte default 1,
   type byte default 0,
   vlevel short,
   constraint pk_brk primary key(brk_sn)
);
create index idx_breaker_sub_sn on T_Breaker (sub_sn);

--5.13 发电机表T_GENERATOR
create table T_Generator
(
   gen_sn int not null,
   sub_sn short not null,
   code short not null,
   uri char(32),
   name char(16),
   state byte default 1,
   vlevel short,
   capacity float,
   constraint pk_gen primary key(gen_sn)
);
create index idx_generator_sub_sn on T_Generator (sub_sn);

--5.14 电抗器表T_REACTANCE
create table T_Reactance
(
   rct_sn int not null,
   sub_sn short not null,
   code short not null,
   uri char(32),
   name char(16),
   state byte default 1,
   vlevel short,
   capacity float,
   constraint pk_rct primary key(rct_sn)
);
create index idx_reactance_sub_sn on T_Reactance (sub_sn);

--5.15 电容器表T_CAPACITANCE
create table T_Capacitance
(
   cap_sn int not null,
   sub_sn short not null,
   code short not null,
   uri char(32),
   name char(16),
   state byte default 1,
   vlevel short,
   capacity float,
   constraint pk_cap primary key(cap_sn)
);
create index idx_capacitance_sub_sn on T_Capacitance (sub_sn);

--5.16 刀闸表T_DISCONN
create table T_DisConn
(
   dis_sn int not null,
   sub_sn short not null,
   code short not null,
   uri char(32),
   name char(16),
   state byte default 1,
   vlevel short,
   type byte default 0,
   constraint pk_disconn primary key(dis_sn)
);
create index idx_disconn_sub_sn on T_DisConn (sub_sn);

--5.17 二次设备表T_IED
create table T_Ied
(
   ied_sn int not null,
   sub_sn short not null,
   deviceno short not null,
   name char(32) not null,
   ied_type byte not null,
   pri_code short not null,
   uri char(32),
   dev_type char(16),
   inf_sn int,
   dev_ver char(32),
   commstate byte default 0,
   constraint pk_ied_ primary key(ied_sn)
);

--5.18 二次设备CPU表T_IED_CPU
create table T_Ied_Cpu
(
   ied_sn int not null,
   cpuno byte not null,
   name char(16) not null,
   ver char(32),
   constraint pk_ied_cpu primary key(ied_sn,cpuno)
);

--5.19 信息点表类型表T_INFOTAB
create table T_InfoTab
(
   inf_sn int not null,
   name char(64) not null,
   ver char(32),
   author char(16),
   type byte default 0,
   manufacturer char(16),
   in_time int,
   file_size int,
   file blob,
   constraint pk_inftab primary key(inf_sn)
);

--5.20 信息点表参数表T_INOTAB_PARAM
create table T_InfoTab_Param
(
   inf_sn int not null,
   comm_type char(16) not null,
   param_name char(32) not null,
   param_type byte not null,
   dsc char(256),
   defval char(128),
   constraint pk_infotab_param primary key(inf_sn,comm_type,param_name)
);

--5.21 信息点表类型CPU表T_INFOTAB_CPU
create table T_InfoTab_Cpu
(
   inf_sn int not null,
   cpuno byte not null,
   name char(16) not null,
   ver char(32),
   constraint pk_infotab_cpu primary key(inf_sn,cpuno)
);

--5.22 组标题定义表T_GROUP_TITLE
create table T_Group_Title
(
   gt_sn int not null,
   inf_sn int not null,
   cpuno byte not null,
   groupno short not null,
   name char(16) not null,
   type byte not null,
   constraint pk_grouptitle primary key(gt_sn)
);
create index idx_gt_inf_sn on T_Group_Title (inf_sn);

--5.23 通用分类组条目表T_GT_GROUP
create table T_GT_Group
(
   gt_sn int not null,
   entry short not null,
   name char(32) not null,
   valtype byte not null,
   dime char(8) ,
   maxval float,
   minval float,
   stepval float,
   precision_n byte,
   precision_m byte,
   mmspath char(128),
   constraint pk_gt_group primary key(gt_sn,entry)
);
create index idx_gt_group_gtsn on T_GT_Group (gt_sn);

--5.24 状态量组条目表T_GT_STATE
create table T_GT_State
(
   gt_sn int not null,
   fun short not null,
   inf short not null,
   entry short not null,
   name char(32) not null,
   type byte not null,
   evt_cls byte not null,
   val_type byte not null,
   on_dsc char(8),
   off_dsc char(8),
   complex_type byte not null,
   mmspath char(128),
   constraint pk_gt_state primary key(gt_sn,fun,inf)
);
create index idx_gt_state_gtsn on T_GT_State (gt_sn);

--5.25 二次设备组内容表T_GROUP_VAL
create table T_Group_Val
(
   ied_sn int not null,
   cpuno byte not null,
   sector byte not null,
   groupno short not null,
   entry short not null,
   sub_sn short not null,
   deviceno short not null,
   currval char(32) not null,
   referval char(32) not null,
   constraint pk_groupval primary key(ied_sn,cpuno,sector,groupno,entry)
);
create index idx_groupval_subdev on T_Group_Val (sub_sn,deviceno);

--5.26 二次设备状态内容表T_STATE_VAL
create table T_State_Val
(
   ied_sn int not null,
   cpuno byte not null,
   fun short not null,
   inf short not null,
   sub_sn short not null,
   deviceno short not null,
   curval byte not null,
   constraint pk_stateval primary key(ied_sn,cpuno,fun,inf)
);
create index idx_stateval_subdev on T_State_Val (sub_sn,deviceno);

--5.27 综自地址映射表T_SCADA_ADDR
create table T_Scada_Addr
(
   deviceno short not null,
   cpuno byte not null,
   addr short not null,
   dsc char(32),
   constraint pk_scadaaddr primary key(deviceno,cpuno)
);

--5.28 子站运行模块表T_MODULE
create table T_Module
(
   mod_sn int not null,
   mod_name char(32) not null,
   mod_type byte not null,
   ver char(16) not null,
   crc char(16) not null,
   create_time char(16),
   constraint pk_submod primary key(mod_name)
);
create index idx_submod_type on T_Module (mod_type);

--5.29 子站运行模块参数表T_MODULE_PARAM
create table T_Module_Param
(
   mod_sn int not null,
   comm_type char(16) not null,
   param_name char(32) not null,
   param_type byte not null,
   dsc char(256) not null,
   defval char(128),
   constraint pk_submod_param primary key(mod_sn,comm_type,param_name)
);

--5.30 运行设备表T_RUNDEV
create table T_RunDev
(
   dev_no int not null,
   dev_name char(64) not null,
   p_dev_no int not null,
   dev_type byte not null,
   comm_state byte not null,
   inf_sn int not null,
   mod_name char(32) not null,
   unit_no byte not null,
   comm_type char(16) not null,
   run_state byte not null,
   remote_ctrl byte,
   clock_sync_s int,
   loop_ms int,
   need_wave byte,
   constraint pk_rundev primary key(dev_no)
);

--5.31 运行设备参数表T_RUNDEV_PARAM
create table T_RunDev_Param
(
   dev_no int not null,
   comm_type char(16) not null,
   param_name char(64) not null,
   param_type byte not null,
   dsc char(256) not null,
   defval char(128),
   curval char(128),
   constraint pk_rundev_param primary key(dev_no,comm_type,param_name)
);

--5.32 运行设备CPU表T_RUNDEV_CPU（删除）
--create table T_RunDev_Cpu
--(
--   dev_no int not null,
--   cpuno byte not null,
--   name char(16) not null,
--   ver char(16),
--   constraint pk_rundev_cpu primary key(dev_no,cpuno)
--);

--5.33 历史事件表T_HIS_EVENT
create table T_His_Event
(
   evt_seq int not null,
   sub_sn short not null,
   deviceno short not null,
   cpuno byte not null,
   act_soc int not null,
   act_usec int not null,
   fun short not null,
   inf short not null,
   type byte not null,
   rettime short not null,
   dpi byte not null,
   nof int,
   fan int,
   act_q int,
   time_q int,
   constraint pk_his_event primary key(evt_seq)
);
create index idx_his_event_soc on T_His_Event (act_soc);
create index idx_his_event_sub_dev on T_His_Event (sub_sn,deviceno);

--5.34 历史故障信息表T_HIS_FAULTINFO
create table T_His_FaultInfo
(
   fi_seq int not null,
   evt_seq int not null,
   sub_sn short not null,
   deviceno short not null,
   cpuno byte not null,
   act_soc int not null,
   act_usec int not null,
   groupno short not null,
   entry short not null,
   type byte not null,
   val char(32) not null,
   constraint pk_his_faultinfo primary key(fi_seq)
);
create index idx_his_faultinfo_evt_seq on T_His_FaultInfo (evt_seq);
create index idx_his_faultinfo_soc on T_His_FaultInfo (act_soc);
create index idx_his_faultinfo_sub_dev on T_His_FaultInfo (sub_sn,deviceno);

--5.35 历史录波表T_HIS_WAVE
create table T_His_Wave
(
   wave_seq int not null,
   sub_sn short not null,
   deviceno short not null,
   cpuno byte not null,
   act_soc int not null,
   act_usec int not null,
   filename char(128) not null,
   files byte not null,
   cfg_bytes int,
   dat_bytes int,
   hdr_bytes int,
   ext_bytes int,
   constraint pk_his_wave primary key(wave_seq)
);
create index idx_his_wave_soc on T_His_Wave (act_soc);
create index idx_his_wave_sub_dev on T_His_Wave (sub_sn,deviceno);

--5.36 上行发送状态表T_UP_SEND_STATE
create table T_Up_Send_State
(
   table_type byte not null,
   row_seq int not null,
   updev_no short not null,
   state byte not null,
   constraint pk_up_send_state primary key(table_type,row_seq,updev_no)
);

--5.37 故障报告表T_FAULT_REPORT
create table T_Fault_Report
(
   rpt_seq int not null,
   sub_sn short not null,
   flt_soc int not null,
   flt_usec int not null,
   gen_soc int not null,
   pri_type byte not null,
   pri_code short not null,
   fpt byte,
   jpt byte,
   scl float,
   constraint pk_fault_report primary key(rpt_seq)
);
create index idx_fltrpt_sub_flt on T_Fault_Report (sub_sn,flt_soc);

--5.38 故障报告明细表T_FLTRPT_DETAIL
create table T_FltRpt_Detail
(
   rpt_seq int not null,
   detail_seq int not null,
   rettime int not null,
   type byte not null,
   deviceno short not null,
   evt_dsc char(64) not null,
   evt_seq int not null,
   constraint pk_fltrpt_detail primary key(rpt_seq,detail_seq)
);

--5.39 系统日志表T_SYSLOG
create table T_SysLog
(
   log_seq int not null,
   sub_sn short not null,
   usr_sn int not null,
   deviceno short not null,
   log_type byte not null,
   log_val float ,
   log_text char(256) ,
   constraint pk_syslog primary key(log_seq)
);

--5.40 运行单元表T_RUN_UNIT
create table T_Run_Unit
(
   unit_no byte not null,
   unit_name char(16) not null,
   ip_addr char(32) not null,
   constraint pk_rununit primary key(unit_no)
);

--5.41 数据库信息表T_DB_INFO
create table T_Db_Info
(
   db_sn int not null,
   db_ver char(16) not null,
   upgrade_time char(24) ,
   constraint pk_dbinfo primary key(db_sn)
);

--5.42 全局配置类型表T_SETUP_TYPE
create table T_Setup_Type
(
   type_id short not null,
   type_name char(32) not null,
   constraint pk_setup_type primary key(type_id)
);

--5.43 全局配置项表T_SETUP
create table T_Setup
(
   set_key char(32) not null,
   type_id short ,
   set_desc char(64) not null,
   set_default char(128),
   set_value char(128),
   set_dime char(8),
   set_class byte not null,
   val_type byte not null,
   val_fmt char(256),
   constraint pk_setup primary key(set_key)
);
create index idx_setup_type on T_Setup (type_id);

--5.44状态量触发动作表T_STATE_ACTION
create table T_State_Action
(
   gt_sn int not null,
   fun short not null,
   inf short not null,
   sub_sn short not null,
   deviceno short not null,
   readonly byte not null,
   on_trg_action byte,
   off_trg_action byte,
   trg_object byte,
   constraint pk_stateaction primary key(gt_sn,fun,inf)
);

--5.45信息上送策略表T_MSG_ORDER
create table T_Msg_Order
(
  deviceno short not null,
  up_devno short not null,
  visible byte,
  intelligent byte,
  run_order byte,
  debug_order byte,
  stop_order byte,
  check_order byte,
  send_event byte,
  send_alert byte,
  send_switch byte,
  send_wave byte,
  constraint pk_msg_order primary key(deviceno,up_devno)
);

--5.46信息上送策略表T_MSG_ORDERX
create table T_Msg_OrderX
(
  deviceno short not null,
  up_devno short not null,
  cpuno byte not null,
  fun short not null,
  inf short not null,
  run_order byte,
  debug_order byte,
  stop_order byte,
  check_order byte,
  constraint pk_msg_orderx primary key(deviceno,up_devno,cpuno,fun,inf)
);
create index idx_msg_orderx on T_Msg_OrderX (deviceno,up_devno);
