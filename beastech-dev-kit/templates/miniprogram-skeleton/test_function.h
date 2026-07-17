#ifndef TEST_FUNCTION
#define TEST_FUNCTION
//诊断所在通道
#define DiagCH CH2
//NM所在通道
#define NMCH CH3

//#include "test_common.h"
#include "TestReport.h"
#include"test_devices.h"
#include "test_parameter.h"
#include<test_case_diag_api.h>
#include <chrono>  // 必须包含此头文件
#include <set>

using namespace std;
#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* 获取x第bit位,第0位:GET_BIT(a, 0) */
#define	SET_BIT(x, bit)	(x |= (1 << bit))	/* 置位x的第bit位为1 */
#define	CLEAR_BIT(x, bit)	(x &= ~(1 << bit))	/* 清零第bit位 */

#define C_DiagDataLenMAX 65535


//创建诊断连接，用以发送/接收诊断数据，主要用于case中读取DTC部分
//int udsHandle = -1;
//诊断响应最大长度
int g_DiagDataLenMAX = 65535;
//int DiagDataLenMAX = 65535;
//用以存储诊断响应数据
u8 DiagResData[C_DiagDataLenMAX];


/*------------------------NM-------------------------------*/


/*------------------------E2E-------------------------------*/

//E2E解析的Python文件绝对路径
char E2EPythonPath[1024] = "";
//E2E解析的Python文件相对路径
char pythonpath[100] = "Toolbox\\GEELY3\\DB\\ParsingARXML.py";//Toolbox\\GEELY3\\
//执行Python的参数，暂无作用
char* pythonResult = nullptr;
//E2E生成的ini文件绝对路径
char E2EINIPath[1024] = "";
//E2E生成的ini文件相对路径
char E2E_ini_path[100] = "Toolbox\\GEELY3\\DB\\E2EInfo.ini";//Toolbox\\GEELY3\\


struct E2EParametersINI
{
    std::string NMBusType;
    std::string DiagBusType;
    std::string DUTWakeUpType;
    std::string NetworkName;
    u32 NMID;
    int NMDLC = 8;
    long NMPeriod;
    std::vector<u8>NMData;
    std::string TestECUName;
    u32 DiagReqID;
    u32 DiagResID;
    u32 DiagFuncID;
};
struct E2EParametersINI g_parameter;

//要等待报文的ID，在函数WaitMessage中赋值，在接收事件中，对Flag赋值
s32 WaitMessage_ID = 0x0;
s32 WaitMessage_TX_ID = 0x0;

//VehMod报文对应ID，在FulfilTRC中赋值，在PreTX预发送中检测
u32 VehModID = 0;
int VehModCH = -1;
//数据库属性

typedef struct _signalinfo
{
    std::vector<TDBSignalProperties>sBuffer;
    int channel;
}Tsignalinfo, * Psignalinfo;
std::vector<Tsignalinfo>signalInfo;

char frameAddress[1024];

//当前测试数据库网络名称
std::string NetworkName;

//当前用例测试的报文的ID，在具体用例中进行赋值
u32 TestMsgID = 0;
int TestCH = CH2;
std::string TestSGName = "";
typedef std::vector<u8>CRCdata;


//定义信号，信号组，信号组关联的Msgid的内容
typedef struct _Single
{
    bool isIntel;
    s32 startbit;
    s32 Len;
    int RearrByteLen;
    int RearrStartByte;
    std::string SName;
    std::string SName_low;
}TSingle, * PSingle;

//信号组结构体
typedef struct _SingleGroup
{
    u16 DataID;
    u8 DLC;
    u32 msgid;
    TSignalType bustype;
    std::string SGName;
    std::vector<TSingle>SGElement;
    std::vector<u8>RearrData;
    u8 Chks;
    u8 Cntr;
    u32 CntrDTC;
    u32 ChksDTC;
    int CntrConfirmedDTCLimit;
    int ChksConfirmedDTCLimit;
    int CntrStepUpValue;
    int CntrTestPeriod;
    int CntrMaxValue;
    int ChksStepUpValue;
    int ChksTestPeriod;
    int ChksMaxValue;

    std::string RelativeSignalCondition;
    std::string RelativeSignalName1;
    u32 RelativeSignalValue1;
    std::string RelativeSignalName2;
    u32 RelativeSignalValue2;
    std::string RelativeSignalName3;
    u32 RelativeSignalValue3;
    std::string RelativeSignalName4;
    u32 RelativeSignalValue4;
    std::string RelativeSignalName5;
    u32 RelativeSignalValue5;

    double PeriodMs;//单位为ms，解析INI时将s转为ms
    int UBPosition;
    int channel;
}TSingleGroup, * PSingleGroup;


//E2E报文结构体
typedef struct _ARXMLE2ESG
{
    TSignalType BusType;
    u32 MsgID;
    double PeriodMs;//单位为ms
    std::vector<TSingleGroup>SGInstance;
    int channel;
}TARXMLE2ESG, * PARXMLE2ESG;

//定义的输入信息，控制器的E2E的信号组，根据信号名称确定信号的bustype和msgid，再将SGTXGetFromArxml/SGRXGetFromArxml，push_back
//std::vector<TARXMLE2ESG>DUT_def_E2E_SG_TX;
//std::vector<TARXMLE2ESG>DUT_def_E2E_SG_RX;

typedef struct {
    s32 int_val; // 第一个成员：整型
    char FName[500];
    std::vector<TARXMLE2ESG>DUT_def_E2E_SG_TX;        // 第二个成员：模拟的 std::vector<TARXMLE2ESG>
} IntWithVector_TX;

typedef struct {
    s32 int_val;                  // 第一个成员：整型
    char FName[500];
    std::vector<TARXMLE2ESG>DUT_def_E2E_SG_RX;        // 第二个成员：模拟的 std::vector<TARXMLE2ESG>
} IntWithVector_RX;

IntWithVector_TX DUT_def_E2E_SG_TX_2D_Array[20];
IntWithVector_RX DUT_def_E2E_SG_RX_2D_Array[20];


// 定义空的二维向量
//std::vector<std::vector<TARXMLE2ESG>> DUT_def_E2E_SG_TX_2D_Array;
//std::vector<std::vector<TARXMLE2ESG>> DUT_def_E2E_SG_RX_2D_Array;

std::vector<TDBFrameProperties>DBFrameBuffer;
//中间量，用以暂存
TSingle SingleBuffer;

//寻找信号名称中是否有Chks/Cntr
const std::string S_Chks = "Chks";
const std::string S_Cntr = "Cntr";
//小写字符串，python生成的ini文件中的键都是小写，用以适配
std::string s_chks;
std::string s_cntr;
const std::string CntrConfirmedDTCLimit = "cntrconfirmeddtclimit";
const std::string ChksConfirmedDTCLimit = "chksconfirmeddtclimit";
const std::string CntrStepUpValue = "cntrstepupvalue";
const std::string CntrTestPeriod = "cntrtestperiod";
const std::string CntrMaxValue = "cntrmaxvalue";
const std::string ChksStepUpValue = "chksstepupvalue";
const std::string ChksTestPeriod = "chkstestperiod";
const std::string ChksMaxValue = "chksmaxvalue";
const std::string RelativeSignalCondition = "relativesignalcondition";
const std::string ConditionAnd = "and";
const std::string ConditionOr = "or";
const std::string RelativeSignalName1 = "relativesignalname1";
const std::string RelativeSignalValue1 = "relativesignalvalue1";
const std::string RelativeSignalName2 = "relativesignalname2";
const std::string RelativeSignalValue2 = "relativesignalvalue2";
const std::string RelativeSignalName3 = "relativesignalname3";
const std::string RelativeSignalValue3 = "relativesignalvalue3";
const std::string RelativeSignalName4 = "relativesignalname4";
const std::string RelativeSignalValue4 = "relativesignalvalue4";
const std::string RelativeSignalName5 = "relativesignalname5";
const std::string RelativeSignalValue5 = "relativesignalvalue5";

//VehModMngtGlbSafe1信号组相关字符串
const std::string VehMod = "VehModMngtGlbSafe1";
const std::string S_Usgmode = "VehModMngtGlbSafe1UsgModSts";
const std::string S_Carmode = "VehModMngtGlbSafe1CarModSts1";
const std::string S_EIPowerLevel = "VehModMngtGlbSafe1PwrLvlElecMai";

//ini文件 键 比对
const std::string INI_dataid = "dataid";
const std::string INI_TX = "TX";
const std::string INI_RX = "RX";
const std::string INI_dir = "dir";
const std::string INI_signal = "signal";
const std::string INI_UBPosition = "ubposition";
const std::string INI_Period = "period";

//从arxml中提取的信息，其中信号的startbit，Len，data未赋值，startbit，Len从tsmaster加载到arxml中获取，data在接收到报文时再填入
std::vector<TSingleGroup>SGTXGetFromArxml;
std::vector<TSingleGroup>SGRXGetFromArxml;

typedef struct _MSGID
{
    std::vector<u32>IDBuffer;
    int channel;
}TMSGID, * PMSGID;
std::vector<TMSGID>MSGID;

////定义总线类型
enum BusType1 { BT_CAN, BT_CANFD, BT_LIN, BT_ETH };
//定义唤醒类型
enum WakeUpType { NM, kl15, custom };
//E2E DTC故障成熟时间，对应等待较长时间
int Time_DTCMaturity = 10000;
//NM总线类型，仅支持CAN/CANFD/FR
BusType1 NMBusType = BT_CAN;

//诊断总线类型
BusType1 DiagBusType = BT_CANFD;

//唤醒类型
WakeUpType DUTWakeUpType = NM;

u32 NMID = 0x53f;//0x57F;
int NMDLC = 8;
long NMPeriod = 500;
u8 NMData[8] = { 0x3f,0x40,0xff,0xff,0xff,0xff,0xff,0xff };

//data id 长度为2字节
const int DataIDLen = 2;

//测试控制器在数据库中的名称
std::string TestECUName = "SODL5";

//诊断报文ID
u32 DiagReqID = 0x750;
u32 DiagResID = 0x650;
u32 DiagFuncID = 0x7FF;

//DTC掩码
u8 F_DTCMask = 0x0F;
u8 DefaultDTCMask = 0x09;
u8 ConfirmedDTCMask = 0x08;
u8 CurrentDTCMask = 0x01;

//存储DTC
typedef struct DTCStorge
{
    u8 DTC[3];
    u8 Mask;
};

DTCStorge DTCBuffer;
std::vector<DTCStorge>DTCData;

typedef std::vector<u8>CRCdata;

//未定义报文ID，在Pretest中遍历数据库后赋值
s32 UndefineID = 1;

//各种标志，一般在接收事件中赋值，接收到对应报文置位
int WaitMessage_Flag = 0;
int WaitMessage_TX_Flag = 0;
int E2E_RX_Flag = 0;
int ShortRecoveryFlag = 0;
int ShortRecoveryCheckFlag = 0;
//定义flag，在接收报文时判断，开始存储E2E报文，  目前置于触发E2E报文中
int CheckDUTSendE2E_TXMsgFlag = 0;
//仿真错误CRC/Counter的Flag，在预发送事件中进行判断
int SIMErrorE2EFlag = 0;
int SIMConstCntrFlag = 0;
//满足TRCFlag，UB检测Flag，在FulfilTRC中置1，在Pre预发送中根据置位进行操作
int FulfilTRCFlag = 0;
int UBCheckFlag = 0;

u8 EIPowerLevelValue = 0;

//报文组
std::vector<TCANFD>RX_E2EMsg;

//仿真E2E报文信息
TCANFD MsgBuffer[100];

TCANFD NMMsg_CAN;

//等待时间
int CheckTime = 2000;
int WaitTime = 22000;

//CarMode不同模式map
std::map<int, std::string>CarMode{ {0, "Factory"}, { 1,"Transport" }, { 2,"Crash" }, { 3,"Normal" } };
std::map<int, std::string>CarModeType{ {2, "Factory"}, { 1,"Transport" }, { 3,"Crash" }, { 0,"Normal" }, { 5,"Dynamometer" } };
u8 CarModeValue = 0;
//UsgMode不同模式map
std::map<int, std::string>UsgMode{ {0, "Inactive"}, { 1,"Abandon" }, { 2,"Convenience" }, { 3,"Active" }, { 4,"Driving" } };
std::map<int, std::string>UsgModeType{ {1, "Inactive"}, { 0,"Abandon" }, { 2,"Convenience" }, { 11,"Active" }, { 13,"Driving" } };
u8 UsgModeValue = 13;//默认Driving模式

//将信号值，赋给报文中，i,j,k代表信号位置DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k]
void SetSignalValue(const PCANFD Msg, int i, int j, int k, u32 value, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int StartByte = 0;
    int StartBit = 0;
    int len = 0;

    StartByte = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].startbit / 8;
    StartBit = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].startbit % 8;
    len = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].Len;

    int StartByte_C = StartByte;
    int StartBit_C = StartBit;
    while ((StartBit_C + len) > 0)
    {
        Msg->FData[StartByte_C] = (Msg->FData[StartByte_C] & (0xff ^ (u8((pow(2, StartBit_C + len) - 1) - (pow(2, StartBit_C) - 1)))));

        if (StartByte_C == StartByte)
        {
            Msg->FData[StartByte_C] = Msg->FData[StartByte_C] + ((value << StartBit_C) & 0xff);
        }
        else
        {
            Msg->FData[StartByte_C] = Msg->FData[StartByte_C] + ((value >> (StartBit + (StartByte_C - StartByte) * 8)) & 0xff);
        }

        StartByte_C++;
        len = len - (8 - StartBit_C);
        StartBit_C = 0;
    }

    /*   Msg->FData[StartByte_C] = (Msg->FData[StartByte_C] & (0xff ^ (u8((pow(2, StartBit + len) - 1) - (pow(2, StartBit) - 1)))));

       Msg->FData[StartByte_C] = Msg->FData[StartByte_C] + (value << StartBit);*/

       /* if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Chks) != std::string::npos)
        {
            if (tmpflag == 1)
            {
                TestInfoPrintWarning("cntr0设置chks  Msg->FData[%d] = 0x%02x", StartByte, Msg->FData[StartByte]);
                tmpflag = 0;
            }

        }*/

        /* if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == VehMod)
         {
             if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Chks) != std::string::npos)
             {
                 TestInfoPrintWarning("Msg->FData[%d] = 0x%02x", StartByte, Msg->FData[StartByte]);
             }
         }*/

}

//重载，对应不同参数类型  
//将信号值，赋给报文中，i,j,k代表信号位置DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k]
void SetSignalValue(TCANFD& Msg, int i, int j, int k, u8 value, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int StartByte = 0;
    int StartBit = 0;
    int len = 0;

    StartByte = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].startbit / 8;
    StartBit = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].startbit % 8;
    len = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].Len;

    int StartByte_C = StartByte;
    int StartBit_C = StartBit;
    while ((StartBit_C + len) > 0)
    {
        Msg.FData[StartByte_C] = (Msg.FData[StartByte_C] & (0xff ^ (u8((pow(2, StartBit_C + len) - 1) - (pow(2, StartBit_C) - 1)))));

        if (StartByte_C == StartByte)
        {
            Msg.FData[StartByte_C] = Msg.FData[StartByte_C] + ((value << StartBit_C) & 0xff);
        }
        else
        {
            Msg.FData[StartByte_C] = Msg.FData[StartByte_C] + ((value >> (StartBit + (StartByte_C - StartByte) * 8)) & 0xff);
        }

        StartByte_C++;
        len = len - (8 - StartBit_C);
        StartBit_C = 0;
    }

    /*Msg.FData[StartByte] = Msg.FData[StartByte] & (0xff ^ (u8((pow(2, StartBit + len) - 1) - (pow(2, StartBit) - 1))));

    Msg.FData[StartByte] = Msg.FData[StartByte] + (value << StartBit);*/
}

//将信号值，赋给RearrData中，i,j,k代表信号位置DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k]
void SetSignalValueToRearrData(int i, int j, int k, u32 value, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int StartByte = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].RearrStartByte;
    int len = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].RearrByteLen;

    //StartByte每次++，value右移8*循环次数，再和0xff取&，而后赋值给RearrData
    for (int i = len; i > 0; i++)
    {
        DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[StartByte] = (value >> ((StartByte - DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].RearrStartByte) * 8)) & 0xff;

        StartByte++;
    }
}

//根据ID仿真对应的E2E报文
void SIME2EMsgByID(u32 MsgID, int Channel, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int i, j, k = 0;

    TCANFD MsgBuffer;

    std::vector<u8>CounterBuffer;
    CounterBuffer.resize(100);

    MsgBuffer.init_w_std_id(MsgID, 8);
    if (Channel < 0)
    {
        return;
    }
    MsgBuffer.FIdxChn = Channel;


    for (i = 0; i < DUT_def_E2E_SG_RX.size(); i++)
    {
        if (DUT_def_E2E_SG_RX[i].MsgID == MsgID)
        {
            //DLC赋值
            if (DUT_def_E2E_SG_RX[i].SGInstance.size() > 0)
            {
                MsgBuffer.FDLC = DUT_def_E2E_SG_RX[i].SGInstance[0].DLC;
            }

            for (j = 0; j < DUT_def_E2E_SG_RX[i].SGInstance.size(); j++)
            {
                //DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr = CounterBuffer[j]++;
                DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr = 0;
                DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[0] = DUT_def_E2E_SG_RX[i].SGInstance[j].DataID & 0xff;
                DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[1] = (DUT_def_E2E_SG_RX[i].SGInstance[j].DataID >> 8) & 0xff;
                DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[2] = DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr;

                //关联信号仿真，and条件下，全部仿真
                if (DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalCondition == ConditionAnd)
                {
                    for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                    {
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalName1 == DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName)
                        {
                            SetSignalValue(MsgBuffer, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue1, DUT_def_E2E_SG_RX);
                            SetSignalValueToRearrData(i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue1, DUT_def_E2E_SG_RX);
                        }
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalName2 == DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName)
                        {
                            SetSignalValue(MsgBuffer, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue2, DUT_def_E2E_SG_RX);
                            SetSignalValueToRearrData(i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue2, DUT_def_E2E_SG_RX);
                        }
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalName3 == DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName)
                        {
                            SetSignalValue(MsgBuffer, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue3, DUT_def_E2E_SG_RX);
                            SetSignalValueToRearrData(i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue3, DUT_def_E2E_SG_RX);
                        }
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalName4 == DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName)
                        {
                            SetSignalValue(MsgBuffer, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue4, DUT_def_E2E_SG_RX);
                            SetSignalValueToRearrData(i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue4, DUT_def_E2E_SG_RX);
                        }
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalName5 == DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName)
                        {
                            SetSignalValue(MsgBuffer, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue5, DUT_def_E2E_SG_RX);
                            SetSignalValueToRearrData(i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue5, DUT_def_E2E_SG_RX);
                        }
                    }
                }
                //or条件下，暂定只仿真RelativeSignalName1
                else if (DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalCondition == ConditionOr)
                {
                    for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                    {
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalName1 == DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName)
                        {
                            SetSignalValue(MsgBuffer, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue1, DUT_def_E2E_SG_RX);
                            SetSignalValueToRearrData(i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].RelativeSignalValue1, DUT_def_E2E_SG_RX);
                        }
                    }
                }

                //计算CRC
                DUT_def_E2E_SG_RX[i].SGInstance[j].Chks = CRCCheck(DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData);

                for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                {
                    //吉利规定CRC 1字节，Counter 4bit，此处暂不考虑len大于8和信号跨字节 的情况
                    if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Chks) != std::string::npos)
                    {
                        SetSignalValue(MsgBuffer, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].Chks, DUT_def_E2E_SG_RX);
                    }
                    if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Cntr) != std::string::npos)
                    {
                        SetSignalValue(MsgBuffer, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr, DUT_def_E2E_SG_RX);
                    }
                }
            }
            com.add_cyclic_message_canfd(&MsgBuffer, DUT_def_E2E_SG_RX[i].PeriodMs);
        }
    }
}

//周期性仿真所有DUT接收的E2E报文
void SIME2EMsgCycle(std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int i = 0;

    for (i = 0; i < DUT_def_E2E_SG_RX.size(); i++)
    {
        if (DUT_def_E2E_SG_RX[i].BusType == stCANSignal)
        {
            if (FulfilTRCFlag == 1)
            {
                if (DUT_def_E2E_SG_RX[i].MsgID == VehModID)
                {
                    continue;
                }
            }

            MsgBuffer[i].init_w_std_id(DUT_def_E2E_SG_RX[i].MsgID, 8);
            MsgBuffer[i].FIdxChn = DUT_def_E2E_SG_RX[i].channel;

            SIME2EMsgByID(DUT_def_E2E_SG_RX[i].MsgID, MsgBuffer[i].FIdxChn, DUT_def_E2E_SG_RX);
        }
    }
}

//修改发送E2E报文的CRC，改为错误的CRC
void SIMErrorE2EMsgCycle(u32 MsgID)
{
    //在预发送事件On_Pre_TX_Message中，根据此Flag进行下一步动作
    SIMErrorE2EFlag = 1;
}

//修改发送E2E报文的Cntr，改为恒定为0
void SIMConstCounterMsgCycle(u32 MsgID)
{
    //在预发送事件On_Pre_TX_Message中，根据此Flag进行下一步动作
    SIMConstCntrFlag = 1;
}

//设置EIPowerLevel的值,全局变量，在FulfilTRC_OnPreTX中进行修改和CRC，再放到具体的报文中
void SetEIPowerLevel(int Level)
{
    EIPowerLevelValue = Level;
}

//设置CarMode的值，在FulfilTRC_OnPreTX中进行修改和CRC，再放到具体的报文中
void SetCarMode(int Mode)
{
    for (auto i = CarModeType.begin(); i != CarModeType.end(); i++)
    {
        if (i->second == CarModeType[Mode])
        {
            CarModeValue = i->first;
        }
    }
}

//重载，设置CarMode的值，在FulfilTRC_OnPreTX中进行修改和CRC，再放到具体的报文中
void SetCarMode(std::string Mode)
{
    int res = 0;
    for (auto i = CarModeType.begin(); i != CarModeType.end(); i++)
    {
        if (i->second == Mode)
        {
            res = 1;
            CarModeValue = i->first;
            /*TestInfoPrintFail("i->first = %d", i->first);
            TestInfoPrintFail("i->second = %s", i->second.c_str());*/
        }
    }

    if (res == 0)
    {
        sprintf(actualResponse, "未识别到正确的Mode，当前设置Mode的字符串为%s，Mode可以设置的字符串包括%s,区分大小写", Mode.c_str(), "Factory,Transport,Crash,Normal,Dynamometer");
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail();
    }
}

//设置UsageMode的值，在FulfilTRC_OnPreTX中进行修改和CRC，再放到具体的报文中
void SetUsageMode(int Mode)
{
    for (auto i = UsgModeType.begin(); i != UsgModeType.end(); i++)
    {
        if (i->second == UsgModeType[Mode])
        {
            UsgModeValue = i->first;
        }
    }
}

//重载，设置UsageMode的值，在FulfilTRC_OnPreTX中进行修改和CRC，再放到具体的报文中
void SetUsageMode(std::string Mode)
{
    int res = 0;
    for (auto i = UsgModeType.begin(); i != UsgModeType.end(); i++)
    {
        if (i->second == Mode)
        {
            res = 1;
            UsgModeValue = i->first;
        }
    }

    if (res == 0)
    {
        sprintf(actualResponse, "未识别到正确的Mode，当前设置Mode的字符串为%s，Mode可以设置的字符串包括%s,区分大小写", Mode.c_str(), "Inactive,Abandon,Convenience,Active,Driving");
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail("未识别到正确的Mode，当前设置Mode的字符串为%s，Mode可以设置的字符串包括%s,区分大小写", Mode.c_str(), "Inactive,Abandon,Convenience,Active,Driving");
    }
}

//比较DTC状态位与StandardMask是否相关
void CompareDTCStatusIncluded(u8 StandardMask, u8 TestMask)
{

    if (StandardMask & TestMask)
    {
        sprintf(actualResponse, "DTC状态位与 0x%02x 相关，状态位为0x%02x", StandardMask, TestMask);
        TestReport_HTML_StepDescription(actualResponse, Pass);
        //TestInfoPrintPass("DTC状态位与 0x%02x 相关，状态位为0x%02x", StandardMask, TestMask);
    }
    else
    {
        ////ExceptResult("DTC状态位与 0x%02x 相关", StandardMask);
        sprintf(actualResponse, "DTC状态位与 0x%02x 无关，状态位为0x%02x", StandardMask, TestMask);
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail("DTC状态位与 0x%02x 无关，状态位为0x%02x", StandardMask, TestMask);
    }
}

//比较DTC状态位与StandardMask是否无关
void CompareDTCStatusNotIncluded(u8 StandardMask, u8 TestMask)
{
    if ((StandardMask & TestMask) == 0)
    {
        sprintf(actualResponse, "DTC状态位与 0x%02x 无关，状态位为0x%02x", StandardMask, TestMask);
        TestReport_HTML_StepDescription(actualResponse, Pass);
        //TestInfoPrintPass("DTC状态位与 0x%02x 无关，状态位为0x%02x", StandardMask, TestMask);
    }
    else
    {
        ////ExceptResult("DTC状态位与 0x%02x 无关", StandardMask);
        sprintf(actualResponse, "DTC状态位与 0x%02x 相关，状态位为0x%02x", StandardMask, TestMask);
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail("DTC状态位与 0x%02x 相关，状态位为0x%02x", StandardMask, TestMask);
    }
}

//满足记录ConfirmDTC的条件，参数为confirmedDTCLimit，在诊断调查表中获取
void FulfilConfirm(int confirmedDTCLimit)
{
    int i = 0;

    if (confirmedDTCLimit > 0)
    {
        for (i = 0; i < confirmedDTCLimit; i++)
        {
            DUTSleep();

            WakeUpDUT();
            app.wait(5000, "");
        }
    }

    else
    {
        sprintf(actualResponse, "confirmedDTCLimit = %d,不大于0，请检查诊断调查表，比对此值是否正确", confirmedDTCLimit);
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail("confirmedDTCLimit = %d,不大于0，请检查诊断调查表，比对此值是否正确", confirmedDTCLimit);
    }
}

//满足TRC
void FulfilTRC(std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int i, j, k = 0;
    std::string res;

    for (i = 0; i < DUT_def_E2E_SG_RX.size(); i++)
    {
        for (j = 0; j < DUT_def_E2E_SG_RX[i].SGInstance.size(); j++)
        {
            if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == VehMod)
            {
                res = VehMod;
                VehModID = DUT_def_E2E_SG_RX[i].MsgID;
                VehModCH = DUT_def_E2E_SG_RX[i].channel;

                FulfilTRCFlag = 1;
                SIME2EMsgByID(VehModID, VehModCH, DUT_def_E2E_SG_RX);

                break;
            }
        }

        if (res == VehMod)
        {
            break;
        }
    }

    if (res != VehMod)
    {
        sprintf(actualResponse, "arxml中未检测到信号组%s，无法仿真满足TRC，请检测是否将正确的arxml放在DB文件夹下", VehMod.c_str());
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail("arxml中未检测到信号组%s，无法仿真满足TRC，请检测是否将正确的arxml放在DB文件夹下", VehMod.c_str());
    }
}

//计算TRC的CRC，Counter等
void FulfilTRC_OnPreTX(const PCANFD Msg,std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int i, j, k = 0;
    int Byte, bit = 0;
    for (i = 0; i < DUT_def_E2E_SG_RX.size(); i++)
    {
        if (Msg->FIdentifier == DUT_def_E2E_SG_RX[i].MsgID)
        {
            for (j = 0; j < DUT_def_E2E_SG_RX[i].SGInstance.size(); j++)
            {
                if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName != VehMod)
                {
                    continue;
                }
                //Counter++，增加至0x0E重置
                if (DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr == 0x0E)
                {
                    DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr = 0x0;
                }
                else
                {
                    DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr++;
                }


                //如果是VehMod信号组，赋值
                if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == VehMod)
                {
                    //设置Usgmode为UsgModeValue
                    for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                    {
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName == S_Usgmode)
                        {
                            Byte = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].RearrStartByte;
                            DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[Byte] = UsgModeValue;
                            SetSignalValue(Msg, i, j, k, UsgModeValue,DUT_def_E2E_SG_RX);
                        }
                    }

                    //设置CarMode为CarModeValue
                    for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                    {
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName == S_Carmode)
                        {
                            Byte = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].RearrStartByte;
                            DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[Byte] = CarModeValue;
                            //TestInfoPrintWarning("CarModeValue = %d", CarModeValue);
                            SetSignalValue(Msg, i, j, k, CarModeValue, DUT_def_E2E_SG_RX);
                        }
                    }

                    //设置EIPowerLevel为EIPowerLevelValue
                    for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                    {
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName == S_EIPowerLevel)
                        {
                            Byte = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].RearrStartByte;
                            DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[Byte] = EIPowerLevelValue;
                            SetSignalValue(Msg, i, j, k, EIPowerLevelValue, DUT_def_E2E_SG_RX);
                        }
                    }
                }

                //计算CRC
                DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[2] = DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr;
                DUT_def_E2E_SG_RX[i].SGInstance[j].Chks = CRCCheck(DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData);
                //TestInfoPrintWarning("Cntr = 0x%02X,Chks = 0x%02X", DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr, DUT_def_E2E_SG_RX[i].SGInstance[j].Chks);

                /*if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == VehMod)
                {
                    TestInfoPrintWarning("---------------------------------");
                    TestInfoPrintWarning("DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr = 0x%02x", DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr);
                    TestInfoPrintWarning("DUT_def_E2E_SG_RX[i].SGInstance[j].Chks = 0x%02x", DUT_def_E2E_SG_RX[i].SGInstance[j].Chks);
                    for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData.size(); k++)
                    {
                        TestInfoPrintWarning("DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[k] = 0x%02x", DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[k]);
                    }
                }*/

                //将CRC和Counter赋值到具体报文的字节中
                for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                {
                    if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Chks) != std::string::npos)
                    {
                        /*if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == VehMod)
                        {
                            TestInfoPrintWarning("2-----DUT_def_E2E_SG_RX[i].SGInstance[j].Chks = 0x%02x", DUT_def_E2E_SG_RX[i].SGInstance[j].Chks);
                        }*/
                        SetSignalValue(Msg, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].Chks, DUT_def_E2E_SG_RX);
                    }
                    if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Cntr) != std::string::npos)
                    {
                        SetSignalValue(Msg, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr, DUT_def_E2E_SG_RX);
                    }
                }
            }
        }
    }
}

//CRC校验
u8 CRCCheck(CRCdata data)//,u8 CheckSum)
{
    int i, j = 0;

    //定义CRC初始值为0x00，多项式值为0x1D
    u8 CRC = 0xff;
    u8 CRCPoly = 0x1D;
    u8 XORvalue = 0xFF;

    CRC = CRC ^ XORvalue;

    if (data.size() > 0)
    {
        //进行CRC运算
        for (i = 0; i < data.size(); i++)
        {
            CRC = CRC ^ data[i];

            for (j = 0; j < 8; j++)
            {
                if (CRC & 0x80)
                {
                    CRC = ((CRC << 1) & 0xff);
                    CRC = CRC ^ CRCPoly;
                }
                else
                {
                    CRC = ((CRC << 1) & 0xff);
                }
            }
        }
    }
    return CRC;

    //CRC = CRC ^ XORvalue;
}

