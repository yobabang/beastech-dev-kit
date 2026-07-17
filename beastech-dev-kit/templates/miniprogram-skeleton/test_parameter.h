#pragma once
#include <vector>

//2 - 只能按照单键连接好测试
//12.4-后面需要配置每个CAN总线的CAN/CANFD
char* cfg_path;
char ini_path[1024] = "";
//用于唤醒的NM报文，其他的诊断报文和APP报文
TCANFD nm_msg;
TCANFD nm_msg1;
TCANFD app_msg;
TCANFD diag_msg;
s64 current_time;
char current_date[1024];
//用PCANFD会出错，运行失败；用引用也不可以
const u8 can_bus_count_max = 20;
enum test_report_type { html, word };
bool TestReportType = html;

 u8 bus_count = 0;//网段个数=样件test can通道+can1继电器，自动获取ini中all_test_channel个数
const u8 pnc_max_count_in_one_bus = 32;//单个通道最大的pnc数量,目前geely是4个byte
 u8 support_pnc_can_count = 2;//使用PNC的CAN通道，改成自动获取非non_pnc_gateway的个数

//ECU为终端（非PNC路由）ECU/网关（有PNC路由）EUC/被动节点（不会外发NM报文，不会进NOS）
enum ECU_gateway_type { non_pnc_gateway,pnc_gateway,passive_node};
enum ECU_gateway_type ecu_gateway_type = pnc_gateway;//ecu_type
//const bool ComMPncGatewayEnabled = ecu_gateway_type;//为true时，PNC gateway
char ECU_name[1024] = "PleaseInputIniFile";//会拿ini里的
s32 ECU_address = 0x1D00;//会拿ini里的
bool if_powered_by_KL30;//是否KL30供电，即30on后就开始通信
bool if_have_low_power_mode = 1;//是否有低功耗模式，是否能进入低功耗，在面板中
bool if_ned_extra_condition_to_go_to_low_power = 1;//是否需要额外条件才能进入低功耗
u16 extra_wait_time_to_enter_low_power = 21000;
bool if_transvier_support_PN = 1;//是否DUT使用支持PN的收发器
bool if_connect_OBD_II = 0;//是否直连OBD
bool if_VFC_ned_the_network_awake = 0;
bool if_use_disturb_device = 1;

u8 disturb_chl = 1;//1=can2
u8 diag_chl = 1;//暂时么用到
u16 WakeupCoding = 0x0105;

u16 stress_test_1088101_count = 50;
u16 stress_test_1088179_count = 50;
//PNC路由分为：没有PNC路由，passive路由，active路由
enum PNC_gateway_type { no_pnc_gateway, passive_pnc_gateway, active_pnc_gateway };
enum All_wakeup_way {network,hardware1,hardware2, diag,other};
const char* hardware_wakeup_way_s[] = { "hardware1", "hardware2" };
const char* all_wakeup_way_s[] = { "network", "hardware1","hardware2","diag","other"};

const u8 pnc_begin_byte = 2;
const u8 pnc_end_byte = 5;
const u8 NoMapFramesCountMax = 255;
const u8 AllPNCMapFramesCountMax = 255;
//enum Hardware_wakeup_type {hardware1,hardware2};
struct BUS_configuration
{
	bool if_support_network_wakeup ;//是否支持NM唤醒
	u16 dut_send_nm_msg_id;
	bool if_support_hardware_wakeup ;//是否支持硬线唤醒
	bool if_sleep_state_with_power_supply_on ;//如果上电后是休眠状态 != if_can_be_sleep_with_power
	bool if_support_VFC;//是否关联了VFC,支持VFC前提必须支持硬线唤醒，因为现在激活VFC就是用硬线 
	bool if_support_I_PDU_control;//是否支持IPDU控制,这些参数还需要加ini读取
	bool if_recv_diag_msg=1;//是否是诊断CAN
	bool CanNmPnEnabled;
	bool if_keep_waking_hardware;//如不能维持唤醒的话脚本针对硬线唤醒源类的用例需要维持反复唤醒硬线唤醒源用来维持住唤醒状态。
	enum  PNC_gateway_type bus_pnc_gateway_type;//当前通道PNC类型
	u8 relevant_pnc[pnc_max_count_in_one_bus];//pnc相关的bit位，geely为PNC29,PNC25（均在NM PDU的byte3中）,从小到大排序
	u8 rx_pnc[pnc_max_count_in_one_bus];
	//vector<u8> relevant_pnc;
	u8 wakeup_way_count = 2;//所有唤醒方式数量
	enum All_wakeup_way all_wakeup_way[8];//所有唤醒方式

