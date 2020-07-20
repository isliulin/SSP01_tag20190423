﻿DROP TABLE IF EXISTS `t_acline`;
CREATE TABLE `t_acline` (
  `acl_sn` decimal(12,0) NOT NULL,
  `peer_acl_sn` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) DEFAULT NULL,
  `code` decimal(5,0) DEFAULT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(16) DEFAULT NULL,
  `state` decimal(1,0) DEFAULT '1',
  `vlevel` decimal(5,0) DEFAULT NULL,
  `conductor_b0ch` decimal(12,4) DEFAULT NULL,
  `conductor_bch` decimal(12,4) DEFAULT NULL,
  `conductor_g0ch` decimal(12,4) DEFAULT NULL,
  `conductor_gch` decimal(12,4) DEFAULT NULL,
  `conductor_length` decimal(12,4) DEFAULT NULL,
  `conductor_r` decimal(12,4) DEFAULT NULL,
  `conductor_r0` decimal(12,4) DEFAULT NULL,
  `conductor_x` decimal(12,4) DEFAULT NULL,
  `conductor_x0` decimal(12,4) DEFAULT NULL,
  `conductor_rc` decimal(12,4) DEFAULT NULL,
  `conductor_rc0` decimal(12,4) DEFAULT NULL,
  `ct_rate` decimal(12,4) DEFAULT NULL,
  `phase` decimal(8,0) DEFAULT NULL,
  UNIQUE KEY `pk_acl` (`acl_sn`),
  KEY `idx_acline_peer_acl_sn` (`peer_acl_sn`),
  KEY `idx_acline_sub_sn` (`sub_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_breaker"
#

DROP TABLE IF EXISTS `t_breaker`;
CREATE TABLE `t_breaker` (
  `brk_sn` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `code` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(16) DEFAULT NULL,
  `state` decimal(1,0) DEFAULT '1',
  `type` decimal(1,0) DEFAULT '0',
  `vlevel` decimal(5,0) DEFAULT NULL,
  UNIQUE KEY `pk_brk` (`brk_sn`),
  KEY `idx_breaker_sub_sn` (`sub_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_busbar"
#

DROP TABLE IF EXISTS `t_busbar`;
CREATE TABLE `t_busbar` (
  `bus_sn` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `code` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(16) DEFAULT NULL,
  `state` decimal(1,0) DEFAULT '1',
  `type` decimal(1,0) DEFAULT '0',
  `vlevel` decimal(5,0) DEFAULT NULL,
  UNIQUE KEY `pk_bus` (`bus_sn`),
  KEY `idx_busbar_sub_sn` (`sub_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_capacitance"
#

DROP TABLE IF EXISTS `t_capacitance`;
CREATE TABLE `t_capacitance` (
  `cap_sn` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `code` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(16) DEFAULT NULL,
  `state` decimal(1,0) DEFAULT '1',
  `vlevel` decimal(5,0) DEFAULT NULL,
  `capacity` decimal(12,4) DEFAULT NULL,
  UNIQUE KEY `pk_cap` (`cap_sn`),
  KEY `idx_capacitance_sub_sn` (`sub_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_col_test"
#

DROP TABLE IF EXISTS `t_col_test`;
CREATE TABLE `t_col_test` (
  `name` varchar(32) NOT NULL DEFAULT '',
  `value1` float DEFAULT NULL,
  `value2` float DEFAULT NULL,
  `value3` float DEFAULT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

#
# Source for table "t_company"
#

DROP TABLE IF EXISTS `t_company`;
CREATE TABLE `t_company` (
  `cmp_sn` decimal(5,0) NOT NULL,
  `pcmp_sn` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `address` varchar(32) DEFAULT NULL,
  UNIQUE KEY `pk_cmp` (`cmp_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_data_window"
#

DROP TABLE IF EXISTS `t_data_window`;
CREATE TABLE `t_data_window` (
  `dw_sn` decimal(8,0) NOT NULL DEFAULT '0',
  `dstype` decimal(1,0) DEFAULT NULL,
  `name` varchar(32) DEFAULT NULL,
  `dbtype` decimal(1,0) DEFAULT NULL,
  `desc` varchar(32) DEFAULT NULL,
  `select` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`dw_sn`),
  UNIQUE KEY `pk_data_window` (`dw_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_data_window_col"
#

DROP TABLE IF EXISTS `t_data_window_col`;
CREATE TABLE `t_data_window_col` (
  `dw_sn` decimal(8,0) NOT NULL DEFAULT '0',
  `col_sn` decimal(5,0) NOT NULL DEFAULT '0',
  `col_name` varchar(32) DEFAULT NULL,
  `col_vtype` varchar(16) DEFAULT NULL,
  `col_width` decimal(5,0) DEFAULT NULL,
  `ref_name` varchar(32) DEFAULT NULL,
  `pkey` decimal(1,0) DEFAULT NULL,
  `isnull` decimal(1,0) DEFAULT NULL,
  `ukey` decimal(1,0) DEFAULT NULL,
  PRIMARY KEY (`dw_sn`,`col_sn`),
  KEY `pk_data_window_col` (`dw_sn`,`col_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_data_window_row"
#

DROP TABLE IF EXISTS `t_data_window_row`;
CREATE TABLE `t_data_window_row` (
  `dw_sn` decimal(8,0) NOT NULL DEFAULT '0',
  `row_sn` decimal(5,0) NOT NULL DEFAULT '0',
  `row_id` varchar(32) DEFAULT NULL,
  `row_name` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`dw_sn`,`row_sn`),
  KEY `pk_data_window_row` (`dw_sn`,`row_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_db_info"
#

DROP TABLE IF EXISTS `t_db_info`;
CREATE TABLE `t_db_info` (
  `db_sn` decimal(12,0) NOT NULL,
  `db_ver` varchar(16) NOT NULL,
  `upgrade_time` varchar(24) DEFAULT NULL,
  UNIQUE KEY `pk_dbinfo` (`db_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_dcline"
#

DROP TABLE IF EXISTS `t_dcline`;
CREATE TABLE `t_dcline` (
  `dcl_sn` decimal(12,0) NOT NULL,
  `peer_dcl_sn` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `code` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(16) DEFAULT NULL,
  `state` decimal(1,0) DEFAULT NULL,
  `vlevel` decimal(5,0) DEFAULT NULL,
  `conductor_b0ch` decimal(12,4) DEFAULT NULL,
  `conductor_bch` decimal(12,4) DEFAULT NULL,
  `conductor_g0ch` decimal(12,4) DEFAULT NULL,
  `conductor_gch` decimal(12,4) DEFAULT NULL,
  `conductor_length` decimal(12,4) DEFAULT NULL,
  `conductor_r` decimal(12,4) DEFAULT NULL,
  `conductor_r0` decimal(12,4) DEFAULT NULL,
  `conductor_x` decimal(12,4) DEFAULT NULL,
  `conductor_x0` decimal(12,4) DEFAULT NULL,
  `conductor_rc` decimal(12,4) DEFAULT NULL,
  `conductor_rc0` decimal(12,4) DEFAULT NULL,
  `ct_rate` decimal(12,4) DEFAULT NULL,
  `phase` decimal(8,0) DEFAULT NULL,
  UNIQUE KEY `pk_dcl` (`dcl_sn`),
  KEY `idx_dcline_peer_dcl_sn` (`peer_dcl_sn`),
  KEY `idx_dcline_sub_sn` (`sub_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_disconn"
#

DROP TABLE IF EXISTS `t_disconn`;
CREATE TABLE `t_disconn` (
  `dis_sn` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `code` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(16) DEFAULT NULL,
  `state` decimal(1,0) DEFAULT '1',
  `vlevel` decimal(5,0) DEFAULT NULL,
  `type` decimal(1,0) DEFAULT '0',
  UNIQUE KEY `pk_disconn` (`dis_sn`),
  KEY `idx_disconn_sub_sn` (`sub_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_fault_report"
#

DROP TABLE IF EXISTS `t_fault_report`;
CREATE TABLE `t_fault_report` (
  `rpt_seq` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `flt_soc` int(11) NOT NULL,
  `flt_usec` int(11) NOT NULL,
  `gen_soc` int(11) NOT NULL,
  `pri_type` decimal(2,0) NOT NULL,
  `pri_code` decimal(5,0) NOT NULL,
  `fpt` decimal(3,0) DEFAULT NULL,
  `jpt` decimal(3,0) DEFAULT NULL,
  `scl` decimal(8,3) DEFAULT NULL,
  UNIQUE KEY `pk_fault_report` (`rpt_seq`),
  KEY `idx_fltrpt_sub_flt` (`sub_sn`,`flt_soc`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_fltrpt_detail"
#

DROP TABLE IF EXISTS `t_fltrpt_detail`;
CREATE TABLE `t_fltrpt_detail` (
  `rpt_seq` decimal(12,0) NOT NULL,
  `detail_seq` decimal(8,0) NOT NULL,
  `rettime` decimal(12,0) NOT NULL,
  `type` decimal(1,0) NOT NULL,
  `deviceno` decimal(5,0) NOT NULL,
  `evt_dsc` varchar(64) NOT NULL,
  `evt_seq` decimal(18,0) NOT NULL,
  UNIQUE KEY `pk_fltrpt_detail` (`rpt_seq`,`detail_seq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_fun_point"
#

DROP TABLE IF EXISTS `t_fun_point`;
CREATE TABLE `t_fun_point` (
  `fun_key` varchar(16) NOT NULL DEFAULT '',
  `p_fun_key` varchar(16) DEFAULT NULL,
  `name` varchar(32) DEFAULT NULL,
  `idx` decimal(3,0) DEFAULT NULL,
  `auth` decimal(1,0) DEFAULT NULL,
  `type` decimal(1,0) DEFAULT NULL,
  `ref_sn` decimal(8,0) DEFAULT NULL,
  `dsc` varchar(128) DEFAULT NULL,
  `path` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`fun_key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_generator"
#

DROP TABLE IF EXISTS `t_generator`;
CREATE TABLE `t_generator` (
  `gen_sn` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `code` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(16) DEFAULT NULL,
  `state` decimal(1,0) DEFAULT '1',
  `vlevel` decimal(5,0) DEFAULT NULL,
  `capacity` decimal(12,4) DEFAULT NULL,
  UNIQUE KEY `pk_gen` (`gen_sn`),
  KEY `idx_generator_sub_sn` (`sub_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_group_title"
#

DROP TABLE IF EXISTS `t_group_title`;
CREATE TABLE `t_group_title` (
  `gt_sn` decimal(12,0) NOT NULL,
  `inf_sn` decimal(12,0) NOT NULL,
  `cpuno` decimal(3,0) NOT NULL,
  `groupno` decimal(5,0) NOT NULL,
  `name` varchar(16) NOT NULL,
  `type` decimal(2,0) NOT NULL,
  UNIQUE KEY `pk_grouptitle` (`gt_sn`),
  KEY `idx_gt_inf_sn` (`inf_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_group_val"
#

DROP TABLE IF EXISTS `t_group_val`;
CREATE TABLE `t_group_val` (
  `ied_sn` decimal(12,0) NOT NULL,
  `cpuno` decimal(3,0) NOT NULL,
  `sector` decimal(3,0) NOT NULL,
  `groupno` decimal(5,0) NOT NULL,
  `entry` decimal(5,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `deviceno` decimal(5,0) NOT NULL,
  `currval` varchar(32) NOT NULL,
  `referval` varchar(32) NOT NULL,
  UNIQUE KEY `pk_groupval` (`ied_sn`,`cpuno`,`sector`,`groupno`,`entry`),
  KEY `idx_groupval_subdev` (`sub_sn`,`deviceno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_gt_group"
#

DROP TABLE IF EXISTS `t_gt_group`;
CREATE TABLE `t_gt_group` (
  `gt_sn` decimal(12,0) NOT NULL,
  `entry` decimal(5,0) NOT NULL,
  `name` varchar(32) NOT NULL,
  `valtype` decimal(1,0) NOT NULL,
  `dime` varchar(8) DEFAULT NULL,
  `maxval` decimal(18,2) DEFAULT NULL,
  `minval` decimal(18,2) DEFAULT NULL,
  `stepval` decimal(18,4) DEFAULT NULL,
  `precision_n` decimal(3,0) DEFAULT NULL,
  `precision_m` decimal(3,0) DEFAULT NULL,
  `mmspath` varchar(128) DEFAULT NULL,
  UNIQUE KEY `pk_gt_group` (`gt_sn`,`entry`),
  KEY `idx_gt_group_gtsn` (`gt_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_gt_state"
#

DROP TABLE IF EXISTS `t_gt_state`;
CREATE TABLE `t_gt_state` (
  `gt_sn` decimal(12,0) NOT NULL,
  `fun` decimal(5,0) NOT NULL,
  `inf` decimal(5,0) NOT NULL,
  `entry` decimal(5,0) NOT NULL,
  `name` varchar(32) NOT NULL,
  `type` decimal(1,0) NOT NULL,
  `evt_cls` decimal(2,0) NOT NULL,
  `val_type` decimal(2,0) NOT NULL,
  `on_dsc` varchar(8) DEFAULT NULL,
  `off_dsc` varchar(8) DEFAULT NULL,
  `complex_type` decimal(1,0) NOT NULL,
  `mmspath` varchar(128) DEFAULT NULL,
  UNIQUE KEY `pk_gt_state` (`gt_sn`,`fun`,`inf`),
  KEY `idx_gt_state_gtsn` (`gt_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_his_event"
#

DROP TABLE IF EXISTS `t_his_event`;
CREATE TABLE `t_his_event` (
  `evt_seq` decimal(18,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `deviceno` decimal(5,0) NOT NULL,
  `cpuno` decimal(3,0) NOT NULL,
  `act_soc` int(11) NOT NULL,
  `act_usec` int(11) NOT NULL,
  `fun` decimal(5,0) NOT NULL,
  `inf` decimal(5,0) NOT NULL,
  `type` decimal(1,0) NOT NULL,
  `rettime` decimal(5,0) NOT NULL,
  `dpi` decimal(1,0) NOT NULL,
  `nof` decimal(8,0) DEFAULT NULL,
  `fan` decimal(8,0) DEFAULT NULL,
  `act_q` int(11) DEFAULT NULL,
  `time_q` int(11) DEFAULT NULL,
  UNIQUE KEY `pk_his_event` (`evt_seq`),
  KEY `idx_his_event_soc` (`act_soc`),
  KEY `idx_his_event_sub_dev` (`sub_sn`,`deviceno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_his_faultinfo"
#

DROP TABLE IF EXISTS `t_his_faultinfo`;
CREATE TABLE `t_his_faultinfo` (
  `fi_seq` decimal(18,0) NOT NULL,
  `evt_seq` decimal(18,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `deviceno` decimal(5,0) NOT NULL,
  `cpuno` decimal(3,0) NOT NULL,
  `act_soc` int(11) NOT NULL,
  `act_usec` int(11) NOT NULL,
  `groupno` decimal(5,0) NOT NULL,
  `entry` decimal(5,0) NOT NULL,
  `type` decimal(1,0) NOT NULL,
  `val` varchar(32) NOT NULL,
  UNIQUE KEY `pk_his_faultinfo` (`fi_seq`),
  KEY `idx_his_faultinfo_evt_seq` (`evt_seq`),
  KEY `idx_his_faultinfo_soc` (`act_soc`),
  KEY `idx_his_faultinfo_sub_dev` (`sub_sn`,`deviceno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_his_wave"
#

DROP TABLE IF EXISTS `t_his_wave`;
CREATE TABLE `t_his_wave` (
  `wave_seq` decimal(18,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `deviceno` decimal(5,0) NOT NULL,
  `cpuno` decimal(3,0) NOT NULL,
  `act_soc` int(11) NOT NULL,
  `act_usec` int(11) NOT NULL,
  `filename` varchar(128) NOT NULL,
  `files` decimal(2,0) NOT NULL,
  `cfg_bytes` decimal(12,0) DEFAULT NULL,
  `dat_bytes` decimal(12,0) DEFAULT NULL,
  `hdr_bytes` decimal(12,0) DEFAULT NULL,
  `ext_bytes` decimal(12,0) DEFAULT NULL,
  UNIQUE KEY `pk_his_wave` (`wave_seq`),
  KEY `idx_his_wave_soc` (`act_soc`),
  KEY `idx_his_wave_sub_dev` (`sub_sn`,`deviceno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_ied"
#

DROP TABLE IF EXISTS `t_ied`;
CREATE TABLE `t_ied` (
  `ied_sn` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `deviceno` decimal(5,0) NOT NULL,
  `name` varchar(32) NOT NULL,
  `ied_type` decimal(3,0) NOT NULL,
  `pri_code` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `dev_type` varchar(16) DEFAULT NULL,
  `inf_sn` decimal(12,0) DEFAULT NULL,
  `dev_ver` varchar(32) DEFAULT NULL,
  `commstate` decimal(1,0) DEFAULT '0',
  UNIQUE KEY `pk_ied_` (`ied_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_ied_accout"
#

DROP TABLE IF EXISTS `t_ied_accout`;
CREATE TABLE `t_ied_accout` (
  `ied_sn` int(11) NOT NULL AUTO_INCREMENT,
  `vender` varchar(32) DEFAULT NULL,
  `soft_ver` varchar(32) DEFAULT NULL,
  `comm_type` varchar(16) DEFAULT NULL,
  `comm_prot` varchar(32) DEFAULT NULL,
  `comm_param` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`ied_sn`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

#
# Source for table "t_ied_cpu"
#

DROP TABLE IF EXISTS `t_ied_cpu`;
CREATE TABLE `t_ied_cpu` (
  `ied_sn` decimal(12,0) NOT NULL,
  `cpuno` decimal(3,0) NOT NULL,
  `name` varchar(16) NOT NULL,
  `ver` varchar(32) DEFAULT NULL,
  UNIQUE KEY `pk_ied_cpu` (`ied_sn`,`cpuno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_infotab"
#

DROP TABLE IF EXISTS `t_infotab`;
CREATE TABLE `t_infotab` (
  `inf_sn` decimal(12,0) NOT NULL,
  `name` varchar(64) NOT NULL,
  `ver` varchar(32) DEFAULT NULL,
  `author` varchar(16) DEFAULT NULL,
  `type` decimal(1,0) DEFAULT '0',
  `manufacturer` varchar(16) DEFAULT NULL,
  `in_time` int(11) DEFAULT NULL,
  `file_size` int(11) DEFAULT NULL,
  `file` blob,
  UNIQUE KEY `pk_inftab` (`inf_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_infotab_cpu"
#

DROP TABLE IF EXISTS `t_infotab_cpu`;
CREATE TABLE `t_infotab_cpu` (
  `inf_sn` decimal(12,0) NOT NULL,
  `cpuno` decimal(3,0) NOT NULL,
  `name` varchar(16) NOT NULL,
  `ver` varchar(32) DEFAULT NULL,
  UNIQUE KEY `pk_infotab_cpu` (`inf_sn`,`cpuno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_infotab_param"
#

DROP TABLE IF EXISTS `t_infotab_param`;
CREATE TABLE `t_infotab_param` (
  `inf_sn` decimal(12,0) NOT NULL,
  `comm_type` varchar(16) NOT NULL,
  `param_name` varchar(32) NOT NULL,
  `param_type` decimal(1,0) NOT NULL,
  `dsc` varchar(256) DEFAULT NULL,
  `defval` varchar(128) DEFAULT NULL,
  UNIQUE KEY `pk_infotab_param` (`inf_sn`,`comm_type`,`param_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_pie_test"
#

DROP TABLE IF EXISTS `t_pie_test`;
CREATE TABLE `t_pie_test` (
  `name` varchar(32) NOT NULL DEFAULT '',
  `value1` float DEFAULT NULL,
  `value2` float DEFAULT NULL,
  `value3` float DEFAULT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_reactance"
#

DROP TABLE IF EXISTS `t_reactance`;
CREATE TABLE `t_reactance` (
  `rct_sn` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `code` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(16) DEFAULT NULL,
  `state` decimal(1,0) DEFAULT '1',
  `vlevel` decimal(5,0) DEFAULT NULL,
  `capacity` decimal(12,4) DEFAULT NULL,
  UNIQUE KEY `pk_rct` (`rct_sn`),
  KEY `idx_reactance_sub_sn` (`sub_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_setup"
#

DROP TABLE IF EXISTS `t_setup`;
CREATE TABLE `t_setup` (
  `set_key` varchar(32) NOT NULL,
  `type_id` decimal(5,0) DEFAULT NULL,
  `set_desc` varchar(64) NOT NULL,
  `set_default` varchar(128) DEFAULT NULL,
  `set_value` varchar(128) DEFAULT NULL,
  `set_dime` varchar(8) DEFAULT NULL,
  `set_class` decimal(1,0) NOT NULL,
  `val_type` decimal(1,0) NOT NULL,
  `val_fmt` varchar(256) DEFAULT NULL,
  UNIQUE KEY `pk_setup` (`set_key`),
  KEY `idx_setup_type` (`type_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_setup_type"
#

DROP TABLE IF EXISTS `t_setup_type`;
CREATE TABLE `t_setup_type` (
  `type_id` decimal(5,0) NOT NULL,
  `type_name` varchar(32) NOT NULL,
  UNIQUE KEY `pk_setup_type` (`type_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_state_action"
#

DROP TABLE IF EXISTS `t_state_action`;
CREATE TABLE `t_state_action` (
  `gt_sn` decimal(12,0) NOT NULL,
  `fun` decimal(5,0) NOT NULL,
  `inf` decimal(5,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `deviceno` decimal(5,0) NOT NULL,
  `readonly` decimal(1,0) NOT NULL,
  `on_trg_action` decimal(2,0) DEFAULT NULL,
  `off_trg_action` decimal(2,0) DEFAULT NULL,
  `trg_object` decimal(3,0) DEFAULT NULL,
  UNIQUE KEY `pk_stateaction` (`gt_sn`,`fun`,`inf`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_state_val"
#

DROP TABLE IF EXISTS `t_state_val`;
CREATE TABLE `t_state_val` (
  `ied_sn` decimal(12,0) NOT NULL,
  `cpuno` decimal(3,0) NOT NULL,
  `fun` decimal(5,0) NOT NULL,
  `inf` decimal(5,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `deviceno` decimal(5,0) NOT NULL,
  `curval` decimal(1,0) NOT NULL,
  UNIQUE KEY `pk_stateval` (`ied_sn`,`cpuno`,`fun`,`inf`),
  KEY `idx_stateval_subdev` (`sub_sn`,`deviceno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_substation"
#

DROP TABLE IF EXISTS `t_substation`;
CREATE TABLE `t_substation` (
  `sub_sn` decimal(5,0) NOT NULL,
  `cmp_sn` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(32) DEFAULT NULL,
  `address` varchar(32) DEFAULT NULL,
  `state` decimal(1,0) DEFAULT '1',
  `comstate` decimal(1,0) DEFAULT '0',
  `vlevel` decimal(5,0) DEFAULT NULL,
  `join_time` int(11) DEFAULT NULL,
  `ip_addr_a` varchar(24) DEFAULT NULL,
  `ip_addr_b` varchar(24) DEFAULT NULL,
  `svr_port` decimal(8,0) DEFAULT NULL,
  `svg_file` blob,
  UNIQUE KEY `pk_sub` (`sub_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_syslog"
#

DROP TABLE IF EXISTS `t_syslog`;
CREATE TABLE `t_syslog` (
  `log_seq` decimal(18,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `usr_sn` decimal(8,0) NOT NULL,
  `deviceno` decimal(5,0) NOT NULL,
  `log_type` decimal(1,0) NOT NULL,
  `log_val` decimal(12,2) DEFAULT NULL,
  `log_text` varchar(256) DEFAULT NULL,
  UNIQUE KEY `pk_syslog` (`log_seq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_transformer"
#

DROP TABLE IF EXISTS `t_transformer`;
CREATE TABLE `t_transformer` (
  `tsf_sn` decimal(12,0) NOT NULL,
  `sub_sn` decimal(5,0) NOT NULL,
  `code` decimal(5,0) NOT NULL,
  `uri` varchar(32) DEFAULT NULL,
  `name` varchar(16) DEFAULT NULL,
  `state` decimal(1,0) DEFAULT '1',
  `type` decimal(1,0) DEFAULT '0',
  `vlevel` decimal(5,0) DEFAULT NULL,
  `vlevel_h` decimal(5,0) DEFAULT NULL,
  `vlevel_m` decimal(5,0) DEFAULT NULL,
  `vlevel_l` decimal(5,0) DEFAULT NULL,
  `capacity` decimal(12,4) DEFAULT NULL,
  UNIQUE KEY `pk_tsf` (`tsf_sn`),
  KEY `idx_transf_sub_sn` (`sub_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_uicfg_wnd"
#

DROP TABLE IF EXISTS `t_uicfg_wnd`;
CREATE TABLE `t_uicfg_wnd` (
  `wnd_sn` decimal(8,0) NOT NULL DEFAULT '0',
  `wnd_name` varchar(32) DEFAULT NULL,
  `create_author` varchar(16) DEFAULT NULL,
  `create_time` int(4) DEFAULT NULL,
  `modify_author` varchar(16) DEFAULT NULL,
  `modify_time` int(4) DEFAULT NULL,
  `wnd_type` decimal(1,0) DEFAULT NULL,
  `svg_file` blob,
  `dataset` varchar(32) DEFAULT NULL,
  `ds_cond` varchar(128) DEFAULT NULL,
  UNIQUE KEY `pk_uicfg_wnd` (`wnd_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_user"
#

DROP TABLE IF EXISTS `t_user`;
CREATE TABLE `t_user` (
  `usr_sn` decimal(8,0) NOT NULL DEFAULT '0',
  `usr_code` varchar(16) NOT NULL DEFAULT '',
  `grp_code` varchar(16) DEFAULT NULL,
  `name` varchar(32) DEFAULT NULL,
  `pwd` varchar(32) DEFAULT NULL,
  `dsc` varchar(128) DEFAULT NULL,
  `email` varchar(64) DEFAULT NULL,
  `mobile` varchar(16) DEFAULT NULL,
  `create_time` int(4) DEFAULT NULL,
  `login_timeout` int(4) DEFAULT NULL,
  PRIMARY KEY (`usr_sn`),
  UNIQUE KEY `usr_code` (`usr_code`),
  KEY `grp_code` (`grp_code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_user_auth"
#

DROP TABLE IF EXISTS `t_user_auth`;
CREATE TABLE `t_user_auth` (
  `usr_sn` decimal(8,0) NOT NULL DEFAULT '0',
  `fun_key` varchar(16) NOT NULL DEFAULT '',
  `auth` decimal(1,0) DEFAULT NULL,
  PRIMARY KEY (`usr_sn`,`fun_key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_user_group"
#

DROP TABLE IF EXISTS `t_user_group`;
CREATE TABLE `t_user_group` (
  `grp_code` varchar(16) NOT NULL DEFAULT '',
  `name` varchar(32) DEFAULT NULL,
  `dsc` varchar(128) DEFAULT NULL,
  `login_timeout` int(4) DEFAULT NULL,
  PRIMARY KEY (`grp_code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Source for table "t_usergroup_auth"
#

DROP TABLE IF EXISTS `t_usergroup_auth`;
CREATE TABLE `t_usergroup_auth` (
  `grp_code` varchar(16) NOT NULL DEFAULT '',
  `fun_key` varchar(16) NOT NULL DEFAULT '',
  `auth` decimal(1,0) DEFAULT NULL,
  PRIMARY KEY (`grp_code`,`fun_key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;