//检测第一帧E2E报文的CRC
void CheckFirstCRC(std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX)
{
    int i, j, k = 0;
    u8 CRC = 0;

    int CRCCorrectFlag = 1;

    if (RX_E2EMsg.size() > 0)
    {
        ChangeMsgToCRCData(RX_E2EMsg[0], DUT_def_E2E_SG_TX);

        for (i = 0; i < DUT_def_E2E_SG_TX.size(); i++)
        {
            if (DUT_def_E2E_SG_TX[i].MsgID == RX_E2EMsg[0].FIdentifier)
            {
                for (j = 0; j < DUT_def_E2E_SG_TX[i].SGInstance.size(); j++)
                {
                    CRC = CRCCheck(DUT_def_E2E_SG_TX[i].SGInstance[j].RearrData);
                    //TestInfoPrintWarning("CRC = 0x%x,DUT_def_E2E_SG_TX[i].SGInstance[j].Chks = 0x%x", CRC, DUT_def_E2E_SG_TX[i].SGInstance[j].Chks);
                    //TestInfoPrintWarning("time = %f", RX_E2EMsg[0].FTimeUs/1000.0);
                    if (CRC == DUT_def_E2E_SG_TX[i].SGInstance[j].Chks)
                    {
                        CRC = 1;
                    }
                    else
                    {
                        CRC = 0;
                    }
                    CRCCorrectFlag = CRCCorrectFlag & CRC;
                    //CRCCorrectFlag = (CRCCorrectFlag & CRCCheck(DUT_def_E2E_SG_TX[i].SGInstance[j].RearrData, DUT_def_E2E_SG_TX[i].SGInstance[j].Chks));
                }

            }

        }

        if (CRCCorrectFlag == 1)
        {
            TestReport_HTML_StepDescription("当前测试E2E报文的CRC正确", Pass);
        }
        else
        {
            //////ExceptResult("当前测试E2E报文的CRC正确");
            TestReport_HTML_StepDescription("当前测试E2E报文的CRC错误", Fail);
        }
    }
    else
    {
        TestReport_HTML_StepDescription("DUT未发出E2E报文", Fail);
        //TestInfoPrintFail("DUT未发出E2E报文");
    }
}

//仿真E2E报文时，周期性发送，此Counter是定值，在此函数内进行正确计算CRC/Counter后 再发送，用于在预发送事件中进行修改
void CalculateCntrAndCRC_OnPreTX(const PCANFD Msg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int i, j, k = 0;
    for (i = 0; i < DUT_def_E2E_SG_RX.size(); i++)
    {
        //判断报文ID是否为E2E报文
        if (Msg->FIdentifier == DUT_def_E2E_SG_RX[i].MsgID)
        {
            for (j = 0; j < DUT_def_E2E_SG_RX[i].SGInstance.size(); j++)
            {
                //UsageMode信号组，不必在此处再进行计算，上文已经计算ok
                if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == VehMod)
                {
                    continue;
                }

                //Counter++，增加至0x0E重置
                if (DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr == 0x0E)
                {
                    DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr = 0x0;
                }
                else
                {
                    DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr++;
                }

                //计算CRC
                DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[2] = DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr;
                DUT_def_E2E_SG_RX[i].SGInstance[j].Chks = CRCCheck(DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData);

                //将CRC和Counter赋值到具体报文的字节中
                for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                {
                    if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Chks) != std::string::npos)
                    {
                        SetSignalValue(Msg, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].Chks, DUT_def_E2E_SG_RX);
                    }
                    if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Cntr) != std::string::npos)
                    {
                        SetSignalValue(Msg, i, j, k, DUT_def_E2E_SG_RX[i].SGInstance[j].Cntr, DUT_def_E2E_SG_RX);
                    }
                }
            }
        }
    }
}

//设置UB位为1（Update Bit）
void SetUpdateBit(const PCANFD Msg,std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int i, j, k = 0;
    int UBbyte = 0;
    int UBbit = 0;

    for (i = 0; i < DUT_def_E2E_SG_RX.size(); i++)
    {
        if (Msg->FIdentifier != DUT_def_E2E_SG_RX[i].MsgID)
        {
            continue;
        }

        /*if (TestMsgID == DUT_def_E2E_SG_RX[i].MsgID)
        {*/
        for (j = 0; j < DUT_def_E2E_SG_RX[i].SGInstance.size(); j++)
        {
            if (DUT_def_E2E_SG_RX[i].SGInstance[j].UBPosition == -1)
            {
                continue;
            }

            UBbyte = DUT_def_E2E_SG_RX[i].SGInstance[j].UBPosition / 8;
            UBbit = DUT_def_E2E_SG_RX[i].SGInstance[j].UBPosition % 8;

            Msg->FData[UBbyte] = (Msg->FData[UBbyte] | int(pow(2, UBbit)));
        }
        //}
    }
}

//将CRC的错误值（取反），赋给报文中，i,j,k代表信号位置DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k]
void SetErrorCRC(const PCANFD Msg, int i, int j, int k, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int StartByte = 0;
    int StartBit = 0;
    int len = 0;

    StartByte = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].startbit / 8;
    StartBit = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].startbit % 8;
    len = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].Len;

    //TestInfoPrintWarning("StartByte = %d，正确crc = 0x%02X，错误crc = 0x%02X", StartByte, Msg->FData[StartByte], Msg->FData[StartByte] ^ 0xff);

    //Msg->FData[StartByte] = Msg->FData[StartByte] ^ 0xff;
    Msg->FData[StartByte] = Msg->FData[StartByte] + 5;
    /*Msg.FData[StartByte] = Msg.FData[StartByte] | u8((pow(2, StartBit + len) - 1) - (pow(2, StartBit) - 1));
    Msg.FData[StartByte] = Msg.FData[StartByte] + pow(2, StartBit);*/
}

//将Cntr的错误值（0），赋给报文中，i,j,k代表信号位置DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k]
void SetConstCntr(const PCANFD Msg, int i, int j, int k, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int StartByte = 0;
    int StartBit = 0;
    int len = 0;

    StartByte = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].startbit / 8;
    StartBit = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].startbit % 8;
    len = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].Len;


    Msg->FData[StartByte] = (Msg->FData[StartByte] & (~u8((pow(2, StartBit + len) - 1) - (pow(2, StartBit) - 1))));

    //TestInfoPrintWarning("固定cntr0-----Msg->FData[%d] = 0x%02x", StartByte, Msg->FData[StartByte]);
}

//void printpasscrc(const PCANFD Msg)
//{
//    int i, j, k = 0;
//    for (i = 0; i < DUT_def_E2E_SG_RX.size(); i++)
//    {
//        if (DUT_def_E2E_SG_RX[i].MsgID == Msg->FIdentifier)
//        {
//            for (j = 0; j < DUT_def_E2E_SG_RX[i].SGInstance.size(); j++)
//            {
//                if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == TestSGName)
//                {
//                    for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
//                    {
//                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Chks) != std::string::npos)
//                        {
//                            int StartByte = 0;
//                            int StartBit = 0;
//                            int len = 0;
//
//                            StartByte = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].startbit / 8;
//                            StartBit = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].startbit % 8;
//                            len = DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].Len;
//
//                            TestInfoPrintWarning("正确crc = 0x%02X", Msg->FData[StartByte]);
//                        }
//                    }
//                }
//
//            }
//        }
//    }
//}

//将报文的CRC转为错误的CRC（取反）
void TurnToErrorCRC(const PCANFD Msg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int i, j, k = 0;
    for (i = 0; i < DUT_def_E2E_SG_RX.size(); i++)
    {
        if (DUT_def_E2E_SG_RX[i].MsgID == Msg->FIdentifier)
        {
            for (j = 0; j < DUT_def_E2E_SG_RX[i].SGInstance.size(); j++)
            {
                /*if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == VehMod)
                {
                    continue;
                }*/

                if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == TestSGName)
                {
                    for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                    {
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Chks) != std::string::npos)
                        {
                            //TestInfoPrintWarning("仿真错误CRC");
                            SetErrorCRC(Msg, i, j, k, DUT_def_E2E_SG_RX);
                        }
                    }
                }

            }
        }
    }
}

//将报文的Cntr转为定值0
void TurnToConstCntr(const PCANFD Msg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX)
{
    int i, j, k = 0;
    if (TestMsgID == Msg->FIdentifier)
    {
        for (i = 0; i < DUT_def_E2E_SG_RX.size(); i++)
        {
            if (DUT_def_E2E_SG_RX[i].MsgID == Msg->FIdentifier)
            {
                for (j = 0; j < DUT_def_E2E_SG_RX[i].SGInstance.size(); j++)
                {
                    if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == TestSGName)
                    {
                        //TestInfoPrintWarning("DUT_def_E2E_SG_RX[i].SGInstance[j].SGName = %s", DUT_def_E2E_SG_RX[i].SGInstance[j].SGName.c_str());
                        for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                        {
                            if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Cntr) != std::string::npos)
                            {
                                //tmpflag = 1;
                                //TestInfoPrintWarning("DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName = %s", DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.c_str());
                                SetConstCntr(Msg, i, j, k, DUT_def_E2E_SG_RX);
                            }
                        }
                    }

                }
            }
        }
    }

    //ReCalculateCRC();

    //更改counter固定为0后，CRC需要重新计算，使用counter为0计算CRC
    for (i = 0; i < DUT_def_E2E_SG_RX.size(); i++)
    {
        if (DUT_def_E2E_SG_RX[i].MsgID == Msg->FIdentifier)
        {

            for (j = 0; j < DUT_def_E2E_SG_RX[i].SGInstance.size(); j++)
            {
                if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGName == TestSGName)
                {
                    u8 CRC = 0;
                    DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData[2] = 0;
                    CRC = CRCCheck(DUT_def_E2E_SG_RX[i].SGInstance[j].RearrData);

                    for (k = 0; k < DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement.size(); k++)
                    {
                        if (DUT_def_E2E_SG_RX[i].SGInstance[j].SGElement[k].SName.find(S_Chks) != std::string::npos)
                        {
                            //TestInfoPrintWarning("cntr为0重新设置CRC");
                            SetSignalValue(Msg, i, j, k, CRC, DUT_def_E2E_SG_RX);
                        }
                    }
                }

            }
        }
    }
}

//检测UB位是否为1（Update Bit）
void UBCheck(TCANFD Msg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX)
{
    int i, j, k = 0;
    int UBbyte = 0;
    int UBbit = 0;
    int res = 0;

    for (i = 0; i < DUT_def_E2E_SG_TX.size(); i++)
    {
        if (TestMsgID == DUT_def_E2E_SG_TX[i].MsgID)
        {
            for (j = 0; j < DUT_def_E2E_SG_TX[i].SGInstance.size(); j++)
            {
                if (DUT_def_E2E_SG_TX[i].SGInstance[j].UBPosition == -1)
                {
                    continue;
                }

                UBbyte = DUT_def_E2E_SG_TX[i].SGInstance[j].UBPosition / 8;
                UBbit = DUT_def_E2E_SG_TX[i].SGInstance[j].UBPosition % 8;

                if (((Msg.FData[UBbyte] >> UBbit) & 0x1) != 1)
                {
                    //////ExceptResult("%s信号组的UB位未置1", DUT_def_E2E_SG_TX[i].SGInstance[j].SGName.c_str());
                    sprintf(actualResponse, "%s信号组的UB位未置1，该信号组UB位的位置为byte[%d]，bit[%d]", DUT_def_E2E_SG_TX[i].SGInstance[j].SGName.c_str(), UBbyte, UBbit);
                    TestReport_HTML_StepDescription(actualResponse, Fail);
                    //TestInfoPrintFail("%s信号组的UB位未置1，该信号组UB位的位置为byte[%d]，bit[%d]", DUT_def_E2E_SG_TX[i].SGInstance[j].SGName.c_str(), UBbyte, UBbit);
                    res = 1;
                }

            }
        }
    }
    if (res == 0)
    {
        TestReport_HTML_StepDescription("信号组的UB位已置1", Pass);

        //TestInfoPrintPass("信号组的UB位已置1");
    }


}


//接收报文事件，通过REG()注册后，收到CAN报文即触发此函数
void __stdcall OnAllCANMessage(const pnative_int pobj, const PCANFD ACANFD)
{
    TCANFD frame = *ACANFD;
    //判断是接收
    if (frame.get_is_tx() != true)
    {
        TCANFD f0 = *ACANFD;
        //printf("0-----f0.FIdxChn=%d,f0.FIdentifier=%0X", f0.FIdxChn, f0.FIdentifier);
        if ((((f0.FProperties >> 7) & 1) != 1))
        {
            //printf("1-----f0.FIdxChn=%d,f0.FIdentifier=%0X", f0.FIdxChn, f0.FIdentifier);
            if (CheckDUTSendE2E_TXMsgFlag == 1)
            {
                //printf("2-----TestCH=%d,TestMsgID=%0X", TestCH, TestMsgID);
                //printf("2-----f0.FIdxChn=%d,f0.FIdentifier=%0X", f0.FIdxChn, f0.FIdentifier);
                if (f0.FIdxChn == TestCH)
                {
                    //printf("3-------TestCH=%d,TestMsgID=%0X", TestCH, TestMsgID);
                    if (f0.FIdentifier == TestMsgID)
                    {
                        RX_E2EMsg.push_back(f0);
                        E2E_RX_Flag = 1;
                    }
                }
            }

            if (ShortRecoveryCheckFlag == 1)
            {
                if (f0.FIdxChn == TestCH)
                {
                    ShortRecoveryFlag = 1;
                    ShortRecoveryCheckFlag = 0;
                }
            }

            if (UBCheckFlag == 1)
            {
                if (f0.FIdxChn == TestCH)
                {
                    UBCheck(f0, DUT_def_E2E_SG_TX_2D_Array[TestCH].DUT_def_E2E_SG_TX);
                }
            }

        }
    }
    //判断是发送
    else
    {
        if (frame.FIdxChn == TestCH)
        {
            if ((frame.FProperties >> 7) & 1)
            {

            }
            else
            {
                if (frame.FIdentifier == WaitMessage_TX_ID)
                {
                    WaitMessage_TX_Flag = 1;
                }

            }
        }
    }

}

//预发送报文事件，通过REG()注册后，收到CAN报文即触发此函数
void __stdcall OnPreCANMessage(const pnative_int pobj, const PCANFD ACANFD)
{
    //UB位设置为1
    SetUpdateBit(ACANFD, DUT_def_E2E_SG_RX_2D_Array[TestCH].DUT_def_E2E_SG_RX);

    if (ACANFD->FIdxChn == VehModCH)
    {
        if (ACANFD->FIdentifier == VehModID)
        {
            if (FulfilTRCFlag == 1)
            {
                FulfilTRC_OnPreTX(ACANFD, DUT_def_E2E_SG_RX_2D_Array[VehModCH].DUT_def_E2E_SG_RX);
            }


        }
    }


    if (ACANFD->FIdxChn == TestCH)
    {
        /*if (ACANFD->FIdentifier != VehModID)
        {*/
        //计算正确CRC/Counter发送
        CalculateCntrAndCRC_OnPreTX(ACANFD, DUT_def_E2E_SG_RX_2D_Array[TestCH].DUT_def_E2E_SG_RX);
        /*}*/

        if (ACANFD->FIdentifier == TestMsgID)
        {
            //SIMErrorE2EFlag置1时，发送错误的CRC
            if (SIMErrorE2EFlag == 1)
            {
                TurnToErrorCRC(ACANFD,DUT_def_E2E_SG_RX_2D_Array[TestCH].DUT_def_E2E_SG_RX);
            }

            //SIMConstCntrFlag置1时，发送错误的Counter
            if (SIMConstCntrFlag == 1)
            {
                TurnToConstCntr(ACANFD, DUT_def_E2E_SG_RX_2D_Array[TestCH].DUT_def_E2E_SG_RX);
            }
        }
    }
}

//接收报文事件，通过REG()注册后，收到CAN报文即触发此函数
void __stdcall OnDiagnosticMessage(const pnative_int pobj, const PCANFD ACANFD)
{
    TCANFD frame = *ACANFD;
    //判断是接收
    if (frame.get_is_tx() != true)
    {
        if (Diag_Response_ID == frame.FIdentifier) {
            if (frame.FDLC <= 8) {
                if (0x7F != frame.FData[1]) {
                    for (u8 i = 0; i < frame.FData[0]; ++i) {
                        TpRxData[i] = frame.FData[i + 1];
                    }
                }
            }
        }
       
    }
    
}

//仿真NM报文
void SIMNMMessage()
{
    u8 NMData_NEW[8] = { 0x3F,0x40,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    switch (NMBusType)
    {
    case BT_CAN:
        NMMsg_CAN.init_w_std_id(0x53f, NMDLC);
        NMMsg_CAN.FIdxChn = NMCH;
        memcpy(NMMsg_CAN.FData, NMData_NEW, NMDLC);
        NMMsg_CAN.set_is_edl(false);
        com.add_cyclic_message_canfd(&NMMsg_CAN, NMPeriod);
        break;
    case BT_CANFD:
        NMMsg_CAN.init_w_std_id(0x53f, NMDLC);
        NMMsg_CAN.FIdxChn = NMCH;
        memcpy(NMMsg_CAN.FData, NMData_NEW, NMDLC);
        com.add_cyclic_message_canfd(&NMMsg_CAN, NMPeriod);
        break;
    default:
        sprintf(actualResponse, "出现未知NM报文总线类型，请检查NMBusType值是否为“CAN”");
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail("出现未知NM报文总线类型，请检查NMBusType值是否为“CAN”");
        break;
    }
}

//停止仿真NM报文
void StopSIMNMMessage()
{
    switch (NMBusType)
    {
    case BT_CAN:
        com.del_cyclic_message_canfd(&NMMsg_CAN);
        break;
    case BT_CANFD:
        com.del_cyclic_message_canfd(&NMMsg_CAN);
        break;
    default:
        sprintf(actualResponse, "出现未知NM报文总线类型，请检查NMBusType值是否为“CAN”");
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail("出现未知NM报文总线类型，请检查NMBusType值是否为“CAN”");
        break;
    }
}

//唤醒DUT
void WakeUpDUT()
{
    switch (DUTWakeUpType)
    {
    case kl15:
        KL15(on);
        break;
    case NM:
        SIMNMMessage();
        break;
    case custom:
        //CustomWakeUp();
    default:
        
        TestReport_HTML_StepDescription("出现未知唤醒类型，请检查DUTWakeUpType", Fail);
        //TestInfoPrintFail("出现未知唤醒类型，请检查DUTWakeUpType");
        break;
    }
    app.wait(1000, "");
}

//DUT休眠
void DUTSleep()
{
    switch (DUTWakeUpType)
    {
    case kl15:
        KL15(off);
        break;
    case NM:
        StopSIMNMMessage();
        break;
    default:
        TestReport_HTML_StepDescription("出现未知唤醒类型，请检查DUTWakeUpType", Fail);
        //TestInfoPrintFail();
        break;
    }

    app.wait(5000, "");
}
//初始化所有Flag
void InitAllFlag()
{
    WaitMessage_Flag = 0;
    WaitMessage_TX_Flag = 0;

    E2E_RX_Flag = 0;
    SIMErrorE2EFlag = 0;
    SIMConstCntrFlag = 0;
    CheckDUTSendE2E_TXMsgFlag = 0;

    ShortRecoveryCheckFlag = 0;
    ShortRecoveryFlag = 0;

    EIPowerLevelValue = 0;
    CarModeValue = 0;
    FulfilTRCFlag = 0;
    UBCheckFlag = 0;
    UsgModeValue = 13;
}

//清空所有vector数组
void ClearAllVector()
{
    RX_E2EMsg.clear();
}
//测试用例前板卡的初始化
inline void SetBoardCardInit()
{
    //SetBoardCardInit_Devices();
}

native_int obj;
native_int Preobj;
native_int obj1;
native_int obj2;
native_int objNVM;
//注册接收报文事件
inline void REG()
{
    com.register_event_canfd(&obj, OnAllCANMessage);
    com.register_pretx_event_canfd(&Preobj, OnPreCANMessage);
}

//取消注册接收报文事件
inline void UNREG()
{
    com.unregister_event_canfd(&obj, OnAllCANMessage);
    com.unregister_pretx_event_canfd(&Preobj, OnPreCANMessage);
    printf("unregister message event");
}

inline void NMREG() {
    com.register_event_canfd(&obj1, On_RX_CAN_Msg);
    com.register_event_canfd(&obj2, On_TX_CAN_Msg);
}

//取消注册接收报文事件
inline void UNNMREG()
{
    com.unregister_event_canfd(&obj1, On_RX_CAN_Msg);
    com.unregister_event_canfd(&obj2, On_TX_CAN_Msg);
    com.unregister_pretx_event_canfd(&Preobj, OnPreCANMessage);
}

//注册接收报文事件
inline void NVMREG()
{
    com.register_event_canfd(&objNVM, OnDiagnosticMessage);
}

//取消注册接收报文事件
inline void UNNVMREG()
{
    com.unregister_event_canfd(&objNVM, OnDiagnosticMessage);
    printf("unregister message event");
}



//触发DUT发送E2E报文
void TiggerDUTSendE2E_TXMsg()
{
    CheckDUTSendE2E_TXMsgFlag = 1;
}

void PreE2Econdition()
{
    Test_Result = Pass;
    stepnumber = 0;

    //UNREG();
    REG();
    //板卡初始化,KL电源连接,暂未连接实体板卡调试
    app.wait(1000, "");
    PrintCaseNameAndPurpose();

    TestReport_StartLogging();

    TestReport_TableInit();

    InitAllFlag();
    ClearAllVector();

    KL15(off);
    app.wait(500, "");
    KL30(off);
    app.wait(500, "");

    //SetVoltage(Vnormal);
    app.wait(1000, "");
}

//将报文下数据赋值到具体的信号中，先进行排序，此处会按排序后的信号序列进行存放
void ChangeMsgToCRCData(TCANFD UnderCheckMsg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX)
{
    int i, j, k = 0;

    for (i = 0; i < DUT_def_E2E_SG_TX.size(); i++)
    {
        if (DUT_def_E2E_SG_TX[i].MsgID == UnderCheckMsg.FIdentifier)
        {
            for (j = 0; j < DUT_def_E2E_SG_TX[i].SGInstance.size(); j++)
            {
                for (k = 0; k < DUT_def_E2E_SG_TX[i].SGInstance[j].SGElement.size(); k++)
                {
                    int len = DUT_def_E2E_SG_TX[i].SGInstance[j].SGElement[k].Len;
                    int startNo = DUT_def_E2E_SG_TX[i].SGInstance[j].SGElement[k].RearrStartByte;// +DataIDLen;
                    //从0开始
                    int start_bit = DUT_def_E2E_SG_TX[i].SGInstance[j].SGElement[k].startbit % 8;
                    int datastartNo = DUT_def_E2E_SG_TX[i].SGInstance[j].SGElement[k].startbit / 8;

                    //判断非chks信号
                    if (DUT_def_E2E_SG_TX[i].SGInstance[j].SGElement[k].SName.find(S_Chks) == std::string::npos)
                    {

                        //其他普通信号
                        while (len > 8)
                        {
                            DUT_def_E2E_SG_TX[i].SGInstance[j].RearrData[startNo] = ((UnderCheckMsg.FData[datastartNo] >> start_bit) + (UnderCheckMsg.FData[datastartNo - 1] << (8 - start_bit)));

                            datastartNo--;
                            startNo++;
                            len -= 8;
                        }

                        if (start_bit + len <= 8)
                        {
                            DUT_def_E2E_SG_TX[i].SGInstance[j].RearrData[startNo] = ((UnderCheckMsg.FData[datastartNo] >> start_bit) & (int(pow(2, len) - 1)));
                        }
                        else
                        {
                            DUT_def_E2E_SG_TX[i].SGInstance[j].RearrData[startNo] = (UnderCheckMsg.FData[datastartNo] >> start_bit) + ((UnderCheckMsg.FData[datastartNo - 1] & (int(pow(2, start_bit + len - 8) - 1))) << (8 - start_bit));
                        }
                        //Cntr赋值,Cntr固定占4bit，在一个字节内，且在RearrData的dataid后，即[2]，此处简单判断处理
                        if (DUT_def_E2E_SG_TX[i].SGInstance[j].SGElement[k].SName.find(S_Cntr) != std::string::npos)
                        {
                            DUT_def_E2E_SG_TX[i].SGInstance[j].Cntr = DUT_def_E2E_SG_TX[i].SGInstance[j].RearrData[DataIDLen];

                        }

                    }
                    //Chks单独赋值，不参与CRC计算，Chks长度固定为8bit
                    else
                    {
                        DUT_def_E2E_SG_TX[i].SGInstance[j].Chks = (UnderCheckMsg.FData[datastartNo] >> start_bit) + ((UnderCheckMsg.FData[datastartNo - 1] & (int(pow(2, start_bit) - 1))) << (8 - start_bit));
                        //TestInfoPrintWarning("获取到chks信号，DUT_def_E2E_SG_TX[i].SGInstance[j].Chks = 0x%x", DUT_def_E2E_SG_TX[i].SGInstance[j].Chks);
                        //TestInfoPrintWarning("time = %f,datastartNo = %d,start_bit = %d", UnderCheckMsg.FTimeUs/1000000.0, datastartNo, start_bit);
                        //TestInfoPrintWarning("DUT_def_E2E_SG_TX[i].SGInstance[j].SGElement[k].startbit = %d", DUT_def_E2E_SG_TX[i].SGInstance[j].SGElement[k].startbit);
                        //TestInfoPrintWarning("%s", DUT_def_E2E_SG_TX[i].SGInstance[j].SGElement[k].SName.c_str());
                        /*for (int cbd = 0;cbd< UnderCheckMsg.get_data_length();cbd++)
                        {
                            TestInfoPrintWarning("data[%d] = 0x%x",cbd, UnderCheckMsg.FData[cbd]);
                        }*/
                        //TestInfoPrintWarning("%s",std::to_string(UnderCheckMsg.FData));
                    }
                }

                //前两个字节存放dataid
                DUT_def_E2E_SG_TX[i].SGInstance[j].RearrData[1] = (DUT_def_E2E_SG_TX[i].SGInstance[j].DataID >> 8);
                DUT_def_E2E_SG_TX[i].SGInstance[j].RearrData[0] = (DUT_def_E2E_SG_TX[i].SGInstance[j].DataID & 0xff);
            }

        }

    }
}



//等待报文，第Number帧，等待时间waittime，Number作为第N帧报文，从1开始计数
void MessageWait(int Number, int waittime)
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    while (RX_E2EMsg.size() < Number)
    {
        if (std::chrono::steady_clock::now() - start > std::chrono::milliseconds(waittime))
        {
            //////ExceptResult("所测%dms时间内，控制器发出第%d帧所等报文", waittime, Number);
            TestInfoPrintPass("所测%dms时间内，控制器未发出第%d帧所等报文", waittime, Number);
            //TestReport_HTML_StepDescription(actualResponse, Pass);
            break;
        }
        app.wait(0, "");
    }
}

//检测CRC，从E2E报文的第StartNumber帧到第StopNumber帧
void CheckCRCNumber(int StartNumber, int StopNumber, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX)
{
    int i, j, k = 0;
    u8 CRC = 0;
    int flag = 1;
    s32 CH = 0;

    if (RX_E2EMsg.size() >= StopNumber)
    {
        for (i = StartNumber; i < StopNumber; i++)
        {
            int CRCCorrectFlag = 1;

            ChangeMsgToCRCData(RX_E2EMsg[i], DUT_def_E2E_SG_TX);

            for (j = 0; j < DUT_def_E2E_SG_TX.size(); j++)
            {
                //printf("j=%d", j);
                if (DUT_def_E2E_SG_TX[j].MsgID == RX_E2EMsg[i].FIdentifier)
                {
                    for (k = 0; k < DUT_def_E2E_SG_TX[j].SGInstance.size(); k++)
                    {
                        //printf("k=%d", k);
                        CRC = CRCCheck(DUT_def_E2E_SG_TX[j].SGInstance[k].RearrData);
                        sprintf(actualResponse, "CRC = 0x%x,Number=%02d,DUT_def_E2E_SG_TX[%d].SGInstance[%d].Chks = 0x%x", CRC,i, j,k,DUT_def_E2E_SG_TX[j].SGInstance[k].Chks);
                        TestReport_HTML_StepDescription(actualResponse, Pass);
                        //TestInfoPrintWarning("CRC = 0x%x,DUT_def_E2E_SG_TX[i].SGInstance[j].Chks = 0x%x", CRC, DUT_def_E2E_SG_TX[j].SGInstance[k].Chks);
                        //TestInfoPrintWarning("time = %f", RX_E2EMsg[i].FTimeUs / 1000.0);
                        if (CRC == DUT_def_E2E_SG_TX[j].SGInstance[k].Chks)
                        {
                            CRC = 1;
                        }
                        else
                        {
                            CRC = 0;
                        }
                        CRCCorrectFlag = CRCCorrectFlag & CRC;
                    }

                }

            }

            if (CRCCorrectFlag == 1)
            {
                /*TestInfoPrintPass("当前测试第%d帧E2E报文的CRC正确", i);*/
            }
            else
            {
                flag = 0;
                sprintf(actualResponse, "当前测试第%d帧E2E报文的CRC错误，报文时间戳为%fs", i, RX_E2EMsg[i].FTimeUs / 1000000.0);
                TestReport_HTML_StepDescription(actualResponse, Fail);

                
            }
        }
        if (flag == 1)
        {
            TestReport_HTML_StepDescription("E2E报文的CRC都正确", Pass);
            //TestInfoPrintPass("E2E报文的CRC都正确");
        }
    }
    else
    {
        sprintf(actualResponse, "DUT未发出%d帧E2E报文", StopNumber);

        //////ExceptResult("DUT发出%d帧E2E报文", StopNumber - StartNumber);
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //////ExceptResult("DUT发出%d帧E2E报文", StopNumber);
        //TestInfoPrintFail("DUT未发出%d帧E2E报文", StopNumber);
    }
}