	u8 hardware_wakeup_count;//硬线唤醒数量
	enum All_wakeup_way hardware_wakeup_way[8];//硬线唤醒方式

	bool if_have_sleep_state;//是否有睡眠状态
	u32 if_have_sleep_state_enter_sleep_wait_time = (u32)gSleepTime;//进入睡眠状态要等待的时间

	u8 pnc2bytembitn[8][8] =
	{
		{0,0,0,0,0,0,0,0},
		{0},
		{0},
		{0},
		{0},
		{0},
		{0},
		{0}
	};//根据relvant pnc调用handle函数自动转成byte位bit
	
	u16 rx_msg[1024] = {0x1};//DUT可以接受的报文,case中用到第一个，从ini里拿，默认是0x1
	//vector<u16> rx_msg = {};
	u16 tx_msg[1024];//DUT发送的报文，暂时用不到
	u8 OnlySourcePNC[pnc_max_count_in_one_bus]={0};//对于CAN bus_count来说的OnlySourcePNC，目前就1个
	u8 DesPNC[pnc_max_count_in_one_bus] = {0};
	std::vector<u16> pnc_MapFramesTx[64] = {};//吉利只用了PNC16-40多
	std::vector<u16> pnc_MapFramesTx_Event[64] = {};
	std::vector<u16> NoMapFrames = {};
	std::vector<u16> NoMapFrames_Event = {};
	char node_name[1024] = "";

	bool if_can_be_sleep_with_power = 0;//上电后可以进入睡眠，可能有条件
	bool if_can_be_keep_requesting_network_when_bus_is_unavailable = 0;//是否在总线不可用时能维持网络请求
	bool if_can_request_net_and_can_release_net_when_bus_is_unavailable = 0;//是否【网段能请求网络，并在总线不可用时能释放网络】
	bool if_ned_extra_condition_to_go_to_sleep = 1;//是否需要额外条件才能进入睡眠

}bus_configuration[can_bus_count_max] =
{
	//{},
	////{ 1,0,1,0,1,1,1,no_pnc_gateway,{25,34,30,31,26},2,{network,hardware1,hardware2},2,{hardware1,hardware2},1,2000,{0},1 }//假设CAN2为PropulsionCANFD_TMCU，关联pnc29和25
	//{ 1,0x505,1,1,1,0,1,1,1,no_pnc_gateway,{29},2,{network,hardware1,hardware2},2,{hardware1,hardware2},1,2000,{0},1 },//假设CAN2为PropulsionCANFD_TMCU，关联pnc29和25
	//{ 1,0x504,1,1,0,1,1,1,1,active_pnc_gateway,{25,29,36},2,{network,hardware1,hardware2},2,{hardware1,hardware2},1,2000,{0},1 }//假设CAN2为PropulsionCANFD_TMCU，关联pnc29和25
//{1,0,0,passive_pnc_gateway,{16,25},1,{network},0,{},0,2000,{0},0},
	//{1,0,0,no_pnc_gateway,{25},1,{network},0,{},0,2000,{0},0}
};
struct _PNC_Gateway_Type {
	enum  PNC_gateway_type bus_pnc_gateway_type;
	std::string bus_pnc_gateway_type_name;
};
_PNC_Gateway_Type NM_Config[can_bus_count_max];


//u16 pnc_MapFramesRx[64][10] = {};



//const u16 pnc25_MapFramesRx[3] = {0x233,0x239,0x279};
//const u16 pnc25_MapFramesTx[8] = { 0x2A0,0x2A3,0x2A6,0x2A9,0x2AC,0x365,0x370,0x421 };
//
//const u16 pnc29_MapFramesRx[4] = { 0x76,0x233,0x239,0x279 };
//const u16 pnc29_MapFramesTx[8] = { 0x2A0,0x2A3,0x2A6,0x2A9,0x2AC,0x365,0x370,0x421 };
//
////还缺VDDMPropSignalIPDU06，VddmPropSignalIPdu26
//const u16 NoMapFramesRx[6] = {0x151,0x56,0x162,0x285};




//定义总线类型
enum bus_type { bus_type_can, bus_type_canfd, bus_type_lin, bus_type_eth };
const bus_type nm_bus_type = bus_type_can;
const bus_type app_bus_type = bus_type_canfd;
const bus_type diag_bus_type = bus_type_canfd;


//用于模拟发送报文时，选择报文类型：常规周期、单帧、快发周期
enum MsgSendType { normol, frame, fast };


