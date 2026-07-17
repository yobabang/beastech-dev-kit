#ifndef TEST_CASE_DIAG_API
#define TEST_CASE_DIAG_API

#include <windows.h>
#include "test_common.h"
#include "TSMaster.h"
#include "test_tp.h"
#include "TestReport.h"
#include <sstream>
#include "SBLParser.h"
#include "PDXParser.h"
#include "MPLibrary.h"
#include "test_devices.h"
#include "tsdisturb.h"
#include "GenerateKeyExOpt.h"


s32 udsHandle = 0;
enum AddressMode { Physical = 0, Functional = 1 };//寻址模式
enum DiagSession { DefaultSession = 0x01, ExtendedSession = 0x03, ProgrammingSession = 0x02 };//诊断会话类型
enum SPRMIB {SPRMIB_FALSE, SPRMIB_TRUE};//肯定响应消息抑制位
enum ECUReset { HardReset = 0x01, SoftReset = 0x03 };//复位方式
//enum PowerControl {on,off};

//多帧数据，不行
//u8 reqMultiFrameDataArray2[5] = { 0x22,0xD0,0XAC,0xed,0xA0 };



//SWDL
u8 reqSWDL_ID = 0x20;
u32 MaxLength = 0x2000;

//数据接收缓存
u8 TpRxData[65535];
s64 TpRxLength;
//

//27解锁相关
u8 SeedArray[16];
u8 KeyArray[16];
char* DLLName;
u8 pincode0x01[16] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
u8 pincode0x03[16] = { 0x5E, 0x40, 0xDC, 0x91, 0xEC, 0xA1, 0x44, 0xEC, 0x66, 0x3F, 0xCC, 0x2E, 0x6E, 0xD3, 0x46, 0xD8 };
u8 pincode0x05[16] = { 0x10, 0x54, 0xA8, 0x09, 0x8E, 0xA0, 0x8C, 0x6F, 0x49, 0x25, 0x78, 0x38, 0x2A, 0x11, 0x84, 0x9B };
u8 pincode0x07[16] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
u8 pincode0x09[16] = { 0x5E, 0x40, 0xDC, 0x91, 0xEC, 0xA1, 0x44, 0xEC, 0x66, 0x3F, 0xCC, 0x2E, 0x6E, 0xD3, 0x46, 0xD8 };
u8 pincode0x0B[16] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x0D[16] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x0F[16] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x11[16] = { 0x5E,0x9A,0xE3,0xD5,0x3A,0xA0,0xF6,0x9F,0x72,0x0C,0x37,0x7E,0x68,0x37,0xC2,0xA0 };
u8 pincode0x13[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x15[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x17[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x19[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x1B[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x1D[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x1F[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
//

//DTC相关
int Ndtc = 0;

struct DTC
{
    u8 DTCNumber[3];
    u8 DTCStatus;
};

u32 testdtc[2];
struct DTC TestDtc[2];
struct DTC recdtc[1000];

//时间参数
u8  SessionFlag = 0;


//函数声明
void TestPostCondition(void);
s32 excelParser(void);
s32 DTCExcelParser(void);
s32 readDB(s32);
s32 FindTRCPath();
s32 SimulateTRC(bool);
s32 StartCANRBS(TSignalPath* path, bool state = true);
s32 FindMissFramePath(void);
s32 getLINNetwork(void);
s32 getCarConfigInfo(void);

//初始化测试用例内需要根据版本生成的读取数据
void TestParamentInit() {
    if (gAsrVersion >= 4.1) {
        gSnapshotRecordNo = 0x21;
    }
    else {
        gSnapshotRecordNo = 0x20;
    }
}

void StepUpTest(int cycle) {
    char despriction[256];
    try {
        while (isRunning) {
            // 这里放置你需要周期性执行的代码
            char Stepdespriction[256];
            gtestIndex++;
            //TestReport_HTML_StepDescription("进入线程！");
            if (gReadEndFlag)
            {
                snprintf(Stepdespriction, sizeof(Stepdespriction), "--------------%dth cycle test of FDC10 Step Up------------", gtestIndex);
                TestReport_HTML_StepDescription(Stepdespriction, Info);
                app.set_system_var_int32("Value_CheckFDC10", 1);
            }
            else
            {
                snprintf(Stepdespriction, sizeof(Stepdespriction), "Present test hasn't been finished, skip %dth cycle test", gtestIndex);
                TestReport_HTML_StepDescription(Stepdespriction, Info);
            }

            // 线程休眠 100 毫秒
            std::this_thread::sleep_for(std::chrono::milliseconds(cycle));
        }
    }
    catch (const std::exception& e) {
        snprintf(despriction, sizeof(despriction), "子线程异常: %s", e.what());
        TestReport_HTML_StepDescription(despriction, Fail);
    }
    catch (...) {
        TestReport_HTML_StepDescription("子线程未知异常", Fail);
    }
    isRunning = false; // 确保线程退出
}

void StepDownTest(int cycle) {
    char despriction[256];
    try {
        while (isRunning) {
            // 这里放置你需要周期性执行的代码
            char Stepdespriction[256];
            gtestIndex++;
            if (gReadEndFlag)
            {
                snprintf(Stepdespriction, sizeof(Stepdespriction), "------------------%dth cycle test of FDC10 Step Down---------------------", gtestIndex);
                TestReport_HTML_StepDescription(Stepdespriction, Info);
                app.set_system_var_int32("Value_CheckFDC10", 1);
            }
            else
            {
                snprintf(Stepdespriction, sizeof(Stepdespriction), "Present test hasn't been finished, skip %dth cycle test", gtestIndex);
                TestReport_HTML_StepDescription(Stepdespriction, Info);
            }
            // 线程休眠 100 毫秒
            std::this_thread::sleep_for(std::chrono::milliseconds(cycle));
        }
    }
    catch (const std::exception& e) {
        snprintf(despriction, sizeof(despriction), "子线程异常: %s", e.what());
        TestReport_HTML_StepDescription(despriction, Fail);
    }
    catch (...) {
        TestReport_HTML_StepDescription("子线程未知异常", Fail);
    }
    isRunning = false; // 确保线程退出
}

/*
s32 GetParameterByExcel(char* cfgpath)
{
    void* excelhandle;
    char bufferlog[1024];
    s32 n;
    s32 rc, cc;
    u16 sheetRowCol[64][2] = { {0,0} };
    char* p;
    std::string Key;
    std::string value;

    if (app.excel_load(cfgpath, &excelhandle))
    {
        sprintf(bufferlog, "打开excle表格 %s 失败", cfgpath);
        app.log_text(bufferlog, lvlError);
        return -1;
    }
    if (app.excel_get_sheet_count(excelhandle, &n))
    {
        app.log_text("获取表格sheet页数失败", lvlError);
        return -1;
    }
    for (u8 i = 0; i < n; i++)
    {
        if (app.excel_get_cell_count(excelhandle, i, &rc, &cc))
        {
            app.log_text("获取表格RowCol行列数失败", lvlError);
            return -1;
        }
        sheetRowCol[i][0] = rc;
        sheetRowCol[i][1] = cc;
        //if (cc < 2)
        //{
        //	app.log_text("获取到的表格Col列数太少", lvlError);
        //	log("row count is %d, col count is %d", rc, cc);
        //	return -1;
        //}
        //log("row count is %d, col count is %d", sheetRowCol[i][0], sheetRowCol[i][1]);
    }
    //处理第一个sheet：GENERAL
    for (u16 row = 1; row < sheetRowCol[0][0]; row++)
    {
        if (0 == app.excel_get_cell_value(excelhandle, 0, row, 0, &p))
        {
            if (p != NULL && strcmp(p, " ") != 0)
            {  //如果NULL情况下使用将会破坏系统内存
                Key = p;
                if (0 == app.excel_get_cell_value(excelhandle, 0, row, 1, &p))
                {
                    if (p != NULL && strcmp(p, " ") != 0)
                    {
                        value = p;
                        //INI赋值给测试变量
                        GetINIValue(Key, value);
                    }
                }
            }
        }
    }
    //处理第二个sheet：SYSTEM
    for (u16 row = 1; row < sheetRowCol[1][0]; row++)
    {
        if (0 == app.excel_get_cell_value(excelhandle, 1, row, 0, &p))
        {
            if (p != NULL && strcmp(p, " ") != 0)
            {  //如果NULL情况下使用将会破坏系统内存
                Key = p;
                if (0 == app.excel_get_cell_value(excelhandle, 1, row, 1, &p))
                {
                    if (p != NULL && strcmp(p, " ") != 0)
                    {
                        value = p;
                        //INI赋值给测试变量
                        GetINIValue(Key, value);
                    }
                }
            }
        }
    }
}
*/

void CarConfigSignalMapping() {
    int dateBaseCount = 0;
    if (0 == app.db_get_can_database_count(&dateBaseCount)) {
        log_ok("CAN database count = %d", dateBaseCount);

        for (int dbIndex = 0; dbIndex < dateBaseCount && dbIndex < 7; dbIndex++) {
            TDBProperties db;
            db.FDBIndex = dbIndex;
            if (0 != app.db_get_can_database_properties_by_index(&db)) continue;
            if (0 != strcmp(db.FName, "ZCU_CANFD1")) continue;
            log_ok("[%d] Database: %s", dbIndex + 1, db.FName);

            for (int ecuIndex = 0; ecuIndex < db.FECUCount; ecuIndex++) {
                TDBECUProperties ecu;
                ecu.FDBIndex = dbIndex;
                ecu.FECUIndex = ecuIndex;
                if (0 != app.db_get_can_ecu_properties_by_index(&ecu)) continue;

                //if (strstr(ecu.FName, "ZCUD") == nullptr) continue;

                for (int msgIndex = 0; msgIndex < ecu.FTxFrameCount; msgIndex++) {
                    TDBFrameProperties msg;
                    msg.FDBIndex = dbIndex;
                    msg.FECUIndex = ecuIndex;
                    msg.FFrameIndex = msgIndex;
                    if (0 != app.db_get_can_frame_properties_by_index(&msg)) continue;
                    if (0x179 == msg.FCANIdentifier) {

                    }

                    if (msg.FCANIdentifier < 0x500 && msg.FCycleTimeMs > 0 && msg.FCycleTimeMs < 10000) {
                        if ((NetworkCluster[db.FName] == CH2) && (msg.FCANIdentifier != 0x132 && msg.FCANIdentifier != 0x34)) {

                        }
                    }
                    else {
                        printf("    Skip Msg: ID=0x%03X, Cycle=%u (invalid)\n",
                            msg.FCANIdentifier, msg.FCycleTimeMs);
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////

//////////////类型转换/////////////////////
//std::string dec2hex(int i, int width = 2)
//{
//    std::stringstream ioss; // 定义字符串流
//    std::string s_temp;     // 存放转化后字符
//    ioss << std::hex << i;  // 以十六制形式输出
//    ioss >> s_temp;
//
//    if (width > s_temp.size())
//    {
//        std::string s_0(width - s_temp.size(), '0'); // 位数不够则补0
//        s_temp = s_0 + s_temp;                       // 合并
//    }
//
//    std::string s = s_temp.substr(s_temp.length() - width, s_temp.length()); // 取右width位
//    return s;
//}

std::string dec2hex(int i, int width = 2)
{
    std::stringstream ioss; // 定义字符串流
    std::string s_temp;     // 存放转化后字符
    ioss << std::hex << std::uppercase << i; // 以十六进制形式输出，并使用大写
    ioss >> s_temp;

    if (width > s_temp.size())
    {
        std::string s_0(width - s_temp.size(), '0'); // 位数不够则补0
        s_temp = s_0 + s_temp;                       // 合并
    }

    std::string s = s_temp.substr(s_temp.length() - width, s_temp.length()); // 取右width位
    return s;
}

char array2Bchange[100] = "\0";

void array2string(u8* sourceArray,char* desString = array2Bchange)
{
    strcpy(desString, "\0");
    int i;
    char _str[100] = "\0";
    char _str1[100] = "\0";

    for (i = 0; i <4 ; i++)
    {
        sprintf(_str, "0x%x", sourceArray[i]);
        strcat(_str1, _str);
    }
    strcpy(desString, _str1);
}

inline void TestWait(const s32 ATimeMs, char* AMsg = nullptr) {
    AppWait(ATimeMs, AMsg);
    //检查是否有手动停止测试
    if (0 != app.is_connected()) {
        throw test_terminate();
    }
}
//////////////////////////////////////////



/// <summary>
/// 初始化诊断模块
/// </summary>
/// <param name="udsHandle"></param> udsHandle 诊断模块的句柄
void uds_create()
{
    if(BusTypeNow==CAN||BusTypeNow==CANFD)
    {
        if (rtlUIDiagnostics.tsdiag_can_create(&udsHandle, Diag_ch, Diag_IFFD, Diag_FD_MAX_LEN, Diag_Request_ID, Diag_Req_IF_STR, Diag_Response_ID, Diag_Res_IF_STR, Diag_Function_ID, Diag_Fun_IF_STR) == 0x00)
        {
            char tempstr[100] = {};
            if (language_id == 2052)
            {
                strcpy(tempstr, "");
                sprintf(tempstr, "%s%d%x", "诊断会话%d创建成功,通道号为", udsHandle,Diag_ch);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%x", "ECU 物理寻址", Diag_Request_ID);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%x", "ECU 功能寻址", Diag_Function_ID);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%x", "ECU 响应ID", Diag_Response_ID);
                app.log_text(tempstr, lvlOK);
            }
            else
            {
                strcpy(tempstr, "");
                sprintf(tempstr, "%s%x", "create diagnostic communication successfully, channel number is ", Diag_ch);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%x", "ECU physical request ID:", Diag_Request_ID);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%x", "ECU functional request ID:", Diag_Function_ID);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%x", "ECU response ID:", Diag_Response_ID);
                app.log_text(tempstr, lvlOK);
            }
        }
    }
    else if (BusTypeNow == LIN)
    {
        if (rtlUIDiagnostics.tsdiag_lin_create(&udsHandle, Diag_ch, NAD) == 0x00)
        {
            char tempstr[100] = {};
            if (language_id == 2052)
            {
                strcpy(tempstr, "");
                sprintf(tempstr, "%s%x", "诊断会话创建成功,通道号为", Diag_ch);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%x", "ECU NAD = ", NAD);
                app.log_text(tempstr, lvlOK);
            }

            rtlUIDiagnostics.tstp_lin_set_run_with_normal_schedule_table(udsHandle, true);
            rtlUIDiagnostics.tstp_lin_tp_para_default(udsHandle, 15, 15, 2);
        }
    }

    //rtlUIDiagnostics.tsdiag_set_is_valid(udsHandle, true);
    //rtlUIDiagnostics.tsdiag_set_p2_timeout(udsHandle, Diag_UDS_P2Server);
    //rtlUIDiagnostics.tsdiag_set_p2_extended(udsHandle, Diag_UDS_P2Server_Delay);
    //rtlUIDiagnostics.tsdiag_set_s3_servertime(udsHandle, Diag_UDS_S3Server);
    //rtlUIDiagnostics.tsdiag_set_s3_clienttime(udsHandle, Diag_UDS_S3Client);
}
void uds_delete() {
    if(BusTypeNow==CAN||BusTypeNow==CANFD||BusTypeNow==LIN)
    {
        if (rtlUIDiagnostics.tsdiag_can_delete_all() == 0x00)
        //if (rtlUIDiagnostics.tsdiag_can_delete(udsHandle) == 0x00)
        {
            app.log_text("Delete Diagnostic Success",lvlOK);
        }
    }
}


/// <summary>
/// 发送任意的诊断报文
/// </summary>
/// <returns></returns>
s32 Raw_diag_serve_sent(pu8 reqDataArray, s32 reqsize, u8 AddressMode = 0, pu8 responseArray = TpRxData) {

    s32 responseArraySize = 65536;
    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, reqDataArray, reqsize, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlOK);
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, reqDataArray, reqsize, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlOK);
        }
    }
    return -1;
}

s32 diag_serve_sent(pu8 reqDataArray, s32 reqsize, u8 AddressMode = 0, pu8 responseArray = TpRxData)
{
    TpRxLength = Raw_diag_serve_sent(reqDataArray, reqsize,AddressMode,responseArray);

    strcpy(requestInfo, "");

    if (reqsize > 5)
        reqsize = 5;

    for (int j = 0; j < reqsize; j++)
    {
        strcat(requestInfo, (" " + dec2hex(reqDataArray[j])).c_str());
    }

    return 0;
}




s32 SendTesterPresent(enum TP TP_State, u64 PeriodTime = Diag_UDS_S3Client)
{
    if (TP_State == TP_ON)
    {
        app.log_text("start TesterPresent",lvlInfo);
        if (BusTypeNow == CAN || BusTypeNow == CANFD)
        {
            return CAN_TP_TesterPresent(1, PeriodTime);
        }
    }
    else
    {
        app.log_text("stop TesterPresent", lvlInfo);
        if (BusTypeNow == CAN || BusTypeNow == CANFD)
        {
            return CAN_TP_TesterPresent(0);
        }
    }
    return -1;
}



/// <summary>
/// 
/// </summary>
/// <param name="Info"></param>
/// <param name="Data"></param>
/// <param name="len"></param>
void PrintInfo(char Info[], u8 Data[] = TpRxData, u32 len = TpRxLength)
{//示例 PrintPID("Pos.Response",responseArray,responseArraySize);
    u32 i;
    char* PrintData = (char*)malloc(TpRxLength+1);
    for (i = 0; i < len; i++)
    {
        if ((Data[i] >> 4) < 10)
            PrintData[i * 3] = ((Data[i] >> 4) + 0x30);
        else
            PrintData[i * 3] = ((Data[i] >> 4) + 0x37);
        if ((Data[i] & 0x0F) < 10)
            PrintData[i * 3 + 1] = ((Data[i] & 0x0F) + 0x30);
        else
            PrintData[i * 3 + 1] = ((Data[i] & 0x0F) + 0x37);
        PrintData[i * 3 + 2] = 0x20;
    }
    PrintData[len * 3] = '\0';

    char* PrintS = (char*)malloc(TpRxLength);
    
    strcpy(PrintS, Info);

    strcat(PrintS, PrintData);

    TestReport_HTML_StepDescription(PrintS);
}

/// <summary>
/// 通用 检查否定响应
/// </summary>
/// <param name="NRC"></param> NRC 期望的否定响应码
/// <param name="responseArraySize"></param> responseArraySize 诊断响应数据长度,由请求函数返回
/// <param name="responseArray"></param> responseArray 诊断响应内容
s32 Raw_CheckNegResponse(u8 NRC, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("NRC " + dec2hex(NRC)).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] != 0x7F)
    {
        strcpy(issuereason, "Unexpected Response Received!");
        return -1;
    }
    else if (responseArray[2] != NRC)
    {
        strcpy(issuereason, "Unexpected NRC Received!");
        return -1;
    }
    else
    {

    }

    if (3 != responseArraySize)
    {
        strcpy(issuereason, "Invalid Data Length!");
        return -1;
    }
    return 0;
}
s32 CheckNegResponse(u8 NRC)
{
    if (0 == Raw_CheckNegResponse(NRC))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return-1;
    }
}

/// <summary>
/// 通用 检查否定响应
/// </summary>
/// <param name="NRC"></param> NRC 期望的否定响应码
/// <param name="responseArraySize"></param> responseArraySize 诊断响应数据长度,由请求函数返回
/// <param name="responseArray"></param> responseArray 诊断响应内容
s32 Raw_CheckNegResponse(u8* NRC, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("NRC " + dec2hex(NRC[0]) +"or NRC "+dec2hex(NRC[1])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        return -1;
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] != 0x7F)
    {
        strcpy(issuereason, "Unexpected Response Received!");
        return -1;
    }
    
    for (int i = 0; i < 5; i++)
    {
        if (responseArray[2] == NRC[i])
        {
            return 0;
        }
    }
    
    strcpy(issuereason, "Unexpected NRC Received!");

    if (3 != responseArraySize)
    {
        strcpy(issuereason, "Invalid Data Length!");
        return -1;
    } 

    return -1;
}

s32 CheckNegResponse(u8* NRC)
{
    if (0 == Raw_CheckNegResponse(NRC))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return-1;
    }
}



s32 CheckNonResponse()
{
    int retval;
    retval = 0;

    retval = WaitMessage(500);

    strcpy(expectedResponse, "ECU NO Response!");

    if (retval == 0)
    {
        TestReport_HTML_StepDescription("ECU Send Response!", Fail);
        strcpy(actualResponse, "");
        for (int i = 0; i < TpRxLength; i++)
        {
            sprintf(actualResponse, " 0x%x", dec2hex(TpRxData[i]));
        }
        TestReport_StepInfo(Fail);
        resultFlag++;
        return -1;
    }
    else
    {
        strcpy(actualResponse, "ECU NO Response!");
        TestReport_StepInfo(Pass);
    }
    return 0;
}

//qq
s32 CheckNonResponse(u16 WaitTime)
{
    int retval;
    retval = 0;

    retval = WaitMessage(WaitTime);

    if (retval == 0)
    {
        TestReport_HTML_StepDescription("ECU Send Response!", Fail);
        strcpy(actualResponse, "");
        for (int i = 0; i < TpRxLength; i++)
        {
            sprintf(actualResponse, " 0x%x", dec2hex(TpRxData[i]));
        }
        //TestReport_StepInfo(Fail);
        resultFlag++;
        return -1;
    }
    else
    {
        strcpy(actualResponse, "ECU NO Response!");
        TestReport_HTML_StepDescription(actualResponse, Pass);
       //TestReport_StepInfo(Pass);
    }
    return 0;
}



/// <summary>
/// Service0x10 诊断会话控制
/// </summary>
/// <param name="SubFunction"></param> SubFunction $10服务子功能
/// <param name="AddressMode"></param> AddressMode $10服务寻址方式（0 = Physical , 1 = Functional）
/// <param name="responseArray"></param> responseArray 获取$10服务的响应数据内容
s32 Raw_SID10_SessionControl(u8 SubFunction, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID10[2] = {};
    SID10[0] = 0x10;
    SID10[1] = SubFunction;
    s32 responseArraySize = 10;

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID10, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlOK);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID10, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlOK);
            return -1;
        }
    }
}
s32 SID10_SessionControl(u8 SubFunction, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID10_SessionControl(SubFunction,AddressMode);

    strcpy(requestInfo, "");
    sprintf(requestInfo, "%s%s", "10 ", dec2hex(SubFunction).c_str());
    return 0;
}

/// <summary>
/// Service0x10 $10服务检查肯定响应
/// </summary>
/// <param name="SubFunction"></param> SubFunction $10服务子功能
/// <param name="responseArraySize"></param> responseArraySize $10服务响应数据长度
/// <param name="responseArray"></param> responseArray $10服务响应数据内容
s32 Raw_CheckPosResponse_SID10(u8 SubFunction, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    u16 TempSessionTime;
    u16 TempP2Time, TempP2STime;

    if (SubFunction == 0x01)
    {
        TempP2Time = Diag_UDS_P2Server;
        TempP2STime = Diag_UDS_P2Server_Delay;
    }
    else if (SubFunction == 0x03)
    {
        TempP2Time = Diag_UDS_P2Server;
        TempP2STime = Diag_UDS_P2Server_Delay;
    }
    else
    {
        TempP2Time = 25;
        TempP2STime = 500;
    }
 
    strcpy(expectedResponse,"");
    for (int i = 0; i < responseArraySize; i++)
    {
        sprintf(expectedResponse, "%s 0%x 00 %x 0%x %x", "50", SubFunction, TempP2Time, TempP2STime >> 8, TempP2STime & 0xff);
    }
    

    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {

        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, dec2hex(responseArray[i]).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x7F && responseArray[2] == 0x78)
    {
        strcpy(issuereason, "The NRC78 is not allowed when the UDS service is 0x10!");
        return -1;
    }
    if (responseArray[0] == 0x50)
    {
        if (responseArray[1] == SubFunction)
        {

        }
        else
        {
            strcpy(issuereason, "Unexpected SubFunction!!");
            return -1;
        }
    }
    else
    {
        strcpy(issuereason, "Unexpected Response!");
        return -1;
    }

    

    TempSessionTime = (responseArray[2] << 8) + responseArray[3];

    

    if (TempSessionTime == TempP2Time)
    {

    }
    else
    {
        strcpy(issuereason, "P2 Server is not correct!");
        return -1;
    }

    TempSessionTime = (responseArray[4] << 8) + responseArray[5];
    if (TempSessionTime == TempP2STime)
    {

    }
    else
    {
        strcpy(issuereason, "P2* Server is not correct!");
        return -1;
    }

    if (6 != responseArraySize)
    {
        strcpy(issuereason, "Invalid Data Length!");
        return -1;
    }

    return 0;
}

s32 CheckPosResponse_SID10(u8 SubFunction)
{
    if (0 == Raw_CheckPosResponse_SID10(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// Service0x22 通过DID读取数据
/// </summary>
/// <param name="DID"></param> DID $22服务DID
/// <param name="responseArray"></param> responseArray $22服务获取响应内容
s32 Raw_SID22_ReadDataByIdentifier(u8 DID[2], u8 AddressMode = 0, u8* responseArray = TpRxData )
{
    u8 SID22[3] = {};
    SID22[0] = 0x22;
    SID22[1] = DID[0];
    SID22[2] = DID[1];
    s32 responseArraySize = 65535;
    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID22, 3, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID22, 3, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }

    }
}
s32 SID22_ReadDataByIdentifier(u8 DID[2], u8 AddressMode = 0)
{
    TpRxLength = Raw_SID22_ReadDataByIdentifier(DID,AddressMode);

    strcpy(requestInfo, ("22 " + dec2hex(DID[0]) + " " + dec2hex(DID[1])).c_str());
    return 0;
}

/// <summary>
/// Service0x22 $22服务检查肯定响应
/// </summary>
/// <param name="DID"></param> DID $22服务DID
/// <param name="Length"></param> Length $22服务DID的数据长度
/// <param name="responseArraySize"></param> responseArraySize $22服务响应数据长度
/// <param name="responseArray"></param> responseArray $22服务响应数据内容
s32 Raw_CheckPosResponse_SID22(u8 DID[2], s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("62 " + dec2hex(DID[0]) + " " + dec2hex(DID[1])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");

        return -1;
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x62)
    {

    }
    else
    {
        strcpy(issuereason, "Unexpected Response!");
        return -1;
    }

    if (responseArray[1] == DID[0] && responseArray[2] == DID[1])
    {

    }
    else
    {
        strcpy(issuereason, "Unexpected DID!");
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID22(u8 DID[2])
{
    if (0 == Raw_CheckPosResponse_SID22(DID))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// Service0x22 $22服务检查肯定响应
/// </summary>
/// <param name="DID"></param> DID $22服务DID
/// <param name="Length"></param> Length $22服务DID的数据长度
/// <param name="responseArraySize"></param> responseArraySize $22服务响应数据长度
/// <param name="responseArray"></param> responseArray $22服务响应数据内容
s32 Raw_CheckPosResponse_SID22(u8 DID[2], u32 Length, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("62 " + dec2hex(DID[0])+" " + dec2hex(DID[1])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x62)
    {

    }
    else
    {
        strcpy(issuereason, "Unexpected Response!");
        return -1;
    }

    if (responseArray[1] == DID[0] && responseArray[2] == DID[1])
    {

    }
    else
    {
        strcpy(issuereason, "Unexpected DID!");
        return -1;
    }

    if (Length != responseArraySize - 3)
    {
        strcpy(issuereason, "Unexpected DataLength!");
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID22(u8 DID[2], u32 Length)
{
    if (0 == Raw_CheckPosResponse_SID22(DID, Length))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// Service0x22 $22服务检查肯定响应,并检查DID数据内容与输入值是否一致
/// </summary>
/// <param name="DID"></param> DID $22服务DID
/// <param name="Length"></param> Length $22服务DID的期望数据内容长度
/// <param name="DIDdata"></param> DIDdata $22服务DID的期望/默认数据内容
/// <param name="responseArraySize"></param> responseArraySize $22服务响应数据长度
/// <param name="responseArray"></param> responseArray $22服务响应数据内容
s32 Raw_CheckPosResponse_SID22(u8 DID[2], u32 Length, u8* DIDdata, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    u32 i;
    u8 TempData[100] = {};

    strcpy(expectedResponse, ("62 " + dec2hex(DID[0]) + " " + dec2hex(DID[1]) + " " + dec2hex(DIDdata[0])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x62)
    {
        if (responseArray[1] == DID[0] && responseArray[2] == DID[1])
        {

        }
        else
        {
            strcpy(issuereason, "Unexpected DID!");
            return -1;
        }

        if (Length != responseArraySize - 3)
        {
            strcpy(issuereason, "Unexpected DataLength!");
            return -1;
        }

        for (i = 0; i < Length; i++)
        {
            TempData[i] = responseArray[i + 3];
        }

        if (memcmp(DIDdata, TempData, Length) != 0)
        {
            strcpy(issuereason, " Data Content Not Match!");
            return -1;
        }
    }
    else
    {
        strcpy(issuereason, "Unexpected Response!");
        return -1;
    }

   

    return 0;
}
s32 CheckPosResponse_SID22(u8 DID[2], u32 Length, u8* DIDdata)
{
    if (0 == Raw_CheckPosResponse_SID22(DID, Length,DIDdata))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// 检查当前会话模式
/// </summary>
/// <param name="DiagnosticSession"></param> 诊断会话
/// <param name="AddressMode"></param> 寻址方式
/// <returns></returns>
s32 CheckCurrentSession(u8 DiagnosticSession, u8 AddressMode = 0)
{
    u8 DID[2] = { 0xF1,0x86 };
    u8 Session[2] = {};

    Session[0] = DiagnosticSession;
    SID22_ReadDataByIdentifier(DID, AddressMode);
    CheckPosResponse_SID22(DID, 1, Session);

    switch (TpRxData[3])
    {
    case 1:
        TestReport_HTML_StepDescription("Current Session is DefaultSession");
        return 0;
    case 2:
        TestReport_HTML_StepDescription("Current Session is ProgrammingSession");
        return 0;
    case 3:
        TestReport_HTML_StepDescription("Current Session is ExtendedSession");
        return 0;
    default:
        TestReport_HTML_StepDescription("Unexpected Session");
        return-1;
    }
}

s32 Raw_SID23_ReadDataByAddress(u8 AddLenFID, u32 memoryAddress , u32 memorySize, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID23[20] = {};
    SID23[0] = 0x23;
    SID23[1] = AddLenFID;
    for (int i = 0; i < (AddLenFID & 0xf); i++)
        SID23[2 + i] = memoryAddress>>(8*((AddLenFID & 0xf)-i-1));
    for (int j = 0; j < (AddLenFID >> 4); j++)
        SID23[2 + (AddLenFID & 0xf) + j] = memorySize >> (8 * ((AddLenFID >>4) - j - 1));

    s32 responseArraySize = 65535;

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID23, 2+ (AddLenFID & 0xf)+ (AddLenFID >> 4), responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID23, 2 + (AddLenFID & 0xf) + (AddLenFID >> 4), responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }

    }
}
s32 SID23_ReadDataByAddress(u8 AddLenFID, u32 memoryAddress, u32 memorySize, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID23_ReadDataByAddress(AddLenFID, memoryAddress,memorySize,AddressMode);

    sprintf(requestInfo, "%s%x %s%x %s%x", "23 ", AddLenFID,"", memoryAddress, "",memorySize);

    return 0;
}

s32 Raw_CheckPosResponse_SID23(u8* responseArray = TpRxData, s8 responseArraySize = TpRxLength)
{
    strcpy(expectedResponse, "63 ");
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x63)
    {
        app.log_text("Positive Response Received!", lvlOK);
    }
    else
    {
        app.log_text("Unexpected Response!", lvlError);
        strcpy(issuereason, "Unexpected Response!");
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID23()
{
    if (0 == Raw_CheckPosResponse_SID23())
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        resultFlag++;
        return -1;
    }
}

/// <summary>
/// Service0x14 清除全部诊断故障码
/// </summary>
/// <param name="AddressMode"></param> AddressMode $14服务寻址方式（0 = Physical , 1 = Functional）
/// <param name="responseArray"></param> responseArray $14服务获取响应数据内容
s32 Raw_SID14_ClearAllDTC(u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID14[4] = {};
    SID14[0] = 0x14;
    SID14[1] = 0xFF;
    SID14[2] = 0xFF;
    SID14[3] = 0xFF;
    s32 responseArraySize = 10;
    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID14, 4, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID14, 4, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID14_ClearAllDTC(u8 AddressMode = 0)
{
    TpRxLength = Raw_SID14_ClearAllDTC(AddressMode);
    
    strcpy(requestInfo, "14 FF FF FF");
    return 0;
}

/// <summary>
/// Service0x14 清除特定诊断故障码
/// </summary>
/// <param name="AddressMode"></param> AddressMode $14服务寻址方式（0 = Physical , 1 = Functional）
/// <param name="responseArray"></param> responseArray $14服务获取响应数据内容
s32 Raw_SID14_ClearDTCInfo(u8 GDTC[3], u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID14[4] = {};
    SID14[0] = 0x14;
    SID14[1] = GDTC[0];
    SID14[2] = GDTC[1];
    SID14[3] = GDTC[2];
    s32 responseArraySize = 10;
    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID14, 4, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID14, 4, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID14_ClearDTCInfo(u8 GDTC[3], u8 AddressMode = 0)
{
    TpRxLength = Raw_SID14_ClearDTCInfo(GDTC, AddressMode);
    
    strcpy(requestInfo, ("14 " + dec2hex(GDTC[0]) + " " + dec2hex(GDTC[1]) + " " + dec2hex(GDTC[2])).c_str());
    return 0;
}

/// <summary>
/// Service0x14 $14服务检查肯定响应
/// </summary>
/// <param name="responseArray"></param> responseArray $14服务响应数据内容
/// <param name="responseArray"></param> responseArray $14服务响应数据内容
s32 Raw_CheckPosResponse_SID14(s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, "54");
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("0x" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" 0x" + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x54)
    {
        app.log_text("Positive Response Received！", lvlOK);
    }
    else
    {
        app.log_text("Unexpected Response Received!", lvlError);
        strcpy(issuereason, "Unexpected Response Received!");
        return -1;
    }

    if (responseArraySize != 1)
    {
        app.log_text("Error！Invalid Data Length!", lvlOK);
        strcpy(issuereason, "Error！Invalid Data Length!");
        return -1;
    }
    return 0;
}
s32 CheckPosResponse_SID14()
{
    if (0 == Raw_CheckPosResponse_SID14())
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// Service0x28 通讯控制
/// </summary>
/// <param name="SubFunction"></param> SubFunction $28服务子功能
/// <param name="CommunicationType"></param> CommunicationType $28服务通讯类型
/// <param name="AddressMode"></param> AddressMode $28服务寻址方式（0 = Physical , 1 = Functional）
/// <param name="responseArray"></param> responseArray $28服务获取数据内容
s32 Raw_SID28_CommunicationControl(u8 SubFunction, u8 CommunicationType, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID28[3] = {};
    SID28[0] = 0x28;
    SID28[1] = SubFunction;
    SID28[2] = CommunicationType;
    s32 responseArraySize = 10;

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID28, 3, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID28, 3, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID28_CommunicationControl(u8 SubFunction, u8 CommunicationType, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID28_CommunicationControl(SubFunction,CommunicationType);
    
    strcpy(requestInfo, ("28 " + dec2hex(SubFunction) + dec2hex(CommunicationType)).c_str());
    return 0;
}

/// <summary>
/// Service0x28 $28服务检查肯定响应
/// </summary>
/// <param name="SubFunction"></param> SubFunction $28服务子功能
/// <param name="responseArray"></param> responseArray $28服务获取数据内容
s32 Raw_CheckPosResponse_SID28(u8 SubFunction, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("68 " + dec2hex(SubFunction)).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("0x" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" 0x" + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x68)
    {
        app.log_text("Positive Response Received!", lvlOK);
        if (responseArray[1] != SubFunction)
        {
            app.log_text("Unexpected SubFunction Received!", lvlError);
            strcpy(issuereason, "Unexpected SubFunction Received!");
            return -1;
        }
    }
    else
    {
        app.log_text("Unexpected Response Received!", lvlError);
        strcpy(issuereason, "Unexpected Response Received!");
        return -1;
    }

    if (2 != responseArraySize)
    {
        app.log_text("Error!Invalid Data Length!", lvlError);
        strcpy(issuereason, "Error!Invalid Data Length!");
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID28(u8 SubFunction)
{
    if (0 == Raw_CheckPosResponse_SID28(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

s32 CheckCommunicationMessage(u8 ifAPPprohibited, u8 ifNMprohibited) {
    APP_Flag = 0;
    NM_Flag = 0;
    TestWait(5000);
    if (ifAPPprohibited == 1) {
        if (APP_Flag == 1)
        {
            strcpy(issuereason, "发送28服务禁用APP报文后，ECU仍然发送APP报文！");
            sprintf(expectedResponse, " %s", "ECU不再发送APP报文!");
            sprintf(actualResponse, " %s", "ECU仍然发送APP报文!");
            TestReport_HTML_StepDescription("监测到APP报文！",Fail);
        }
        else
        {
            TestReport_HTML_StepDescription("未监测到APP报文！",Pass);
        }
    }
    else
    {
        if (APP_Flag == 0)
        {
            strcpy(issuereason, "发送28服务启用APP报文后，ECU仍然不发送APP报文！");
            sprintf(expectedResponse, " %s", "ECU发送APP报文!");
            sprintf(actualResponse, " %s", "ECU仍然不发送APP报文!");
            TestReport_HTML_StepDescription("未监测到APP报文！", Fail);
        }
        else
        {
            TestReport_HTML_StepDescription("监测到APP报文！",Pass);
        }
    }

    if (ifNMprohibited == 1 ) {
        if (NM_Flag == 1)
        {
            strcpy(issuereason, "发送28服务禁用NM报文后，ECU仍然发送NM报文！");
            sprintf(expectedResponse, " %s", "ECU不再发送NM报文!");
            sprintf(actualResponse, " %s", "ECU仍然发送APP报文!");
            TestReport_HTML_StepDescription("监测到NM报文！", Fail);
        }
        else
        {
            TestReport_HTML_StepDescription("未监测到NM报文！",Pass);
        }
    }
    else
    {
        if (NM_Flag == 0)
        {
            strcpy(issuereason, "发送28服务启用NM报文后，ECU仍然不发送NM报文！");
            sprintf(expectedResponse, " %s", "ECU发送NM报文!");
            sprintf(actualResponse, " %s", "ECU仍然不发送NM报文!");
            TestReport_HTML_StepDescription("未监测到NM报文！", Fail);
        }
        else
        {
            TestReport_HTML_StepDescription("监测到NM报文！",Pass);
        }
    }
    return 0;
}

/// <summary>
/// Service0x11 ECU复位
/// </summary>
/// <param name="SubFunction"></param> SubFunction $11服务子功能
/// <param name="AddressMode"></param> AddressMode $11服务寻址方式（0 = Physical , 1 = Functional）
/// <param name="responseArray"></param> responseArray 获取$11服务的响应数据内容
s32 Raw_SID11_ECUReset(u8 SubFunction, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID11[2] = {};
    SID11[0] = 0x11;
    SID11[1] = SubFunction;
    s32 responseArraySize = 10;

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID11, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID11, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID11_ECUReset(u8 SubFunction, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID11_ECUReset(SubFunction,AddressMode);

    strcpy(requestInfo, ("11 " + dec2hex(SubFunction)).c_str());
    return 0;

}

/// <summary>
/// Service0x11 $11服务检查肯定响应
/// </summary>
/// <param name="SubFunction"></param> SubFunction $11服务子功能
/// <param name="responseArraySize"></param> responseArraySize $11服务响应数据长度
/// <param name="responseArray"></param> responseArray $11服务获取数据内容
s32 Raw_CheckPosResponse_SID11(u8 SubFunction, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("11 " + dec2hex(SubFunction)).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x51) {
        app.log_text("Positive Response Received!", lvlOK);
        if (responseArray[1] != SubFunction)
        {
            strcpy(issuereason, "Unexpected SubFunction Received!");
            return -1;
        }
    }
    else {
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    if (2 != responseArraySize)
    {
        strcpy(issuereason, "Invalid Data Length!");
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID11(u8 SubFunction)
{
    if (0 == Raw_CheckPosResponse_SID11(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}


/// <summary>
/// Service0x85 控制DTC设置
/// </summary>
/// <param name="SubFunction"></param> SubFunction $85服务子功能
/// <param name="AddressMode"></param> AddressMode $85服务寻址方式（0 = Physical , 1 = Functional）
/// <param name="responseArray"></param> responseArray $85服务获取数据内容
s32 Raw_SID85_ControlDTCSetting(u8 SubFunction, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID85[2] = {};
    SID85[0] = 0x85;
    SID85[1] = SubFunction;
    s32 responseArraySize = 10;

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID85, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID85, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID85_ControlDTCSetting(u8 SubFunction, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID85_ControlDTCSetting(SubFunction,AddressMode);
    
    strcpy(requestInfo, ("85 " + dec2hex(SubFunction)).c_str());
    return 0;
}

/// <summary>
/// Service0x85 $5服务检查肯定响应
/// </summary>
/// <param name="SubFunction"></param> SubFunction $85服务子功能
/// <param name="SubFunction"></param> SubFunction $85服务响应数据长度
/// <param name="responseArray"></param> responseArray $85服务获取数据内容
s32 Raw_CheckPosResponse_SID85(u8 SubFunction, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("85 " + dec2hex(SubFunction)).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {

        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0xC5) {
        app.log_text("Positive Response Received!", lvlOK);
        if (responseArray[1] != SubFunction)
        {
            strcpy(issuereason, "Unexpected Subfunction Received!");
            app.log_text("Unexpected SubFunction Received!", lvlError);
            return -1;
        }
    }
    else {
        strcpy(issuereason, "No Response Received!");
        app.log_text("No Response Received!", lvlError);
        return -1;
    }

    if (2 != responseArraySize)
    {
        strcpy(issuereason, "Invalid Data Length!");
        app.log_text("Error!Invalid Data Length!", lvlError);
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID85(u8 SubFunction)
{
    if (0 == Raw_CheckPosResponse_SID85(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// Service0x3E 诊断仪在线
/// </summary>
/// <param name="SubFunction"></param> SubFunction $3E服务子功能
/// <param name="AddressMode"></param> AddressMode $3E服务寻址方式（0 = Physical , 1 = Functional）
/// <param name="responseArray"></param> responseArray $3E服务获取数据内容
s32 Raw_SID3E_TesterPresent(u8 SubFunction, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID3E[2] = {};
    SID3E[0] = 0x3E;
    SID3E[1] = SubFunction;
    s32 responseArraySize = 10;

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID3E, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID3E, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID3E_TesterPresent(u8 SubFunction, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID3E_TesterPresent(SubFunction, AddressMode);

    strcpy(requestInfo, ("3E " + dec2hex(SubFunction)).c_str());
    return 0;
}

/// <summary>
/// Service0x3E 检查3E服务肯定响应
/// </summary>
/// <param name="SubFunction"></param> SubFunction $3E服务子功能
/// <param name="responseArraySize"></param> responseArraySize $3E服务响应数据长度
/// <param name="responseArray"></param> responseArray $3E服务数据内容
s32 Raw_CheckPosResponse_SID3E(u8 SubFunction, s64 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("3E " + dec2hex(SubFunction)).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {

        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("0x" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" 0x" + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x7E) {
        app.log_text("Positive Response Received!", lvlOK);
        if (responseArray[1] != SubFunction)
        {
            strcpy(issuereason, "Unexpected Subfunction Received!");
            app.log_text("Unexpected SubFunction Received!", lvlError);
            return -1;
        }
    }
    else {
        strcpy(issuereason, "No Response Received!");
        app.log_text("No Response Received!", lvlError);
        return -1;
    }

    if (2 != responseArraySize)
    {
        strcpy(issuereason, "Invalid Data Length!");
        app.log_text("Error!Invalid Data Length!", lvlError);
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID3E(u8 SubFunction)
{
    if (0 == Raw_CheckPosResponse_SID3E(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        resultFlag++;
        return -1;
    }
}


/// <summary>
/// Service0x31 例程控制
/// </summary>
/// <param name="SubFunction"></param> SubFunction $31服务子功能
/// <param name="RID"></param> RID $31服务例程ID
/// <param name="OptionRecord"></param> OptionRecord $31服务额外内容
/// <param name="OptionRecordLength"></param> OptionRecordLength $31服务额外内容长度
/// <param name="responseArray"></param> responseArray $31服务获取数据内容
s32 Raw_SID31_RoutineControl(u8 SubFunction, u16 RID, u8* OptionRecord = NULL, u64 OptionRecordLength =0 , u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID31[300] = {};
    u64 reqLength = 0;
    s32 responseArraySize = 20;
    SID31[0] = 0x31;
    SID31[1] = SubFunction;
    SID31[2] = RID>>8;
    SID31[3] = RID;
    for (int i = 0; i < OptionRecordLength; i++)
    {
        SID31[i + 4] = OptionRecord[i];
    }
    
    reqLength = OptionRecordLength + 4;

    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID31, reqLength, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else 
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID31, reqLength, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID31_RoutineControl(u8 SubFunction, u16 RID, u64 OptionRecordLength = 0,u8* OptionRecord = NULL, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID31_RoutineControl(SubFunction, RID, OptionRecord, OptionRecordLength, AddressMode);
    
    strcpy(requestInfo, ("31 " + dec2hex(SubFunction) + " " + dec2hex(RID >> 8) + " " + dec2hex(RID & 0xff)).c_str());
    return 0;
}

/// <summary>
/// Service0x31 例程控制
/// </summary>
/// <param name="SubFunction"></param> SubFunction $31服务子功能
/// <param name="RID"></param> RID $31服务例程ID
/// <param name="OptionRecord"></param> OptionRecord $31服务额外内容
/// <param name="OptionRecordLength"></param> OptionRecordLength $31服务额外内容长度
/// <param name="responseArray"></param> responseArray $31服务获取数据内容
s32 Raw_SID31_RoutineControl_EraseMemory(u8* OptionRecord = NULL, u64 OptionRecordLength = 0, u8 AddressMode = 0)
{
    u8 SID31[300] = {};
    u64 reqLength = 0;
    s32 responseArraySize = 20;
    SID31[0] = 0x31;
    SID31[1] = 0x01;
    SID31[2] = 0xFF;
    SID31[3] = 0x00;
    for (int i = 0; i < OptionRecordLength; i++)
    {
        SID31[i + 4] = OptionRecord[i];
    }

    reqLength = OptionRecordLength + 4;

    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_send_request(udsHandle, SID31, reqLength) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_send_request(udsHandle, SID31, reqLength) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID31_RoutineControl_EraseMemory(u8* OptionRecord = NULL, u64 OptionRecordLength = 0, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID31_RoutineControl_EraseMemory(OptionRecord, OptionRecordLength, AddressMode);

    strcpy(requestInfo, "31 01 FF 00 ");
    for (int i = 0; i < OptionRecordLength; i++)
    {
        strcat(requestInfo, dec2hex(OptionRecord[i]).c_str());
    }

    return 0;
}

/// <summary>
/// Service0x31 检查31服务肯定响应
/// </summary>
/// <param name="SubFunction"></param> SubFunction $31服务子功能
/// <param name="RID"></param> RID $31服务例程ID
/// <param name="optionRecord"></param> optionRecord $31服务额外内容
/// <param name="optionRecordLength"></param> optionRecordLength $31服务额外内容长度
/// <param name="responseArray"></param> responseArray $31服务数据内容
s32 Raw_CheckPosResponse_SID31(u8 SubFunction, u16 RID, u32 optionRecordLength, u8 optionRecord[], s8 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    u32 i;

    strcpy(expectedResponse, ("71 " + dec2hex(SubFunction) + " " + dec2hex(RID >> 8) + " " + dec2hex(RID & 0xff)).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {

        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x71)
    {
        app.log_text("Positive Response Received!", lvlOK);

        if (responseArray[1] != SubFunction)
        {
            strcpy(issuereason, "Unexpected Subfunction Received!");
            app.log_text("Unexpected SubFunction Received!", lvlError);
            return -1;
        }
    }
    else {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected Response Received!", lvlError);
        return -1;
    }

    if (responseArray[2] != (RID>>8) || responseArray[3] != (RID&0xff))
    {
        strcpy(issuereason, "Unexpected RoutineIdentifier Received!");
        app.log_text("Unexpected RoutienIdentifier Received!", lvlError);
        return -1;
    }

    for (i = 0; i < optionRecordLength; i++)
    {
        if (responseArray[i + 4] != optionRecord[i])
        {
            strcpy(issuereason, "Unexpected optionRecord Received!");
            app.log_text("Unexpected optionRecord Received!", lvlError);
            return -1;
        }
        else
            app.log_text("Correct optionRecord Received!", lvlOK);
    }

    if (optionRecordLength + 4 != responseArraySize)
    {
        strcpy(issuereason, "Invalid Data Length Received!");
        app.log_text("Error!Invalid Data Length!", lvlError);
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID31(u8 SubFunction, u16 RID, u32 optionRecordLength = 0, u8 optionRecord[] = {})
{
    if (0 == Raw_CheckPosResponse_SID31(SubFunction, RID, optionRecordLength,optionRecord ))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}


/// <summary>
/// Service0x31 检查31服务肯定响应
/// </summary>
/// <param name="SubFunction"></param> SubFunction $31服务子功能
/// <param name="RID"></param> RID $31服务例程ID
/// <param name="optionRecord"></param> optionRecord $31服务额外内容
/// <param name="optionRecordLength"></param> optionRecordLength $31服务额外内容长度
/// <param name="responseArray"></param> responseArray $31服务数据内容
s32 Raw_CheckPosResponse_SID31_forCCR(s8 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    u8 DataCCR[27][5] = {};
    u8 checkflag = 0;
    DataCCR[0][0] = 0x10;  DataCCR[0][1] = 0x00; DataCCR[0][2] = 0x00; DataCCR[0][3] = 0x00; DataCCR[0][4] = 0x11;
    DataCCR[1][0] = 0x10;  DataCCR[1][1] = 0x00; DataCCR[1][2] = 0x00; DataCCR[1][3] = 0x00; DataCCR[1][4] = 0x21;
    DataCCR[2][0] = 0x10;  DataCCR[2][1] = 0x00; DataCCR[2][2] = 0x00; DataCCR[2][3] = 0x00; DataCCR[2][4] = 0x41;
    DataCCR[3][0] = 0x10;  DataCCR[3][1] = 0x00; DataCCR[3][2] = 0x00; DataCCR[3][3] = 0x01; DataCCR[3][4] = 0x01;
    DataCCR[4][0] = 0x10;  DataCCR[4][1] = 0x00; DataCCR[4][2] = 0x00; DataCCR[4][3] = 0x02; DataCCR[4][4] = 0x01;
    DataCCR[5][0] = 0x10;  DataCCR[5][1] = 0x00; DataCCR[5][2] = 0x00; DataCCR[5][3] = 0x04; DataCCR[5][4] = 0x01;
    DataCCR[6][0] = 0x10;  DataCCR[6][1] = 0x00; DataCCR[6][2] = 0x00; DataCCR[6][3] = 0x08; DataCCR[6][4] = 0x01;
    DataCCR[7][0] = 0x10;  DataCCR[7][1] = 0x00; DataCCR[7][2] = 0x00; DataCCR[7][3] = 0x10; DataCCR[7][4] = 0x01;
    DataCCR[8][0] = 0x10;  DataCCR[8][1] = 0x00; DataCCR[8][2] = 0x00; DataCCR[8][3] = 0x20; DataCCR[8][4] = 0x01;
    DataCCR[9][0] = 0x10;  DataCCR[9][1] = 0x00; DataCCR[9][2] = 0x00; DataCCR[9][3] = 0x40; DataCCR[9][4] = 0x01;
    DataCCR[10][0] = 0x10; DataCCR[10][1] = 0x00; DataCCR[10][2] = 0x00; DataCCR[10][3] = 0x80; DataCCR[10][4] = 0x01;
    DataCCR[11][0] = 0x10; DataCCR[11][1] = 0x00; DataCCR[11][2] = 0x01; DataCCR[11][3] = 0x00; DataCCR[11][4] = 0x01;
    DataCCR[12][0] = 0x10; DataCCR[12][1] = 0x00; DataCCR[12][2] = 0x02; DataCCR[12][3] = 0x00; DataCCR[12][4] = 0x01;
    DataCCR[13][0] = 0x10; DataCCR[13][1] = 0x00; DataCCR[13][2] = 0x04; DataCCR[13][3] = 0x00; DataCCR[13][4] = 0x01;
    DataCCR[14][0] = 0x10; DataCCR[14][1] = 0x00; DataCCR[14][2] = 0x08; DataCCR[14][3] = 0x00; DataCCR[14][4] = 0x01;
    DataCCR[15][0] = 0x10; DataCCR[15][1] = 0x00; DataCCR[15][2] = 0x10; DataCCR[15][3] = 0x00; DataCCR[15][4] = 0x01;
    DataCCR[16][0] = 0x10; DataCCR[16][1] = 0x00; DataCCR[16][2] = 0x20; DataCCR[16][3] = 0x00; DataCCR[16][4] = 0x01;
    DataCCR[17][0] = 0x10; DataCCR[17][1] = 0x00; DataCCR[17][2] = 0x40; DataCCR[17][3] = 0x00; DataCCR[17][4] = 0x01;
    DataCCR[18][0] = 0x10; DataCCR[18][1] = 0x00; DataCCR[18][2] = 0x80; DataCCR[18][3] = 0x00; DataCCR[18][4] = 0x01;
    DataCCR[19][0] = 0x10; DataCCR[19][1] = 0x01; DataCCR[19][2] = 0x00; DataCCR[19][3] = 0x00; DataCCR[19][4] = 0x01;
    DataCCR[20][0] = 0x10; DataCCR[20][1] = 0x02; DataCCR[20][2] = 0x00; DataCCR[20][3] = 0x00; DataCCR[20][4] = 0x01;
    DataCCR[21][0] = 0x10; DataCCR[21][1] = 0x04; DataCCR[21][2] = 0x00; DataCCR[21][3] = 0x00; DataCCR[21][4] = 0x01;
    DataCCR[22][0] = 0x10; DataCCR[22][1] = 0x08; DataCCR[22][2] = 0x00; DataCCR[22][3] = 0x00; DataCCR[22][4] = 0x01;
    DataCCR[23][0] = 0x10; DataCCR[23][1] = 0x10; DataCCR[23][2] = 0x00; DataCCR[23][3] = 0x00; DataCCR[23][4] = 0x01;
    DataCCR[24][0] = 0x10; DataCCR[24][1] = 0x20; DataCCR[24][2] = 0x00; DataCCR[24][3] = 0x00; DataCCR[24][4] = 0x01;
    DataCCR[25][0] = 0x10; DataCCR[25][1] = 0x40; DataCCR[25][2] = 0x00; DataCCR[25][3] = 0x00; DataCCR[25][4] = 0x01;
    DataCCR[26][0] = 0x10; DataCCR[26][1] = 0x80; DataCCR[26][2] = 0x00; DataCCR[26][3] = 0x00; DataCCR[26][4] = 0x01;

    strcpy(expectedResponse, ("71 01 02 05"));
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {

        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x71)
    {
        app.log_text("Positive Response Received!", lvlOK);

        if (responseArray[1] != 0x01)
        {
            strcpy(issuereason, "Unexpected Subfunction Received!");
            app.log_text("Unexpected SubFunction Received!", lvlError);
            return -1;
        }
    }
    else {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected Response Received!", lvlError);
        return -1;
    }

    if (responseArray[2] != 0x02 || responseArray[3] != 0x05)
    {
        strcpy(issuereason, "Unexpected RoutineIdentifier Received!");
        app.log_text("Unexpected RoutienIdentifier Received!", lvlError);
        return -1;
    }

    for(int m=0;m<27;m++)
    {
        checkflag = 1;
        for (int i = 0; i < 5; i++)
        {
            if (DataCCR[m][i] != responseArray[i + 5])
                checkflag = 0;
        }
        if (checkflag == 1)
            break;
    }

    if (checkflag!=1)
    {
        strcpy(issuereason, "Unexpected optionRecord Received!");
        app.log_text("Unexpected optionRecord Received!", lvlError);
        return -1;
    }
    else
        app.log_text("Correct optionRecord Received!", lvlOK);

    if (9 != responseArraySize)
    {
        strcpy(issuereason, "Invalid Data Length Received!");
        app.log_text("Error!Invalid Data Length!", lvlError);
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID31_forCCR()
{
    if (0 == Raw_CheckPosResponse_SID31_forCCR())
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}


/// <summary>
/// SID27 请求种子
/// </summary>
/// <param name="SecurityLevel"></param>  解锁等级
/// <param name="AddressMode"></param>  寻址方式，默认物理寻址
/// <param name="responseArray"></param>  获取响应
/// <returns></returns>
s32 Raw_SID27_SecurityAccess_RequestSeed(u8 SecurityLevel, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID27[2] = {};
    SID27[0] = 0x27;
    SID27[1] = SecurityLevel;
    s32 responseArraySize = 100;

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID27, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID27, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID27_SecurityAccess_RequestSeed(u8 SecurityLevel, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID27_SecurityAccess_RequestSeed(SecurityLevel, AddressMode);
    
    strcpy(requestInfo, ("27 " + dec2hex(SecurityLevel)).c_str());
    return 0;
}

/// <summary>
/// SID27 检查请求种子的响应
/// </summary>
/// <param name="SecurityLevel"></param>安全等级
/// <param name="responseArraySize"></param>响应数据长度
/// <param name="responseArray"></param>响应数据内容
/// <returns></returns>
s32 Raw_CheckPosResponse_SID27_RequestSeed(u8 SecurityLevel, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("67 " + dec2hex(SecurityLevel)).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }
    if (responseArray[0] == 0x67)
    {
        if (responseArraySize == 18)
        {
            app.log_text("ECU give positive response!", lvlOK);
            if (responseArray[1] != SecurityLevel)
            {
                app.log_text("Unexpected Subfunction!", lvlError);
                strcpy(issuereason, "Unexpected Subfunction!");
                return -1;
            }
            for (int i = 0; i < responseArraySize - 2; i++)
            {
                SeedArray[i] = responseArray[i + 2];
                //printf("SeedArray:%x", SeedArray[i]);
            }
            return 0;
        }
        else
        {
            app.log_text("Incorrect response array length! ", lvlError);
            strcpy(issuereason, "Incorrect response array length!");
            return -1;
        }
    }
    else
    {
        app.log_text("Unexpected  Resposne!", lvlError);
        strcpy(issuereason, "Unexpected Response Received!");
        return -1;
    }
}
s32 CheckPosResponse_SID27_RequestSeed(u8 SubFunction)
{
    if (0 == Raw_CheckPosResponse_SID27_RequestSeed(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        resultFlag++;
        return -1;
    }
}

/// <summary>
/// SID27 发送密钥
/// </summary>
/// <param name="SecurityLevel"></param>安全等级
/// <param name="AddressMode"></param>寻址方式，默认物理寻址
/// <param name="responseArray"></param>获取响应数据
/// <returns></returns>
s32 Raw_SID27_SecurityAccess_SendKey(u8 SecurityLevel, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID27[18] = {};
    SID27[0] = 0x27;
    SID27[1] = SecurityLevel + 1;
    s32 responseArraySize = 100;

    for (int i = 0; i < 16; i++)
        SID27[i + 2] = KeyArray[i];

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID27, 18, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID27, 18, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID27_SecurityAccess_SendKey(u8 SecurityLevel, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID27_SecurityAccess_SendKey(SecurityLevel, AddressMode);
    
    strcpy(requestInfo, ("27 " + dec2hex(SecurityLevel + 1) ).c_str());
    for (int i = 0; i < 16; i++)
        strcat(requestInfo, (" " + dec2hex(KeyArray[i])).c_str());
    return 0;
}

/// <summary>
/// SID27 检查发送密钥的响应
/// </summary>
/// <param name="SecurityLevel"></param>安全等级
/// <param name="responseArraySize"></param>响应数据长度
/// <param name="responseArray"></param>响应数据内容
/// <returns></returns>
s32 Raw_CheckPosResponse_SID27_SendKey(u8 SecurityLevel, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("67 " + dec2hex(SecurityLevel+1)).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x67)
    {
        if (responseArraySize == 2)
        {
            app.log_text("ECU give positive response!", lvlOK);
            if (responseArray[1] != SecurityLevel + 1)
            {
                app.log_text("Unexpected Subfunction!", lvlError);
                strcpy(issuereason, "Unexpected Subfunction!");
                return -1;
            }
            return 0;
        }
        else
        {
            app.log_text("Incorrect response array length! ", lvlError);
            strcpy(issuereason, "Incorrect response array length!");
        }
        return -1;
    }
    else
    {
        app.log_text("Unexpected  Resposne!", lvlError);
        strcpy(issuereason, "Unexpected Response Received!");
        return -1;
    }
}
s32 CheckPosResponse_SID27_SendKey(u8 SecurityLevel)
{
    if (0 == Raw_CheckPosResponse_SID27_SendKey(SecurityLevel))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        resultFlag++;
        return -1;
    }
}


s32 Raw_SID29_Authentication_SF00(u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID29[2] = {};
    SID29[0] = 0x29;
    SID29[1] = 0x00;
    s32 responseArraySize = 200;

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID29, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID29, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID29_Authentication_SF00(u8 AddressMode = 0)
{
    TpRxLength = Raw_SID29_Authentication_SF00(AddressMode);

    strcpy(requestInfo, "29 00");
    return 0;
}

s32 Raw_CheckPosResponse_SID29_SF00(s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, "69 00 ");
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }
    if (responseArray[0] == 0x69)
    {
        if (responseArraySize == 2)
        {
            app.log_text("ECU give positive response!", lvlOK);
            if (responseArray[1] != 0x00)
            {
                app.log_text("Unexpected Subfunction!", lvlError);
                strcpy(issuereason, "Unexpected Subfunction!");
                return -1;
            }
            return 0;
        }
        else
        {
            app.log_text("Incorrect response array length! ", lvlError);
            strcpy(issuereason, "Incorrect response array length!");
        }
        return -1;
    }
    else
    {
        app.log_text("Unexpected  Resposne!", lvlError);
        strcpy(issuereason, "Unexpected Response Received!");
        return -1;
    }
}
s32 CheckPosResponse_SID29_SF00()
{
    if (0 == Raw_CheckPosResponse_SID29_SF00())
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

s32 Raw_SID29_Authentication_SF01(u8 communicationConfiguration, u16 lengthOfProofOfOwnershipClient, u8* certificateClient, u16 lengthOfChallengeClient, u8* challengeClient, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID29[100] = {};
    s32 responseArraySize = 100;
    SID29[0] = 0x29;
    SID29[1] = 0x01;
    SID29[2] = communicationConfiguration;
    SID29[3] = lengthOfProofOfOwnershipClient >> 8;
    SID29[4] = lengthOfProofOfOwnershipClient & 0xff;
    for (int i = 0; i < lengthOfProofOfOwnershipClient; i++)
        SID29[i + 5] = certificateClient[i];
    SID29[5+ lengthOfProofOfOwnershipClient] = lengthOfChallengeClient >> 8;
    SID29[6+ lengthOfProofOfOwnershipClient] = lengthOfChallengeClient & 0xff;
    for (int i = 0; i < lengthOfChallengeClient; i++)
        SID29[7 + i + lengthOfProofOfOwnershipClient] = challengeClient[i];

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID29, 7+ lengthOfProofOfOwnershipClient+ lengthOfChallengeClient, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID29, 7 + lengthOfProofOfOwnershipClient + lengthOfChallengeClient, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID29_Authentication_SF01(u8 communicationConfiguration, u16 lengthOfProofOfOwnershipClient, u8* certificateClient, u16 lengthOfChallengeClient, u8* challengeClient, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID29_Authentication_SF01(communicationConfiguration, lengthOfProofOfOwnershipClient, certificateClient, lengthOfChallengeClient, challengeClient, AddressMode);

    strcpy(requestInfo, ("29 01 " + dec2hex(communicationConfiguration)).c_str());
    return 0;
}

//s32 Raw_CheckPosResponse_SID29_SF01(u8 returnValue,u8* challengeServer = ChallengeServer,u8* ephemeralPublicKeyServer = EphemeralPublicKeyServer, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
//{
//    strcpy(expectedResponse, ("69 01 " + dec2hex(returnValue)).c_str());
//    if (responseArraySize < 0)
//    {
//        strcpy(actualResponse, "No Response Received!");
//        strcpy(issuereason, "No Response Received!");
//    }
//    else
//    {
//        for (int i = 0; i < responseArraySize; i++)
//        {
//            if (i == 0)
//                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
//            else
//                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
//        }
//    }
//
//    if (responseArray[0] == 0x69)
//    {
//        TestReport_HTML_StepDescription("ECU give positive response!", Pass);
//        if (responseArray[1] != 0x01)
//        {
//            strcpy(issuereason, "Unexpected Subfunction!");
//            app.log_text("Unexpected Subfunction!", lvlError);
//            return -1;
//        }
//        return 0;
//        if (responseArray[2] != returnValue)
//        {
//            strcpy(issuereason, "Unexpected return value!");
//            app.log_text("Unexpected returnValue!", lvlError);
//            return -1;
//        }
//
//        //校验ChallengeServer和EphemeralPublicKeyServer
//
//    }
//    else
//    {
//        strcpy(issuereason, "Unexpected Response Received!");
//        app.log_text("Unexpected  Resposne!", lvlError);
//        return -1;
//    }
//}
//s32 CheckPosResponse_SID29_SF01(u8 returnValue)
//{
//    if (0 == Raw_CheckPosResponse_SID29_SF01(returnValue))
//    {
//        TestReport_StepInfo(Pass);
//        return 0;
//    }
//    else
//    {
//        TestReport_StepInfo(Fail);
//        return -1;
//    }
//}

//s32 Raw_SID29_Authentication_SF02(u8 communicationConfiguration, u16 lengthOfProofOfOwnershipClient, u8* certificateClient, u16 lengthOfChallengeClient, u8* challengeClient, u8 AddressMode = 0, u8* responseArray = TpRxData)
//{
//    u8 SID29[100] = {};
//    s32 responseArraySize = 100;
//    SID29[0] = 0x29;
//    SID29[1] = 0x02;
//    SID29[2] = communicationConfiguration;
//    SID29[3] = lengthOfProofOfOwnershipClient >> 8;
//    SID29[4] = lengthOfProofOfOwnershipClient & 0xff;
//    for (int i = 0; i < lengthOfProofOfOwnershipClient; i++)
//        SID29[i + 5] = certificateClient[i];
//    SID29[5 + lengthOfProofOfOwnershipClient] = lengthOfChallengeClient >> 8;
//    SID29[6 + lengthOfProofOfOwnershipClient] = lengthOfChallengeClient & 0xff;
//    for (int i = 0; i < lengthOfChallengeClient; i++)
//        SID29[7 + i + lengthOfProofOfOwnershipClient] = challengeClient[i];
//
//    if (AddressMode == 0) {
//        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID29, 7 + lengthOfProofOfOwnershipClient + lengthOfChallengeClient, responseArray, &responseArraySize) == 0x00) {
//            app.log_text("send diagnostic payload and get response success!", lvlOK);
//            return responseArraySize;
//        }
//        else {
//            app.log_text("send diagnostic payload and get response faliled!", lvlError);
//            return -1;
//        }
//    }
//    else {
//        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID29, 7 + lengthOfProofOfOwnershipClient + lengthOfChallengeClient, responseArray, &responseArraySize) == 0x00) {
//            app.log_text("send diagnostic payload and get response success!", lvlOK);
//            return responseArraySize;
//        }
//        else {
//            app.log_text("send diagnostic payload and get response faliled!", lvlError);
//            return -1;
//        }
//    }
//}
//s32 SID29_Authentication_SF02(u8 communicationConfiguration, u16 lengthOfProofOfOwnershipClient, u8* certificateClient, u16 lengthOfChallengeClient, u8* challengeClient, u8 AddressMode = 0)
//{
//    TpRxLength = Raw_SID29_Authentication_SF02(communicationConfiguration, lengthOfProofOfOwnershipClient, certificateClient, lengthOfChallengeClient, challengeClient, AddressMode);
//    
//    strcpy(requestInfo, ("29 02 " + dec2hex(communicationConfiguration)).c_str());
//    return 0;
//}

//s32 Raw_CheckPosResponse_SID29_SF02(u8 returnValue, u8* challengeServer = ChallengeServer, u8* ephemeralPublicKeyServer = EphemeralPublicKeyServer, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
//{
//    strcpy(expectedResponse, ("69 02 " + dec2hex(returnValue)).c_str());
//    if (responseArraySize < 0)
//    {
//        strcpy(actualResponse, "No Response Received!");
//        strcpy(issuereason, "No Response Received!");
//    }
//    else
//    {
//        for (int i = 0; i < responseArraySize; i++)
//        {
//            if (i == 0)
//                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
//            else
//                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
//        }
//    }
//
//    if (responseArray[0] == 0x69)
//    {
//        app.log_text("ECU give positive response!", lvlOK);
//        if (responseArray[1] != 0x02)
//        {
//            strcpy(issuereason, "Unexpected Subfunction!");
//            app.log_text("Unexpected Subfunction!", lvlError);
//            return -1;
//        }
//        return 0;
//        if (responseArray[2] != returnValue)
//        {
//            strcpy(issuereason, "Unexpected return value!");
//            app.log_text("Unexpected returnValue!", lvlError);
//            return -1;
//        }
//
//        //校验ChallengeServer和EphemeralPublicKeyServer
//
//    }
//    else
//    {
//        strcpy(issuereason, "Unexpected Response Received!");
//        app.log_text("Unexpected  Resposne!", lvlError);
//        return -1;
//    }
//}
//s32 CheckPosResponse_SID29_SF02(u8 returnValue)
//{
//    if (0 == Raw_CheckPosResponse_SID29_SF02(returnValue))
//    {
//        TestReport_StepInfo(Pass);
//        return 0;
//    }
//    else
//    {
//        TestReport_StepInfo(Fail);
//        return -1;
//    }
//}

//s32 Raw_SID29_Authentication_SF03(u8 communicationConfiguration, u16 lengthOfProofOfOwnershipClient, u8* proofOfOwnershipClient, u16 lengthOfEphemeralPublicKeyClient, u8* ephemeralPublicKeyClient, u8 AddressMode = 0, u8* responseArray = TpRxData)
//{
//    u8 SID29[100] = {};
//    s32 responseArraySize = 100;
//    SID29[0] = 0x29;
//    SID29[1] = 0x03;
//    SID29[2] = communicationConfiguration;
//    SID29[3] = lengthOfProofOfOwnershipClient >> 8;
//    SID29[4] = lengthOfProofOfOwnershipClient & 0xff;
//    for (int i = 0; i < lengthOfProofOfOwnershipClient; i++)
//        SID29[i + 5] = proofOfOwnershipClient[i];
//    SID29[5 + lengthOfProofOfOwnershipClient] = lengthOfEphemeralPublicKeyClient >> 8;
//    SID29[6 + lengthOfProofOfOwnershipClient] = lengthOfEphemeralPublicKeyClient & 0xff;
//    for (int i = 0; i < lengthOfEphemeralPublicKeyClient; i++)
//        SID29[7 + i + lengthOfProofOfOwnershipClient] = ephemeralPublicKeyClient[i];
//
//    if (AddressMode == 0) {
//        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID29, 7 + lengthOfProofOfOwnershipClient + lengthOfEphemeralPublicKeyClient, responseArray, &responseArraySize) == 0x00) {
//            app.log_text("send diagnostic payload and get response success!", lvlOK);
//            return responseArraySize;
//        }
//        else {
//            app.log_text("send diagnostic payload and get response faliled!", lvlError);
//            return -1;
//        }
//    }
//    else {
//        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID29, 7 + lengthOfProofOfOwnershipClient + lengthOfEphemeralPublicKeyClient, responseArray, &responseArraySize) == 0x00) {
//            app.log_text("send diagnostic payload and get response success!", lvlOK);
//            return responseArraySize;
//        }
//        else {
//            app.log_text("send diagnostic payload and get response faliled!", lvlError);
//            return -1;
//        }
//    }
//}
//s32 SID29_Authentication_SF03(u8 communicationConfiguration, u16 lengthOfProofOfOwnershipClient, u8* proofOfOwnershipClient, u16 lengthOfEphemeralPublicKeyClient, u8* ephemeralPublicKeyClient, u8 AddressMode = 0)
//{
//    TpRxLength = Raw_SID29_Authentication_SF03(communicationConfiguration, lengthOfProofOfOwnershipClient, ProofOfOwnershipClient, lengthOfEphemeralPublicKeyClient, ephemeralPublicKeyClient, AddressMode);
//    strcpy(requestInfo, ("29 03 " + dec2hex(communicationConfiguration)).c_str());
//    return 0;
//}
//
//s32 Raw_CheckPosResponse_SID29_SF03(u8 returnValue, u16 lengthOfSessionKeyInfo, u8* sessionKeyInfo, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
//{
//    strcpy(expectedResponse, ("69 03 " + dec2hex(returnValue)).c_str());
//
//    if (responseArraySize < 0)
//    {
//        strcpy(actualResponse, "No Response Received!");
//        strcpy(issuereason, "No Response Received!");
//    }
//    else
//    {
//        for (int i = 0; i < responseArraySize; i++)
//        {
//            if (i == 0)
//                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
//            else
//                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
//        }
//    }
//
//    if (responseArray[0] == 0x69)
//    {
//        app.log_text("ECU give positive response!", lvlOK);
//        if (responseArray[1] != 0x03)
//        {
//            strcpy(issuereason, "Unexpected Subfunction!");
//            app.log_text("Unexpected Subfunction!", lvlError);
//            return -1;
//        }
//        return 0;
//        if (responseArray[2] != returnValue)
//        {
//            strcpy(issuereason, "Unexpected return value!");
//            app.log_text("Unexpected returnValue!", lvlError);
//            return -1;
//        }
//
//        //校验lengthOfSessionKeyInfo和sessionKeyInfo
//
//    }
//    else
//    {
//        app.log_text("Unexpected  Resposne!", lvlError);
//        strcpy(issuereason, "Unexpected Response Received!");
//        return -1;
//    }
//}
//s32 CheckPosResponse_SID29_SF03(u8 returnValue, u16 lengthOfSessionKeyInfo, u8* sessionKeyInfo)
//{
//    if (0 == Raw_CheckPosResponse_SID29_SF03(returnValue, lengthOfSessionKeyInfo, sessionKeyInfo))
//    {
//        TestReport_StepInfo(Pass);
//        return 0;
//    }
//    else
//    {
//        TestReport_StepInfo(Fail);
//        return -1;
//    }
//}


s32 Raw_SID29_Authentication_SF08(u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID29[2] = {};
    s32 responseArraySize = 100;
    SID29[0] = 0x29;
    SID29[1] = 0x08;

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID29, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID29, 2, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID29_Authentication_SF08(u8 AddressMode = 0)
{
    TpRxLength = Raw_SID29_Authentication_SF08(AddressMode);
    strcpy(requestInfo, "29 08");
    return 0;
}

s32 Raw_CheckPosResponse_SID29_SF08(u8 returnValue, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("69 08 " + dec2hex(returnValue)).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x69)
    {
        app.log_text("ECU give positive response!", lvlOK);
        if (responseArray[1] != 0x04)
        {
            strcpy(issuereason, "Unexpected Subfunction!");
            app.log_text("Unexpected Subfunction!", lvlError);
            return -1;
        }
        return 0;
        if (responseArray[2] != returnValue)
        {
            strcpy(issuereason, "Unexpected return value!");
            app.log_text("Unexpected returnValue!", lvlError);
            return -1;
        }
    }
    else
    {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected  Resposne!", lvlError);
        return -1;
    }
}
s32 CheckPosResponse_SID29_SF08(u8 returnValue)
{
    if (0 == Raw_CheckPosResponse_SID29_SF08(returnValue))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}


s32 Raw_SID19_ReadDTC(pu8 SubFunction, s32 subsize,  u8 Mode = Physical, u8* responseArray = TpRxData) {
    u8 SID19[10] = {};
    s32 responseArraySize = 500;
    SID19[0] = 0x19;
    for (int i = 0; i < subsize; i++) {
        SID19[i + 1] = SubFunction[i];
    }
    if (Mode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID19, subsize + 1, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID19, subsize + 1, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID19_ReadDTC(pu8 SubFunction, s32 subsize, u8 Mode = Physical)
{
    TpRxLength = Raw_SID19_ReadDTC(SubFunction, subsize, Mode);

    strcpy(requestInfo, "19");
    for (int i = 0; i < subsize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }
    return 0;
}

//处理DTC码为字符串
std::string intToHexStr(u32 mask, int bit = 6) {
    // 最终16进制字符串长度为偶数
    std::string hexes[16] = { "0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F" };
    std::string hexstring;
    int h;
    while (mask > 15)
    {
        h = mask % 16;
        mask = mask / 16;
        hexstring = hexes[h] + hexstring;
    }
    hexstring = hexes[mask] + hexstring;
    if (hexstring.size() < bit) {
        for (int i = 0; i < bit - hexstring.size(); ) {
            hexstring = "0" + hexstring;
        }
    }
    hexstring = "0x" + hexstring;
    return hexstring;
}
//处理19 02 以及 19 0A 返回的DTC码
std::string  dealDTC(u8* responseArray, int responseArraySize) {
    std::string hex_DTC;
    for (int i = 3; i < responseArraySize; i += 4) {
        u32 evdtc = 0;
        for (int a = 0; a < 4; a++) {
            evdtc += responseArray[i + a];
            if (a < 3)
                evdtc <<= 8;
        }
        hex_DTC += intToHexStr(evdtc) + "/";
    }
    return hex_DTC;
}

/// <summary>
/// 检查19服务的读取回来的内容
/// </summary>
/// <param name="SubFunction"></param> 19子服务
/// <param name="subsize"></param> 子服务的长度
/// <param name="responseArraySize"></param> 读取回来的数组长度
/// <param name="responseArray"></param> 读取回来的数组
/// <returns></returns> 返回来的是检查的正确与否，正确为0，错误为-1
s32 Raw_CheckPosResponse_SID19(pu8 SubFunction,s32 responseArraySize=TpRxLength, u8* responseArray = TpRxData) 
{
    //strcpy(expectedResponse, ("59 " + dec2hex(SubFunction[0]) + " "+dec2hex(SubFunction[1])).c_str());
    strcpy(expectedResponse, ("59 " + dec2hex(SubFunction[0])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArraySize <= 0) {
        return -1;
    }
    if (responseArray[0] == 0x7F)
    {
        app.log_text("Unexpected Negative Response Received!", lvlError);
        strcpy(issuereason, "Unexpected Negative Response Received!");
        return -1;
    }

    if (responseArray[0] == 0x59 && responseArray[1] == SubFunction[0])
    {
        return 0;
    }
    else {
        app.log_text("Unexpected Negative Response Received!", lvlError);
        strcpy(issuereason, "Unexpected Negative Response Received!");
        return -1;
    }
    //处理所有子服务？？
    //switch (SubFunction[1])
    //{
    //    //读取客户端定义状态掩码匹配的DTC（Diagnostic Trouble Code）数量
    //case 0x01: {
    //    s32 dtc_number = (responseArray[4] << 8) + responseArray[5];
    //    //app.log_text(("读取到的DTC数量为" + std::to_string(dtc_number)).c_str(), Pass);
    //    strcpy(printChar, "");
    //    sprintf(printChar, "%s%d", "读取到的DTC数量为", dtc_number);
    //    TestReport_HTML_StepDescription(printChar);

    //    break; }
    //         //读取客户端定义状态掩码匹配的DTC及其掩码
    //case 0x02: {
    //    std::string dtc_hex = dealDTC(responseArray, responseArraySize);
    //    //app.log_text(("与状态码匹配的DTC为:" + dtc_hex).c_str(), Pass);
    //    strcpy(printChar, "");
    //    sprintf(printChar, "%s%s", "与状态码匹配的DTC为:0x", dtc_hex.c_str());
    //    TestReport_HTML_StepDescription(printChar);
    //    break;
    //}
    //         //该子功能请求来检索所有捕获的 DTC快照记录的ID信息(所以id支持的快照信息)
    //case 0x03: {
    //    std::string hex_DTC;
    //    for (int i = 2; i < responseArraySize; i += 4) {
    //        u32 evdtc = 0;
    //        for (int a = 0; a < 4; a++) {
    //            evdtc += responseArray[i + a];
    //            if (a < 3)
    //                evdtc <<= 8;
    //        }
    //        hex_DTC += intToHexStr(evdtc) + "/";
    //    }
    //    //TestReport_HTML_StepDescription(("DTC快照记录的信息" + hex_DTC).c_str(), Pass);
    //    strcpy(printChar, "");
    //    sprintf(printChar, "%s%s", "与状态码匹配的DTC为:0x", hex_DTC.c_str());
    //    TestReport_HTML_StepDescription(printChar);
    //}
    //         //检索客户端定义DTC掩码的记录数据(快照)如发生某一故障记录DTC时的车速，电源电压状态等
    //case 0x04: {
    //    TestReport_HTML_StepDescription(("DTC的状态掩码为:" + intToHexStr(responseArray[5], 2)).c_str(), Pass);
    //    //TestReport_HTML_StepDescription(("快照DID的个数"+std::to_string(responseArray[7])).c_str(), Pass);
    //    break;
    //}
    //         //检索所有DTC中是否有指定故障的DTC
    //case 0x05: {
    //    std::string hex_DTC;
    //    for (int i = 2; i < responseArraySize; i += 4) {
    //        u32 evdtc = 0;
    //        for (int a = 0; a < 4; a++) {
    //            evdtc += responseArray[i + a];
    //            if (a < 3)
    //                evdtc <<= 8;
    //        }
    //        std::string currentDTC = intToHexStr(evdtc);
    //        hex_DTC += currentDTC + "/";

    //        // 检查当前DTC是否为DC4588
    //        if (currentDTC == "DC4588") {
    //            return 0;
    //        }
    //        else {
    //            return -1;
    //        }
    //    }
    //    //TestReport_HTML_StepDescription(("DTC快照记录的信息" + hex_DTC).c_str(), Pass);
    //    strcpy(printChar, "");
    //    sprintf(printChar, "%s%s", "与状态码匹配的DTC为:0x", hex_DTC.c_str());
    //    TestReport_HTML_StepDescription(printChar);
    //}
    //         //读取某个DTC及其相关的环境数据
    //case 0x06: {
    //    TestReport_HTML_StepDescription(("DTC的状态掩码为:" + intToHexStr(responseArray[5], 2)).c_str(), Pass);
    //    //TestReport_HTML_StepDescription(("快照DID的个数" + std::to_string(responseArray[7])).c_str(), Pass);
    //    break;
    //}
    //         //获取所有DTC的状态位
    //case 0x0A: {
    //    std::string dtc_hex = dealDTC(responseArray, responseArraySize);
    //    //TestReport_HTML_StepDescription(("所有的DTC及其状态位为:" + dtc_hex).c_str(), Pass);
    //    strcpy(printChar, "");
    //    sprintf(printChar, "%s%s", "所有的DTC及其状态位为:", dtc_hex.c_str());
    //    TestReport_HTML_StepDescription(printChar);
    //    break;
    //}
    //case 0x15: {
    //    std::string dtc_hex = dealDTC(responseArray, responseArraySize);
    //    TestReport_HTML_StepDescription(("所有的DTC及其状态位为:" + dtc_hex).c_str(), Pass);
    //    break;
    //}
    //default:
    //    break;
    //}
    return 0;
}

s32 Raw_CheckPosResponse_DTC_SID19(pu8 SubFunction, int dtc ,s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("59 " + dec2hex(SubFunction[0]) + " " + dec2hex(SubFunction[1])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArraySize <= 0) {
        return -1;
    }
    if (responseArray[0] == 0x7F)
    {
        app.log_text("Unexpected Negative Response Received!", lvlError);
        strcpy(issuereason, "Unexpected Negative Response Received!");
        return -1;
    }

    std::string hex_DTC;
    int DTCexitFlag = 0;
    for (int i = 3; i < responseArraySize; i += 4) {
        u32 evdtc = 0;
        u8 Dstate = 0;
        for (int a = 0; a < 3; a++) {
            evdtc += responseArray[i + a];
            if (a < 2)
                evdtc <<= 8;
        }
        Dstate = responseArray[i + 3];
        std::string currentDTC = intToHexStr(evdtc);
        std::string currentstate = intToHexStr(Dstate);
        //hex_DTC += currentDTC + "/";

        // 检查当前DTC是否为High voltage DTC
        if (evdtc == dtc ) 
        {
            strcpy(printChar, "");
            sprintf(printChar, "Find DTC : %s with State:%s", currentDTC.c_str(), currentstate.c_str());
            TestReport_HTML_StepDescription(printChar);

            if ((Dstate & SubFunction[1]) > 0)
            {
                DTCexitFlag = 1;
                //TestReport_HTML_StepDescription("进入判断条件");
            }
        }
    }

    if (DTCexitFlag == 1)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

s32 CheckPosResponse_SID19(pu8 SubFunction)
{
    if (0 == Raw_CheckPosResponse_SID19(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

s32 CheckPosResponse_DTC_SID19(pu8 SubFunction, int dtc, u8 Astats)
{
    if (Astats == 0)
    {
        if (0 == Raw_CheckPosResponse_DTC_SID19(SubFunction, dtc))
        {
            TestReport_StepInfo(Fail);
            snprintf(StepDespriction, sizeof(StepDespriction), "Find DTC 0x%X in the response.", dtc);
            TestReport_HTML_StepDescription(StepDespriction, Fail);
            return -1;
        }
        else
        {
            TestReport_StepInfo(Pass);
            snprintf(StepDespriction, sizeof(StepDespriction), "No DTC 0x%X in the response.", dtc);
            TestReport_HTML_StepDescription(StepDespriction, Pass);
            return 0;
        }
    }
    else
    {
        if (0 == Raw_CheckPosResponse_DTC_SID19(SubFunction, dtc))
        {
            TestReport_StepInfo(Pass);
            snprintf(StepDespriction, sizeof(StepDespriction), "Find DTC 0x%X in the response.", dtc);
            TestReport_HTML_StepDescription(StepDespriction, Pass);
            return 0;
        }
        else
        {
            TestReport_StepInfo(Fail);
            snprintf(StepDespriction, sizeof(StepDespriction), "No DTC 0x%X in the response.", dtc);
            TestReport_HTML_StepDescription(StepDespriction, Fail);
            return -1;
        }
    }
}

/// <summary>
///  通过id控制输入输出
/// </summary>
/// <param name="SubFunction"></param> 2F子服务
/// <param name="subsize"></param> 子服务长度
/// <param name=""></param> 物理寻址以及功能寻址的判断
/// <param name="responseArray"></param> 返回内容的数组指针
/// <returns></returns> 返回的是读取回来的字节长度
s32 Raw_SID2F_IO_Control_byid(pu8 SubFunction, s32 subsize, enum AddressMode Mode = Physical, u8* responseArray = TpRxData) {
    u8 SID2F[1000] = {};
    s32 responseArraySize = 20;
    SID2F[0] = 0x2F;
    for (int i = 0; i < subsize; i++) {
        SID2F[i + 1] = SubFunction[i];
    }
    if (Mode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID2F, subsize + 1, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID2F, subsize + 1, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID2F_IO_Control_byid(pu8 SubFunction, s32 subsize, enum AddressMode Mode = Physical)
{
    TpRxLength = Raw_SID2F_IO_Control_byid(SubFunction, subsize, Mode);
    strcpy(requestInfo, "2F");
    for (int i = 0; i < subsize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }
    return 0;
}

/// <summary>
/// 检查2F服务读回来的内容
/// </summary>
/// <param name="SubFunction"></param> 2F子服务
/// <param name="subsize"></param> 子服务的长度
/// <param name="responseArraySize"></param> 读取回来的数组长度
/// <param name="responseArray"></param> 读取回来的数组
/// <returns></returns> 返回来的是检查的正确与否，正确为0，错误为-1
s32 Raw_CheckPosResponse_SID2F(pu8 SubFunction, s32 subsize, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData) 
{
    strcpy(expectedResponse, ("6F " + dec2hex(SubFunction[0]) + " " + dec2hex(SubFunction[1]) + " " + dec2hex(SubFunction[2])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }
    
    if (responseArraySize <= 0) {
        return -1;
    }
    if (responseArray[0] == 0x7F)
    {
        strcpy(issuereason, "Unexpected Negative Response Received!");
        app.log_text("Unexpected Negative Response Received!", lvlError);
        return -1;
    }
    for (int i = 0; i < subsize - 4; i++) {
        strcpy(issuereason, "Unexpected Negative Response Received!");
        TestReport_HTML_StepDescription(("CS" + std::to_string(i + 1) + " =" + intToHexStr(responseArray[i + 4])).c_str(), Pass);
    }
    return 0;
}
s32 CheckPosResponse_SID2F(pu8 SubFunction, s32 subsize)
{
    if (0 == Raw_CheckPosResponse_SID2F(SubFunction, subsize))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

s32 Raw_SID2A_useIDReadData( u8 SubFunction[], u32 SubSize, u8 AddressMode=0, u8* responseArray = TpRxData)
{
    u8 SID2A[100] = {};
    SID2A[0] = 0x2A;
    s32 responseArraySize = 100;
    for (int i = 0; i < SubSize; i++)
    {
        SID2A[i + 1] = SubFunction[i];
    }
    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID2A, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID2A, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID2A_useIDReadData(u8 SubFunction[], u32 SubSize, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID2A_useIDReadData(SubFunction, SubSize, AddressMode);
    strcpy(requestInfo, "2A");
    for (int i = 0; i < SubSize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }
    return 0;
}

/// <summary>
/// 检查2A服务读回来的内容
/// </summary>
/// <param name="SubFunction"></param> 2A子服务
/// <param name="subsize"></param> 子服务的长度
/// <param name="responseArraySize"></param> 读取回来的数组长度
/// <param name="responseArray"></param> 读取回来的数组
/// <returns></returns> 返回来的是检查的正确与否，正确为0，错误为-1
s32 Raw_CheckPosResponse_SID2A(u8 SubFunction[], s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("6A " + dec2hex(SubFunction[0]) + " " + dec2hex(SubFunction[1])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x7F)
    {
        strcpy(issuereason, "Unexpected Negative resposne received!");
        app.log_text("Unexpected negative response received!", lvlError);
        return -1;
    }
    if (responseArray[0] == 0x6A)
    {
        app.log_text("Positive Response Received!", lvlOK);
    }
    else
    {
        TestReport_HTML_StepDescription("Unexpected Response!", Fail);
        strcpy(issuereason, "Unexpected Response Received!");
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID2A(u8 SubFunction[])
{
    if (0 == Raw_CheckPosResponse_SID2A(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// Service0x2C 通过周期ID读取数据
/// </summary>
/// <param name="SubSize"></param> SubSize         $2C服务子功能长度
/// <param name="SubFunction"></param> SubFunction $2C服务子功能
/// <param name="AddressMode"></param> AddressMode $2C服务寻址方式
s32 Raw_SID2C_DynamicallyDefDataId(  u8 SubFunction[], u8 SubSize, u8 AddressMode=0, u8* responseArray = TpRxData)
{
    u8 SID2C[100] = {};
    SID2C[0] = 0x2C;
    for (int i = 0; i < SubSize; i++)
    {
        SID2C[i + 1] = SubFunction[i];
    }
    s32 responseArraySize = 100;
    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID2C, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID2C, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID2C_DynamicallyDefDataId(u8 SubFunction[], u8 SubSize, u8 AddressMode = 0)
{   
    TpRxLength = Raw_SID2C_DynamicallyDefDataId( SubFunction, SubSize);

    strcpy(requestInfo, "2C");
    for (int i = 0; i < SubSize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }
    return 0;
}

/// <summary>
/// 检查2C服务读回来的内容
/// </summary>
/// <param name="SubFunction"></param> 2C子服务
/// <param name="subsize"></param> 子服务的长度
/// <param name="responseArraySize"></param> 读取回来的数组长度
/// <param name="responseArray"></param> 读取回来的数组
/// <returns></returns> 返回来的是检查的正确与否，正确为0，错误为-1
s32 Raw_CheckPosResponse_SID2C(u8 SubFunction[], s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("6C " + dec2hex(SubFunction[0]) + " " + dec2hex(SubFunction[1])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x7F)
    {
        strcpy(issuereason, "Unexpected Negative Response Received!");
        app.log_text("Unexpected Negative Response Received!", lvlOK);
        return -1;
    }
    if (responseArray[0] == 0x6C && responseArray[1] == SubFunction[1])
    {
        app.log_text("Positive Response Received!", lvlOK);
    }
    else
    {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected Response!", lvlError);
        return -1;
    }
    return 0;
}
s32 CheckPosResponse_SID2C(u8 SubFunction[])
{
    if (0 == Raw_CheckPosResponse_SID2C(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// Service0x2E 按数据标识符写入数据
/// </summary>
/// <param name="SubSize"></param> SubSize         $2E服务子功能长度
/// <param name="SubFunction"></param> SubFunction $2E服务子功能
/// <param name="AddressMode"></param> AddressMode $2E服务寻址方式
s32 Raw_SID2E_WriteDataById(u8 SubFunction[], u8 SubSize, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID2E[100] = {};
    SID2E[0] = 0x2E;
    for (int i = 0; i < SubSize; i++)
    {
        SID2E[1 + i] = SubFunction[i];
    }
    s32 responseArraySize = 10;
    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID2E, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID2E, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID2E_WriteDataById(u8 SubFunction[], u8 SubSize, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID2E_WriteDataById(SubFunction, SubSize, AddressMode);
    strcpy(requestInfo, "2E");
    for (int i = 0; i < SubSize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }
    return 0;
}

/// <summary>
/// 检查2E服务读回来的内容
/// </summary>
/// <param name="SubFunction"></param> 2E子服务
/// <param name="subsize"></param> 子服务的长度
/// <param name="responseArraySize"></param> 读取回来的数组长度
/// <param name="responseArray"></param> 读取回来的数组
/// <returns></returns> 返回来的是检查的正确与否，正确为0，错误为-1
s32 Raw_CheckPosResponse_SID2E(u8 SubFunction[], s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("6E " + dec2hex(SubFunction[0]) + " " + dec2hex(SubFunction[1]) + " " + dec2hex(SubFunction[2])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }


    if (responseArray[0] == 0x7F)
    {
        strcpy(issuereason, "Unexpected Negative Response Received!");
        app.log_text("The NRC is not allowed when the UDS service is 0x2E!", lvlOK);
        return -1;
    }
    if (responseArray[0] == 0x6E && responseArray[1] == SubFunction[1] && responseArray[2] == SubFunction[2])
    {
        app.log_text("Positive Response Received!", lvlOK);
    }
    else
    {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected Response!", lvlError);
        return -1;
    }
    return 0;
}
s32 CheckPosResponse_SID2E(u8 SubFunction[])
{
    if (0 == Raw_CheckPosResponse_SID2E(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// Service0x3D 按地址写内存
/// </summary>
/// <param name="SubSize"></param> SubSize         $3D服务子功能长度
/// <param name="SubFunction"></param> SubFunction $3D服务子功能
/// <param name="AddressMode"></param> AddressMode $3D服务寻址方式
s32 Raw_SID3D_writeMemoryByAddress(u8 AddLenFID, u32 memoryAddress, u32 memorySize, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID3D[20] = {};
    SID3D[0] = 0x3D;
    SID3D[1] = AddLenFID;
    for (int i = 0; i < (AddLenFID & 0xf); i++)
        SID3D[2 + i] = memoryAddress >> (8 * ((AddLenFID & 0xf) - i - 1));
    for (int j = 0; j < (AddLenFID >> 4); j++)
        SID3D[2 + (AddLenFID & 0xf) + j] = memorySize >> (8 * ((AddLenFID >> 4) - j - 1));

    s32 responseArraySize = 200;

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID3D, 2 + (AddLenFID & 0xf) + (AddLenFID >> 4), responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID3D, 2 + (AddLenFID & 0xf) + (AddLenFID >> 4), responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }

    }
}
s32 SID3D_writeMemoryByAddress(u8 AddLenFID, u32 memoryAddress, u32 memorySize, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID3D_writeMemoryByAddress(AddLenFID, memoryAddress, memorySize, AddressMode);

    sprintf(requestInfo, "%s%x %s%x %s%x", "3D ", AddLenFID, "", memoryAddress, "", memorySize);
    return 0;
}

/// <summary>
/// 检查3D服务读回来的内容
/// </summary>
/// <param name="SubFunction"></param> 3D子服务
/// <param name="subsize"></param> 子服务的长度
/// <param name="responseArraySize"></param> 读取回来的数组长度
/// <param name="responseArray"></param> 读取回来的数组
/// <returns></returns> 返回来的是检查的正确与否，正确为0，错误为-1
s32 Raw_CheckPosResponse_SID3D(u8* responseArray = TpRxData, s64 responseArraySize = TpRxLength)
{
    strcpy(expectedResponse, "7D ");
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {

        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }
    if (responseArray[0] == 0x7D)
    {
        app.log_text("Positive Response Received!", lvlOK);
    }
    else
    {
        app.log_text("Unexpected Response!", lvlError);
        strcpy(issuereason, "Unexpected Response Received!");
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID3D()
{
    if (0 == Raw_CheckPosResponse_SID3D())
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// 34服务，请求下载
/// </summary>
/// <param name="AddressMode"></param>
/// <param name="SubSize"></param>
/// <param name="SubFunction"></param>
/// <param name="responseArray"></param>
/// <returns></returns>
s32 Raw_SID34_Request_Download(  u8 SubFunction[], u8 SubSize, u8 AddressMode=0, u8* responseArray = TpRxData)
{
    u8 SID34[100] = {};
    SID34[0] = 0x34;
    for (int i = 0; i < SubSize; i++)
    {
        SID34[1 + i] = SubFunction[i];
    }
    s32 responseArraySize = 10;
    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID34, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID34, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID34_Request_Download(u8 SubFunction[], u8 SubSize, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID34_Request_Download(SubFunction, SubSize, AddressMode);

    strcpy(requestInfo, "34");
    for (int i = 0; i < SubSize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }
    return 0;
}

/// <summary>
/// 检查34服务
/// </summary>
/// <param name="SubFunction"></param>
/// <param name="responseArraySize"></param>
/// <param name="responseArray"></param>
/// <returns></returns>
s32 Raw_CheckPosResponse_SID34(u8* SubFunction, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("74 " + dec2hex(SubFunction[0]) + " " + dec2hex(SubFunction[1]).c_str() + " " + dec2hex(SubFunction[2])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(issuereason, "No Response Received!");
        strcpy(actualResponse, "No Response Received!");
        return -1;
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x7F)
    {
        strcpy(issuereason, "Unexpected Negative Response Received!");
        app.log_text("Unexpected Negative Response Received!", lvlOK);
        return -1;
    }
    if (responseArray[0] == 0x74)
    {
        app.log_text("Positive Response Received!", lvlOK);

        reqSWDL_ID = responseArray[1];
        MaxLength = (responseArray[2] << 8) + responseArray[3];
        strcpy(printChar, "");
        sprintf(printChar, "%s%x", "MaxLength is 0x", MaxLength);
        TestReport_HTML_StepDescription(printChar);

        if (BusTypeNow == CAN) {
            if (MaxLength < 3 || MaxLength>4095) {
                strcpy(issuereason, "MaxNumberOfBlockLength is too small or too big");
                TestReport_HTML_StepDescription("ECU shall allow a minimum of\
one data byte or a maximum of 4095 data\
bytes to be transferred with a single\
transferData Service", Fail);
                return -1;
            }
            else if (MaxLength < 2048) {
                strcpy(issuereason, "MaxNumberOfBlockLength is set to a low amount");
                TestReport_HTML_StepDescription("MaxNumberOfBlockLength is set to a low amount", Fail);
                return -1;
            }
        }
        else if (BusTypeNow == CANFD) {
            if (MaxLength < 3 || MaxLength>16386) {
                strcpy(issuereason, "MaxNumberOfBlockLength is too small or too big");
                TestReport_HTML_StepDescription("ECU shall allow a minimum of\
one data byte or a maximum of 16386 data\
bytes to be transferred with a single\
transferData Service", Fail);
                return -1;
            }
            else if (MaxLength < 2048) {
                strcpy(issuereason, "MaxNumberOfBlockLength is set to a low amount");
                TestReport_HTML_StepDescription("MaxNumberOfBlockLength is set to a low amount", Fail);
                return -1;
            }
        }
        else if (BusTypeNow == Ethernet) {
            if (MaxLength < 6 || MaxLength>65535) {
                strcpy(issuereason, "MaxNumberOfBlockLength is too small or too big");
                TestReport_HTML_StepDescription("ECU shall allow a minimum of\
one data byte or a maximum of 64kb data\
bytes to be transferred with a single\
transferData Service", Fail);
                return -1;
            }
            else if (MaxLength < 16386) {
                strcpy(issuereason, "MaxNumberOfBlockLength is set to a low amount");
                TestReport_HTML_StepDescription("MaxNumberOfBlockLength is set to a low amount", Fail);
                return -1;
            }
        }
        else if (BusTypeNow == LIN) {
            if (MaxLength < 3 || MaxLength>2048) {
                strcpy(issuereason, "MaxNumberOfBlockLength is too small or too big");
                TestReport_HTML_StepDescription("ECU shall\
allow a minimum of one data byte or a\
maximum of 20 % of the total addressable\
programmable area in the LIN slave data\
bytes to be transferred with a single\
transferData Service", Fail);
                return -1;
            }
            else if (MaxLength > 2050) {
                strcpy(issuereason, "MaxNumberOfBlockLength is set to a high amount");
                TestReport_HTML_StepDescription("MaxNumberOfBlockLength is set to a high  amount", Fail);
                return -1;
            }
        }
    }
    else
    {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected Response!", lvlError);
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID34(u8 SubFunction[])
{
    if (0 == Raw_CheckPosResponse_SID34(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        resultFlag++;
        return -1;
    }
}

/// <summary>
/// 38服务，请求下载
/// </summary>
/// <param name="AddressMode"></param>
/// <param name="SubSize"></param>
/// <param name="SubFunction"></param>
/// <param name="responseArray"></param>
/// <returns></returns>
s32 Raw_SID38_Request_File_Transfer(u8 SubFunction[], u8 SubSize, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID38[100] = {};
    SID38[0] = 0x38;
    for (int i = 0; i < SubSize; i++)
    {
        SID38[1 + i] = SubFunction[i];
    }
    s32 responseArraySize = 100;
    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID38, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID38, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID38_Request_File_Transfer(u8 SubFunction[], u8 SubSize, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID38_Request_File_Transfer(SubFunction, SubSize, AddressMode);

    strcpy(requestInfo, "38");
    for (int i = 0; i < SubSize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }
    return 0;
}

/// <summary>
/// 检查34服务
/// </summary>
/// <param name="SubFunction"></param>
/// <param name="responseArraySize"></param>
/// <param name="responseArray"></param>
/// <returns></returns>
s32 Raw_CheckPosResponse_SID38(u8* SubFunction, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("78 " + dec2hex(SubFunction[0]) + " " + dec2hex(SubFunction[1]).c_str() + " " + dec2hex(SubFunction[2])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {

        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }

        reqSWDL_ID = responseArray[1];
        MaxLength = (responseArray[2] << 8) + responseArray[3];
        strcpy(printChar, "");
        sprintf(printChar, "%s%x", "maxlength is", MaxLength);
        TestReport_HTML_StepDescription(printChar);
    }

    if (responseArray[0] == 0x7F)
    {
        strcpy(issuereason, "Unexpected Negative Response Code Received!");
        app.log_text("Unexpected negative Response Code Received!", lvlOK);
        return -1;
    }
    if (responseArray[0] == 0x78)
    {
        app.log_text("Positive Response Received!", lvlOK);
    }
    else
    {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected Response!", lvlError);
        return -1;
    }
    
    strcpy(printChar, "");
    sprintf(printChar, "%s%x", "the length of maxNumberOfBlockLength is 0x", responseArray[2]);
    TestReport_HTML_StepDescription(printChar);
    strcpy(printChar, "");
    sprintf(printChar, "%s%x", "maxNumberOfBlockLength is 0x", ((responseArray[3] << 8) + responseArray[4]));
    TestReport_HTML_StepDescription(printChar);

    return 0;
}
s32 CheckPosResponse_SID38(u8 SubFunction[])
{
    if (0 == Raw_CheckPosResponse_SID38(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// 36服务，
/// </summary>
/// <param name="AddressMode"></param>
/// <param name="SubSize"></param>
/// <param name="SubFunction"></param>
/// <param name="responseArray"></param>
/// <returns></returns>
s32 Raw_SID35_Request_Upload(  u8 SubFunction[], u8 SubSize, u8 AddressMode=0, u8* responseArray = TpRxData)
{
    u8 SID35[100] = {};
    SID35[0] = 0x35;
    for (int i = 0; i < SubSize; i++)
    {
        SID35[1 + i] = SubFunction[i];
    }
    s32 responseArraySize = 100;
    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID35, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID35, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID35_Request_Upload(u8 SubFunction[], u8 SubSize, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID35_Request_Upload(SubFunction, SubSize, AddressMode);
    strcpy(requestInfo, "35");
    for (int i = 0; i < SubSize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }
    return 0;
}

/// <summary>
/// 检查36服务返回内容
/// </summary>
/// <param name="SubFunction"></param>
/// <param name="responseArraySize"></param>
/// <param name="responseArray"></param>
/// <returns></returns>
s32 Raw_CheckPosResponse_SID35(u8 SubFunction[], s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("75 " + dec2hex(SubFunction[0]) + " " + dec2hex(SubFunction[1])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {

        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x7F)
    {
        strcpy(issuereason, "Unexpected Negative Response Received!");
        app.log_text("Unexpected Negative Response Received!", lvlOK);
        return -1;
    }
    if (responseArray[0] == 0x75)
    {
        app.log_text("Positive Response Received!", lvlOK);
    }
    else
    {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected Response!", lvlError);
        return -1;
    }
    strcpy(printChar, "");
    sprintf(printChar, "%s%x", "the length of maxNumberOfBlockLength is 0x", responseArray[1]);
    TestReport_HTML_StepDescription(printChar);
    strcpy(printChar, "");
    sprintf(printChar, "%s%x", "maxNumberOfBlockLength is 0x", ((responseArray[2] << 8) + responseArray[3]));
    TestReport_HTML_StepDescription(printChar);

    return 0;
}
s32 CheckPosResponse_SID35(u8 SubFunction[])
{
    if (0 == Raw_CheckPosResponse_SID35(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/// <summary>
/// 36服务，client向server发送数据（download下载）或从server请求数据（upload上传）
/// </summary>
/// <param name="AddressMode"></param>
/// <param name="SubSize"></param>
/// <param name="SubFunction"></param>
/// <param name="responseArray"></param>
/// <returns></returns>
s32 Raw_SID36_Transfer_Data(u8 SequenceNumber, u8 SubFunction[], u64 SubSize, u8 AddressMode=0, u8* responseArray = TpRxData)
{
    u8 SID36[65535] = {};
    SID36[0] = 0x36;
    SID36[1] = SequenceNumber;
    for (int i = 0; i < SubSize; i++)
    {
        SID36[2 + i] = SubFunction[i];
    }
    s32 responseArraySize = 100;
    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID36, SubSize + 2, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID36, SubSize + 2, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID36_Transfer_Data(u8 SequenceNumber, u8 SubFunction[], u64 SubSize, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID36_Transfer_Data(SequenceNumber, SubFunction, SubSize, AddressMode);
    strcpy(requestInfo, "36 ");
    strcat(requestInfo, dec2hex(SequenceNumber).c_str());
    for (int i = 0; i < SubSize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }
    return 0;
}

/// <summary>
/// 检查36服务
/// </summary>
/// <param name="SubFunction"></param>
/// <param name="responseArraySize"></param>
/// <param name="responseArray"></param>
/// <returns></returns>
s32 Raw_CheckPosResponse_SID36(u8 SubFunction, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("76 " + dec2hex(SubFunction)).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
        return -1;
    }
    else
    {

        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }
    if (responseArray[0] == 0x7F)
    {
        strcpy(issuereason, "Unexpected Negative Response Received!");
        app.log_text("Unexpected Negative Response Received!", lvlOK);
        return -1;
    }
    if (responseArray[0] == 0x76)
    {
        app.log_text("Positive Response Received!", lvlOK);
    }
    else
    {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected Response!", lvlError);
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID36(u8 SubFunction)
{
    if (0 == Raw_CheckPosResponse_SID36(SubFunction))
    {
        //TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        //TestReport_StepInfo(Fail);
        return -1;
    }
}

s32 Raw_SID37_RequestTransferExit(u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID37[1] = { 0x37 };
    s32 responseArraySize = 100;
    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID37, 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID37, 1, responseArray, &responseArraySize) == 0x00)
        {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID37_RequestTransferExit(u8 AddressMode = 0)
{
    TpRxLength = Raw_SID37_RequestTransferExit(AddressMode);
    strcpy(requestInfo, "37");
    return 0;
}

s32 Raw_CheckPosResponse_SID37(s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, "77");
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
        strcpy(issuereason, "No Response Received!");
    }
    else
    {

        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArray[0] == 0x7F)
    {
        strcpy(issuereason, "Unexpected Negative Response Received!");
        app.log_text("ECU give negative response!", lvlError);
        return -1;
    }
    if (responseArray[0] == 0x77)
    {
        app.log_text("Positive Response Received!", lvlOK);
    }
    else
    {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected Response!", lvlError);
        return -1;
    }

    return 0;
}
s32 CheckPosResponse_SID37()
{
    if (0 == Raw_CheckPosResponse_SID37())
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 RX_SID31_RoutineControl(pu8 SubFunction, u8 subsize, AddressMode type, u8* responseArray = TpRxData) {

    u8 RID[2] = { SubFunction[1],SubFunction[2] };
    u8 OptionRecord[65535] = {};
    for (int i = 0; i < subsize - 3; i++) {
        OptionRecord[i] = SubFunction[i + 3];
    }

    u16 Routine = (RID[0] << 8) + RID[1];

    return  SID31_RoutineControl(SubFunction[0], Routine,  subsize - 3, OptionRecord, type);

}
s32 RX_CheckPosResponse_SID31(pu8 SubFunction, u8 subsize, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData) {

    u8 RID[2] = { SubFunction[1],SubFunction[2] };
    u8 OptionRecord[65535] = {};
    for (int i = 0; i < subsize - 3; i++) {
        OptionRecord[i] = SubFunction[i + 3];
    }
    u16 Routine = (RID[0] << 8) + RID[1];
    return CheckPosResponse_SID31(SubFunction[0], Routine,subsize - 3, OptionRecord);
};

//19 02
s32 RX_SID19_ReportDTCByStatusMask(u8 Mask, AddressMode Mode = Physical) {
    u8 sub1902[2] = {};
    sub1902[0] = 0x02;
    sub1902[1] = Mask;
    return SID19_ReadDTC(sub1902, 2, Physical);
}
s32 RX_CheckPosResponse_SID19_02(u8 Mask, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData) {
    u8 sub1902[2] = {};
    sub1902[0] = 0x02;
    sub1902[1] = Mask;
    return  CheckPosResponse_SID19(sub1902);
}
s32 RX_CheckResponse_DTC_SID19_02(u8 Mask, int dtc, u8 Astats = 0) {
    u8 sub1902[2] = {};
    sub1902[0] = 0x02;
    sub1902[1] = Mask;
    return  CheckPosResponse_DTC_SID19(sub1902, dtc, Astats);
}


//19 03
s32 RX_SID19_ReportDTCSnapshotIdentification(AddressMode Mode = Physical) {
    u8 sub1903[2] = {};
    sub1903[0] = 0x03;
    return SID19_ReadDTC(sub1903, 1, Mode);
}
s32 RX_CheckPosResponse_SID19_03(s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData) {
    u8 sub1903[2] = {};
    sub1903[0] = 0x03;
    return CheckPosResponse_SID19(sub1903);
}

s32 Raw_RX_SID19_CheckSnapshotIdentification(pu8 SubFunction, int dtc, u8 snapshotRecordNum, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("59 " + dec2hex(SubFunction[0])).c_str());
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
    }
    else
    {
        for (int i = 0; i < responseArraySize; i++)
        {
            if (i == 0)
                strcpy(actualResponse, ("" + dec2hex(responseArray[i])).c_str());
            else
                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
        }
    }

    if (responseArraySize <= 0) {
        return -5;
    }
    if (responseArray[0] == 0x7F)
    {
        app.log_text("Unexpected Negative Response Received!", lvlError);
        strcpy(issuereason, "Unexpected Negative Response Received!");
        return -4;
    }

    if (responseArray[0] != 0x59)
    {
        return -3;
    }
    if (responseArraySize < 3)
    {
        TestReport_HTML_StepDescription("No DTC in response");
        return -2;
    }

    std::string hex_DTC;
    int DTCexitFlag = 0;
    for (int i = 2; i < responseArraySize; i += 4) {
        u32 evdtc = 0;
        u8 Dstate = 0;
        for (int a = 0; a < 3; a++) {
            evdtc += responseArray[i + a];
            if (a < 2)
                evdtc <<= 8;
        }
        Dstate = responseArray[i + 3];
        std::string currentDTC = intToHexStr(evdtc);
        std::string currentstate = intToHexStr(Dstate);
        //hex_DTC += currentDTC + "/";

        if (evdtc == dtc)
        {
            if (Dstate == snapshotRecordNum)
                return 1;
            else
                return 0;
        }
        else if (i + 4 >= responseArraySize)
            return -1;
    }
}

s32 RX_SID19_CheckSnapshotIdentification(pu8 SubFunction, int dtc, u8 snapshotRecordNum,u8 state = 1)
{
    s8 returnCode = 99;
    returnCode = Raw_RX_SID19_CheckSnapshotIdentification(SubFunction, dtc, snapshotRecordNum);

    if (0 <= returnCode)
    {
        snprintf(StepDespriction, sizeof(StepDespriction), "Find DTC 0x%X With State %0xX in the response.", dtc, snapshotRecordNum);
        if(state == 1)
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        else
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 1;
    }
    else if(0 == returnCode)
    {
        snprintf(StepDespriction, sizeof(StepDespriction), "Find DTC 0x%X, State is Not %0xX in the response.", dtc, snapshotRecordNum);
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }
    else if(returnCode == -1 || returnCode == -2){
        snprintf(StepDespriction, sizeof(StepDespriction), "No DTC 0x%X in the response.", dtc, snapshotRecordNum);
        if(state == 0)
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        else
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        return -1;
    }
    else {
        TestReport_HTML_StepDescription("Unexpected Response!", Fail);
        return -2;
    }

}

//19 04 
s32 RX_SID19_ReportDTCSnapshotRecordByDTCNumber(pu8 DTC, u8 DTCSnapshotRecordNumber, AddressMode Mode = Physical) {

    u8 sub1904[5] = {};
    sub1904[0] = 0x04;
    sub1904[1] = DTC[0];
    sub1904[2] = DTC[1];
    sub1904[3] = DTC[2];
    sub1904[4] = DTCSnapshotRecordNumber;

    return SID19_ReadDTC(sub1904, 5, Mode);
}
s32 RX_CheckPosResponse_SID19_04(pu8 DTC, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData) {
    u8 sub1904[5] = {};
    sub1904[0] = 0x04;
    sub1904[1] = DTC[0];
    sub1904[2] = DTC[1];
    sub1904[3] = DTC[2];
    return CheckPosResponse_SID19(sub1904);

}

//19 06
s32 RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(pu8 DTC, u8 DTCSnapshotRecordNumber, AddressMode Mode = Physical) {
    u8 sub1906[5] = {};
    sub1906[0] = 0x06;
    sub1906[1] = DTC[0];
    sub1906[2] = DTC[1];
    sub1906[3] = DTC[2];
    sub1906[4] = DTCSnapshotRecordNumber;
    
    return SID19_ReadDTC(sub1906, 5, Mode);
}
s32 Rx_CheckPosResponse_SID19_06(pu8 DTC, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData) {
    u8 sub1906[5] = {};
    sub1906[0] = 0x06;
    sub1906[1] = DTC[0];
    sub1906[2] = DTC[1];
    sub1906[3] = DTC[2];
    return CheckPosResponse_SID19(sub1906);
}

//19 0A
s32 RX_SID19_ReportSupportDTC(AddressMode Mode = Physical) {
    u8 sub190A[4] = {};
    sub190A[0] = 0x0A;
    
    return SID19_ReadDTC(sub190A, 1, Mode);
}
s32 RX_CheckPosResponse_SID19_0A(s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData) {
    u8 sub190A[4] = {};
    sub190A[0] = 0x0A;
    return CheckPosResponse_SID19(sub190A);
}
//19 15
s32 RX_ReportDTCWithPermanentStatus(AddressMode Mode = Physical) {
    u8 sub1915[4] = {};
    sub1915[0] = 0x15;
    
    return SID19_ReadDTC(sub1915, 1, Mode);
}
s32 RX_CheckPosResponse_SID19_15(s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData) {
    u8 sub1915[4] = {};
    sub1915[0] = 0x15;
    return CheckPosResponse_SID19(sub1915);
}

s32 SIDAF_ReportGenericSnapshotByDTCNumber(pu8 DTC, u8 DTCSnapshotRecordNumber, AddressMode Mode = Physical) {
    u8 SIDAF[6] = {};
    SIDAF[0] = 0xAF;
    SIDAF[1] = 0x06;
    SIDAF[2] = DTC[0];
    SIDAF[3] = DTC[1];
    SIDAF[4] = DTC[2];
    SIDAF[5] = DTCSnapshotRecordNumber;
    return diag_serve_sent(SIDAF, 6, Mode);
}

s32 CheckPosResponse_SIDAF_04(pu8 DTC, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData) {
    if (responseArray[0] == 0xEF)
    {

    }
    else {
        return -1;
    }
    if (responseArray[1] == 0x04)
    {

    }
    else return -1;
    if ((DTC[0] == 0xFF) && (DTC[1] == 0xFF) && (DTC[2] == 0xFF))
    {
        return 0;
    }
    if ((DTC[0] == responseArray[2]) && (DTC[1] == responseArray[3]) && (DTC[2] == responseArray[4]))
    {
        TestReport_HTML_StepDescription("DTC IS RIGHT", Pass);
    }
    else
    {

    }
    return 0;
}

s32 SIDAF_ReportGenericExtendedDataByDTCNumber(pu8 DTC, u8 DTCSnapshotRecordNumber, AddressMode Mode = Physical) {
    u8 SIDAF[6] = {};
    SIDAF[0] = 0xAF;
    SIDAF[1] = 0x06;
    SIDAF[2] = DTC[0];
    SIDAF[3] = DTC[1];
    SIDAF[4] = DTC[2];
    SIDAF[5] = DTCSnapshotRecordNumber;
    return diag_serve_sent(SIDAF, 6, Mode);

}

s32 CheckPosResponse_SIDAF_06(pu8 DTC, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData) {
    if (responseArray[0] == 0xEF)
    {

    }
    else {
        return -1;
    }
    if (responseArray[1] == 0x06)
    {

    }
    else return -1;
    if ((DTC[0] == 0xFF) && (DTC[1] == 0xFF) && (DTC[2] = 0xFF))
    {
        return 0;
    }
    if ((DTC[0] == responseArray[2]) && (DTC[1] == responseArray[3]) && (DTC[2] == responseArray[4]))
    {
        TestReport_HTML_StepDescription("DTC IS RIGHT", Pass);
    }
    else
    {

    }
    return 0;
}


/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//获取当前时间戳，单位是ms
s64 time_now() {
    s64 T1;
    if (0 == app.get_timestamp(&T1)) {
    }
    return T1 / 1000;
}

//存储诊断信息
s32 StoreArray(u8* DestinationArray, u8* SourceArray, u32 Length = 3, u32 SourceOffset = 0, u32 DestiOffset = 0)
{
    for (int i = 0; i < Length; i++)
    {
        DestinationArray[i+DestiOffset] = SourceArray[i + SourceOffset];
    }
    return 0;
}

s32 CheckCCR()
{
    u16 CompleteAndCompatible = 0x0205;
    u8 CCR[5] = { 0x10,0x00,0x00,0x00,0x00 };
    if (strcmp(VbfFileInfo.sw_part_type, "EXE") == 0)
    {
        CCR[0] = 0x10;
        CCR[1] = 0x00;
        CCR[2] = 0x00;
        CCR[3] = 0x00;
        CCR[4] = 0x05;

        SID31_RoutineControl(0x01, CompleteAndCompatible);
        if (0 == CheckPosResponse_SID31(0x01, CompleteAndCompatible, 5, CCR))
        {
            return 0;
        }
        else
            return -1;
    }
    else if (strcmp(VbfFileInfo.sw_part_type, "SIGCFG") == 0)
    {
        CCR[0] = 0x10;
        CCR[1] = 0x00;
        CCR[2] = 0x00;
        CCR[3] = 0x00;
        CCR[4] = 0x09;

        SID31_RoutineControl(0x01, CompleteAndCompatible);
        if (0 == CheckPosResponse_SID31(0x01, CompleteAndCompatible, 5, CCR))
        {
            return 0;
        }
        else
            return -1;
    }
    else if (strcmp(VbfFileInfo.sw_part_type, "CARCONFIG") == 0)
    {
        CCR[0] = 0x10;
        CCR[1] = 0x00;
        CCR[2] = 0x00;
        CCR[3] = 0x00;
        CCR[4] = 0x11;

        SID31_RoutineControl(0x01, CompleteAndCompatible);
        if (0 == CheckPosResponse_SID31(0x01, CompleteAndCompatible, 5, CCR))
        {
            return 0;
        }
        else
            return -1;
    }
    else if (strcmp(VbfFileInfo.sw_part_type, "DATA") == 0)
    {

        SID31_RoutineControl(0x01, CompleteAndCompatible);
        if (0 == CheckPosResponse_SID31_forCCR())
        {
            return 0;
        }
        else
            return -1;
    }

}

s32 EraseAddress(u8 ifaboart = 0) {
    u8 opt[8] = {};
    u8 rec[2] = { 0x10,0x00 };
    for(int i=0;i<VbfFileInfo.DataBlockNumber;i++)
    {
        opt[0] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 24;
        opt[1] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 16;
        opt[2] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 8;
        opt[3] = VbfFileInfo.VbfDataBlcok[i].StartAddress;
        opt[4] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 24;
        opt[5] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 16;
        opt[6] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 8;
        opt[7] = VbfFileInfo.VbfDataBlcok[i].DataLength;
        
        SID31_RoutineControl(0x01, 0xFF00, 8, opt);
        CheckPosResponse_SID31(0x01, 0xFF00, 2, rec);

        if (ifaboart == i)
            return 0;
    }

    u8 DID[2] = { 0xF1,0xAA };
    SID22_ReadDataByIdentifier(DID);
    CheckPosResponse_SID22(DID, 8);

    return 0;
}

s32 EraseAddress_DisconnectPowerSupply(u8 i = 0, u8 different = 0)
{
    u8 opt[8] = {};
    u8 rec[2] = { 0x10,0x00 };

    opt[0] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 24;
    opt[1] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 16;
    opt[2] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 8;
    opt[3] = VbfFileInfo.VbfDataBlcok[i].StartAddress;
    opt[4] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 24;
    opt[5] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 16;
    opt[6] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 8;
    opt[7] = VbfFileInfo.VbfDataBlcok[i].DataLength;

    if(different==0)
        SID31_RoutineControl_EraseMemory(opt,8);
    else
    {
        SID31_RoutineControl(0x01, 0xFF00, 8, opt);
        CheckPosResponse_SID31(0x01, 0xFF00, 2, rec);
    }
    return 0;
}

s32 FileDownload(u8 Dupli36=0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};

    for(int i=0;i<VbfFileInfo.DataBlockNumber;i++)
    {
        sequenceNum = 1;

        subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[1] = 0x44;
        subfunc[2] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 24;
        subfunc[3] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 16;
        subfunc[4] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 8;
        subfunc[5] = VbfFileInfo.VbfDataBlcok[i].StartAddress;
        subfunc[6] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 24;
        subfunc[7] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 16;
        subfunc[8] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 8;
        subfunc[9] = VbfFileInfo.VbfDataBlcok[i].DataLength;
        SID34_Request_Download(subfunc, 10);
        if(0!=CheckPosResponse_SID34(subfunc))
            return -1;

        u64 MaxDataLength = MaxLength - 2;
        u64 remainLength = VbfFileInfo.VbfDataBlcok[i].DataLength;

        for (u64 offset=0; offset <= remainLength; offset+=MaxDataLength)
        {
            for (int j = 0; j < MaxDataLength; j++)
            {
                transferData[j] = VbfFileInfo.VbfDataBlcok[i].BlockData[j + offset];
            }

            if (remainLength - offset >= MaxDataLength)
            {

              
                SID36_Transfer_Data(sequenceNum, transferData, MaxDataLength);
                CheckPosResponse_SID36(sequenceNum);
            }
                    
            else
            {


                if (Dupli36 == 1)
                {
                    if (remainLength - offset == 1)
                    {
                        SID36_Transfer_Data(sequenceNum, transferData, 0);
                    }
                    else if(remainLength - offset <=7)
                    {
                        SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset - 1);
                    }
                    else if ((remainLength - offset - 6) % 7 == 0)
                    {
                        SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset - 7);
                    }
                    else
                    {
                        SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset - (remainLength - offset - 6) % 7);
                    }
                    CheckNonResponse();

                    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                    CheckPosResponse_SID36(sequenceNum);

                    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                    CheckPosResponse_SID36(sequenceNum);
                }
                else
                {
                    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                    CheckPosResponse_SID36(sequenceNum);
                }
            }
            sequenceNum++;
        }

        SID37_RequestTransferExit();
        CheckPosResponse_SID37();
    }

    return 0;
}

s32 FileDownload_with_decreased_length()
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};

    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        sequenceNum = 1;

        subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[1] = 0x44;
        subfunc[2] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 24;
        subfunc[3] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 16;
        subfunc[4] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 8;
        subfunc[5] = VbfFileInfo.VbfDataBlcok[i].StartAddress;
        subfunc[6] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 24;
        subfunc[7] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 16;
        subfunc[8] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 8;
        subfunc[9] = VbfFileInfo.VbfDataBlcok[i].DataLength;
        SID34_Request_Download(subfunc, 10);
        if (0 != CheckPosResponse_SID34(subfunc))
            return -1;

        u64 MaxDataLength = 3840;
        u64 remainLength = VbfFileInfo.VbfDataBlcok[i].DataLength;

        for (u64 offset = 0; offset <= remainLength; offset += MaxDataLength)
        {
            for (int j = 0; j < MaxDataLength; j++)
            {
                transferData[j] = VbfFileInfo.VbfDataBlcok[i].BlockData[j + offset];
            }

            if (remainLength - offset >= MaxDataLength)
            {
                SID36_Transfer_Data(sequenceNum, transferData, MaxDataLength);
                CheckPosResponse_SID36(sequenceNum);
            }

            else
            {
                SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                CheckPosResponse_SID36(sequenceNum);
            }
            sequenceNum++;
        }

        SID37_RequestTransferExit();
        CheckPosResponse_SID37();
    }

    return 0;
}

s32 FileDownload_with_Incorrect_Data()
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};

    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        sequenceNum = 1;

        subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[1] = 0x44;
        subfunc[2] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 24;
        subfunc[3] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 16;
        subfunc[4] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 8;
        subfunc[5] = VbfFileInfo.VbfDataBlcok[i].StartAddress;
        subfunc[6] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 24;
        subfunc[7] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 16;
        subfunc[8] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 8;
        subfunc[9] = VbfFileInfo.VbfDataBlcok[i].DataLength;
        SID34_Request_Download(subfunc, 10);
        if (0 != CheckPosResponse_SID34(subfunc))
            return -1;

        u64 MaxDataLength = MaxLength - 2;
        u64 remainLength = VbfFileInfo.VbfDataBlcok[i].DataLength;

        for (u64 offset = 0; offset <= remainLength; offset += MaxDataLength)
        {
            for (int j = 0; j < MaxDataLength; j++)
            {
                transferData[j] = VbfFileInfo.VbfDataBlcok[i].BlockData[j + offset] + 1;
            }

            if (remainLength - offset >= MaxDataLength)
            {
                SID36_Transfer_Data(sequenceNum, transferData, MaxDataLength);
                CheckPosResponse_SID36(sequenceNum);
            }

            else
            {
                SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                CheckPosResponse_SID36(sequenceNum);
            }
            sequenceNum++;
        }

        SID37_RequestTransferExit();
        CheckPosResponse_SID37();
    }

    return 0;
}

s32 FileDownload_with_checkccr()
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};

    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        sequenceNum = 1;

        subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[1] = 0x44;
        subfunc[2] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 24;
        subfunc[3] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 16;
        subfunc[4] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 8;
        subfunc[5] = VbfFileInfo.VbfDataBlcok[i].StartAddress;
        subfunc[6] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 24;
        subfunc[7] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 16;
        subfunc[8] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 8;
        subfunc[9] = VbfFileInfo.VbfDataBlcok[i].DataLength;
        SID34_Request_Download(subfunc, 10);
        if (0 != CheckPosResponse_SID34(subfunc))
            return -1;

        u64 MaxDataLength = MaxLength - 2;
        u64 remainLength = VbfFileInfo.VbfDataBlcok[i].DataLength;

        for (u64 offset = 0; offset <= remainLength; offset += MaxDataLength)
        {
            for (int j = 0; j < MaxDataLength; j++)
            {
                transferData[j] = VbfFileInfo.VbfDataBlcok[i].BlockData[j + offset];
            }

            if (remainLength - offset >= MaxDataLength)
            {
                SID36_Transfer_Data(sequenceNum, transferData, MaxDataLength);
                CheckPosResponse_SID36(sequenceNum);
            }

            else
            {
                SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                CheckPosResponse_SID36(sequenceNum);
            }
            sequenceNum++;

            CheckCCR();
        }

        SID37_RequestTransferExit();
        CheckPosResponse_SID37();
    }

    return 0;
}


// 0 = No 1 = Min 2 = Max
s32 FileDownload_Abort(u8 AbortAt = 0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;


    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[1] = 0x44;
        subfunc[2] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 24;
        subfunc[3] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 16;
        subfunc[4] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 8;
        subfunc[5] = VbfFileInfo.VbfDataBlcok[i].StartAddress;
        subfunc[6] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 24;
        subfunc[7] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 16;
        subfunc[8] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 8;
        subfunc[9] = VbfFileInfo.VbfDataBlcok[i].DataLength;
        SID34_Request_Download(subfunc, 10);
        if (0 != CheckPosResponse_SID34(subfunc))
            return -1;

        u8* transferData = (u8*)malloc(MaxLength);
        u32 MaxDataLength = MaxLength - 2;
        u32 remainLength = VbfFileInfo.VbfDataBlcok[i].DataLength;

        for (u32 offset = 0; offset <= remainLength; offset += MaxDataLength)
        {
            if (remainLength - offset > MaxDataLength)
            {
                for (int j = 0; j < MaxDataLength; j++)
                {
                    transferData[j + 2] = VbfFileInfo.VbfDataBlcok[i].BlockData[j + offset];
                }
                SID36_Transfer_Data(sequenceNum, transferData, MaxLength);
                CheckPosResponse_SID36(sequenceNum);
            }

            else
            {
                for (int j = 0; j < remainLength - offset; j++)
                {
                    transferData[j + 2] = VbfFileInfo.VbfDataBlcok[i].BlockData[j + offset];
                }

                SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                CheckPosResponse_SID36(sequenceNum);

                if (AbortAt == 2)
                    return -1;
            }

            if (AbortAt == 1 && sequenceNum == 1)
                return -1;

            sequenceNum++;
        }

        SID37_RequestTransferExit();
        CheckPosResponse_SID37();
    }

    return 0;
}



s32 ActivateSBL()
{
    u16 ActivateSBL =0x0301 ;
    u8 SBLAddress[4] = {};
    u8 ExpectedResponse[2] = { 0x10, 0x01};

    char* pEnd;
    u32 CallAddress = strtol(VbfFileInfo.call, &pEnd, 16);

    SBLAddress[0] = CallAddress >> 24;
    SBLAddress[1] = CallAddress >> 16;
    SBLAddress[2] = CallAddress >> 8;
    SBLAddress[3] = CallAddress;

    SID31_RoutineControl(0x01, ActivateSBL, 4, SBLAddress);
    CheckPosResponse_SID31(0x01, ActivateSBL, 2, ExpectedResponse);
    return 0;
}

s32 CheckSignature()
{
    u16 FileSignature = 0x0212;
    
    u8 ExpectedResponse[2] = { 0x10,0x00 };

    SID31_RoutineControl(0x01, FileSignature, VbfFileInfo.sw_signature_dev.count, VbfFileInfo.sw_signature_dev.array);
    CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse);

    return 0;
}

s32 CheckSignatureInvalid()
{
    u16 FileSignature = 0x0212;

    u8 ExpectedResponse[2] = { 0x10,0x01 };

    u8 invalidSig[512] = {};

    for (int i = 0; i < VbfFileInfo.sw_signature_dev.count; i++)
    {
        invalidSig[i] = VbfFileInfo.sw_signature_dev.array[i] + 1;
    }

    SID31_RoutineControl(0x01, FileSignature, VbfFileInfo.sw_signature_dev.count, invalidSig);
    CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse);

    return 0;
}

s32 CheckCompleteAndCompatible()
{
    u16 CompleteAndCompatible =0x0205 ;
    u8 ExpectedResponse[5] = { 0x10,0x00,0x00,0x00,0x00 };

    SID31_RoutineControl(0x01, CompleteAndCompatible);
    if (0 == CheckPosResponse_SID31(0x01, CompleteAndCompatible, 5, ExpectedResponse))
    {
        return 0;
    }
    else
        return -1;
}



s32 ReadWriteDID(u16 rDID, u32 Length)
{
    u8 DID[2] = {};
    DID[0] = rDID >> 8;
    DID[1] = rDID & 0xff;

    u8* WriteData = (u8*)malloc(Length + 3);
    u8 nrc[2] = { 0x31,0x11 };

    WriteData[0] = DID[0]; WriteData[1] = DID[1];

    SID22_ReadDataByIdentifier(DID);
    if (0 == CheckPosResponse_SID22(DID, Length))
    {
        for (int i = 0; i < Length; i++)
        {
            WriteData[i + 2] = TpRxData[i + 3];
        }

        SID2E_WriteDataById(WriteData, Length+2);
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            WriteData[i + 2] = i + 1;
        }

        SID2E_WriteDataById(WriteData, Length+2);
    }
    if (CheckNegResponse(nrc) == 0)
        return 0;
    else
        return -1;
}

s32 ReadWriteDID_Func(u16 rDID, u32 Length)
{
    u8 DID[2] = {};
    DID[0] = rDID >> 8;
    DID[1] = rDID & 0xff;

    u8* WriteData = (u8*)malloc(Length + 3);
    u8 nrc[2] = { 0x31,0x11 };

    WriteData[0] = DID[0]; WriteData[1] = DID[1];

    SID22_ReadDataByIdentifier(DID);
    if (0 == CheckPosResponse_SID22(DID, Length))
    {
        for (int i = 0; i < Length; i++)
        {
            WriteData[i + 2] = TpRxData[i + 3];
        }

        SID2E_WriteDataById(WriteData, Length + 2);
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            WriteData[i + 2] = i + 1;
        }

        SID2E_WriteDataById(WriteData, Length + 2);
    }
    if (0 == CheckNonResponse())
        return 0;
    else
        return -1;
}

s32 CheckProgrammingPrecondition()
{
    u16 ProPrecondition= 0x0206 ;
    u8 ExpectedResponse[2] = { 0x10,01 };

    SID31_RoutineControl(0x01, ProPrecondition);
    CheckPosResponse_SID31(0x01, ProPrecondition, 2, ExpectedResponse);
    return 0;
}

s32 ReadPublicKey()
{
    u8 D01C[2] = { 0XD0,0X1C };
    //如果文件包含公钥
    SID22_ReadDataByIdentifier(D01C);

    //如果未写入公钥
    if (0 == CheckNegResponse(0x22)) {
        
    }
    else if (TpRxData[2] = 0x62) {
        TestReport_HTML_StepDescription("ECU已写入公钥！");
    }

    return 0;
}

void GenSAKeyEx(u8 SeLevel)
{
     char DLLName[100] = "\\GenerateKeyExOpt.dll";
     unsigned int ActualSize = 1;
     u8 ChallengeBytes[32] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };

     // 运行时加载DLL库
     HMODULE DLLhandle = LoadLibraryA(DLLName);     // 根据DLL文件名，加载DLL，返回一个模块句柄
     if (DLLhandle == NULL)
     {
         TestReport_HTML_StepDescription("加载DLL动态库失败\n", Fail);
         return;
     }

     typedef int (*GenerateKeyEx)
         (
             const unsigned char* iSeedArray,     /* Array for the seed [in] */
             unsigned int			iSeedArraySize, /* Length of the array for the seed [in] */
             const unsigned int		iSecurityLevel, /* Security level [in] */
             const char* iVariant,       /* Name of the active variant [in] */
             unsigned char* ioKeyArray,     /* Array for the key [in, out] */
             unsigned int			iKeyArraySize,  /* Maximum length of the array for the key [in] */
             unsigned int& oSize           /* Length of the key [out] */
             );												// 定义函数指针类型
     GenerateKeyEx GenKey;

     GenKey = (GenerateKeyEx)GetProcAddress(DLLhandle, "GenerateKeyEx");     // GetProcAddress返回指向的函数名的函数地址

     u8 pincode0x05[16] = {0X2B,0X7E,0X15,0X16,0X28,0XAE,0XD2,0XA6,0XAB,0XF7,0X15,0X88,0X09,0XCF,0X4F,0X3C};

     if (SeLevel == 0x05)
     {
         for (int i = 0; i < 16; i++)
         {
             ChallengeBytes[i + 16] = pincode0x05[i];
         }
     }

     for (int i = 0; i < 16; i++)
     {
         ChallengeBytes[i] = SeedArray[i];
     }

     GenKey(ChallengeBytes, 16, SeLevel, "", KeyArray, 16, ActualSize);

     FreeLibrary(DLLhandle);
}

//void GenSAKeyExOpt(u8 SeLevel)
//{
//    char* DLLName = NULL;
//    unsigned int ActualSize = 1;
//    u8 ChallengeBytes[32] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
//
//    app.get_configuration_file_path(&DLLName);
//    strcat(DLLName, "\\GenerateKeyExOpt.dll");
//
//    // 运行时加载DLL库
//    HMODULE DLLhandle = LoadLibraryA(DLLName);     // 根据DLL文件名，加载DLL，返回一个模块句柄
//    if (DLLhandle == NULL)
//    {
//        TestReport_HTML_StepDescription("加载DLL动态库失败\n", Fail);
//        return;
//    }
//
//    typedef int (*GenerateKeyExOpt)
//        (
//            const unsigned char* iSeedArray,     /* Array for the seed [in] */
//            unsigned int			iSeedArraySize, /* Length of the array for the seed [in] */
//            const unsigned int		iSecurityLevel, /* Security level [in] */
//            const char* iVariant,       /* Name of the active variant [in] */
//            const char* ipOptions,              // Optional parameter which might be used for OEM specific information [in]
//            unsigned char* ioKeyArray,     /* Array for the key [in, out] */
//            unsigned int			iKeyArraySize,  /* Maximum length of the array for the key [in] */
//            unsigned int& oSize           /* Length of the key [out] */
//            );												// 定义函数指针类型
//    GenerateKeyExOpt GenKey;
//
//    GenKey = (GenerateKeyExOpt)GetProcAddress(DLLhandle, "GenerateKeyExOpt");     // GetProcAddress返回指向的函数名的函数地址
//
//    for (int i = 0; i < 16; i++)
//    {
//        ChallengeBytes[i] = SeedArray[i];
//    }
//
//    if(SeLevel==0x01)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x01[i];
//            //printf("pincode0x01 %d", pincode0x01[i]);
//        }
//    }
//    else if (SeLevel == 0x03)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x03[i];
//        }
//    }
//    else if (SeLevel == 0x05)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x05[i];
//        }
//    }
//    else if (SeLevel == 0x07)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x07[i];
//        }
//    }
//    else if (SeLevel == 0x09)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x09[i];
//        }
//    }
//    else if (SeLevel == 0x0B)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x0B[i];
//        }
//    }
//    else if (SeLevel == 0x0D)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x0D[i];
//        }
//    }
//    else if (SeLevel == 0x0F)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x0F[i];
//        }
//    }
//    else if (SeLevel == 0x11)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x11[i];
//        }
//    }
//    else if (SeLevel == 0x13)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x13[i];
//        }
//    }
//    else if (SeLevel == 0x15)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x15[i];
//        }
//    }
//    else if (SeLevel == 0x17)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x17[i];
//        }
//    }
//    else if (SeLevel == 0x19)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x19[i];
//        }
//    }
//    else if (SeLevel == 0x1B)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x1B[i];
//        }
//    }
//    else if (SeLevel == 0x1D)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x1D[i];
//        }
//    }
//    else if (SeLevel == 0x1F)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            ChallengeBytes[i + 16] = pincode0x1F[i];
//        }
//    }
//
//    //for (int i = 0; i < 32; i++) {
//    //    printf("%x", ChallengeBytes[i]);
//    //}
//
//    GenKey(ChallengeBytes, 16, SeLevel, "","", KeyArray, 16, ActualSize);
//
//    FreeLibrary(DLLhandle);
//}

void GenSAKeyExOpt(u8 SeLevel)
{
    char* DLLName = NULL;
    unsigned int ActualSize = 1;
    u8 ChallengeBytes[32] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };

    if (SeLevel == 0x01)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x01, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x03)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x03, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x05)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x05, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x07)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x07, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x09)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x09, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x0B)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x0B, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x0D)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x0D, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x0F)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x0F, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x11)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x11, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x13)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x13, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x15)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x15, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x17)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x17, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x19)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x19, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x1B)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x1B, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x1D)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x1D, SeedArray, 16, KeyArray);
        }
    }
    else if (SeLevel == 0x1F)
    {
        for (int i = 0; i < 16; i++)
        {
            AES_CMAC(pincode0x1F, SeedArray, 16, KeyArray);
        }
    }
}

void genkeydebug()
{
    char* DLLName = NULL;
    unsigned int ActualSize = 1;
    u8 ChallengeBytes[32] = { 0x6B,0xC1,0xBE,0xE2,0x2E,0x40,0x9F,0x96,0xE9,0x3D,0x7E,0x11,0x73,0x93,0x17,0x2A,0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C };
    u8 ChallengeBytes1[32] = { 0x30,0xE1,0x7B,0x36,0x7B,0x4F,0x5F,0xC7,0x40,0x12,0x21,0x36,0xE2,0xA6,0x9E,0x28,0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C };
    u8 ChallengeBytes2[32] = { 0x9A,0x9E,0xA7,0xCB,0xB8,0x3A,0x6D,0xED,0x59,0xAB,0x4F,0xD4,0x35,0x7C,0xE7,0xD2,0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C };
    u8 ChallengeBytes3[32] = { 0x01,0x34,0xe9,0x00,0x02,0x68,0xd2,0x01,0x05,0xd0,0xa4,0x03,0x0a,0xa0,0x49,0x07,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff };

    app.get_configuration_file_path(&DLLName);
    strcat(DLLName, "\\GenerateKeyExOpt.dll");

    // 运行时加载DLL库
    HMODULE DLLhandle = LoadLibraryA(DLLName);     // 根据DLL文件名，加载DLL，返回一个模块句柄
    if (DLLhandle == NULL)
    {
        TestReport_HTML_StepDescription("加载DLL动态库失败\n", Fail);
        return;
    }

    typedef int (*GenerateKeyExOpt)
        (
            const unsigned char* iSeedArray,     /* Array for the seed [in] */
            unsigned int			iSeedArraySize, /* Length of the array for the seed [in] */
            const unsigned int		iSecurityLevel, /* Security level [in] */
            const char* iVariant,       /* Name of the active variant [in] */
            const char* ipOptions,              // Optional parameter which might be used for OEM specific information [in]
            unsigned char* ioKeyArray,     /* Array for the key [in, out] */
            unsigned int			iKeyArraySize,  /* Maximum length of the array for the key [in] */
            unsigned int& oSize           /* Length of the key [out] */
            );												// 定义函数指针类型
    GenerateKeyExOpt GenKey;

    GenKey = (GenerateKeyExOpt)GetProcAddress(DLLhandle, "GenerateKeyExOpt");     // GetProcAddress返回指向的函数名的函数地址

    GenKey(ChallengeBytes, 16, 0x01, "", "", KeyArray, 16, ActualSize);

    //for(int i =0;i<16;i++)
    //{
    //    printf("%x", KeyArray[i]);
    //}

    //log("111111111111111111111");

    GenKey(ChallengeBytes1, 16, 0x01, "", "", KeyArray, 16, ActualSize);

    //for (int i = 0; i < 16; i++)
    //{
    //    printf("%x", KeyArray[i]);
    //}

    //log("2222222222222222222222");

    GenKey(ChallengeBytes2, 16, 0x01, "", "", KeyArray, 16, ActualSize);

    //for (int i = 0; i < 16; i++)
    //{
    //    printf("%x", KeyArray[i]);
    //}

    //log("333333333333333333333333");
    GenKey(ChallengeBytes3, 16, 0x01, "", "", KeyArray, 16, ActualSize);

    //for (int i = 0; i < 16; i++)
    //{
    //    printf("%x", KeyArray[i]);
    //}

    FreeLibrary(DLLhandle);
}


s32 SecurityUnlock(u8 SecurityLevel)
{
    SID27_SecurityAccess_RequestSeed(SecurityLevel);
    CheckPosResponse_SID27_RequestSeed(SecurityLevel);
    if (SeedArray[0] == 0 && SeedArray[1] == 0 && SeedArray[2] == 0)
        TestReport_HTML_StepDescription("Test fail,seed is all zero!", Fail);

    GenSAKeyExOpt(SecurityLevel);

    SID27_SecurityAccess_SendKey(SecurityLevel);
    CheckPosResponse_SID27_SendKey(SecurityLevel);
    return 0;
}

int StorageDTC()
{
    int StoI = 0;
    if (TpRxData[0] != 0x59)
    {
        TestReport_HTML_StepDescription("Error during process DTCs", Fail);
        return -1;
    }
    if (TpRxLength < 4)
    {
        TestReport_HTML_StepDescription("No avaliable DTC(TpRxLength<4)", Fail);
        Ndtc = 0;
        return 0;
    }
    for (StoI = 0; StoI < (TpRxLength - 3) / 4; StoI++)
    {
        recdtc[StoI].DTCNumber[0] = TpRxData[StoI * 4 + 3];
        recdtc[StoI].DTCNumber[1] = TpRxData[StoI * 4 + 4];
        recdtc[StoI].DTCNumber[2] = TpRxData[StoI * 4 + 5];
        recdtc[StoI].DTCStatus = TpRxData[StoI * 4 + 6];
        if (StoI < 2)
        {
            testdtc[StoI] = recdtc[StoI].DTCNumber[0] * 0x10000 + recdtc[StoI].DTCNumber[1] * 0x100 + recdtc[StoI].DTCNumber[2];
            TestDtc[StoI].DTCNumber[0] = TpRxData[StoI * 4 + 3];
            TestDtc[StoI].DTCNumber[1] = TpRxData[StoI * 4 + 4];
            TestDtc[StoI].DTCNumber[2] = TpRxData[StoI * 4 + 5];
            TestDtc[StoI].DTCStatus = TpRxData[StoI * 4 + 6];
        }
    }
    Ndtc = StoI;
    char Amsginfo[100] = {0};
    sprintf(Amsginfo,"Ndtc in function is %d and TpRxLength is %4d",StoI,TpRxLength);
    TestReport_HTML_StepDescription(Amsginfo);
    return Ndtc;
}


void DownloadSBL()
{
    SecurityUnlock(0x01);

    char keyvalue[20] = "SBL_Path";
    vbfparser(keyvalue);
    //此处新加，地址擦除，原代码没写。
    EraseAddress();
    FileDownload();
    CheckSignature();
    ActivateSBL();
}

void DownloadSBL_with_InvalidSig()
{
    ReadPublicKey();  //原代码没写，甚是疑惑，没读公匙怎么解锁
    SecurityUnlock(0x01);

    char keyvalue[20] = "SBL_Path";
    vbfparser(keyvalue);

    FileDownload();

    CheckSignatureInvalid();
    CheckSignature();
    ActivateSBL();
}

void SetParameters()
{
    char* INI_FILE_PATH = nullptr;
    char* iniptr = INI_FILE_PATH;
    char title[200] = "DiagUserDefine";
    char key[20] = "ECU_Address";
    char Buffer[200] = "";

    GetIniFilePath(&iniptr);

    GetINIKeyString(title, key, iniptr, Buffer);
    char* strptr;
    ECU_Address = strtol(Buffer, &strptr, 16);

    strcpy(title, "DiagParameters");
    strcpy(key, "Bus_Type");
    GetINIKeyString(title, key, iniptr, Buffer);
    char* strppp;
    BusTypeNow = strtod(Buffer,&strppp);
    printf("Now Bus Type is %d", BusTypeNow);


    strcpy(title, "DiagUserDefine");
    strcpy(key, "Platform");
    GetINIKeyString(title, key, iniptr, testPlatform);
    printf("Now Platform is %s", testPlatform);

    strcpy(key, "ECU Name");
    GetINIKeyString(title, key, iniptr, ECUName);
    printf("Now ECU Name is %s", ECUName);

    strcpy(key, "Supplier");
    GetINIKeyString(title, key, iniptr, SupplierName);
    printf("Now Suppliername is %s", SupplierName);

    strcpy(key, "Stage");
    GetINIKeyString(title, key, iniptr, TestStage);
    printf("Now Test Stage is %s", TestStage);

    strcpy(key, "Apartment");
    GetINIKeyString(title, key, iniptr, TestApartment);
    printf("Now Test Apartment is %s", TestApartment);

    strcpy(key, "Test Engineer");
    GetINIKeyString(title, key, iniptr, TestEngineer);
    printf("Now Test Engineer is %s", TestEngineer);

    strcpy(key, "Test Environment");
    GetINIKeyString(title, key, iniptr, TestEnvironment);
    printf("Now Test Environment is %s", TestEnvironment);

    char str[2] = "";
    char* ptr = NULL;
    u8 arr[16] = {};

    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x01");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x01[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x01[i-1]);
    }

    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x03");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        /*printf("%s", str);*/
        pincode0x03[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x03[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x05");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x05[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x05[i-1]);
    }

    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x07");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x07[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x07[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x09");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x09[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x09[i-1]);
    }

    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x0B");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x0B[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x0B[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x0D");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x0D[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x0D[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x0F");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x0F[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x0F[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x11");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x11[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x11[i-1]);
    }

    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x13");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x13[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x13[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x15");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x15[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x15[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x17");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x17[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x17[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x19");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x19[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x19[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x1B");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x1B[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x1B[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x1D");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x1D[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x1D[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x1F");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x1F[i-1] = strtol(str, &strptr, 16);
        printf("%x", pincode0x1F[i-1]);
    }


    if (BusTypeNow == CAN)
    {
        Diag_IFFD = 0;
    }
    else if (BusTypeNow == CANFD)
    {
        Diag_IFFD = 1;
    }

    Diag_Response_ID = ECU_Address & 0xfff;//63b
    Diag_Request_ID = Diag_Response_ID + 0x100;

    TScanfdResp.FIdentifier = ECU_Address & 0xfff;
    printf("resp id = %x", TScanfdResp.FIdentifier);
    TScanfdReq.FIdentifier = TScanfdResp.FIdentifier + 0x100;

    TScanResp.FIdentifier = ECU_Address & 0xfff;
    printf("resp id = %x", TScanResp.FIdentifier);
    TScanReq.FIdentifier = TScanResp.FIdentifier + 0x100;

    appindex = 0;
}


void Pretest()
{
    excelParser();
    DTCExcelParser();
    getCarConfigInfo();
    //strncpy(DUTName,"ZCUD",4);
    //Diag_Function_ID = 0X7FF;
    //Diag_Request_ID = 0X701;
    //Diag_Response_ID = 0x601;
    //gCEMFlg = 1;
    //printf("Power Serial is %s", main_power::instance()->main_power_Serial);
    //NetworkCluster["ZCU_CANFD2"] = CH11;
    //NetworkCluster["ZCU_CANFD1"] = CH9;
    //NetworkCluster["ChassisCANFD2"] = CH13;
    //NetworkCluster["ZCUD_CAN1"] = CH7;
    //NetworkCluster["PropulsionCAN"] = CH5;
    //NetworkCluster["ChassisCAN1"] = CH1;
    //NetworkCluster["BodyExposedCAN"] = CH15;
    NetworkCluster["ZCU_CANFD2"] = CH2;
    NetworkCluster["ZCU_CANFD1"] = CH3;
    NetworkCluster["ChassisCANFD2"] = CH4;
    NetworkCluster["ZCUD_CAN1"] = CH5;
    NetworkCluster["PropulsionCAN"] = CH6;
    NetworkCluster["ChassisCAN1"] = CH7;
    //NetworkCluster["BodyExposedCAN"] = CH8;
    NetworkCluster["ZCUD_CANFD3"] = CH8;
    NetworkCluster["ZCUD_CANFD4"] = CH9;
    NetworkCluster["ZCUP_CANFD5"] = CH4;
    NetworkCluster["ZCUP_CAN1"] = CH5;
    app.set_system_var_int32("MPLib.GEEA_CAN_DIAG", 1);
    app.connect();
    Diag_ch = NetworkCluster[TpBusName_Temp];
    //Diag_ch = CH2;
    //TestReport_Init();
    TestReport_HTML_Init();
    TestReport_TestInfo(HTMLHandle,DUTName, "","","", TestEngineer, Manufacturer);
    //TestReport_Cover();
    //readDB(1);
    //handle ini
    //SetParameters();

//    Test_Report.test_openissue(HTMLHandle, "序号,问题分类,测试案例ID,标题,提出时间,问题状态,问题描述,问题严重度,\
//平台,ECU名称,软件版本号,供应商,测试环境,测试部门,附件,责任方,计划完成时间,对策/答案,实际完成时间,测试阶段,问题关闭阶段,测试人员,审核人员,备注");

    DevicesCreat();
    AppWait(500, "");
    //SetBoardCardInit_Devices();
    AppWait(500, "");
    SetVoltage(12);
    AppWait(500, "");
    FindTRCPath();
    FindMissFramePath();
    getLINNetwork();
}

void TestFinish()
{
    DevicesInit();
    //BoardCardInit_Devices();
    
    //TestReport_End();
    TestReport_HTML_Final();
    uds_delete();
    app.set_system_var_int32("MPLib.GEEA_CAN_DIAG", 0);
    app.disconnect();
    //TestReport_ExportReport();
}

//void KL30(PowerControl type)
//{
//    if (type == off)
//    {
//        //下电
//    }
//    else
//    {
//        //上电
//    }
//}
//
//void KL15(PowerControl type)
//{
//    if (type == off)
//    {
//        //下电
//    }
//    else
//    {
//        //上电
//    }
//}

s8 CheckCommunication(u16 checkTime,u8 CheckChannel = 99) {
    char StepDescription[500] = {};
    s64 lastTime, currentTime = 0;
    s32 v;
    DUTBusoffChannel = CheckChannel;
    if (CheckChannel == 99) {
        snprintf(StepDescription, sizeof(StepDescription), "Check DUT Communication", CheckChannel + 1);
        TestReport_HTML_StepDescription(StepDescription, Info);
    }
    else{
        snprintf(StepDescription, sizeof(StepDescription), "Check Channel CH%d Communication", CheckChannel + 1);
        TestReport_HTML_StepDescription(StepDescription, Info);
    }
    //app.log_text(StepDescription,lvlInfo);
    app.set_system_var_int32("CheckCom", 0);
    printf("set CheckCom 0");
    //AppWait(checkTime, "");

    app.get_timestamp(&lastTime);
    while (1) {
        app.get_timestamp(&currentTime);
        app.get_system_var_int32("CheckCom", &v);
        if ((currentTime - lastTime) / 1000 > checkTime) {
            TestReport_HTML_StepDescription("DUT Communication Is Not Normal", Fail);
            return -1;
        }
        else if (v == 1) {

            if (((currentTime - lastTime) / 1000 < checkTime) && CheckChannel == 99) {
                TestReport_HTML_StepDescription("DUT Communication Is Normal", Info);
                return 0;
            }
            else if (abs((currentTime - lastTime) / 1000 - checkTime) < tolerabRecoveryleTime) {
                TestReport_HTML_StepDescription("DUT Communication Is Normal", Info);
                return 0;
            }
            else {
                TestReport_HTML_StepDescription("The DUT's recovery time for communication is shorter or longer than defined.", Warning);
                return 0;
            }
        }
    }

    //s32 v;
    //app.get_system_var_int32("CheckCom", &v);
    if (v == 1) {
        TestReport_HTML_StepDescription("DUT Communication Is Normal", Info);
        return 0;
    }
    else {
        TestReport_HTML_StepDescription("DUT Communication Is Not Normal", Fail);
        return -1;
    }
    return 0;
}

void PowerReset(s64 time=ECUResetTime)
{
    KL15(off);
    TestWait(500);
    KL30(off);
    TestWait(time);
    KL30(on);
    TestWait(500);
    KL15(on);
    TestWait(ECUResetTime);
}

double SetVoltageAndRead(double voltage,u8 flag = 1,u32 waitTime = 200) {
    double currentVol = 0;
    snprintf(StepDespriction, sizeof(StepDespriction), "Set Voltage is %.3lfV", voltage);
    TestReport_HTML_StepDescription(StepDespriction);
    main_power::instance()->Set_Voltage(voltage);

    if (flag) {
        AppWait(waitTime, "wait for read voltage");
        currentVol = main_power::instance()->Read_Val();
        snprintf(StepDespriction, sizeof(StepDespriction), "Current Voltage is %.3lfV", currentVol);
        TestReport_HTML_StepDescription(StepDespriction);
        return currentVol;
    }
    return 0;
}

s8 TestPrecondition()
{
    strcpy(issuetype, "Diagnostic Protocal");
    app.get_language_id(&language_id);
    TestWait(50);
    Test_Result = TestOK;
    char  systemLog[200];
    snprintf(systemLog,sizeof(systemLog),"%s\\", &logPath[0]);
    app.start_system_message_log(systemLog);

    if (language_id == 2052)
    {
        strcpy(printChar, "创建诊断会话");
        TestReport_PrintInfo(printChar);
        TestWait(50);
        strcpy(printChar, "开启诊断仪在线");
        TestReport_PrintInfo(printChar);
    }
    else
    {
        strcpy(printChar, "create diagnostic communication");
        TestReport_PrintInfo(printChar);
        TestWait(stepseptime);
        strcpy(printChar, "tester present on!");
        TestReport_PrintInfo(printChar);
    }
    TestParamentInit();
    Pretestcase();
    PrintCaseNameAndPurpose();


    BoardCard_Init();
    MainPwr_to_NBATGND(ON);
    //DUT KL30\KL31上电
    KL30(ON);
    KL31(ON);

    TestReport_StartLogging();
    TestReport_TableInit();
    //main_power::instance()->Set_Voltage(12);
    SetVoltageAndRead(12,1,500);
    MainPower_ON();

    //KL30(on);
    TestWait(500);
    //KL15(on);

    NMWakeUp(1);
    
    TestWait(caseSepTime);

    SendTesterPresent(TP_ON);
    TestWait(stepseptime);

    uds_create();

    resultFlag = 0; issueflag = 0;

    reportOutputFlag = 1;

    TestStep("上电并唤醒被测件");
    if (0 != CheckCommunication(5000)) {
        TestPostCondition();
        return -1;
    }
    SimulateTRC(true);
    return 0;
}

void TestPrecondition_SWDL()
{
  

    strcpy(issuetype, "Software Download");

    app.get_language_id(&language_id);
    TestWait(50);

    

    if (language_id == 2052)
    {
        strcpy(printChar, "创建诊断会话");
        TestReport_PrintInfo(printChar);
        TestWait(50);
        strcpy(printChar, "开启诊断仪在线");
        TestReport_PrintInfo(printChar);
    }
    else
    {
        strcpy(printChar, "create diagnostic communication");
        TestReport_PrintInfo(printChar);
        TestWait(stepseptime);
        strcpy(printChar, "tester present on!");
        TestReport_PrintInfo(printChar);
    }

    SendTesterPresent(TP_ON);
    TestWait(stepseptime);

    PrintCaseNameAndPurpose();

    TestReport_StartLogging();

    TestReport_TableInit();

    KL30(on);
    TestWait(500);
    KL15(on);

    NMWakeUp(1);

    TestWait(caseSepTime);

    uds_create();

    SID10_SessionControl(0x01);
    TestWait(1500);

    resultFlag = 0; issueflag = 0;

    reportOutputFlag = 1;
}



void TestPostCondition_SWDL()
{
    TestReport_StopLogging();

    TestWait(stepseptime);
    SendTesterPresent(TP_OFF);
    TestWait(stepseptime);

    reportOutputFlag = 0;
    TestWait(1500);

    uds_delete();

    NMWakeUp(0);

    KL15(off);
    TestWait(500);
    KL30(off);
}

void TestPrecondition_TP_CAN()
{
  

    strcpy(issuetype, "CAN Diagnostic Communication");

    app.get_language_id(&language_id);

    TestWait(stepseptime);

    PrintCaseNameAndPurpose();

    TestReport_StartLogging();

    TestReport_TableInit();

    KL30(on);
    TestWait(500);
    KL15(on);

    NMWakeUp(1);

    TestWait(caseSepTime);

    resultFlag = 0; issueflag = 0;

    ReportDescriptionFlag = 1;
}

void TestPostCondition_TP_CAN()
{
    //TestReport_EndTable();
    ReportDescriptionFlag = 0;

    CAN_TP_TesterPresent(0);

    TestReport_StopLogging();

    TestWait(caseSepTime);
    
    NMWakeUp(0);
    
    KL15(off);
    TestWait(500);
    KL30(off);
}

void TestPrecondition_TP_CANFD()
{
   

    strcpy(issuetype, "CANFD Diagnostic Communication");

    app.get_language_id(&language_id);

    TestWait(stepseptime);

    PrintCaseNameAndPurpose();

    TestReport_StartLogging();

    TestReport_TableInit();


    KL30(on);
    TestWait(500);
    KL15(on);

    NMWakeUp(1);

    TestWait(caseSepTime);

    resultFlag = 0; issueflag = 0;

    TScanfdReq.FDLC = 8;

    ReportDescriptionFlag = 1;
}

void TestPostCondition_TP_CANFD()
{
    ReportDescriptionFlag = 0;
    //TestReport_EndTable();
    CAN_TP_TesterPresent(0);

    TestReport_StopLogging();

    TestWait(caseSepTime);

    NMWakeUp(0);

    KL15(off);
    TestWait(500);
    KL30(off);

   // app.disconnect();
}

void TestPrecondition_LIN()
{
    strcpy(issuetype, "LIN Diagnostic Communication");

    app.get_language_id(&language_id);

    TestWait(stepseptime);

    PrintCaseNameAndPurpose();

    TestReport_StartLogging();

    TestReport_TableInit();

    KL30(on);
    TestWait(500);
    KL15(on);

    NMWakeUp(1);

    TestWait(caseSepTime);

    uds_create();

    resultFlag = 0; issueflag = 0;

    ReportDescriptionFlag = 1;
}

void TestPostCondition_LIN()
{
    uds_delete();

    ReportDescriptionFlag = 0;

    TestReport_StopLogging();

    TestWait(caseSepTime);

    NMWakeUp(0);

    KL15(off);
    TestWait(500);
    KL30(off);
}


s32 CheckSoftwareVersion() {
    u8 DID[2] = { 0xF1,0xAE };
    int i = 0, j = 0;
    int resflag = 0;

    for (int k = 0; k < APPFileNum; k++) {
        _sw_[k].CheckFlag = 0;
    }

    SID22_ReadDataByIdentifier(DID);
    if (0 == CheckPosResponse_SID22(DID)) {
        u8 swnum = TpRxData[5];
        char temp1[20] = "", temp2[10] = "";

        for(j=0;j<swnum;j++)
        {
            for (i = 0; i < 6; i++) {
                sprintf(temp1, "%d", TpRxData[6 + i + 8*j]);
            }
            sprintf(temp2, "%s", char(TpRxData[6 + i + 8 * j + 7]));

            for (int k = 0; k < APPFileNum; k++) {
                if (strcmp(temp1, _sw_[k].sw_part_number) == 0 && strcmp(temp2, _sw_[k].sw_version) == 0) {
                    _sw_[k].CheckFlag = 1;
                }
            }
        }

        for (int k = 0; k < APPFileNum; k++) {
            if (_sw_[k].CheckFlag != 1) {
                resflag++;
            }
        }
    }

    if (resflag == 0) {
        TestReport_HTML_StepDescription("读取到的软件版本号和写入的软件版本一致！", Pass);
    }
    else
    {
        TestReport_HTML_StepDescription("读取到的软件版本号和写入的软件版本不一致！", Fail);
    }

    return 0;
}

void TestSeq_EnterProgrammingSession() {
    TestReport_HTML_StepDescription("----Execute TestSeq_EnterProgrammingSession----");

    CheckCurrentSession(0x01);

    CheckProgrammingPrecondition();

    for (int i = 0; i < 5; i++)
    {
        SID10_SessionControl(0x82);
        CheckNonResponse();
    }

    SID10_SessionControl(0x02);
    CheckPosResponse_SID10(0x02);

    CheckCurrentSession(0x02);
}

void TestSeq_DownloadSBL(u8 ifactivate = 1, u8 ifDupli36 = 0) {
    TestReport_HTML_StepDescription("----Execute TestSeq_DownloadSBL----");

    char keyvalue[20] = "SBL_Path";
    ReadPublicKey();
    SecurityUnlock(0x01);
    vbfparser(keyvalue);
    //EraseAddress(); //vector的报告里确实没有擦除
    FileDownload(ifDupli36);
    CheckSignature();
    if (ifactivate == 1)
        ActivateSBL();
}

void TestSeq_DownloadAPP(u8 ifDupli36 = 0, u8 ifAbort = 0, u8 abortat = 0)
{
    TestReport_HTML_StepDescription("----Execute TestSeq_DownloadAPP----");

    char keyvalue[20] = "APP0Path";

    for (int i = 0; i < APPFileNum; i++)
    {
        vbfparser(keyvalue);
        EraseAddress();
        if (ifAbort == 1 && abortat == i)
        {
            return;
        }
        FileDownload(ifDupli36);
        CheckSignature();
        keyvalue[3]++;
    }
}

void TestSeq_DownloadAPP_with_InvalidSig()
{
    char keyvalue[20] = "APP0Path";

    for (int i = 0; i < APPFileNum; i++)
    {
        vbfparser(keyvalue);
        EraseAddress();
        FileDownload();
        CheckSignatureInvalid();
        CheckSignature();
        keyvalue[3]++;
    }
}

// 1 = Min  2 = Max
void TestSeq_DownloadAPP_Aborted(u8 abortat = 0)
{
    char keyvalue[20] = "APP0Path";

    for (int i = 0; i < APPFileNum; i++)
    {
        vbfparser(keyvalue);
        EraseAddress();
        if (0 != FileDownload_Abort(abortat))
        {
            return;
        }
        CheckSignature();
        keyvalue[3]++;
    }
}

//add function by WYB
void UsgModSwtich(u8 usgmod)
{
    if (gCEMFlg == 1)
    {
        u8 Req_2F[4];
        //2f dd 0a
        Req_2F[0] = 0xDD;
        Req_2F[1] = 0x0A;
        Req_2F[2] = 0x03;
        Req_2F[3] = usgmod;

        SID2F_IO_Control_byid(Req_2F, 4);
        CheckPosResponse_SID2F(Req_2F, 4);
    }
    else
    {
        gUagMode = usgmod;
    }
}

void CarModSwtich(u8 carmod)
{
    if (gCEMFlg == 1)
    {
        u8 Req_2F[4];
        //2f D1 34
        Req_2F[0] = 0xD1;
        Req_2F[1] = 0x34;
        Req_2F[2] = 0x03;
        Req_2F[3] = carmod;

        SID2F_IO_Control_byid(Req_2F, 4);
        CheckPosResponse_SID2F(Req_2F, 4);
    }
    else
    {
        gCarMode = carmod;
    }
}

void EPLvlSwitch(u8 eplv)
{
    if (gCEMFlg == 1)
    {
        u8 Req_2F[5];
        //2f 42 9e
        Req_2F[0] = 0x42;
        Req_2F[1] = 0x9e;
        Req_2F[2] = 0x03;
        Req_2F[3] = eplv << 4;
        Req_2F[4] = 0x80;

        SID2F_IO_Control_byid(Req_2F, 5);
        CheckPosResponse_SID2F(Req_2F, 4);
    }
    else
    {
        gEPLvl = eplv;
    }
}

void TRCSwitch(u8 carmode, u8 usgmode, u8 EIPwrLvl)
{
    if (gCEMFlg == 1)
    {
        snprintf(StepDespriction, sizeof(StepDespriction), "切换Carmode为0x%X,Usgmode为0x%X,EIPwrLvl为0x%X", carmode, usgmode, EIPwrLvl);
        TestReport_HTML_StepDescription(StepDespriction);
        CarModSwtich(carmode);
        UsgModSwtich(usgmode);
        EPLvlSwitch(EIPwrLvl);
    }
    else
    {
        snprintf(StepDespriction, sizeof(StepDespriction), "切换Carmode为%d,Usgmode为0x%X,EIPwrLvl为%d", carmode, usgmode, EIPwrLvl);
        gEPLvl = EIPwrLvl;
        gUagMode = usgmode;
        gCarMode = carmode;
    }
  
}

void TestWaitForCurInRange(float current, long time)
{
    int i;
    float Cur;
    char StepDespriction[256];

    for (i = 0; i < (time / 1000); i++)
    {
        Cur = main_power::instance()->Read_Cur();
        snprintf(StepDespriction, sizeof(StepDespriction), "The current is %5.3fmA", Cur * 1000);
        TestReport_HTML_StepDescription(StepDespriction, Info);
        AppWait(1000, "");
        if (Cur*1000 <= current)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "The current is %5.3fmA", Cur*1000);
            TestReport_HTML_StepDescription(StepDespriction, Pass);
            break;
        }

    }
    if (i == (time / 1000))
    {
        snprintf(StepDespriction, sizeof(StepDespriction), "Time out! The current is %5.3fmA, expected is %5.3fmA", Cur*1000, current);
        TestReport_HTML_StepDescription(StepDespriction, Info);
    }
    AppWait(2000, "");

}

int ReadOCC1(u8 dtc[])
{
    int OCC1;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x01, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        OCC1 = TpRxData[7];
    else
        OCC1 = 0;
    return OCC1;
}

int ReadOCC2(u8 dtc[])
{
    int OCC2;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x02, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        OCC2 = TpRxData[7];
    else
        OCC2 = 0;
    return OCC2;
}

int ReadOCC3(u8 dtc[])
{
    int OCC3;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x03, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        OCC3 = TpRxData[7];
    else
        OCC3 = 0;
    return OCC3;
}

int ReadOCC4(u8 dtc[])
{
    int OCC4;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x04, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        OCC4 = TpRxData[7];
    else
        OCC4 = 0;
    return OCC4;
}

int ReadOCC5(u8 dtc[])
{
    int OCC5;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x05, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        OCC5 = TpRxData[7];
    else
        OCC5 = 0;
    return OCC5;
}

int ReadOCC6(u8 dtc[])
{
    int OCC6;
    char Stepdespriction[256];

    if (gAsrVersion >= 4.1)
    {
        OCC6 = -1;
        strcpy(Stepdespriction, "For Autosar 4.1 or later, it is not mandatory that the value of OCC6 can be read by diagnostic services.");
        TestReport_HTML_StepDescription(Stepdespriction, Info);
    }
    else
    {
        RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x06, Physical);
        Rx_CheckPosResponse_SID19_06(dtc);
        if (TpRxLength >= 8)
            OCC6 = TpRxData[7];
        else
            OCC6 = 0;
    }

    return OCC6;
}

int ReadOCC7(u8 dtc[])
{
    int OCC7;
    char Stepdespriction[256];

    if (gAsrVersion >= 4.1)
    {
        OCC7 = -1;
        strcpy(Stepdespriction, "OCC7 is not applicable for AUTOSAR 4.1 or newer.");
        TestReport_HTML_StepDescription(Stepdespriction, Info);
    }
    else
    {
        RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x07, Physical);
        Rx_CheckPosResponse_SID19_06(dtc);
        if (TpRxLength >= 8)
            OCC7 = TpRxData[7];
        else
            OCC7 = 0;
    }

    return OCC7;
}

void ReadAllOCC(pu8 dtc,s16 OC1 = -1, s16 OC2 = -1, s16 OC3 = -1, s16 OC4 = -1, s16 OC6 = -1, s16 OC7 = -1) {
    int occ1 = -1, occ2, occ3, occ4, occ6, occ7; 

    if(OC1!= -1){
        occ1 = ReadOCC1(dtc);
        snprintf(StepDespriction, sizeof(StepDespriction), "OCC1 is %d", occ1);
        if (occ1 == OC1) {
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        }
        else {
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        }
    }

    if (OC2 != -1) {
        occ2 = ReadOCC2(dtc);
        snprintf(StepDespriction, sizeof(StepDespriction), "OCC1 is %d", occ1);
        if (occ2 == OC2) {
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        }
        else {
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        }
    }

    if (OC3 != -1) {
        occ3 = ReadOCC3(dtc);
        snprintf(StepDespriction, sizeof(StepDespriction), "OCC3 is %d", occ1);
        if (occ3 == OC3) {
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        }
        else {
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        }
    }

    if (OC4 != -1) {
        occ4 = ReadOCC4(dtc);
        snprintf(StepDespriction, sizeof(StepDespriction), "OCC4 is %d", occ1);
        if (occ4 == OC4) {
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        }
        else {
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        }
    }

    if (OC6 != -1) {
        occ6 = ReadOCC6(dtc);
        snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 is %d", occ1);
        if (occ6 == OC6) {
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        }
        else {
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        }
    }

    if (OC7 != -1) {
        if (gAsrVersion >= 4.1) {
            TestReport_HTML_StepDescription("OCC7 is not applicable for AUTOSAR 4.1 or newer.", Warning);
        }
        else {
            occ7 = ReadOCC7(dtc);
            snprintf(StepDespriction, sizeof(StepDespriction), "OCC7 is %d", occ1);
            if (occ7 == OC7) {
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }
            else {
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }
        }
    }
}

int ReadAgingCounter(u8 dtc[], u8 agingCounter)
{
    if (agingCounter == 0x2)
        return ReadOCC2(dtc);
    else
        return ReadOCC5(dtc);
}

int ReadAgedDTC(u8 dtc[])
{
    int state;
    char Stepdespriction[256];
    if (gAsrVersion >= 4.1)
    {
        state = -1;
        strcpy(issuereason, "Note: This step is applicable for Autosar 4.0.x");
        TestReport_HTML_Step(StepNumber, issuereason, Info);
    }
    else
    {
        RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x30, Physical);
        Rx_CheckPosResponse_SID19_06(dtc);
        if (TpRxLength >= 8)
            state = (TpRxData[7] & 0x08) >> 3;
        else
            state = 0;
    }
    return state;
}


u8 ReadSnapShot(u8 dtc[])
{
    u8 state;
    RX_SID19_ReportDTCSnapshotRecordByDTCNumber(dtc, gSnapshotRecordNo, Physical);
    RX_CheckPosResponse_SID19_04(dtc);
    if (TpRxLength >= 8)
        state = 1;
    else
        state = 0;
    return state;
}

u8 ReadUncofirmedDTC(u8 dtc[])
{
    u8 state;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x30, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        state = TpRxData[7] & 0x01;
    else
        state = 0;
    return state;
}

u8 ReadUncnfrmDTCSncLstClr(u8 dtc[])
{
    u8 state;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x30, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        state = (TpRxData[7] & 0x04) >> 2;
    else
        state = 0;
    return state;
}

u8 ReadUncnfrmDTCThisOpCyc(u8 dtc[])
{
    u8 state;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x30, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        state = (TpRxData[7] & 0x02) >> 1;
    else
        state = 0;
    return state;
}

u8 ReadTestFailSncLstClrOrAged(u8 dtc[])
{
    u8 state;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x30, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        state = (TpRxData[7] & 0x80) >> 7;
    else
        state = 0;
        return state;
}

int ConvertUnsgnToSgn(u8 a)
{
    int value;
    if (a > 0x7F)
    {
        value = -(((~a) & 0x7f) + 1);
    }
    else
    {
        value = a;
    }
    return value;
}

int ReadFDC10(u8 dtc[])
{
    int FDC;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x10, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        FDC = TpRxData[7];
    else
        FDC = 0;
    return FDC;
}

int ReadFDC11(u8 dtc[])
{
    int FDC;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x11, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        FDC = TpRxData[7];
    else
        FDC = 0;
    return FDC;
}

int ReadFDC12(u8 dtc[])
{
    int FDC;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, 0x12, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
        FDC = TpRxData[7];
    else
        FDC = 0;
    return FDC;
}
/*
void CheckStepUpValue(u8 dtc[], int cycle, int step, u8 ReadSnapshot = 1)
{
    int n, fdcflag;
    char Stepdespriction[256];

    gtestDTC[0] = dtc[0];
    gtestDTC[1] = dtc[1];
    gtestDTC[2] = dtc[2];
    gstepValue = step;
    gtestIndex = 0;
    gLastIndex = 0;
    gfdc1 = 0;
    gfdc2 = 0;
    gsnapshotFlag = 0;
    gReadEndFlag = 1;
    isRunning = true;

    std::thread myThread1(StepUpTest, cycle);
    while (1)
    {
        if (gLastIndex > 150)
        {
            snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 can't reach 127!");
            TestReport_HTML_Step(StepNumber, Stepdespriction, Fail);
            break;
        }
        if (0 != AppWait_system_var("Value_CheckFDC10", "1", 6000))
        {
            TestReport_HTML_StepDescription("等待超时，在规定时间内，线程没有赋值");
            break;// Debug
        }
        app.set_system_var_int32("Value_CheckFDC10", 0);
        gReadEndFlag = 0;
        n = gtestIndex - gLastIndex;
        gLastIndex = gtestIndex;
        gfdc2 = ConvertUnsgnToSgn(ReadFDC10(gtestDTC));
        snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 is %d", gfdc2);
        TestReport_HTML_StepDescription(Stepdespriction, Info);
        if (gfdc2 > 0)
        {
            if (gfdc2 != 0x7F)fdcflag = 1;
            //if ((((gfdc2 - gfdc1 != n * gstepValue) || (gfdc2 - gfdc1 != (n + 1) * gstepValue)) && (gfdc2 != 0x7F)) || ((gfdc2 - gfdc1) > n * gstepValue))
            if (((gfdc2 - gfdc1 != n * gstepValue) && (gfdc2 != 0x7F)))
            {
                if ((gfdc2 - gfdc1 == (n + 1) * gstepValue) || (gfdc2 - gfdc1 == (n - 1) * gstepValue)){
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepUpValue is %d, expect is %d", gfdc2 - gfdc1, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Warning);
                }
                else {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepUpValue is %d, expect is %d", gfdc2 - gfdc1, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            else
            {
                if (fdcflag)
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepUpValue is %d", gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Pass);
                }
                else
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The first positive value is 127!");
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            gfdc1 = gfdc2;
        }
        else
        {
            if (gtestIndex > 100)
            {
                snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 can't increase! Break test.");
                TestReport_HTML_StepDescription(Stepdespriction, Fail);
                break;
            }
        }
        //------------------Read snapshot to test UnconfirmedDTCLimit-------------------
        if (gUnCofrmLmt)
        {
            if (gsnapshotFlag == 0 && ReadSnapshot == 1)
            {
                gsnapshotFlag = ReadSnapShot(gtestDTC);
                if (gsnapshotFlag > 0)
                {
                    if (gfdc2 >= gUnCofrmLmt)
                    {
                        snprintf(Stepdespriction, sizeof(Stepdespriction), "Find snapshot when FDC10 is %d(>=%d)", gfdc2, gUnCofrmLmt);
                        TestReport_HTML_StepDescription(Stepdespriction, Pass);
                    }
                    else
                    {
                        if (gtestIndex * cycle >= gUnCofrmLmt)
                        {
                            snprintf(Stepdespriction, sizeof(Stepdespriction), "Find snapshot when FDC10 is %d(<%d). Maybe FDC10 has reached unconfirmedDTCLimit, please check the time stamp.", gfdc2, gUnCofrmLmt);
                            TestReport_HTML_StepDescription(Stepdespriction, Info);
                        }
                        else
                        {
                            snprintf(Stepdespriction, sizeof(Stepdespriction), "Find snapshot when FDC10 is %d(<%d)", gfdc2, gUnCofrmLmt);
                            TestReport_HTML_StepDescription(Stepdespriction, Fail);
                        }
                    }
                }
                else
                {
                    if (gfdc2 >= gUnCofrmLmt)
                    {
                        snprintf(Stepdespriction, sizeof(Stepdespriction), "No snapshot when FDC10 is %d(>=%d)", gfdc2, gUnCofrmLmt);
                        TestReport_HTML_StepDescription(Stepdespriction, Fail);
                    }
                    else
                    {
                        snprintf(Stepdespriction, sizeof(Stepdespriction), "No snapshot when FDC10 is %d(<%d)", gfdc2, gUnCofrmLmt);
                        TestReport_HTML_StepDescription(Stepdespriction, Pass);
                    }
                }
            }
        }
        if (gfdc2 == 0x7F)
        {
            snprintf(Stepdespriction, sizeof(Stepdespriction), "The max value of FDC10 is 0x7F");
            TestReport_HTML_StepDescription(Stepdespriction, Pass);
            break;
        }
        gReadEndFlag = 1;
    }
    //停止线程
    isRunning = false;
    // 等待线程结束
    myThread1.join();
    gUnCofrmLmt = 0;
    //return 0;
}
*/

void CheckStepUpValue(u8 dtc[], int cycle, int step, u8 ReadSnapshot = 1)
{
    int n, fdcflag;
    char Stepdespriction[256];

    // --- 初始化全局变量 ---
    gtestDTC[0] = dtc[0];
    gtestDTC[1] = dtc[1];
    gtestDTC[2] = dtc[2];
    gstepValue = step;
    gtestIndex = 0;
    gLastIndex = 0;
    gfdc1 = 0;
    gfdc2 = 0;
    gsnapshotFlag = 0;
    gReadEndFlag = 1;

    // 设置模式为 Step Up
    gCurrentTestMode = MODE_STEP_UP;

    // --- 启动定时器 ---
    // 假设 NewTimer1 是定时器对象名称
    // 如果定时器支持设置周期，请使用 NewTimer1.start(cycle);
    // 如果固定100ms，则直接 start()
    printf("NewTimer1.start()");
    NewTimer1.start();

    while (1)
    {
        if (gLastIndex > 150)
        {
            snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 can't reach 127!");
            TestReport_HTML_Step(StepNumber, Stepdespriction, Fail);
            break;
        }

        // 等待定时器触发后置位变量
        if (0 != app.wait_system_var("Value_CheckFDC10", "1", 6000))
        {
            TestReport_HTML_StepDescription("等待超时，在规定时间内，定时器没有赋值");
            break;
        }

        // 清除信号，准备下一次
        app.set_system_var_int32("Value_CheckFDC10", 0);
        gReadEndFlag = 0; // 标记正在处理读取

        n = gtestIndex - gLastIndex;
        gLastIndex = gtestIndex;

        // 读取 FDC 值
        gfdc2 = ConvertUnsgnToSgn(ReadFDC10(gtestDTC));

        snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 is %d", gfdc2);
        TestReport_HTML_StepDescription(Stepdespriction, Info);

        // --- 逻辑判断部分 (保持原样) ---
        if (gfdc2 > 0)
        {
            if (gfdc2 != 0x7F) fdcflag = 1;

            if (((gfdc2 - gfdc1 != n * gstepValue) && (gfdc2 != 0x7F)))
            {
                if ((gfdc2 - gfdc1 == (n + 1) * gstepValue) || (gfdc2 - gfdc1 == (n - 1) * gstepValue)) {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepUpValue is %d, expect is %d", gfdc2 - gfdc1, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Warning);
                }
                else {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepUpValue is %d, expect is %d", gfdc2 - gfdc1, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            else
            {
                if (fdcflag)
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepUpValue is %d", gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Pass);
                }
                else
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The first positive value is 127!");
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            gfdc1 = gfdc2;
        }
        else
        {
            if (gtestIndex > 100)
            {
                snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 can't increase! Break test.");
                TestReport_HTML_StepDescription(Stepdespriction, Fail);
                break;
            }
        }

        /*------------------Read snapshot check (保持原样)-------------------*/
        if (gUnCofrmLmt)
        {
            // 注意：这里用到了 cycle 参数进行计算
            // 如果传入的 cycle != 定时器实际周期(100ms)，这里的计算会有误差
            if (gsnapshotFlag == 0 && ReadSnapshot == 1)
            {
                gsnapshotFlag = ReadSnapShot(gtestDTC);
                if (gsnapshotFlag > 0)
                {
                    if (gfdc2 >= gUnCofrmLmt)
                    {
                        snprintf(Stepdespriction, sizeof(Stepdespriction), "Find snapshot when FDC10 is %d(>=%d)", gfdc2, gUnCofrmLmt);
                        TestReport_HTML_StepDescription(Stepdespriction, Pass);
                    }
                    else
                    {
                        if (gtestIndex * cycle >= gUnCofrmLmt)
                        {
                            snprintf(Stepdespriction, sizeof(Stepdespriction), "Find snapshot when FDC10 is %d(<%d). Maybe FDC10 has reached unconfirmedDTCLimit...", gfdc2, gUnCofrmLmt);
                            TestReport_HTML_StepDescription(Stepdespriction, Info);
                        }
                        else
                        {
                            snprintf(Stepdespriction, sizeof(Stepdespriction), "Find snapshot when FDC10 is %d(<%d)", gfdc2, gUnCofrmLmt);
                            TestReport_HTML_StepDescription(Stepdespriction, Fail);
                        }
                    }
                }
                else
                {
                    // Copy paste from original logic...
                    if (gfdc2 >= gUnCofrmLmt) {
                        snprintf(Stepdespriction, sizeof(Stepdespriction), "No snapshot when FDC10 is %d(>=%d)", gfdc2, gUnCofrmLmt);
                        TestReport_HTML_StepDescription(Stepdespriction, Fail);
                    }
                    else {
                        snprintf(Stepdespriction, sizeof(Stepdespriction), "No snapshot when FDC10 is %d(<%d)", gfdc2, gUnCofrmLmt);
                        TestReport_HTML_StepDescription(Stepdespriction, Pass);
                    }
                }
            }
        }

        if (gfdc2 == 0x7F)
        {
            snprintf(Stepdespriction, sizeof(Stepdespriction), "The max value of FDC10 is 0x7F");
            TestReport_HTML_StepDescription(Stepdespriction, Pass);
            break;
        }
        gReadEndFlag = 1; // 处理完毕，允许定时器进行下一次
    }

    // --- 停止定时器 ---
    NewTimer1.stop();
    gCurrentTestMode = MODE_IDLE;
    gUnCofrmLmt = 0;
}

void CheckStepUpValueByIndex(int index, u8 dtc[], int cycle, int step, u8 ReadSnapshot = 0)
{
    int n, fdcflag;
    char Stepdespriction[256];
    s64 nowTime = 0, lastTime = 0;

    // --- 初始化全局变量 ---
    gtestDTC[0] = dtc[0];
    gtestDTC[1] = dtc[1];
    gtestDTC[2] = dtc[2];
    gstepValue = step;
    gtestIndex = 1;
    gLastIndex = 0;
    gfdc1 = 0;
    gfdc2 = 0;
    gReadEndFlag = 1;

    printf("Start Timer");
    TestStepValueByIndex.start();
    TestStepValueByIndex.set_interval(cycle);
    app.get_timestamp(&lastTime);

    while (gtestIndex < index && (nowTime - lastTime) < (cycle*index*1000 + 3000000))
    {
        n = gtestIndex - gLastIndex;
        gLastIndex = gtestIndex;
        app.get_timestamp(&nowTime);
        if (n < 1)
            continue;

        snprintf(Stepdespriction, sizeof(Stepdespriction), "--------------%dth cycle test of FDC10 Step Up------------", gtestIndex);
        TestReport_HTML_StepDescription(Stepdespriction, Info);



        // 读取 FDC 值
        gfdc2 = ConvertUnsgnToSgn(ReadFDC10(gtestDTC));

        snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 is %d", gfdc2);
        TestReport_HTML_StepDescription(Stepdespriction, Info);

        if (gfdc2 > 0)
        {
            if (gfdc2 != 0x7F) fdcflag = 1;

            if (((gfdc2 - gfdc1 != n * gstepValue) && (gfdc2 != 0x7F)))
            {
                if ((gfdc2 - gfdc1 == (n + 1) * gstepValue) || (gfdc2 - gfdc1 == (n - 1) * gstepValue)) {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepUpValue is %d, expect is %d", gfdc2 - gfdc1, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Warning);
                }
                else {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepUpValue is %d, expect is %d", gfdc2 - gfdc1, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            else
            {
                if (fdcflag)
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepUpValue is %d", gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Pass);
                }
                else
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The first positive value is 127!");
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            gfdc1 = gfdc2;
        }
    }
    gReadEndFlag = 0;
    TestStepValueByIndex.stop();
}


/*
void CheckStepDownValue(u8 dtc[], int cycle, int step)
{
    int n, fdcflag;
    char Stepdespriction[256];

    gtestDTC[0] = dtc[0];
    gtestDTC[1] = dtc[1];
    gtestDTC[2] = dtc[2];
    gstepValue = step;
    gtestIndex = 0;
    gLastIndex = 0;
    gfdc1 = 0;
    gfdc2 = 0;
    gReadEndFlag = 1;
    isRunning = true;

    std::thread myThread2(StepDownTest,cycle);

    while (1)
    {
        if (gLastIndex > 150)
        {
            strcpy(Stepdespriction, "FDC10 can't reach -128!");
            TestReport_HTML_StepDescription(Stepdespriction, Fail);
            break;
        }
        if (0 != AppWait_system_var("Value_CheckFDC10", "1", 6000))
        {

        }
        app.set_system_var_int32("Value_CheckFDC10", 0);
        gReadEndFlag = 0;
        n = gtestIndex - gLastIndex;
        gLastIndex = gtestIndex;
        gfdc2 = ConvertUnsgnToSgn(ReadFDC10(gtestDTC));
        snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 is %d", gfdc2);
        TestReport_HTML_StepDescription(Stepdespriction, Info);
        if (gfdc2 < 0)
        {
            if (gfdc2 != -128)fdcflag = 1;
            if (((gfdc1 - gfdc2 != n * gstepValue) && (gfdc2 != -128)))
            {
                if ((gfdc1 - gfdc2 == (n + 1) * gstepValue) || (gfdc1 - gfdc2 == 0)) {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepDownValue is %d, expect is %d", gfdc1 - gfdc2, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Warning);
                }
                else {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepDownValue is %d, expect is %d", gfdc1 - gfdc2, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            else
            {
                if (fdcflag)
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepDownValue is %d", gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Pass);
                }
                else
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The first negative value is -128!");
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            gfdc1 = gfdc2;
        }
        else
        {
            if (gtestIndex > 100)
            {
                snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 can't decrease! Break test.");
                TestReport_HTML_StepDescription(Stepdespriction, Fail);
                break;
            }
        }
        if (gfdc2 == -128)
        {
            snprintf(Stepdespriction, sizeof(Stepdespriction), "The minimun value of FDC10 is 0x80");
            TestReport_HTML_StepDescription(Stepdespriction, Pass);
            break;
        }
        gReadEndFlag = 1;
    }
    isRunning = false;
    // 等待线程结束
    myThread2.join();
}*/

void CheckStepDownValue(u8 dtc[], int cycle, int step)
{
    int n, fdcflag;
    char Stepdespriction[256];

    // --- 初始化 ---
    gtestDTC[0] = dtc[0];
    gtestDTC[1] = dtc[1];
    gtestDTC[2] = dtc[2];
    gstepValue = step;
    gtestIndex = 0;
    gLastIndex = 0;
    gfdc1 = 0;
    gfdc2 = 0;
    gReadEndFlag = 1;

    // 设置模式为 Step Down
    gCurrentTestMode = MODE_STEP_DOWN;

    // --- 启动定时器 ---
    NewTimer1.start(); // 同样，如果支持设置周期，使用 start(cycle)

    while (1)
    {
        if (gLastIndex > 150)
        {
            strcpy(Stepdespriction, "FDC10 can't reach -128!");
            TestReport_HTML_StepDescription(Stepdespriction, Fail);
            break;
        }

        // 等待定时器触发
        if (0 != app.wait_system_var("Value_CheckFDC10", "1", 6000))
        {
            // 超时处理（可选）
        }

        app.set_system_var_int32("Value_CheckFDC10", 0);
        gReadEndFlag = 0;

        n = gtestIndex - gLastIndex;
        gLastIndex = gtestIndex;

        gfdc2 = ConvertUnsgnToSgn(ReadFDC10(gtestDTC));
        snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 is %d", gfdc2);
        TestReport_HTML_StepDescription(Stepdespriction, Info);

        // --- 逻辑判断 (保持原样) ---
        if (gfdc2 < 0)
        {
            if (gfdc2 != -128) fdcflag = 1;
            if (((gfdc1 - gfdc2 != n * gstepValue) && (gfdc2 != -128)))
            {
                if ((gfdc1 - gfdc2 == (n + 1) * gstepValue) || (gfdc1 - gfdc2 == 0)) {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepDownValue is %d, expect is %d", gfdc1 - gfdc2, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Warning);
                }
                else {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepDownValue is %d, expect is %d", gfdc1 - gfdc2, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            else
            {
                if (fdcflag)
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepDownValue is %d", gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Pass);
                }
                else
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The first negative value is -128!");
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            gfdc1 = gfdc2;
        }
        else
        {
            if (gtestIndex > 100)
            {
                snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 can't decrease! Break test.");
                TestReport_HTML_StepDescription(Stepdespriction, Fail);
                break;
            }
        }

        if (gfdc2 == -128)
        {
            snprintf(Stepdespriction, sizeof(Stepdespriction), "The minimun value of FDC10 is 0x80");
            TestReport_HTML_StepDescription(Stepdespriction, Pass);
            break;
        }
        gReadEndFlag = 1;
    }

    // --- 停止定时器 ---
    NewTimer1.stop();
    gCurrentTestMode = MODE_IDLE;
}

void CheckStepDownValueByIndex(int index,u8 dtc[], int cycle, int step)
{
    int n, fdcflag;
    char Stepdespriction[256];
    s64 nowTime = 0,lastTime = 0;

    // --- 初始化 ---
    gtestDTC[0] = dtc[0];
    gtestDTC[1] = dtc[1];
    gtestDTC[2] = dtc[2];
    gstepValue = step;
    gtestIndex = 0;
    gLastIndex = 0;
    gfdc1 = 0;
    gfdc2 = 0;
    gReadEndFlag = 1;

    printf("Start Timer");
    TestStepValueByIndex.start();
    TestStepValueByIndex.set_interval(cycle);
    app.get_timestamp(&lastTime);

    while (gtestIndex <= index && (nowTime - lastTime) < (cycle * index * 1000 + 3000000))
    {
        n = gtestIndex - gLastIndex;
        gLastIndex = gtestIndex;
        app.get_timestamp(&nowTime);
        if (n < 1)
            continue;

        snprintf(Stepdespriction, sizeof(Stepdespriction), "------------------%dth cycle test of FDC10 Step Down---------------------", gtestIndex);
        TestReport_HTML_StepDescription(Stepdespriction, Info);

        gfdc2 = ConvertUnsgnToSgn(ReadFDC10(gtestDTC));
        snprintf(Stepdespriction, sizeof(Stepdespriction), "FDC10 is %d", gfdc2);
        TestReport_HTML_StepDescription(Stepdespriction, Info);

        // --- 逻辑判断 (保持原样) ---
        if (gfdc2 < 0)
        {
            if (gfdc2 != -128) fdcflag = 1;
            if (((gfdc1 - gfdc2 != n * gstepValue) && (gfdc2 != -128)))
            {
                if ((gfdc1 - gfdc2 == (n + 1) * gstepValue) || (gfdc1 - gfdc2 == 0)) {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepDownValue is %d, expect is %d", gfdc1 - gfdc2, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Warning);
                }
                else {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepDownValue is %d, expect is %d", gfdc1 - gfdc2, gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            else
            {
                if (fdcflag)
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The StepDownValue is %d", gstepValue);
                    TestReport_HTML_StepDescription(Stepdespriction, Pass);
                }
                else
                {
                    snprintf(Stepdespriction, sizeof(Stepdespriction), "The first negative value is -128!");
                    TestReport_HTML_StepDescription(Stepdespriction, Fail);
                }
            }
            gfdc1 = gfdc2;
        }
    }
    gReadEndFlag = 0;
    TestStepValueByIndex.stop();

}


void Signal_assignment_VehSpdLgt()
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u64 value;
    int sigGroup_updatebit;
    int sg_ub;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = VehSpdLgt.sigGroup_UB + VehSpdLgt.pduStartBit;//  FR add PDU startBit

    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < VehSpdLgt.sigNum; j++)
    {
        //motorolar Encoding
        startbit = VehSpdLgt.e2eSignalList[j].startBit + VehSpdLgt.pduStartBit;// FR add PDU startBit
        length = VehSpdLgt.e2eSignalList[j].sigLength;
        value = VehSpdLgt.e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));
        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    /*------------------For when VehSpdLgt and VehMtnSt are in same frame-------------------*/
    if (VehMtnSt.slotID == VehSpdLgt.slotID)
    {
        sigGroup_updatebit = VehMtnSt.sigGroup_UB + VehMtnSt.pduStartBit;//  FR add PDU startBit

        _signal[sigGroup_updatebit] = 1;

        for (j = 0; j < VehMtnSt.sigNum; j++)
        {
            //motorolar Encoding
            startbit = VehMtnSt.e2eSignalList[j].startBit + VehMtnSt.pduStartBit;// FR add PDU startBit
            length = VehMtnSt.e2eSignalList[j].sigLength;
            value = VehMtnSt.e2eSignalList[j].sigValue;

            bit_num = startbit;
            _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));
            for (n = length - 2; n >= 0; n--)
            {
                if ((bit_num % 8) == 0)
                {
                    bit_num = bit_num + 16;
                }
                bit_num--;
                _signal[bit_num] = ((value & (0x1 << n)) >> n);
            }
        }
    }
    /*--------------------------------------------------------------------------------------*/

    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

u8 Crc_CalculateCRC8_PL(u8 data[], int dataLen, u8 Crc_StartValue8)
{
    int i, j;
    u8 polynomial;
    u8 initValue;

    polynomial = 0x1d;
    initValue = Crc_StartValue8 ^ 0XFF;
    for (i = 0; i < dataLen; i++)
    {
        initValue = initValue ^ data[i];
        for (j = 0; j < 8; j++)
        {
            if (initValue & 0x80)
            {
                initValue = ((initValue << 1) & 0xff);
                initValue = initValue ^ polynomial;
            }
            else
            {
                initValue = ((initValue << 1) & 0xff);
            }
        }
    }
    initValue = initValue ^ 0xFF;
    return initValue;
}

u8 E2E_Check(u8 dataID, u8 Data[], int DataLen)
{
    u8 crc1;
    u8 crc2;
    u8 crc3;
    u8 crc;
    u8 dataHigh[1];
    u8 dataLow[1];
    dataHigh[0] = dataID >> 8;
    dataLow[0] = dataID & 0xFF;

    crc1 = Crc_CalculateCRC8_PL(dataLow, 1, 0XFF);
    crc2 = Crc_CalculateCRC8_PL(dataHigh, 1, crc1);
    crc3 = Crc_CalculateCRC8_PL(Data, DataLen, crc2);

    crc = crc3 ^ 0xff;
    return crc;
}

void Signal_assignment_CANFD_VehBattU()
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    int value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = VehBattU.sigGroup_UB;//  FR add PDU startBit
    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < VehBattU.sigNum; j++)
    {
        //motorolar Encoding
        startbit = VehBattU.e2eSignalList[j].startBit;// FR add PDU startBit
        length = VehBattU.e2eSignalList[j].sigLength;
        value = VehBattU.e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }
    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void Signal_assignment_CANFD_UsageMode()
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    int value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = VehGlbSafe.sigGroup_UB;//  FR add PDU startBit
    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < VehGlbSafe.sigNum; j++)
    {
        //motorolar Encoding
        startbit = VehGlbSafe.e2eSignalList[j].startBit;// FR add PDU startBit
        length = VehGlbSafe.e2eSignalList[j].sigLength;
        value = VehGlbSafe.e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }
    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void Signal_assignment_DrvgCyc(int index)
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    int value;
    int sigGroup_updatebit;
    int sg_ub;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = DrvgAndMonrCyc[index].updateBit;//  update bit

    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < 1; j++)
    {
        //motorolar Encoding
        startbit = DrvgAndMonrCyc[index].startBit;// signal startBit
        length = 1;
        value = DrvgAndMonrCyc[index].value;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));
        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    if (index == 1)
    {
        sigGroup_updatebit = DrvgAndMonrCyc[2].updateBit;//  update bit

        _signal[sigGroup_updatebit] = 1;

        //motorolar Encoding
        startbit = DrvgAndMonrCyc[2].startBit;// signal startBit
        length = 1;
        value = DrvgAndMonrCyc[2].value;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));
        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

//void SimVehMtnGlbSafeCANFD()
//{
//    int i, j, n;
//    int sigLength;
//    int sigLength_d;
//    int sigLength_m;
//    u8 Data[100];
//    int dataLen;
//    u8 dataID;
//    u8 CRC;
//    //信号值初始化
//    for (j = 2; j < VehGlbSafe.sigNum; j++)
//    {
//        VehGlbSafe.e2eSignalList[j].sigValue = 0x0;
//    }
//    VehGlbSafe.e2eSignalList[2].sigValue = gCarMode;
//    VehGlbSafe.e2eSignalList[7].sigValue = gEPLvl;
//    VehGlbSafe.e2eSignalList[9].sigValue = gUagMode;
//
//
//    if (counterVehMnGlSa == 0xF)
//    {
//        counterVehMnGlSa = 0;
//    }
//
//    Data[0] = counterVehMnGlSa;
//    dataLen = 1;
//
//    for (j = 2; j < VehGlbSafe.sigNum; j++)
//    {
//        sigLength = VehGlbSafe.e2eSignalList[j].sigLength;
//        sigLength_d = sigLength / 8;
//        sigLength_m = sigLength % 8;
//        if (sigLength_m > 0)
//        {
//            sigLength_d = sigLength_d + 1;
//        }
//
//        for (n = 0; n < sigLength_d; n++)
//        {
//            Data[dataLen] = VehGlbSafe.e2eSignalList[j].sigValue >> (n * 8);
//
//            dataLen++;
//        }
//    }
//
//    dataID = VehGlbSafe.dataID;
//    CRC = E2E_Check(dataID, Data, dataLen);
//
//    VehGlbSafe.e2eSignalList[0].sigValue = CRC;
//    VehGlbSafe.e2eSignalList[1].sigValue = counterVehMnGlSa;
//
//    Signal_assignment_CANFD_UsageMode();
//
//    //Sim_UsgMode_CANFD.FIdxChn = NetworkCluster[VehGlbSafe.cluster];
//    Sim_UsgMode_CANFD.FIdxChn = CH3;
//    Sim_UsgMode_CANFD.FIdentifier = VehGlbSafe.slotID;
//    Sim_UsgMode_CANFD.FDLC = 15;
//    Sim_UsgMode_CANFD.FFDProperties = 0x1;  //isfd
//    Sim_UsgMode_CANFD.set_is_brs(false);
//    for (i = 0; i < VehGlbSafe.dlc; i++)
//    {
//        Sim_UsgMode_CANFD.FData[i] = Assignment_data[i];
//    }
//    //com.add_cyclic_message_canfd(&Sim_UsgMode_CANFD, 10);
//
//    counterVehMnGlSa++;
//}

void SimVehMtnGlbSafeCANFD()
{
    StartCANRBS(&g_VehModMngtGlbSafe1_path);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_VehModMngtGlbSafe1_path.FDBName], g_VehModMngtGlbSafe1_path.FDBName, g_VehModMngtGlbSafe1_path.FECUName, g_VehModMngtGlbSafe1_path.FMsgName, "VehModMngtGlbSafe1_UB", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_VehModMngtGlbSafe1_path.FDBName], g_VehModMngtGlbSafe1_path.FDBName, g_VehModMngtGlbSafe1_path.FECUName, g_VehModMngtGlbSafe1_path.FMsgName, "VehModMngtGlbSafe1CarModSts1", gCarMode);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_VehModMngtGlbSafe1_path.FDBName], g_VehModMngtGlbSafe1_path.FDBName, g_VehModMngtGlbSafe1_path.FECUName, g_VehModMngtGlbSafe1_path.FMsgName, "VehModMngtGlbSafe1PwrLvlElecMai", gEPLvl);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_VehModMngtGlbSafe1_path.FDBName], g_VehModMngtGlbSafe1_path.FDBName, g_VehModMngtGlbSafe1_path.FECUName, g_VehModMngtGlbSafe1_path.FMsgName, g_VehModMngtGlbSafe1_path.FSignalName, gUagMode);
}


void SimVehMtnGlbSafeCANFD_Creat() {
    SimVehMtnGlbSafeCANFD();
    //com.add_cyclic_message_canfd(&Sim_UsgMode_CANFD, 10);
    //printf("Add SimVehMtnGlbSafeCANFD Cyclic Message,Channel is %d,ID is %d",Sim_UsgMode_CANFD.FIdxChn,Sim_UsgMode_CANFD.FIdentifier);
}

void SimVehMtnSpdCANFD()
{
    int i, j, n;
    int sigLength;
    int sigLength_d;
    int sigLength_m;
    u8 Data[100];
    int dataLen;
    u8 dataID;
    u8 CRC;

    if (VehSpdLgt.slotID == VehModMngtGlbSafe[0].slotID)return;
    if (strncmp(VehSpdLgt.cluster, "", 50) == 0)return;

    for (j = 2; j < VehSpdLgt.sigNum; j++)
    {
        VehSpdLgt.e2eSignalList[j].sigValue = 0x0;
    }
    VehSpdLgt.e2eSignalList[3].sigValue = 0x3;
    VehSpdLgt.e2eSignalList[2].sigValue = gVehSpd;

    if (counterVehSpd == 0xF)
    {
        counterVehSpd = 0;
    }

    Data[0] = counterVehSpd;////
    dataLen = 1;

    for (j = 2; j < VehSpdLgt.sigNum; j++)
    {
        sigLength = VehSpdLgt.e2eSignalList[j].sigLength;
        sigLength_d = sigLength / 8;
        sigLength_m = sigLength % 8;
        if (sigLength_m > 0)
        {
            sigLength_d = sigLength_d + 1;
        }

        for (n = 0; n < sigLength_d; n++)
        {
            Data[dataLen] = VehSpdLgt.e2eSignalList[j].sigValue >> (n * 8);

            dataLen++;
        }
    }

    dataID = VehSpdLgt.dataID;
    CRC = E2E_Check(dataID, Data, dataLen);

    VehSpdLgt.e2eSignalList[0].sigValue = CRC;
    VehSpdLgt.e2eSignalList[1].sigValue = counterVehSpd;

    /*------------------For when VehSpdLgt and VehMtnSt are in same frame-------------------*/
    if (VehMtnSt.slotID == VehSpdLgt.slotID)
    {
        for (j = 2; j < VehMtnSt.sigNum; j++)
        {
            VehMtnSt.e2eSignalList[j].sigValue = 0x0;
        }
        VehMtnSt.e2eSignalList[2].sigValue = 0x3;

        if (counterVehMn == 0xF)
        {
            counterVehMn = 0;
        }

        Data[0] = counterVehMn;////
        dataLen = 1;

        for (j = 2; j < VehMtnSt.sigNum; j++)
        {
            sigLength = VehMtnSt.e2eSignalList[j].sigLength;
            sigLength_d = sigLength / 8;
            sigLength_m = sigLength % 8;
            if (sigLength_m > 0)
            {
                sigLength_d = sigLength_d + 1;
            }

            for (n = 0; n < sigLength_d; n++)
            {
                Data[dataLen] = VehMtnSt.e2eSignalList[j].sigValue >> (n * 8);

                dataLen++;
            }
        }

        dataID = VehMtnSt.dataID;
        CRC = E2E_Check(dataID, Data, dataLen);

        VehMtnSt.e2eSignalList[0].sigValue = CRC;
        VehMtnSt.e2eSignalList[1].sigValue = counterVehMn;

        counterVehMn++;
    }
    /*--------------------------------------------------------------------------------------*/

    Signal_assignment_VehSpdLgt();////
    Sim_VehSpd_CANFD.FIdxChn = NetworkCluster[VehSpdLgt.cluster];
    Sim_VehSpd_CANFD.FIdentifier = VehSpdLgt.slotID;
    Sim_VehSpd_CANFD.FDLC = 15;
    Sim_VehSpd_CANFD.FFDProperties = 0x1;  //isfd
    for (i = 0; i < VehSpdLgt.dlc; i++)
    {
        Sim_VehSpd_CANFD.FData[i] = Assignment_data[i];
    }
    //com.add_cyclic_message_canfd(&Sim_VehSpd_CANFD, 50);
    counterVehSpd++;
}

void SimVehMtnSpdCANFD_Creat()
{
    com.add_cyclic_message_canfd(&Sim_VehSpd_CANFD, 50);
}

void SimDrvgCycAndWarmUpCyc(int drvgValue, int monrValue)
{
    int i;
    if (gNxtOpCycCndtn == 1)
    {
        if (drvgValue == 1)
        {
            gDrvgCycMsg.FIdxChn = NetworkCluster[DrvgAndMonrCyc[0].cluster];
            gDrvgCycMsg.FIdentifier = DrvgAndMonrCyc[0].msgId;
            DrvgAndMonrCyc[0].value = drvgValue;
            Signal_assignment_DrvgCyc(0);
            for (i = 0; i < 8; i++)
            {
                gDrvgCycMsg.FData[i] = Assignment_data[i];
            }
            com.add_cyclic_message_can(&gDrvgCycMsg,100);
            AppWait(2000, "");

            gMonrCycCmpl.FIdxChn = NetworkCluster[DrvgAndMonrCyc[1].cluster];
            gMonrCycCmpl.FIdentifier = DrvgAndMonrCyc[1].msgId;
            DrvgAndMonrCyc[1].value = monrValue;
            DrvgAndMonrCyc[2].value = monrValue;
            Signal_assignment_DrvgCyc(1);
            for (i = 0; i < 8; i++)
            {
                gMonrCycCmpl.FData[i] = Assignment_data[i];
            }
            com.add_cyclic_message_can(&gMonrCycCmpl, 300);
        }
        else
        {
            gMonrCycCmpl.FIdxChn = NetworkCluster[DrvgAndMonrCyc[1].cluster];
            gMonrCycCmpl.FIdentifier = DrvgAndMonrCyc[1].msgId;
            DrvgAndMonrCyc[1].value = monrValue;
            DrvgAndMonrCyc[2].value = monrValue;
            Signal_assignment_DrvgCyc(1);
            for (i = 0; i < 8; i++)
            {
                gMonrCycCmpl.FData[i] = Assignment_data[i];
            }
            com.add_cyclic_message_can(&gMonrCycCmpl, 300);

            AppWait(2000,"");

            gDrvgCycMsg.FIdxChn = NetworkCluster[DrvgAndMonrCyc[0].cluster];
            gDrvgCycMsg.FIdentifier = DrvgAndMonrCyc[0].msgId;
            DrvgAndMonrCyc[0].value = drvgValue;
            Signal_assignment_DrvgCyc(0);
            for (i = 0; i < 8; i++)
            {
                gDrvgCycMsg.FData[i] = Assignment_data[i];
            }
            com.add_cyclic_message_can(&gDrvgCycMsg, 100);
        }
    }

}

void Signal_assignment_CAN(int FrameIndex)
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u64 value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = VehModMngtGlbSafe[FrameIndex].sigGroup_UB;//  FR add PDU startBit
    if (FrameIndex == 0)
        _signal[sigGroup_updatebit] = gVehManUB;
    else
        _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < VehModMngtGlbSafe[FrameIndex].sigNum; j++)
    {
        //motorolar Encoding
        startbit = VehModMngtGlbSafe[FrameIndex].e2eSignalList[j].startBit;// FR add PDU startBit
        length = VehModMngtGlbSafe[FrameIndex].e2eSignalList[j].sigLength;
        value = VehModMngtGlbSafe[FrameIndex].e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    /*------------------------------------For when BkpOfDstTrvld and Ubat are in the same frame-------------------------------------------------------*/
    if ((BkpOfDstTrvld.msgId == VehModMngtGlbSafe[1].slotID && FrameIndex == 1) && gBkpOfDstTrvldSimFlag == 1)
    {
        sigGroup_updatebit = BkpOfDstTrvld.updateBit;
        _signal[sigGroup_updatebit] = 1;

        for (j = 0; j < 1; j++)
        {
            //motorolar Encoding
            startbit = BkpOfDstTrvld.startBit;// FR add PDU startBit
            length = 21;
            value = BkpOfDstTrvld.value;

            bit_num = startbit;
            _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

            for (n = length - 2; n >= 0; n--)
            {
                if ((bit_num % 8) == 0)
                {
                    bit_num = bit_num + 16;
                }
                bit_num--;
                _signal[bit_num] = ((value & (0x1 << n)) >> n);
            }
        }
    }
    /*-------------------------------------------------------------------------------------------------------------------------------------------*/
    /*-------------if missMsg is vmm frame or ubat frame------------------*/
    if (!FrameIndex)
    {
        for (long bit : gVmmFrameSetUB)
        {
            long bitnum;
            bitnum = bit;
            _signal[bitnum] = gVmmFrameSetUB[bit];
        }
    }
    else
    {
        for (long bit : gUbatFrameSetUB)
        {
            long bitnum;
            bitnum = bit;
            _signal[bitnum] = gUbatFrameSetUB[bit];
        }
    }
    /*--------------------------------------------------------------------*/
    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void Signal_assignment_CANFD(int FrameIndex)
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u64 value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = VehModMngtGlbSafe[FrameIndex].sigGroup_UB;//  FR add PDU startBit
    if (FrameIndex == 0)
        _signal[sigGroup_updatebit] = gVehManUB;
    else
        _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < VehModMngtGlbSafe[FrameIndex].sigNum; j++)
    {
        //motorolar Encoding
        startbit = VehModMngtGlbSafe[FrameIndex].e2eSignalList[j].startBit;// FR add PDU startBit
        length = VehModMngtGlbSafe[FrameIndex].e2eSignalList[j].sigLength;
        value = VehModMngtGlbSafe[FrameIndex].e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }
    // for SafetyCANFD3, Ubat and VMM is contained in the same frame.
    if (((VehModMngtGlbSafe[1].slotID == VehModMngtGlbSafe[0].slotID) && (VehModMngtGlbSafe[1].baseCycle == VehModMngtGlbSafe[0].baseCycle) && (VehModMngtGlbSafe[1].RepetitionCycle == VehModMngtGlbSafe[0].RepetitionCycle)) && (FrameIndex == 0))
    {
        sigGroup_updatebit = VehModMngtGlbSafe[1].sigGroup_UB + VehModMngtGlbSafe[1].pduStartBit;//  FR add PDU startBit
        _signal[sigGroup_updatebit] = 1;
        for (j = 0; j < VehModMngtGlbSafe[1].sigNum; j++)
        {
            //motorolar Encoding
            startbit = VehModMngtGlbSafe[1].e2eSignalList[j].startBit + VehModMngtGlbSafe[1].pduStartBit;// FR add PDU startBit
            length = VehModMngtGlbSafe[1].e2eSignalList[j].sigLength;
            value = VehModMngtGlbSafe[1].e2eSignalList[j].sigValue;

            bit_num = startbit;
            _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));
            for (n = length - 2; n >= 0; n--)
            {
                if ((bit_num % 8) == 0)
                {
                    bit_num = bit_num + 16;
                }
                bit_num--;
                _signal[bit_num] = ((value & (0x1 << n)) >> n);
            }
        }
    }

    /*------------------------------------For when VehSpdLgt and VMM are in the same frame-------------------------------------------------------*/
    if (gVehSpdSimFlag)
    {
        if ((VehSpdLgt.slotID == VehModMngtGlbSafe[0].slotID) && (FrameIndex == 0))
        {
            sigGroup_updatebit = VehSpdLgt.sigGroup_UB + VehSpdLgt.pduStartBit;//  FR add PDU startBit

            _signal[sigGroup_updatebit] = 1;

            for (j = 0; j < VehSpdLgt.sigNum; j++)
            {
                //motorolar Encoding
                startbit = VehSpdLgt.e2eSignalList[j].startBit + VehSpdLgt.pduStartBit;// FR add PDU startBit
                length = VehSpdLgt.e2eSignalList[j].sigLength;
                value = VehSpdLgt.e2eSignalList[j].sigValue;

                bit_num = startbit;
                _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));
                for (n = length - 2; n >= 0; n--)
                {
                    if ((bit_num % 8) == 0)
                    {
                        bit_num = bit_num + 16;
                    }
                    bit_num--;
                    _signal[bit_num] = ((value & (0x1 << n)) >> n);
                }
            }
        }
    }
    /*------------------------------------For when CarTiGlb and VMM are in the same frame-------------------------------------------------------*/
    if (((CarTiGlb.msgId == VehModMngtGlbSafe[0].slotID && FrameIndex == 0) || (CarTiGlb.msgId == VehModMngtGlbSafe[1].slotID && FrameIndex == 1)) && gCarTiGlbSimFlag == 1)
    {
        sigGroup_updatebit = CarTiGlb.updateBit;
        _signal[sigGroup_updatebit] = 1;

        for (j = 0; j < 1; j++)
        {
            //motorolar Encoding
            startbit = CarTiGlb.startBit;// FR add PDU startBit
            length = 32;
            value = CarTiGlb.value;

            bit_num = startbit;
            _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

            for (n = length - 2; n >= 0; n--)
            {
                if ((bit_num % 8) == 0)
                {
                    bit_num = bit_num + 16;
                }
                bit_num--;
                _signal[bit_num] = ((value & (0x1 << n)) >> n);
            }
        }
    }
    /*------------------------------------For when BkpOfDstTrvld and VMM are in the same frame-------------------------------------------------------*/
    if (((BkpOfDstTrvld.msgId == VehModMngtGlbSafe[0].slotID && FrameIndex == 0) || (BkpOfDstTrvld.msgId == VehModMngtGlbSafe[1].slotID && FrameIndex == 1)) && gBkpOfDstTrvldSimFlag == 1)
    {
        sigGroup_updatebit = BkpOfDstTrvld.updateBit;
        _signal[sigGroup_updatebit] = 1;

        for (j = 0; j < 1; j++)
        {
            //motorolar Encoding
            startbit = BkpOfDstTrvld.startBit;// FR add PDU startBit
            length = 21;
            value = BkpOfDstTrvld.value;

            bit_num = startbit;
            _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

            for (n = length - 2; n >= 0; n--)
            {
                if ((bit_num % 8) == 0)
                {
                    bit_num = bit_num + 16;
                }
                bit_num--;
                _signal[bit_num] = ((value & (0x1 << n)) >> n);
            }
        }
    }
    /*-------------------------------------------------------------------------------------------------------------------------------------------*/
    /*-------------if missMsg is vmm frame or ubat frame------------------*/
    if (!FrameIndex)
    {
        for (long bit : gVmmFrameSetUB)
        {
            long bitnum;
            bitnum = bit;
            _signal[bitnum] = gVmmFrameSetUB[bit];
        }
    }
    else
    {
        for (long bit : gUbatFrameSetUB)
        {
            long bitnum;
            bitnum = bit;
            _signal[bitnum] = gUbatFrameSetUB[bit];
        }
    }
    /*--------------------------------------------------------------------*/
    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void Signal_assignment_EngSts(int index)
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u64 value;
    int sigGroup_updatebit;
    int sg_ub;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = EngSts[index].sigGroup_UB + EngSts[index].pduStartBit;//  FR add PDU startBit

    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < EngSts[index].sigNum; j++)
    {
        //motorolar Encoding
        startbit = EngSts[index].e2eSignalList[j].startBit + EngSts[index].pduStartBit;// FR add PDU startBit
        length = EngSts[index].e2eSignalList[j].sigLength;
        value = EngSts[index].e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));
        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void Signal_assignment_CarTiGlb()
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u64 value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = CarTiGlb.updateBit;
    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < 1; j++)
    {
        //motorolar Encoding
        startbit = CarTiGlb.startBit;// FR add PDU startBit
        length = 32;
        value = CarTiGlb.value;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    /*-------------------------------------------------------------*/
    if (BkpOfDstTrvld.msgId == CarTiGlb.msgId)
    {
        sigGroup_updatebit = BkpOfDstTrvld.updateBit;
        _signal[sigGroup_updatebit] = 1;

        for (j = 0; j < 1; j++)
        {
            //motorolar Encoding
            startbit = BkpOfDstTrvld.startBit;// FR add PDU startBit
            length = 21;
            value = BkpOfDstTrvld.value;

            bit_num = startbit;
            _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

            for (n = length - 2; n >= 0; n--)
            {
                if ((bit_num % 8) == 0)
                {
                    bit_num = bit_num + 16;
                }
                bit_num--;
                _signal[bit_num] = ((value & (0x1 << n)) >> n);
            }
        }
    }
    /*-------------------------------------------------------------*/

    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void Signal_assignment_BkpOfDstTrvld()
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u64 value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = BkpOfDstTrvld.updateBit;
    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < 1; j++)
    {
        //motorolar Encoding
        startbit = BkpOfDstTrvld.startBit;// FR add PDU startBit
        length = 21;
        value = BkpOfDstTrvld.value;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void Signal_assignment_TiAndDateIndn()
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u64 value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = TiAndDateIndn.sigGroup_UB;
    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < TiAndDateIndn.sigNum; j++)
    {
        //motorolar Encoding
        startbit = TiAndDateIndn.e2eSignalList[j].startBit;// FR add PDU startBit
        length = TiAndDateIndn.e2eSignalList[j].sigLength;
        value = TiAndDateIndn.e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void SimVehBatOnCAN()
{
    int i, j, n;

    Sim_VehBat_CAN.FIdxChn = NetworkCluster[VehBattU.cluster];
    Sim_VehBat_CAN.FIdentifier = VehBattU.slotID;
    Sim_VehBat_CAN.FDLC = VehBattU.dlc;

    for (j = 0; j < VehBattU.sigNum; j++)
    {
        VehBattU.e2eSignalList[j].sigValue = 0x0;
    }
    VehBattU.e2eSignalList[0].sigValue = gVehBatVotg;
    VehBattU.e2eSignalList[1].sigValue = gVehBatQf;

    Signal_assignment_CAN(1);
    for (i = 0; i < VehBattU.dlc; i++)
    {
        Sim_VehBat_CAN.FData[i] = Assignment_data[i];
    }
    //com.transmit_can_async(&Sim_VehBat_CAN);
}

void SimVehBatOnCAN_Creat() {
    SimVehBatOnCAN();
    com.add_cyclic_message_can(&Sim_VehBat_CAN, VehBattU.FrameCycle);
    printf("Add Sim_VehBat_CAN Cyclic Message,Channel is %d,ID is %d", Sim_VehBat_CAN.FIdxChn, Sim_VehBat_CAN.FIdentifier);
}

void SimVehBatOnCANFD()
{
    int i, j, n;
    //Sim_VehBat_CANFD.FIdxChn = NetworkCluster[VehBattU.cluster];
    Sim_VehBat_CANFD.FIdxChn = CH3;
    Sim_VehBat_CANFD.FIdentifier = VehBattU.slotID;
    Sim_VehBat_CANFD.FDLC = 15;
    Sim_VehBat_CANFD.FFDProperties = 0x1;  //isfd
    Sim_VehBat_CANFD.set_is_brs(false);

    for (j = 0; j < VehBattU.sigNum; j++)
    {
        VehBattU.e2eSignalList[j].sigValue = 0x0;
    }
    VehBattU.e2eSignalList[0].sigValue = gVehBatVotg;
    VehBattU.e2eSignalList[1].sigValue = 3;

    Signal_assignment_CANFD_VehBattU();
    for (i = 0; i < VehBattU.dlc; i++)
    {
        Sim_VehBat_CANFD.FData[i] = Assignment_data[i];
    }
    Sim_VehBat_CANFD.FData[0] = 0x3;
    //com.add_cyclic_message_canfd(&Sim_VehBat_CANFD, VehBattU.FrameCycle);
}

void SimVehBatOnCANFD_Creat() {
    SimVehBatOnCANFD();
    com.add_cyclic_message_canfd(&Sim_VehBat_CANFD, VehBattU.FrameCycle);
    printf("Add Sim_VehBat_CANFD Cyclic Message,Channel is %d,ID is %d", Sim_VehBat_CANFD.FIdxChn, Sim_VehBat_CANFD.FIdentifier);
}

void SetCarTiGlb()
{
    char busname[100];
    int i, j, n;

    Sim_CarTiGlb.FIdxChn = NetworkCluster[CarTiGlb.cluster];
    Sim_CarTiGlb.FIdentifier = CarTiGlb.msgId;
    Sim_CarTiGlb.FDLC = CANFD_DLC[CarTiGlb.msgdatalen];

    CarTiGlb.value = gCarTiGlb;

    /*--------------------------------*/
    if (BkpOfDstTrvld.msgId == CarTiGlb.msgId)
        BkpOfDstTrvld.value = gBkpOfDstTrvld;
    /*--------------------------------*/

    Signal_assignment_CarTiGlb();
    for (i = 0; i < CarTiGlb.msgdatalen; i++)
    {
        Sim_CarTiGlb.FData[i] = Assignment_data[i];
    }

    strncpy(busname, CarTiGlb.cluster, 100);
    for (int i = 0; busname[i] != '\0'; i++) {
        busname[i] = toupper(busname[i]);
    }
    if (strstr(busname, "FD") != NULL) {
        Sim_CarTiGlb.set_is_edl(true); //设置FD报文
        Sim_CarTiGlb.set_is_brs(true); //设置BRS
    }

    com.add_cyclic_message_canfd(&Sim_CarTiGlb, 50);
}

void SetBkpOfDstTrvld()
{
    char busname[100];
    int i, j, n;

    Sim_BkpOfDstTrvld.FIdxChn = NetworkCluster[BkpOfDstTrvld.cluster];
    Sim_BkpOfDstTrvld.FIdentifier = BkpOfDstTrvld.msgId;
    Sim_BkpOfDstTrvld.FDLC = CANFD_DLC[BkpOfDstTrvld.msgdatalen];

    BkpOfDstTrvld.value = gBkpOfDstTrvld;

    Signal_assignment_BkpOfDstTrvld();
    for (i = 0; i < BkpOfDstTrvld.msgdatalen; i++)
    {
        Sim_BkpOfDstTrvld.FData[i] = Assignment_data[i];
    }
    strncpy(busname, BkpOfDstTrvld.cluster, 100);
    for (int i = 0; busname[i] != '\0'; i++) {
        busname[i] = toupper(busname[i]);
    }
    if (strstr(busname, "FD") != NULL)
    {
        Sim_BkpOfDstTrvld.set_is_edl(true); //设置FD报文
        Sim_BkpOfDstTrvld.set_is_brs(true); //设置BRS
    }
    com.add_cyclic_message_canfd(&Sim_BkpOfDstTrvld, 50);
}

void SetTiAndDateIndn()
{
    char busname[100];
    int i, j, n;

    Sim_TiAndDateIndn.FIdxChn = NetworkCluster[TiAndDateIndn.cluster];
    Sim_TiAndDateIndn.FIdentifier = TiAndDateIndn.slotID;
    Sim_TiAndDateIndn.FDLC = CANFD_DLC[TiAndDateIndn.dlc];

    for (j = 0; j < TiAndDateIndn.sigNum; j++)
    {
        TiAndDateIndn.e2eSignalList[j].sigValue = 0x0;
    }

    TiAndDateIndn.e2eSignalList[6].sigValue = 1;//dataValid
    TiAndDateIndn.e2eSignalList[1].sigValue = gTiAndDateIndn[0];//year
    TiAndDateIndn.e2eSignalList[3].sigValue = gTiAndDateIndn[1];//month
    TiAndDateIndn.e2eSignalList[0].sigValue = gTiAndDateIndn[2];//day
    TiAndDateIndn.e2eSignalList[4].sigValue = gTiAndDateIndn[3];//hour
    TiAndDateIndn.e2eSignalList[2].sigValue = gTiAndDateIndn[4];//minute
    TiAndDateIndn.e2eSignalList[5].sigValue = gTiAndDateIndn[5];//second

    Signal_assignment_TiAndDateIndn();
    for (i = 0; i < TiAndDateIndn.dlc; i++)
    {
        Sim_TiAndDateIndn.FData[i] = Assignment_data[i];
    }

    strncpy(busname, TiAndDateIndn.cluster, 100);
    for (int i = 0; busname[i] != '\0'; i++) {
        busname[i] = toupper(busname[i]);
    }
    if (strstr(busname, "FD") != NULL)
    {
        Sim_TiAndDateIndn.set_is_edl(true); //设置FD报文
        Sim_TiAndDateIndn.set_is_brs(true); //设置BRS
    }
    if (TiAndDateIndn.slotID > 0)
    {
        com.add_cyclic_message_canfd(&Sim_TiAndDateIndn,50);
    }
}

void SimCarTiGlb()
{
    if (CarTiGlb.msgId == VehModMngtGlbSafe[0].slotID)
    {
        gCarTiGlbSimFlag = 1;
        return;
    }

    if (strncmp(CarTiGlb.cluster, "", 100) == 0)return;

    SetCarTiGlb();
}

void SimBkpOfDstTrvld()
{
    if (BkpOfDstTrvld.msgId == VehModMngtGlbSafe[0].slotID || BkpOfDstTrvld.msgId == VehModMngtGlbSafe[1].slotID)
    {
        gBkpOfDstTrvldSimFlag = 1;
        return;
    }
    if (BkpOfDstTrvld.msgId == CarTiGlb.msgId)return;

    if (strncmp(BkpOfDstTrvld.cluster, "", 100) == 0)return;

    SetBkpOfDstTrvld();
}

void SimTiAndDateIndn()
{
    if (strncmp(TiAndDateIndn.cluster, "", 100) == 0)return;

    SetTiAndDateIndn();
}

//void SimVehMtnSpd()
//{
//    int i, j, n;
//    int sigLength;
//    int sigLength_d;
//    int sigLength_m;
//    byte Data[100];
//    int dataLen;
//    u16 dataID;
//    byte CRC;
//
//    if (VehSpdLgt.slotID == VehModMngtGlbSafe[0].slotID)return;
//    if (strncmp(VehSpdLgt.cluster, "", 50) == 0)return;
//
//    for (j = 2; j < VehSpdLgt.sigNum; j++)
//    {
//        VehSpdLgt.e2eSignalList[j].sigValue = 0x0;
//    }
//    VehSpdLgt.e2eSignalList[3].sigValue = 0x3;
//    VehSpdLgt.e2eSignalList[2].sigValue = gVehSpd;
//
//    if (counterVehSpd == 0xF)
//    {
//        counterVehSpd = 0;
//    }
//
//    Data[0] = counterVehSpd;////
//    dataLen = 1;
//
//    for (j = 2; j < VehSpdLgt.sigNum; j++)
//    {
//        sigLength = VehSpdLgt.e2eSignalList[j].sigLength;
//        sigLength_d = sigLength / 8;
//        sigLength_m = sigLength % 8;
//        if (sigLength_m > 0)
//        {
//            sigLength_d = sigLength_d + 1;
//        }
//
//        for (n = 0; n < sigLength_d; n++)
//        {
//            Data[dataLen] = VehSpdLgt.e2eSignalList[j].sigValue >> (n * 8);
//
//            dataLen++;
//        }
//    }
//
//    dataID = VehSpdLgt.dataID;
//    CRC = E2E_Check(dataID, Data, dataLen);
//
//    VehSpdLgt.e2eSignalList[0].sigValue = CRC;
//    VehSpdLgt.e2eSignalList[1].sigValue = counterVehSpd;
//
//    /*------------------For when VehSpdLgt and VehMtnSt are in same frame-------------------*/
//    if (VehMtnSt.slotID == VehSpdLgt.slotID)
//    {
//        for (j = 2; j < VehMtnSt.sigNum; j++)
//        {
//            VehMtnSt.e2eSignalList[j].sigValue = 0x0;
//        }
//        VehMtnSt.e2eSignalList[2].sigValue = 0x3;
//
//        if (counterVehMn == 0xF)
//        {
//            counterVehMn = 0;
//        }
//
//        Data[0] = counterVehMn;////
//        dataLen = 1;
//
//        for (j = 2; j < VehMtnSt.sigNum; j++)
//        {
//            sigLength = VehMtnSt.e2eSignalList[j].sigLength;
//            sigLength_d = sigLength / 8;
//            sigLength_m = sigLength % 8;
//            if (sigLength_m > 0)
//            {
//                sigLength_d = sigLength_d + 1;
//            }
//
//            for (n = 0; n < sigLength_d; n++)
//            {
//                Data[dataLen] = VehMtnSt.e2eSignalList[j].sigValue >> (n * 8);
//
//                dataLen++;
//            }
//        }
//
//        dataID = VehMtnSt.dataID;
//        CRC = E2E_Check(dataID, Data, dataLen);
//
//        VehMtnSt.e2eSignalList[0].sigValue = CRC;
//        VehMtnSt.e2eSignalList[1].sigValue = counterVehMn;
//
//        counterVehMn++;
//    }
//    /*--------------------------------------------------------------------------------------*/
//
//    Signal_assignment_VehSpdLgt();////
//
//    if (gVehSpdfdFlag)
//    {
//        Sim_VehSpd.set_is_brs(false);
//        Sim_VehSpd.set_is_edl(true);
//    }
//    else
//    {
//        Sim_VehSpd.set_is_brs(false);
//        Sim_VehSpd.set_is_edl(false);
//    }
//    Sim_VehSpd.FIdxChn = NetworkCluster[VehSpdLgt.cluster];
//    Sim_VehSpd.FIdentifier = VehSpdLgt.slotID;
//    Sim_VehSpd.FDLC = CANFD_DLC[VehSpdLgt.dlc];
//    Sim_VehSpd.set_is_brs(false);
//    for (i = 0; i < VehSpdLgt.dlc; i++)
//    {
//        Sim_VehSpd.FData[i] = Assignment_data[i];
//    }
//    counterVehSpd++;
//}

void SimVehMtnSpd() {
    StartCANRBS(&g_VehMtnSt_path);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_VehMtnSt_path.FDBName], g_VehMtnSt_path.FDBName, g_VehMtnSt_path.FECUName, g_VehMtnSt_path.FMsgName, "VehMtnSt_UB", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_VehMtnSt_path.FDBName], g_VehMtnSt_path.FDBName, g_VehMtnSt_path.FECUName, g_VehMtnSt_path.FMsgName, g_VehMtnSt_path.FSignalName, 3);

}

void SimVehMtnSpd_Creat() {
    SimVehMtnSpd();
    //com.add_cyclic_message_canfd(&Sim_VehSpd,50);
}

void SimVehBat(u8 voltage)
{
    gVehBatVotg = voltage;
    StartCANRBS(&g_VehUbat_path);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_VehUbat_path.FDBName], g_VehUbat_path.FDBName, g_VehUbat_path.FECUName, g_VehUbat_path.FMsgName, g_VehUbat_path.FSignalName, gVehBatVotg);
    //// 检查 cluster 字符串中是否包含 "CANFD"
    //if (strstr(VehBattU.cluster, "CANFD") != NULL) {
    //    gTRCfdFlag = 1;
    //}
    //else {
    //    gTRCfdFlag = 0;
    //}
    //if (gTRCfdFlag)
    //    SimVehBatOnCANFD_Creat();
    //else
    //    SimVehBatOnCAN_Creat();
}

//void SimEngStsCAN()
//{
//    int i, j, n;
//    int sigLength;
//    int sigLength_d;
//    int sigLength_m;
//    u8 Data[100];
//    int dataLen;
//    u32 dataID;
//    u32 CRC;
//    for (j = 2; j < EngSts[0].sigNum; j++)
//    {
//        EngSts[0].e2eSignalList[j].sigValue = 0x0;
//    }
//    EngSts[0].e2eSignalList[2].sigValue = 0x1;
//
//    if (cntrEngSts == 0xF)
//    {
//        cntrEngSts = 0;
//    }
//
//    Data[0] = cntrEngSts;////
//    dataLen = 1;
//
//    for (j = 2; j < EngSts[0].sigNum; j++)
//    {
//        sigLength = EngSts[0].e2eSignalList[j].sigLength;
//        sigLength_d = sigLength / 8;
//        sigLength_m = sigLength % 8;
//        if (sigLength_m > 0)
//        {
//            sigLength_d = sigLength_d + 1;
//        }
//
//        for (n = 0; n < sigLength_d; n++)
//        {
//            Data[dataLen] = EngSts[0].e2eSignalList[j].sigValue >> (n * 8);
//
//            dataLen++;
//        }
//    }
//
//    dataID = EngSts[0].dataID;
//    CRC = E2E_Check(dataID, Data, dataLen);
//
//    EngSts[0].e2eSignalList[0].sigValue = CRC;
//    EngSts[0].e2eSignalList[2].sigValue = cntrEngSts;
//
//    Signal_assignment_EngSts(0);////
//    if (gCEMFlg)
//    {
//        Sim_EngSts_CAN.FIdxChn = NetworkCluster[EngSts[0].cluster];
//        Sim_EngSts_CAN.FIdentifier = EngSts[0].slotID;
//        Sim_EngSts_CAN.FDLC = EngSts[0].dlc;
//        for (i = 0; i < EngSts[0].dlc; i++)
//        {
//            Sim_EngSts_CAN.FData[i] = Assignment_data[i];
//        }
//        //com.add_cyclic_message_can(&Sim_EngSts_CAN, 50);
//    }
//
//    cntrEngSts++;
//}

void SimEngStsCAN() {
    StartCANRBS(&g_EngSt1WdSts_path);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_EngSt1WdSts_path.FDBName], g_EngSt1WdSts_path.FDBName, g_EngSt1WdSts_path.FECUName, g_EngSt1WdSts_path.FMsgName, "EngSt1WdSts_UB", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_EngSt1WdSts_path.FDBName], g_EngSt1WdSts_path.FDBName, g_EngSt1WdSts_path.FECUName, g_EngSt1WdSts_path.FMsgName, g_EngSt1WdSts_path.FSignalName, 1);
}

void SimEngStsCAN_Creat() {
    if (gCEMFlg) {
        SimEngStsCAN();
        com.add_cyclic_message_can(&Sim_EngSts_CAN, 50);
    }
}

//void SimIDcDcActLoSideCAN()
//{
//    int i, j, n;
//    int sigLength;
//    int sigLength_d;
//    int sigLength_m;
//    u8 Data[100];
//    int dataLen;
//    u32 dataID;
//    u32 CRC;
//    for (j = 2; j < EngSts[1].sigNum; j++)
//    {
//        EngSts[1].e2eSignalList[j].sigValue = 0x0;
//    }
//    EngSts[1].e2eSignalList[2].sigValue = 0x0;
//
//    if (cntrIDcDcActLoSide == 0xF)
//    {
//        cntrIDcDcActLoSide = 0;
//    }
//
//    Data[0] = cntrIDcDcActLoSide;////
//    dataLen = 1;
//
//    for (j = 2; j < EngSts[1].sigNum; j++)
//    {
//        sigLength = EngSts[1].e2eSignalList[j].sigLength;
//        sigLength_d = sigLength / 8;
//        sigLength_m = sigLength % 8;
//        if (sigLength_m > 0)
//        {
//            sigLength_d = sigLength_d + 1;
//        }
//
//        for (n = 0; n < sigLength_d; n++)
//        {
//            Data[dataLen] = EngSts[1].e2eSignalList[j].sigValue >> (n * 8);
//
//            dataLen++;
//        }
//    }
//    dataID = EngSts[1].dataID;
//    CRC = E2E_Check(dataID, Data, dataLen);
//
//    EngSts[1].e2eSignalList[1].sigValue = CRC;
//    EngSts[1].e2eSignalList[0].sigValue = cntrIDcDcActLoSide;
//
//    Signal_assignment_EngSts(1);////
//
//
//    if (gCEMFlg)
//    {
//        Sim_IDcDcActLoSide_CAN.FIdxChn = NetworkCluster[EngSts[1].cluster];
//        Sim_IDcDcActLoSide_CAN.FIdentifier = EngSts[1].slotID;
//        Sim_IDcDcActLoSide_CAN.FDLC = EngSts[1].dlc;
//        for (i = 0; i < EngSts[1].dlc; i++)
//        {
//            Sim_IDcDcActLoSide_CAN.FData[i] = Assignment_data[i];
//        }
//        //com.add_cyclic_message_can(&Sim_IDcDcActLoSide_CAN, 50);
//    }
//
//    cntrIDcDcActLoSide++;
//}

void SimIDcDcActLoSideCAN()
{
    StartCANRBS(&g_IDcDcActLoSide_path);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_IDcDcActLoSide_path.FDBName], g_IDcDcActLoSide_path.FDBName, g_IDcDcActLoSide_path.FECUName, g_IDcDcActLoSide_path.FMsgName, "IDcDcActLoSide_UB", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_IDcDcActLoSide_path.FDBName], g_IDcDcActLoSide_path.FDBName, g_IDcDcActLoSide_path.FECUName, g_IDcDcActLoSide_path.FMsgName, g_IDcDcActLoSide_path.FSignalName, 0);
}


void SimIDcDcActLoSideCAN_Creat() {
    //if (gCEMFlg)
    //{
        SimIDcDcActLoSideCAN();
    //    com.add_cyclic_message_can(&Sim_IDcDcActLoSide_CAN, 50);
    //}
}

void StopSimCarTiGlb()
{
    gCarTiGlbSimFlag = 0;
    com.del_cyclic_message_canfd(&Sim_CarTiGlb);
}

void StopSimBkpOfDstTrvld()
{
    gBkpOfDstTrvldSimFlag = 0;
    com.del_cyclic_message_canfd(&Sim_BkpOfDstTrvld);
}

void StopSimTiAndDateIndn()
{
    com.del_cyclic_message_canfd(&Sim_TiAndDateIndn);
}

void FulfilTRC()
{
    if (gCEMFlg == 1)
    {
        //仿真报文
        //SimVehMtnSpdCANFD_Creat();
        SimulateTRC(false);
        TestWait(500);
        SID10_SessionControl(0x01);
        CheckPosResponse_SID10(0x01);

        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);

        SecurityUnlock(0x03);
        TestWait(500);

        SimulateTRC(true);

        //SimVehMtnSpd_Creat();
        //SimEngStsCAN_Creat();
        //SimIDcDcActLoSideCAN_Creat();

        TestWait(500);

    }
    else
    {
        SimVehMtnGlbSafeCANFD_Creat();
        SimVehBat(gUbat12);
        //StartCANRBS(&g_VehModMngtGlbSafe1_path);
        
    }
}

void NM_NextOperation()
{
    UsgModSwtich(0x1);
    EPLvlSwitch(0x0);
    AppWait(1000, "");
    SendTesterPresent(TP_OFF);
    NMWakeUp(0);
    SimulateTRC(false);
    com.del_cyclic_messages();
    CCPTimer.stop();
    KL15(off);
    AppWait(2000, "");
    TestWaitForCurInRange(gLowPwrCur, gSleepTime);

    AppWait(2000, "");
    KL15(on);
    NMWakeUp(1);
    SimulateTRC(true);
    SendTesterPresent(TP_ON);
    AppWait(10000, "");
    FulfilTRC();
    AppWait(1000, "");
    TRCSwitch(0x0, 0xD, 0x0);
    //CarModSwtich(0x0);
    //UsgModSwtich(0xD);
    //EPLvlSwitch(0x0);
}

void NextWarmUpCyc()
{
    if (gNxtOpCycCndtn == 1)
    {
        SimDrvgCycAndWarmUpCyc(0, 0);
        AppWait(5000, "");
        SimDrvgCycAndWarmUpCyc(1, 1);
        AppWait(5000, "");
    }

}

void TestPostCondition()
{
    SID10_SessionControl(0x03);
    CheckPosResponse_SID10(0x03);

    SecurityUnlock(0x03);

    UsgModSwtich(0x1);
    EPLvlSwitch(0x0);

    TestReport_StopLogging();

    TestWait(stepseptime);
    SendTesterPresent(TP_OFF);
    TestWait(stepseptime);

    reportOutputFlag = 0;
    TestWait(1500);

    uds_delete();
    SimulateTRC(false);
    NMWakeUp(0);

    MainPower_OFF();
    BoardCard_Init();
    AppWait(2000, "");
    char* s = NULL;
    app.end_system_message_log(&s);
    //app.set_system_var_int32("MPLib.GEEA_CAN_DIAG", 0);
    //char  systemLog[200];
    //snprintf(systemLog,sizeof(systemLog),"%s\\%s", &logPath[0], TestCaseName);
    //app.export_system_messages(systemLog);
}

/////////////////////////////////////////////////////////////////////////////XCZ/////////////////////////////////////////////////////////////
void ListofNode(u8 state)
{
    int i, j;
    int position;
    u32 ListofNodeAv[8];
    int d, n, enc;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u32 value;

    //if (strncmp(VehModMngtGlbSafe[0].cluster, "ZCU_CANFD1", 20) != 0)return;
    if (strncmp(VehGlbSafe.cluster, "ZCU_CANFD1", 20) != 0)return;
    if (state == 1) {
        for (i = 0; i < 8; i++)
        {
            ListofNodeAv[i] = 0xffffffff;
        }
    }
    else {
        for (i = 0; i < 8; i++)
        {
            ListofNodeAv[i] = 0;
        }
    }

    for (j = 0; j < 8; j++)
    {
        //motorolar Encoding
        startbit = j * 32 + 23;// FR add PDU startBit
        length = 32;
        value = ListofNodeAv[j];

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));
        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }
    _signal[15] = 1;//Update Bit

    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }

    for (i = 1; i < 34; i++)
    {
        Sim_LstOfNod.FData[i] = Assignment_data[i];
    }

    Sim_LstOfNod.FIdentifier = 0x379;
    Sim_LstOfNod.FDLC = 15;
    Sim_LstOfNod.FFDProperties = 1;
    Sim_LstOfNod.set_is_brs(1);
    Sim_LstOfNod.FIdxChn = NetworkCluster["ZCU_CANFD1"];
}

void SimCCPNetwork(u8 state = 1) {
    ListofNode(state);
    Sim_LstOfNod.set_is_brs(false);
    com.add_cyclic_message_canfd(&Sim_LstOfNod,200);
}

void CancelSimCCPNetwork() {
    com.del_cyclic_message_canfd(&Sim_LstOfNod);
}

void Signal_assignment_CANFD_CCPExt()
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u32 value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = CCPConfigTemp[1].sigGroup_UB;
    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < CCPConfigTemp[1].sigNum; j++)
    {
        //motorolar Encoding
        startbit = CCPConfigTemp[1].e2eSignalList[j].startBit;
        length = CCPConfigTemp[1].e2eSignalList[j].sigLength;
        value = CCPConfigTemp[1].e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void Signal_assignment_CANFD_CCP()
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u32 value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = CCPConfigTemp[0].sigGroup_UB;//  FR add PDU startBit
    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < CCPConfigTemp[0].sigNum; j++)
    {
        //motorolar Encoding
        startbit = CCPConfigTemp[0].e2eSignalList[j].startBit;// FR add PDU startBit
        length = CCPConfigTemp[0].e2eSignalList[j].sigLength;
        value = CCPConfigTemp[0].e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    if (CCPConfigTemp[0].slotID == CCPConfigTemp[1].slotID)
    {
        //igVehCfgPrmExt_0
        sigGroup_updatebit = CCPConfigTemp[1].sigGroup_UB;
        _signal[sigGroup_updatebit] = 1;

        for (j = 0; j < CCPConfigTemp[1].sigNum; j++)
        {
            //motorolar Encoding
            startbit = CCPConfigTemp[1].e2eSignalList[j].startBit;
            length = CCPConfigTemp[1].e2eSignalList[j].sigLength;
            value = CCPConfigTemp[1].e2eSignalList[j].sigValue;

            bit_num = startbit;
            _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

            for (n = length - 2; n >= 0; n--)
            {
                if ((bit_num % 8) == 0)
                {
                    bit_num = bit_num + 16;
                }
                bit_num--;
                _signal[bit_num] = ((value & (0x1 << n)) >> n);
            }
        }
    }
    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void Signal_assignment_CAN_CCP()
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u32 value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = CCPConfigTemp[0].sigGroup_UB;
    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < CCPConfigTemp[0].sigNum; j++)
    {
        //motorolar Encoding
        startbit = CCPConfigTemp[0].e2eSignalList[j].startBit;// FR add PDU startBit
        length = CCPConfigTemp[0].e2eSignalList[j].sigLength;
        value = CCPConfigTemp[0].e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void Signal_assignment_CAN_CCPExt()
{
    int d, n, enc, j;
    int _signal[64 * 8];
    int bit_num;

    int startbit;
    int length;
    u32 value;
    int sigGroup_updatebit;

    //初始化 
    for (d = 0; d < 8 * 64; d++)
    {
        _signal[d] = 0;
    }

    for (d = 0; d < 64; d++)
    {
        Assignment_data[d] = 0x0;
    }

    sigGroup_updatebit = CCPConfigTemp[1].sigGroup_UB;
    _signal[sigGroup_updatebit] = 1;

    for (j = 0; j < CCPConfigTemp[1].sigNum; j++)
    {
        //motorolar Encoding
        startbit = CCPConfigTemp[1].e2eSignalList[j].startBit;// FR add PDU startBit
        length = CCPConfigTemp[1].e2eSignalList[j].sigLength;
        value = CCPConfigTemp[1].e2eSignalList[j].sigValue;

        bit_num = startbit;
        _signal[bit_num] = ((value & (0x1 << (length - 1))) >> (length - 1));

        for (n = length - 2; n >= 0; n--)
        {
            if ((bit_num % 8) == 0)
            {
                bit_num = bit_num + 16;
            }
            bit_num--;
            _signal[bit_num] = ((value & (0x1 << n)) >> n);
        }
    }

    for (enc = 0; enc < 64; enc++)
    {
        Assignment_data[enc] = _signal[enc * 8 + 7] * 128 + _signal[enc * 8 + 6] * 64 + _signal[enc * 8 + 5] * 32 + _signal[enc * 8 + 4] * 16 + _signal[enc * 8 + 3] * 8 + _signal[enc * 8 + 2] * 4 + _signal[enc * 8 + 1] * 2 + _signal[enc * 8 + 0] * 1;
    }
}

void SimVehCCP_RBS() {
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, g_CarConfig1_path.FSignalName, 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmBlkIDBytePosn1", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn2", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn3", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn4", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn5", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn6", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn7", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn8", 1);

    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, g_CarConfig2_path.FSignalName, 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtBlkIDBytePosn1", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn2", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn3", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn4", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn5", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn6", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn7", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn8", 1);

    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, g_CarConfig3_path.FSignalName, 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtBlkIDBytePosn1", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn2", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn3", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn4", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn5", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn6", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn7", 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn8", 1);
}

void SimVehCPPOnCAN()
{
    int i, j, n;

    Sim_VehCCP_CAN.FIdxChn = NetworkCluster[CCPConfigTemp[0].cluster];
    Sim_VehCCP_CAN.FIdxChn = CCPConfigTemp[0].slotID;
    Sim_VehCCP_CAN.FDLC = CCPConfigTemp[0].dlc;

    for (j = 0; j < CCPConfigTemp[0].sigNum; j++)
    {
        CCPConfigTemp[0].e2eSignalList[j].sigValue = 0x0;
    }

    CCPConfigTemp[0].e2eSignalList[1].sigValue = gBolockId + 1;
    CCPConfigTemp[0].e2eSignalList[0].sigValue = gCCPData[gBolockId * 7 + 5 - 1];
    CCPConfigTemp[0].e2eSignalList[2].sigValue = gCCPData[gBolockId * 7 + 3 - 1];
    CCPConfigTemp[0].e2eSignalList[3].sigValue = gCCPData[gBolockId * 7 + 2 - 1];
    CCPConfigTemp[0].e2eSignalList[4].sigValue = gCCPData[gBolockId * 7 + 1 - 1];
    CCPConfigTemp[0].e2eSignalList[5].sigValue = gCCPData[gBolockId * 7 + 7 - 1];
    CCPConfigTemp[0].e2eSignalList[6].sigValue = gCCPData[gBolockId * 7 + 6 - 1];
    CCPConfigTemp[0].e2eSignalList[7].sigValue = gCCPData[gBolockId * 7 + 4 - 1];
    gBolockId++;
    if (gBolockId == 72)
    {
        gBolockId = 0;
    }

    Signal_assignment_CAN_CCP();
    for (i = 0; i < CCPConfigTemp[0].dlc; i++)
    {
        Sim_VehCCP_CAN.FData[i] = Assignment_data[i];
    }
    if (CCPConfigTemp[0].slotID > 0)
    {
        com.transmit_can_async(&Sim_VehCCP_CAN);
    }

    //simulate igVehCfgPrmExt_0
    Sim_VehCCPExt_CAN.FIdxChn = NetworkCluster[CCPConfigTemp[1].cluster];
    Sim_VehCCPExt_CAN.FIdentifier = CCPConfigTemp[1].slotID;
    Sim_VehCCPExt_CAN.FDLC = CCPConfigTemp[1].dlc;

    for (j = 0; j < CCPConfigTemp[1].sigNum; j++)
    {
        CCPConfigTemp[1].e2eSignalList[j].sigValue = 0x0;
    }

    CCPConfigTemp[1].e2eSignalList[7].sigValue = gBolockExtId + 1;
    CCPConfigTemp[1].e2eSignalList[0].sigValue = gCCPDataExt[gBolockExtId * 7 + 4 - 1];
    CCPConfigTemp[1].e2eSignalList[1].sigValue = gCCPDataExt[gBolockExtId * 7 + 5 - 1];
    CCPConfigTemp[1].e2eSignalList[2].sigValue = gCCPDataExt[gBolockExtId * 7 + 1 - 1];
    CCPConfigTemp[1].e2eSignalList[3].sigValue = gCCPDataExt[gBolockExtId * 7 + 2 - 1];
    CCPConfigTemp[1].e2eSignalList[4].sigValue = gCCPDataExt[gBolockExtId * 7 + 7 - 1];
    CCPConfigTemp[1].e2eSignalList[5].sigValue = gCCPDataExt[gBolockExtId * 7 + 3 - 1];
    CCPConfigTemp[1].e2eSignalList[6].sigValue = gCCPDataExt[gBolockExtId * 7 + 6 - 1];
    gBolockExtId++;
    if (gBolockExtId == 72)
    {
        gBolockExtId = 0;
    }

    Signal_assignment_CAN_CCPExt();
    for (i = 0; i < CCPConfigTemp[1].dlc; i++)
    {
        Sim_VehCCPExt_CAN.FData[i] = Assignment_data[i];
    }
    if (CCPConfigTemp[1].slotID > 0)
    {
        com.transmit_can_async(&Sim_VehCCPExt_CAN);
    }

}

void SimVehCCPOnCANFD()
{
    int i, j, n;
    Sim_VehCCP_CANFD.FIdxChn = NetworkCluster[CCPConfigTemp[0].cluster];
    //Sim_VehCCP_CANFD.FIdentifier = CCPConfigTemp[0].slotID;
    //Sim_VehCCP_CANFD.FDLC = CCPConfigTemp[0].dlc;
    //Sim_VehCCP_CANFD.FIdxChn = 3;
    Sim_VehCCP_CANFD.FIdentifier = CCPConfigTemp[0].slotID;
    Sim_VehCCP_CANFD.FDLC = 15;
    for (int index = 0; index < MISMSG_DTC_NUM; index++)
    {
        if (misMsgDTCList[index].CCPPsn20 < 504)
        {
            if (gCCPData[misMsgDTCList[index].CCPPsn20] != misMsgDTCList[index].CCPValue20)
            {
                gCCPData[misMsgDTCList[index].CCPPsn20] = misMsgDTCList[index].CCPValue20;
            }
            /* else
             {
                 log("misMsgDTCList[index].CCPValue20  %x", misMsgDTCList[index].CCPValue20);
                 log(" gCCPData[misMsgDTCList[index].CCPPsn20]  %x", gCCPData[misMsgDTCList[index].CCPPsn20]);
                 log("ccp没有赋值");
                 break;

             }*/


        }
        else if (504 < misMsgDTCList[index].CCPPsn20 && misMsgDTCList[index].CCPPsn20 <= 1000)
        {
            if (gCCPDataExt[misMsgDTCList[index].CCPPsn20 - 504] != misMsgDTCList[index].CCPValue20)
            {
                gCCPDataExt[misMsgDTCList[index].CCPPsn20 - 504] = misMsgDTCList[index].CCPValue20;
            }
            /* else
             {
                 log("misMsgDTCList[index].CCPValue20  %x", misMsgDTCList[index].CCPValue20);
                 log(" gCCPDataExt[misMsgDTCList[index].CCPPsn20 - 504]  %x", gCCPDataExt[misMsgDTCList[index].CCPPsn20 - 504]);
                 log("ccp没有赋值");
                 break;
             }*/

        }
        else if (1256 <= misMsgDTCList[index].CCPPsn20 && misMsgDTCList[index].CCPPsn20 <= 1760)
        {
            if (gCCPDataExt2[misMsgDTCList[index].CCPPsn20 - 1256] != misMsgDTCList[index].CCPValue20)
            {
                gCCPDataExt2[misMsgDTCList[index].CCPPsn20 - 1256] = misMsgDTCList[index].CCPValue20;
            }
            /* else
             {
                 log("misMsgDTCList[index].CCPValue20  %x", misMsgDTCList[index].CCPValue20);
                 log(" gCCPDataExt[misMsgDTCList[index].CCPPsn20 - 504]  %x", gCCPDataExt[misMsgDTCList[index].CCPPsn20 - 504]);
                 log("ccp没有赋值");
                 break;
             }*/

        }
        else
        {
            continue;
        }

    }
    gBolockId++;
    Sim_VehCCP_CANFD.FData[31] = gBolockId;
    Sim_VehCCP_CANFD.FData[32] = gCCPData[(gBolockId - 1) * 7 + 1 - 1];
    Sim_VehCCP_CANFD.FData[33] = gCCPData[(gBolockId - 1) * 7 + 2 - 1];
    Sim_VehCCP_CANFD.FData[34] = gCCPData[(gBolockId - 1) * 7 + 3 - 1];
    Sim_VehCCP_CANFD.FData[35] = gCCPData[(gBolockId - 1) * 7 + 4 - 1];
    Sim_VehCCP_CANFD.FData[36] = gCCPData[(gBolockId - 1) * 7 + 5 - 1];
    Sim_VehCCP_CANFD.FData[37] = gCCPData[(gBolockId - 1) * 7 + 6 - 1];
    Sim_VehCCP_CANFD.FData[38] = gCCPData[(gBolockId - 1) * 7 + 7 - 1];



    if (index_cc1 != 3)
    {
        Sim_VehCCP_CANFD.set_is_brs(1);
        Sim_VehCCP_CANFD.set_is_edl(1);
        com.transmit_canfd_async(&Sim_VehCCP_CANFD);
    }
    if (gBolockId == 72)
    {
        gBolockId = 0;
        index_cc1++;
    }

    gBolockExtId++;
    Sim_VehCCPExt_CANFD.FData[0] = gBolockExtId;
    Sim_VehCCPExt_CANFD.FData[1] = gCCPDataExt[(gBolockExtId - 1) * 7 + 1 - 1];
    Sim_VehCCPExt_CANFD.FData[2] = gCCPDataExt[(gBolockExtId - 1) * 7 + 2 - 1];
    Sim_VehCCPExt_CANFD.FData[3] = gCCPDataExt[(gBolockExtId - 1) * 7 + 3 - 1];
    Sim_VehCCPExt_CANFD.FData[4] = gCCPDataExt[(gBolockExtId - 1) * 7 + 4 - 1];
    Sim_VehCCPExt_CANFD.FData[5] = gCCPDataExt[(gBolockExtId - 1) * 7 + 5 - 1];
    Sim_VehCCPExt_CANFD.FData[6] = gCCPDataExt[(gBolockExtId - 1) * 7 + 6 - 1];
    Sim_VehCCPExt_CANFD.FData[7] = gCCPDataExt[(gBolockExtId - 1) * 7 + 7 - 1];

    gBolockExt2Id++;
    Sim_VehCCPExt_CANFD.FData[17] = gBolockExt2Id;
    Sim_VehCCPExt_CANFD.FData[18] = gCCPDataExt2[(gBolockExt2Id - 1) * 7 + 1 - 1];
    Sim_VehCCPExt_CANFD.FData[19] = gCCPDataExt2[(gBolockExt2Id - 1) * 7 + 2 - 1];
    Sim_VehCCPExt_CANFD.FData[20] = gCCPDataExt2[(gBolockExt2Id - 1) * 7 + 3 - 1];
    Sim_VehCCPExt_CANFD.FData[21] = gCCPDataExt2[(gBolockExt2Id - 1) * 7 + 4 - 1];
    Sim_VehCCPExt_CANFD.FData[22] = gCCPDataExt2[(gBolockExt2Id - 1) * 7 + 5 - 1];
    Sim_VehCCPExt_CANFD.FData[23] = gCCPDataExt2[(gBolockExt2Id - 1) * 7 + 6 - 1];
    Sim_VehCCPExt_CANFD.FData[24] = gCCPDataExt2[(gBolockExt2Id - 1) * 7 + 7 - 1];

    if (index_cc2 != 3)
    {
        if (CCPConfigTemp[0].slotID != CCPConfigTemp[1].slotID)
        {
            Sim_VehCCPExt_CANFD.FIdxChn = NetworkCluster[CCPConfigTemp[1].cluster];
            Sim_VehCCPExt_CANFD.FIdentifier = CCPConfigTemp[1].slotID;
            Sim_VehCCPExt_CANFD.FDLC = 15;
            Sim_VehCCPExt_CANFD.set_is_brs(false);
            Sim_VehCCPExt_CANFD.set_is_edl(1);
            com.transmit_canfd_async(&Sim_VehCCPExt_CANFD);
        }
    }
    else
    {
        log("停止仿真1F5");
    }
    lock_cc2 = false;
    if (gBolockExtId == 72)
    {
        gBolockExt2Id = 0;
        gBolockExtId = 0;
        index_cc2++;
    }
}

void SimVehCPP() {
    //gCCPfdFlag留出配置信息，根据配置页面填写参数决定TpBus类型
    if (gCCPfdFlag) {
        SimVehCCPOnCANFD();
    }
    else {
        SimVehCPPOnCAN();
    }
}

void SimMissMsg(int index)
{
    
    //确定通道
    /*
    log("misMsgDTCList[index].busType  %s", misMsgDTCList[index].busType);
    if (strcmp(misMsgDTCList[index].busType, "CAN") == 0)
    {
        int ret;

        log("misMsgDTCList[index].busType  %s", misMsgDTCList[index].busType);
        log("cluster[%s]: ",misMsgDTCList[index].cluster);
        log("NetworkCluster[misMsgDTCList[index].cluster]  %d", NetworkCluster[misMsgDTCList[index].cluster]);
        log("misMsgDTCList[index].cluster  %s", misMsgDTCList[index].cluster);
        misfram_can.FIdentifier = misMsgDTCList[index].misMsgList->msgID;
        //misfram_can.init_w_std_id(Missmage_ID, misMsgDTCList[index].misMsgList->msgDataLgth);
        misfram_can.FIdxChn = NetworkCluster[misMsgDTCList[index].cluster];
        misfram_can.FDLC = misMsgDTCList[index].misMsgList->msgDataLgth;
        //log("misMsgDTCList[index].misMsgList->msgDataLgth   %x", misMsgDTCList[index].misMsgList->msgDataLgth);
        //log(" misMsgDTCList[index].misMsgList->msgID  %x", misMsgDTCList[index].misMsgList->msgID);
        //log("Missmage_ID   %x", Missmage_ID);
        //log("misMsgDTCList[index].misMsgList->msgCycTime   %d", misMsgDTCList[index].misMsgList->msgCycTime);
        ret = com.add_cyclic_message_can(&misfram_can, misMsgDTCList[index].misMsgList->msgCycTime);
        log("ret  %d", ret);
    }
    else if (strcmp(misMsgDTCList[index].busType, "CANFD") == 0)
    {

        misfram_canfd.FIdentifier = misMsgDTCList[index].misMsgList->msgID;
        misfram_canfd.FIdxChn = NetworkCluster[misMsgDTCList[index].cluster];
        misfram_canfd.FDLC = misMsgDTCList[index].misMsgList->msgDataLgth;
        com.add_cyclic_message_canfd(&misfram_canfd, misMsgDTCList[index].misMsgList->msgCycTime);
    }
    else
    {
        log("否定条件misMsgDTCList[index].busType  %s", misMsgDTCList[index].busType);
    }
    log("111");
    */

    //misfram_canfd.set_is_edl(misMsgDTCList[index].signalResult->isFD);
    //misfram_canfd.FIdentifier = misMsgDTCList[index].signalResult->messageId;
    //misfram_canfd.FIdxChn = NetworkCluster[misMsgDTCList[index].signalResult->dbName];
    //misfram_canfd.FDLC = misMsgDTCList[index].signalResult->DLC;
    //misfram_canfd.FData[misMsgDTCList[index].signalResult->ubStartBit / 8] = (0xFF & (0x01 << (misMsgDTCList[index].signalResult->ubStartBit % 8)));
    //com.add_cyclic_message_canfd(&misfram_canfd, misMsgDTCList[index].signalResult->cycle);
    char signalUB[50] = { 0 };
    snprintf(signalUB,sizeof(signalUB),"%s_UB", g_MissFrame_path[index].FSignalName);
    if (0 == com.can_rbs_activate_network_by_name(NetworkCluster[g_MissFrame_path[index].FDBName], true, g_MissFrame_path[index].FDBName, false)) {
        log("Active CH%d %s Successful", NetworkCluster[g_MissFrame_path[index].FDBName] + 1, g_MissFrame_path[index].FDBName);
    }
    else {
        log("Active CH%d %s Failed", NetworkCluster[g_MissFrame_path[index].FDBName] + 1, g_MissFrame_path[index].FDBName);
    }
    if (0 == com.can_rbs_activate_node_by_name(NetworkCluster[g_MissFrame_path[index].FDBName], true, g_MissFrame_path[index].FDBName, g_MissFrame_path[index].FECUName, false)) {
        log("Active %s Successful", g_MissFrame_path[index].FECUName);
    }
    else {
        log("Active %s Failed", g_MissFrame_path[index].FECUName);
    }
    if (0 == com.can_rbs_activate_message_by_name(NetworkCluster[g_MissFrame_path[index].FDBName], true, g_MissFrame_path[index].FDBName, g_MissFrame_path[index].FECUName, g_MissFrame_path[index].FMsgName)) {
        log("Active ID[0x%X] %s Successful", g_MissFrame_path[index].FFrameIndentifier,g_MissFrame_path[index].FMsgName);
    }
    else {
        log("Active ID[0x%X] %s Failed", g_MissFrame_path[index].FFrameIndentifier, g_MissFrame_path[index].FMsgName);
    }
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_MissFrame_path[index].FDBName], g_MissFrame_path[index].FDBName, g_MissFrame_path[index].FECUName, g_MissFrame_path[index].FMsgName, signalUB, 1);
    com.can_rbs_set_signal_value_by_element(NetworkCluster[g_MissFrame_path[index].FDBName], g_MissFrame_path[index].FDBName, g_MissFrame_path[index].FECUName, g_MissFrame_path[index].FMsgName, g_MissFrame_path[index].FSignalName, 0);
}   

void CancelMissMsg(int index)
{
    com.can_rbs_activate_message_by_name(NetworkCluster[g_MissFrame_path[index].FDBName], false, g_MissFrame_path[index].FDBName, g_MissFrame_path[index].FECUName, g_MissFrame_path[index].FMsgName);
    //com.can_rbs_activate_node_by_name(NetworkCluster[g_MissFrame_path[index].FDBName], false, g_MissFrame_path[index].FDBName, g_MissFrame_path[index].FECUName, false);
    //com.can_rbs_activate_network_by_name(NetworkCluster[g_MissFrame_path[index].FDBName], false, g_MissFrame_path[index].FDBName, false);
    

    //if (strcmp(misMsgDTCList[index].busType, "CAN") == 0)
    //{
    //    com.del_cyclic_message_can(&misfram_can);
    //}
    //else if (strcmp(misMsgDTCList[index].busType, "CANFD") == 0)
    //{
    //    com.del_cyclic_message_canfd(&misfram_canfd);
    //}
}

u8 ReadSnapShotBy20(u8 dtc[])
{
    u8 state;
    RX_SID19_ReportDTCSnapshotRecordByDTCNumber(dtc, 0x20, Physical);
    RX_CheckPosResponse_SID19_04(dtc);
    if (TpRxLength >= 8)
        state = 1;
    else
        state = 0;
    return state;
}

u64 ReadTimeStamp(u8 dtc[], u8 recordNum)
{
    u64 timestamp = 0;
    RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(dtc, recordNum, Physical);
    Rx_CheckPosResponse_SID19_06(dtc);
    if (TpRxLength >= 8)
    {
        if (recordNum == 0x20 || recordNum == 0x21)
        {
            timestamp = TpRxData[7] * 0x1000000 + TpRxData[8] * 0x10000 + TpRxData[9] * 0x100 + TpRxData[10];
        }
        else if (recordNum == 0x22 || recordNum == 0x23)
        {
            timestamp = TpRxData[7] * 0x10000000000ll + TpRxData[8] * 0x100000000ll + TpRxData[9] * 0x1000000 + TpRxData[10] * 0x10000 + TpRxData[11] * 0x100 + TpRxData[12];
        }
    }
    else
        timestamp = 0;
    return timestamp;
}

void CheckSnapshotDataValue(u8 dtc[], u32 globalRealTime, u32 totalDistance, u8 Ubat, u16 VehSpd, u8 GNSSTime[], u8 usgmod, u8 pnc[], u8 eplevel,u8 carmode)
{
    int i = 0;
    int dataid[65535], dataidresp[65535];
    u8 timeArray[4];
    u8 bkpArray[3];
    u8 vehSpdArray[2];

    /*--------Snapshot DID list---------*/
  //  dataid[0xDD00] = 1;//需求确认，3.0不要求必须支持DD00；支持则检查DID值，不支持不报错
    dataid[0xDD01] = 1;
    dataid[0xDD02] = 1;
    dataid[0xDD06] = 1;
    dataid[0xDD07] = 1;
    dataid[0xDD0A] = 1;
    dataid[0xDD0C] = 1;
    if (gCEMFlg)
        dataid[0xDD0B] = 1;//VMM Master必须支持此快照DID
    //  dataid[0xD134] = 1;//需求确认，D134不是全局快照，不强制支持；支持则检查DID值，不支持不报错
      /*----------------------------------*/

    if (TpRxData[0] != 0x59)return;
    if (TpRxLength < 8)
    {
        TestReport_HTML_StepDescription("No snapshot record in the response.",Fail);
        return;
    }
    for (i = 8; i < TpRxLength; i++)
    {
        /*----------------------Check DD00---------------------------*/
        if (TpRxData[i] == 0xDD && TpRxData[i + 1] == 0x00)
        {
            timeArray[0] = (globalRealTime & 0xFF000000) >> 24;
            timeArray[1] = (globalRealTime & 0xFF0000) >> 16;
            timeArray[2] = (globalRealTime & 0xFF00) >> 8;
            timeArray[3] = globalRealTime & 0xFF;

            if (strncmp(CarTiGlb.cluster, "", 50) != 0)
            {
                if (TpRxData[i + 2] == timeArray[0] &&
                    TpRxData[i + 3] == timeArray[1] &&
                    TpRxData[i + 4] == timeArray[2] &&
                    TpRxData[i + 5] == timeArray[3])
                    TestReport_HTML_StepDescription("Snapshot 0xDD00 value is correct",Pass);
                else {
                    snprintf(StepDespriction, sizeof(StepDespriction), "Snapshot 0xDD00 value is incorrect, expected is 0x%08lX", globalRealTime);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }
            else
                TestReport_HTML_StepDescription("Find Snapshot 0xDD00 in response", Pass);

            dataidresp[0xDD00] = 1;
            continue;
        }
        /*----------------------Check DD01---------------------------*/
        if (TpRxData[i] == 0xDD && TpRxData[i + 1] == 0x01)
        {
            bkpArray[0] = (totalDistance & 0xFF0000) >> 16;
            bkpArray[1] = (totalDistance & 0xFF00) >> 8;
            bkpArray[2] = totalDistance & 0xFF;

            if (strncmp(BkpOfDstTrvld.cluster, "", 50) != 0)
            {
                if (TpRxData[i + 2] == bkpArray[0] &&
                    TpRxData[i + 3] == bkpArray[1] &&
                    TpRxData[i + 4] == bkpArray[2])
                    TestReport_HTML_StepDescription("Snapshot 0xDD01 value is correct", Pass);
                else {
                    snprintf(StepDespriction, sizeof(StepDespriction), "Snapshot 0xDD01 value is incorrect, expected is 0x%06lX", totalDistance);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }
            else
                TestReport_HTML_StepDescription("Find Snapshot 0xDD01 in response", Pass);

            dataidresp[0xDD01] = 1;
            continue;
        }
        /*----------------------Check DD02---------------------------*/
        if (TpRxData[i] == 0xDD && TpRxData[i + 1] == 0x02)
        {
            if (!gCEMFlg)
            {
                if (TpRxData[i + 2] == Ubat)
                    TestReport_HTML_StepDescription("Snapshot 0xDD02 value is correct",Pass);
                else {
                    snprintf(StepDespriction, sizeof(StepDespriction), "Snapshot 0xDD02 value is incorrect, expected is 0x%X", Ubat);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }
            else
                TestReport_HTML_StepDescription("Find Snapshot 0xDD02 in response", Pass);

            dataidresp[0xDD02] = 1;
            continue;
        }
        /*----------------------Check DD06---------------------------*/
        if (TpRxData[i] == 0xDD && TpRxData[i + 1] == 0x06)
        {
            vehSpdArray[0] = (VehSpd & 0xFF00) >> 8;
            vehSpdArray[1] = VehSpd & 0xFF;

            if (strncmp(VehSpdLgt.cluster, "", 50) != 0)
            {
                if (TpRxData[i + 2] == vehSpdArray[0] && TpRxData[i + 3] == vehSpdArray[1])
                    TestReport_HTML_StepDescription("Snapshot 0xDD06 value is correct", Pass);
                else {
                    snprintf(StepDespriction, sizeof(StepDespriction), "Snapshot 0xDD06 value is incorrect, expected is 0x%04X", VehSpd);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }
            else
                TestReport_HTML_StepDescription("Find Snapshot 0xDD06 in response", Pass);

            dataidresp[0xDD06] = 1;
            continue;
        }
        /*----------------------Check DD07---------------------------*/
        if (TpRxData[i] == 0xDD && TpRxData[i + 1] == 0x07)
        {
            if (strncmp(TiAndDateIndn.cluster, "", 50) != 0)
            {
                if (TpRxData[i + 2] == GNSSTime[0] && TpRxData[i + 3] == GNSSTime[1] && TpRxData[i + 4] == GNSSTime[2] && TpRxData[i + 5] == GNSSTime[3] && TpRxData[i + 6] == GNSSTime[4] && TpRxData[i + 7] == GNSSTime[5])
                    TestReport_HTML_StepDescription("Snapshot 0xDD07 value is correct", Pass);
                else {
                    snprintf(StepDespriction, sizeof(StepDespriction), "Snapshot 0xDD07 value is incorrect, expected is 0x%02X%02X%02X%02X%02X%02X", GNSSTime[0], GNSSTime[1], GNSSTime[2], GNSSTime[3], GNSSTime[4], GNSSTime[5]);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }
            else
                TestReport_HTML_StepDescription("Find Snapshot 0xDD07 in response", Pass);

            dataidresp[0xDD07] = 1;
            continue;
        }
        /*----------------------Check DD0A---------------------------*/
        if (TpRxData[i] == 0xDD && TpRxData[i + 1] == 0x0A)
        {
            if (TpRxData[i + 2] == usgmod)
                TestReport_HTML_StepDescription("Snapshot 0xDD0A value is correct", Pass);
            else {
                snprintf(StepDespriction, sizeof(StepDespriction), "Snapshot 0xDD0A value is incorrect, expected is 0x%X", usgmod);
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }

            dataidresp[0xDD0A] = 1;
            continue;
        }
        /*----------------------Check DD0B---------------------------*/
        if (TpRxData[i] == 0xDD && TpRxData[i + 1] == 0x0B)
        {
            if (gCEMFlg)
            {
                if (TpRxData[i + 2] == 0 && TpRxData[i + 3] == 0 && TpRxData[i + 4] == 0 && TpRxData[i + 5] == 0)
                    TestReport_HTML_StepDescription("Snapshot 0xDD0B value shall not be all zero", Fail);
                else if ((pnc[0] | ~TpRxData[i + 2]) &
                    (pnc[1] | ~TpRxData[i + 3]) &
                    (pnc[2] | ~TpRxData[i + 4]) &
                    (pnc[3] | ~TpRxData[i + 5]) == 0xFFFFFFFF)
                    TestReport_HTML_StepDescription("Snapshot 0xDD0B value is correct", Pass);
                else {
                    snprintf(StepDespriction, sizeof(StepDespriction), "Snapshot 0xDD0B value is incorrect. PNC not in {%d,%d,%d,%d} shall not be active", pnc[0], pnc[1], pnc[2], pnc[3]);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }

            dataidresp[0xDD0B] = 1;
            continue;
        }
        /*----------------------Check DD0C---------------------------*/
        if (TpRxData[i] == 0xDD && TpRxData[i + 1] == 0x0C)
        {
            if (TpRxData[i + 2] == eplevel)
                TestReport_HTML_StepDescription("Snapshot 0xDD0C value is correct", Pass);
            else {
                snprintf(StepDespriction, sizeof(StepDespriction), "Snapshot 0xDD0C value is incorrect, expected is 0x%X", eplevel);
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }

            dataidresp[0xDD0C] = 1;
            continue;
        }
        /*----------------------Check D134---------------------------*/
        if (TpRxData[i] == 0xD1 && TpRxData[i + 1] == 0x34)
        {
            if (TpRxData[i + 2] == carmode)
                TestReport_HTML_StepDescription( "Snapshot 0xD134 value is correct", Pass);
            else
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "Snapshot 0xD134 value is incorrect, expected is 0x%X", carmode);
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }

            dataidresp[0xD134] = 1;
            continue;
        }
    }

    for (long id : dataid)
    {
        if (!dataidresp[id])
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "No Snapshot Data 0x%X in response", id);
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        }
    }
}

void CheckTimeStamp(u8 recordNum, u8 dtc[], u32 expectedValue)
{
    u64 timestamp;
    if (recordNum == 0x20 || recordNum == 0x21)
    {
        if (!gSupportTs2021)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "It's not mandatory to support time stamp #%X", recordNum);
            TestReport_HTML_StepDescription(StepDespriction);
            return;
        }
        timestamp = ReadTimeStamp(dtc, recordNum);
        if (strncmp(CarTiGlb.cluster, "", 100) == 0)
        {
            if (TpRxLength >= 8)
                TestReport_HTML_StepDescription("DUT doesn't receive CarTiGlb in SDB, just check timestamp is recorded.", Pass);
            else {
                snprintf(StepDespriction, sizeof(StepDespriction), "No time stamp #%X record in the response.", recordNum);
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }
            return;
        }
        if (timestamp == expectedValue)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "Time stamp #%X is 0x%08lX.", recordNum, expectedValue);
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        }
        else
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "Time stamp #%X is not correct, expected is 0x%08lX, actual value is 0x%08lX", recordNum, expectedValue, timestamp);
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        }
    }
    else
        TestReport_HTML_StepDescription("Unknown Extended Data Record Number", Warning);
}

void CheckTimeStamp(u8 recordNum, u8 dtc[], u8 expectedValue[])
{
    u64 timestamp;
    if (recordNum == 0x22 || recordNum == 0x23)
    {
        timestamp = ReadTimeStamp(dtc, recordNum);
        if (strncmp(TiAndDateIndn.cluster, "", 100) == 0)
        {
            if (TpRxLength >= 8)
                TestReport_HTML_StepDescription("DUT doesn't receive TiAndDateIndn in SDB, just check timestamp is recorded.", Pass);
            else {
                snprintf(StepDespriction, sizeof(StepDespriction), "No time stamp #%X record in the response.", recordNum);
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }
            return;
        }
        if (timestamp == expectedValue[0] * 0x10000000000ll + expectedValue[1] * 0x100000000ll + expectedValue[2] * 0x1000000 + expectedValue[3] * 0x10000 + expectedValue[4] * 0x100 + expectedValue[5])
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "Time stamp #%X is 0x%012llX.", recordNum, timestamp);
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        }
        else
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "Time stamp #%X is 0x%012llX, expected is 0x%02X%02X%02X%02X%02X%02X", recordNum, timestamp, expectedValue[0], expectedValue[1], expectedValue[2], expectedValue[3], expectedValue[4], expectedValue[5]);
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        }
    }
    else
        TestReport_HTML_StepDescription("Unknown Extended Data Record Number",Warning);
}


void Snapshot_SigValueChange(u32 globalRealTime, u32 totalDistance, u16 VehSpd, u8 GNSSTime[],u8 GNSSTimeLength)
{
    int i;

    gCarTiGlb = globalRealTime;
    snprintf(StepDespriction,sizeof(StepDespriction), "Simulate CarTiGlb to 0x%08lX", gCarTiGlb);
    TestReport_HTML_StepDescription(StepDespriction);

    gBkpOfDstTrvld = totalDistance;
    snprintf(StepDespriction, sizeof(StepDespriction), "Simulate BkpOfDstTrvld to 0x%06lX", gBkpOfDstTrvld);
    TestReport_HTML_StepDescription(StepDespriction);

    if (gCndtnOfClrDtc >= 2 && VehSpd > 0xD5)
        gVehSpd = gVehSpd_valid;
    else
        gVehSpd = VehSpd;
    snprintf(StepDespriction, sizeof(StepDespriction), "Simulate VehSpd to 0x%04X", gVehSpd);
    TestReport_HTML_StepDescription(StepDespriction);
    for (i = 0; i < GNSSTimeLength; i++)
    {
        gTiAndDateIndn[i] = GNSSTime[i];
    }
    snprintf(StepDespriction, sizeof(StepDespriction), "Simulate GNSS Time to 0x%02X%02X%02X%02X%02X%02X", gTiAndDateIndn[0], gTiAndDateIndn[1], gTiAndDateIndn[2], gTiAndDateIndn[3], gTiAndDateIndn[4], gTiAndDateIndn[5]);
    TestReport_HTML_StepDescription(StepDespriction);
}


/////////////////////////////////////////////////干扰仪//////////////////////////////////////////////

s32 DisturbConnect() {
    //连接设备
    s32 handle = 0;
    s32 count = 0;
    char TempString[50];
    count = tsdisturb.tsdisturb_scan();
    snprintf(TempString, 50, "硬件数量 = %d", count);
    app.log_text(TempString,lvlInfo);
    //printf("handle初始化值 = %d", handle);
    snprintf(TempString, 50, "handle初始化值 = %d", handle);
    app.log_text(TempString, lvlInfo);
    s32 result1 = tsdisturb.tsdisturb_connect_can(0, &handle, 500, 80, 2000, 80);
    //s32 result1 = tsdisturb_dll.th7011_connect(&handle, 500, 2000, 80, 80);
    //printf("connect函数返回值 = %d", result1);
    snprintf(TempString, 50, "connect函数返回值 = %d", result1);
    app.log_text(TempString, lvlInfo);
    return handle;
}

void DisturbDisconnect() {
    //断开干扰仪连接
    tsdisturb.tsdisturb_disconnect_all();
    //tsdisturb_dll.th7011_disconnect(handle);
}

void DisturbFrame(s32 handle,u8 index,int times, int cycles,int timeOut,u8 status = 1) {

    u8 channel = NetworkCluster[busFltDTCTemp[index].cluster];
    char StepDescription[200] = {};
    u8 FDC10_1 = 0;
    u8 fdtc[3] = { 0 };
    s64 currentTime, lastTime;
    s32 result2;

    fdtc[0] = (busFltDTCTemp[index].busOffDTC >> 16) & 0xFF;
    fdtc[1] = (busFltDTCTemp[index].busOffDTC >> 8) & 0xFF;
    fdtc[2] = busFltDTCTemp[index].busOffDTC & 0xFF;


    //配置帧触发
    can_disturbance_trigger trigger = {};
    canfd_filter filter = {};
    Disturb_Frame_config_t start_pos = {};
    memset(&filter, 0, sizeof(canfd_filter));

    //配置干扰起始位
    start_pos.Frame_Field = CRC_FIELD_FLAG;//CRC场
    start_pos.Pos = 15;//第几位
    start_pos.Stuff_Pos = 0;
    tsdisturb.tsdisturb_set_trigger_frame(&trigger, &filter, &start_pos);

    //配置位序列
    can_disturbance_sequence sequence = {};
    sequence_config_t config = {};
    config.bit_num = 20;//序列中有多少 BIT
    config.Frame_format[0] = 0b101010;//表示序列的高低电平
    config.div = 1;//表示 1bit 长度
    tsdisturb.tsdisturb_set_sequence(&sequence, &config);

    //配置大小干扰周期、间隔
    disturbance_repetitions repetitions = {};//干扰次数与周期
    memset(&repetitions, 0, sizeof(repetitions));
    repetitions.Cycles = cycles;//周期
    //repetitions.Cycles = 1;//周期
    // //GEEA3.0
    //repetitions.Cycles_hold = 15;//周期间隔
    // //GEEA3.5
    repetitions.Cycles_hold = 0;//周期间隔
    repetitions.Repetitions = times;//重复次数
    repetitions.Repetitions_hold = 0;//重复间隔

    //目的是干扰ECU所发的报文，所以先停止报文仿真
    com.del_cyclic_messages();
    //printf("delete all cyclic messages");
    SendTesterPresent(TP_ON);
    //if (BusTypeNow == CAN)
    //{
    //    com.transmit_can_async(&TesterPresent);
    //}
    //else if (BusTypeNow == CANFD)
    //{
    //    com.transmit_canfd_async(&TesterPresentFD);
    //}
    TestReport_HTML_StepDescription("开始干扰");
    DUTErrorCount = 0;
    result2 = tsdisturb.tsdisturb_can_disturb_start(handle, &trigger, &sequence, &repetitions, NULL);
    printf("startt函数返回值 = %d", result2);


    //if (cycles >= 32) {
    //    result2 = tsdisturb.tsdisturb_can_disturb_start(handle, &trigger, &sequence, &repetitions, NULL);
    //    log("startt函数返回值 = %d", result2);
    //}
    //else {
    // 
    //    //每次干扰周期为1，间隔fastRecoveryTime或slowRecoveryTime后干扰下一次
    //    repetitions.Cycles = 1;//周期
    //    repetitions.Cycles_hold = 1;//周期间隔
    //    repetitions.Repetitions = times;//重复次数
    //    repetitions.Repetitions_hold = 0;//重复间隔
    //    for (u8 cycleCount = 0; cycleCount < cycles; cycleCount++) {
    //        s32 result2 = tsdisturb.tsdisturb_can_disturb_start(handle, &trigger, &sequence, &repetitions, NULL);
    //        log("startt函数返回值 = %d", result2);
    //        app.get_timestamp(&lastTime);
    //        while (1) {
    //            app.get_timestamp(&currentTime);
    //            if ((currentTime - lastTime) / 1000 > 2000) {
    //                log("Cycle TimeOut,error count is %d", DUTErrorCount);
    //                break;
    //            }
    //            else if (DUTErrorCount >= times* (cycleCount+1)) {
    //                log("Cycle error count is %d", DUTErrorCount);
    //                break;
    //            }
    //        }
    //        //log("startt函数返回值 = %d,ErrorCount is %d", result2, DUTErrorCount);
    //        //FDC10_1 = ReadFDC10(fdtc);
    //        //snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x%X", FDC10_1);
    //        //TestReport_HTML_StepDescription(issuereason, Info);
    //        AppWait(fastRecoveryTime, "");
    //    }
    //}

    app.get_timestamp(&lastTime);
    while (1) {
        app.get_timestamp(&currentTime);
        if ((currentTime - lastTime) / 1000 > timeOut) {
            snprintf(StepDescription, sizeof(StepDescription), "干扰结束，ErrorFrame %d", DUTErrorCount);
            TestReport_HTML_StepDescription(StepDescription);
            printf("TimeOut,error count is %d", DUTErrorCount);
            break;
        }
        else if (DUTErrorCount >= cycles * times) {
            snprintf(StepDescription, sizeof(StepDescription), "干扰结束，ErrorFrame %d", DUTErrorCount);
            TestReport_HTML_StepDescription(StepDescription);
            printf("error count is %d", DUTErrorCount);
            break;
        }
    }

    //AppWait(10, "wait 10ms,read FDC10");
    //TestStep("读Busoff DTC的FDC10");
    

    ////干扰后读取FDC10，取消使用
    //FDC10_1 = ReadFDC10(fdtc);
    //log("FDC10 is 0x%X", FDC10_1);
    //snprintf(StepDescription, sizeof(StepDescription), "FDC10 is 0x%X", FDC10_1);
    //TestReport_HTML_StepDescription(StepDescription, Info);


    ////干扰后根据读取FDC10，判断干扰是否进行完成，未完成继续进行干扰，已停用
    //if (busFltDTCTemp[index].SUValue * cycles != 0x80) {
    //    log("StepUp * cycles != 0x80");
    //    if (FDC10_1 <= busFltDTCTemp[index].SUValue * cycles) {
    //        DisturbFrame(handle, index, 32, (busFltDTCTemp[index].SUValue * cycles - FDC10_1) / 4, 5000);
    //    }
    //}
    //else {
    //    if(FDC10_1 != 0x7F) {
    //        log("FDC10 is not satisfied，FDC10 is 0x%X", FDC10_1);
    //        DisturbFrame(handle, index, 32, (0x80 - FDC10_1) / 4, 5000);
    //        FDC10_1 = ReadFDC10(fdtc);
    //        snprintf(StepDescription, sizeof(StepDescription), "FDC10 is 0x%X", FDC10_1);
    //        TestReport_HTML_StepDescription(StepDescription, Info);
    //    }
    //}
    
    ////在干扰函数内进行等待快慢恢复，已改为主函数内判断恢复状态
    //app.get_timestamp(&lastTime);
    //if(cycles<32)
    //    AppWait(fastRecoveryTime,"wait DUT Fast Recovery");
    //else {
    //    app.set_system_var_int32("CheckCom", 0);
    //    s32 v;
    //    while (1) {
    //        app.get_timestamp(&currentTime);
    //        app.get_system_var_int32("CheckCom", &v);
    //        if ((currentTime - lastTime) / 1000 > slowRecoveryTime + tolerabRecoveryleTime) {
    //            log("TimeOut,Communication Is Not Normal");
    //            break;
    //        }
    //        else if (v == 1) {

    //        }
    //    }
    //    AppWait(slowRecoveryTime, "wait DUT Slow Recovery");
    //}

    //GEEA3.0
    /*if (cycles < 32) {
        DUTrecoveryTime = fastRecoveryTime;
    }
    else {
        DUTrecoveryTime = slowRecoveryTime;
    }*/
    //GEEA3.5
    if (cycles < 10) {
        DUTrecoveryTime = fastRecoveryTime;
    }
    else {
        DUTrecoveryTime = slowRecoveryTime;
    }

    //if (times * cycles < 200)
    //{
    //    AppWait(1000, ""); //干扰是异步进行的，所以要在这里确保干扰成功执行
    //}
    //else
    //{
    //    AppWait(2500, ""); //干扰是异步进行的，所以要在这里确保干扰成功执行
    //}

    //if (BusTypeNow == CAN)
    //{
    //    com.transmit_can_async(&TesterPresent);
    //}
    //else if (BusTypeNow == CANFD)
    //{
    //    com.transmit_canfd_async(&TesterPresentFD);
    //}

    //tsdisturb.tsdisturb_stop(handle);

    //SendTesterPresent(TP_ON);

    //u32 turetimes = 0;
    //tsdisturb.tsdisturb_query(handle, &turetimes);
    //log("干扰帧数 = %d", turetimes);
    //
    //if (turetimes != times * cycles)
    //{
    //    snprintf(StepDespriction, sizeof(StepDespriction), "Interferetimes is %d, expected is %d", turetimes, times * cycles);
    //    TestReport_HTML_StepDescription(StepDespriction, Warning);
    //}
   
    if (DUTErrorCount != times * cycles)
    {
        snprintf(StepDespriction, sizeof(StepDespriction), "Interferetimes is %d, expected is %d", DUTErrorCount, times * cycles);
        TestReport_HTML_StepDescription(StepDespriction, Warning);
    }
    if(status == 1){
        if (gCEMFlg == 1)
        {
            //SendTesterPresent(TP_ON);
            printf("start cycle message transmit");
            NMWakeUp(1, channel);
            AppWait(10, "");
            //SimVehMtnSpdCANFD_Creat();
            //SimEngStsCAN_Creat();
            //SimIDcDcActLoSideCAN_Creat();
            SimVehMtnSpd_Creat();
            SimEngStsCAN_Creat();
            SimIDcDcActLoSideCAN_Creat();
            //SID10_SessionControl(0x01);
            //CheckPosResponse_SID10(0x01);

            //SID10_SessionControl(0x03);
            //CheckPosResponse_SID10(0x03);

            //SecurityUnlock(0x03);
        }
        else
        {
            NMWakeUp(1, channel);
            SimVehMtnGlbSafeCANFD_Creat();
            SimVehBat(gUbat12);
        }
    }
}

//times代表一次干扰多少帧报文，cycles代表触发多少次
void InterfereCanMessage(int times, int cycles)
{   
    //连接设备
    s32 handle = 0;
    s32 count = 0;
    count = tsdisturb.tsdisturb_scan();
    log("硬件数量 = %d", count);
    log("handle初始化值 = %d", handle);
    s32 result1 = tsdisturb.tsdisturb_connect_can(0, &handle, 500, 80, 2000, 80);
    log("connect函数返回值 = %d", result1);

    //配置帧触发
    can_disturbance_trigger trigger = {};
    canfd_filter filter = {};
    Disturb_Frame_config_t start_pos = {};
    memset(&filter, 0, sizeof(canfd_filter));

    //配置干扰起始位
    start_pos.Frame_Field = CRC_FIELD_FLAG;//CRC场
    start_pos.Pos = 15;//第几位
    start_pos.Stuff_Pos = 0;
    tsdisturb.tsdisturb_set_trigger_frame(&trigger, &filter, &start_pos);

    //配置位序列
    can_disturbance_sequence sequence = {};
    sequence_config_t config = {};
    config.bit_num = 20;//序列中有多少 BIT
    config.Frame_format[0] = 0b101010;//表示序列的高低电平
    config.div = 1;//表示 1bit 长度
    tsdisturb.tsdisturb_set_sequence(&sequence, &config);

    //配置大小干扰周期、间隔
    disturbance_repetitions repetitions = {};//干扰次数与周期
    memset(&repetitions, 0, sizeof(repetitions));
    repetitions.Cycles = cycles;//周期
    repetitions.Cycles_hold = 15;//周期间隔
    repetitions.Repetitions = times;//重复次数
    repetitions.Repetitions_hold = 0;//重复间隔

    //目的是干扰ECU所发的报文，所以先停止报文仿真
    com.del_cyclic_messages();

    if (BusTypeNow == CAN)
    {
        com.transmit_can_async(&TesterPresent);
    }
    else if (BusTypeNow == CANFD)
    {
        com.transmit_canfd_async(&TesterPresentFD);
    }

    s32 result2 = tsdisturb.tsdisturb_can_disturb_start(handle, &trigger, &sequence, &repetitions, NULL);
    log("startt函数返回值 = %d", result2);

    if (times * cycles < 200)
    {
        //这个延时和读取DTC相关，太小导致干扰未完成，太大导致DTC读取不到
        AppWait(600, ""); //干扰是异步进行的，所以要在这里确保干扰成功执行
    }
    else
    {
        AppWait(2500, ""); //干扰是异步进行的，所以要在这里确保干扰成功执行
    }

    if (BusTypeNow == CAN)
    {
        com.transmit_can_async(&TesterPresent);
    }
    else if (BusTypeNow == CANFD)
    {
        com.transmit_canfd_async(&TesterPresentFD);
    }

    tsdisturb.tsdisturb_stop(handle);

    SendTesterPresent(TP_ON);

    u32 turetimes = 0;
    tsdisturb.tsdisturb_query(handle, &turetimes);
    log("干扰帧数 = %d", turetimes);

    if (gCEMFlg == 1)
    {
        NMWakeUp(1);
        AppWait(200, "");
        SimVehMtnSpdCANFD_Creat();
        SimEngStsCAN_Creat();
        SimIDcDcActLoSideCAN_Creat();
    }
    else
    {
        NMWakeUp(1);
        SimVehMtnGlbSafeCANFD_Creat();
        SimVehBat(gUbat12);
    }

    //断开干扰仪连接
    tsdisturb.tsdisturb_disconnect_all();

    if (turetimes != times*cycles)
    {
        snprintf(StepDespriction, sizeof(StepDespriction), "Interferetimes is %d, expected is %d", turetimes, times * cycles);
        TestReport_HTML_StepDescription(StepDespriction, Warning);
    }

    if (cycles > 31)
    {
        AppWait(61000, "");
    }
}

s32 CheckBusSleepCondition(char cluster[]) {
    u8 checkCntr = 0;
    NMWakeUp(0);
    SendTesterPresent(TP_OFF);
    com.del_cyclic_messages();
    Bus_APP_Chn = NetworkCluster[cluster];
    while (1) {
        Bus_APP_Flag = 0;
        AppWait(1000, "");
        if (Bus_APP_Flag == 0) {
            TestReport_HTML_StepDescription("Meet Slepp Condition; No APP Message", Pass);
            return 0;
        }
        else {
            checkCntr++;
        }
        if (checkCntr >= 255) {
            TestReport_HTML_StepDescription("Timeout Not Meet Slepp Condition", Fail);
            return -1;
        }
    }
    return 0;
}

s32 SimulateAPPMessage(char busType[], char cluster[],u8 state,u16 period) {
    u8 chn = NetworkCluster[cluster];
    char StepDescription[200];
    s16 returnCode = -1;
    TCAN SimulateAPPMessage_CAN = { chn,0,8,0,0x201,0,{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF} };
    TCANFD SimulateAPPMessage_CANFD = { chn ,0,15,0,0x201,0,{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF} };

    if (state == 1) {
        if (strcmp(busType, "CAN") == 0) {
            returnCode = com.add_cyclic_message_can(&SimulateAPPMessage_CAN, period);
            if (returnCode == 0) {
                TestReport_HTML_StepDescription("Add CAN Cyclic APP Message Successful");
            }
            else {
                snprintf(StepDescription,sizeof(StepDescription),"Add CAN Cyclic APP Message Fail,return code is %d",returnCode);
                TestReport_HTML_StepDescription(StepDescription,Fail);
            }
        }
        else if (strcmp(busType, "CANFD") == 0) {
            returnCode = com.add_cyclic_message_canfd(&SimulateAPPMessage_CANFD, period);
            if (returnCode == 0) {
                TestReport_HTML_StepDescription("Add CANFD Cyclic APP Message Successful");
            }
            else {
                snprintf(StepDescription, sizeof(StepDescription), "Add CANFD Cyclic APP Message Fail,return code is %d", returnCode);
                TestReport_HTML_StepDescription(StepDescription, Fail);
            }
        }
        else {
            TestReport_HTML_StepDescription("The Bus Type Is Not CAN Or CANFD", Fail);
            return -1;
        }
    }
    else {
        if (strcmp(busType, "CAN") == 0) {
            returnCode = com.del_cyclic_message_can(&SimulateAPPMessage_CAN);
            if (returnCode == 0) {
                TestReport_HTML_StepDescription("Delete CAN Cyclic APP Message Successful");
            }
            else {
                snprintf(StepDescription, sizeof(StepDescription), "Delete CAN Cyclic APP Message Fail,return code is %d", returnCode);
                TestReport_HTML_StepDescription(StepDescription, Fail);
            }
        }
        else if (strcmp(busType, "CANFD") == 0) {
            returnCode = com.del_cyclic_message_canfd(&SimulateAPPMessage_CANFD);
            if (returnCode == 0) {
                TestReport_HTML_StepDescription("Delete CANFD Cyclic APP Message Successful");
            }
            else {
                snprintf(StepDescription, sizeof(StepDescription), "Delete CANFD Cyclic APP Message Fail,return code is %d", returnCode);
                TestReport_HTML_StepDescription(StepDescription, Fail);
            }
        }
    }
    return 0;
}

//u8 ReturnTestResult()
//{
//    if (resultFlag == 0)
//        return 0;
//    else
//        return 1;
//}

s32 excelParser() {
    void* excelObj = nullptr;
    s32 sheetCount = 0;
    char dirPath[500] = { 0 };
    char excelPath[500] = { 0 };

    char* tempPath = dirPath;  // 让指针指向 dirPath

    app.get_configuration_file_path(&tempPath);

    snprintf(excelPath, sizeof(excelPath), "%s01_Input\\DUT_Test_Para_Conf.xlsx", dirPath);

    if (0 != app.excel_load(excelPath, &excelObj)) {
        app.log_text("Failed to load Excel file", lvlError);
        return -1;
    }

    app.log_text("Excel file loaded successfully", lvlOK);

    if (0 != app.excel_get_sheet_count(excelObj, &sheetCount)) {
        app.log_text("Failed to get sheet count", lvlError);
        app.excel_unload(excelObj);
        return -1;
    }
    

    for (s32 i = 0; i < sheetCount; i++) {
        char sheetName[50] = { 0 };
        char* tempName = sheetName;

        if (0 != app.excel_get_sheet_name(excelObj, i, &tempName)) {
            app.log_text("Failed to get sheet name", lvlError);
            continue;
        }

        if (strncmp(tempName, "GENERAL", 50) == 0) {
            app.log_text("Processing GENERAL sheet", lvlOK);

            s32 rowCount = 0, colCount = 0;
            if (0 != app.excel_get_cell_count(excelObj, i, &rowCount, &colCount)) {
                app.log_text("Failed to get cell count for GENERAL sheet", lvlError);
                continue;
            }

            for (s32 row = 1; row < rowCount; row++) {
                char* varName = nullptr;
                char* varValue = nullptr;

                if (0 == app.excel_get_cell_value(excelObj, i, row, 0, &varName)) {
                    char tempName[100];
                    strncpy(tempName, varName,100);
                    if (0 == app.excel_get_cell_value(excelObj, i, row, 1, &varValue)) {
                        char tempValue[100] = { 0 };
                        strncpy(tempValue, varValue, 100);
                        size_t nameLen = strlen(tempName);
                        size_t valueLen = strlen(tempValue);

                        if (nameLen > 0 && strncmp(tempName, "Manufacturer", nameLen) == 0) {
                            snprintf(Manufacturer, sizeof(Manufacturer), "%s", tempValue);
                            printf("Manufacturer is %s", Manufacturer);
                        }
                        else if (nameLen > 0 && strncmp(tempName, "TestEngineer", nameLen) == 0) {
                            strncpy(TestEngineer, tempValue, sizeof(TestEngineer) - 1);
                            TestEngineer[sizeof(TestEngineer) - 1] = '\0';
                            printf("TestEngineer is %s", TestEngineer);
                        }
                        else if (nameLen > 0 && strncmp(tempName, "DUTName", nameLen) == 0) {
                            strncpy(DUTName, tempValue, sizeof(DUTName) - 1);
                            DUTName[sizeof(DUTName) - 1] = '\0';
                            if (strncmp(DUTName, "ZCUD", 20) == 0) {
                                gCEMFlg = 1;
                            }
                            else {
                                gCEMFlg = 0;
                            }
                            printf("DUTName is %s", DUTName);
                        }
                        else if (nameLen > 0 && strncmp(tempName, "DUTBusName", nameLen) == 0) {
                            strncpy(TpBusName_Temp, tempValue, sizeof(TpBusName_Temp) - 1);
                            TpBusName_Temp[sizeof(TpBusName_Temp) - 1] = '\0';
                            printf("TpBusName_Temp is %s", TpBusName_Temp);
                        }
                        else if (nameLen > 0 && strncmp(tempName, "DUTBusType", nameLen) == 0) {
                            strncpy(TpBusType_Temp, tempValue, sizeof(TpBusType_Temp) - 1);
                            TpBusType_Temp[sizeof(TpBusType_Temp) - 1] = '\0';
                            printf("TpBusType_Temp is %s", TpBusType_Temp);
                            if (strncmp(TpBusType_Temp, "CAN", 10) == 0) {
                                BusTypeNow = CAN;
                                printf("BusTypeNow is CAN");
                            }
                            else if (strncmp(TpBusType_Temp, "CANFD", 10) == 0) {
                                BusTypeNow = CANFD;
                                printf("BusTypeNow is CANFD");
                            }
                            else if (strncmp(TpBusType_Temp, "LIN", 10) == 0) {
                                BusTypeNow = LIN;
                                printf("BusTypeNow is LIN");
                            }
                            else if (strncmp(TpBusType_Temp, "Ethernet", 10) == 0) {
                                BusTypeNow = Ethernet;
                                printf("BusTypeNow is Ethernet");
                            }
                        }
                        else if (nameLen > 0 && strncmp(tempName, "DUTNMBusName", nameLen) == 0) {
                            strncpy(TpNMBusName_Temp, tempValue, sizeof(TpNMBusName_Temp) - 1);
                            TpNMBusName_Temp[sizeof(TpNMBusName_Temp) - 1] = '\0';
                            printf("TpNMBusName_Temp is %s", TpNMBusName_Temp);
                        }
                        else if (nameLen > 0 && strncmp(tempName, "DUTWakeUpType", nameLen) == 0) {
                            strncpy(DUTWakeUpType_Temp, tempValue, sizeof(DUTWakeUpType_Temp) - 1);
                            DUTWakeUpType_Temp[sizeof(DUTWakeUpType_Temp) - 1] = '\0';
                            printf("DUTWakeUpType_Temp is %s", DUTWakeUpType_Temp);
                            if (0 == strncmp(DUTWakeUpType_Temp,"NM",10)) {
                                DUTWakeUpType = _NM;
                                printf("DUTWakeUpType is _NM");
                            }
                            else if (0 == strncmp(DUTWakeUpType_Temp, "KL15", 10)) {
                                DUTWakeUpType = _KL15;
                                printf("DUTWakeUpType is _KL15");
                            }
                            else if (0 == strncmp(DUTWakeUpType_Temp, "NM+KL15", 10)) {
                                DUTWakeUpType = _NM_KL15;
                                printf("DUTWakeUpType is _NM_KL15");
                            }
                        }
                        else if (nameLen > 0 && strncmp(tempName, "CANDiag_FunID", nameLen) == 0) {
                            Diag_Function_ID = strtol(tempValue, nullptr, 0);
                            printf("Diag_Function_ID is 0x%03X", Diag_Function_ID);
                        }
                        else if (nameLen > 0 && strncmp(tempName, "CANDiag_PhyID", nameLen) == 0) {
                             Diag_Request_ID = strtol(tempValue, nullptr, 0);
                            printf("Diag_Request_ID is 0x%03X", Diag_Request_ID);
                        }
                        else if (nameLen > 0 && strncmp(tempName, "CANDiag_ResID", nameLen) == 0) {
                            Diag_Response_ID = strtol(tempValue, nullptr, 0);
                            printf("Diag_Response_ID is 0x%03X", Diag_Response_ID);
                        }
                        else if (nameLen > 0 && strncmp(tempName, "MainPowerSerial", nameLen) == 0) {
                            strncpy(main_power::instance()->main_power_Serial, tempValue,
                                sizeof(main_power::instance()->main_power_Serial) - 1);
                            main_power::instance()->main_power_Serial[sizeof(main_power::instance()->main_power_Serial) - 1] = '\0';

                            printf("\n=== 调试 MainPowerSerial ===\n");
                            printf("字符串内容: '%s'\n", main_power::instance()->main_power_Serial);
                            printf("字符串长度: %zu\n", strlen(main_power::instance()->main_power_Serial));
                            printf("十六进制码: ");
                            for (size_t i = 0; i < strlen(main_power::instance()->main_power_Serial); i++) {
                                printf("%02X ", (unsigned char)main_power::instance()->main_power_Serial[i]);
                            }
                            printf("\n=== 调试结束 ===\n\n");
                        }
                        else if (nameLen > 0 && strncmp(tempName, "PowerType", nameLen) == 0) {
                            //printf("PowerType is %s", tempValue);
                            if (0 == atoi(tempValue)) {
                                PowerType = Power;
                                printf("PowerType is Power");
                            }
                            else if(1 == atoi(tempValue)) {
                                PowerType = ProgramPower;
                                printf("PowerType is ProgramPower");
                            }
                        }
                        else if (nameLen > 0 && strncmp(tempName, "Pincode_0x01", nameLen) == 0) {
                            if (tempValue != nullptr) {
                                size_t len = strlen(tempValue);
                                if (len == 34 && strncmp(tempValue, "0x", 2) == 0) {
                                    const char* hexStr = tempValue + 2;
                                    for (size_t i = 0; i < 16; i++) {
                                        unsigned int byte;
                                        if (sscanf(hexStr + (i * 2), "%2x", &byte) == 1) {
                                            pincode0x01[i] = (uint8_t)byte;
                                            printf("pincode0x01[%d] is 0x%02X", i, pincode0x01[i]);
                                        }
                                        else {
                                            pincode0x01[i] = 0xFF;
                                        }
                                    }
                                }
                                else {
                                    printf("pincode0x01 format is not correct, expected 34 chars with 0x prefix\n");
                                    memset(pincode0x01, 0xFF, 16);
                                }
                            }
                        }
                        else if (nameLen > 0 && strncmp(tempName, "Pincode_0x03", nameLen) == 0) {
                            if (tempValue != nullptr) {
                                size_t len = strlen(tempValue);
                                if (len == 34 && strncmp(tempValue, "0x", 2) == 0) {
                                    const char* hexStr = tempValue + 2;
                                    for (size_t i = 0; i < 16; i++) {
                                        unsigned int byte;
                                        if (sscanf(hexStr + (i * 2), "%2x", &byte) == 1) {
                                            pincode0x03[i] = (uint8_t)byte;
                                            printf("pincode0x03[%d] is 0x%02X", i, pincode0x03[i]);
                                        }
                                        else {
                                            pincode0x03[i] = 0xFF;
                                        }
                                    }
                                }
                                else {
                                    printf("pincode0x03 format is not correct, expected 34 chars with 0x prefix\n");
                                    memset(pincode0x03, 0xFF, 16);
                                }
                            }
                        }
                        else if (nameLen > 0 && strncmp(tempName, "Pincode_0x05", nameLen) == 0) {
                            if (tempValue != nullptr) {
                                size_t len = strlen(tempValue);
                                if (len == 34 && strncmp(tempValue, "0x", 2) == 0) {
                                    const char* hexStr = tempValue + 2;
                                    for (size_t i = 0; i < 16; i++) {
                                        unsigned int byte;
                                        if (sscanf(hexStr + (i * 2), "%2x", &byte) == 1) {
                                            pincode0x05[i] = (uint8_t)byte;
                                            printf("pincode0x05[%d] is 0x%02X", i, pincode0x05[i]);
                                        }
                                        else {
                                            pincode0x05[i] = 0xFF;
                                        }
                                    }
                                }
                                else {
                                    printf("pincode0x05 format is not correct, expected 34 chars with 0x prefix\n");
                                    memset(pincode0x05, 0xFF, 16);
                                }
                            }
                        }
                        else if (nameLen > 0 && strncmp(tempName, "Pincode_0x11", nameLen) == 0) {
                            if (tempValue != nullptr) {
                                size_t len = strlen(tempValue);
                                if (len == 34 && strncmp(tempValue, "0x", 2) == 0) {
                                    const char* hexStr = tempValue + 2;
                                    for (size_t i = 0; i < 16; i++) {
                                        unsigned int byte;
                                        if (sscanf(hexStr + (i * 2), "%2x", &byte) == 1) {
                                            pincode0x11[i] = (uint8_t)byte;
                                            printf("pincode0x11[%d] is 0x%02X", i, pincode0x11[i]);
                                        }
                                        else {
                                            pincode0x11[i] = 0xFF;
                                        }
                                    }
                                }
                                else {
                                    printf("pincode0x11 format is not correct, expected 34 chars with 0x prefix\n");
                                    memset(pincode0x11, 0xFF, 16);
                                }
                            }
                        }
                        else if (nameLen > 0 && strncmp(tempName, "Pincode_0x19", nameLen) == 0) {
                            if (tempValue != nullptr) {
                                size_t len = strlen(tempValue);
                                if (len == 34 && strncmp(tempValue, "0x", 2) == 0) {
                                    const char* hexStr = tempValue + 2;
                                    for (size_t i = 0; i < 16; i++) {
                                        unsigned int byte;
                                        if (sscanf(hexStr + (i * 2), "%2x", &byte) == 1) {
                                            pincode0x19[i] = (uint8_t)byte;
                                            printf("pincode0x19[%d] is 0x%02X", i, pincode0x19[i]);
                                        }
                                        else {
                                            pincode0x19[i] = 0xFF;
                                        }
                                    }
                                }
                                else {
                                    printf("pincode0x19 format is not correct, expected 34 chars with 0x prefix\n");
                                    memset(pincode0x19, 0xFF, 16);
                                }
                            }
                        }
                        else if (nameLen > 0 && strncmp(tempName, "CarConfigBus", nameLen) == 0) {
                            snprintf(CarConfigBus_Temp,sizeof(CarConfigBus_Temp),"%s", tempValue);
                            printf("CarConfigBus is %s", CarConfigBus_Temp);
                            if (strstr(CarConfigBus_Temp, "CANFD") != nullptr) {
                                g_CCPIsCanFD = true;
                            }
                            else {
                                g_CCPIsCanFD = false;
                            }
                        }
                        else if (nameLen > 0 && strncmp(tempName, "LowPowerTime", nameLen) == 0) {
                            gSleepTime = atoi(tempValue) * 1000;
                            printf("gSleepTime is %dms", gSleepTime);
                        }
                    }
                    //if (varValue) free(varValue);
                    varValue = nullptr;
                }
                //if (varName) free(varName);
                varName = nullptr;
            }
        }
        
        else if (strncmp(tempName, "Mapping", 50) == 0) {
            app.log_text("Processing Mapping sheet", lvlOK);

            s32 rowCount = 0, colCount = 0;
            if (0 != app.excel_get_cell_count(excelObj, i, &rowCount, &colCount)) {
                app.log_text("Failed to get cell count for Mapping sheet", lvlError);
                continue;
            }

            for (s32 row = 1; row < rowCount; row++) {
                char* networkName = nullptr;
                char* channel = nullptr;
                if (0 == app.excel_get_cell_value(excelObj, i, row, 0, &networkName) && networkName) {
                    char tempNetwork[100];
                    strncpy(tempNetwork, networkName, 100);
                    if (0 == app.excel_get_cell_value(excelObj, i, row, 1, &channel)) {
                        char tempChannel[100];
                        strncpy(tempChannel, channel, 100);
                        NetworkCluster[tempNetwork] = atoi(tempChannel) - 1;
                        printf("NetworkCluster[%s] = %d", tempNetwork, NetworkCluster[tempNetwork]);
                    }
                    //if (channel) free(channel);
                }
                //if (networkName) free(networkName);
            }
        }
        /*
        // 处理SWDL sheet
        else if (strncmp(tempName, "SWDL", 50) == 0) {
            app.log_text("Processing SWDL sheet", lvlOK);

            s32 rowCount = 0, colCount = 0;
            if (0 != app.excel_get_cell_count(excelObj, i, &rowCount, &colCount)) {
                app.log_text("Failed to get cell count for SWDL sheet", lvlError);
                continue;
            }

            for (s32 row = 1; row < rowCount; row++) {
                char* paramName = nullptr;
                char* paramValue = nullptr;

                if (0 == app.excel_get_cell_value(excelObj, i, row, 0, &paramName) && paramName) {
                    if (0 == app.excel_get_cell_value(excelObj, i, row, 1, &paramValue) && paramValue) {
                        size_t paramLen = strlen(paramName);

                        if (paramLen > 0 && strncmp(paramName, "RobutnessTimes", paramLen) == 0) {
                            RobutnessTimes = atoi(paramValue);
                        }
                    }
                    if (paramValue) free(paramValue);
                }
                if (paramName) free(paramName);
            }
            
        }*/
    }
    app.excel_unload(excelObj);
    return 0;
}

s32 DTCExcelParser() {
    void* excelObj = nullptr;
    s32 sheetCount = 0;
    char excelPath[500] = "";
    char* tempPath = excelPath;
    s32 parseResult = 0;

    app.get_configuration_file_path(&tempPath);
    snprintf(excelPath, sizeof(excelPath), "%s01_Input\\DTC.xlsx", tempPath);

    if (0 != app.excel_load(excelPath, &excelObj)) {
        app.log_text("Failed to load DTC.xlsx file", lvlError);
        return -1;
    }
    app.log_text("DTC.xlsx file loaded successfully", lvlOK);

    if (0 != app.excel_get_sheet_count(excelObj, &sheetCount)) {
        app.log_text("Failed to get DTC sheet count", lvlError);
        app.excel_unload(excelObj);
        return -1;
    }

    for (s32 i = 0; i < sheetCount; i++) {
        char sheetName[50] = { 0 };
        char* tempName = sheetName;

        if (0 != app.excel_get_sheet_name(excelObj, i, &tempName)) {
            app.log_text("Failed to get DTC sheet name", lvlError);
            continue;
        }

        // 解析 MissFrameDTC sheet
        if (strncmp(tempName, "MissFrameDTC", 50) == 0) {
            app.log_text("Processing MissFrameDTC sheet", lvlOK);

            s32 rowCount = 0, colCount = 0;
            if (0 != app.excel_get_cell_count(excelObj, i, &rowCount, &colCount)) {
                app.log_text("Failed to get cell count for MissFrameDTC sheet", lvlError);
                continue;
            }
            MISMSG_DTC_NUM = rowCount - 1;
            std::vector<std::string> signalNames;
            std::vector<std::string> dbNames;
            std::vector<SignalLookupResult> signalResults;
            for (s32 row = 1; row < rowCount && row <= MISMSG_DTC_NUM; row++) {
                char* varValue = nullptr;
                char* varValue2 = nullptr;
                if (0 == app.excel_get_cell_value(excelObj, i, row, 4, &varValue) && varValue) {
                    signalNames.push_back(varValue);
                    //if (varValue) free(varValue);
                }
                if (0 == app.excel_get_cell_value(excelObj, i, row, 1, &varValue2) && varValue2) {
                    dbNames.push_back(varValue2);
                    //if (varValue) free(varValue);
                }
            }

            std::vector<SignalLookupResult> result(signalNames.size());
            for (size_t i = 0; i < result.size(); i++) {
                result[i].signalName = signalNames[i];
                result[i].dbName = dbNames[i];
                result[i].ubStartBit = -1;
                printf("result[%d].signalName = %s",i + 1, signalNames[i].c_str());
            }

            s32 dbCount = 0;
            if (0 == app.db_get_can_database_count(&dbCount)) {
                printf("CAN database count = %d", dbCount);

                
                for (s32 dbIdx = 0; dbIdx < dbCount; dbIdx++) {
                    TDBProperties db;
                    db.FDBIndex = dbIdx;
                    if (0 != app.db_get_can_database_properties_by_index(&db)) continue;

                    //if (strncmp(result[dbIdx].dbName.c_str(), db.FName, 12) != 0) continue;

                    for (s32 ecuIdx = 0; ecuIdx < db.FECUCount; ecuIdx++) {
                        TDBECUProperties ecu = { 0 };
                        ecu.FDBIndex = dbIdx;
                        ecu.FECUIndex = ecuIdx;
                        if (0 != app.db_get_can_ecu_properties_by_index(&ecu)) continue;
                        if (strstr(ecu.FName, DUTName) != nullptr) continue;
                        for (s32 msgIdx = 0; msgIdx < ecu.FRxFrameCount; msgIdx++) {
                            TDBFrameProperties msg = { 0 };
                            msg.FDBIndex = dbIdx;
                            msg.FECUIndex = ecuIdx;
                            msg.FFrameIndex = msgIdx;
                            msg.FIsTx = 0;
                            if (0 != app.db_get_can_frame_properties_by_index(&msg)) continue;
                            //printf("msg.FIsTx is %d", msg.FIsTx);

                            for (s32 sigIdxDb = 0; sigIdxDb < msg.FSignalCount; sigIdxDb++) {
                                TDBSignalProperties sgn;
                                sgn.FDBIndex = dbIdx;
                                sgn.FECUIndex = ecuIdx;
                                sgn.FFrameIndex = msgIdx;
                                sgn.FSignalIndex = sigIdxDb;
                                sgn.FIsTx = msg.FIsTx;

                                if (0 != app.db_get_can_signal_properties_by_index(&sgn)) continue;

                                for (size_t resultIndex = 0; resultIndex < result.size(); resultIndex++) {
                                    // 检查信号
                                    if (sgn.FName == result[resultIndex].signalName) {
                                        result[resultIndex].messageId = msg.FCANIdentifier;
                                        result[resultIndex].cycle = msg.FCycleTimeMs;
                                        result[resultIndex].isFD = msg.FCANIsEdl;
                                        result[resultIndex].dbName = db.FName;
                                        result[resultIndex].DLC = msg.FCANDLC;

                                        printf("[%s]Found signal '%s' in DB '%s', Msg ID=0x%03X",
                                            db.FName,
                                            result[resultIndex].signalName.c_str(),
                                            result[resultIndex].dbName.c_str(),
                                            result[resultIndex].messageId);
                                    }

                                    // 检查UB信号
                                    std::string ubSignalName = result[resultIndex].signalName + "_UB";
                                    if (sgn.FName == ubSignalName) {
                                        result[resultIndex].ubStartBit = sgn.FCANSignal.FStartBit;
                                        printf("Found UB signal '%s' with startBit=%d",
                                            ubSignalName.c_str(), result[resultIndex].ubStartBit);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            s32 dtcIndex = 0;
            for (s32 row = 1; row < rowCount && dtcIndex < MISMSG_DTC_NUM; row++) {
                char* varValue = nullptr;
                misMsgDTCList[dtcIndex].tstRsltFlg = 1;
                // node
                if (0 == app.excel_get_cell_value(excelObj, i, row, 0, &varValue) && varValue) {
                    strncpy(misMsgDTCList[dtcIndex].node, varValue, sizeof(misMsgDTCList[dtcIndex].node) - 1);
                    misMsgDTCList[dtcIndex].node[sizeof(misMsgDTCList[dtcIndex].node) - 1] = '\0';
                    printf("misMsgDTCList node is %s", misMsgDTCList[dtcIndex].node);
                    //free(varValue); varValue = nullptr;
                }

                // cluster
                if (0 == app.excel_get_cell_value(excelObj, i, row, 1, &varValue) && varValue) {
                    strncpy(misMsgDTCList[dtcIndex].cluster, varValue, sizeof(misMsgDTCList[dtcIndex].cluster) - 1);
                    misMsgDTCList[dtcIndex].cluster[sizeof(misMsgDTCList[dtcIndex].cluster) - 1] = '\0';
                    printf("misMsgDTCList cluster is %s", misMsgDTCList[dtcIndex].cluster);
                    MissingFrame_CHANNEL_PRIORITY[row - 1][0] = NetworkCluster[misMsgDTCList[dtcIndex].cluster];
                    snprintf(g_MissFrame_path[row - 1].FDBName, sizeof(g_MissFrame_path[row - 1].FDBName), "%s", misMsgDTCList[dtcIndex].cluster);
                    //free(varValue); varValue = nullptr;
                }

                // busType
                if (0 == app.excel_get_cell_value(excelObj, i, row, 2, &varValue) && varValue) {
                    strncpy(misMsgDTCList[dtcIndex].busType, varValue, sizeof(misMsgDTCList[dtcIndex].busType) - 1);
                    misMsgDTCList[dtcIndex].busType[sizeof(misMsgDTCList[dtcIndex].busType) - 1] = '\0';
                    printf("misMsgDTCList busType is %s", misMsgDTCList[dtcIndex].busType);
                    //free(varValue); varValue = nullptr;
                }

                // dtc
                if (0 == app.excel_get_cell_value(excelObj, i, row, 3, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].dtc = strtoul(varValue, nullptr, 16);
                    printf("misMsgDTCList dtc is 0x%06X", misMsgDTCList[dtcIndex].dtc);
                    //free(varValue); varValue = nullptr;
                }
                // MsgFrame
                if (0 == app.excel_get_cell_value(excelObj, i, row, 4, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].signalResult[0] = result[dtcIndex];
                    printf("misMsgDTCList [%s] ID is 0x%03X,signalName is %s", 
                        misMsgDTCList[dtcIndex].signalResult[0].dbName.c_str(),
                        misMsgDTCList[dtcIndex].signalResult[0].messageId,
                        misMsgDTCList[dtcIndex].signalResult[0].signalName.c_str());
                    snprintf(g_MissFrame_path[row - 1].FSignalName, sizeof(g_MissFrame_path[row - 1].FSignalName), "%s", misMsgDTCList[dtcIndex].signalResult[0].signalName.c_str());
                }
                // usgmod
                if (0 == app.excel_get_cell_value(excelObj, i, row, 5, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].usgmod = atoi(varValue);
                    printf("misMsgDTCList usgmod is %d", misMsgDTCList[dtcIndex].usgmod);
                    //free(varValue); varValue = nullptr;
                }
                // testProd
                if (0 == app.excel_get_cell_value(excelObj, i, row, 6, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].testProd = atoi(varValue);
                    printf("misMsgDTCList testProd is %d", misMsgDTCList[dtcIndex].testProd);
                    //free(varValue); varValue = nullptr;
                }
                // SUValue
                if (0 == app.excel_get_cell_value(excelObj, i, row, 7, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].SUValue = atoi(varValue);
                    printf("misMsgDTCList SUValue is %d", misMsgDTCList[dtcIndex].SUValue);
                    //free(varValue); varValue = nullptr;
                }

                // SDValue
                if (0 == app.excel_get_cell_value(excelObj, i, row, 8, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].SDValue = atoi(varValue);
                    printf("misMsgDTCList SDValue is %d", misMsgDTCList[dtcIndex].SDValue);
                    //free(varValue); varValue = nullptr;
                }

                // UnCofrmLmt
                if (0 == app.excel_get_cell_value(excelObj, i, row, 9, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].UnCofrmLmt = atoi(varValue);
                    printf("misMsgDTCList UnCofrmLmt is %d", misMsgDTCList[dtcIndex].UnCofrmLmt);
                    //free(varValue); varValue = nullptr;
                }

                // CofrmLmt
                if (0 == app.excel_get_cell_value(excelObj, i, row, 10, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].CofrmLmt = atoi(varValue);
                    printf("misMsgDTCList CofrmLmt is %d", misMsgDTCList[dtcIndex].CofrmLmt);
                    //free(varValue); varValue = nullptr;
                }

                // agdLmt
                if (0 == app.excel_get_cell_value(excelObj, i, row, 11, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].agdLmt = atoi(varValue);
                    printf("misMsgDTCList agdLmt is %d", misMsgDTCList[dtcIndex].agdLmt);
                    //free(varValue); varValue = nullptr;
                }

                // CCPPsn20
                if (0 == app.excel_get_cell_value(excelObj, i, row, 12, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].CCPPsn20 = atoi(varValue);
                    printf("misMsgDTCList CCPPsn20 is %d", misMsgDTCList[dtcIndex].CCPPsn20);
                    //free(varValue); varValue = nullptr;
                }

                // CCPValue20
                if (0 == app.excel_get_cell_value(excelObj, i, row, 13, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].CCPValue20 = atoi(varValue);
                    printf("misMsgDTCList CCPValue20 is %d", misMsgDTCList[dtcIndex].CCPValue20);
                    //free(varValue); varValue = nullptr;
                }

                // CCPValueNotMet20
                if (0 == app.excel_get_cell_value(excelObj, i, row, 14, &varValue) && varValue) {
                    misMsgDTCList[dtcIndex].CCPValueNotMet20 = atoi(varValue);
                    printf("misMsgDTCList CCPValueNotMet20 is %d", misMsgDTCList[dtcIndex].CCPValueNotMet20);
                    //free(varValue); varValue = nullptr;
                }

                dtcIndex++;
            }
            ActualMissFrameCount = dtcIndex;
            printf("MissFrame DTC Number is %d", ActualMissFrameCount);
        }

        // 解析 HighVoltageDTC sheet
        else if (strncmp(tempName, "VoltDTC", 50) == 0) {
            app.log_text("Processing VoltageDTC sheet", lvlOK);
            char* varValue = nullptr;
            
            if (0 == app.excel_get_cell_value(excelObj, i, 1, 0, &varValue) && varValue) {
                if (strncmp("HighVolt", varValue, 20) == 0) {
                    highVoltDTC.tstRlst = 1;
                    if (0 == app.excel_get_cell_value(excelObj, i, 1, 1, &varValue) && varValue) {
                        //highVoltDTC.dtc = atoi(varValue);
                        highVoltDTC.dtc = strtoul(varValue, nullptr, 16);
                        //app.log_text("highVoltDTC dtc", lvlHint);
                        printf("highVoltDTC dtc is 0x%06X", highVoltDTC.dtc);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 1, 2, &varValue) && varValue) {
                        highVoltDTC.volt = atof(varValue);
                        printf("highVoltDTC volt is %.2lf", highVoltDTC.volt);
                       // free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 1, 3, &varValue) && varValue) {
                        highVoltDTC.usgMod = atoi(varValue);
                        printf("highVoltDTC usgMod is 0x%X", highVoltDTC.usgMod);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 1, 4, &varValue) && varValue) {
                        highVoltDTC.agedLmt = atoi(varValue);
                        printf("highVoltDTC agedLmt is %d", highVoltDTC.agedLmt);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 1, 5, &varValue) && varValue) {
                        
                        highVoltDTC.testProd = atoi(varValue);
                        printf("highVoltDTC testProd is %d", highVoltDTC.testProd);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 1, 6, &varValue) && varValue) {
                        highVoltDTC.SUValue = atoi(varValue);
                        printf("highVoltDTC SUValue is %d", highVoltDTC.SUValue);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 1, 7, &varValue) && varValue) {
                        highVoltDTC.SDValue = atoi(varValue);
                        printf("highVoltDTC SDValue is %d", highVoltDTC.SDValue);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 1, 8, &varValue) && varValue) {
                        highVoltDTC.UnCofrmLmt = atoi(varValue);
                        printf("highVoltDTC UnCofrmLmt is %d", highVoltDTC.UnCofrmLmt);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 1, 9, &varValue) && varValue) {
                        highVoltDTC.CofrmLmt = atoi(varValue);
                        printf("highVoltDTC CofrmLmt is %d", highVoltDTC.CofrmLmt);
                        //free(varValue); varValue = tempValue;
                    }
                }
            }
            if (0 == app.excel_get_cell_value(excelObj, i, 2, 0, &varValue) && varValue) {
                if (strncmp("LowVolt", varValue, 20) == 0) {
                    lowVoltDTC.tstRlst = 1;
                    if (0 == app.excel_get_cell_value(excelObj, i, 2, 1, &varValue) && varValue) {
                        lowVoltDTC.dtc = strtoul(varValue, nullptr, 16);
                        printf("lowVoltDTC dtc is 0x%06X", lowVoltDTC.dtc);
                        //free(varValue); varValue = tempValue;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 2, 2, &varValue) && varValue) {
                        lowVoltDTC.volt = atof(varValue);
                        printf("lowVoltDTC volt is %.2lf", lowVoltDTC.volt);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 2, 3, &varValue) && varValue) {
                        lowVoltDTC.usgMod = atoi(varValue);
                        printf("lowVoltDTC usgMod is 0x%X", lowVoltDTC.usgMod);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 2, 4, &varValue) && varValue) {
                        lowVoltDTC.agedLmt = atoi(varValue);
                        printf("lowVoltDTC agedLmt is %d", lowVoltDTC.agedLmt);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 2, 5, &varValue) && varValue) {
                        lowVoltDTC.testProd = atoi(varValue);
                        printf("lowVoltDTC testProd is %d", lowVoltDTC.testProd);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 2, 6, &varValue) && varValue) {
                        lowVoltDTC.SUValue = atoi(varValue);
                        printf("lowVoltDTC SUValue is %d", lowVoltDTC.SUValue);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 2, 7, &varValue) && varValue) {
                        lowVoltDTC.SDValue = atoi(varValue);
                        printf("lowVoltDTC SDValue is %d", lowVoltDTC.SDValue);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 2, 8, &varValue) && varValue) {
                        lowVoltDTC.UnCofrmLmt = atoi(varValue);
                        printf("lowVoltDTC UnCofrmLmt is %d", lowVoltDTC.UnCofrmLmt);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 2, 9, &varValue) && varValue) {
                        lowVoltDTC.CofrmLmt = atoi(varValue);
                        printf("lowVoltDTC CofrmLmt is %d", lowVoltDTC.CofrmLmt);
                        //free(varValue); varValue = nullptr;
                    }
                }
            }
            if (0 == app.excel_get_cell_value(excelObj, i, 3, 0, &varValue) && varValue) {
                if (strncmp("ComparedFailureDTC", varValue, 20) == 0) {
                    batVoltDTC.tstRlst = 1;
                    if (0 == app.excel_get_cell_value(excelObj, i, 3, 1, &varValue) && varValue) {
                        batVoltDTC.dtc = strtoul(varValue, nullptr, 16);
                        printf("batVoltDTC dtc is 0x%06X", batVoltDTC.dtc);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 3, 2, &varValue) && varValue) {
                        batVoltDTC.volt = atof(varValue);
                        printf("batVoltDTC volt is %.2lf", batVoltDTC.volt);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 3, 3, &varValue) && varValue) {
                        batVoltDTC.usgMod = atoi(varValue);
                        printf("batVoltDTC usgMod is 0x%X", batVoltDTC.usgMod);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 3, 4, &varValue) && varValue) {
                        batVoltDTC.agedLmt = atoi(varValue);
                        printf("batVoltDTC agedLmt is %d", batVoltDTC.agedLmt);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 3, 3, &varValue) && varValue) {
                        batVoltDTC.testProd = atoi(varValue);
                        printf("batVoltDTC testProd is %d", batVoltDTC.testProd);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 3, 6, &varValue) && varValue) {
                        batVoltDTC.SUValue = atoi(varValue);
                        printf("batVoltDTC SUValue is %d", batVoltDTC.SUValue);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 3, 7, &varValue) && varValue) {
                        batVoltDTC.SDValue = atoi(varValue);
                        printf("batVoltDTC SDValue is %d", batVoltDTC.SDValue);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 3, 8, &varValue) && varValue) {
                        batVoltDTC.UnCofrmLmt = atoi(varValue);
                        printf("batVoltDTC UnCofrmLmt is %d", batVoltDTC.UnCofrmLmt);
                        //free(varValue); varValue = nullptr;
                    }
                    if (0 == app.excel_get_cell_value(excelObj, i, 3, 9, &varValue) && varValue) {
                        batVoltDTC.CofrmLmt = atoi(varValue);
                        printf("batVoltDTC CofrmLmt is %d", batVoltDTC.CofrmLmt);
                        //free(varValue); varValue = nullptr;
                    }
                }
            }
        }

        // 解析 BusOffDTC sheet
        else if (strncmp(tempName, "BusFault", 50) == 0) {
            app.log_text("Processing BusOffDTC sheet", lvlOK);

            s32 rowCount = 0, colCount = 0;
            if (0 != app.excel_get_cell_count(excelObj, i, &rowCount, &colCount)) {
                app.log_text("Failed to get cell count for BusOffDTC sheet", lvlError);
                continue;
            }
            BusNum = rowCount;
            s32 busIndex = 0;
            for (s32 row = 1; row < rowCount && busIndex < BusNum; row++) {
                char* varValue = nullptr;
                busFltDTCTemp[busIndex].TestRslt = 1;
                // cluster
                if (0 == app.excel_get_cell_value(excelObj, i, row, 0, &varValue) && varValue) {
                    strncpy(busFltDTCTemp[busIndex].cluster, varValue, sizeof(busFltDTCTemp[busIndex].cluster) - 1);
                    busFltDTCTemp[busIndex].cluster[sizeof(busFltDTCTemp[busIndex].cluster) - 1] = '\0';
                    printf("busFltDTCTemp cluster is %s", busFltDTCTemp[busIndex].cluster);
                    //free(varValue);
                    //varValue = nullptr;
                }

                // busType
                if (0 == app.excel_get_cell_value(excelObj, i, row, 1, &varValue) && varValue) {
                    strncpy(busFltDTCTemp[busIndex].busType, varValue, sizeof(busFltDTCTemp[busIndex].busType) - 1);
                    busFltDTCTemp[busIndex].busType[sizeof(busFltDTCTemp[busIndex].busType) - 1] = '\0';
                    printf("busFltDTCTemp busType is %s", busFltDTCTemp[busIndex].busType);
                    //free(varValue); varValue = nullptr;
                }

                // busOffDTC
                if (0 == app.excel_get_cell_value(excelObj, i, row, 2, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].busOffDTC = strtoul(varValue, nullptr, 16);
                    printf("busFltDTCTemp busOffDTC is 0x%06X", busFltDTCTemp[busIndex].busOffDTC);
                    //free(varValue); varValue = nullptr;
                }

                // UsgMod
                if (0 == app.excel_get_cell_value(excelObj, i, row, 3, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].UsgMod = atoi(varValue);
                    printf("busFltDTCTemp UsgMod is 0x%X", busFltDTCTemp[busIndex].UsgMod);
                    //free(varValue); varValue = nullptr;
                }

                // Volt
                if (0 == app.excel_get_cell_value(excelObj, i, row, 4, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].Volt = atoi(varValue);
                    printf("busFltDTCTemp Volt is %.2lf", busFltDTCTemp[busIndex].Volt);
                    //free(varValue); varValue = nullptr;
                }

                // bsfUcfm
                if (0 == app.excel_get_cell_value(excelObj, i, row, 5, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].bsfUcfm = atoi(varValue);
                    printf("busFltDTCTemp bsfUcfm is %d", busFltDTCTemp[busIndex].bsfUcfm);
                    //free(varValue); varValue = nullptr;
                }

                // bsfCfm
                if (0 == app.excel_get_cell_value(excelObj, i, row, 6, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].bsfCfm = atoi(varValue);
                    printf("busFltDTCTemp bsfCfm is %d", busFltDTCTemp[busIndex].bsfCfm);
                    //free(varValue); varValue = nullptr;
                }

                // ElctrFltDTC
                if (0 == app.excel_get_cell_value(excelObj, i, row, 7, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].ElctrFltDTC = strtoul(varValue, nullptr, 16);
                    printf("busFltDTCTemp ElctrFltDTC is 0x%06X", busFltDTCTemp[busIndex].ElctrFltDTC);
                    //free(varValue); varValue = nullptr;
                }

                // testProd
                if (0 == app.excel_get_cell_value(excelObj, i, row, 8, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].testProd = atoi(varValue);
                    printf("busFltDTCTemp testProd is %d", busFltDTCTemp[busIndex].testProd);
                    //free(varValue); varValue = nullptr;
                }

                // SUValue
                if (0 == app.excel_get_cell_value(excelObj, i, row, 9, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].SUValue = atoi(varValue);
                    printf("busFltDTCTemp SUValue is %d", busFltDTCTemp[busIndex].SUValue);
                    //free(varValue); varValue = nullptr;
                }

                // SDvalue
                if (0 == app.excel_get_cell_value(excelObj, i, row, 10, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].SDvalue = atoi(varValue);
                    printf("busFltDTCTemp SDvalue is %d", busFltDTCTemp[busIndex].SDvalue);
                    //free(varValue); varValue = nullptr;
                }

                // UnCofrmLmt
                if (0 == app.excel_get_cell_value(excelObj, i, row, 11, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].UnCofrmLmt = atoi(varValue);
                    printf("busFltDTCTemp UnCofrmLmt is %d", busFltDTCTemp[busIndex].UnCofrmLmt);
                    //free(varValue); varValue = nullptr;
                }

                // CofrmLmt
                if (0 == app.excel_get_cell_value(excelObj, i, row, 12, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].CofrmLmt = atoi(varValue);
                    printf("busFltDTCTemp CofrmLmt is %d", busFltDTCTemp[busIndex].CofrmLmt);
                    //free(varValue); varValue = nullptr;
                }

                // AgeCounter
                if (0 == app.excel_get_cell_value(excelObj, i, row, 13, &varValue) && varValue) {
                    busFltDTCTemp[busIndex].ageCounter = atoi(varValue);
                    printf("busFltDTCTemp ageCounter is %d", busFltDTCTemp[busIndex].ageCounter);
                    //free(varValue); varValue = nullptr;
                }
                //if (varValue) 
                //    free(varValue);
                busIndex++;
            }
        }

        // 解析 MissResponseDTC sheet
        else if (strncmp(tempName, "MissResponse", 50) == 0) {
            app.log_text("Processing MissResponse sheet", lvlOK);

            s32 rowCount = 0, colCount = 0;
            if (0 != app.excel_get_cell_count(excelObj, i, &rowCount, &colCount)) {
                app.log_text("Failed to get cell count for MissResponse sheet", lvlError);
                continue;
            }
            MISRESP_DTC_NUM = rowCount - 1;

            s32 dtcIndex = 0;
            for (s32 row = 1; row < rowCount && dtcIndex < MISRESP_DTC_NUM; row++) {
                char* varValue = nullptr;
                misResponseDTCList[dtcIndex].tstRsltFlg = 1;
                // node
                if (0 == app.excel_get_cell_value(excelObj, i, row, 1, &varValue) && varValue) {
                    snprintf(misResponseDTCList[dtcIndex].node, sizeof(misResponseDTCList[dtcIndex].node), "%s", varValue);
                    printf("misResponseDTCList node is %s", misResponseDTCList[dtcIndex].node);
                    //free(varValue); varValue = nullptr;
                }

                // cluster
                if (0 == app.excel_get_cell_value(excelObj, i, row, 0, &varValue) && varValue) {
                    snprintf(misResponseDTCList[dtcIndex].cluster, sizeof(misResponseDTCList[dtcIndex].cluster), "%s", varValue);
                    printf("misResponseDTCList cluster is %s", misResponseDTCList[dtcIndex].cluster);
                }

                // dtc
                if (0 == app.excel_get_cell_value(excelObj, i, row, 2, &varValue) && varValue) {
                    misResponseDTCList[dtcIndex].dtc = strtoul(varValue, nullptr, 16);
                    printf("misResponseDTCList dtc is 0x%06X", misResponseDTCList[dtcIndex].dtc);
                    //free(varValue); varValue = nullptr;
                }
                
                // testProd
                if (0 == app.excel_get_cell_value(excelObj, i, row, 3, &varValue) && varValue) {
                    misResponseDTCList[dtcIndex].testProd = atoi(varValue);
                    printf("misResponseDTCList testProd is %d", misResponseDTCList[dtcIndex].testProd);
                    //free(varValue); varValue = nullptr;
                }
                // SUValue
                if (0 == app.excel_get_cell_value(excelObj, i, row, 4, &varValue) && varValue) {
                    misResponseDTCList[dtcIndex].SUValue = atoi(varValue);
                    printf("misResponseDTCList SUValue is %d", misResponseDTCList[dtcIndex].SUValue);
                    //free(varValue); varValue = nullptr;
                }

                // SDValue
                if (0 == app.excel_get_cell_value(excelObj, i, row, 5, &varValue) && varValue) {
                    misResponseDTCList[dtcIndex].SDValue = atoi(varValue);
                    printf("misResponseDTCList SDValue is %d", misResponseDTCList[dtcIndex].SDValue);
                    //free(varValue); varValue = nullptr;
                }

                // UnCofrmLmt
                if (0 == app.excel_get_cell_value(excelObj, i, row, 6, &varValue) && varValue) {
                    misResponseDTCList[dtcIndex].UnCofrmLmt = atoi(varValue);
                    printf("misResponseDTCList UnCofrmLmt is %d", misResponseDTCList[dtcIndex].UnCofrmLmt);
                    //free(varValue); varValue = nullptr;
                }

                // CofrmLmt
                if (0 == app.excel_get_cell_value(excelObj, i, row, 7, &varValue) && varValue) {
                    misResponseDTCList[dtcIndex].CofrmLmt = atoi(varValue);
                    printf("misResponseDTCList CofrmLmt is %d", misResponseDTCList[dtcIndex].CofrmLmt);
                    //free(varValue); varValue = nullptr;
                }

                // agdLmt
                if (0 == app.excel_get_cell_value(excelObj, i, row, 8, &varValue) && varValue) {
                    misResponseDTCList[dtcIndex].agdLmt = atoi(varValue);
                    printf("misResponseDTCList agdLmt is %d", misResponseDTCList[dtcIndex].agdLmt);
                    //free(varValue); varValue = nullptr;
                }

                // CCPPsn20
                if (0 == app.excel_get_cell_value(excelObj, i, row, 9, &varValue) && varValue) {
                    misResponseDTCList[dtcIndex].CCPPsn20 = atoi(varValue);
                    printf("misResponseDTCList CCPPsn20 is %d", misResponseDTCList[dtcIndex].CCPPsn20);
                    //free(varValue); varValue = nullptr;
                }

                // CCPValue20
                if (0 == app.excel_get_cell_value(excelObj, i, row, 10, &varValue) && varValue) {
                    misResponseDTCList[dtcIndex].CCPValue20 = atoi(varValue);
                    printf("misResponseDTCList CCPValue20 is %d", misResponseDTCList[dtcIndex].CCPValue20);
                    //free(varValue); varValue = nullptr;
                }

                dtcIndex++;
            }
            ActualMissResponseCount = dtcIndex;
            printf("misResponse DTC Number is %d", ActualMissResponseCount);
        }

    }

    app.excel_unload(excelObj);
    log("DTC.xlsx parsing completed");
    return 0;
}

int FindSignalPath(const char* dut_name, const char* signalName, TSignalPath* out_path,int CHANNEL_PRIORITY_COUNT,const u32 CHANNEL_PRIORITY[]) {
    s32 i, j, k, m, n;
    int prio_idx;

    if (!dut_name || !out_path) return -1;

    for (prio_idx = 0; prio_idx < CHANNEL_PRIORITY_COUNT; prio_idx++) {
        u32 target_channel = CHANNEL_PRIORITY[prio_idx];

        if (0 != app.db_get_can_database_count(&n)) continue;

        for (i = 0; i < n; i++) {
            TDBProperties db;
            db.FDBIndex = i;
            if (0 != app.db_get_can_database_properties_by_index(&db)) continue;

            if (NetworkCluster[db.FName] != target_channel) {
                log("NetworkCluster[%s] = %d, target_channel is %d,非优先级通道", db.FName, NetworkCluster[db.FName], target_channel);
                continue;
            }
            log("NetworkCluster[%s] = %d, target_channel is %d,优先级通道", db.FName, NetworkCluster[db.FName], target_channel);

            for (j = 0; j < db.FECUCount; j++) {
                TDBECUProperties ecu;
                ecu.FDBIndex = i;
                ecu.FECUIndex = j;
                if (0 != app.db_get_can_ecu_properties_by_index(&ecu)) continue;

                if (strcmp(ecu.FName, dut_name) == 0) continue;

                for (k = 0; k < ecu.FTxFrameCount; k++) {
                    TDBFrameProperties msg;
                    msg.FDBIndex = i;
                    msg.FECUIndex = j;
                    msg.FFrameIndex = k;
                    msg.FIsTx = 1;
                    if (0 != app.db_get_can_frame_properties_by_index(&msg)) continue;

                    for (m = 0; m < msg.FSignalCount; m++) {
                        TDBSignalProperties sgn;
                        sgn.FDBIndex = i;
                        sgn.FECUIndex = j;
                        sgn.FFrameIndex = k;
                        sgn.FSignalIndex = m;
                        sgn.FIsTx = 1;
                        if (0 != app.db_get_can_signal_properties_by_index(&sgn)) continue;

                        if (strcmp(sgn.FName, signalName) == 0) {
                            out_path->FDBIndex = i;
                            out_path->FECUIndex = j;
                            out_path->FFrameIndex = k;
                            out_path->FSignalIndex = m;
                            out_path->FIsTx = 1;

                            strncpy(out_path->FDBName, db.FName, sizeof(out_path->FDBName) - 1);
                            strncpy(out_path->FECUName, ecu.FName, sizeof(out_path->FECUName) - 1);
                            strncpy(out_path->FMsgName, msg.FName, sizeof(out_path->FMsgName) - 1);
                            strncpy(out_path->FSignalName, sgn.FName, sizeof(out_path->FSignalName) - 1);
                            out_path->FFrameIndentifier = msg.FCANIdentifier;
                            //out_path->FChannelMask = db.FSupportedChannelMask;
                            out_path->FFoundChannel = target_channel;

                            log("路径找到: DB[%s]->ECU[%s]->Msg[%s]->Signal[%s] @通道0x%X(优先级%d)",
                                db.FName, ecu.FName, msg.FName, sgn.FName,
                                target_channel, prio_idx);
                            return 0;
                        }
                    }
                }
            }
        }
    }

    log("未找到DUT[%s]的 %s 信号路径", dut_name,signalName);
    return -1;
}

s32 StartCANRBS(TSignalPath* path,bool state) {
    if (NetworkCluster[path->FDBName] == path->FFoundChannel) {
        printf( "当前查找通道一致" );
    }
    else {
        printf("当前查找NetworkCluster[%s] = CH%d\n", path->FDBName, NetworkCluster[path->FDBName]+1);
        printf("当前查找FFoundChannel[%s] = CH%d", path->FDBName, path->FFoundChannel + 1);
    }
    if (state) {
        if (0 == com.can_rbs_activate_network_by_name(NetworkCluster[path->FDBName], true, path->FDBName, false)) {
            log("Active CH%d %s Successful", NetworkCluster[path->FDBName]+1, path->FDBName);
        }
        else {
            log("Active CH%d %s Failed", NetworkCluster[path->FDBName] + 1, path->FDBName);
        }
        com.can_rbs_activate_node_by_name(NetworkCluster[path->FDBName], true, path->FDBName, path->FECUName, false);
        com.can_rbs_activate_message_by_name(NetworkCluster[path->FDBName], true, path->FDBName, path->FECUName, path->FMsgName);
    }
    else {
        com.can_rbs_activate_message_by_name(NetworkCluster[path->FDBName], false, path->FDBName, path->FECUName, path->FMsgName);
        com.can_rbs_activate_node_by_name(NetworkCluster[path->FDBName], false, path->FDBName, path->FECUName, false);
        com.can_rbs_activate_network_by_name(NetworkCluster[path->FDBName], false, path->FDBName, false);
    }
    if (gCEMFlg == 1) {
        if (strncmp(path->FSignalName, "VehSpdLgtA", 10) == 0) {
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehSpdLgt_UB", 1);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehSpdLgtQf", 3);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, 0);
        }
        else if (strncmp(path->FSignalName, "VehMtnStVehMtnSt", 30) == 0) {
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehMtnSt_UB", 1);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, 3);
        }
        else if (strncmp(path->FSignalName, "EngSt1WdStsEngSt1WdSts", 30) == 0) {
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "EngSt1WdSts_UB", 1);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, 1);
        }
        else if (strncmp(path->FSignalName, "IDcDcActLoSideIDcDcActLoSide", 30) == 0) {
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "IDcDcActLoSide_UB", 1);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, 0);
        }
    }
    else {
        if (strncmp(path->FSignalName, "VehModMngtGlbSafe1UsgModSts", 30) == 0) {
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehModMngtGlbSafe1_UB", 1);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehModMngtGlbSafe1CarModSts1", gCarMode); 
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehModMngtGlbSafe1PwrLvlElecMai", gEPLvl);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gUagMode);
        }
        else if (strncmp(path->FSignalName, "VehBattUSysU", 30) == 0) {
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehBattU_UB", 1);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehBattUSysUQf", 3);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gUbat12);
        }
    }
    return 0;
}

s32 SimulateTRC(bool state) {
    if (state) {
        com.can_rbs_start();
        if (gCEMFlg) {
            if (g_VehSpdLgt_found) {
                StartCANRBS(&g_VehSpdLgt_path);
            }
            if (g_EngSt1WdSts_found) {
                StartCANRBS(&g_EngSt1WdSts_path);
            }
            if (g_IDcDcActLoSide_found) {
                StartCANRBS(&g_IDcDcActLoSide_path);
            }
            if (g_VehMtnSt_found) {
                StartCANRBS(&g_VehMtnSt_path);
            }
        }
        else
        {
            if (g_VehModMngtGlbSafe1_found) {
                StartCANRBS(&g_VehModMngtGlbSafe1_path);
            }
            if (g_VehUbat_found) {
                StartCANRBS(&g_VehUbat_path);
            }
        }
    }
    else {
        if (gCEMFlg) {
            if (g_VehSpdLgt_found) {
                StartCANRBS(&g_VehSpdLgt_path,false);
            }
            if (g_EngSt1WdSts_found) {
                StartCANRBS(&g_EngSt1WdSts_path, false);
            }
            if (g_IDcDcActLoSide_found) {
                StartCANRBS(&g_IDcDcActLoSide_path, false);
            }
            if (g_VehMtnSt_found) {
                StartCANRBS(&g_VehMtnSt_path, false);
            }
        }
        else
        {
            if (g_VehModMngtGlbSafe1_found) {
                StartCANRBS(&g_VehModMngtGlbSafe1_path, false);
            }
            if (g_VehUbat_found) {
                StartCANRBS(&g_VehUbat_path, false);
            }
        }
        com.can_rbs_stop();
    }
    return 0;
}

s32 FindTRCPath() {
    if (gCEMFlg) {
        g_VehSpdLgt_found = (0 == FindSignalPath(DUTName, "VehSpdLgtA", &g_VehSpdLgt_path, VehSpd_CHANNEL_PRIORITY_COUNT, VehSpd_CHANNEL_PRIORITY));
        g_EngSt1WdSts_found = (0 == FindSignalPath(DUTName, "EngSt1WdStsEngSt1WdSts", &g_EngSt1WdSts_path, Engsts_CHANNEL_PRIORITY_COUNT, Engsts_CHANNEL_PRIORITY));
        g_IDcDcActLoSide_found = (0 == FindSignalPath(DUTName, "IDcDcActLoSideIDcDcActLoSide", &g_IDcDcActLoSide_path, Engsts_CHANNEL_PRIORITY_COUNT, Engsts_CHANNEL_PRIORITY));
        g_VehMtnSt_found = (0 == FindSignalPath(DUTName, "VehMtnStVehMtnSt", &g_VehMtnSt_path, VehMtn_CHANNEL_PRIORITY_COUNT, VehMtn_CHANNEL_PRIORITY));
    }
    else {
        g_VehModMngtGlbSafe1_found = (0 == FindSignalPath(DUTName, "VehModMngtGlbSafe1UsgModSts", &g_VehModMngtGlbSafe1_path, VehMtn_CHANNEL_PRIORITY_COUNT, VehMtn_CHANNEL_PRIORITY));
        g_VehUbat_found = (0 == FindSignalPath(DUTName, "VehBattUSysU", &g_VehUbat_path, VehMtn_CHANNEL_PRIORITY_COUNT, VehMtn_CHANNEL_PRIORITY));
    }
    return 0;
}

s32 FindMissFramePath() {
    for (int i = 0; i < ActualMissFrameCount; i++) {
        printf("当前查找第%d个，FSignalName is %s,PRIORITY Channel is CH%d",i+1, g_MissFrame_path[i].FSignalName, MissingFrame_CHANNEL_PRIORITY[i][0]);
        g_MissFrame_found[i] = (0 == FindSignalPath(DUTName, g_MissFrame_path[i].FSignalName, &g_MissFrame_path[i], MissingFrame_CHANNEL_PRIORITY_COUNT, MissingFrame_CHANNEL_PRIORITY[i]));
        if (1 == g_MissFrame_found[i]) {
            printf("Find MissFrame Signal");
            printf("DB is %s,ECU is %s,ID 0x%X %s,Signal %s", g_MissFrame_path[i].FDBName, g_MissFrame_path[i].FECUName, g_MissFrame_path[i].FFrameIndentifier, g_MissFrame_path[i].FMsgName, g_MissFrame_path[i].FSignalName);
        }
        else {
            printf("No MissFrame Signal");
        }
    }
    return 0;
}

s32 FindCarConfigPath() {
    g_CarConfig1_found = (0 == FindSignalPath(DUTName, "VehCfgPrm_UB", &g_CarConfig1_path, CarConfig_CHANNEL_PRIORITY_COUNT, CarConfig_CHANNEL_PRIORITY));
    g_CarConfig2_found = (0 == FindSignalPath(DUTName, "VehCfgPrmExt_UB", &g_CarConfig2_path, CarConfig_CHANNEL_PRIORITY_COUNT, CarConfig_CHANNEL_PRIORITY));
    g_CarConfig3_found = (0 == FindSignalPath(DUTName, "VehCfgPrmExt2_UB", &g_CarConfig3_path, CarConfig_CHANNEL_PRIORITY_COUNT, CarConfig_CHANNEL_PRIORITY));
    return 0;
}

s32 SimulateCCP(bool state = true) {

    if (g_CarConfig1_path.FFrameIndentifier == g_CarConfig2_path.FFrameIndentifier && g_CarConfig1_path.FFrameIndentifier == g_CarConfig3_path.FFrameIndentifier) {
        if (state) {
            com.can_rbs_activate_network_by_name(NetworkCluster[g_CarConfig1_path.FDBName], true, g_CarConfig1_path.FDBName, false);
            com.can_rbs_activate_node_by_name(NetworkCluster[g_CarConfig1_path.FDBName], true, g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, false);
            com.can_rbs_activate_message_by_name(NetworkCluster[g_CarConfig1_path.FDBName], true, g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName);
        }
        else {
            com.can_rbs_activate_message_by_name(NetworkCluster[g_CarConfig1_path.FDBName], false, g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName);
            com.can_rbs_activate_node_by_name(NetworkCluster[g_CarConfig1_path.FDBName], false, g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, false);
            com.can_rbs_activate_network_by_name(NetworkCluster[g_CarConfig1_path.FDBName], false, g_CarConfig1_path.FDBName, false);
        }
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, g_CarConfig1_path.FSignalName, 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmBlkIDBytePosn1", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn2", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn3", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn4", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn5", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn6", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn7", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmCCPBytePosn8", 1);


        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, g_CarConfig2_path.FSignalName, 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtBlkIDBytePosn1", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn2", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn3", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn4", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn5", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn6", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn7", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtCCPBytePosn8", 1);

        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, g_CarConfig3_path.FSignalName, 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExtBlkIDBytePosn1", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn2", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn3", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn4", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn5", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn6", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn7", 1);
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_CarConfig1_path.FDBName], g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName, "VehCfgPrmExt2CCPBytePosn8", 1);
    }
    else {
        if (state) {
            com.can_rbs_activate_network_by_name(NetworkCluster[g_CarConfig1_path.FDBName], true, g_CarConfig1_path.FDBName, false);
            com.can_rbs_activate_node_by_name(NetworkCluster[g_CarConfig1_path.FDBName], true, g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, false);
            com.can_rbs_activate_message_by_name(NetworkCluster[g_CarConfig1_path.FDBName], true, g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName);
            com.can_rbs_activate_network_by_name(NetworkCluster[g_CarConfig2_path.FDBName], true, g_CarConfig2_path.FDBName, false);
            com.can_rbs_activate_node_by_name(NetworkCluster[g_CarConfig2_path.FDBName], true, g_CarConfig2_path.FDBName, g_CarConfig2_path.FECUName, false);
            com.can_rbs_activate_message_by_name(NetworkCluster[g_CarConfig2_path.FDBName], true, g_CarConfig2_path.FDBName, g_CarConfig2_path.FECUName, g_CarConfig2_path.FMsgName);
            com.can_rbs_activate_network_by_name(NetworkCluster[g_CarConfig3_path.FDBName], true, g_CarConfig3_path.FDBName, false);
            com.can_rbs_activate_node_by_name(NetworkCluster[g_CarConfig3_path.FDBName], true, g_CarConfig3_path.FDBName, g_CarConfig3_path.FECUName, false);
            com.can_rbs_activate_message_by_name(NetworkCluster[g_CarConfig3_path.FDBName], true, g_CarConfig3_path.FDBName, g_CarConfig3_path.FECUName, g_CarConfig3_path.FMsgName);

        }
        else {
            com.can_rbs_activate_message_by_name(NetworkCluster[g_CarConfig1_path.FDBName], false, g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, g_CarConfig1_path.FMsgName);
            com.can_rbs_activate_node_by_name(NetworkCluster[g_CarConfig1_path.FDBName], false, g_CarConfig1_path.FDBName, g_CarConfig1_path.FECUName, false);
            com.can_rbs_activate_network_by_name(NetworkCluster[g_CarConfig1_path.FDBName], false, g_CarConfig1_path.FDBName, false);
            com.can_rbs_activate_message_by_name(NetworkCluster[g_CarConfig2_path.FDBName], false, g_CarConfig2_path.FDBName, g_CarConfig2_path.FECUName, g_CarConfig2_path.FMsgName);
            com.can_rbs_activate_node_by_name(NetworkCluster[g_CarConfig2_path.FDBName], false, g_CarConfig2_path.FDBName, g_CarConfig2_path.FECUName, false);
            com.can_rbs_activate_network_by_name(NetworkCluster[g_CarConfig2_path.FDBName], false, g_CarConfig2_path.FDBName, false);
            com.can_rbs_activate_message_by_name(NetworkCluster[g_CarConfig3_path.FDBName], false, g_CarConfig3_path.FDBName, g_CarConfig3_path.FECUName, g_CarConfig3_path.FMsgName);
            com.can_rbs_activate_node_by_name(NetworkCluster[g_CarConfig3_path.FDBName], false, g_CarConfig3_path.FDBName, g_CarConfig3_path.FECUName, false);
            com.can_rbs_activate_network_by_name(NetworkCluster[g_CarConfig3_path.FDBName], false, g_CarConfig3_path.FDBName, false);
        }
    }
    return 0;
}

s32 readDB(s32 ZCUFlag){
    s32 i; s32 j; s32 k; s32 m; s32 n;
    if (0 == app.db_get_can_database_count(&n)) {
        log_ok("CAN database count = %d", n);
        for (i = 0; i < n; i++) {
            TDBProperties db;
            db.FDBIndex = i;
            if (0 == app.db_get_can_database_properties_by_index(&db)) {
                //log_ok("[%d] %s Signals=%d, Messages=%d, Nodes=%d, Channels=0x%02X", i + 1, db.FName, db.FSignalCount, db.FFrameCount, db.FECUCount, db.FSupportedChannelMask);
                for (j = 0; j < db.FECUCount; j++) {
                    TDBECUProperties ecu;
                    ecu.FDBIndex = i;
                    ecu.FECUIndex = j;
                    if (0 == app.db_get_can_ecu_properties_by_index(&ecu)) {
                        TDBFrameProperties msg;
                        TDBSignalProperties sgn;
                        //log_warning("    ECU %d %s Tx Messages=%d, Rx Messages=%d", j + 1, ecu.FName, ecu.FTxFrameCount, ecu.FRxFrameCount);
                        // Tx messages and signals
                        for (k = 0; k < ecu.FTxFrameCount; k++) {
                            msg.FDBIndex = i;
                            msg.FECUIndex = j;
                            msg.FFrameIndex = k;
                            msg.FIsTx = 1;
                            if (0 == app.db_get_can_frame_properties_by_index(&msg)) {
                                //log_hint("        Tx Msg %d %s Id=0x%03X DLC=%d", k + 1, msg.FName, msg.FCANIdentifier, msg.FCANDLC);
                                for (m = 0; m < msg.FSignalCount; m++) {
                                    sgn.FDBIndex = i;
                                    sgn.FECUIndex = j;
                                    sgn.FFrameIndex = k;
                                    sgn.FSignalIndex = m;
                                    sgn.FIsTx = msg.FIsTx;
                                    if (0 == app.db_get_can_signal_properties_by_index(&sgn)) {
                                        //log("            Signal %d %s Factor=%f, Offset=%f", m + 1, sgn.FName, sgn.FCANSignal.FFactor, sgn.FCANSignal.FOffset);
                                    }
                                }
                            }
                        }
                        if (ZCUFlag) {
                            if (strstr(ecu.FName, "ZCUD") == nullptr) continue;
                            // Rx messages and signals
                            for (k = 0; k < ecu.FRxFrameCount; k++) {
                                msg.FDBIndex = i;
                                msg.FECUIndex = j;
                                msg.FFrameIndex = k;
                                msg.FIsTx = 0;
                                if (0 == app.db_get_can_frame_properties_by_index(&msg)) {
                                    //log_hint("        Rx Msg %d %s Id=0x%03X DLC=%d", k + 1, msg.FName, msg.FCANIdentifier, msg.FCANDLC);
                                    for (m = 0; m < msg.FSignalCount; m++) {
                                        sgn.FDBIndex = i;
                                        sgn.FECUIndex = j;
                                        sgn.FFrameIndex = k;
                                        sgn.FSignalIndex = m;
                                        sgn.FIsTx = msg.FIsTx;
                                        if (0 != app.db_get_can_signal_properties_by_index(&sgn))
                                            continue;
                                        for (u8 signalCount = 0; signalCount < 3; signalCount++) {
                                            if (strstr(sgn.FName, TRC_SignalGroup[signalCount].signalGroup) == nullptr) 
                                                continue;
                                            strncpy(VehGlbSafe.cluster, db.FName, 50);
                                            strncpy(VehGlbSafe.FrameName, msg.FName, 32);
                                            VehGlbSafe.FrameCycle = msg.FCycleTimeMs;
                                            VehGlbSafe.dlc = GetDLCLength(msg.FCANDLC);

                                            log("            Signal=%s,MSG=%s, Node=%s, DB=%s", sgn.FName, msg.FName, ecu.FName, db.FName);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    app.excel_unload_all();
    return 0;
}

void TestTerminate() {

    //TestInfoPrint("关闭数据记录");
    TestReport_StopLogging();
    //TestPostCondition();
    //CAN一致性板卡初始化
    //BoardCardInit_Devices();
    //if (0 == TestTerminateFlag) {
        //app.log_text("停止测试报告", lvlInfo);
    TestReport_HTML_Final();
    //app.log_text("断开VISA设备连接", lvlInfo);
    DevicesInit();
    //}
    Test_Result = TestWarning;
    app.connect();
    app.disconnect();
}

s32 getLINNetwork() {
    int LINDBCount = 0;
    app.db_get_lin_database_count(&LINDBCount);
    for (int i = 0; i < MISRESP_DTC_NUM; i++) {
        char cluster[500] = { 0 };
        snprintf(cluster, sizeof(cluster), "%s", misResponseDTCList[i].cluster);
        for (int dbIndex = 0; dbIndex < LINDBCount; dbIndex++) {
            TDBProperties dbPro;
            dbPro.FDBIndex = dbIndex;
            app.db_get_lin_database_properties_by_index(&dbPro);

            if (strstr(dbPro.FName, cluster) == nullptr)
                continue;
            else {
                clusterToDBNameMap[cluster] = dbPro.FName;
                printf("Find %s Network is %s", cluster, clusterToDBNameMap[cluster].c_str());
                break;
            }
        }
    }
    return 0;
}

s32 SimulateLINResponse(int index,bool status) {
    if (status) {

        if (0 == com.lin_rbs_activate_network_by_name(LINNetworkCluster[misResponseDTCList[index].cluster], true, misResponseDTCList[index].cluster, true)) {
            printf("Active CH%d Successful", LINNetworkCluster[misResponseDTCList[index].cluster] + 1, misResponseDTCList[index].cluster);
        }
        else {
            printf("Active CH%d Failed", LINNetworkCluster[misResponseDTCList[index].cluster] + 1, misResponseDTCList[index].cluster);
        }

        if (0 == com.lin_rbs_activate_node_by_name(LINNetworkCluster[misResponseDTCList[index].cluster], true, misResponseDTCList[index].cluster, misResponseDTCList[index].node, true)) {
            printf("Active CH%d %s %s Successful", LINNetworkCluster[misResponseDTCList[index].cluster] + 1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
        }
        else {
            printf("Active CH%d %s %s Failed", LINNetworkCluster[misResponseDTCList[index].cluster] + 1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
        }

        TDBECUProperties ecuPro;
        char tempPath[200] = { 0 };
        snprintf(tempPath, sizeof(tempPath), "%d/%s/%s", LINNetworkCluster[misResponseDTCList[index].cluster], clusterToDBNameMap[misResponseDTCList[index].cluster].c_str(), misResponseDTCList[index].node);
        printf("%s", tempPath);
        app.db_get_lin_ecu_properties_by_address(tempPath,&ecuPro);
        TDBFrameProperties msgPro;
        msgPro.FDBIndex = ecuPro.FDBIndex;
        msgPro.FECUIndex = ecuPro.FECUIndex;
        msgPro.FIsTx = 1;
        for (int msgCount = 0; msgCount < ecuPro.FTxFrameCount; msgCount++) {
            msgPro.FFrameIndex = msgCount;
            app.db_get_lin_frame_properties_by_index(&msgPro);
            printf("Msg Name is %s", msgPro.FName);
            if (0 == com.lin_rbs_activate_message_by_name(LINNetworkCluster[misResponseDTCList[index].cluster], false, misResponseDTCList[index].cluster, misResponseDTCList[index].node, msgPro.FName)) {
                printf("InActive CH%d %s %s Successful", LINNetworkCluster[misResponseDTCList[index].cluster] + 1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
            }
            else {
                printf("InActive CH%d %s %s Failed", LINNetworkCluster[misResponseDTCList[index].cluster] + 1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
            }
        }
        for (int msgCount = 0; msgCount < ecuPro.FTxFrameCount; msgCount++) {
            msgPro.FFrameIndex = msgCount;
            app.db_get_lin_frame_properties_by_index(&msgPro);
            if (0 == com.lin_rbs_activate_message_by_name(LINNetworkCluster[misResponseDTCList[index].cluster], true, misResponseDTCList[index].cluster, misResponseDTCList[index].node, msgPro.FName)) {
                printf("Active CH%d %s %s Successful", LINNetworkCluster[misResponseDTCList[index].cluster] + 1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
            }
            else {
                printf("Active CH%d %s %s Failed", LINNetworkCluster[misResponseDTCList[index].cluster] + 1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
            }
        }

        //if (0 == com.lin_rbs_start()) {
        //    printf("Start LIN RBS Successful");
        //}
        //else {
        //    printf("Start LIN RBS Failed");
        //}
        //TestWait(500);
        //if (0 == com.lin_rbs_stop()) {
        //    printf("Stop LIN RBS Successful");
        //}
        //else {
        //    printf("Stop LIN RBS Failed");
        //}
        //if (0 == com.lin_rbs_activate_network_by_name(LINNetworkCluster[misResponseDTCList[index].cluster], false, misResponseDTCList[index].cluster, true)) {
        //    printf("InActive CH%d %s Successful", LINNetworkCluster[misResponseDTCList[index].cluster] + 1, misResponseDTCList[index].cluster);
        //}
        //else {
        //    printf("InActive CH%d %s Failed", LINNetworkCluster[misResponseDTCList[index].cluster] + 1, misResponseDTCList[index].cluster);
        //}
        //if (0 == com.lin_rbs_activate_network_by_name(LINNetworkCluster[misResponseDTCList[index].cluster], true, misResponseDTCList[index].cluster, false)) {
        //    printf("Active CH%d %s Successful", LINNetworkCluster[misResponseDTCList[index].cluster]+1, misResponseDTCList[index].cluster);
        //}
        //else {
        //    printf("Active CH%d %s Failed", LINNetworkCluster[misResponseDTCList[index].cluster]+1, misResponseDTCList[index].cluster);
        //}
        ////if (0 == com.lin_rbs_activate_node_by_name(LINNetworkCluster[misResponseDTCList[index].cluster]+1, false, misResponseDTCList[index].cluster, misResponseDTCList[index].node, true)) {
        ////    printf("InActive CH%d %s %s Successful", LINNetworkCluster[misResponseDTCList[index].cluster]+1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
        ////}
        ////else {
        ////    printf("InActive CH%d %s %s Failed", LINNetworkCluster[misResponseDTCList[index].cluster]+1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
        ////}
        //if (0 == com.lin_rbs_activate_node_by_name(LINNetworkCluster[misResponseDTCList[index].cluster], true, misResponseDTCList[index].cluster, misResponseDTCList[index].node, true)) {
        //    printf("Active CH%d %s %s Successful", LINNetworkCluster[misResponseDTCList[index].cluster]+1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
        //}
        //else {
        //    printf("Active CH%d %s %s Failed", LINNetworkCluster[misResponseDTCList[index].cluster]+1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
        //}
        if (0 == com.lin_rbs_start()) {
            printf("Start LIN RBS Successful");
        }
        else {
            printf("Start LIN RBS Failed");
        }
    }
    else {
        if (0 == com.lin_rbs_activate_network_by_name(LINNetworkCluster[misResponseDTCList[index].cluster], false, misResponseDTCList[index].cluster, false)) {
            printf("InActive CH%d %s Successful", LINNetworkCluster[misResponseDTCList[index].cluster]+1, misResponseDTCList[index].cluster);
        }
        else {
            printf("InActive CH%d %s Failed", LINNetworkCluster[misResponseDTCList[index].cluster]+1, misResponseDTCList[index].cluster);
        }
        if (0 == com.lin_rbs_activate_node_by_name(LINNetworkCluster[misResponseDTCList[index].cluster], false, misResponseDTCList[index].cluster, misResponseDTCList[index].node, true)) {
            printf("InActive CH%d %s %s Successful", LINNetworkCluster[misResponseDTCList[index].cluster]+1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
        }
        else {
            printf("InActive CH%d %s %s Failed", LINNetworkCluster[misResponseDTCList[index].cluster]+1, misResponseDTCList[index].cluster, misResponseDTCList[index].node);
        }
        if (0 == com.lin_rbs_stop()) {
            printf("Stop LIN RBS Successful");
        }
        else {
            printf("Stop LIN RBS Failed");
        }
    }
    return 0;
}

s32 getCarConfigInfo() {
    s32 i; s32 j; s32 k; s32 m; s32 n;
    //CarConfigSignalGroup.clear();
    if (0 == app.db_get_can_database_count(&n)) {
        log_ok("CAN database count = %d", n);
        for (i = 0; i < n; i++) {
            TDBProperties db;
            db.FDBIndex = i;
            if (0 == app.db_get_can_database_properties_by_index(&db)) {
                if (0 != strcmp(db.FName, CarConfigBus_Temp)) continue;
                //log_ok("[%d] %s Signals=%d, Messages=%d, Nodes=%d, Channels=0x%02X", i + 1, db.FName, db.FSignalCount, db.FFrameCount, db.FECUCount, db.FSupportedChannelMask);
                for (j = 0; j < db.FECUCount; j++) {
                    TDBECUProperties ecu;
                    ecu.FDBIndex = i;
                    ecu.FECUIndex = j;
                    if (0 == app.db_get_can_ecu_properties_by_index(&ecu)) {
                        TDBFrameProperties msg;
                        TDBSignalProperties sgn;
                        for (k = 0; k < ecu.FTxFrameCount; k++) {
                            msg.FDBIndex = i;
                            msg.FECUIndex = j;
                            msg.FFrameIndex = k;
                            msg.FIsTx = 1;
                            if (0 == app.db_get_can_frame_properties_by_index(&msg)) {
                                for (m = 0; m < msg.FSignalCount; m++) {
                                    sgn.FDBIndex = i;
                                    sgn.FECUIndex = j;
                                    sgn.FFrameIndex = k;
                                    sgn.FSignalIndex = m;
                                    sgn.FIsTx = msg.FIsTx;
                                    if (0 == app.db_get_can_signal_properties_by_index(&sgn)) {
                                        char tempSgnUBName[50] = { 0 };
                                        char tempSgnIDName[50] = { 0 };
                                        char tempSgnByteName[50] = { 0 };
                                        for (int groupIndex = 0; groupIndex < CarConfigSignalGroup.size(); groupIndex++) {

                                            snprintf(tempSgnIDName, sizeof(tempSgnIDName), "%sBlkIDBytePosn1", CarConfigSignalGroup[groupIndex].c_str());
                                            if (g_CCPIsCanFD) {
                                                snprintf(tempSgnUBName, sizeof(tempSgnUBName), "%s_UB", CarConfigSignalGroup[groupIndex].c_str());
                                            }
                                            else {
                                                snprintf(tempSgnUBName, sizeof(tempSgnUBName), "%s", tempSgnIDName);
                                            }
                                            snprintf(tempSgnByteName, sizeof(tempSgnByteName), "%sCCPBytePosn", CarConfigSignalGroup[groupIndex].c_str());
                                            
                                            if ((CarConfigSignalGroup.size() - 1) == groupIndex && g_CCPIsCanFD) {
                                                if (0 == strcmp(sgn.FName, tempSgnUBName)) {
                                                    carConfigData[groupIndex].DBName = std::string(db.FName);
                                                    carConfigData[groupIndex].ECUName = std::string(ecu.FName);
                                                    carConfigData[groupIndex].FrameName = std::string(msg.FName);
                                                    carConfigData[groupIndex].FFrameIdentifier = msg.FCANIdentifier;
                                                    carConfigData[groupIndex].UBbit = sgn.FCANSignal.FStartBit;
                                                    printf("DBName is %s,ECUName is %s,FrameName is %s,FFrameIdentifier is 0x%X,UBbit is %d",
                                                        carConfigData[groupIndex].DBName.c_str(), carConfigData[groupIndex].ECUName.c_str(),
                                                        carConfigData[groupIndex].FrameName.c_str(), carConfigData[groupIndex].FFrameIdentifier,
                                                        carConfigData[groupIndex].UBbit);
                                                }
                                                for (u8 listOfNodvIndex = 1; listOfNodvIndex <= 8; listOfNodvIndex++) {
                                                    snprintf(tempSgnIDName, sizeof(tempSgnIDName), "%s%d", CarConfigSignalGroup[groupIndex].c_str(), listOfNodvIndex);

                                                    if (0 == strcmp(sgn.FName, tempSgnIDName)) {
                                                        carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].StartBit = sgn.FCANSignal.FStartBit;
                                                        carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].Length = sgn.FCANSignal.FLength;
                                                        carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].SignalName = std::string(sgn.FName);
                                                        printf("Signal is %s,Length is %d,startBit is %d", carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].SignalName.c_str(),
                                                            carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].Length,
                                                            carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].StartBit);
                                                    }
                                                }
                                                continue;
                                            }
                                            
                                            if (0 == strcmp(sgn.FName, tempSgnUBName)) {
                                                carConfigData[groupIndex].DBName = std::string(db.FName);
                                                carConfigData[groupIndex].ECUName = std::string(ecu.FName);
                                                carConfigData[groupIndex].FrameName = std::string(msg.FName);
                                                carConfigData[groupIndex].FFrameIdentifier = msg.FCANIdentifier;
                                                carConfigData[groupIndex].UBbit = sgn.FCANSignal.FStartBit;
                                                printf("DBName is %s,ECUName is %s,FrameName is %s,FFrameIdentifier is 0x%X,UBbit is %d", 
                                                    carConfigData[groupIndex].DBName.c_str(), carConfigData[groupIndex].ECUName.c_str(),
                                                    carConfigData[groupIndex].FrameName.c_str(), carConfigData[groupIndex].FFrameIdentifier,
                                                    carConfigData[groupIndex].UBbit);
                                            }
                                            else  if (0 == strcmp(sgn.FName, tempSgnIDName)) {
                                                carConfigData[groupIndex].carConfigSignalGroupData[0].StartBit = sgn.FCANSignal.FStartBit;
                                                carConfigData[groupIndex].carConfigSignalGroupData[0].Length = sgn.FCANSignal.FLength;
                                                carConfigData[groupIndex].carConfigSignalGroupData[0].SignalName = std::string(sgn.FName);
                                                printf("Signal is %s,Length is %d,startBit is %d", carConfigData[groupIndex].carConfigSignalGroupData[0].SignalName.c_str(),
                                                    carConfigData[groupIndex].carConfigSignalGroupData[0].Length, carConfigData[groupIndex].carConfigSignalGroupData[0].StartBit);
                                            }
                                            else  if (strstr(sgn.FName, tempSgnByteName) != nullptr) {
                                                for (u8 carConfigSignalIndex = 1; carConfigSignalIndex < 8; carConfigSignalIndex++) {
                                                    char tempCCPByteName[50] = { 0 };
                                                    snprintf(tempCCPByteName,sizeof(tempCCPByteName), "%sCCPBytePosn%d", CarConfigSignalGroup[groupIndex].c_str(), carConfigSignalIndex + 1);
                                                    if (strncmp(sgn.FName, tempCCPByteName,50) == 0) {
                                                        carConfigData[groupIndex].carConfigSignalGroupData[carConfigSignalIndex].StartBit = sgn.FCANSignal.FStartBit;
                                                        carConfigData[groupIndex].carConfigSignalGroupData[carConfigSignalIndex].Length = sgn.FCANSignal.FLength;
                                                        carConfigData[groupIndex].carConfigSignalGroupData[carConfigSignalIndex].SignalName = std::string(sgn.FName);
                                                        printf("Signal is %s,Length is %d,startBit is %d", carConfigData[groupIndex].carConfigSignalGroupData[carConfigSignalIndex].SignalName.c_str(),
                                                            carConfigData[groupIndex].carConfigSignalGroupData[carConfigSignalIndex].Length, 
                                                            carConfigData[groupIndex].carConfigSignalGroupData[carConfigSignalIndex].StartBit);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

s32 setMissFrameCarConfigValue(bool state) {
    if (!state) {
        for (int i = 0; i < 600;i++) {
            gCCPData[i] = 0;
            gCCPDataExt[i] = 0;
            gCCPDataExt2[i] = 0;
        }
        for (int j = 0; j < 8; j++) {
            gCCPDataListOfNodAv[j] = 0;
        }
    }
    else {
        for (int index = 0; index < ActualMissFrameCount; index++)
        {
            if (misMsgDTCList[index].CCPPsn20 <= 504)
            {
                if (gCCPData[misMsgDTCList[index].CCPPsn20 - 1] != misMsgDTCList[index].CCPValue20)
                {
                    gCCPData[misMsgDTCList[index].CCPPsn20 - 1] = misMsgDTCList[index].CCPValue20;
                }
            }
            else if (505 <= misMsgDTCList[index].CCPPsn20 && misMsgDTCList[index].CCPPsn20 < 1000)
            {
                if (gCCPDataExt[misMsgDTCList[index].CCPPsn20 - 505] != misMsgDTCList[index].CCPValue20)
                {
                    gCCPDataExt[misMsgDTCList[index].CCPPsn20 - 505] = misMsgDTCList[index].CCPValue20;
                }
            }
            else if (1256 <= misMsgDTCList[index].CCPPsn20 && misMsgDTCList[index].CCPPsn20 <= 1760)
            {
                if (gCCPDataExt2[misMsgDTCList[index].CCPPsn20 - 1256] != misMsgDTCList[index].CCPValue20)
                {
                    gCCPDataExt2[misMsgDTCList[index].CCPPsn20 - 1256] = misMsgDTCList[index].CCPValue20;
                }
                //printf("gCCPDataExt2[%d] = %d;", misMsgDTCList[index].CCPPsn20 - 1256, gCCPDataExt2[misMsgDTCList[index].CCPPsn20 - 1256]);
            }
            else
            {
                for (u8 i = 0; i < 8; i++) {
                    gCCPDataListOfNodAv[i] = 0xFFFFFFFF;
                }
            }

        }
    }
    return 0;
}

s32 setMissResponseCarConfigValue(bool state) {
    if (!state) {
        for (int i = 0; i < 600; i++) {
            gCCPData[i] = 0;
            gCCPDataExt[i] = 0;
            gCCPDataExt2[i] = 0;
        }
        for (int j = 0; j < 8; j++) {
            gCCPDataListOfNodAv[j] = 0;
        }
    }
    else {
        for (int index = 0; index < ActualMissResponseCount; index++)
        {
            if (misResponseDTCList[index].CCPPsn20 <= 504)
            {
                if (gCCPData[misResponseDTCList[index].CCPPsn20 - 1] != misResponseDTCList[index].CCPValue20)
                {
                    gCCPData[misResponseDTCList[index].CCPPsn20 - 1] = misResponseDTCList[index].CCPValue20;
                }
            }
            else if (505 <= misResponseDTCList[index].CCPPsn20 && misResponseDTCList[index].CCPPsn20 < 1000)
            {
                if (gCCPDataExt[misResponseDTCList[index].CCPPsn20 - 505] != misResponseDTCList[index].CCPValue20)
                {
                    gCCPDataExt[misResponseDTCList[index].CCPPsn20 - 505] = misResponseDTCList[index].CCPValue20;
                }
            }
            else if (1256 <= misResponseDTCList[index].CCPPsn20 && misResponseDTCList[index].CCPPsn20 <= 1760)
            {
                if (gCCPDataExt2[misResponseDTCList[index].CCPPsn20 - 1256] != misResponseDTCList[index].CCPValue20)
                {
                    gCCPDataExt2[misResponseDTCList[index].CCPPsn20 - 1256] = misResponseDTCList[index].CCPValue20;
                }
                //printf("gCCPDataExt2[%d] = %d;", misResponseDTCList[index].CCPPsn20 - 1256, gCCPDataExt2[misResponseDTCList[index].CCPPsn20 - 1256]);
            }
            else
            {
                for (u8 i = 0; i < 8; i++) {
                    gCCPDataListOfNodAv[i] = 0xFFFFFFFF;
                }
            }

        }
    }
    return 0;
}


s32 signCarConfigData() {
    u8* pDataSources[3] = { gCCPData, gCCPDataExt, gCCPDataExt2 };
    int* pBlockIds[3] = { &gBolockId, &gBolockExtId, &gBolockExt2Id };

    if (g_CCPIsCanFD)
    {
        TCANFD CANFDCarConfig,CarConfigListOfNodv;
        CANFDCarConfig.init_w_std_id(carConfigData[0].FFrameIdentifier, 15);
        CANFDCarConfig.FIdxChn = NetworkCluster[CarConfigBus_Temp];
        CarConfigListOfNodv.init_w_std_id(carConfigData[3].FFrameIdentifier, 15);
        CarConfigListOfNodv.FIdxChn = NetworkCluster[CarConfigBus_Temp];
        //printf("CH%d CarConfigListOfNodv ID 0x%X", CarConfigListOfNodv.FIdxChn, carConfigData[3].FFrameIdentifier);
        gBolockId++;
        for (int group = 0; group < 3; group++) {
            int currentBlockId = gBolockId;
            int byteOffset = group * 8;
            int pos0 = carConfigData[group].carConfigSignalGroupData[0].StartBit / 8;
            CANFDCarConfig.FData[pos0] = currentBlockId;
            CANFDCarConfig.FData[carConfigData[group].UBbit / 8] |= (1 << carConfigData[group].UBbit % 8);
            for (int i = 1; i < 8; i++) {
                int pos = carConfigData[group].carConfigSignalGroupData[i].StartBit / 8;
                int dataIndex = (currentBlockId - 1) * 7 + (i - 1);
                CANFDCarConfig.FData[pos] = pDataSources[group][dataIndex];
            }
        }
        for (int ListIndex = 0; ListIndex < 8; ListIndex++) {
            for (int ListBitIndex = 0; ListBitIndex < carConfigData[3].carConfigSignalGroupData[ListIndex].Length / 8; ListBitIndex++) {
                CarConfigListOfNodv.FData[(carConfigData[3].carConfigSignalGroupData[ListIndex].StartBit - carConfigData[3].carConfigSignalGroupData[ListIndex].Length + (ListBitIndex + 1) * 8) / 8] = (u8)(gCCPDataListOfNodAv[ListIndex] & 0xFF);
            }
        }
        CarConfigListOfNodv.FData[carConfigData[3].UBbit / 8] |= (1 << carConfigData[3].UBbit%8);
        com.transmit_canfd_async(&CANFDCarConfig);
        com.transmit_canfd_async(&CarConfigListOfNodv);
    }
    else
    {
        TCAN CfgFrames[3];
        for (int group = 0; group < 3; group++) {
            CfgFrames[group].init_w_std_id(carConfigData[group].FFrameIdentifier, 8);
            (*pBlockIds[group])++;
            int currentBlockId = *pBlockIds[group];

            int pos0 = carConfigData[group].carConfigSignalGroupData[0].StartBit / 8;
            CfgFrames[group].FData[pos0] = currentBlockId;
            for (int i = 1; i < 8; i++) {
                int pos = carConfigData[group].carConfigSignalGroupData[i].StartBit / 8;
                int dataIndex = (currentBlockId - 1) * 7 + (i - 1);
                CfgFrames[group].FData[pos] = pDataSources[group][dataIndex];
            }
            com.transmit_can_async(&CfgFrames[group]);
        }
    }

    if (gBolockId >= 72)
    {
        gBolockId = 0;
        index_cc1++;
        printf("index_cc1 is %d", index_cc1);
    }
    if (index_cc1 >= 3)
    {
        carConfigEndFlg = true;
        CCPTimer.stop();
        log("停止仿真CCP");
    }
    if (gBolockExtId >= 72)
    {
        gBolockExt2Id = 0;
        gBolockExtId = 0;
        index_cc2++;
    }
    return 0;
}

s32 simulateCCP(bool state = true) {
    s64 nowTime = 0, lastTime = 0;
    for (int k = 0; k < 600; k++)
    {
        gCCPDataExt2[k] = 0;
        gCCPDataExt[k] = 0;
        gCCPData[k] = 0;
    }
    index_cc1 = 0;
    index_cc2 = 0;
    gBolockId = 0;
    gBolockExtId = 0;
    gBolockExt2Id = 0;

    switch (carConfigType) {
        case MissFrame:
            setMissFrameCarConfigValue(state);
            break;
        case MissResponse:
            setMissResponseCarConfigValue(state);
            break;
        case FaultResponse:
            break;
        case SlaveFault:
            break;
        default:
            break;
    }
    

    CCPTimer.start();
    CCPTimer.set_interval(50);
    log("开始仿真CCP");
    carConfigEndFlg = false;
    app.get_timestamp(&lastTime);
    while ((nowTime - lastTime) <= 20000000) {
        app.get_timestamp(&nowTime);
        AppWait(1000, "");
        if (carConfigEndFlg) {
            break;
        }
    }
    return 0;
}
#endif // !TEST_CASE_DIAG_API