//检测Conter，从E2E报文的第StartNumber帧到第StopNumber帧
void CheckCounterNumber(int StartNumber, int StopNumber, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX)
{
    int i, j, k = 0;
    int flag = 1;
    std::vector<u8>CounterBuffer;
    CounterBuffer.resize(100);
    int CounterCorrectFlag = 1;

    if (RX_E2EMsg.size() >= StopNumber)
    {
        for (i = StartNumber; i < StopNumber; i++)
        {
            //int CounterCorrectFlag = 1;
            ChangeMsgToCRCData(RX_E2EMsg[i], DUT_def_E2E_SG_TX);

            //首帧赋counter初值，不判断是否正确
            if (i == StartNumber)
            {
                for (j = 0; j < DUT_def_E2E_SG_TX.size(); j++)
                {
                    if (DUT_def_E2E_SG_TX[j].MsgID == RX_E2EMsg[i].FIdentifier)
                    {
                        for (k = 0; k < DUT_def_E2E_SG_TX[j].SGInstance.size(); k++)
                        {
                            //TestInfoPrintWarning("DUT_def_E2E_SG_TX[j].SGInstance[k].Cntr = 0x%02X", DUT_def_E2E_SG_TX[j].SGInstance[k].Cntr);
                            CounterBuffer[k] = DUT_def_E2E_SG_TX[j].SGInstance[k].Cntr;

                        }

                    }

                }
            }
            //首帧后进行counter++，判断是否递增
            else
            {
                for (j = 0; j < DUT_def_E2E_SG_TX.size(); j++)
                {
                    if (DUT_def_E2E_SG_TX[j].MsgID == RX_E2EMsg[i].FIdentifier)
                    {
                        for (k = 0; k < DUT_def_E2E_SG_TX[j].SGInstance.size(); k++)
                        {
                            if (CounterBuffer[k] == 0xe)
                            {
                                CounterBuffer[k] = 0;
                            }
                            else
                            {
                                CounterBuffer[k]++;
                            }

                            if (CounterBuffer[k] != DUT_def_E2E_SG_TX[j].SGInstance[k].Cntr)
                            {
                                //////ExceptResult("当前测试第%d帧E2E报文的%s信号组的Counter正确，应当为0x%x", i + 1 - StartNumber, DUT_def_E2E_SG_TX[j].SGInstance[k].SGName.c_str(), CounterBuffer[k]);
                                sprintf(actualResponse, "当前测试第%d帧E2E报文的%s信号组的Counter错误，当前counter值为0x%x，应当为0x%x，报文时间戳%fms", i + 1 - StartNumber, DUT_def_E2E_SG_TX[j].SGInstance[k].SGName.c_str(), DUT_def_E2E_SG_TX[j].SGInstance[k].Cntr, CounterBuffer[k], RX_E2EMsg[i].FTimeUs / 1000.0);
                                TestReport_HTML_StepDescription(actualResponse, Fail);

                                //TestInfoPrintFail("当前测试第%d帧E2E报文的%s信号组的Counter错误，当前counter值为0x%x，应当为0x%x，报文时间戳%fms", i + 1 - StartNumber, DUT_def_E2E_SG_TX[j].SGInstance[k].SGName.c_str(), DUT_def_E2E_SG_TX[j].SGInstance[k].Cntr, CounterBuffer[k], RX_E2EMsg[i].FTimeUs / 1000.0);
                                CounterCorrectFlag = 0;
                            }
                            else
                            {
                                sprintf(actualResponse, "当前测试第%d帧E2E报文的%s信号组的Counter正确，当前counter值为0x%x，报文时间戳%fms", i + 1 - StartNumber, DUT_def_E2E_SG_TX[j].SGInstance[k].SGName.c_str(), DUT_def_E2E_SG_TX[j].SGInstance[k].Cntr, RX_E2EMsg[i].FTimeUs / 1000.0);
                                TestReport_HTML_StepDescription(actualResponse, Pass);

                                //TestInfoPrintPass("当前测试第%d帧E2E报文的%s信号组的Counter正确，当前counter值为0x%x，报文时间戳%fms", i + 1 - StartNumber, DUT_def_E2E_SG_TX[j].SGInstance[k].SGName.c_str(), DUT_def_E2E_SG_TX[j].SGInstance[k].Cntr, RX_E2EMsg[i].FTimeUs / 1000.0);
                            }
                        }

                    }

                }
            }


            //if (CounterCorrectFlag == 1)
            //{
            //    //TestInfoPrintPass("当前测试第%d帧E2E报文的Counter正确", i+1 - StartNumber);
            //}
            //else
            //{
            //    flag = 0;
            //    ////ExceptResult("当前测试第%d帧E2E报文的Counter正确",i + 1 - StartNumber);
            //    TestInfoPrintFail("当前测试第%d帧E2E报文的Counter错误，报文时间戳为%fs", i+1 - StartNumber, RX_E2EMsg[i- StartNumber].FTimeUs / 1000000.0);
            //}
        }
        /*if (flag == 1)
        {
            TestInfoPrintPass("E2E报文的Counter都正确");
        }*/
        if (CounterCorrectFlag == 1)
        {
            TestReport_HTML_StepDescription("E2E报文的Counter都正确", Pass);
            //TestInfoPrintPass("E2E报文的Counter都正确");
        }
    }
    else
    {
        sprintf(actualResponse, "DUT未发出%d帧E2E报文", StopNumber - StartNumber);

        //////ExceptResult("DUT发出%d帧E2E报文", StopNumber - StartNumber);
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail("DUT未发出%d帧E2E报文", StopNumber - StartNumber);
    }
}

int WaitFlag(int& Flag, int waittime)
{
    int res = 0;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    while (1)
    {
        if (Flag == 1)
        {
            res++;
            Flag = 0;
            break;
        }
        if (std::chrono::steady_clock::now() - start > std::chrono::milliseconds(waittime))
        {
            res = 0;
            break;
        }
        app.wait(0, "");
    }

    return res;
}


void RegexSpace(std::string& str1, std::string str2)
{

    std::regex re("\\s+");

    str1 = std::regex_replace(str1, re, str2);
}

int hexStringToInt(const std::string& hexStr) {
    int num;
    std::istringstream iss(hexStr);
    // 设置输入流为十六进制模式
    iss >> std::hex >> num;
    if (iss.fail()) {
        throw std::invalid_argument("无法将输入字符串转换为十六进制整数");
    }
    return num;
}

//获取INI Key和Value----------------------------------NM数据长度不同，待修改，还有工具箱也待修改
void GetINIValue(std::string Key, std::string Value)
{
    g_parameter.NMData.resize(8);

    if (Key == "TestECUName")
    {
        g_parameter.TestECUName = Value;
    }
    if (Key == "DiagReqID")
    {
        g_parameter.DiagReqID = hexStringToInt(Value);
    }
    if (Key == "DiagResID")
    {
        g_parameter.DiagResID = hexStringToInt(Value);
    }
    if (Key == "DiagFuncID")
    {
        g_parameter.DiagFuncID = hexStringToInt(Value);
    }
    if (Key == "NMDLC")
    {
        g_parameter.NMDLC = stoi(Value);
    }
    if (Key == "NMPeriod")
    {
        g_parameter.NMPeriod = stoi(Value);
    }
    if (Key == "NMID")
    {
        g_parameter.NMID = hexStringToInt(Value);
    }

    if (Key == "DiagBusType")
    {
        g_parameter.DiagBusType = Value;
    }
    if (Key == "DUTWakeUpType")
    {
        g_parameter.DUTWakeUpType = Value;
    }
    if (Key == "NMBusType")
    {
        g_parameter.NMBusType = Value;
    }


    if (Key == "NMData0")
    {
        g_parameter.NMData[0] = stoi(Value);
    }
    if (Key == "NMData1")
    {
        g_parameter.NMData[1] = stoi(Value);
    }
    if (Key == "NMData2")
    {
        g_parameter.NMData[2] = stoi(Value);
    }
    if (Key == "NMData3")
    {
        g_parameter.NMData[3] = stoi(Value);
    }
    if (Key == "NMData4")
    {
        g_parameter.NMData[4] = stoi(Value);
    }
    if (Key == "NMData5")
    {
        g_parameter.NMData[5] = stoi(Value);
    }
    if (Key == "NMData6")
    {
        g_parameter.NMData[6] = stoi(Value);
    }
    if (Key == "NMData7")
    {
        g_parameter.NMData[7] = stoi(Value);
    }
    if (Key == "NetworkName")
    {
        g_parameter.NetworkName = Value;
    }

}

//将g_parameter参数赋值到parameter中
void SetParametersFromINI()
{
    //测试ECU名称
    TestECUName = g_parameter.TestECUName;

    //测试网络名称
    NetworkName = g_parameter.NetworkName;

    //诊断参数赋值
    DiagReqID = g_parameter.DiagReqID;
    DiagFuncID = g_parameter.DiagFuncID;
    DiagResID = g_parameter.DiagResID;

    //NM参数赋值
    NMPeriod = g_parameter.NMPeriod;
    NMDLC = g_parameter.NMDLC;
    NMID = g_parameter.NMID;

    std::copy(g_parameter.NMData.begin(), g_parameter.NMData.end(), NMData);

    //诊断总线类型
    if (g_parameter.DiagBusType == "CAN")
    {
        DiagBusType = BT_CAN;
    }
    else if (g_parameter.DiagBusType == "CANFD")
    {
        DiagBusType = BT_CANFD;
    }
    else if (g_parameter.DiagBusType == "ETH")
    {
        DiagBusType = BT_ETH;
    }

    //NM总线类型
    if (g_parameter.NMBusType == "CAN")
    {
        NMBusType = BT_CAN;
    }
    else if (g_parameter.NMBusType == "CANFD")
    {
        NMBusType = BT_CANFD;
    }
    else if (g_parameter.NMBusType == "ETH")
    {
        NMBusType = BT_ETH;
    }

    //唤醒
    if (g_parameter.DUTWakeUpType == "NM")
    {
        DUTWakeUpType = NM;
    }
    else if (g_parameter.DUTWakeUpType == "KL15")
    {
        DUTWakeUpType = kl15;
    }
}
//定义用于解析INI的类
class ParsingINI
{
public:
    void setINIPath(std::string Filename);
    int LoadINI();
    void GetValue(std::string Key, std::string Value);
    //void GetDTCValue(std::string Key, std::string Value);
    void INISGBufferInit();

private:
    std::string INIfilePath;
    TSingleGroup INISGBuffer;
    std::string DIR;
};

//初始化INISGBuffer
void ParsingINI::INISGBufferInit()
{
    INISGBuffer.SGElement.clear();
    INISGBuffer.PeriodMs = -1;
    INISGBuffer.RearrData.clear();;
    INISGBuffer.SGName.clear();
    INISGBuffer.UBPosition = -1;
    INISGBuffer.bustype = stCANSignal;
    INISGBuffer.channel = -1;
    INISGBuffer.Chks = 0;
    INISGBuffer.ChksConfirmedDTCLimit = 0;
    INISGBuffer.ChksDTC = 0;
    INISGBuffer.ChksMaxValue = 0;
    INISGBuffer.ChksStepUpValue = 0;
    INISGBuffer.ChksTestPeriod = 0;
    INISGBuffer.Cntr = 0;
    INISGBuffer.CntrConfirmedDTCLimit = 0;
    INISGBuffer.CntrDTC = 0;
    INISGBuffer.CntrMaxValue = 0;
    INISGBuffer.DataID = 0;
    INISGBuffer.msgid = 0;
}

//设置INI文件路径
void ParsingINI::setINIPath(std::string Filename)
{
    INIfilePath = Filename;
}

//加载/解析INI文件
int ParsingINI::LoadINI()
{
    std::string line;
    std::ifstream fs(INIfilePath.c_str());
    std::string Key;
    std::string Value;


    if (!fs.is_open())
    {
        printf("打开ini文件 %s 失败", INIfilePath.c_str());
        //TestInfoPrintFail("打开ini文件 %s 失败", INIfilePath.c_str());
        //TestTerminate();
    }

    //逐行获取
    while (getline(fs, line))
    {
        //跳过空行
        if (line.empty())
        {
            continue;
        }

        //清除line中的空格
        RegexSpace(line, "");

        //判断为ini的节
        if ((line[0] == '[' && line[line.size() - 1] == ']'))
        {
            //根据INI中DIR判断为TX/RX报文
            if (DIR == INI_TX)
            {
                SGTXGetFromArxml.push_back(INISGBuffer);
            }
            if (DIR == INI_RX)
            {
                SGRXGetFromArxml.push_back(INISGBuffer);
            }

            //初始化INISGBuffer
            INISGBufferInit();
            //ini节的内容作为INISGBuffer中的SGName
            INISGBuffer.SGName = line.substr(1, line.size() - 2);
        }
        //判断为ini的键和具体的值
        else
        {
            //根据‘=’划分键和值
            std::string::size_type Equalpos = line.find('=');

            if (Equalpos == std::string::npos)
            {
                continue;
            }

            Key = line.substr(0, Equalpos);
            Value = line.substr(Equalpos + 1, line.size() - 1);

            GetValue(Key, Value);
        }
    }

    // if ((line[0] == '[' && line[line.size() - 1] == ']')),由于前文此处以节判断一个信号组的结束，最后一个信号组就不会被push到SGTXGetFromArxml/SGRXGetFromArxml中，需再执行一次
    if (DIR == INI_TX)
    {
        SGTXGetFromArxml.push_back(INISGBuffer);
    }
    if (DIR == INI_RX)
    {
        SGRXGetFromArxml.push_back(INISGBuffer);
    }

    fs.close();
    return 0;
}

std::string lowerChange(std::string str1)
{
    int i = 0;

    for (i = 0; i < str1.size(); i++)
    {
        if (str1[i] >= 'A' && str1[i] <= 'Z')
        {
            str1[i] = tolower(str1[i]);
        }
    }

    return str1;
}
//将键和对应的值 根据键名，赋到INISGBuffer中
void ParsingINI::GetValue(std::string Key, std::string Value)
{
    int i = 0;
    std::string stringbuffer;

    if (Key == INI_dataid)
    {
        INISGBuffer.DataID = stoi(Value);
    }

    if (Key == INI_dir)
    {
        DIR = Value;
    }

    if (Key == INI_UBPosition)
    {
        INISGBuffer.UBPosition = stoi(Value);
    }

    if (Key == INI_Period)
    {
        INISGBuffer.PeriodMs = stod(Value) * 1000;//转成ms单位
    }

    if (Key.find(INI_signal) != std::string::npos)
    {
        SingleBuffer.SName = Value;
        INISGBuffer.SGElement.push_back(SingleBuffer);
    }

    if (Key.find(CntrConfirmedDTCLimit) != std::string::npos)
    {
        INISGBuffer.CntrConfirmedDTCLimit = stoi(Value);
    }

    if (Key.find(ChksConfirmedDTCLimit) != std::string::npos)
    {
        INISGBuffer.ChksConfirmedDTCLimit = stoi(Value);
    }

    if (Key.find(CntrStepUpValue) != std::string::npos)
    {
        INISGBuffer.CntrStepUpValue = stoi(Value);
    }

    if (Key.find(CntrTestPeriod) != std::string::npos)
    {
        INISGBuffer.CntrTestPeriod = stoi(Value);
    }

    if (Key.find(CntrMaxValue) != std::string::npos)
    {
        INISGBuffer.CntrMaxValue = stoi(Value);
    }

    if (Key.find(ChksStepUpValue) != std::string::npos)
    {
        INISGBuffer.ChksStepUpValue = stoi(Value);
    }

    if (Key.find(ChksTestPeriod) != std::string::npos)
    {
        INISGBuffer.ChksTestPeriod = stoi(Value);
    }

    if (Key.find(ChksMaxValue) != std::string::npos)
    {
        INISGBuffer.ChksMaxValue = stoi(Value);
    }


    //信号关联获取
    if (Key.find(RelativeSignalCondition) != std::string::npos)
    {
        INISGBuffer.RelativeSignalCondition = Value;
    }

    if (Key.find(RelativeSignalName1) != std::string::npos)
    {
        INISGBuffer.RelativeSignalName1 = Value;
    }
    if (Key.find(RelativeSignalValue1) != std::string::npos)
    {
        INISGBuffer.RelativeSignalValue1 = stoi(Value);
    }

    if (Key.find(RelativeSignalName2) != std::string::npos)
    {
        INISGBuffer.RelativeSignalName2 = Value;
    }
    if (Key.find(RelativeSignalValue2) != std::string::npos)
    {
        INISGBuffer.RelativeSignalValue2 = stoi(Value);
    }

    if (Key.find(RelativeSignalName3) != std::string::npos)
    {
        INISGBuffer.RelativeSignalName3 = Value;
    }
    if (Key.find(RelativeSignalValue3) != std::string::npos)
    {
        INISGBuffer.RelativeSignalValue3 = stoi(Value);
    }

    if (Key.find(RelativeSignalName4) != std::string::npos)
    {
        INISGBuffer.RelativeSignalName4 = Value;
    }
    if (Key.find(RelativeSignalValue4) != std::string::npos)
    {
        INISGBuffer.RelativeSignalValue4 = stoi(Value);
    }

    if (Key.find(RelativeSignalName5) != std::string::npos)
    {
        INISGBuffer.RelativeSignalName5 = Value;
    }
    if (Key.find(RelativeSignalValue5) != std::string::npos)
    {
        INISGBuffer.RelativeSignalValue5 = stoi(Value);
    }


    for (i = 0; i < INISGBuffer.SGElement.size(); i++)
    {
        stringbuffer = lowerChange(INISGBuffer.SGElement[i].SName);

        if (Key.find(stringbuffer) != std::string::npos)
        {
            if (stringbuffer.find(s_chks) != std::string::npos)
            {
                INISGBuffer.ChksDTC = stoi(Value, nullptr, 16);
            }
            if (stringbuffer.find(s_cntr) != std::string::npos)
            {
                INISGBuffer.CntrDTC = stoi(Value, nullptr, 16);
            }
        }
    }
}

//解析数据库，将数据库内容存放到signalInfo中，通道信息存放到MSGID
void GetInfoByDataBase(std::vector<Tsignalinfo>& signalInfo_Func, std::vector<TMSGID>& MSGID_Func, std::vector<TDBFrameProperties>& framebuffer)
{
    int i = 0, j = 0;
    s32 DBCount = 0;
    TDBProperties DBPro;
    Tsignalinfo signalBuffer;
    TDBSignalProperties sgn;
    TDBFrameProperties fme;

    app.db_get_can_database_count(&DBCount);
    if (DBCount == 0)
    {
        printf("未加载CAN数据库，请检查");
        //TestInfoPrintFail("未加载CAN数据库，请检查");
        //TestTerminate();
    }

    MSGID_Func.resize(DBCount);

    for (i = 0; i < DBCount; i++)
    {
        DBPro.FDBIndex = i;
        app.db_get_can_database_properties_by_index(&DBPro);

        MSGID_Func[i].channel = DBPro.FSupportedChannelMask - 1;

        //获取数据库中的所有信号存在sgnBuffer中
        signalBuffer.sBuffer.clear();
        for (j = 0; j < DBPro.FSignalCount; j++)
        {
            app.db_get_can_signal_properties_by_db_index(DBPro.FDBIndex, j, &sgn);

            signalBuffer.sBuffer.push_back(sgn);


        }
        signalBuffer.channel = DBPro.FSupportedChannelMask - 1;

        //遍历报文进行存储
        for (j = 0; j < DBPro.FFrameCount; j++)
        {
            app.db_get_can_frame_properties_by_db_index(DBPro.FDBIndex, j, &fme);

            framebuffer.push_back(fme);
        }

        signalInfo_Func.push_back(signalBuffer);

    }
}

void GetInfoByDataBase_ByNetwork(std::vector<Tsignalinfo>& signalInfo_Func, std::vector<TMSGID>& MSGID_Func, std::vector<TDBFrameProperties>& framebuffer, std::string Network)
{
    int i = 0, j = 0;
    s32 DBCount = 0;
    TDBProperties DBPro;
    Tsignalinfo signalBuffer;
    TDBSignalProperties sgn;
    TDBFrameProperties fme;

    app.db_get_can_database_count(&DBCount);
    if (DBCount == 0)
    {
        printf("未加载CAN数据库，请检查");
        //TestInfoPrintFail("未加载CAN数据库，请检查");
        //TestTerminate();
    }

    MSGID_Func.resize(1);
    printf("DBCount=%d", DBCount);
    for (i = 0; i < DBCount; i++)
    {
        
        DBPro.FDBIndex = i;
        app.db_get_can_database_properties_by_index(&DBPro);
       
        //只解析其中一个网络的信号
        //if (Network == DBPro.FName)
        if(strcmp(Network.c_str(), DBPro.FName)==0)
        {
            //printf("Network == DBPro.FName=%s", DBPro.FName);
            //MSGID_Func[0].channel = DBPro.FSupportedChannelMask - 1;
            MSGID_Func[0].channel = NetworkCluster[DBPro.FName];

            //获取数据库中的所有信号存在sgnBuffer中
            signalBuffer.sBuffer.clear();
            //printf("Network=%s,FName=%s", Network.c_str(), DBPro.FName);
            for (j = 0; j < DBPro.FSignalCount; j++)
            {
                app.db_get_can_signal_properties_by_db_index(DBPro.FDBIndex, j, &sgn);
                //log_hint("            Signal %d %s Factor=%f, Offset=%f", j + 1, sgn.FName, sgn.FCANSignal.FFactor, sgn.FCANSignal.FOffset);
                signalBuffer.sBuffer.push_back(sgn);
                //printf("add signalBuffer,sgn is %s", sgn.FName);

            }
            //signalBuffer.channel = DBPro.FSupportedChannelMask - 1;
            signalBuffer.channel = NetworkCluster[DBPro.FName];
            log_hint("DBPro.FName=%s,signalBuffer.channel=%d", DBPro.FName,signalBuffer.channel);
            //遍历报文进行存储

            for (j = 0; j < DBPro.FFrameCount; j++)
            {
                app.db_get_can_frame_properties_by_db_index(DBPro.FDBIndex, j, &fme);
                //log_hint("        can_frame %d %s Id=0x%03X DLC=%d", j+1, fme.FName, fme.FCANIdentifier, fme.FCANDLC);
                framebuffer.push_back(fme);
                //if (strcmp(Network.c_str(), DBPro.FName))
                //if (strcmp(DBPro.FName,"ZCUD_CAN1")==0) {
                //    log_hint("        can_frame %d %s Id=0x%03X DLC=%d", j + 1, fme.FName, fme.FCANIdentifier, fme.FCANDLC);
                //}
                //printf("add framebuffer,[%s]fme is %s,ID is %d", DBPro.FName, fme.FName, fme.FCANIdentifier);
            }
            
            signalInfo_Func.push_back(signalBuffer);
        }
    }
    printf("signalInfo_Func=%d", signalInfo_Func.size());
}

//设置RX报文周期
void SetRxPeriod_DBC(std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX_Func)
{
    int i = 0, j = 0;
    log_hint("DUT_def_E2E_SG_RX_Func.size()=%d", DUT_def_E2E_SG_RX_Func.size());
    for (i = 0; i < DUT_def_E2E_SG_RX_Func.size(); i++)
    {
        double PeriodMs = 0;

        if (DUT_def_E2E_SG_RX_Func[i].SGInstance.size() < 1)
        {
            log_hint("DUT_def_E2E_SG_RX[%d].MsgID = 0x%x，此DUT_def_E2E_SG_RX[%d].SGInstance.size = 0", i, DUT_def_E2E_SG_RX_Func[i].MsgID, i);
            //TestInfoPrintWarning("DUT_def_E2E_SG_RX[%d].MsgID = 0x%x，此DUT_def_E2E_SG_RX[%d].SGInstance.size = 0", i, DUT_def_E2E_SG_RX_Func[i].MsgID, i);
            continue;
        }

        PeriodMs = DUT_def_E2E_SG_RX_Func[i].SGInstance[0].PeriodMs;
        //printf("PeriodMs=%d", PeriodMs);
        for (j = 0; j < DUT_def_E2E_SG_RX_Func[i].SGInstance.size(); j++)
        {
            
            if (DUT_def_E2E_SG_RX_Func[i].SGInstance[j].PeriodMs != PeriodMs)
            {
                //TestInfoPrintWarning("解析有误，%s信号组的周期与（DUT_def_E2E_SG_RX[%d].SGInstance[0]）%s信号组的周期不同，请检查ini和arxml", DUT_def_E2E_SG_RX_Func[i].SGInstance[j].SGName.c_str(), i, DUT_def_E2E_SG_RX_Func[i].SGInstance[0].SGName.c_str());
                log_hint("解析有误，%s信号组的周期与（DUT_def_E2E_SG_RX[%d].SGInstance[0]）%s信号组的周期不同，请检查ini和arxml", DUT_def_E2E_SG_RX_Func[i].SGInstance[j].SGName.c_str(), i, DUT_def_E2E_SG_RX_Func[i].SGInstance[0].SGName.c_str());
                break;
            }
            if (PeriodMs == -1)
            {
                log_hint("解析有误，信号组%s的周期为-1，arxml中未检测到此信号组对应pdu的周期", DUT_def_E2E_SG_RX_Func[i].SGInstance[j].SGName.c_str());
                //TestInfoPrintWarning("解析有误，信号组%s的周期为-1，arxml中未检测到此信号组对应pdu的周期", DUT_def_E2E_SG_RX_Func[i].SGInstance[j].SGName.c_str());
                break;
            }
        }
        DUT_def_E2E_SG_RX_Func[i].PeriodMs = PeriodMs;
    }
}

//
void GetSGFromArxml(std::vector<TSingleGroup>& SGGetFromArxml_Func, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX_Func)
{
    int i = 0, j = 0, k = 0, m = 0, n = 0;
    std::vector<TSingleGroup>SGGetFromArxmlBuffer;
    TARXMLE2ESG DUT_def_E2E_SGBuffer;
    int flag = 0;
    printf("SGGetFromArxml_Func.size()=%d", SGGetFromArxml_Func.size());
    for (i = 0; i < SGGetFromArxml_Func.size(); i++)
    {
        //遍历TX_SG下信号组中的信号
        for (j = 0; j < SGGetFromArxml_Func[i].SGElement.size(); j++)
        {
            //遍历sgnBuffer与  TX_SG下信号组中的信号名  进行比对
            for (k = 0; k < signalInfo.size(); k++)
            {
                for (m = 0; m < signalInfo[k].sBuffer.size(); m++)
                {
                    if (signalInfo[k].sBuffer[m].FName == SGGetFromArxml_Func[i].SGElement[j].SName)
                    {
                        int No = 0;

                        SGGetFromArxml_Func[i].SGElement[j].SName_low = lowerChange(SGGetFromArxml_Func[i].SGElement[j].SName);
                        SGGetFromArxml_Func[i].msgid = signalInfo[k].sBuffer[m].FParentFrameId;
                        SGGetFromArxml_Func[i].bustype = signalInfo[k].sBuffer[m].FSignalType;
                        SGGetFromArxml_Func[i].channel = signalInfo[k].channel;

                        //DLC赋值
                        for (n = 0; n < DBFrameBuffer.size(); n++)
                        {
                            if (DBFrameBuffer[n].FCANIdentifier == SGGetFromArxml_Func[i].msgid)
                            {
                                SGGetFromArxml_Func[i].DLC = DBFrameBuffer[n].FCANDLC;
                            }
                        }

                        for (n = 0; n < MSGID.size(); n++)
                        {
                            if (signalInfo[k].channel == MSGID[n].channel)
                            {
                                if (find(MSGID[n].IDBuffer.begin(), MSGID[n].IDBuffer.end(), SGGetFromArxml_Func[i].msgid) == MSGID[n].IDBuffer.end())
                                {
                                    MSGID[n].IDBuffer.push_back(SGGetFromArxml_Func[i].msgid);
                                }
                            }
                        }

                        //判断tsmaster加载的arxml中的 sgnBuffer的总线类型FSignalType，可能是can/lin/fr等，根据不同类型，将对应信号的FStartBit，FLength，FIsIntel，赋给 python解析的 TX_SG下信号组中的信号
                        switch (signalInfo[k].sBuffer[m].FSignalType)
                        {
                        case stCANSignal:

                            SGGetFromArxml_Func[i].SGElement[j].startbit = signalInfo[k].sBuffer[m].FCANSignal.FStartBit;
                            SGGetFromArxml_Func[i].SGElement[j].Len = signalInfo[k].sBuffer[m].FCANSignal.FLength;
                            SGGetFromArxml_Func[i].SGElement[j].isIntel = signalInfo[k].sBuffer[m].FCANSignal.FIsIntel;
                            /*if (SGGetFromArxml_Func[i].SGElement[j].SName.find(S_Chks) != std::string::npos)
                            {
                                TestInfoPrintWarning("%s的startbit = %d,Len = %d", SGGetFromArxml_Func[i].SGElement[j].SName.c_str(), SGGetFromArxml_Func[i].SGElement[j].startbit, SGGetFromArxml_Func[i].SGElement[j].Len);
                            }*/
                            break;
                        case stLINSignal:
                            SGGetFromArxml_Func[i].SGElement[j].startbit = signalInfo[k].sBuffer[m].FLINSignal.FStartBit;
                            SGGetFromArxml_Func[i].SGElement[j].Len = signalInfo[k].sBuffer[m].FLINSignal.FLength;
                            SGGetFromArxml_Func[i].SGElement[j].isIntel = signalInfo[k].sBuffer[m].FLINSignal.FIsIntel;
                            break;
                        case stFlexRay:
                            SGGetFromArxml_Func[i].SGElement[j].startbit = signalInfo[k].sBuffer[m].FFlexRaySignal.FStartBit;
                            SGGetFromArxml_Func[i].SGElement[j].Len = signalInfo[k].sBuffer[m].FFlexRaySignal.FLength;
                            SGGetFromArxml_Func[i].SGElement[j].isIntel = signalInfo[k].sBuffer[m].FFlexRaySignal.FIsIntel;
                            break;
                        default:
                            break;
                        }
                        //有同名但通道不同的信号，对应id等属性都不同，此处先后赋值给SGRXGetFromArxml[i]，会覆盖掉;用SGRXGetFromArxmlBuffer暂存完整的
                        /*for (No = 0; No < SGGetFromArxmlBuffer.size(); No++)
                        {
                            if (SGGetFromArxmlBuffer[No].SGName == SGGetFromArxml_Func[i].SGName)
                            {
                                if (SGGetFromArxmlBuffer[No].channel == SGGetFromArxml_Func[i].channel)
                                {
                                    flag = 1;
                                    break;
                                }
                            }

                        }*/
                        /*if (No == SGGetFromArxmlBuffer.size())
                        {
                            SGGetFromArxmlBuffer.push_back(SGGetFromArxml_Func[i]);
                        }*/
                        /*if (flag == 0)
                        {
                            TestInfoPrintWarning("0--SGGetFromArxml_Func[%d].SGName = %s",i, SGGetFromArxml_Func[i].SGName.c_str());

                            SGGetFromArxmlBuffer.push_back(SGGetFromArxml_Func[i]);
                        }
                        else
                        {
                            TestInfoPrintWarning("1--SGGetFromArxml_Func[%d].SGName = %s",i, SGGetFromArxml_Func[i].SGName.c_str());
                            flag = 0;
                        }*/
                    }
                }
            }

        }

        SGGetFromArxmlBuffer.push_back(SGGetFromArxml_Func[i]);

    }

    /*for (i = 0; i < SGGetFromArxmlBuffer.size(); i++)
    {
        for (j = 0; j < SGGetFromArxmlBuffer[i].SGElement.size(); j++)
        {

                if (SGGetFromArxmlBuffer[i].SGElement[j].SName.find(S_Chks) != std::string::npos)
                {
                    TestInfoPrintWarning("---%s startbit = %d,len = %d", SGGetFromArxmlBuffer[i].SGElement[j].SName.c_str(), SGGetFromArxmlBuffer[i].SGElement[j].startbit, SGGetFromArxmlBuffer[i].SGElement[j].Len);
                }

        }

    }*/

    //根据id进行遍历，将报文id相同的信号组放到一起,；同一ID和CH的信号组（SGGetFromArxmlBuffer[k]），放到同一个DUT_def_E2E_SGBuffer.SGInstance中
    for (i = 0; i < MSGID.size(); i++)
    {
        for (j = 0; j < MSGID[i].IDBuffer.size(); j++)
        {
            DUT_def_E2E_SGBuffer.SGInstance.clear();
            DUT_def_E2E_SGBuffer.MsgID = 0;

            for (k = 0; k < SGGetFromArxmlBuffer.size(); k++)
            {
                //判断id和通道相同
                if ((SGGetFromArxmlBuffer[k].msgid == MSGID[i].IDBuffer[j]) && (SGGetFromArxmlBuffer[k].channel == MSGID[i].channel))
                {
                    DUT_def_E2E_SGBuffer.MsgID = MSGID[i].IDBuffer[j];
                    DUT_def_E2E_SGBuffer.channel = MSGID[i].channel;
                    DUT_def_E2E_SGBuffer.BusType = SGGetFromArxmlBuffer[k].bustype;
                    //SGTXGetFromArxml填至DUT_def_E2E_SGBuffer中
                    DUT_def_E2E_SGBuffer.SGInstance.push_back(SGGetFromArxmlBuffer[k]);

                }
            }
            DUT_def_E2E_SG_TX_Func.push_back(DUT_def_E2E_SGBuffer);
        }
    }

    for (i = 0; i < MSGID.size(); i++)
    {
        MSGID[i].IDBuffer.clear();
    }

    /*for (i=0;i< DUT_def_E2E_SG_TX_Func.size();i++)
    {
        for (j = 0; j < DUT_def_E2E_SG_TX_Func[i].SGInstance.size(); j++ )
        {
            for (k = 0; k < DUT_def_E2E_SG_TX_Func[i].SGInstance[j].SGElement.size(); k++)
            {
                if (DUT_def_E2E_SG_TX_Func[i].SGInstance[j].SGElement[k].SName.find(S_Chks) != std::string::npos)
                {
                    TestInfoPrintWarning("%s startbit = %d", DUT_def_E2E_SG_TX_Func[i].SGInstance[j].SGElement[k].SName.c_str(), DUT_def_E2E_SG_TX_Func[i].SGInstance[j].SGElement[k].startbit);
                }
            }
        }

    }*/
}