//特殊通道，用于防止错误NM报文导致的错误帧
//const u8 recv_error_special_chl = 2;
const u32 CanStopCommunicate = 20000;//20000
const u16 CanNmPnResetTime = 2.95*1000; //GEEA3.0/GEEA3.5 2.95s
const u16 CanNmWaitBusSleepTime = 4*1000;//默认4000
const u16 CanNmRepeatMessageTime = 1 * 1000;
const u16 CanNmMsgCycleTime = 1 * 1000;
const u16 CanNmImmediateNmTransmissions = 20;
const u16 CanNmImmediateNmCycleTime = 20;
const u16 CanNmTimeoutTime = 4000;//默认4000
const u16 CanNmMsgCycleOffset = 100;
const u16 CanNmImmediateNmCycleTimeMax = CanNmImmediateNmCycleTime;
const u16 CanNmImmediateNmCycleTimeMin = 0;
const u16 CanNmMsgCycleTimeMax = CanNmMsgCycleTime+ CanNmMsgCycleOffset;
const u16 CanNmMsgCycleTimeMin = CanNmMsgCycleTime- CanNmMsgCycleOffset;
const u16 CanNmPnResetTimeMax = 1.1 * CanNmPnResetTime;
const u16 CanNmPnResetTimeMin = 0.9 * CanNmPnResetTime;
const u16 CanNmMsgTimeoutTime = 500;
//以下为busoff设置
u16 CanSMBorTimeL1 = 10;
u16 CanSMBorCounterL1ToL2 = 32;
u16 CanSMBorTimeL2 = 60000;


const u16 T_BusSleep = CanNmTimeoutTime+ CanNmWaitBusSleepTime;
const u16 T_LowPower = 105;//从最后一个NM PDU开始到DUT进入低功耗的时间（如果有低功耗才适用）
const u16 Twake_network = 100;
const u16 Twake_hardware = 100;
const u16 TCANWakeAndGwPncRequest = 15;
const u16 TCANGwPncRequest = 15;
const u16 TCANEventWakeUpToApp = 50;
const u16 TCANPowerWakeUpToApp = 150;//If ECU has security bootloader requirement,TCANPowerWakeUpToApp can be up to 250ms.
double low_power_current_max = 0.01;//10ma

//DUT send NM msg
//const u16 nm_dut_send_id = 0x517;//0x45E
const u16 nm_dut_send_id_min = 0x500;
const u16 nm_dut_send_id_max = 0x53F;
const u8 nm_dut_send_dlc = 8;
const u8 nm_dut_send_length = 8;
//const u8 nm_dut_send_data_nm[nm_dut_send_length] = { 0x17,0x40,0x00,0x00,0x00,0x00,0x00,0x00 };
//const u8 nm_dut_send_data_kl15[nm_dut_send_length] = { 0x17,0x40,0x00,0x00,0x00,0x02,0x00,0x00 };
const u8 nm_dut_send_fast_times = CanNmImmediateNmTransmissions;//15
const u16 nm_dut_send_nor_period = CanNmMsgCycleTime;
const u8 nm_dut_send_fast_period = CanNmImmediateNmCycleTime;
const u16 nm_dut_send_fast_periodMax = nm_dut_send_fast_period * (1 + 0.1);
const u16 nm_dut_send_nor_periodMax = nm_dut_send_nor_period * (1 + 0.05);
const u16 nm_dut_send_fast_periodMin = nm_dut_send_fast_period * (1 - 0.1);
const u16 nm_dut_send_nor_periodMin = nm_dut_send_nor_period * (1 - 0.05);

//仿真发送的报文配置
//const u8 nm_dlc = 8;
//const u8 nm_length = 8;
const u16 nm_nor_period = CanNmMsgCycleTime;
//const u8 nm_fast_period = nm_dut_send_fast_period;
//const u8 nm_fast_times = 5;
//const u8 nm_msg_chl = 1;
//const u16 nm_id = 0x516;
//const u8 nm_data[nm_length] = { 0x16,0x40,0x00,0x00,0x00,0x02,0x00,0x00 };
//const u8 nm_rms_data[nm_length] = {};//设置RMS为1的NM报文


//诊断报文
 u16 fun_id = 0x7FF;
 u16 rsp_id = 0x601;
 u16 phy_id = 0x701;

 u8 SeLevel = 0;
  //u16 rsp_id = ECU_address & 0XFFF;
 //u16 phy_id = rsp_id + 0x100;

const u8 diag_dlc = 8;
const u8 diag_data_length = 8;//data length != dlc
const u8 diag_msg_data[diag_dlc] = { 0 };
//const u8 diag_chl = 1;
const u8 diag_msg_normol_period = 50;