//对信号组中信号重新进行排序（首个信号为CRC），确定各个信号的startbyte，bytelen，和总的数据（RearrData）长度
void SignalRearrangement(std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_Func)
{
    int i, j, k = 0;
    int SignalLen = 0;
    int SignalLenBuffer = 0;

    TSingle SingleBuffer_Func;

    //重排序时用于遍历
    int RearrNo = 0;

    //TX
    for (i = 0; i < DUT_def_E2E_SG_Func.size(); i++)
    {
        for (j = 0; j < DUT_def_E2E_SG_Func[i].SGInstance.size(); j++)
        {

            //排序，先将CheckSum放到首位
            for (k = 0; k < DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement.size(); k++)
            {
                if (DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k].SName.find(S_Chks) != std::string::npos)
                {
                    SingleBuffer_Func = DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[0];
                    DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[0] = DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k];
                    DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k] = SingleBuffer_Func;
                }
                if (DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k].SName.find(S_Cntr) != std::string::npos)
                {
                    SingleBuffer_Func = DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[1];
                    DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[1] = DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k];
                    DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k] = SingleBuffer_Func;
                }
            }
            //对剩余信号进行排序
            for (k = 2; k < DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement.size(); k++)
            {
                for (RearrNo = k + 1; RearrNo < DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement.size(); RearrNo++)
                {
                    //信号排序不区分大小写
                    if (DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k].SName_low > DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[RearrNo].SName_low)
                    {
                        SingleBuffer_Func = DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k];
                        DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k] = DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[RearrNo];
                        DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[RearrNo] = SingleBuffer_Func;
                    }
                    /*if (DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k].SName > DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[RearrNo].SName)
                    {
                        SingleBuffer_Func = DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k];
                        DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k] = DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[RearrNo];
                        DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[RearrNo] = SingleBuffer_Func;

                    }*/
                }
            }

            //确定重排序后信号的bytelen，起始byte
            for (k = 1; k < DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement.size(); k++)
            {
                SignalLenBuffer = ((DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k].Len - 1) / 8 + 1) * 8;//长度填充至8bit倍数

                DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k].RearrByteLen = SignalLenBuffer / 8;
                DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k].RearrStartByte = SignalLen + DataIDLen;

                SignalLen += DUT_def_E2E_SG_Func[i].SGInstance[j].SGElement[k].RearrByteLen;
            }

            //排序后的信号组的数据长度等于所有信号的长度和
            DUT_def_E2E_SG_Func[i].SGInstance[j].RearrData.resize(SignalLen + DataIDLen);
            SignalLen = 0;

        }
    }
}

//获取测试参数，解析INI--------------------------------------------------------------------------待和许工确认
int GetParameter(std::string inipath)
{
    std::string line;
    std::ifstream fs(inipath.c_str());
    std::string Key;
    std::string Value;

    if (!fs.is_open())
    {
        printf("打开ini文件 %s 失败", inipath.c_str());
        //TestInfoPrintFail("打开ini文件 %s 失败", inipath.c_str());
        //TestTerminate();
    }

    //逐行获取
    while (getline(fs, line))
    {
        //跳过空行
        if (line.empty())
        {
            continue;
        }

        //清除line中的空格
        RegexSpace(line, "");

        //判断为ini的节
        if ((line[0] == '[' && line[line.size() - 1] == ']'))
        {

        }
        //判断为ini的键和具体的值
        else
        {
            //根据‘=’划分键和值
            std::string::size_type Equalpos = line.find('=');

            if (Equalpos == std::string::npos)
            {
                continue;
            }

            Key = line.substr(0, Equalpos);
            Value = line.substr(Equalpos + 1, line.size() - 1);

            GetINIValue(Key, Value);
        }
    }

    SetParametersFromINI();

    fs.close();
    return 0;
}
//将字符串中的字符全部转为小写return

//获取E2E相关信息
void E2EInfoGet()
{
    s_chks = lowerChange(S_Chks);
    s_cntr = lowerChange(S_Cntr);

    /*-------生成/解析INI文件-------*/
    ParsingINI E2EINI;
    char* tempConfigPath = nullptr;
    app.get_configuration_file_path(&tempConfigPath);
    printf("config abs path is %s", tempConfigPath);
    sprintf(E2EPythonPath, "%s%s", tempConfigPath, pythonpath);
    printf("python path is %s", E2EPythonPath);
    //sprintf(E2EPythonPath, "C:\\Users\\OrKKK\\Desktop\\GEEA3.0_Diag_Test\\Toolbox\\GEELY3\\DB\\ParsingARXML.py");
    //执行python脚本，先从ARXML中提取信息（dataid等），此处参数请参考TSMaster中函数介绍，此处重点参数为路径
    //if (gCEMFlg == 1) {
    //    printf("解析ZCUDArxml");
    //    app.execute_python_script(E2EPythonPath, "ZCUD", true, false, &pythonResult);
    //}
    //else {
    //    printf("解析ZCUPArxml");
    //    app.execute_python_script(E2EPythonPath, "ZCUP", true, false, &pythonResult);
    //}
    app.execute_python_script(E2EPythonPath,"", true, false, &pythonResult);
    //解析python生成的ini文件，将内容赋给SGTXGetFromArxml/SGRXGetFromArxml
    //sprintf(E2EINIPath, "C:\\Users\\OrKKK\\Desktop\\GEEA3.0_Diag_Test\\Toolbox\\GEELY3\\DB\\E2EInfo.ini");
    sprintf(E2EINIPath, "%s%s", tempConfigPath, E2E_ini_path);
    printf("ini path is %s", E2EINIPath);
    E2EINI.setINIPath(E2EINIPath);
    E2EINI.LoadINI();
    /*结束-------生成/解析INI文件-------*/
    memset(DUT_def_E2E_SG_TX_2D_Array, 0, sizeof(DUT_def_E2E_SG_TX_2D_Array));
    memset(DUT_def_E2E_SG_RX_2D_Array, 0, sizeof(DUT_def_E2E_SG_RX_2D_Array));
    if(gCEMFlg == 1){
        for (s32 t = 0; t < 9; t++) {

            //VariableINIT();
            char Name[500] = "";

            if (t == 0) strcpy(Name, "ZCU_CANFD2");
            else if (t == 1) strcpy(Name, "ZCU_CANFD1");
            else if (t == 2) strcpy(Name, "ChassisCANFD2");
            else if (t == 3) strcpy(Name, "ZCUD_CAN1");
            else if (t == 4) strcpy(Name, "PropulsionCANFD");
            else if (t == 5) strcpy(Name, "ChassisCANFD1");
            //else if (t == 6) strcpy(Name, "BodyExposedCAN");
            else if (t == 6) strcpy(Name, "ZCUD_CANFD3");
            else if (t == 7) strcpy(Name, "ZCUD_CANFD4");
          
            //printf("t=%d,NetworkName=%s,NetworkCluster[%d]=%d", t, Name,t,NetworkCluster[Name]);
            printf("t=%d,NetworkName=%s", t, Name);
            strcpy(DUT_def_E2E_SG_TX_2D_Array[t].FName, Name);
            strcpy(DUT_def_E2E_SG_RX_2D_Array[t].FName, Name);
            //处理数据库信息
            signalInfo.clear();
            MSGID.clear();
            DBFrameBuffer.clear();
            GetInfoByDataBase_ByNetwork(signalInfo, MSGID, DBFrameBuffer, Name);
            //GetInfoByDataBase(signalInfo, MSGID, DBFrameBuffer);

            //解析加载的数据库
            int i, j, k = 0;
            int m, n = 0;

            /*--------比对信号名称, 获取信号总线类型，获取信号len，startbit--------*/

            GetSGFromArxml(SGTXGetFromArxml, DUT_def_E2E_SG_TX_2D_Array[t].DUT_def_E2E_SG_TX);
            GetSGFromArxml(SGRXGetFromArxml, DUT_def_E2E_SG_RX_2D_Array[t].DUT_def_E2E_SG_RX);

            //设置RX报文周期
            SetRxPeriod_DBC(DUT_def_E2E_SG_RX_2D_Array[t].DUT_def_E2E_SG_RX);

            /*结束--------比对信号名称, 获取信号总线类型，获取信号len，startbit--------*/

            //对信号组中信号重新进行排序
            SignalRearrangement(DUT_def_E2E_SG_TX_2D_Array[t].DUT_def_E2E_SG_TX);
            SignalRearrangement(DUT_def_E2E_SG_RX_2D_Array[t].DUT_def_E2E_SG_RX);
        }
    }
    else {
        for (s32 t = 0; t < 4; t++) {

            //VariableINIT();
            char Name[500] = "";
            if (t == 0) strcpy(Name, "ZCU_CANFD2");
            else if (t == 1) strcpy(Name, "ZCU_CANFD1");
            else if (t == 2) strcpy(Name, "ZCUP_CANFD5");
            else if (t == 3) strcpy(Name, "ZCUP_CAN1");
            //printf("t=%d,NetworkName=%s,NetworkCluster[%d]=%d", t, Name,t,NetworkCluster[Name]);
            printf("t=%d,NetworkName=%s", t, Name);
            strcpy(DUT_def_E2E_SG_TX_2D_Array[t].FName, Name);
            strcpy(DUT_def_E2E_SG_RX_2D_Array[t].FName, Name);
            //处理数据库信息
            signalInfo.clear();
            MSGID.clear();
            DBFrameBuffer.clear();
            GetInfoByDataBase_ByNetwork(signalInfo, MSGID, DBFrameBuffer, Name);
            //GetInfoByDataBase(signalInfo, MSGID, DBFrameBuffer);

            //解析加载的数据库
            int i, j, k = 0;
            int m, n = 0;

            /*--------比对信号名称, 获取信号总线类型，获取信号len，startbit--------*/

            GetSGFromArxml(SGTXGetFromArxml, DUT_def_E2E_SG_TX_2D_Array[t].DUT_def_E2E_SG_TX);
            GetSGFromArxml(SGRXGetFromArxml, DUT_def_E2E_SG_RX_2D_Array[t].DUT_def_E2E_SG_RX);

            //设置RX报文周期
            SetRxPeriod_DBC(DUT_def_E2E_SG_RX_2D_Array[t].DUT_def_E2E_SG_RX);

            /*结束--------比对信号名称, 获取信号总线类型，获取信号len，startbit--------*/

            //对信号组中信号重新进行排序
            SignalRearrangement(DUT_def_E2E_SG_TX_2D_Array[t].DUT_def_E2E_SG_TX);
            SignalRearrangement(DUT_def_E2E_SG_RX_2D_Array[t].DUT_def_E2E_SG_RX);
        }
    }
    printf("out");
    ////处理数据库信息
    //GetInfoByDataBase_ByNetwork(signalInfo, MSGID, DBFrameBuffer, NetworkName);
    ////GetInfoByDataBase(signalInfo, MSGID, DBFrameBuffer);

    ////解析加载的数据库
    //int i, j, k = 0;
    //int m, n = 0;

    ///*--------比对信号名称, 获取信号总线类型，获取信号len，startbit--------*/

    //GetSGFromArxml(SGTXGetFromArxml, DUT_def_E2E_SG_TX);
    //GetSGFromArxml(SGRXGetFromArxml, DUT_def_E2E_SG_RX);

    ////设置RX报文周期
    //SetRxPeriod_DBC(DUT_def_E2E_SG_RX);

    ///*结束--------比对信号名称, 获取信号总线类型，获取信号len，startbit--------*/

    ////对信号组中信号重新进行排序
    //SignalRearrangement(DUT_def_E2E_SG_TX);
    //SignalRearrangement(DUT_def_E2E_SG_RX);
}

//短路
void Short_CANH_To_CANL()
{
    Short_CANH_To_CANL_Devices();
}

//短路恢复
void ShortRecovery()
{
    ShortRecovery_Devices();
}
//等待报文，等待时间waittime，Number作为第N帧报文，从1开始计数
int MessageWaitFromNow(int Number, int waittime)
{
    int startsize = RX_E2EMsg.size();

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    while (startsize - RX_E2EMsg.size() < Number)
    {
        if (std::chrono::steady_clock::now() - start > std::chrono::milliseconds(waittime))
        {
            ////ExceptResult("所测%dms时间内，控制器发出第%d帧所等报文", waittime, Number);
            sprintf(actualResponse, "所测%dms时间内，控制器未发出第%d帧所等报文", waittime, Number);
            TestReport_HTML_StepDescription(actualResponse, Fail);
            //TestInfoPrintFail("所测%dms时间内，控制器未发出第%d帧所等报文", waittime, Number);
            break;
        }
        app.wait(0, "");
        //TestTerminateCheck();
    }

    return RX_E2EMsg.size();
}

//等待DUT恢复通信，调用WaitFlag函数，在CheckTime内，DUT只要发送出报文，即认为恢复通信
void WaitDUTCommunicationRecovery()
{
    int res = 0;

    ShortRecoveryCheckFlag = 1;
    res = WaitFlag(ShortRecoveryFlag, CheckTime);

    if (res == 0)
    {
        ////ExceptResult("短路故障移除后60+%ds内DUT恢复通信", CheckTime);
        sprintf(actualResponse, "短路故障移除后60+%ds内DUT未恢复通信", CheckTime/1000);
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail("短路故障移除后60+%ds内DUT未恢复通信", CheckTime);
    }
    else
    {
        sprintf(actualResponse, "短路故障移除后60+%ds内DUT恢复通信", CheckTime/1000);
        TestReport_HTML_StepDescription(actualResponse, Pass);
        //TestInfoPrintPass("短路故障移除后60+%ds内DUT恢复通信", CheckTime);
    }
}

//使用14服务清除DTC
void SID_14_ClearDTC()
{
    u8 DiagTXData[4] = { 0x14,0xff,0xff,0xff };

    rtlUIDiagnostics.tstp_send_request(udsHandle, DiagTXData, sizeof(DiagTXData) / sizeof(DiagTXData[0]));
}

//使用19服务读取DTC，掩码为Mask，读取的数据存放到DiagResData中
void SID_19_ReadDTCByMask(u8 Mask)
{
    int DiagDataLenMAX = C_DiagDataLenMAX;

    u8 DiagTXData[3] = { 0x19,0x02 };

    DiagTXData[2] = Mask;

    rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, DiagTXData, sizeof(DiagTXData) / sizeof(DiagTXData[0]), DiagResData, &DiagDataLenMAX);

    //app.wait(1000, "");
    g_DiagDataLenMAX = DiagDataLenMAX;
}

//存储DTC，将DiagResData中的DTC和Mask，存到DTCData中
void storgeDTC()
{
    u8 ReadDTCByte = 0x59;
    u8 ReadDTCSubByte = 0x02;
    int i = 0;

    DTCData.clear();

    if (DiagResData[0] == ReadDTCByte && DiagResData[1] == ReadDTCSubByte)
    {
        int DTCNo = (g_DiagDataLenMAX - 3) / 4;

        for (i = 0; i < DTCNo; i++)
        {
            DTCBuffer.DTC[0] = DiagResData[i * 4 + 3];
            DTCBuffer.DTC[1] = DiagResData[i * 4 + 4];
            DTCBuffer.DTC[2] = DiagResData[i * 4 + 5];
            DTCBuffer.Mask = DiagResData[i * 4 + 6];

            DTCData.push_back(DTCBuffer);
        }

    }
    else
    {
        //ExceptResult("DUT响应正确，期望为0x%02x 0x%02x", ReadDTCByte, ReadDTCSubByte);
        sprintf(actualResponse, "DUT响应错误，响应为0x%02x 0x%02x，期望为0x%02x 0x%02x", DiagResData[0], DiagResData[1], ReadDTCByte, ReadDTCSubByte);
        TestReport_HTML_StepDescription(actualResponse, Fail);
        //TestInfoPrintFail("DUT响应错误，响应为0x%02x 0x%02x，期望为0x%02x 0x%02x", DiagResData[0], DiagResData[1], ReadDTCByte, ReadDTCSubByte);
    }

}

//检测是否没有记录DTC，返回对应掩码，如未记录，返回值为0
u8 CheckDTCNotRecord(u32 DTC)
{
    int i = 0;
    u8 res = 0;

    for (i = 0; i < DTCData.size(); i++)
    {
        if (((DTC >> 16) & 0xff) == DTCData[i].DTC[2] && ((DTC >> 8) & 0xff) == DTCData[i].DTC[1] && (DTC & 0xff) == DTCData[i].DTC[0])
        {
            res = DTCData[i].Mask;

            //ExceptResult("DUT未记录DTC 0x%X", DTC);
            //TestInfoPrintFail("DUT记录了DTC 0x%X，状态位为0x%02x", DTC, res);
            sprintf(actualResponse, "DUT记录了DTC 0x%X，状态位为0x%02x", DTC, res);
            TestReport_HTML_StepDescription(actualResponse, Pass);
        }
    }

    if (res == 0)
    {
        sprintf(actualResponse, "DUT未记录DTC 0x%X", DTC);
        TestReport_HTML_StepDescription(actualResponse, Pass);
        //TestInfoPrintPass("DUT未记录DTC 0x%X", DTC);
    }

    return res;

}

//检测是否记录DTC，返回对应掩码，如未记录，返回值为0
u8 CheckDTCRecord(u32 DTC)
{
    int i = 0;
    u8 res = 0;

    for (i = 0; i < DTCData.size(); i++)
    {
        if (((DTC >> 16) & 0xff) == DTCData[i].DTC[0] && ((DTC >> 8) & 0xff) == DTCData[i].DTC[1] && (DTC & 0xff) == DTCData[i].DTC[2])
        {
            res = DTCData[i].Mask;

            //TestInfoPrintPass("DUT记录了DTC 0x%X，状态位为0x%02x", DTC, res);
            sprintf(actualResponse, "DUT记录了DTC 0x%X，状态位为0x%02x", DTC, res);
            TestReport_HTML_StepDescription(actualResponse, Pass);
        }
    }

    if (res == 0)
    {
        char buffer[1024] = "";

        //ExceptResult("测试信号组%s，DUT记录DTC 0x%X", TestSGName.c_str(), DTC);
        sprintf(actualResponse, "测试信号组%s，DUT未记录DTC 0x%X", TestSGName.c_str(), DTC);
        TestReport_HTML_StepDescription(actualResponse, Pass);
        //TestInfoPrintFail("测试信号组%s，DUT未记录DTC 0x%X", TestSGName.c_str(), DTC);
        sprintf(buffer, "DUT记录的DTC和状态位依次为");
        TestReport_HTML_StepDescription(buffer, Info);
        for (i = 0; i < DTCData.size(); i++)
        {
            sprintf(actualResponse, "DTC--%02X%02X%02X state--%02X", DTCData[i].DTC[0], DTCData[i].DTC[1], DTCData[i].DTC[2], DTCData[i].Mask);
            TestReport_HTML_StepDescription(actualResponse, Info);
            //TestInfoPrint("DTC--%02X%02X%02X state--%02X", DTCData[i].DTC[0], DTCData[i].DTC[1], DTCData[i].DTC[2], DTCData[i].Mask);
            //sprintf(buffer, "%s DTC--%02X%02X%02X state--%02X", buffer,DTCData[i].DTC[2], DTCData[i].DTC[1], DTCData[i].DTC[0], DTCData[i].Mask);

        }
        //TestInfoPrintFail("%s", buffer);
    }

    return res;
}

/*--------------------------------------NM--------------------------------------*/

//清空所有vector数组
void ClearAllNMVector()
{
    u8 m = 1;
    for (m; m < bus_count; m++)
    {
        ALLRxMsg[m].clear();
        DUT_NMRxMsg[m].clear();
        DUT_APPRxMsg[m].clear();
        ErrorRxMsg[m].clear();
        DiagRxMsg[m].clear();
        OutErrorAndDiagRxMsg[m].clear();

        ALLTxMsg[m].clear();
        DUT_NMTxMsg[m].clear();
        DUT_APPTxMsg[m].clear();
        ErrorTxMsg[m].clear();
        DiagTxMsg[m].clear();
        OutErrorTxMsg[m].clear();

    }
}

//byte2:16-23,byte3:24-31,32-39,40-47,48-55,56-63//geely 3.0就到47，后面是唤醒原因byte
void handle_all_pnc2bytembitn_and_mapframes()
{
    u8 i = 0;
    u8 bytem = 0;
    u8 bitn = 0;
    u8 m = 0;
    for (m; m < bus_count; m++)
    {
        for (i = 0; i < pnc_max_count_in_one_bus; i++)
        {
            //if (bus_configuration[m].relevant_pnc[i] >= 56)
            //{
            //	bytem = 7;
            //	bitn = bus_configuration[m].relevant_pnc[i] % 8;
            //	bus_configuration[m].pnc2bytembitn[bytem][bitn] = 1;

            //}
            //else if (bus_configuration[m].relevant_pnc[i] >= 48)
            //{
            //	bytem = 6;
            //	bitn = bus_configuration[m].relevant_pnc[i] % 8;
            //	bus_configuration[m].pnc2bytembitn[bytem][bitn] = 1;

            //}
            if (bus_configuration[m].relevant_pnc[i] >= 40)
            {
                bytem = 5;
                bitn = bus_configuration[m].relevant_pnc[i] % 8;
                bus_configuration[m].pnc2bytembitn[bytem][bitn] = 1;

            }
            else if (bus_configuration[m].relevant_pnc[i] >= 32)
            {
                bytem = 4;
                bitn = bus_configuration[m].relevant_pnc[i] % 8;
                bus_configuration[m].pnc2bytembitn[bytem][bitn] = 1;

            }
            else if (bus_configuration[m].relevant_pnc[i] >= 24)
            {
                bytem = 3;
                bitn = bus_configuration[m].relevant_pnc[i] % 8;
                bus_configuration[m].pnc2bytembitn[bytem][bitn] = 1;
                //log("%d,j:%d,k:%d", pnc2bytembitn[bytem][bitn], bytem, bitn);

            }
            else if (bus_configuration[m].relevant_pnc[i] >= 16)
            {
                bytem = 2;
                bitn = bus_configuration[m].relevant_pnc[i] % 8;
                bus_configuration[m].pnc2bytembitn[bytem][bitn] = 1;
            }
            else if (bus_configuration[m].relevant_pnc[i] != 0)
            {
                //TestInfoPrintFail("错误的PNC码，NM PDU中Byte0为Source node id，Byte1为CBV位，后续为6个Byte均为PNC位，首个PNC值为16，最后PNC值为63");

                sprintf(actualResponse, "错误的PNC码，NM PDU中Byte0为Source node id，Byte1为CBV位，后续为6个Byte均为PNC位，首个PNC值为16，最后PNC值为47");
                TestReport_HTML_StepDescription(actualResponse, Fail);
            }
        }
    }

    //bus_configuration[1].pnc_MapFramesTx[24] = { 0x221,0x105,0x353 };
    //bus_configuration[1].pnc_MapFramesTx[28] = { 0x363,0x221,0x105, 0x353 };
    //bus_configuration[1].pnc_MapFramesTx[35] = { 0x221,0x105,0x353 };
    //bus_configuration[1].pnc_MapFramesTx[37] = { 0x221,0x105};
    //bus_configuration[1].NoMapFrames = { 0x243, 0x1C3, 0x3A3, 0x53, 0x61, 0x543, 0x345, };

    //bus_configuration[2].pnc_MapFramesTx[24] = { 0x29B, 0x11A,0x255,0x130, 0x31,0x139,0x41,0x76,0x39,0x12E };
    //bus_configuration[2].pnc_MapFramesTx[28] = { 0x31, 0x11A, 0x130, 0x39, 0x35, 0x81, 0x41B, 0x76, 0x29B, 0x41, 0x12E, 0x139, 0x255, 0x238 };
    //bus_configuration[2].pnc_MapFramesTx[35] = { 0x29B,0x31,0x41 };
    //bus_configuration[2].NoMapFrames = { 0x162,0x56, 0x285, 0x545, };




    //PNC25
    //pnc_MapFramesRx[24][0] = 0x233;
    //pnc_MapFramesRx[24][1] = 0x239;
    //pnc_MapFramesRx[24][2] = 0x279;
    //pnc_MapFramesRx[24][3] = 0x121;

    //这里希望自动输入所有DUT需要发送的周期报文
    //bus_configuration[m].pnc_MapFramesTx[24][0] = 0x2A9;
    //bus_configuration[m].pnc_MapFramesTx[24][1] = 0x2A6;
    //bus_configuration[m].pnc_MapFramesTx[24][2] = 0x2AC;
    //bus_configuration[m].pnc_MapFramesTx[24][3] = 0x421;
    //bus_configuration[m].pnc_MapFramesTx[24][4] = 0x365;
    //bus_configuration[m].pnc_MapFramesTx[24][5] = 0x370;
    //bus_configuration[m].pnc_MapFramesTx[24][6] = 0x2A0;
    //bus_configuration[m].pnc_MapFramesTx[24][7] = 0x2A3;

    ////PNC29
    ////pnc_MapFramesRx[28][0] = 0x233;
    ////pnc_MapFramesRx[28][1] = 0x239;
    ////pnc_MapFramesRx[28][2] = 0x279;
    ////pnc_MapFramesRx[28][3] = 0x76;

    //bus_configuration[m].pnc_MapFramesTx[28][0] = 0x2A0;
    //bus_configuration[m].pnc_MapFramesTx[28][1] = 0x2A3;
    //bus_configuration[m].pnc_MapFramesTx[28][2] = 0x2A6;
    //bus_configuration[m].pnc_MapFramesTx[28][3] = 0x2A9;
    //bus_configuration[m].pnc_MapFramesTx[28][4] = 0x2AC;
    //bus_configuration[m].pnc_MapFramesTx[28][5] = 0x365;
    //bus_configuration[m].pnc_MapFramesTx[28][6] = 0x370;
    //bus_configuration[m].pnc_MapFramesTx[28][7] = 0x421;

}

//输入要干扰的通道m和busoff的次数：CanSMBorCounterL1ToL2 + 1，注意是busoff，不是错误帧
void make_busoff_case_Recover_time_of_CAN_bus_off(u8 m)
{
    u8 short_recovery_count = 0;
    //u8 *Pshort_recovery_count = &short_recovery_count;
    s64 T1 = 0, T2 = 0;
    TestStep("保持仿真，持续干扰DUT发送的报文");
    TestStep("等待网段出现至少33次bus off恢复");//代码写的是一次次检测，不是等33次之后再校验
    TestStep("检查快恢复时间为CanSMBorTimeL1");//第一帧应用报文 和 上一帧错误帧
    for (u8 i = 0; i < (CanSMBorCounterL1ToL2 + 1); i++)
    {
        sprintf(actualResponse, "开始第%d次干扰", i + 1);
        TestStep(actualResponse);
        //设置干扰某报文，32次，产生32帧错误帧即1次busoff
        //

        //等待DUT发出报文
        check_com(m, 0);//测慢恢复要改一下

        //开始校验
        if (ALLRxMsg[m].size() == 0)
            TestReport_HTML_StepDescription("DUT未发送任何报文，无法校验快恢复和慢恢复", Fail);
        else if (OutErrorAndDiagRxMsg[m].size() == 0)
            //TestInfoPrintFail("DUT只发送了错误帧，无法校验快恢复和慢恢复");
            TestReport_HTML_StepDescription("DUT只发送了错误帧，无法校验快恢复和慢恢复", Fail);

        else if (ErrorRxMsg[m].size() == 0)
            TestReport_HTML_StepDescription("DUT未发送错误帧，无法校验快恢复和慢恢复", Fail);
        //TestInfoPrintFail("DUT未发送错误帧，无法校验快恢复和慢恢复");
        else
        {
            //此时有报文和错误帧，计算diff,检查DUT发送的第一帧报文T2和上一帧错误帧的时间T1
            for (u64 j = 0; j < ALLRxMsg[m].size(); j++)
            {
                if (ALLRxMsg[m][j].get_is_err() == 0)
                {
                    T2 = ALLRxMsg[m][j].FTimeUs;
                    if (j != 0)
                    {
                        T1 = ALLRxMsg[m][j - 1].FTimeUs;
                        sprintf(actualResponse, "DUT发送的第一帧报文时间T2：%fs，上一帧错误帧的时间T1：%fs", T2 / 1000000.0, T1 / 1000000.0);
                        TestReport_HTML_StepDescription(actualResponse, Pass);
                    }
                    else
                    {
                        TestReport_HTML_StepDescription("DUT发送的第一帧报文不是错误帧，无法校验快恢复和慢恢复", Fail);
                        break;
                    }
                    break;
                }
            }
        }

        //计算快恢复时间，快恢复次数，慢恢复时间
        if (i < CanSMBorCounterL1ToL2)
        {
            //等待CanSMBorTimeL1 * 2，检查DUT发送的第一帧报文T2和上一帧错误帧的时间T1是快恢复
            if (check_diff(T2, T1, 0, CanSMBorTimeL1))
                short_recovery_count++;
            ClearAllNMVector();
        }
        else
        {
            //等待CanSMBorTimeL2 * 2，检查DUT发送的第一帧报文和上一帧错误帧的时间是慢恢复
            if (check_diff_no_info(T2, T1, 0, CanSMBorTimeL1))
                short_recovery_count++;

            TestStep("检查快恢复次数为CanSMBorCounterL1ToL2-1次（即第32次恢复为慢恢复）");
            if (short_recovery_count == CanSMBorCounterL1ToL2)
            {
                sprintf(actualResponse, "期望快恢复次数为：%d，实际快恢复次数为：%d", CanSMBorCounterL1ToL2 - 1, short_recovery_count);
                TestReport_HTML_StepDescription(actualResponse, Pass);
            }


            else {
                sprintf(actualResponse, "期望快恢复次数为：%d，实际快恢复次数为：%d", CanSMBorCounterL1ToL2 - 1, short_recovery_count);
                TestReport_HTML_StepDescription(actualResponse, Fail);
            }



            TestStep("检查慢恢复时间为CanSMBorTimeL2");
            check_diff(T2, T1, 0, CanSMBorTimeL2);

            ClearAllNMVector();
        }
    }

}