u8 send_1001[diag_data_length] = { 0x2,0x10,0x1 };
u8 send_1002[diag_data_length] = { 0x2,0x10,0x2 };
u8 send_1003[diag_data_length] = {0x2,0x10,0x3};
u8 send_1081[diag_data_length] = { 0x2,0x10,0x81 };
u8 send_22F186[diag_data_length] = {0x3,0x22,0xF1,0x86};
u8 send_1101[diag_data_length] = { 0x2,0x11,0x1 };
u8 send_3E00[8] = { 0x2,0x3E,0x00 };//3E 80会禁止肯定响应
u8 send_usgmod[diag_data_length] = { 0x05,0x2F,0xDD,0x0A,0x03,0x02 };

u8 send_ProDID[diag_data_length] = { 0x3,0x22,0xD2,0x7A };
u8 send_ConDID[diag_data_length] = { 0x3,0x22,0xD2,0x7B };
u8 send_ZCUFDDID[diag_data_length] = { 0x3,0x22,0xD2,0x7C };
u8 send_ChassDID[diag_data_length] = { 0x3,0x22,0xD2,0x7D };
u8 send_ZCUDCANDID[diag_data_length] = { 0x3,0x22,0xD2,0x7E };
u8 send_SafeADDID[diag_data_length] = { 0x3,0x22,0xD2,0x7F };

u16 p2_server = 1000;

//const double Threshold_normol = 0.01;

//RX报文组
std::vector<TCANFD>ALLRxMsg[can_bus_count_max];//动态报文组
std::vector<TCANFD>OutErrorAndDiagRxMsg[can_bus_count_max];
std::vector<TCANFD>DUT_NMRxMsg[can_bus_count_max];
std::vector<TCANFD>DUT_APPRxMsg[can_bus_count_max];
std::vector<TCANFD>ErrorRxMsg[can_bus_count_max];
std::vector<TCANFD>DiagRxMsg[can_bus_count_max];

//TX报文组
std::vector<TCANFD>ALLTxMsg[can_bus_count_max];//动态报文组
std::vector<TCANFD>OutErrorTxMsg[can_bus_count_max];
std::vector<TCANFD>DUT_NMTxMsg[can_bus_count_max];
std::vector<TCANFD>DUT_APPTxMsg[can_bus_count_max];
std::vector<TCANFD>ErrorTxMsg[can_bus_count_max];
std::vector<TCANFD>DiagTxMsg[can_bus_count_max];
std::vector<TCANFD>DUT_NMTxMsg_nm_msg1[can_bus_count_max];//只有用nm_id_max的才行



struct _openissue_cfg
{
	//"序号,问题分类,测试案例ID,标题,提出时间,问题状态,问题描述,问题严重度,平台,ECU名称,软件版本号,供应商,测试环境,测试部门,附件,责任方,计划完成时间,对策/答案,实际完成时间,测试阶段,问题关闭阶段,测试人员,审核人员,备注");
	//u16 number=0;
	char problem_classification[1024];
	char test_id[1024];
	char title[1024];
	char put_time[1024];//
	char problem_state[10] = "open";
	char problem_description[1024];
	char problem_severity[1024];
	char platform[10]="GEEA3.0";
	char ecu_name[10]="TMCU";
	char svn[1024];
	char supplier[10]="TOSUN";
	char test_envir[10]="BENCH";
	char test_department[10]="EE";
	char attachment[1024];//附件？
	char responsible_party[1024];
	char plan_to_finish_time[1024];
	char solution[1024];
	//实际完成时间,测试阶段,问题关闭阶段,测试人员,审核人员,备注
	char actural_finished_time[1024];
	char test_period[10]="PP";
	char problem_close_period[1024];
	char test_engineer[1024]="shanqingqing";
	char check_engineer[1024];
	char remark[1024];

}openissue_cfg;

u8 current_test_result = 0;
char current_step[1024];
//s64 the_last_msg_time = 

struct NodeDiagMapping {
	const char* node_name;
	u8* diag_data;
};

const NodeDiagMapping NODE_DIAG_MAPPINGS[] = {
	{"ZCU_CANFD1", send_ZCUFDDID},
	{"ZCU_CANFD2", send_ZCUFDDID},
	{"PropulsionCANFD", send_ProDID},
	{"ConnectivityCANFD", send_ConDID},
	{"ChassisCANFD1", send_ChassDID},
	{"ChassisCANFD2", send_ChassDID},
	{"ZCUD_CAN1", send_ZCUDCANDID},
	{"SafeCANFD1", send_SafeADDID},
	{"SafeCANFD2", send_SafeADDID},
	{"SafeCANFD3", send_SafeADDID},
	{"SafeCANFD4", send_SafeADDID},
	{"SafeCANFD5", send_SafeADDID},
	{"ADCAN", send_SafeADDID}
};



//struct e2eSignal
//{
//	char sigName[100];
//	u8 sigLength;
//	u8 startBit;
//	u8 sigValue;////////////////
//};