void set_self_ACK(u8 m, bool onoff)
{
    if (onoff == on)
        app.configure_canfd_baudrate(m, 500, 2000, fdtISOCANFD, fdmNormal, true);//设置错误波特率500，让此条通道收，（如果不想让testcan有错误帧，就把波特率设置正确，用于部分case）
    else
        app.configure_canfd_baudrate(m, 500, 2000, fdtISOCANFD, fdmNormal, true);//设置错误波特率500，让此条通道收，（如果不想让testcan有错误帧，就把波特率设置正确，用于部分case）
}

void set_ACK(u8 m, bool onoff)
{
    if (onoff == on)
        app.configure_canfd_baudrate(m, 500, 2000, fdtISOCANFD, fdmNormal, true);//设置错误波特率500，让此条通道收，（如果不想让testcan有错误帧，就把波特率设置正确，用于部分case）
    else
        app.configure_canfd_baudrate(m, 500, 2000, fdtISOCANFD, fdmACKOff, true);//设置错误波特率500，让此条通道收，（如果不想让testcan有错误帧，就把波特率设置正确，用于部分case）
}


void set_all_ACK_nor()
{
    for (u8 i = 1; i < bus_count; i++)
        app.configure_canfd_baudrate(i, 500, 2000, fdtISOCANFD, fdmNormal, true);//设置错误波特率500，让此条通道收，（如果不想让testcan有错误帧，就把波特率设置正确，用于部分case）
}


//
void pre_test()
{
    //UNREG();
    NMREG();
    //板卡初始化,KL电源连接,暂未连接实体板卡调试
    app.wait(1000, "");
    PrintCaseNameAndPurpose();

    TestReport_StartLogging();

    TestReport_TableInit();

    app.connect();

    if (main_power::instance()->DeviceCreat() == false)
        return;
    app.wait(500, "");

    //SetBoardCardInit_Devices();
    app.wait(500, "");

    BoardCard_Init();
    MainPwr_to_NBATGND(ON);
    //DUT KL30\KL31上电
    KL30(ON);
    KL31(ON);
    main_power::instance()->Set_Voltage(12, 3);
    MainPower_ON();
    //log("Handle:%d", Handle);
        //wait(5000);


    //log("qq cfg path:%s", cfg_path);//C:\Users\shanqingqing\Desktop\NM_3.0\

    handle_all_pnc2bytembitn_and_mapframes();
    set_all_ACK_nor();

    //log("Handle:%d", Handle);
    //此时电流0.124A

}


void post_test()
{
    //app.disconnect();//不能放在下面
   //app.wait(1000, "");
    UNNMREG();
    TestReport_StopLogging();
    TestWait(stepseptime);
    SendTesterPresent(TP_OFF);
    TestWait(stepseptime);
    reportOutputFlag = 0;
    TestWait(1500);
    UNREG();
    uds_delete();

    NMWakeUp(0);

    //KL15(off);
    TestWait(500);
    //KL30(off);

    SetVoltage(0, 0);
    MainPower_OFF();
    BoardCard_Init();
    printf("Post Condition End");
}


vector<u16> splitCharArrayAnd2u16(const char* char_array) {
    vector<u16> result;
    stringstream ss(char_array);
    string item;
    while (getline(ss, item, ',')) {
        u16 num = 0;
        size_t foundPos = item.find("0x");
        if (foundPos != std::string::npos) {
            item.erase(foundPos, 1);
            item.erase(foundPos, 1);
            char* endptr;
            long int result = strtol(item.c_str(), &endptr, 16);
            if (*endptr == '\0') {
                num = static_cast<int>(result);
                //log("0x%X", hexValue);
                //log("%d", hexValue);
            }
            else {
                app.log_text("输入的字符串无法正确转换为十六进制整数", lvlError);
            }
        }
        else {
            num = static_cast<u16>(stoi(item));
        }

        result.push_back(num);
    }
    return result;
}

//1，2，3字符串按，分隔开，并传入vector u8数组[1,2,3]
vector<u8> splitCharArrayAnd2u8(const char* char_array) {
    vector<u8> result;
    stringstream ss(char_array);
    string item;
    while (getline(ss, item, ',')) {
        u8 num = 0;
        size_t foundPos = item.find("0x");
        if (foundPos != std::string::npos) {
            item.erase(foundPos, 1);
            item.erase(foundPos, 1);
            char* endptr;
            long int result = strtol(item.c_str(), &endptr, 16);
            if (*endptr == '\0') {
                num = static_cast<int>(result);
                //log("0x%X", hexValue);
                //log("%d", hexValue);
            }
            else {
                app.log_text("输入的字符串无法正确转换为十六进制整数", lvlError);
            }
        }
        else
            num = static_cast<u8>(stoi(item));

        result.push_back(num);
    }
    return result;
}

//字符串转十进制的u16，字符串是char类型也不要紧
u16 stringToDecu16(const std::string& str) {
    u16 result;
    std::stringstream ss;
    ss << std::dec << str;
    ss >> result;
    return result;
}

//void Read_NM_INI(const char* ini_path) {
//    native_int ini_handle;
//    s32 pncgatewaytype_ValueCapacity = 10000;
//    char pnc_gateway_type[1024] = "";
//    if (0 == app.ini_create(ini_path, &ini_handle))
//    {
//        if (1 == gCEMFlg) {
//            std::vector<std::string> separatedStrings = { "ZCU_CANFD2","ZCU_CANFD1" ,"ChassisCANFD2","ZCUD_CAN1","PropulsionCANFD","ChassisCANFD1","ZCUD_CANFD3","ZCUD_CANFD4"};
//            u8 channelIndex = 1;
//            for (const auto& Str : separatedStrings) {
//            std::string  Network = Str + "_ZCUD5";
//                if (0 == app.ini_read_string(ini_handle, Network.c_str(), "pncgatewaytype", pnc_gateway_type, &pncgatewaytype_ValueCapacity, "pncgatewaytype read Error"))
//                {
//                    if (strncmp(pnc_gateway_type, "ACTIVE", 10) == 0) {
//                        NM_Config[channelIndex].bus_pnc_gateway_type = active_pnc_gateway;
//                        NM_Config[channelIndex].bus_pnc_gateway_type_name = "ACTIVE";
//                    }
//                    else if (strncmp(pnc_gateway_type, "NONE", 5) == 0) {
//                        NM_Config[channelIndex].bus_pnc_gateway_type = no_pnc_gateway;
//                        NM_Config[channelIndex].bus_pnc_gateway_type_name = "NONE";
//                    }
//                    else if (strncmp(pnc_gateway_type, "PASSIVE", 10) == 0) {
//                        NM_Config[channelIndex].bus_pnc_gateway_type = passive_pnc_gateway;
//                        NM_Config[channelIndex].bus_pnc_gateway_type_name = "PASSIVE";
//                    }
//                }
//                channelIndex++;
//                if (channelIndex > can_bus_count_max)
//                    break;
//            }
//        }
//        else {
//            std::vector<std::string> separatedStrings = { "ZCU_CANFD2","ZCU_CANFD1" ,"ZCUP_CAN1","ZCUP_CANFD5" };
//            u8 channelIndex = 1;
//            for (const auto& Str : separatedStrings) {
//                std::string  Network = Str + "_ZCUP5";
//                if (0 == app.ini_read_string(ini_handle, Network.c_str(), "pncgatewaytype", pnc_gateway_type, &pncgatewaytype_ValueCapacity, "pncgatewaytype read Error"))
//                {
//                    if (strncmp(pnc_gateway_type, "ACTIVE", 10) == 0) {
//                        NM_Config[channelIndex].bus_pnc_gateway_type = active_pnc_gateway;
//                        NM_Config[channelIndex].bus_pnc_gateway_type_name = "ACTIVE";
//                    }
//                    else if (strncmp(pnc_gateway_type, "NONE", 5) == 0) {
//                        NM_Config[channelIndex].bus_pnc_gateway_type = no_pnc_gateway;
//                        NM_Config[channelIndex].bus_pnc_gateway_type_name = "NONE";
//                    }
//                    else if (strncmp(pnc_gateway_type, "PASSIVE", 10) == 0) {
//                        NM_Config[channelIndex].bus_pnc_gateway_type = passive_pnc_gateway;
//                        NM_Config[channelIndex].bus_pnc_gateway_type_name = "PASSIVE";
//                    }
//                }
//                channelIndex++;
//                if (channelIndex > can_bus_count_max)
//                    break;
//            }
//        }
//        
//    }
//}



// 假设已有 log 函数用于输出日志
// 假设 app.ini_read_string 的第五个参数会被修改为实际读取的字符数（不含 '\0'）

void Read_NM_INI(const char* ini_path) {
    native_int ini_handle;
    char pnc_gateway_type[1024] = "";

    if (0 != app.ini_create(ini_path, &ini_handle)) {
        log("Failed to open INI file: %s", ini_path);
        return;
    }

    log("=== Read_NM_INI start, ini_path=%s ===", ini_path);

    // 根据标志选择网段列表
    std::vector<std::string> separatedStrings;
    if (1 == gCEMFlg) {
        separatedStrings = { "ZCU_CANFD2", "ZCU_CANFD1", "ChassisCANFD2", "ZCUD_CAN1",
                            "PropulsionCANFD", "ChassisCANFD1", "ZCUD_CANFD3", "ZCUD_CANFD4" ,"BodyExposedCAN"};
    }
    else {
        separatedStrings = { "ZCU_CANFD2", "ZCU_CANFD1", "ZCUP_CAN1", "ZCUP_CANFD5" };
    }

    u8 channelIndex = 1;
    for (const auto& Str : separatedStrings) {
        if (channelIndex > can_bus_count_max) {
            log("channelIndex=%d exceeds can_bus_count_max=%d, break", channelIndex, can_bus_count_max);
            break;
        }

        // 构造节名：网段名 + "_ZCUD5" 或 "_ZCUP5"
        std::string network = Str + (gCEMFlg ? "_ZCUD5" : "_ZCUP5");

        // 【关键修正】每次循环重新初始化容量，避免被上次调用修改
        s32 capacity = static_cast<s32>(sizeof(pnc_gateway_type));
        int read_result = app.ini_read_string(ini_handle, network.c_str(), "pncgatewaytype",
            pnc_gateway_type, &capacity,
            "pncgatewaytype read Error");

        if (0 == read_result) {
            log("Read success: section=%s, raw value='%s', capacity=%d", network.c_str(), pnc_gateway_type, capacity);

            // 打印十六进制，用于检查不可见字符
            log("  Hex:");
            for (size_t i = 0; i < strlen(pnc_gateway_type); ++i) {
                printf("%02X ", static_cast<unsigned char>(pnc_gateway_type[i]));
            }
            printf("\n");

            // 【关键修正】不区分大小写比较，避免大小写问题
            // 方法1：使用 strcasecmp（Linux/Windows 部分环境支持）
            #ifdef _WIN32
            #define strcasecmp _stricmp
            #endif
            if (strcasecmp(pnc_gateway_type, "ACTIVE") == 0) {
                NM_Config[channelIndex].bus_pnc_gateway_type = active_pnc_gateway;
                NM_Config[channelIndex].bus_pnc_gateway_type_name = "ACTIVE";
                log("  Matched ACTIVE (case-insensitive)");
            }
            else if (strcasecmp(pnc_gateway_type, "NONE") == 0) {
                NM_Config[channelIndex].bus_pnc_gateway_type = no_pnc_gateway;
                NM_Config[channelIndex].bus_pnc_gateway_type_name = "NONE";
                log("  Matched NONE (case-insensitive)");
            }
            else if (strcasecmp(pnc_gateway_type, "PASSIVE") == 0) {
                NM_Config[channelIndex].bus_pnc_gateway_type = passive_pnc_gateway;
                NM_Config[channelIndex].bus_pnc_gateway_type_name = "PASSIVE";
                log("  Matched PASSIVE (case-insensitive)");
            }
            else {
                log("  Unrecognized value: '%s'", pnc_gateway_type);
            }
        }
        else {
            log("Read failed (return %d) for section=%s", read_result, network.c_str());
        }

        channelIndex++;
    }

    app.ini_close(ini_handle);
    log("INI file closed.");

    // 可选：输出最终填充的 NM_Config 内容
    for (u8 i = 1; i <= can_bus_count_max; ++i) {
        log("Final NM_Config[%d].bus_pnc_gateway_type_name = '%s'", i,
            NM_Config[i].bus_pnc_gateway_type_name.c_str());
    }
}
void handle_ini(const char* ini_path)
{
    native_int ini_handle;
    s32 all_channel_ValueCapacity = 1024;
    s32 ecu_type_ValueCapacity = 1024;
    s32 ecu_name_ValueCapacity = 1024;
    //s32 ecu_address_ValueCapacity = 1024;
    s32 if_kl30_power_ValueCapacity = 1024;


    char all_test_channel[1024] = "";
    char ecu_type[1024] = "";
    char ecu_name[1024] = "";
    //char ecu_address[1024] = "";
    char if_kl30_power[1024] = "";
    s32 cap = 10000;

    if (0 == app.ini_create(ini_path, &ini_handle))
    {
        if (0 == app.ini_section_exists(ini_handle, "AllChannel"))
        {
            //可以添加逻辑，如果if_test=0，从字符串中删除这个名字
            if (0 == app.ini_read_string(ini_handle, "AllChannel", "all_test_channel", all_test_channel, &all_channel_ValueCapacity, "all_test_channel read Error"))
            {
                log("all_test_channel:%s", all_test_channel);

                std::vector<std::string> separatedStrings;
                std::stringstream ss(all_test_channel);
                std::string item;
                while (std::getline(ss, item, ',')) {
                    separatedStrings.push_back(item);
                }

                if (0 == app.ini_read_string(ini_handle, "AllChannel", "ecu_type", ecu_type, &ecu_type_ValueCapacity, "ecu_type read Error"))
                {
                    if (stringToDecu16(ecu_type) == 0)
                        ecu_gateway_type = non_pnc_gateway;
                    else if (stringToDecu16(ecu_type) == 1)
                        ecu_gateway_type = pnc_gateway;
                    else if (stringToDecu16(ecu_type) == 2)
                        ecu_gateway_type = passive_node;
                    else
                        app.log_text("ecu_gateway_type错误，请检查", lvlError);

                    log("ecu_gateway_type:%d", ecu_gateway_type);

                }

                if (0 == app.ini_read_string(ini_handle, "AllChannel", "ecu_name", ecu_name, &ecu_name_ValueCapacity, "ecu_name read Error"))
                {
                    strcpy(ECU_name, ecu_name);
                    log("ECU_name:%s", ECU_name);
                }


                if (0 == app.ini_read_int32(ini_handle, "AllChannel", "ecu_address", &ECU_address, 0))
                {
                    //会区分16进制
                    log("ECU_address:0x%X", ECU_address);
                }

                if (0 == app.ini_read_string(ini_handle, "AllChannel", "if_kl30_power", if_kl30_power, &if_kl30_power_ValueCapacity, "if_kl30_power read Error"))
                {
                    u8 if_kl30_power_u8 = stringToDecu16(if_kl30_power);
                    if (if_kl30_power_u8 == 0 || if_kl30_power_u8 == 1)
                        if_powered_by_KL30 = if_kl30_power_u8;
                    else
                        app.log_text("if_kl30_power错误，请检查", lvlError);
                    log("if_powered_by_KL30:%d", if_powered_by_KL30);

                }


                if (0 == app.ini_read_bool(ini_handle, "AllChannel", "if_ned_extra_condition_to_go_to_low_power", &if_ned_extra_condition_to_go_to_low_power, 0))
                {
                    log("if_ned_extra_condition_to_go_to_low_power:%d", if_ned_extra_condition_to_go_to_low_power);
                }
                else
                    app.log_text("if_ned_extra_condition_to_go_to_low_power错误，请检查", lvlError);


                if (0 == app.ini_read_bool(ini_handle, "AllChannel", "if_transvier_support_PN", &if_transvier_support_PN, 0))
                    log("if_transvier_support_PN:%d", if_transvier_support_PN);
                else
                    app.log_text("if_transvier_support_PN错误，请检查", lvlError);

                // 输出分隔后的字符串
                u8 current_channel = 1;
                for (const auto& str : separatedStrings)
                {
                    const char* charStr = str.c_str();

                    s32 pncgatewaytype_ValueCapacity = 10000;
                    s32 pnccode_ValueCapacity = 10000;
                    s32 rxpnccode_ValueCapacity = 10000;  // 新增接收PNC容量变量
                    s32 tx_msg_no_map_ValueCapacity = 10000;
                    s32 rx_msg_no_map_ValueCapacity = 10000;
                    s32 tx_msg_pnc_ValueCapacity = 10000;
                    s32 txnmpduid_ValueCapacity = 10000;
                    s32 support_network_wakeup_ValueCapacity = 10000;
                    s32 support_hardware_wakeup_ValueCapacity = 10000;
                    s32 if_sleep_power_on_ValueCapacity = 10000;



                    char pnc_gateway_type[1024] = "";
                    char pnc_code[1024] = "";
                    char rx_pnc_code[1024] = "";  // 新增接收PNC字符串
                    char tx_msg_no_map[1024] = "";
                    char rx_msg_no_map[1024] = "";
                    char tx_msg_pnc[1024] = "";
                    char txnmpduid[1024] = "";
                    char support_network_wakeup[1024] = "";
                    char support_hardware_wakeup[1024] = "";
                    char if_sleep_power_on[1024] = "";

                    char tmp_buf[10000] = "";
                    cap = 1024;

                    strcpy(bus_configuration[current_channel].node_name, charStr);
                    //memset(pnc_code,0,sizeof(pnc_code));

                    log("当前网段：%s", bus_configuration[current_channel].node_name);
                    //if(0 == app.ini_read_int32(ini_handle, charStr, "pncgatewaytype", pnc_gateway_type, "pncgatewaytype read Error"))
                    if (0 == app.ini_read_string(ini_handle, charStr, "pncgatewaytype", pnc_gateway_type, &pncgatewaytype_ValueCapacity, "pncgatewaytype read Error"))
                    {
                        u8 type = 0;
                        type = stringToDecu16(pnc_gateway_type);
                        if (type == 2)
                            bus_configuration[current_channel].bus_pnc_gateway_type = active_pnc_gateway;
                        else if (type == 1)
                            bus_configuration[current_channel].bus_pnc_gateway_type = passive_pnc_gateway;
                        else if (type == 0)
                            bus_configuration[current_channel].bus_pnc_gateway_type = no_pnc_gateway;
                        else
                            app.log_text("错误的pncgatewaytype，请检查", lvlError);

                        //log("pncgatewaytype:%s", pnc_gateway_type);//active
                        log("pncgatewaytype:%d", bus_configuration[current_channel].bus_pnc_gateway_type);//active
                    }
                    if (0 == app.ini_read_string(ini_handle, charStr, "pnccode", pnc_code, &pnccode_ValueCapacity, "pnccode read Error")) {
                        log("pnccode:%s", pnc_code);
                    }

                    // 新增：读取接收PNC码 rxpnccode
                    if (0 == app.ini_read_string(ini_handle, charStr, "rxpnccode", rx_pnc_code, &rxpnccode_ValueCapacity, "")) {
                        log("rxpnccode:%s", rx_pnc_code);
                        // 解析接收PNC码到rx_pnc数组
                        std::vector<u8> rx_pnc_codes = splitCharArrayAnd2u8(rx_pnc_code);
                        for (size_t i = 0; i < rx_pnc_codes.size() && i < pnc_max_count_in_one_bus; i++) {
                            bus_configuration[current_channel].rx_pnc[i] = rx_pnc_codes[i];
                            log("  接收PNC[%d] = %d", i, bus_configuration[current_channel].rx_pnc[i]);
                        }
                    }
                    else {
                        log("未找到rxpnccode字段或为空");
                        // 如果没有rxpnccode字段，清空rx_pnc数组
                        memset(bus_configuration[current_channel].rx_pnc, 0, sizeof(bus_configuration[current_channel].rx_pnc));
                    }

                    if (0 == app.ini_read_string(ini_handle, charStr, "txnmpduid", txnmpduid, &txnmpduid_ValueCapacity, "txnmpduid read Error")) {
                        bus_configuration[current_channel].dut_send_nm_msg_id = stringToDecu16(txnmpduid);

                        log("txnmpduid:0x%x", bus_configuration[current_channel].dut_send_nm_msg_id);

                    }
                    //if (0 == app.ini_read_string(ini_handle, charStr, "tx_msg_no_map", tx_msg_no_map, &tx_msg_no_map_ValueCapacity, "tx_msg_no_map read Error"))
                    //{
                    //    log("tx_msg_no_map:%s", tx_msg_no_map);
                    //    bus_configuration[current_channel].NoMapFrames = splitCharArrayAnd2u16(tx_msg_no_map);
                    //    //log("%d", bus_configuration[current_channel].NoMapFrames[0]);
                    //    //log("%d", bus_configuration[current_channel].NoMapFrames.size());
                    //}
                    // 2. 读取 tx_msg_no_map (周期 & 事件)
                    cap = 10000;
                    if (0 == app.ini_read_string(ini_handle, charStr, "tx_msg_no_map", tmp_buf, &cap, ""))
                        bus_configuration[current_channel].NoMapFrames = splitCharArrayAnd2u16(tmp_buf);

                    cap = 10000;
                    memset(tmp_buf, 0, sizeof(tmp_buf));
                    if (0 == app.ini_read_string(ini_handle, charStr, "tx_msg_no_map_event", tmp_buf, &cap, ""))
                        bus_configuration[current_channel].NoMapFrames_Event = splitCharArrayAnd2u16(tmp_buf);


                    if (0 == app.ini_read_bool(ini_handle, charStr, "if_support_i_pdu_control", &bus_configuration[current_channel].if_support_I_PDU_control, 0))
                        log("if_support_i_pdu_control:%d", bus_configuration[current_channel].if_support_I_PDU_control);
                    else
                        app.log_text("if_support_I_PDU_control错误，请检查", lvlError);

                    // 1. 读取 PNC 列表 (pnccode)
                    char pnc_code_str[1024] = "";
                    cap = 1024;
                    if (0 == app.ini_read_string(ini_handle, charStr, "pnccode", pnc_code_str, &cap, ""))
                    {
                        std::vector<u8> codes = splitCharArrayAnd2u8(pnc_code_str);
                        for (u8 vector_i = 0; vector_i < codes.size(); vector_i++)
                        {
                            u8 c_pnc = codes[vector_i];
                            bus_configuration[current_channel].relevant_pnc[vector_i] = c_pnc;

                            // 读取周期报文 tx_msg_pncXX
                            std::string key_cyclic = "tx_msg_pnc" + std::to_string(c_pnc);
                            cap = 10000;
                            if (0 == app.ini_read_string(ini_handle, charStr, key_cyclic.c_str(), tmp_buf, &cap, ""))
                                bus_configuration[current_channel].pnc_MapFramesTx[c_pnc - 1] = splitCharArrayAnd2u16(tmp_buf);

                            // 读取事件报文 tx_msg_pncXX_event
                            std::string key_event = key_cyclic + "_event";
                            cap = 10000;
                            memset(tmp_buf, 0, sizeof(tmp_buf));
                            if (0 == app.ini_read_string(ini_handle, charStr, key_event.c_str(), tmp_buf, &cap, ""))
                                bus_configuration[current_channel].pnc_MapFramesTx_Event[c_pnc - 1] = splitCharArrayAnd2u16(tmp_buf);
                        }
                    }

                    if (0 == app.ini_read_string(ini_handle, charStr, "rx_msg_no_map", rx_msg_no_map, &rx_msg_no_map_ValueCapacity, "rx_msg_no_map read Error"))
                    {
                        //log("rx_msg_no_map:%s", rx_msg_no_map);
                        bus_configuration[current_channel].rx_msg[0] = splitCharArrayAnd2u16(rx_msg_no_map)[0];
                        log("rx_msg_no_map[0]:%d", bus_configuration[current_channel].rx_msg[0]);
                    }

                    if (0 == app.ini_read_string(ini_handle, charStr, "support_network_wakeup", support_network_wakeup, &support_network_wakeup_ValueCapacity, "support_network_wakeup read Error"))
                    {
                        //log("rx_msg_no_map:%s", rx_msg_no_map);
                        bus_configuration[current_channel].if_support_network_wakeup = stringToDecu16(support_network_wakeup);
                        log("support_network_wakeup:%d", bus_configuration[current_channel].if_support_network_wakeup);
                    }

                    if (0 == app.ini_read_string(ini_handle, charStr, "support_hardware_wakeup", support_hardware_wakeup, &support_hardware_wakeup_ValueCapacity, "support_hardware_wakeup read Error"))
                    {
                        //log("rx_msg_no_map:%s", rx_msg_no_map);
                        bus_configuration[current_channel].if_support_hardware_wakeup = stringToDecu16(support_hardware_wakeup);
                        log("support_hardware_wakeup:%d", bus_configuration[current_channel].if_support_hardware_wakeup);
                    }


                    if (0 == app.ini_read_bool(ini_handle, charStr, "if_support_vfc", &bus_configuration[current_channel].if_support_VFC, 0))
                        log("if_support_vfc:%d", bus_configuration[current_channel].if_support_VFC);
                    else
                        app.log_text("if_support_vfc错误，请检查", lvlError);


                    if (0 == app.ini_read_string(ini_handle, charStr, "if_sleep_power_on", if_sleep_power_on, &if_sleep_power_on_ValueCapacity, "if_sleep_power_on read Error"))
                    {
                        //log("rx_msg_no_map:%s", rx_msg_no_map);
                        u8 if_sleep_power_on_u8 = stringToDecu16(if_sleep_power_on);
                        if (if_sleep_power_on_u8 == 0 || if_sleep_power_on_u8 == 1)
                            bus_configuration[current_channel].if_sleep_state_with_power_supply_on = if_sleep_power_on_u8;
                        else
                            app.log_text("if_sleep_power_on错误，请检查", lvlError);
                        log("if_sleep_power_on:%d", bus_configuration[current_channel].if_sleep_state_with_power_supply_on);
                    }



                    if (0 == app.ini_read_bool(ini_handle, charStr, "if_can_be_sleep_with_power", &bus_configuration[current_channel].if_can_be_sleep_with_power, 0))
                        log("if_can_be_sleep_with_power:%d", bus_configuration[current_channel].if_can_be_sleep_with_power);
                    else
                        app.log_text("if_can_be_sleep_with_power错误，请检查", lvlError);





                    current_channel++;
                }
                bus_count = current_channel;
                log("bus_count:%d", bus_count);
            }
        }
        else
            app.log_text("AllChannel节不存在于 ini文件中", lvlError);
    };
    app.ini_close(ini_handle);
};

//void handle_ini(const char* ini_path)
//{
//    native_int ini_handle;
//    s32 all_channel_ValueCapacity = 1024;
//    s32 ecu_type_ValueCapacity = 1024;
//    s32 ecu_name_ValueCapacity = 1024;
//    //s32 ecu_address_ValueCapacity = 1024;
//    s32 if_kl30_power_ValueCapacity = 1024;
//
//
//    char all_test_channel[1024] = "";
//    char ecu_type[1024] = "";
//    char ecu_name[1024] = "";
//    //char ecu_address[1024] = "";
//    char if_kl30_power[1024] = "";
//    s32 cap = 10000;
//
//    if (0 == app.ini_create(ini_path, &ini_handle))
//    {
//        if (0 == app.ini_section_exists(ini_handle, "AllChannel"))
//        {
//            //可以添加逻辑，如果if_test=0，从字符串中删除这个名字
//            if (0 == app.ini_read_string(ini_handle, "AllChannel", "all_test_channel", all_test_channel, &all_channel_ValueCapacity, "all_test_channel read Error"))
//            {
//                log("all_test_channel:%s", all_test_channel);
//
//                std::vector<std::string> separatedStrings;
//                std::stringstream ss(all_test_channel);
//                std::string item;
//                while (std::getline(ss, item, ',')) {
//                    separatedStrings.push_back(item);
//                }
//
//                if (0 == app.ini_read_string(ini_handle, "AllChannel", "ecu_type", ecu_type, &ecu_type_ValueCapacity, "ecu_type read Error"))
//                {
//                    if (stringToDecu16(ecu_type) == 0)
//                        ecu_gateway_type = non_pnc_gateway;
//                    else if (stringToDecu16(ecu_type) == 1)
//                        ecu_gateway_type = pnc_gateway;
//                    else if (stringToDecu16(ecu_type) == 2)
//                        ecu_gateway_type = passive_node;
//                    else
//                        app.log_text("ecu_gateway_type错误，请检查", lvlError);
//
//                    log("ecu_gateway_type:%d", ecu_gateway_type);
//
//                }
//
//                if (0 == app.ini_read_string(ini_handle, "AllChannel", "ecu_name", ecu_name, &ecu_name_ValueCapacity, "ecu_name read Error"))
//                {
//                    strcpy(ECU_name, ecu_name);
//                    log("ECU_name:%s", ECU_name);
//                }
//
//
//                if (0 == app.ini_read_int32(ini_handle, "AllChannel", "ecu_address", &ECU_address, 0))
//                {
//                    //会区分16进制
//                    log("ECU_address:0x%X", ECU_address);
//                }
//
//                if (0 == app.ini_read_string(ini_handle, "AllChannel", "if_kl30_power", if_kl30_power, &if_kl30_power_ValueCapacity, "if_kl30_power read Error"))
//                {
//                    u8 if_kl30_power_u8 = stringToDecu16(if_kl30_power);
//                    if (if_kl30_power_u8 == 0 || if_kl30_power_u8 == 1)
//                        if_powered_by_KL30 = if_kl30_power_u8;
//                    else
//                        app.log_text("if_kl30_power错误，请检查", lvlError);
//                    log("if_powered_by_KL30:%d", if_powered_by_KL30);
//
//                }
//
//
//                if (0 == app.ini_read_bool(ini_handle, "AllChannel", "if_ned_extra_condition_to_go_to_low_power", &if_ned_extra_condition_to_go_to_low_power, 0))
//                {
//                    log("if_ned_extra_condition_to_go_to_low_power:%d", if_ned_extra_condition_to_go_to_low_power);
//                }
//                else
//                    app.log_text("if_ned_extra_condition_to_go_to_low_power错误，请检查", lvlError);
//
//
//                if (0 == app.ini_read_bool(ini_handle, "AllChannel", "if_transvier_support_PN", &if_transvier_support_PN, 0))
//                    log("if_transvier_support_PN:%d", if_transvier_support_PN);
//                else
//                    app.log_text("if_transvier_support_PN错误，请检查", lvlError);
//
//                // 输出分隔后的字符串
//                u8 current_channel = 1;
//                for (const auto& str : separatedStrings)
//                {
//                    const char* charStr = str.c_str();
//
//                    s32 pncgatewaytype_ValueCapacity = 10000;
//                    s32 pnccode_ValueCapacity = 10000;
//                    s32 tx_msg_no_map_ValueCapacity = 10000;
//                    s32 rx_msg_no_map_ValueCapacity = 10000;
//                    s32 tx_msg_pnc_ValueCapacity = 10000;
//                    s32 txnmpduid_ValueCapacity = 10000;
//                    s32 support_network_wakeup_ValueCapacity = 10000;
//                    s32 support_hardware_wakeup_ValueCapacity = 10000;
//                    s32 if_sleep_power_on_ValueCapacity = 10000;
//
//
//
//                    char pnc_gateway_type[1024] = "";
//                    char pnc_code[1024] = "";
//                    char rx_pnc_code[1024] = "";
//                    char tx_msg_no_map[1024] = "";
//                    char rx_msg_no_map[1024] = "";
//                    char tx_msg_pnc[1024] = "";
//                    char txnmpduid[1024] = "";
//                    char support_network_wakeup[1024] = "";
//                    char support_hardware_wakeup[1024] = "";
//                    char if_sleep_power_on[1024] = "";
//
//                    char tmp_buf[10000] = "";
//                    cap = 1024;
//
//                    strcpy(bus_configuration[current_channel].node_name, charStr);
//                    //memset(pnc_code,0,sizeof(pnc_code));
//
//                    log("当前网段：%s", bus_configuration[current_channel].node_name);
//                    //if(0 == app.ini_read_int32(ini_handle, charStr, "pncgatewaytype", pnc_gateway_type, "pncgatewaytype read Error"))
//                    if (0 == app.ini_read_string(ini_handle, charStr, "pncgatewaytype", pnc_gateway_type, &pncgatewaytype_ValueCapacity, "pncgatewaytype read Error"))
//                    {
//                        u8 type = 0;
//                        type = stringToDecu16(pnc_gateway_type);
//                        if (type == 2)
//                            bus_configuration[current_channel].bus_pnc_gateway_type = active_pnc_gateway;
//                        else if (type == 1)
//                            bus_configuration[current_channel].bus_pnc_gateway_type = passive_pnc_gateway;
//                        else if (type == 0)
//                            bus_configuration[current_channel].bus_pnc_gateway_type = no_pnc_gateway;
//                        else
//                            app.log_text("错误的pncgatewaytype，请检查", lvlError);
//
//                        //log("pncgatewaytype:%s", pnc_gateway_type);//active
//                        log("pncgatewaytype:%d", bus_configuration[current_channel].bus_pnc_gateway_type);//active
//                    }
//                    if (0 == app.ini_read_string(ini_handle, charStr, "pnccode", pnc_code, &pnccode_ValueCapacity, "pnccode read Error")) {
//                        log("pnccode:%s", pnc_code);
//                    }
//                    if (0 == app.ini_read_string(ini_handle, charStr, "txnmpduid", txnmpduid, &txnmpduid_ValueCapacity, "txnmpduid read Error")) {
//                        bus_configuration[current_channel].dut_send_nm_msg_id = stringToDecu16(txnmpduid);
//
//                        log("txnmpduid:0x%x", bus_configuration[current_channel].dut_send_nm_msg_id);
//
//                    }
//                    //if (0 == app.ini_read_string(ini_handle, charStr, "tx_msg_no_map", tx_msg_no_map, &tx_msg_no_map_ValueCapacity, "tx_msg_no_map read Error"))
//                    //{
//                    //    log("tx_msg_no_map:%s", tx_msg_no_map);
//                    //    bus_configuration[current_channel].NoMapFrames = splitCharArrayAnd2u16(tx_msg_no_map);
//                    //    //log("%d", bus_configuration[current_channel].NoMapFrames[0]);
//                    //    //log("%d", bus_configuration[current_channel].NoMapFrames.size());
//                    //}
//                    // 2. 读取 tx_msg_no_map (周期 & 事件)
//                    cap = 10000;
//                    if (0 == app.ini_read_string(ini_handle, charStr, "tx_msg_no_map", tmp_buf, &cap, ""))
//                        bus_configuration[current_channel].NoMapFrames = splitCharArrayAnd2u16(tmp_buf);
//
//                    cap = 10000;
//                    memset(tmp_buf, 0, sizeof(tmp_buf));
//                    if (0 == app.ini_read_string(ini_handle, charStr, "tx_msg_no_map_event", tmp_buf, &cap, ""))
//                        bus_configuration[current_channel].NoMapFrames_Event = splitCharArrayAnd2u16(tmp_buf);
//
//
//                    if (0 == app.ini_read_bool(ini_handle, charStr, "if_support_i_pdu_control", &bus_configuration[current_channel].if_support_I_PDU_control, 0))
//                        log("if_support_i_pdu_control:%d", bus_configuration[current_channel].if_support_I_PDU_control);
//                    else
//                        app.log_text("if_support_I_PDU_control错误，请检查", lvlError);
//
//                    // 1. 读取 PNC 列表 (pnccode)
//                    char pnc_code_str[1024] = "";
//                    cap = 1024;
//                    if (0 == app.ini_read_string(ini_handle, charStr, "pnccode", pnc_code_str, &cap, ""))
//                    {
//                        std::vector<u8> codes = splitCharArrayAnd2u8(pnc_code_str);
//                        for (u8 vector_i = 0; vector_i < codes.size(); vector_i++)
//                        {
//                            u8 c_pnc = codes[vector_i];
//                            bus_configuration[current_channel].relevant_pnc[vector_i] = c_pnc;
//
//                            // 读取周期报文 tx_msg_pncXX
//                            std::string key_cyclic = "tx_msg_pnc" + std::to_string(c_pnc);
//                            cap = 10000;
//                            if (0 == app.ini_read_string(ini_handle, charStr, key_cyclic.c_str(), tmp_buf, &cap, ""))
//                                bus_configuration[current_channel].pnc_MapFramesTx[c_pnc - 1] = splitCharArrayAnd2u16(tmp_buf);
//
//                            // 读取事件报文 tx_msg_pncXX_event
//                            std::string key_event = key_cyclic + "_event";
//                            cap = 10000;
//                            memset(tmp_buf, 0, sizeof(tmp_buf));
//                            if (0 == app.ini_read_string(ini_handle, charStr, key_event.c_str(), tmp_buf, &cap, ""))
//                                bus_configuration[current_channel].pnc_MapFramesTx_Event[c_pnc - 1] = splitCharArrayAnd2u16(tmp_buf);
//                        }
//                    }
//
//                    if (0 == app.ini_read_string(ini_handle, charStr, "rx_msg_no_map", rx_msg_no_map, &rx_msg_no_map_ValueCapacity, "rx_msg_no_map read Error"))
//                    {
//                        //log("rx_msg_no_map:%s", rx_msg_no_map);
//                        bus_configuration[current_channel].rx_msg[0] = splitCharArrayAnd2u16(rx_msg_no_map)[0];
//                        log("rx_msg_no_map[0]:%d", bus_configuration[current_channel].rx_msg[0]);
//                    }
//
//                    if (0 == app.ini_read_string(ini_handle, charStr, "support_network_wakeup", support_network_wakeup, &support_network_wakeup_ValueCapacity, "support_network_wakeup read Error"))
//                    {
//                        //log("rx_msg_no_map:%s", rx_msg_no_map);
//                        bus_configuration[current_channel].if_support_network_wakeup = stringToDecu16(support_network_wakeup);
//                        log("support_network_wakeup:%d", bus_configuration[current_channel].if_support_network_wakeup);
//                    }
//
//                    if (0 == app.ini_read_string(ini_handle, charStr, "support_hardware_wakeup", support_hardware_wakeup, &support_hardware_wakeup_ValueCapacity, "support_hardware_wakeup read Error"))
//                    {
//                        //log("rx_msg_no_map:%s", rx_msg_no_map);
//                        bus_configuration[current_channel].if_support_hardware_wakeup = stringToDecu16(support_hardware_wakeup);
//                        log("support_hardware_wakeup:%d", bus_configuration[current_channel].if_support_hardware_wakeup);
//                    }
//
//
//                    if (0 == app.ini_read_bool(ini_handle, charStr, "if_support_vfc", &bus_configuration[current_channel].if_support_VFC, 0))
//                        log("if_support_vfc:%d", bus_configuration[current_channel].if_support_VFC);
//                    else
//                        app.log_text("if_support_vfc错误，请检查", lvlError);
//
//
//                    if (0 == app.ini_read_string(ini_handle, charStr, "if_sleep_power_on", if_sleep_power_on, &if_sleep_power_on_ValueCapacity, "if_sleep_power_on read Error"))
//                    {
//                        //log("rx_msg_no_map:%s", rx_msg_no_map);
//                        u8 if_sleep_power_on_u8 = stringToDecu16(if_sleep_power_on);
//                        if (if_sleep_power_on_u8 == 0 || if_sleep_power_on_u8 == 1)
//                            bus_configuration[current_channel].if_sleep_state_with_power_supply_on = if_sleep_power_on_u8;
//                        else
//                            app.log_text("if_sleep_power_on错误，请检查", lvlError);
//                        log("if_sleep_power_on:%d", bus_configuration[current_channel].if_sleep_state_with_power_supply_on);
//                    }
//
//
//
//                    if (0 == app.ini_read_bool(ini_handle, charStr, "if_can_be_sleep_with_power", &bus_configuration[current_channel].if_can_be_sleep_with_power, 0))
//                        log("if_can_be_sleep_with_power:%d", bus_configuration[current_channel].if_can_be_sleep_with_power);
//                    else
//                        app.log_text("if_can_be_sleep_with_power错误，请检查", lvlError);
//
//
//
//
//
//                    current_channel++;
//                }
//                bus_count = current_channel;
//                log("bus_count:%d", bus_count);
//            }
//        }
//        else
//            app.log_text("AllChannel节不存在于 ini文件中", lvlError);
//    };
//    app.ini_close(ini_handle);
//};
/*
void handle_ini(const char* ini_path)
{
    native_int ini_handle;
    s32 all_channel_ValueCapacity = 1024;
    s32 ecu_type_ValueCapacity = 1024;
    s32 ecu_name_ValueCapacity = 1024;
    s32 if_kl30_power_ValueCapacity = 1024;

    char all_test_channel[1024] = "";
    char ecu_type[1024] = "";
    char ecu_name[1024] = "";
    char if_kl30_power[1024] = "";

    if (0 == app.ini_create(ini_path, &ini_handle))
    {
        if (0 == app.ini_section_exists(ini_handle, "AllChannel"))
        {
            if (0 == app.ini_read_string(ini_handle, "AllChannel", "all_test_channel", all_test_channel, &all_channel_ValueCapacity, "all_test_channel read Error"))
            {
                log("all_test_channel:%s", all_test_channel);

                std::vector<std::string> separatedStrings;
                std::stringstream ss(all_test_channel);
                std::string item;
                while (std::getline(ss, item, ',')) {
                    separatedStrings.push_back(item);
                }

                // 读取 ECU 通用配置
                app.ini_read_string(ini_handle, "AllChannel", "ecu_type", ecu_type, &ecu_type_ValueCapacity, "");
                app.ini_read_string(ini_handle, "AllChannel", "ecu_name", ecu_name, &ecu_name_ValueCapacity, "");
                app.ini_read_int32(ini_handle, "AllChannel", "ecu_address", &ECU_address, 0);

                u8 current_channel = 1;
                for (const auto& str : separatedStrings)
                {
                    const char* charStr = str.c_str();
                    strcpy(bus_configuration[current_channel].node_name, charStr);
                    log("--- 处理网段：%s ---", charStr);

                    s32 cap = 10000;
                    char tmp_buf[10000] = "";

                    // 1. 读取 PNC 列表 (pnccode)
                    char pnc_code_str[1024] = "";
                    cap = 1024;
                    if (0 == app.ini_read_string(ini_handle, charStr, "pnccode", pnc_code_str, &cap, ""))
                    {
                        std::vector<u8> codes = splitCharArrayAnd2u8(pnc_code_str);
                        for (u8 vector_i = 0; vector_i < codes.size(); vector_i++)
                        {
                            u8 c_pnc = codes[vector_i];
                            bus_configuration[current_channel].relevant_pnc[vector_i] = c_pnc;

                            // 读取周期报文 tx_msg_pncXX
                            std::string key_cyclic = "tx_msg_pnc" + std::to_string(c_pnc);
                            cap = 10000;
                            if (0 == app.ini_read_string(ini_handle, charStr, key_cyclic.c_str(), tmp_buf, &cap, ""))
                                bus_configuration[current_channel].pnc_MapFramesTx[c_pnc - 1] = splitCharArrayAnd2u16(tmp_buf);

                            // 读取事件报文 tx_msg_pncXX_event
                            std::string key_event = key_cyclic + "_event";
                            cap = 10000;
                            memset(tmp_buf, 0, sizeof(tmp_buf));
                            if (0 == app.ini_read_string(ini_handle, charStr, key_event.c_str(), tmp_buf, &cap, ""))
                                bus_configuration[current_channel].pnc_MapFramesTx_Event[c_pnc - 1] = splitCharArrayAnd2u16(tmp_buf);
                        }
                    }

                    // 2. 读取 tx_msg_no_map (周期 & 事件)
                    cap = 10000;
                    if (0 == app.ini_read_string(ini_handle, charStr, "tx_msg_no_map", tmp_buf, &cap, ""))
                        bus_configuration[current_channel].NoMapFrames = splitCharArrayAnd2u16(tmp_buf);

                    cap = 10000;
                    memset(tmp_buf, 0, sizeof(tmp_buf));
                    if (0 == app.ini_read_string(ini_handle, charStr, "tx_msg_no_map_event", tmp_buf, &cap, ""))
                        bus_configuration[current_channel].NoMapFrames_Event = splitCharArrayAnd2u16(tmp_buf);

                    // 3. 读取其他常规配置
                    cap = 1024;
                    app.ini_read_string(ini_handle, charStr, "txnmpduid", tmp_buf, &cap, "");
                    bus_configuration[current_channel].dut_send_nm_msg_id = stringToDecu16(tmp_buf);

                    current_channel++;
                }
                bus_count = current_channel;
            }
        }
    }
    app.ini_close(ini_handle);
}
*/
//检查PN识别和映射关系
/*
vector<u16> check_dut_send_pnc_MapFrames_and_NoMapFrames(u8 m, u8 PNC)
{
    u8 s = 0;
    u16 t = 0;
    vector <u16> need_dut_send_msg_id;
    u16 recv_msg_count[1024] = { 0 };
    u16 p = 0, q = 0;
    for (p = 0; p < AllPNCMapFramesCountMax; p++)
    {
        if (bus_configuration[m].pnc_MapFramesTx[PNC - 1].size() > p)
            //if (bus_configuration[m].pnc_MapFramesTx[PNC - 1][p] != 0)
            need_dut_send_msg_id.push_back(bus_configuration[m].pnc_MapFramesTx[PNC - 1][p]);
    }
    for (q = 0; q < NoMapFramesCountMax; q++)
    {
        if (bus_configuration[m].NoMapFrames.size() > q)
            //if(bus_configuration[m].NoMapFrames[q]!=0)
            need_dut_send_msg_id.push_back(bus_configuration[m].NoMapFrames[q]);
    }
    //for (i; i < need_recv_msg_id.size(); i++)
    //	log("0x%X", need_recv_msg_id[i]);
    if (DUT_APPRxMsg[m].size() == 0)
        TestReport_HTML_StepDescription("DUT未发送任何APP报文", Fail);

    else
    {
        for (s = 0; s < need_dut_send_msg_id.size(); s++)
        {
            printf("Need CheckID :0x%X",need_dut_send_msg_id[s]);
            for (t = 0; t < DUT_APPRxMsg[m].size(); t++)
            {
                //if (DUT_APPRxMsg[m][t].FIdentifier == 0x121)
                //	log("%fs", DUT_APPRxMsg[m][t].FTimeUs / 1000000.0);
                if (DUT_APPRxMsg[m][t].FIdentifier == need_dut_send_msg_id[s] && DUT_APPRxMsg[m][t].FIdentifier != bus_configuration[m].dut_send_nm_msg_id)
                {
                    sprintf(actualResponse, "DUT发送0x%X报文到此网段，报文时间戳%fs", need_dut_send_msg_id[s], DUT_APPRxMsg[m][t].FTimeUs / 1000000.0);
                    TestReport_HTML_StepDescription(actualResponse, Pass);
                    recv_msg_count[s] = 1;
                    break;
                }
            }

            if (recv_msg_count[s] == 0) {
                sprintf(actualResponse, "DUT未发送0x%X报文到此网段", need_dut_send_msg_id[s]);
                TestReport_HTML_StepDescription(actualResponse, Fail);
            }
        }

        vector<u16>all_recv_msg_id;
        for (u16 a = 0; a < DUT_APPRxMsg[m].size(); a++)
        {
            bool all_recv_msg_id_flag = 1;
            if (all_recv_msg_id.size() == 0)
                all_recv_msg_id.push_back(DUT_APPRxMsg[m][0].FIdentifier);
            else
            {
                for (u16 b = 0; b < all_recv_msg_id.size(); b++)
                {
                    if (DUT_APPRxMsg[m][a].FIdentifier == all_recv_msg_id[b])
                    {
                        all_recv_msg_id_flag = 0;
                        break;
                    }

                }
                if (all_recv_msg_id_flag == 1)
                    all_recv_msg_id.push_back(DUT_APPRxMsg[m][a].FIdentifier);
            }
        }

        //
        if (all_recv_msg_id.size() == 0)
            TestReport_HTML_StepDescription("DUT未发送任何报文", Fail);
        else
        {
            //for (u16 d = 0; d < need_dut_send_msg_id.size(); d++)
            //{
            //    bool no_ned_recv_msg_id_flag = 1;
            //    bool no_nm_frame_flag = 1;
            //    for (u16 c = 0; c < all_recv_msg_id.size(); c++)
            //    {
            //        printf("1----need_dut_send_msg_id[d]{0x%X} == all_recv_msg_id[c]{0x%X}", need_dut_send_msg_id[d], all_recv_msg_id[c]);
            //        if (need_dut_send_msg_id[d] == all_recv_msg_id[c] || (all_recv_msg_id[c] == bus_configuration[m].dut_send_nm_msg_id))
            //        {
            //            printf("2---need_dut_send_msg_id[d]{0x%X} == all_recv_msg_id[c]{0x%X}", need_dut_send_msg_id[d], all_recv_msg_id[c]);
            //            no_ned_recv_msg_id_flag = 0;
            //            break;
            //        }
            //        //if (all_recv_msg_id[c] != bus_configuration[m].dut_send_nm_msg_id)
            //        //    no_nm_frame_flag = 0;
            //    }
            //
            //    //printf("d=%d,all_recv_msg_id[c]{0x%X} != bus_configuration[m].dut_send_nm_msg_id {0x%X}", d,all_recv_msg_id[c], bus_configuration[m].dut_send_nm_msg_id);
            //    //if (no_ned_recv_msg_id_flag == 1 && no_nm_frame_flag == 0) {
            //    if(no_ned_recv_msg_id_flag == 1){
            //        sprintf(actualResponse, "DUT发送0x%X报文到此网段", all_recv_msg_id[c]);
            //        TestReport_HTML_StepDescription(actualResponse, Fail);
            //    }
            //}
            for (u16 c = 0; c < all_recv_msg_id.size(); c++)
            {
                bool no_ned_recv_msg_id_flag = 1;
                for (u16 d = 0; d < need_dut_send_msg_id.size(); d++)
                {
                    printf("1----need_dut_send_msg_id[d]{0x%X} == all_recv_msg_id[c]{0x%X}", need_dut_send_msg_id[d], all_recv_msg_id[c]);
                    if (need_dut_send_msg_id[d] == all_recv_msg_id[c] || (all_recv_msg_id[c] == bus_configuration[m].dut_send_nm_msg_id))
                    {
                        printf("2---need_dut_send_msg_id[d]{0x%X} == all_recv_msg_id[c]{0x%X}", need_dut_send_msg_id[d], all_recv_msg_id[c]);
                        no_ned_recv_msg_id_flag = 0;
                        break;
                    }
                }
                if (no_ned_recv_msg_id_flag == 1 && all_recv_msg_id[c] != bus_configuration[m].dut_send_nm_msg_id)
                    TestInfoPrintFail("DUT发送0x%X报文到此网段", all_recv_msg_id[c]);
            }

        }

    }

    return need_dut_send_msg_id;
}
*/
vector<u16> check_dut_send_pnc_MapFrames_and_NoMapFrames(u8 m, u8 PNC)
{
    vector<u16> need_cyclic_ids;    // 定义：必须发送的报文 (周期性)
    vector<u16> allowed_event_ids;  // 定义：允许发送的报文 (事件性白名单)

    // 1. 组合该 PNC 相关的报文池
    // 周期池
    for (u16 id : bus_configuration[m].pnc_MapFramesTx[PNC - 1]) need_cyclic_ids.push_back(id);
    for (u16 id : bus_configuration[m].NoMapFrames) need_cyclic_ids.push_back(id);

    // 事件池
    for (u16 id : bus_configuration[m].pnc_MapFramesTx_Event[PNC - 1]) allowed_event_ids.push_back(id);
    for (u16 id : bus_configuration[m].NoMapFrames_Event) allowed_event_ids.push_back(id);

    // --- 校验阶段 A: 检查周期性报文是否漏发 ---
    for (u16 i = 0; i < need_cyclic_ids.size(); i++)
    {
        u16 target_id = need_cyclic_ids[i];
        bool is_received = false;

        for (u16 t = 0; t < DUT_APPRxMsg[m].size(); t++)
        {
            if (DUT_APPRxMsg[m][t].FIdentifier == target_id && target_id != bus_configuration[m].dut_send_nm_msg_id)
            {
                sprintf(actualResponse, "DUT已发送周期报文0x%X", target_id);
                TestReport_HTML_StepDescription(actualResponse, Pass);
                is_received = true;
                break;
            }
        }

        if (!is_received)
        {
            sprintf(actualResponse, "DUT未发送周期报文0x%X", target_id);
            TestReport_HTML_StepDescription(actualResponse, Fail);
        }
    }

    // --- 校验阶段 B: 检查收到的所有报文是否都在映射内 (含事件报文) ---
    // 先获取收到的所有 ID 的去重集合
    std::set<u16> actual_recv_ids;
    for (auto& msg : DUT_APPRxMsg[m]) actual_recv_ids.insert(msg.FIdentifier);

    for (u16 recv_id : actual_recv_ids)
    {
        // 排除掉网关自身的网管报文
        if (recv_id == bus_configuration[m].dut_send_nm_msg_id) continue;

        // 1. 检查是否在周期列表内 (上面已检测过 Pass，此处仅作合法性判断)
        bool in_cyclic = std::find(need_cyclic_ids.begin(), need_cyclic_ids.end(), recv_id) != need_cyclic_ids.end();
        if (in_cyclic) continue;

        // 2. 检查是否在事件白名单内
        bool in_event = std::find(allowed_event_ids.begin(), allowed_event_ids.end(), recv_id) != allowed_event_ids.end();

        if (in_event)
        {
            sprintf(actualResponse, "DUT发送事件报文0x%X", recv_id);
            TestReport_HTML_StepDescription(actualResponse, Pass);
        }
        else
        {
            // 既不在周期列表，也不在事件映射表，判定为非法报文
            TestInfoPrintFail("DUT发送非法报文0x%X (不在该PNC的周期或事件映射中)", recv_id);
        }
    }

    // 返回周期列表用于后续逻辑（兼容原代码结构）
    return need_cyclic_ids;
}

//检查PN识别和映射关系
vector<u16> return_dut_send_pnc_MapFrames(u8 m, u8 PNC)
{
    u8 s = 0;
    u16 t = 0;
    vector <u16> need_dut_send_msg_id;
    u16 recv_msg_count[1024] = { 0 };
    u16 p = 0, q = 0;
    for (p = 0; p < AllPNCMapFramesCountMax; p++)
    {
        if (bus_configuration[m].pnc_MapFramesTx[PNC - 1].size() > p)
            //if (bus_configuration[m].pnc_MapFramesTx[PNC - 1][p] != 0)
            need_dut_send_msg_id.push_back(bus_configuration[m].pnc_MapFramesTx[PNC - 1][p]);
    }
    return need_dut_send_msg_id;
}


void check_map_msg_time(u8 m, vector <u16> return_need_recv_msg_id)
{
    u16 i = 0, j = 0;
    if (return_need_recv_msg_id.size() == 0)
        TestReport_HTML_StepDescription("无DUT需要发送的报文，请检查", Warning);

    else if (DUT_APPRxMsg[m].size() == 0)
        TestReport_HTML_StepDescription("DUT未发送任何APP报文", Fail);
    else
    {
        for (i = 0; i < return_need_recv_msg_id.size(); i++)
        {
            vector <TCANFD> test_app_msg;
            s64 T1, T2;
            float diff;
            for (j = 0; j < DUT_APPRxMsg[m].size(); j++)
            {
                if (return_need_recv_msg_id[i] == DUT_APPRxMsg[m][j].FIdentifier)
                    test_app_msg.push_back(DUT_APPRxMsg[m][j]);
                //log("%fs", test_app_msg[0].FTimeUs/1000000.0);

            }
            if (test_app_msg.size() == 0) {
                sprintf(actualResponse, "DUT未发送0x%X报文，无法校验映射报文停止时间", return_need_recv_msg_id[i]);
                TestReport_HTML_StepDescription(actualResponse, Fail);

            }
            else
            {
                T1 = DUT_NMTxMsg[m][DUT_NMTxMsg[m].size() - 1].FTimeUs;
                //T1 = test_app_msg[0].FTimeUs;
                T2 = test_app_msg[test_app_msg.size() - 1].FTimeUs;
                diff = (T2 - T1) / 1000.0;
                //log("%fms,T2:%fs,T1:%fs", diff,T2/1000000.0,T1/1000000.0);
                if (diff >= CanNmPnResetTimeMin && diff <= CanNmPnResetTimeMax) {
                    sprintf(actualResponse, "映射报文0x%X停止时间为%.2fms，TS发送最后一个相关PNC的NM报文时间戳%fs，停止报文时间戳%fs", return_need_recv_msg_id[i], diff, T1 / 1000000.0, T2 / 1000000.0);
                    TestReport_HTML_StepDescription(actualResponse, Pass);
                }
                else if (diff <= CanNmPnResetTimeMin) {
                    sprintf(actualResponse, "映射报文0x%X停止时间为%.2fms，TS发送最后一个相关PNC的NM报文时间戳%fs，停止报文时间戳%fs", return_need_recv_msg_id[i], diff, T1 / 1000000.0, T2 / 1000000.0);
                    TestReport_HTML_StepDescription(actualResponse, Warning);
                }
                else {
                    sprintf(actualResponse, "映射报文0x%X停止时间为%.2fms，TS发送最后一个相关PNC的NM报文时间戳%fs，停止报文时间戳%fs", return_need_recv_msg_id[i], diff, T1 / 1000000.0, T2 / 1000000.0);
                    TestReport_HTML_StepDescription(actualResponse, Fail);
                }

            }
        }
    }
}

void send_nm_msg_set_pncm_1(u8 m, u8 PNC)
{
    ClearAllNMVector();
    u16 test_id = nm_dut_send_id_min;
    u8 test_data[8] = { test_id & 0XFF,0x40 };
    set_InterestPNC_1(PNC, test_data);
    nm_msg.init_w_std_id(test_id, 8);
    nm_msg.FIdxChn = m;//0代表CAN1通道
    nm_msg.FFDProperties = 0;//必须要有这个
    memcpy(nm_msg.FData, test_data, 8);//设置data内容
    log("发送NM报文ID：0x%X，Data：0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X", nm_dut_send_id_min, test_data[0], test_data[1], test_data[2], test_data[3], test_data[4], test_data[5], test_data[6], test_data[7]);
    com.transmit_canfd_async(&nm_msg);
    com.add_cyclic_message_canfd(&nm_msg, CanNmMsgCycleTime);
}


void send_nm_msg1_set_pncm_1(u8 m, u8 PNC)
{
    ClearAllNMVector();
    u16 test_id = nm_dut_send_id_max;
    u8 test_data[8] = { test_id & 0XFF,0x40 };
    set_InterestPNC_1(PNC, test_data);
    nm_msg1.init_w_std_id(test_id, 8);
    nm_msg1.FIdxChn = m;//0代表CAN1通道
    nm_msg1.FFDProperties = 0;//必须要有这个
    memcpy(nm_msg1.FData, test_data, 8);//设置data内容
    log("发送NM报文ID：0x%X，Data：0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X", nm_dut_send_id_max, test_data[0], test_data[1], test_data[2], test_data[3], test_data[4], test_data[5], test_data[6], test_data[7]);
    com.transmit_canfd_async(&nm_msg1);
    com.add_cyclic_message_canfd(&nm_msg1, CanNmMsgCycleTime);
}

//输入m和pnc，检查第一帧发出的对应的1个pnc是1，其余是0
void check_PNC_is1_and_other_is0(u8 m, u8 PNC)
{
    u8 bytem = 0;
    u8 bitn = 0;
    if (PNC != 0)
    {
        if (PNC > 63)
        {
            TestReport_HTML_StepDescription("错误的PNC码，NM PDU中Byte0为Source node id，Byte1为CBV位，后续为6个Byte均为PNC位，首个PNC值为16，最后PNC值为63", Fail);
            return;
        }
        else if (PNC >= 56)
            bytem = 7;
        else if (PNC >= 48)
            bytem = 6;
        else if (PNC >= 40)
            bytem = 5;
        else if (PNC >= 32)
            bytem = 4;
        else if (PNC >= 24)
            bytem = 3;
        //std::bitset<4> b3{ "00000000" };
        //b3.set(bitn);
        else if (PNC >= 16)
            bytem = 2;
        else if (PNC != 0)
        {
            TestReport_HTML_StepDescription("错误的PNC码，NM PDU中Byte0为Source node id，Byte1为CBV位，后续为6个Byte均为PNC位，首个PNC值为16，最后PNC值为63", Fail);
            return;
        }

        bitn = PNC % 8;
        if (DUT_NMRxMsg[m].size() != 0)
            GET_BIT(DUT_NMRxMsg[m][0].FData[bytem], bitn);
        else
            TestReport_HTML_StepDescription("DUT未发出NM报文", Fail);
    }
}


//设置所有InterestPNC的bit=0
void set_all_InterestPNC_0(u8 m, u8* data)
{
    u8 bytem = 0;
    u8 bitn = 0;
    //test_data[2] = test_data[3] = test_data[4] = test_data[5] = test_data[6] = test_data[7] = 0;//不加的话会累计置1
    for (int i = 0; i < pnc_max_count_in_one_bus; i++)
    {
        if (bus_configuration[m].relevant_pnc[i] != 0)
        {
            if (bus_configuration[m].relevant_pnc[i] > 63)
            {
                TestReport_HTML_StepDescription("错误的PNC码，NM PDU中Byte0为Source node id，Byte1为CBV位，后续为6个Byte均为PNC位，首个PNC值为16，最后PNC值为63", Fail);
                return;
            }
            else if (bus_configuration[m].relevant_pnc[i] >= 56)
                bytem = 7;
            else if (bus_configuration[m].relevant_pnc[i] >= 48)
                bytem = 6;
            else if (bus_configuration[m].relevant_pnc[i] >= 40)
                bytem = 5;
            else if (bus_configuration[m].relevant_pnc[i] >= 32)
                bytem = 4;
            else if (bus_configuration[m].relevant_pnc[i] >= 24)
                bytem = 3;
            //std::bitset<4> b3{ "00000000" };
            //b3.set(bitn);
            else if (bus_configuration[m].relevant_pnc[i] >= 16)
                bytem = 2;
            else if (bus_configuration[m].relevant_pnc[i] != 0)
            {
                TestReport_HTML_StepDescription("错误的PNC码，NM PDU中Byte0为Source node id，Byte1为CBV位，后续为6个Byte均为PNC位，首个PNC值为16，最后PNC值为63", Fail);
                return;
            }

            bitn = bus_configuration[m].relevant_pnc[i] % 8;
            CLEAR_BIT(data[bytem], bitn);//这里只能改的是test_data[0]的内容
            //log("%d,%d,%d,%d",data[0],data[1],data[2],data[3]);
        }
    }
    // 2. 处理接收PNC (rx_pnc) - 新增逻辑
    for (int i = 0; i < pnc_max_count_in_one_bus; i++)
    {
        if (bus_configuration[m].rx_pnc[i] != 0)
        {
            if (bus_configuration[m].rx_pnc[i] > 63)
            {
                TestReport_HTML_StepDescription("错误的接收PNC码，NM PDU中Byte0为Source node id，Byte1为CBV位，后续为6个Byte均为PNC位，首个PNC值为16，最后PNC值为63", Fail);
                return;
            }
            else if (bus_configuration[m].rx_pnc[i] >= 56)
                bytem = 7;
            else if (bus_configuration[m].rx_pnc[i] >= 48)
                bytem = 6;
            else if (bus_configuration[m].rx_pnc[i] >= 40)
                bytem = 5;
            else if (bus_configuration[m].rx_pnc[i] >= 32)
                bytem = 4;
            else if (bus_configuration[m].rx_pnc[i] >= 24)
                bytem = 3;
            else if (bus_configuration[m].rx_pnc[i] >= 16)
                bytem = 2;
            else if (bus_configuration[m].rx_pnc[i] != 0)
            {
                TestReport_HTML_StepDescription("错误的接收PNC码，NM PDU中Byte0为Source node id，Byte1为CBV位，后续为6个Byte均为PNC位，首个PNC值为16，最后PNC值为63", Fail);
                return;
            }

            bitn = bus_configuration[m].rx_pnc[i] % 8;
            CLEAR_BIT(data[bytem], bitn);//清零接收PNC位
            //log("[接收PNC] 清零字节%d的bit%d (PNC%d)", bytem, bitn, bus_configuration[m].rx_pnc[i]);
        }
    }
}


//检查Active PNC gateway相关的第一帧NM报文pnc码是1，其余是0
void check_active_all_pnc_gateway_bit(u8 m)
{
    u8 j, k;
    u8 error_count = 0;
    if (DUT_NMRxMsg[m].size() != 0)
    {
        for (j = pnc_begin_byte; j <= pnc_end_byte; j++)
        {
            for (k = 0; k < 8; k++)
            {
                //if (bus_configuration[m].pnc2bytembitn[j][k] == 1)
                //	log("%d,j:%d,k:%d", bus_configuration[m].pnc2bytembitn[j][k], j, k);
                if (GET_BIT(DUT_NMRxMsg[m][0].FData[j], k) != bus_configuration[m].pnc2bytembitn[j][k])
                    error_count++;
            }
        }
        if (error_count == 0) {
            sprintf(actualResponse, "网段是Active PNC gateway，与该网段的PNC掩码相关的PNC为1，其他PNC为0，byte2-byte5分别为0x%X，0x%X，0x%X，0x%X", DUT_NMRxMsg[m][0].FData[2], DUT_NMRxMsg[m][0].FData[3], DUT_NMRxMsg[m][0].FData[4], DUT_NMRxMsg[m][0].FData[5]);
            TestReport_HTML_StepDescription(actualResponse, Pass);
        }


        else {
            sprintf(actualResponse, "网段是Active PNC gateway，与该网段的PNC掩码相关的PNC置位错误，byte2-byte5分别为0x%X，0x%X，0x%X，0x%X", DUT_NMRxMsg[m][0].FData[2], DUT_NMRxMsg[m][0].FData[3], DUT_NMRxMsg[m][0].FData[4], DUT_NMRxMsg[m][0].FData[5]);
            TestReport_HTML_StepDescription(actualResponse, Fail);
        }

    }
    else {
        sprintf(actualResponse, "网段是Active PNC gateway，DUT没有发出任何NM报文");
        TestReport_HTML_StepDescription(actualResponse, Fail);
    }

}

//检查Active PNC gateway相关的第一帧NM报文某个pnc码是1，其余是0
void check_active_pnc_gateway_bit(u8 m, u8* tx_data)
{
    if (DUT_NMRxMsg[m].size() != 0)
    {
        u8* rx_data = &DUT_NMRxMsg[m][0].FData[0];
        //log("%x,%x,%x,%x", rx_data[2], rx_data[3], rx_data[4], rx_data[5]);
        //log("%x,%x,%x,%x",tx_data[2], &tx_data[3], tx_data[4], tx_data[5]);
        //log("%x,%x,%x,%x", tx_data + pnc_begin_byte, tx_data + 3, tx_data + 4, tx_data + 5);
        //log("%x,%x,%x,%x", data1[2], data1[3], data1[4], data1[5], data1[6], data1[7]);
        if (memcmp(rx_data + pnc_begin_byte, tx_data + pnc_begin_byte, (pnc_end_byte + 1 - pnc_begin_byte)) == 0)
            TestInfoPrintPass("网段CAN%d，是Active PNC gateway，与该网段的PNC掩码相关的PNC为1，其他PNC为0，byte2-byte5分别为0x%X，0x%X，0x%X，0x%X", m + 1, DUT_NMRxMsg[m][0].FData[2], DUT_NMRxMsg[m][0].FData[3], DUT_NMRxMsg[m][0].FData[4], DUT_NMRxMsg[m][0].FData[5]);
        else
            TestInfoPrintFail("网段CAN%d，是Active PNC gateway，与该网段的PNC掩码相关的PNC置位错误，byte2-byte5分别为0x%X，0x%X，0x%X，0x%X", m + 1, DUT_NMRxMsg[m][0].FData[2], DUT_NMRxMsg[m][0].FData[3], DUT_NMRxMsg[m][0].FData[4], DUT_NMRxMsg[m][0].FData[5]);
    }
    else
        TestInfoPrintFail("网段CAN%d，是Active PNC gateway，DUT没有发出任何NM报文", m + 1);
}


//检查Passive PNC gateway的第一帧NM报文所有pnc码对应bit均是0
void check_passive_pnc_gateway_bit(u8 m)
{
    u8 j, k;
    u8 error_count = 0;
    if (DUT_NMRxMsg[m].size() != 0)
    {
        for (j = pnc_begin_byte; j <= pnc_end_byte; j++)
        {
            for (k = 0; k < 8; k++)
            {
                if (GET_BIT(DUT_NMRxMsg[m][0].FData[j], k) != 0)
                    error_count++;
            }
        }
        if (error_count == 0)
            TestInfoPrintPass("网段CAN%d，是Passive PNC gateway，与该网段的PNC掩码相关的PNC均为0，byte2-byte5分别为0x%X，0x%X，0x%X，0x%X", m + 1, DUT_NMRxMsg[m][0].FData[2], DUT_NMRxMsg[m][0].FData[3], DUT_NMRxMsg[m][0].FData[4], DUT_NMRxMsg[m][0].FData[5]);
        else
            TestInfoPrintFail("网段CAN%d，是Passive PNC gateway，与该网段的PNC掩码相关的PNC置位错误，byte2-byte5分别为0x%X，0x%X，0x%X，0x%X", m + 1, DUT_NMRxMsg[m][0].FData[2], DUT_NMRxMsg[m][0].FData[3], DUT_NMRxMsg[m][0].FData[4], DUT_NMRxMsg[m][0].FData[5]);
    }
    else
        TestInfoPrintFail("网段CAN%d，是Passive PNC gateway，DUT没有发出任何NM报文", m + 1);

}



//检查第一帧NM报文PNC gateway相关的pnc码是1，其余是0,用于hardware吧
void check_all_pnc_gateway_bit_is1(u8 m)
{
    u8 j, k;
    u8 error_count = 0;
    if (DUT_NMRxMsg[m].size() != 0)
    {
        for (j = pnc_begin_byte; j <= pnc_end_byte; j++)
        {
            for (k = 0; k < 8; k++)
            {
                //校验第一帧NM报文的所有bit位都与 pnc2bytembitn的值 一样，也就是所有PNC都置1
                if (GET_BIT(DUT_NMRxMsg[m][0].FData[j], k) != bus_configuration[m].pnc2bytembitn[j][k])
                    error_count++;
            }
        }
        if (error_count == 0)
            TestInfoPrintPass("与该网段CAN%d的PNC掩码相关的PNC为1，其他PNC为0，byte2-byte5分别为0x%X，0x%X，0x%X，0x%X，第一帧报文时间戳：%fs", m + 1, DUT_NMRxMsg[m][0].FData[2], DUT_NMRxMsg[m][0].FData[3], DUT_NMRxMsg[m][0].FData[4], DUT_NMRxMsg[m][0].FData[5], DUT_NMRxMsg[m][0].FTimeUs / 1000000.0);
        else
            TestInfoPrintFail("与该网段CAN%d的PNC掩码相关的PNC置位错误，byte2-byte5分别为0x%X，0x%X，0x%X，0x%X，第一帧报文时间戳：%fs", m + 1, DUT_NMRxMsg[m][0].FData[2], DUT_NMRxMsg[m][0].FData[3], DUT_NMRxMsg[m][0].FData[4], DUT_NMRxMsg[m][0].FData[5], DUT_NMRxMsg[m][0].FTimeUs / 1000000.0);
    }
    else
    {
        TestInfoPrintFail("网段CAN%d，DUT未发送NM报文，无法校验PNC", m + 1);
        return;
    }
}

//检查所有NM报文PNC gateway相关的pnc码是1，其余是0
void check_all_nm_msg_pnc_is_1(u8 m)
{
    u8 i, j, k;
    u8 error_count = 0;
    if (DUT_NMRxMsg[m].size() != 0)
    {
        for (i = 0; i < DUT_NMRxMsg[m].size(); i++)
        {
            for (j = pnc_begin_byte; j <= pnc_end_byte; j++)
            {
                for (k = 0; k < 8; k++)
                {
                    if (GET_BIT(DUT_NMRxMsg[m][i].FData[j], k) != bus_configuration[m].pnc2bytembitn[j][k])
                        error_count++;
                }
            }
        }
        if (error_count == 0)
            TestInfoPrintPass("与该网段CAN%d的PNC掩码相关的PNC为1，其他PNC为0，byte2-byte5分别为0x%X，0x%X，0x%X，0x%X，第一帧NM报文时间戳：%fs", m + 1, DUT_NMRxMsg[m][0].FData[2], DUT_NMRxMsg[m][0].FData[3], DUT_NMRxMsg[m][0].FData[4], DUT_NMRxMsg[m][0].FData[5], DUT_NMRxMsg[m][0].FTimeUs / 1000000.0);
        else
            TestInfoPrintFail("与该网段CAN%d的PNC掩码相关的PNC置位错误，byte2-byte5分别为0x%X，0x%X，0x%X，0x%X，第一帧NM报文时间戳：%fs，错误PNC置位数量：%d", m + 1, DUT_NMRxMsg[m][0].FData[2], DUT_NMRxMsg[m][0].FData[3], DUT_NMRxMsg[m][0].FData[4], DUT_NMRxMsg[m][0].FData[5], DUT_NMRxMsg[m][0].FTimeUs / 1000000.0, error_count);
    }
    else
    {
        TestInfoPrintFail("网段CAN%d，DUT未发送NM报文，无法校验PNC", m + 1);
        return;
    }

}

s8 check_uncom(u8 cann, u8 type, u32 check_time) {
    s8 res = 0;
    float Cur;
    bool Flag = false;
    for (int i = 0;i < check_time / 100;i++) {
        app.wait(100, "");
        Cur = main_power::instance()->Read_Cur();
        log("The current is %5.3fmA", Cur * 1000);
        if (Cur == 0) {
            Flag = true;
            break;
        }
    }
    if (Flag == true)
    {
        TestInfoPrintPass("网段CAN%d，DUT当前未发送报文，已停止通信", cann + 1);
        res = 1;
    }
    else
    {
        TestInfoPrintFail("网段CAN%d，DUT发送报文，未停止通信，最后一帧报文ID:0x%X，时间%fs", cann + 1, OutErrorAndDiagRxMsg[cann][OutErrorAndDiagRxMsg[cann].size() - 1].FIdentifier, OutErrorAndDiagRxMsg[cann][OutErrorAndDiagRxMsg[cann].size() - 1].FTimeUs / 1000000.0);
        res = -3;
    }

    return res;
}

/*
检查通信
cann：通道几，从0开始；
type：当0开始通信，1是持续通信，或2停止通信，3为不通信，1和3等待10s自动退出
check_time：检测时间，>=500ms。只有停止通信后会立马返回，不会等完check_time时间，每500ms检测一次
*/
s8 check_com(u8 cann, u8 type, u32 check_time)
{
    u8 i = 0;
    u8 stop_com_flag = 0;
    s8 res = 0;
    switch (type)
    {
    case 0:
    {
        u16 wait_time = 0;
        while (OutErrorAndDiagRxMsg[cann].size() == 0)
        {
            app.wait(1, "");//实际测下来是每2ms差不多，wait_time++
            wait_time++;
            //log(" %d", wait_time);
            if ((wait_time * 2) >= check_time)
            {
                TestInfoPrintFail("网段CAN%d，DUT未发送报文，%.2fs内未开始通信", cann + 1, check_time / 1000.0);
                res = 0;
                return res;
            }
        }
        TestInfoPrintPass("网段CAN%d，DUT发送报文0x%X，%.2fs内开始通信，第一帧报文ID:0x%X，时间%fs", cann + 1, OutErrorAndDiagRxMsg[cann][0].FIdentifier, check_time / 1000.0, OutErrorAndDiagRxMsg[cann][0].FIdentifier, OutErrorAndDiagRxMsg[cann][0].FTimeUs / 1000000.0);
        res = 1;
        break;
    }

    case 1:
    {
        for (i = 0; i < (check_time / 500); i++)
        {
            OutErrorAndDiagRxMsg[cann].clear();;
            app.wait(500, "");
            if (OutErrorAndDiagRxMsg[cann].size() == 0)
            {
                stop_com_flag = 1;
                //break;
            }
        }
        if (stop_com_flag == 0 && OutErrorAndDiagRxMsg[cann].size() != 0)
        {
            TestInfoPrintPass("网段CAN%d，DUT持续通信，最后一帧报文ID:0x%X，时间%fs", cann + 1, OutErrorAndDiagRxMsg[cann][OutErrorAndDiagRxMsg[cann].size() - 1].FIdentifier, OutErrorAndDiagRxMsg[cann][OutErrorAndDiagRxMsg[cann].size() - 1].FTimeUs / 1000000.0);
            res = 1;
        }
        else
        {
            TestInfoPrintFail("网段CAN%d，DUT停止通信", cann + 1);
            res = -1;
        }
        break;
    }
    case 2:
    {
        //这里=是为了确保在下一个500ms内停止
        for (i = 0; i <= (check_time / 500); i++)
        {
            OutErrorAndDiagRxMsg[cann].clear();;
            app.wait(500, "");
            if (OutErrorAndDiagRxMsg[cann].size() == 0)
            {
                stop_com_flag = 1;
                break;
            }
        }
        if (stop_com_flag == 1)
        {
            TestInfoPrintPass("网段CAN%d，DUT停止通信", cann + 1);
            res = 1;
        }
        else
        {
            TestInfoPrintFail("网段CAN%d，DUT未停止通信，最后一帧报文ID:0x%X，时间%fs", cann + 1, OutErrorAndDiagRxMsg[cann][OutErrorAndDiagRxMsg[cann].size() - 1].FIdentifier, OutErrorAndDiagRxMsg[cann][OutErrorAndDiagRxMsg[cann].size() - 1].FTimeUs / 1000000.0);
            res = -2;
        }
        break;
    }
    case 3:
    {
        app.wait(check_time, "");
        /*float Cur;
        bool Flag = false;
        for (int i = 0;i < check_time/100;i++) {
            app.wait(100,"");
            Cur = main_power::instance()->Read_Cur();
            log("The current is %5.3fmA", Cur * 1000);
            if (Cur ==0) {
                Flag = true;
                break;
            }
        }
        if (Flag == true)
        {
            TestInfoPrintPass("网段CAN%d，DUT当前未发送报文，已停止通信", cann + 1);
            res = 1;
        }
        else
        {
            TestInfoPrintFail("网段CAN%d，DUT发送报文，未停止通信，最后一帧报文ID:0x%X，时间%fs", cann + 1, OutErrorAndDiagRxMsg[cann][OutErrorAndDiagRxMsg[cann].size() - 1].FIdentifier, OutErrorAndDiagRxMsg[cann][OutErrorAndDiagRxMsg[cann].size() - 1].FTimeUs / 1000000.0);
            res = -3;
        }
        break;*/
        if (OutErrorAndDiagRxMsg[cann].size() < 10)
        {
            TestInfoPrintPass("网段CAN%d，DUT未发送报文，未开始通信", cann + 1);
            res = 1;
        }
        else
        {
            TestInfoPrintFail("网段CAN%d，DUT发送报文，开始通信，最后一帧报文ID:0x%X，时间%fs", cann + 1, OutErrorAndDiagRxMsg[cann][OutErrorAndDiagRxMsg[cann].size() - 1].FIdentifier, OutErrorAndDiagRxMsg[cann][OutErrorAndDiagRxMsg[cann].size() - 1].FTimeUs / 1000000.0);
            res = -3;
        }
        break;
    }
    default:
        break;
    }
    return res;
}

void wait_the_first_nm_msg_and_check(u8 m)
{
    u16 check_time = 5000;
    u16 wait_time = 0;
    while (DUT_NMRxMsg[m].size() == 0)
    {
        app.wait(1, "");//实际测下来是每2ms差不多，wait_time++
        wait_time++;
        //log(" %d", wait_time);
        if ((wait_time * 2) >= check_time)
        {
            TestInfoPrintFail("网段CAN%d，DUT未发送报文，%.2fs内未开始通信", m + 1, check_time / 1000.0);
            return;
        }
    }
    TestInfoPrintPass("网段CAN%d，DUT发送报文0x%X，%.2fs内开始通信，第一帧报文ID:0x%X，时间%fs", m + 1, DUT_NMRxMsg[m][0].FIdentifier, check_time / 1000.0, DUT_NMRxMsg[m][0].FIdentifier, DUT_NMRxMsg[m][0].FTimeUs / 1000000.0);

}

//检查DUT停发NM报文：当前-上一帧NM>CanNmMsgCycleTimeMax,CanNmMsgCycleTimeMaxms一次，每次清空DUT_NM_RX,最多10次
void check_dut_stop_send_nm_msg(u8 m)
{
    u8 i = 0;
    u16 check_time = CanNmMsgCycleTimeMax * 10;
    bool stop_com_flag = 0;
    for (i; i < (check_time / CanNmMsgCycleTimeMax); i++)
    {
        //DUT_NMRxMsg[m].clear();//不能用这个，用的是
        app.wait(CanNmMsgCycleTimeMax, "");
        app.get_timestamp(&current_time);
        //if (DUT_NMRxMsg[m].size() == 0)
        if (DUT_NMRxMsg[m].size() != 0)
        {
            if ((current_time - DUT_NMRxMsg[m][DUT_NMRxMsg[m].size() - 1].FTimeUs) / 1000 > CanNmMsgCycleTimeMax)
            {
                stop_com_flag = 1;
                break;
            }
        }
    }
    if (stop_com_flag == 1 || DUT_NMRxMsg[m].size() == 0)
        TestInfoPrintPass("网段CAN%d，DUT停止发送NM报文", m + 1);
    else
        TestInfoPrintFail("网段CAN%d，DUT未停止发送NM报文，最后一帧NM报文ID:0x%X，时间%fs", m + 1, DUT_NMRxMsg[m][DUT_NMRxMsg[m].size() - 1].FIdentifier, DUT_NMRxMsg[m][DUT_NMRxMsg[m].size() - 1].FTimeUs / 1000000.0);

}

//检查DUT停发NM报文：当前-上一帧NM>CanNmMsgCycleTimeMax,100ms检测一次
void check_dut_stop_send_nm_msg(u8 m, u16 check_time)
{
    u8 i = 0;
    bool stop_com_flag = 0;
    for (i; i < (check_time / 100); i++)
    {
        //DUT_NMRxMsg[m].clear();//不能用这个，用的是
        app.wait(100, "");
        app.get_timestamp(&current_time);
        //if (DUT_NMRxMsg[m].size() == 0)
        if (DUT_NMRxMsg[m].size() != 0)
        {
            if ((current_time - DUT_NMRxMsg[m][DUT_NMRxMsg[m].size() - 1].FTimeUs) / 1000 > CanNmMsgCycleTimeMax)
            {
                stop_com_flag = 1;
                break;
            }
        }
    }
    if (stop_com_flag == 1 || DUT_NMRxMsg[m].size() == 0)
        TestInfoPrintPass("网段CAN%d，DUT停止发送NM报文", m + 1);
    else
        TestInfoPrintFail("网段CAN%d，DUT未停止发送NM报文，最后一帧NM报文ID:0x%X，时间%fs", m + 1, DUT_NMRxMsg[m][DUT_NMRxMsg[m].size() - 1].FIdentifier, DUT_NMRxMsg[m][DUT_NMRxMsg[m].size() - 1].FTimeUs / 1000000.0);

}

//当0开始通信，1是持续通信，或2停止通信，3为不通信，1和3等待10s自动退出
void check_com(u8 m, u8 type)
{
    u8 i = 0;
    u8 stop_com_flag = 0;
    //u16 check_time = 10000;
    u16 check_time = 10000;
    switch (type)
    {
    case 0:
    {
        u16 wait_time = 0;
        s64 begin_time = 0;
        app.get_timestamp(&begin_time);
        while (OutErrorAndDiagRxMsg[m].size() == 0)
        {
            //app.wait(100, "");//实际测下来是每2ms差不多，wait_time++,这种时间可能不太准
            //wait_time++;
            ////log(" %d", wait_time);
            //if ((wait_time * 2) >= check_time)
            //{
            //	TestInfoPrintFail("网段CAN%d，DUT未发送报文，%.2fs内未开始通信", m + 1, check_time / 1000.0);
            //	//issue_flag = -1;
            //	return;
            //}

            app.get_timestamp(&current_time);
            if (((current_time - begin_time) / 1000) >= check_time)
            {
                TestInfoPrintFail("网段CAN%d，DUT未发送报文，%.2fs内未开始通信", m + 1, check_time / 1000.0);
                return;
            }
            app.wait(100, "");

        }
        TestInfoPrintPass("网段CAN%d，DUT发送报文0x%X，%.2fs内开始通信，第一帧报文ID:0x%X，时间%fs", m + 1, OutErrorAndDiagRxMsg[m][0].FIdentifier, check_time / 1000.0, OutErrorAndDiagRxMsg[m][0].FIdentifier, OutErrorAndDiagRxMsg[m][0].FTimeUs / 1000000.0);
        break;
    }
    case 1:
    {
        for (i; i < (check_time / 500); i++)
        {
            OutErrorAndDiagRxMsg[m].clear();
            app.wait(500, "");
            if (OutErrorAndDiagRxMsg[m].size() == 0)
            {
                stop_com_flag = 1;
                //break;
            }
        }
        if (stop_com_flag == 0 && OutErrorAndDiagRxMsg[m].size() != 0)
            TestInfoPrintPass("网段CAN%d，DUT持续通信，最后一帧报文ID:0x%X，时间%fs", m + 1, OutErrorAndDiagRxMsg[m][OutErrorAndDiagRxMsg[m].size() - 1].FIdentifier, OutErrorAndDiagRxMsg[m][OutErrorAndDiagRxMsg[m].size() - 1].FTimeUs / 1000000.0);
        else
        {
            TestInfoPrintFail("网段CAN%d，DUT停止通信", m + 1);
            //issue_flag = -2;
        }
        break;
    }

    case 2:
    {
        //u8 i = 0;
        //u8 stop_com_flag = 0;
        for (i; i < (check_time / 500); i++)
        {
            OutErrorAndDiagRxMsg[m].clear();
            app.wait(1000, "");
            if (OutErrorAndDiagRxMsg[m].size() == 0)
            {
                stop_com_flag = 1;
                break;
            }
        }
        if (stop_com_flag == 1)
            TestInfoPrintPass("网段CAN%d，DUT停止通信", m + 1);
        else
        {
            TestInfoPrintFail("网段CAN%d，DUT未停止通信，最后一帧报文ID:0x%X，时间%fs", m + 1, OutErrorAndDiagRxMsg[m][OutErrorAndDiagRxMsg[m].size() - 1].FIdentifier, OutErrorAndDiagRxMsg[m][OutErrorAndDiagRxMsg[m].size() - 1].FTimeUs / 1000000.0);
            //issue_flag = -3;
        }
        break;
    }
    case 3:
    {
        app.wait(check_time, "");
        if (OutErrorAndDiagRxMsg[m].size() == 0)
            TestInfoPrintPass("网段CAN%d，DUT未发送报文，未开始通信", m + 1);
        else
        {
            TestInfoPrintFail("网段CAN%d，DUT发送报文，开始通信", m + 1);
            //issue_flag = -4;
        }
        break;
    }
    default:
        break;
    }
}

//关闭仿真，开启供电，如果需要额外条件进入睡眠，满足
void close_simulate_network_and_begin_power_on_and_check(u8 m)
{
    s8 res = 0;
    ClearAllNMVector();
    //BoardCard_Init();
    //MainPwr_to_NBATGND(ON);
    ////DUT KL30\KL31上电
    //KL30(ON);
    //KL31(ON);

    //SetVoltageAndRead(12, 0, 0);
    SetVoltage(12, 3);
    MainPower_ON();
    AppWait(1000, "");
    if (gCEMFlg == 0) gSleepTime = 300000;
    //log("gSleepTimegSleepTimegSleepTimegSleepTimegSleepTime:%d", gSleepTime);
    if (bus_configuration[m].if_ned_extra_condition_to_go_to_sleep == 1)
        TestWaitForCurInRange(gLowPwrCur, gSleepTime);
        //app.wait(bus_configuration[m].if_have_sleep_state_enter_sleep_wait_time, "");//ZCUM需要21s休下去
    ClearAllNMVector();
    res = check_com(m, 3, 500);
    //if (res == -3)
    //{
    //	strcpy(openissue_cfg.problem_classification, "总线通信");
    //	strcpy(openissue_cfg.title, "开启供电电源后有通信");
    //	strcpy(openissue_cfg.problem_description, "步骤：关闭网络仿真，开启供电电源，检查网段处于休眠；实际：未休眠");
    //	output_openissue(m,__func__);
    //}
}


void close_simulate_network_and_power_off_and_check(u8 m)
{
    ClearAllNMVector();
    KL30(off);
    check_com(m, 3, 0);
}

void close_simulate_network_and_begin_power_on(u8 m)
{
    ClearAllNMVector();
    KL30(on);
    //wait(23000);//ZCUM需要21s休下去
    ClearAllNMVector();

}

void close_simulate_network_and_power_off(u8 m)
{
    ClearAllNMVector();
    KL30(off);
}

void close_self_ack_and_begin_power_on_and_check(u8 m)
{
    ClearAllNMVector();
    //函数关闭ACK自应答
    KL30(on);
    check_com(m, 3, 0);
}

u8* return_data_set_all_pnc_is1(u8 msg_id, u8 CBV)
{
    //u8 test_data[pnc_end_byte - pnc_begin_byte + 1] = {0};
    u8 test_data[8] = { 0 };
    test_data[0] = msg_id & 0xFF;
    test_data[1] = CBV;
    for (u8 i = pnc_begin_byte; i <= pnc_end_byte; i++)
        test_data[i] = 0xff;
    return test_data;
}

//数组本身就是指针，用的地址
void return_data_set_all_pnc_is1(u8 msg_id, u8 CBV, u8 test_data[8])
{
    //u8 test_data[pnc_end_byte - pnc_begin_byte + 1] = {0};
    //u8 test_data[8] = { 0 };
    test_data[0] = msg_id & 0xFF;
    test_data[1] = CBV;
    for (u8 i = pnc_begin_byte; i <= pnc_end_byte; i++)
        test_data[i] = 0xff;

}

void send_nm_msg(u8 m, u16 test_id, u8* test_data)
{
    ClearAllNMVector();
    nm_msg.init_w_std_id(test_id, 8);
    nm_msg.FIdxChn = m;//0代表CAN1通道
    nm_msg.FFDProperties = 0;//必须要有这个
    memcpy(nm_msg.FData, test_data, 8);//设置data内容
    com.transmit_canfd_async(&nm_msg);
    com.add_cyclic_message_canfd(&nm_msg, CanNmMsgCycleTime);
}

void send_nm_msg(u8 m, u8* test_data)
{
    ClearAllNMVector();
    nm_msg.init_w_std_id(nm_dut_send_id_min, 8);
    nm_msg.FIdxChn = m;//0代表CAN1通道
    nm_msg.FFDProperties = 0;//必须要有这个
    memcpy(nm_msg.FData, test_data, 8);//设置data内容
    com.transmit_canfd_async(&nm_msg);
    com.add_cyclic_message_canfd(&nm_msg, CanNmMsgCycleTime);
}

void send_nm_msg_pni_and_all_pnc_is1(u8 m, u16 test_id)
{
    ClearAllNMVector();
    //u8* tmp = return_data_set_all_pnc_is1(test_id, 0x40);
    //u8 test_data[8] = { tmp[0],tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7] };
    u8 test_data[8] = { 0 };
    //u8 test_data[8] = { test_id & 0xFF, 0x40,0xFF,0xFF, 0xFF, 0xFF};
    return_data_set_all_pnc_is1(test_id, 0x40, test_data);
    //log("0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x", test_data[0], test_data[1], test_data[2], test_data[3], test_data[4], test_data[5], test_data[6], test_data[7]);

    nm_msg.init_w_std_id(test_id, 8);
    nm_msg.FIdxChn = m;//0代表CAN1通道
    nm_msg.FFDProperties = 0;//必须要有这个
    memcpy(nm_msg.FData, test_data, 8);//设置data内容

    com.transmit_canfd_async(&nm_msg);
    com.add_cyclic_message_canfd(&nm_msg, CanNmMsgCycleTime);
}

//先快发，等待CanNmImmediateNmTransmissions * CanNmImmediateNmCycleTime时间后，常规发；注意已经等待的时间
void send_nm_msg_pni_and_all_pnc_is1_fast2nor(u8 m, u16 test_id)
{
    ClearAllNMVector();
    u8 test_data[8] = { 0 };
    return_data_set_all_pnc_is1(test_id, 0x40, test_data);
    nm_msg.init_w_std_id(test_id, 8);
    nm_msg.FIdxChn = m;//0代表CAN1通道
    nm_msg.FFDProperties = 0;//必须要有这个
    memcpy(nm_msg.FData, test_data, 8);//设置data内容
    com.transmit_canfd_async(&nm_msg);
    com.add_cyclic_message_canfd(&nm_msg, CanNmImmediateNmTransmissions);
    app.wait(CanNmImmediateNmTransmissions * CanNmImmediateNmCycleTime, "");
    com.del_cyclic_message_canfd(&nm_msg);
    com.add_cyclic_message_canfd(&nm_msg, CanNmMsgCycleTime);
}

//先快发，等待CanNmImmediateNmTransmissions * CanNmImmediateNmCycleTime时间后，常规发；注意已经等待的时间
void send_nm_msg_pni_and_all_pnc_is1_fast2nor(u8 m)
{
    ClearAllNMVector();
    u8 test_data[8] = { nm_dut_send_id_min & 0XFF,0x40,0xff,0xff,0xff,0xff,0xff,0xff };
    nm_msg.init_w_std_id(nm_dut_send_id_min, 8);
    nm_msg.FIdxChn = m;//0代表CAN1通道
    nm_msg.FFDProperties = 0;//必须要有这个
    memcpy(nm_msg.FData, test_data, 8);//设置data内容
    com.transmit_canfd_async(&nm_msg);
    com.add_cyclic_message_canfd(&nm_msg, CanNmImmediateNmTransmissions);
    app.wait(CanNmImmediateNmTransmissions * CanNmImmediateNmCycleTime, "");
    com.del_cyclic_message_canfd(&nm_msg);
    com.add_cyclic_message_canfd(&nm_msg, CanNmMsgCycleTime);
}

//不行，或者一起发60ms取消
void send_nm_msg_case_CAN_NM_PNC_reception(u8 m, u16 cycle_ms)
{
    ClearAllNMVector();
    TCANFD ALL_PN_msg, No_PN_msg, No_PN2_msg;
    u16 ALL_PN_id = 0x53f;
    u16 No_PN_id = 0x500;
    u16 No_PN2_id = 0x53e;

    u8 ALL_PN_data[8] = { ALL_PN_id & 0XFF,0x40,0xff,0xff,0xff,0xff,0xff,0xff };
    u8 No_PN_data[8] = { No_PN_id & 0XFF,0x40,0xff,0xff,0xff,0xff,0xff,0xff };
    u8 No_PN2_data[8] = { No_PN2_id & 0XFF,0x40 ,0xff,0xff,0xff,0xff,0xff,0xff };

    ALL_PN_msg.init_w_std_id(ALL_PN_id, 8);
    ALL_PN_msg.FIdxChn = m;//0代表CAN1通道
    ALL_PN_msg.FFDProperties = 0;//必须要有这个
    memcpy(ALL_PN_msg.FData, ALL_PN_data, 8);//设置data内容
    ALL_PN_msg.FIdxChn = m;//0代表CAN1通道
    ALL_PN_msg.FFDProperties = 0;//必须要有这个

    No_PN_msg.init_w_std_id(No_PN_id, 8);
    No_PN_msg.FIdxChn = m;//0代表CAN1通道
    No_PN_msg.FFDProperties = 0;//必须要有这个
    memcpy(No_PN_msg.FData, No_PN_data, 8);//设置data内容
    No_PN_msg.FIdxChn = m;//0代表CAN1通道
    No_PN_msg.FFDProperties = 0;//必须要有这个

    No_PN2_msg.init_w_std_id(No_PN2_id, 8);
    No_PN2_msg.FIdxChn = m;//0代表CAN1通道
    No_PN2_msg.FFDProperties = 0;//必须要有这个
    memcpy(No_PN2_msg.FData, No_PN2_data, 8);//设置data内容
    No_PN2_msg.FIdxChn = m;//0代表CAN1通道
    No_PN2_msg.FFDProperties = 0;//必须要有这个

    com.transmit_canfd_async(&ALL_PN_msg);
    com.transmit_canfd_async(&No_PN_msg);
    com.transmit_canfd_async(&No_PN2_msg);

    com.add_cyclic_message_canfd(&ALL_PN_msg, cycle_ms);
    com.add_cyclic_message_canfd(&No_PN_msg, cycle_ms);
    com.add_cyclic_message_canfd(&No_PN2_msg, cycle_ms);

    check_com(m, 1, 60000);
    //app.wait(60000,"");
    com.del_cyclic_message_canfd(&ALL_PN_msg);//可以这样取消，放到外面不行
    com.del_cyclic_message_canfd(&No_PN_msg);//可以这样取消，放到外面不行
    com.del_cyclic_message_canfd(&No_PN2_msg);//可以这样取消，放到外面不行
}


void send_nm_msg_fast2nor(u8 m, u16 test_id, u8* test_data)
{
    ClearAllNMVector();
    nm_msg.init_w_std_id(test_id, 8);
    nm_msg.FIdxChn = m;//0代表CAN1通道
    nm_msg.FFDProperties = 0;//必须要有这个
    memcpy(nm_msg.FData, test_data, 8);//设置data内容
    com.transmit_canfd_async(&nm_msg);
    com.add_cyclic_message_canfd(&nm_msg, CanNmImmediateNmTransmissions);
    app.wait(CanNmImmediateNmTransmissions * CanNmImmediateNmCycleTime, "");
    com.del_cyclic_message_canfd(&nm_msg);
    com.add_cyclic_message_canfd(&nm_msg, CanNmMsgCycleTime);

}

void send_nm_pni_and_all_pnc_frame(u8 m, u16 test_id, u8* test_data)
{
    ClearAllNMVector();
    nm_msg.init_w_std_id(test_id, 8);
    nm_msg.FIdxChn = m;//0代表CAN1通道
    nm_msg.FFDProperties = 0;//必须要有这个
    memcpy(nm_msg.FData, test_data, 8);//设置data内容
    com.transmit_canfd_async(&nm_msg);
}

void send_app_msg_frame(u8 m)
{
    u16 test_id = 1;
    ClearAllNMVector();
    u8 test_data[8] = { 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    app_msg.init_w_std_id(test_id, 8);
    app_msg.FIdxChn = m;//0代表CAN1通道
    app_msg.FFDProperties = 0;//必须要有这个
    memcpy(app_msg.FData, test_data, 8);//设置data内容
    com.transmit_canfd_async(&app_msg);
}

void send_app_msg(u8 m)
{
    u16 test_id = 1;
    ClearAllNMVector();
    app_msg.init_w_std_id(test_id, 8);
    app_msg.FIdxChn = m;//0代表CAN1通道
    app_msg.FFDProperties = 0;//必须要有这个
    com.add_cyclic_message_canfd(&app_msg, 1);
}

void send_app_msg_200ums(u8 m)
{
    u16 test_id = 1;
    ClearAllNMVector();
    app_msg.init_w_std_id(test_id, 8);
    app_msg.FIdxChn = m;//0代表CAN1通道
    app_msg.FFDProperties = 0;//必须要有这个
    com.add_cyclic_message_canfd(&app_msg, 0.2);
}




void send_app_msg(u8 m, u16 cycle)
{
    u16 test_id = 1;
    u8 test_data[8] = { 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ClearAllNMVector();
    app_msg.init_w_std_id(test_id, 8);
    app_msg.FIdxChn = m;//0代表CAN1通道
    app_msg.FFDProperties = 0;//必须要有这个
    memcpy(app_msg.FData, test_data, 8);//设置data内容
    com.add_cyclic_message_canfd(&app_msg, cycle);
}



void send_app_msg(u8 m, u16 id, u16 cycle)
{
    u16 test_id = id;
    u8 test_data[8] = { 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ClearAllNMVector();
    app_msg.init_w_std_id(test_id, 8);
    app_msg.FIdxChn = m;//0代表CAN1通道
    app_msg.FFDProperties = 0;//必须要有这个
    memcpy(app_msg.FData, test_data, 8);//设置data内容
    com.add_cyclic_message_canfd(&app_msg, cycle);
}

void stop_send_app_msg()
{
    com.del_cyclic_message_canfd(&app_msg);
}

//适用于遍历所有硬件唤醒，或指定某种硬件唤醒
void trigger_hardware_wakeup(u8 m, enum All_wakeup_way wakeup_way)
{
    ClearAllNMVector();
    switch (wakeup_way)
    {
    case hardware1:
    {
        KL15(on);
        break;
    }
    case hardware2:
    {
        //另一种硬件唤醒
        KL15(on);
        break;
    }
    default:
        break;
    }

}

//适用指定第一种硬件唤醒
void trigger_hardware_wakeup(u8 m)
{
    send_nm_msg_pni_and_all_pnc_is1_fast2nor(m, nm_dut_send_id_min);
    trigger_hardware_wakeup(m, bus_configuration[m].hardware_wakeup_way[0]);
}

//触发唤醒：如果支持网络唤醒，则发PNI和PNC置位的NM报文；如果支持硬件唤醒，触发第一个硬线唤醒事件
void trigger_wakeup(u8 m)
{
    //ClearAllNMVector();
    if (bus_configuration[m].if_support_network_wakeup == 1)
        //send_nm_msg_pni_and_all_pnc_is1(m, nm_dut_send_id_min);
        send_nm_msg_pni_and_all_pnc_is1_fast2nor(m, nm_dut_send_id_min);
    else if (bus_configuration[m].if_support_hardware_wakeup)
        trigger_hardware_wakeup(m);
    else
        return;
}

//适用于遍历所有硬件唤醒
void stop_trigger_hardware_wakeup(u8 m, enum All_wakeup_way wakeup_way)
{
    switch (wakeup_way)
    {
    case hardware1:
    {
        KL15(off);
        break;
    }
    case hardware2:
    {
        //另一种硬件唤醒
        KL15(off);
        break;
    }
    default:
        break;
    }

}


//适用于断开所有硬件唤醒
void stop_trigger_all_hardware_wakeup(u8 m)
{
    //stop_send_nm_msg();
    for (u8 i = 0; i < bus_configuration[m].hardware_wakeup_count; i++)
    {
        stop_trigger_hardware_wakeup(m, bus_configuration[m].hardware_wakeup_way[i]);
    }
}

//适用于遍历所有硬件唤醒
void stop_trigger_hardware_wakeup(u8 m)
{
    //stop_send_nm_msg();
    stop_trigger_hardware_wakeup(m, bus_configuration[m].hardware_wakeup_way[0]);
}

//停止触发唤醒：如果支持网络唤醒，则停发NM报文；如果支持硬件唤醒，KL15（off）
void stop_trigger_wakeup(u8 m)
{
    if (bus_configuration[m].if_support_network_wakeup == 1)
        com.del_cyclic_message_canfd(&nm_msg);
    else if (bus_configuration[m].if_support_hardware_wakeup)
        stop_trigger_hardware_wakeup(m);
    else
        return;
}

void stop_send_nm_msg()
{
    com.del_cyclic_message_canfd(&nm_msg);
}

void stop_send_nm_msg1()
{
    com.del_cyclic_message_canfd(&nm_msg1);
}


void stop_send_diag_msg()
{
    com.del_cyclic_message_canfd(&diag_msg);
}
//注意这里的test_data传的是指针，设置某个InterestPNC的bit=1
void set_InterestPNC_1(u8 PNC, u8* test_data)
{
    u8 bytem = 0;
    u8 bitn = 0;
    test_data[2] = test_data[3] = test_data[4] = test_data[5] = test_data[6] = test_data[7] = 0;//不加的话会累计置1
    if (PNC != 0)
    {
        if (PNC > 63)
        {
            TestReport_HTML_StepDescription("错误的PNC码，NM PDU中Byte0为Source node id，Byte1为CBV位，后续为6个Byte均为PNC位，首个PNC值为16，最后PNC值为63", Fail);
            return;
        }
        else if (PNC >= 56)
            bytem = 7;
        else if (PNC >= 48)
            bytem = 6;
        else if (PNC >= 40)
            bytem = 5;
        else if (PNC >= 32)
            bytem = 4;
        else if (PNC >= 24)
            bytem = 3;
        //std::bitset<4> b3{ "00000000" };
        //b3.set(bitn);
        else if (PNC >= 16)
            bytem = 2;
        else if (PNC != 0)
        {
            TestReport_HTML_StepDescription("错误的PNC码，NM PDU中Byte0为Source node id，Byte1为CBV位，后续为6个Byte均为PNC位，首个PNC值为16，最后PNC值为63", Fail);
            return;
        }

        bitn = PNC % 8;
        SET_BIT(test_data[bytem], bitn);
    }
}
//停止通信后，等待T_wait_bus时间，即进入休眠
void check_enter_wakeup(u8 m)
{
    check_com(m, 2);
    app.wait(CanNmWaitBusSleepTime, "");
}


void check_diff_case_CAN_Timing_at_network_wake_up(u8 m, s64 T2, s64 T1, u16 actural_id)
{
    u16 test_state[3] = { 0 };//[0]为带电状态，[1]为第一帧为NM状态，[2]为是PNC gateway状态
    double current;
    double with_power_current = 0;//A
    current = main_power::instance()->Read_Cur();
    log("%.2fA", current);
    u16 test_case[8][4] = {
        {1,1,1,TCANEventWakeUpToApp },
        {0,1,1,TCANPowerWakeUpToApp },
        {1,1,0,(TCANEventWakeUpToApp + CanNmMsgCycleOffset) },
        {0,1,0,(TCANPowerWakeUpToApp + CanNmMsgCycleOffset) },
        {1,0,0,TCANEventWakeUpToApp },
        {0,0,0,TCANPowerWakeUpToApp },
        {1,0,1,TCANEventWakeUpToApp },
        {0,0,1,TCANPowerWakeUpToApp }

    };

    if (T2 == -1)
    {
        TestInfoPrintFail("DUT没有发送报文，无法计算");
        return;
    }
    else
    {

        if (current >= with_power_current)
            test_state[0] = 1;
        if (actural_id >= nm_dut_send_id_min && actural_id <= nm_dut_send_id_max)
            test_state[1] = 1;
        if (bus_configuration[m].bus_pnc_gateway_type != no_pnc_gateway)
            test_state[2] = 1;


        u8 i = 0;
        u8 test_flag = 0;
        for (i = 0; i < 8; i++)
        {
            u8 cmp_res;
            cmp_res = memcmp(test_state, test_case[i], 6);//用的u16，所以是6个byte
            if (cmp_res == 0)
            {
                test_flag = 1;
                double diff = (T2 - T1) / 1000.0;
                if (diff < test_case[i][3])
                    TestInfoPrintPass("测试情况(是否带电，第一帧是否为NM报文，是否为PNC gateway；是为1，否则为0)：%d，%d，%d，T2-T1值为%.2fms，小于期望%dms，T1：%fs，T2：%fs", test_state[0], test_state[1], test_state[2], diff, test_case[i][3], T1 / 1000000.0, T2 / 1000000.0);
                else
                    TestInfoPrintFail("测试情况(是否带电，第一帧是否为NM报文，是否为PNC gateway；是为1，否则为0)：%d，%d，%d，T2-T1值为%.2fms，大于等于期望%dms，T1：%fs，T2：%fs", test_state[0], test_state[1], test_state[2], diff, test_case[i][3], T1 / 1000000.0, T2 / 1000000.0);
                return;
            }
        }
        if (test_flag == 0)
            TestInfoPrintFail("测试情况(是否带电，第一帧是否为NM报文，是否为PNC gateway；是为1，否则为0)：%d，%d，%d，没有匹配的情况，无法测试T2-T1的值", test_state[0], test_state[1], test_state[2]);
    }
}

//diff单位为ms,期望小于expect_diff
bool check_diff(s64 Tbig, s64 Tsmall, s64 expect_diff)
{
    bool res_right = 0;
    if (Tbig == -1 || Tsmall == -1)
        TestInfoPrintFail("参数不全，无法计算");
    else
    {
        double diff = (Tbig - Tsmall) / 1000.0;
        if (diff < expect_diff && diff > 0)
        {
            res_right = 1;
            //log("%fs", (double)Tsmall / 1000000.0);
            //log("%fs", (double)Tbig / 1000000.0);
            TestInfoPrintPass("Tsmall：%fs，Tbig：%fs，Tbig-Tsmall值为%.2fms，期望小于%dms", Tsmall / 1000000.0, Tbig / 1000000.0, diff, expect_diff);
            //TestInfoPrintPass("Tbig-Tsmall值为%.2fms，期望小于%dms，Tsmall：%fs，Tbig：%fs", diff, expect_diff, Tsmall / 1000000.0, Tbig / 1000000.0);
        }
        else
            TestInfoPrintFail("Tsmall：%fs，Tbig：%fs，Tbig-Tsmall值为%.2fms，期望小于%dms", Tsmall / 1000000.0, Tbig / 1000000.0, diff, expect_diff);
    }
    return res_right;
}

//diff单位为ms,期望（expect_diff_min，expect_diff_max）ms内
bool check_diff(s64 Tbig, s64 Tsmall, double expect_diff_min, double expect_diff_max)
{
    bool res_right = 0;
    if (Tbig == -1 || Tsmall == -1)
        TestInfoPrintFail("参数不全，无法计算");
    else
    {
        double diff = (Tbig - Tsmall) / 1000.0;
        (double)Tbig;
        (double)Tsmall;
        if (diff < expect_diff_max && diff > expect_diff_min)
        {
            TestInfoPrintPass("Tsmall：%fs，Tbig：%fs，Tbig-Tsmall值为%.2fms，期望(%.2f，%.2f)ms", Tsmall / 1000000.0, Tbig / 1000000.0, diff, expect_diff_min, expect_diff_max);
            res_right = 1;
        }
        else
            TestInfoPrintFail("Tsmall：%fs，Tbig：%fs，Tbig-Tsmall值为%.2fms，期望(%.2f，%.2f)ms", Tsmall / 1000000.0, Tbig / 1000000.0, diff, expect_diff_min, expect_diff_max);
    }
    return res_right;
}

//diff单位为ms
bool check_diff_no_info(s64 Tbig, s64 Tsmall, double expect_diff_min, double expect_diff_max)
{
    bool res_right = 0;
    if (Tbig == -1 || Tsmall == -1)
        return res_right;
    else
    {
        double diff = (Tbig - Tsmall) / 1000.0;
        if (diff < expect_diff_max && diff > expect_diff_min)
            res_right = 1;
    }
    return res_right;
}


//diff单位为ms
void check_diff_case_CAN_Timing_at_hardware_wake_up(u8 m, s64 T2, s64 T1)
{
    u16 expect_diff = 0;
    double current;
    double with_power_current = 1;//A
    current = main_power::instance()->Read_Cur();

    if (current >= with_power_current)
        expect_diff = TCANEventWakeUpToApp;
    else
        expect_diff = TCANPowerWakeUpToApp;

    if (T2 == -1)
    {
        TestInfoPrintFail("DUT没有发送任何报文，无法计算T2-T1");
        return;
    }
    else
    {
        double diff = (T2 - T1) / 1000.0;
        if (diff < expect_diff)
            TestInfoPrintPass("T2-T1值为%.2fms，小于期望%dms，T1：%fs，T2：%fs", diff, expect_diff, T1 / 1000000.0, T2 / 1000000.0);
        else
            TestInfoPrintFail("T2-T1值为%.2fms，大于等于期望%dms，T1：%fs，T2：%fs", diff, expect_diff, T1 / 1000000.0, T2 / 1000000.0);
        return;
    }
}

//diff单位为ms
void check_diff_case_CAN_Timing(u8 m, s64 T2, s64 T1)
{
    u16 expect_diff = 0;
    double current;
    double with_power_current = 1;//A
    current = main_power::instance()->Read_Cur();

    expect_diff = 250;

    if (T2 == -1)
    {
        TestInfoPrintFail("DUT没有发送任何报文，无法计算T2-T1");
        return;
    }
    else
    {
        double diff = (T2 - T1) / 1000.0;
        if (diff < expect_diff)
            TestInfoPrintPass("T2-T1值为%.2fms，小于期望%dms，T1：%fs，T2：%fs", diff, expect_diff, T1 / 1000000.0, T2 / 1000000.0);
        else
            TestInfoPrintFail("T2-T1值为%.2fms，大于等于期望%dms，T1：%fs，T2：%fs", diff, expect_diff, T1 / 1000000.0, T2 / 1000000.0);
        return;
    }
}



void check_current(double min, double max)
{
    double current;
    current = main_power::instance()->Read_Cur();
    if (current >= min && current <= max)
        TestInfoPrintPass("电流在范围[%.3f，%.3f]A内，测试值：%.3fA", min, max, current);
    else
        TestInfoPrintFail("电流不在范围[%.3f，%.3f]A内，测试值：%.3fA", min, max, current);
}

double return_current()
{
    double current;
    current = main_power::instance()->Read_Cur();
    return current;

}

//接收报文
void __stdcall On_RX_CAN_Msg(const pnative_int pobj, const PCANFD ACANFD) {

    TCANFD frame = *ACANFD;
    if (frame.get_is_tx() == false) {
        u8 m = 1;
        for (m; m < bus_count; m++)
        {
            if (frame.FIdxChn == m)
            {
                ALLRxMsg[m].push_back(frame);//包含错误帧
                if ((frame.FProperties >> 7) & 1)
                    ErrorRxMsg[m].push_back(frame);//error frame 
                else
                {
                    if (frame.FIdentifier == rsp_id) {
                        //printf("01_frame.FIdentifier == rsp_id 0x%X,%lld", frame.FIdentifier, frame.FTimeUs);
                        DiagRxMsg[m].push_back(frame);
                    }
                    else
                    {
                        //printf("02_frame.FIdentifier == rsp_id 0x%X, %lld", frame.FIdentifier, frame.FTimeUs);
                        OutErrorAndDiagRxMsg[m].push_back(frame);//除错误帧和诊断的其他报文
                        //if ((frame.FIdentifier >= nm_dut_send_id_min) && (frame.FIdentifier <= nm_dut_send_id_max))
                        if (frame.FIdentifier == bus_configuration[m].dut_send_nm_msg_id)
                        {
                            //printf("03_frame.FIdentifier == rsp_id 0x%X, %lld", frame.FIdentifier, frame.FTimeUs);
                            DUT_NMRxMsg[m].push_back(frame);
                        }
                        else
                        {
                            //此处必须<0x500或>0x580
                            //if (frame.FIdentifier < nm_dut_send_id_min || frame.FIdentifier >0x580)
                            //printf("01_frame.FIdentifier == rsp_id 0x%X, %lld", frame.FIdentifier, frame.FTimeUs);
                            DUT_APPRxMsg[m].push_back(frame);
                        }
                    }
                }
            }
        }

    }

}

void __stdcall On_TX_CAN_Msg(const pnative_int pobj, const PCANFD ACANFD) {
    TCANFD frame = *ACANFD;
    if (frame.get_is_tx() == true) {
        u8 m = 0;
        for (m; m < bus_count; m++)
        {
            if (frame.FIdxChn == m)
            {
                ALLTxMsg[m].push_back(frame);//包含错误帧
                if ((frame.FProperties >> 7) & 1)
                    ErrorTxMsg[m].push_back(frame);//error frame 
                else
                {
                    OutErrorTxMsg[m].push_back(frame);//除错误帧的其他报文
                    if ((frame.FIdentifier >= nm_dut_send_id_min) && (frame.FIdentifier <= nm_dut_send_id_max))
                    {
                        DUT_NMTxMsg[m].push_back(frame);
                        if (frame.FIdentifier == nm_dut_send_id_max)
                            DUT_NMTxMsg_nm_msg1[m].push_back(frame);
                        //log("0x%X,time:%fs",frame.FIdentifier,frame.FTimeUs/1000000.0);
                    }
                    else if (frame.FIdentifier == phy_id || frame.FIdentifier == fun_id)
                    {
                        //收到诊断响应
                        DiagTxMsg[m].push_back(frame);

                    }
                    else
                    {
                        DUT_APPTxMsg[m].push_back(frame);
                        //if (frame.FIdentifier == 0x121)
                        //log("%fs", frame.FTimeUs / 1000000.0);

                    }
                }
            }
        }

    }

}

void TestPrintNMMappingInfo() {
    for (int i = 0; i < bus_count; i++) {
        bus_configuration[i].bus_pnc_gateway_type;
    }
}
/*
void print_all_bus_configurations()
{
    log("===== START PRINTING ALL BUS CONFIGURATIONS =====");

    for (u8 i = 1; i < bus_count; i++)
    {
        printf("Set PNC Type is %d,Read PNC Type is %d - %s",bus_configuration[i].bus_pnc_gateway_type,NM_Config[i].bus_pnc_gateway_type, NM_Config[i].bus_pnc_gateway_type_name.c_str());
        if(bus_configuration[i].bus_pnc_gateway_type == NM_Config[i].bus_pnc_gateway_type)
            TestInfoPrintPass("[%s] Gateway Type: %s,NM Wakeup: %d,HW Wakeup: %d", bus_configuration[i].node_name, NM_Config[i].bus_pnc_gateway_type_name.c_str(), bus_configuration[i].if_support_network_wakeup ? 1 : 0, bus_configuration[i].if_support_hardware_wakeup ? 1 : 0);
        else
            TestInfoPrintFail("[%s] Gateway Type: %s,NM Wakeup: %d,HW Wakeup: %d", bus_configuration[i].node_name, NM_Config[i].bus_pnc_gateway_type_name.c_str(), bus_configuration[i].if_support_network_wakeup ? 1 : 0, bus_configuration[i].if_support_hardware_wakeup ? 1 : 0);
    }
    
    for (u8 i = 1; i < bus_count; i++)
    {
        TestInfoPrint("[%s] NM PDU ID: 0x%X", bus_configuration[i].node_name, bus_configuration[i].dut_send_nm_msg_id);
    }
        // PNC相关配置
    for (u8 i = 1; i < bus_count; i++)
    {
        snprintf(StepDespriction, sizeof(StepDespriction), "----------%s:", bus_configuration[i].node_name);
        char PNCArray[20];
        for (u8 j = 0; j < pnc_max_count_in_one_bus; j++)
        {
            if (bus_configuration[i].relevant_pnc[j] != 0)
            {
                snprintf(PNCArray,sizeof(PNCArray),"PNC%d ", bus_configuration[i].relevant_pnc[j]);
                strcat(StepDespriction, PNCArray);
            }
        }
        TestInfoPrint(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "No Mapping:");
        for (int k = 0; k < bus_configuration[i].NoMapFrames.size(); k++) {
            snprintf(PNCArray, sizeof(PNCArray), "0x%03X ", bus_configuration[i].NoMapFrames[k]);
            strcat(StepDespriction, PNCArray);
        }
        TestInfoPrint(StepDespriction);
       
        
        for (int PNCSize = 0; PNCSize < 64; PNCSize++) {
            if (bus_configuration[i].pnc_MapFramesTx[PNCSize].size() > 0) {
                snprintf(StepDespriction, sizeof(StepDespriction), "PNC%d:", PNCSize+1);
                for (int k = 0; k < bus_configuration[i].pnc_MapFramesTx[PNCSize].size(); k++) {
                    snprintf(PNCArray, sizeof(PNCArray), "0x%X ", bus_configuration[i].pnc_MapFramesTx[PNCSize][k]);
                    strcat(StepDespriction, PNCArray);
                }
                TestInfoPrint(StepDespriction);
            }
        }
    }

    log("===== END PRINTING ALL BUS CONFIGURATIONS =====");
}
*/
void print_all_bus_configurations()
{
    log("===== START PRINTING ALL BUS CONFIGURATIONS =====");

    // 1. 打印基础信息（网段类型、唤醒支持情况等）
    for (u8 i = 1; i < bus_count; i++)
    {
        printf("Set PNC Type is %d, Read PNC Type is %d - %s",
            bus_configuration[i].bus_pnc_gateway_type,
            NM_Config[i].bus_pnc_gateway_type,
            NM_Config[i].bus_pnc_gateway_type_name.c_str());

        if (bus_configuration[i].bus_pnc_gateway_type == NM_Config[i].bus_pnc_gateway_type)
            TestInfoPrintPass("[%s] Gateway Type: %s, NM Wakeup: %d, HW Wakeup: %d",
                bus_configuration[i].node_name,
                NM_Config[i].bus_pnc_gateway_type_name.c_str(),
                bus_configuration[i].if_support_network_wakeup ? 1 : 0,
                bus_configuration[i].if_support_hardware_wakeup ? 1 : 0);
        else
            TestInfoPrintFail("[%s] Gateway Type: %s, NM Wakeup: %d, HW Wakeup: %d",
                bus_configuration[i].node_name,
                NM_Config[i].bus_pnc_gateway_type_name.c_str(),
                bus_configuration[i].if_support_network_wakeup ? 1 : 0,
                bus_configuration[i].if_support_hardware_wakeup ? 1 : 0);
    }

    // 2. 打印网段报文映射细节
    for (u8 i = 1; i < bus_count; i++)
    {
        snprintf(StepDespriction, sizeof(StepDespriction), "--- 网段: [%s] NM PDU ID: 0x%X ---",
            bus_configuration[i].node_name, bus_configuration[i].dut_send_nm_msg_id);
        TestInfoPrint(StepDespriction);

        // A. 打印 NoMapFrames (周期性 + 事件型)
        if (!bus_configuration[i].NoMapFrames.empty() || !bus_configuration[i].NoMapFrames_Event.empty())
        {
            // 周期性 NoMap
            snprintf(StepDespriction, sizeof(StepDespriction), "  [No Mapping] Cyclic: ");
            for (size_t k = 0; k < bus_configuration[i].NoMapFrames.size(); k++) {
                char tmp[16];
                snprintf(tmp, sizeof(tmp), "0x%03X ", bus_configuration[i].NoMapFrames[k]);
                strcat(StepDespriction, tmp);
            }
            TestInfoPrint(StepDespriction);

            // 事件型 NoMap (非空打印)
            if (!bus_configuration[i].NoMapFrames_Event.empty()) {
                snprintf(StepDespriction, sizeof(StepDespriction), "  [No Mapping] Event : ");
                for (size_t k = 0; k < bus_configuration[i].NoMapFrames_Event.size(); k++) {
                    char tmp[16];
                    snprintf(tmp, sizeof(tmp), "0x%03X ", bus_configuration[i].NoMapFrames_Event[k]);
                    strcat(StepDespriction, tmp);
                }
                TestInfoPrint(StepDespriction);
            }
        }

        // B. 遍历 64 个 PNC 槽位
        for (int pnc_idx = 0; pnc_idx < 64; pnc_idx++)
        {
            bool has_cyclic = !bus_configuration[i].pnc_MapFramesTx[pnc_idx].empty();
            bool has_event = !bus_configuration[i].pnc_MapFramesTx_Event[pnc_idx].empty();

            if (has_cyclic || has_event)
            {
                // 打印 PNC 编号头
                snprintf(StepDespriction, sizeof(StepDespriction), "  [PNC%d Mapping]", pnc_idx + 1);
                TestInfoPrint(StepDespriction);

                // 打印该 PNC 下的周期报文
                if (has_cyclic) {
                    snprintf(StepDespriction, sizeof(StepDespriction), "---- Cyclic: ");
                    for (size_t k = 0; k < bus_configuration[i].pnc_MapFramesTx[pnc_idx].size(); k++) {
                        char tmp[16];
                        snprintf(tmp, sizeof(tmp), "0x%X ", bus_configuration[i].pnc_MapFramesTx[pnc_idx][k]);
                        strcat(StepDespriction, tmp);
                    }
                    TestInfoPrint(StepDespriction);
                }

                // 打印该 PNC 下的事件报文
                if (has_event) {
                    snprintf(StepDespriction, sizeof(StepDespriction), "---- Event : ");
                    for (size_t k = 0; k < bus_configuration[i].pnc_MapFramesTx_Event[pnc_idx].size(); k++) {
                        char tmp[16];
                        snprintf(tmp, sizeof(tmp), "0x%X ", bus_configuration[i].pnc_MapFramesTx_Event[pnc_idx][k]);
                        strcat(StepDespriction, tmp);
                    }
                    TestInfoPrint(StepDespriction);
                }
            }
        }
    }

    log("===== END PRINTING ALL BUS CONFIGURATIONS =====");
}


#endif