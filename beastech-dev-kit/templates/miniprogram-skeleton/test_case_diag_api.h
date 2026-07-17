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
#include <iostream>
#include <cstdlib>

#include <array>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <unordered_map>
#include <set>
#include <numeric>
#include <algorithm>
//#include <sstream>
//#include <string>

#include"FunctionDeclaration.h"
#include"test_parameter.h"
#include <GenerateKeyExOpt.h>
//工程路径
char* ABSPath = nullptr;
//python工具箱生成的ini文件路径
std::string Parameter_inipath = "Toolbox\\GEELY3\\Parameters1.ini";//

extern char E2E_ini_path[100];

s32 udsHandle = 0;
enum AddressMode { Physical = 0, Functional = 1 };//寻址模式
enum DiagSession { DefaultSession = 0x01, ExtendedSession = 0x03, ProgrammingSession = 0x02 };//诊断会话类型
enum SPRMIB {SPRMIB_FALSE, SPRMIB_TRUE};//肯定响应消息抑制位
enum ECUReset { HardReset = 0x01, SoftReset = 0x03 };//复位方式
enum DTCState {NotFind = 0x00,Find = 0x01};
//enum PowerControl {on,off};

struct _securityUnlock {
    u8 level;
    u8 session;
};

_securityUnlock securityUnlock[5] = {
    {0x03,ExtendedSession},
    {0x05,ExtendedSession},
    {0x11,ExtendedSession},
    {0x19,ExtendedSession},
    {0x01,ProgrammingSession}
};

//多帧数据，不行
//u8 reqMultiFrameDataArray2[5] = { 0x22,0xD0,0XAC,0xed,0xA0 };


//Queue相关定义
struct _RxDiaBuffer {
    s32 FIdentifier;
    u8 FDLC;
    u8 FData[64];
    u16 Num;
} RxDiaBuffer;
_RxDiaBuffer rx[1000];
bool CheckFlag = false;
s32 SID36_Transfer_Data_Package(uint8_t startSeq, const uint8_t* data, size_t length);


//SWDL
u8 reqSWDL_ID = 0x20;
u32 MaxLength = 0x2000;
u8 robustnessTimes = 2;

//数据接收缓存
u8 TpRxData[65535];
s32 TpRxLength;
//s64 c;
//

//27解锁相关
u8 SeedArray[16];
u8 KeyArray[16];
char* DLLName;
//u8 pincode0x01[16] = { 0x5E,0x9A,0xE3,0xD5,0x3A,0xA0,0xF6,0x9F,0x72,0x0C,0x37,0x7E,0x68,0x37,0xC2,0xA0 };
u8 pincode0x01[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x03[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x05[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x07[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x09[16] = { 0x5E,0x9A,0xE3,0xD5,0x3A,0xA0,0xF6,0x9F,0x72,0x0C,0x37,0x7E,0x68,0x37,0xC2,0xA0 };
u8 pincode0x0B[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x0D[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x0F[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
//u8 pincode0x11[16] = { 0x5E,0x9A,0xE3,0xD5,0x3A,0xA0,0xF6,0x9F,0x72,0x0C,0x37,0x7E,0x68,0x37,0xC2,0xA0 };
u8 pincode0x11[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x13[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x15[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x17[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x19[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x1B[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x1D[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
u8 pincode0x1F[16] = { 0xFF, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF };
//

//28服务检测报文
u8 APP_Flag = 0;
u8 NM_Flag = 0;

//29安全验证
u8 CertificateClient[100];
u8 ChallengeServer[100];
u8 ProofOfOwnershipClient[100];
u8 EphemeralPublicKeyServer[100];
//

//DTC相关
int Ndtc = 0;

//函数声明
s32 CheckArrayDifferent(pu8 array1, pu8 array2, u8 length);
void TestPostCondition();
double SetVoltageAndRead(double, u8, u32);
s32 excelParser();
s32 ParserArxml();
s32 CheckSoftVersion();
s32 FindCarConfigPath();
s32 getCarConfigInfo();

struct DTC
{
    u8 DTCNumber[3];
    u8 DTCStatus;
};

u32 testdtc[2];
struct DTC TestDtc[2];
struct DTC recdtc[1000];

//时间参数
u64 stepseptime = 50;
u64 caseSepTime = 2000;
u8  SessionFlag = 0;
u64 Session02_Time = 2000;

////////////////////////////////////////////////////////

//////////////SHA-256//////////////////////

class SHA256 {
private:
    uint32_t m_hash[8];

    uint8_t m_buffer[64];

    uint64_t m_bitCount;

    static const uint32_t K[64];

    static const uint32_t init_hash[8];

    uint32_t rotr(uint32_t x, uint32_t n) const {
        return (x >> n) | (x << (32 - n));
    }

    uint32_t shr(uint32_t x, uint32_t n) const {
        return x >> n;
    }

    uint32_t sigma0(uint32_t x) const {
        return rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3);
    }

    uint32_t sigma1(uint32_t x) const {
        return rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10);
    }

    uint32_t Sigma0(uint32_t x) const {
        return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
    }

    uint32_t Sigma1(uint32_t x) const {
        return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
    }

    uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) const {
        return (x & y) ^ (~x & z);
    }
    uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) const {
        return (x & y) ^ (x & z) ^ (y & z);
    }
    void compress(const uint8_t* data);
    void pad();

public:
    SHA256();
    void reset();
    void update(const uint8_t* data, size_t length);
    std::array<uint8_t, 32> finalize();
    std::string digest();
};

const uint32_t SHA256::K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

const uint32_t SHA256::init_hash[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

SHA256::SHA256() {
    reset();
}

void SHA256::reset() {
    m_bitCount = 0;
    std::memcpy(m_hash, init_hash, 8 * sizeof(uint32_t));
}

void SHA256::compress(const uint8_t* chunk) {
    uint32_t a = m_hash[0], b = m_hash[1], c = m_hash[2], d = m_hash[3];
    uint32_t e = m_hash[4], f = m_hash[5], g = m_hash[6], h = m_hash[7];

    uint32_t w[64];

    // 消息扩展
    for (int t = 0; t < 16; ++t) {
        w[t] = (chunk[4 * t] << 24) | (chunk[4 * t + 1] << 16) |
            (chunk[4 * t + 2] << 8) | chunk[4 * t + 3];
    }

    for (int t = 16; t < 64; ++t) {
        w[t] = sigma1(w[t - 2]) + w[t - 7] + sigma0(w[t - 15]) + w[t - 16];
    }

    // 64轮迭代计算
    for (int t = 0; t < 64; ++t) {
        uint32_t T1 = h + Sigma1(e) + Ch(e, f, g) + K[t] + w[t];
        uint32_t T2 = Sigma0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    // 更新哈希状态
    m_hash[0] += a;
    m_hash[1] += b;
    m_hash[2] += c;
    m_hash[3] += d;
    m_hash[4] += e;
    m_hash[5] += f;
    m_hash[6] += g;
    m_hash[7] += h;
}

void SHA256::pad() {
    uint64_t bitLen = m_bitCount;
    uint32_t index = (m_bitCount / 8) % 64;

    m_buffer[index++] = 0x80;

    if (index > 56) {
        std::memset(m_buffer + index, 0, 64 - index);
        compress(m_buffer);
        std::memset(m_buffer, 0, 56);
    }
    else {
        std::memset(m_buffer + index, 0, 56 - index);
    }

    // 附加64位消息长度（大端序）
    m_buffer[56] = (bitLen >> 56) & 0xFF;
    m_buffer[57] = (bitLen >> 48) & 0xFF;
    m_buffer[58] = (bitLen >> 40) & 0xFF;
    m_buffer[59] = (bitLen >> 32) & 0xFF;
    m_buffer[60] = (bitLen >> 24) & 0xFF;
    m_buffer[61] = (bitLen >> 16) & 0xFF;
    m_buffer[62] = (bitLen >> 8) & 0xFF;
    m_buffer[63] = bitLen & 0xFF;

    compress(m_buffer);
}

void SHA256::update(const uint8_t* data, size_t length) {
    size_t offset = 0;

    while (offset < length) {
        size_t copyLen = 64 - ((m_bitCount / 8) % 64);
        if (copyLen > length - offset) {
            copyLen = length - offset;
        }

        std::memcpy(m_buffer + ((m_bitCount / 8) % 64), data + offset, copyLen);
        m_bitCount += copyLen * 8;
        offset += copyLen;

        if ((m_bitCount % 512) == 0) {
            compress(m_buffer);
        }
    }
}

std::array<uint8_t, 32> SHA256::finalize() {
    pad();

    std::array<uint8_t, 32> digest;
    for (int i = 0; i < 8; ++i) {
        digest[4 * i] = (m_hash[i] >> 24) & 0xFF;
        digest[4 * i + 1] = (m_hash[i] >> 16) & 0xFF;
        digest[4 * i + 2] = (m_hash[i] >> 8) & 0xFF;
        digest[4 * i + 3] = m_hash[i] & 0xFF;
    }

    reset();
    return digest;
}

std::string SHA256::digest() {
    auto result = finalize();
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (auto& b : result) {
        oss << std::setw(2) << (int)b;
    }

    return oss.str();
}

std::string sha256_hash(const std::string& input) {
    SHA256 hasher;
    hasher.update(reinterpret_cast<const uint8_t*>(input.data()), input.size());
    return hasher.digest();
}

std::vector<uint8_t> hex_string_to_bytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byte_str = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}


//////////////类型转换/////////////////////
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
        sprintf(_str, "0x%X", sourceArray[i]);
        strcat(_str1, _str);
    }
    strcpy(desString, _str1);
}

void TestWait(u64 AppWaitTime)
{
    char tempTime[100] = {};
    snprintf(tempTime, 100, "wait %ldms", AppWaitTime);
    app.wait(AppWaitTime, tempTime);
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
            if (rtlUIDiagnostics.tsdiag_set_fdmode(udsHandle, true, true, Diag_FD_MAX_LEN) == 0x00)
            {
                log("set device work as FDCAN mode, enable brs and max dlc is %d!", Diag_FD_MAX_LEN);
            }
            else
            {
                log("set fdmode of device failed!");
            }
            char tempstr[100] = {};
            if (language_id == 2052)
            {
                strcpy(tempstr, "");
                sprintf(tempstr, "%s%X", "诊断会话创建成功,通道号为", Diag_ch);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%x", "ECU 物理寻址", Diag_Request_ID);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%X", "ECU 功能寻址", Diag_Function_ID);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%X", "ECU 响应ID", Diag_Response_ID);
                app.log_text(tempstr, lvlOK);
            }
            else
            {
                strcpy(tempstr, "");
                sprintf(tempstr, "%s%X", "create diagnostic communication successfully, channel number is ", Diag_ch);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%X", "ECU physical request ID:", Diag_Request_ID);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%X", "ECU functional request ID:", Diag_Function_ID);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%X", "ECU response ID:", Diag_Response_ID);
                app.log_text(tempstr, lvlOK);
            }
            if (rtlUIDiagnostics.tsdiag_set_fdmode(udsHandle, true, true, 15) == 0x00)
            {
                log("set device work as FDCAN mode, enable brs and max dlc is 15!");
            }
            else
            {
                log("set fdmode of device failed!");
            }
        }
        if (rtlUIDiagnostics.tsdiag_set_filled_byte(udsHandle, 0x00) == 0x00)
        {
            app.log_text("set 0x00 as filled byte", lvlInfo);
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
                sprintf(tempstr, "%s%X", "诊断会话创建成功,通道号为", Diag_ch);
                app.log_text(tempstr, lvlOK);

                strcpy(tempstr, "");
                sprintf(tempstr, "%s 0x%X", "ECU NAD = ", NAD);
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
        if (rtlUIDiagnostics.tsdiag_can_delete(udsHandle) == 0x00)
        {
            app.log_text("Delete Diagnostic Success",lvlOK);
        }
    }
    printf("delete uds");
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
        if (BusTypeNow == CAN || BusTypeNow == CANFD)
        {
            return CAN_TP_TesterPresent(1, PeriodTime);
        }
    }
    else
    {
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

    //if (3 != responseArraySize)
    //{
    //    strcpy(issuereason, "Invalid Data Length!");
    //    return -1;
    //}
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

s32 CheckNegResponse_Warning(u8 NRC)
{
    if (0 == Raw_CheckNegResponse(NRC))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Warning);
        return-1;
    }
}

/// <summary>
/// 通用 检查否定响应
/// </summary>
/// <param name="NRC"></param> NRC 期望的否定响应码
/// <param name="responseArraySize"></param> responseArraySize 诊断响应数据长度,由请求函数返回
/// <param name="responseArray"></param> responseArray 诊断响应内容
s32 Raw_CheckNegResponse(u8* NRC, u8 nrcLength = 5,s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    char TempNRC[50] = { 0 };

    // 验证输入
    if (NRC == nullptr || responseArraySize <= 0) {
        return -1;
    }
    strcpy(expectedResponse, "NRC");
    for (s32 i = 0; i < nrcLength; i++) {
        sprintf(TempNRC, " %02X", NRC[i]);
        strcat(expectedResponse, TempNRC);
        printf("For inside is%s\n", TempNRC);
    }

    printf("expectedResponse is %s\n", expectedResponse);
   
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
    
    for (int i = 0; i < nrcLength; i++)
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

s32 CheckNegResponse(u8* NRC,u8 nrcLength = 5)
{
    if (0 == Raw_CheckNegResponse(NRC, nrcLength))
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

s32 CheckNegResponse_Warning(u8* NRC, u8 nrcLength = 5)
{
    if (0 == Raw_CheckNegResponse(NRC, nrcLength))
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
            sprintf(actualResponse, " 0x%X", dec2hex(TpRxData[i]));
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
            sprintf(actualResponse, " 0x%X", dec2hex(TpRxData[i]));
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
        TempP2Time = Diag_UDS_P2Server_Extend;
        TempP2STime = Diag_UDS_P2Server_Delay_Extend;
    }
    else
    {
        TempP2Time = Diag_UDS_P2Server_Program;
        TempP2STime = Diag_UDS_P2Server_Delay_Program;
    }
 
    strcpy(expectedResponse,"");
    for (int i = 0; i < responseArraySize; i++)
    {
        sprintf(expectedResponse, "%s 0%X 00 %X 0%X %X", "50", SubFunction, TempP2Time, TempP2STime >> 8, TempP2STime & 0xff);
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
        return 22;
    }

    TempSessionTime = (responseArray[4] << 8) + responseArray[5];
    if (TempSessionTime == TempP2STime)
    {

    }
    else
    {
        strcpy(issuereason, "P2* Server is not correct!");
        return 22;
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
    s8 SID10_Return = 9;
    SID10_Return = Raw_CheckPosResponse_SID10(SubFunction);
    if (0 == SID10_Return)
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else if (22 == SID10_Return) {
        TestReport_StepInfo(Warning);
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

s32 Raw_SID22_ReadDataByIdentifier_Queue(u8 DID[2], u8 AddressMode = 0)
{
    u8 SID22[3] = {};
    SID22[0] = 0x22;
    SID22[1] = DID[0];
    SID22[2] = DID[1];
    s32 responseArraySize = 65535;
    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_send_request(udsHandle, SID22, 3) == 0x00) {
            app.log_text("send diagnostic payload success!", lvlOK);
            return 0;
        }
        else
        {
            app.log_text("send diagnostic payload fail!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_send_functional(udsHandle, SID22, 3) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return 0;
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
    TpRxLength = Raw_SID22_ReadDataByIdentifier(DID, AddressMode);
    printf("length is %d", TpRxLength);
    strcpy(requestInfo, ("22 " + dec2hex(DID[0]) + " " + dec2hex(DID[1])).c_str());
    return 0;
}

s32 SID22_ReadDataByIdentifier_Queue(u8 DID[2], u8 AddressMode = 0)
{
    TpRxLength = Raw_SID22_ReadDataByIdentifier_Queue(DID, AddressMode);

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

    if (responseArraySize > 0 && responseArray != NULL) {
        char hexBuffer[1000] = { 0 };
        int pos = 0;

        for (u64 i = 0; i < responseArraySize; i++) {
            if (i > 0) {
                hexBuffer[pos++] = ' ';
            }
            pos += sprintf(hexBuffer + pos, "%02X", responseArray[i]);
        }
        strcat(actualResponse, " ");
        strcat(actualResponse, hexBuffer);
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


s32 Raw_CheckPosResponse_SID22_DataRecord(u8 DID[2], u32 Length, pu8 dataRecord, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    if (Length == 0 || TpRxLength < 3) {
        return -1;
    }
    for (u8 i = 0; i < Length && i < TpRxLength - 3; i++) {
        dataRecord[i] = responseArray[3 + i];
    }
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

s32 CheckPosResponse_SID22_DataRecord(u8 DID[2], u32 Length,pu8 dataRecord)
{
    if (0 == Raw_CheckPosResponse_SID22_DataRecord(DID, Length, dataRecord))
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

//reload
s32 Raw_CheckPosResponse_SID22(u8 DID[2], u32 Length, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
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

s32 Raw_CheckPosResponse_Queue(u8 Sequence)
{
    s32 responseArraySize;
    u8 responseArray[100];
    for (int i = 0; i < 1000; i++) {
        if ((rx[i].FData[0] == 0x76 && rx[i].FData[1] == Sequence)) {
            //responseArraySize = GetDLCLength(rx[i].FDLC) - 2;
            //for (int j = 0; j < responseArraySize; j++) {
            //    responseArray[j] = rx[i].FData[j];
            //}
            return 0;
        }
        else if (rx[i].FData[0] == 0x7F && rx[i].FData[1] == 0x36) {
            //responseArraySize = 3;
            //responseArray[0] = rx[i].FData[0];
            //responseArray[1] = rx[i].FData[1];
            //responseArray[2] = rx[i].FData[2];
            if (rx[i].FData[2] == 0x78)
                continue;
        }
        else
            continue;

    }

    return -1;
}
s32 CheckPosResponse_Queue(u8 Sequence)
{
    if (0 == Raw_CheckPosResponse_Queue(Sequence))
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

        //if (Length != responseArraySize - 3)
        //{
        //    strcpy(issuereason, "Unexpected DataLength!");
        //    return 22;
        //}

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
    //s8 returnCode = 9;
    //returnCode = Raw_CheckPosResponse_SID22(DID, Length, DIDdata);
    if (0 == Raw_CheckPosResponse_SID22(DID, Length, DIDdata))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    //else if(returnCode == 22)
    //    TestReport_StepInfo(Warning);
    else
    {
        TestReport_StepInfo(Fail);
        return -1;
    }
}

s32 CheckPosResponse_SID22_Warning(u8 DID[2], u32 Length, u8* DIDdata)
{
    if (0 == Raw_CheckPosResponse_SID22(DID, Length, DIDdata))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Warning);
        return -1;
    }
}

/// <summary>
/// 检查当前会话模式
/// </summary>
/// <param name="DiagnosticSession"></param> 诊断会话
/// <param name="AddressMode"></param> 寻址方式
/// <returns></returns>
s32 CheckCurrentSession(u8 DiagnosticSession, u8 AddressMode = 0,u8 Delay = 1)
{
    u8 DID[2] = { 0xF1,0x86 };
    u8 Session[2] = {};
    if(Delay == 1)
    {
        if (DiagnosticSession == 2)  TestWait(6000);
        else TestWait(1000);
    }
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

s32 CheckCurrentSession_Return(u8 DiagnosticSession, u8 AddressMode = 0, u8 Delay = 1)
{
    u8 DID[2] = { 0xF1,0x86 };
    u8 Session[2] = {};
    if (Delay == 1)
        TestWait(1000);
    Session[0] = DiagnosticSession;
    SID22_ReadDataByIdentifier(DID, AddressMode);
    CheckPosResponse_SID22(DID, 1, Session);

    switch (TpRxData[3])
    {
    case 1:
        TestReport_HTML_StepDescription("Current Session is DefaultSession");
        return 1;
    case 2:
        TestReport_HTML_StepDescription("Current Session is ProgrammingSession");
        return 2;
    case 3:
        TestReport_HTML_StepDescription("Current Session is ExtendedSession");
        return 3;
    default:
        TestReport_HTML_StepDescription("Unexpected Session");
        return-1;
    }
}

s32 CheckCurrentSession_Warning(u8 DiagnosticSession, u8 AddressMode = 0)
{
    u8 DID[2] = { 0xF1,0x86 };
    u8 Session[2] = {};
    s8 ReturnCode = 99;
    Session[0] = DiagnosticSession;
    SID22_ReadDataByIdentifier(DID, AddressMode);
    ReturnCode = CheckPosResponse_SID22_Warning(DID, 1, Session);
    /*if (ReturnCode == 0)
        return 0;*/
    switch (TpRxData[3])
    {
    case 1:
        TestReport_HTML_StepDescription("Current Session is DefaultSession",Warning);
        return 1;
    case 2:
        TestReport_HTML_StepDescription("Current Session is ProgrammingSession", Warning);
        return 2;
    case 3:
        TestReport_HTML_StepDescription("Current Session is ExtendedSession", Warning);
        return 3;
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

    sprintf(requestInfo, "%s%X %s%X %s%X", "23 ", AddLenFID,"", memoryAddress, "",memorySize);

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
        //log("CALL IS %X", SID31[i + 4]);
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
    if (OptionRecordLength > 0 && OptionRecord != NULL) {
        char hexBuffer[1000] = { 0 };
        int pos = 0;

        for (u64 i = 0; i < OptionRecordLength; i++) {
            if (i > 0) {
                hexBuffer[pos++] = ' ';
            }
            pos += sprintf(hexBuffer + pos, "%02X", OptionRecord[i]);
        }
        strcat(requestInfo, " ");
        strcat(requestInfo, hexBuffer);
    }
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
    char baseResponse[256] = { 0 };
    //snprintf(baseResponse, sizeof(baseResponse), "71 %s %s %s", dec2hex(SubFunction).c_str(), dec2hex(RID >> 8).c_str(), dec2hex(RID & 0xff).c_str());
    strcpy(baseResponse, ("71 " + dec2hex(SubFunction) + " " + dec2hex(RID >> 8) + " " + dec2hex(RID & 0xff)).c_str());
    char optionStr[1024] = { 0 };
    for (i = 0; i < optionRecordLength; i++) {
        snprintf(optionStr + strlen(optionStr), sizeof(optionStr) - strlen(optionStr), " %02X", optionRecord[i]);
    }

    snprintf(expectedResponse, sizeof(expectedResponse), "%s %s", baseResponse, optionStr);

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

    //if (optionRecordLength + 4 != responseArraySize)
    //{
    //    strcpy(issuereason, "Invalid Data Length Received!");
    //    app.log_text("Error!Invalid Data Length!", lvlError);
    //    return -1;
    //}

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
            if (DataCCR[m][i] != responseArray[i + 4])
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

//s32 Raw_CheckPosResponse_SID31_forCCR(s8 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
//{
//    u8 DataCCR[27][5] = {};
//    u8 checkflag = 0;
//    DataCCR[0][0] = 0x10;  DataCCR[0][1] = 0x00; DataCCR[0][2] = 0x00; DataCCR[0][3] = 0x00; DataCCR[0][4] = 0x11;
//    DataCCR[1][0] = 0x10;  DataCCR[1][1] = 0x00; DataCCR[1][2] = 0x00; DataCCR[1][3] = 0x00; DataCCR[1][4] = 0x21;
//    DataCCR[2][0] = 0x10;  DataCCR[2][1] = 0x00; DataCCR[2][2] = 0x00; DataCCR[2][3] = 0x00; DataCCR[2][4] = 0x41;
//    DataCCR[3][0] = 0x10;  DataCCR[3][1] = 0x00; DataCCR[3][2] = 0x00; DataCCR[3][3] = 0x01; DataCCR[3][4] = 0x01;
//    DataCCR[4][0] = 0x10;  DataCCR[4][1] = 0x00; DataCCR[4][2] = 0x00; DataCCR[4][3] = 0x02; DataCCR[4][4] = 0x01;
//    DataCCR[5][0] = 0x10;  DataCCR[5][1] = 0x00; DataCCR[5][2] = 0x00; DataCCR[5][3] = 0x04; DataCCR[5][4] = 0x01;
//    DataCCR[6][0] = 0x10;  DataCCR[6][1] = 0x00; DataCCR[6][2] = 0x00; DataCCR[6][3] = 0x08; DataCCR[6][4] = 0x01;
//    DataCCR[7][0] = 0x10;  DataCCR[7][1] = 0x00; DataCCR[7][2] = 0x00; DataCCR[7][3] = 0x10; DataCCR[7][4] = 0x01;
//    DataCCR[8][0] = 0x10;  DataCCR[8][1] = 0x00; DataCCR[8][2] = 0x00; DataCCR[8][3] = 0x20; DataCCR[8][4] = 0x01;
//    DataCCR[9][0] = 0x10;  DataCCR[9][1] = 0x00; DataCCR[9][2] = 0x00; DataCCR[9][3] = 0x40; DataCCR[9][4] = 0x01;
//    DataCCR[10][0] = 0x10; DataCCR[10][1] = 0x00; DataCCR[10][2] = 0x00; DataCCR[10][3] = 0x80; DataCCR[10][4] = 0x01;
//    DataCCR[11][0] = 0x10; DataCCR[11][1] = 0x00; DataCCR[11][2] = 0x01; DataCCR[11][3] = 0x00; DataCCR[11][4] = 0x01;
//    DataCCR[12][0] = 0x10; DataCCR[12][1] = 0x00; DataCCR[12][2] = 0x02; DataCCR[12][3] = 0x00; DataCCR[12][4] = 0x01;
//    DataCCR[13][0] = 0x10; DataCCR[13][1] = 0x00; DataCCR[13][2] = 0x04; DataCCR[13][3] = 0x00; DataCCR[13][4] = 0x01;
//    DataCCR[14][0] = 0x10; DataCCR[14][1] = 0x00; DataCCR[14][2] = 0x08; DataCCR[14][3] = 0x00; DataCCR[14][4] = 0x01;
//    DataCCR[15][0] = 0x10; DataCCR[15][1] = 0x00; DataCCR[15][2] = 0x10; DataCCR[15][3] = 0x00; DataCCR[15][4] = 0x01;
//    DataCCR[16][0] = 0x10; DataCCR[16][1] = 0x00; DataCCR[16][2] = 0x20; DataCCR[16][3] = 0x00; DataCCR[16][4] = 0x01;
//    DataCCR[17][0] = 0x10; DataCCR[17][1] = 0x00; DataCCR[17][2] = 0x40; DataCCR[17][3] = 0x00; DataCCR[17][4] = 0x01;
//    DataCCR[18][0] = 0x10; DataCCR[18][1] = 0x00; DataCCR[18][2] = 0x80; DataCCR[18][3] = 0x00; DataCCR[18][4] = 0x01;
//    DataCCR[19][0] = 0x10; DataCCR[19][1] = 0x01; DataCCR[19][2] = 0x00; DataCCR[19][3] = 0x00; DataCCR[19][4] = 0x01;
//    DataCCR[20][0] = 0x10; DataCCR[20][1] = 0x02; DataCCR[20][2] = 0x00; DataCCR[20][3] = 0x00; DataCCR[20][4] = 0x01;
//    DataCCR[21][0] = 0x10; DataCCR[21][1] = 0x04; DataCCR[21][2] = 0x00; DataCCR[21][3] = 0x00; DataCCR[21][4] = 0x01;
//    DataCCR[22][0] = 0x10; DataCCR[22][1] = 0x08; DataCCR[22][2] = 0x00; DataCCR[22][3] = 0x00; DataCCR[22][4] = 0x01;
//    DataCCR[23][0] = 0x10; DataCCR[23][1] = 0x10; DataCCR[23][2] = 0x00; DataCCR[23][3] = 0x00; DataCCR[23][4] = 0x01;
//    DataCCR[24][0] = 0x10; DataCCR[24][1] = 0x20; DataCCR[24][2] = 0x00; DataCCR[24][3] = 0x00; DataCCR[24][4] = 0x01;
//    DataCCR[25][0] = 0x10; DataCCR[25][1] = 0x40; DataCCR[25][2] = 0x00; DataCCR[25][3] = 0x00; DataCCR[25][4] = 0x01;
//    DataCCR[26][0] = 0x10; DataCCR[26][1] = 0x80; DataCCR[26][2] = 0x00; DataCCR[26][3] = 0x00; DataCCR[26][4] = 0x01;
//
//    // 新增调试：记录响应长度
//    char sizeBuf[32];
//    snprintf(sizeBuf, sizeof(sizeBuf), "Response Size: %d", responseArraySize);
//    app.log_text(sizeBuf, lvlError);
//
//    // 新增调试：记录完整响应数据
//    char responseHex[256] = "";
//    for (int i = 0; i < responseArraySize; i++) {
//        if (i == 0)
//            snprintf(responseHex, sizeof(responseHex), "%s", dec2hex(responseArray[i]).c_str());
//        else
//            snprintf(responseHex + strlen(responseHex), sizeof(responseHex) - strlen(responseHex),
//                " %s", dec2hex(responseArray[i]).c_str());
//    }
//    app.log_text("Response Data: ", lvlError);
//    app.log_text(responseHex, lvlError);
//
//    strcpy(expectedResponse, "71 01 02 05");
//    if (responseArraySize < 0)
//    {
//        strcpy(actualResponse, "No Response Received!");
//        strcpy(issuereason, "No Response Received!");
//        app.log_text("No Response Received!", lvlError);
//    }
//    else
//    {
//        for (int i = 0; i < responseArraySize; i++)
//        {
//            if (i == 0)
//                strcpy(actualResponse, dec2hex(responseArray[i]).c_str());
//            else
//                strcat(actualResponse, (" " + dec2hex(responseArray[i])).c_str());
//        }
//    }
//
//    if (responseArray[0] == 0x71)
//    {
//        app.log_text("Positive Response Received!", lvlOK);
//
//        if (responseArray[1] != 0x01)
//        {
//            strcpy(issuereason, "Unexpected Subfunction Received!");
//            app.log_text("Unexpected SubFunction Received!", lvlError);
//            return -1;
//        }
//    }
//    else {
//        strcpy(issuereason, "Unexpected Response Received!");
//        app.log_text("Unexpected Response Received!", lvlError);
//        return -1;
//    }
//
//    if (responseArray[2] != 0x02 || responseArray[3] != 0x05)
//    {
//        strcpy(issuereason, "Unexpected RoutineIdentifier Received!");
//        app.log_text("Unexpected RoutienIdentifier Received!", lvlError);
//        return -1;
//    }
//
//    // 新增调试：记录选项记录
//    char optionRecord[64] = "";
//    for (int i = 5; i < responseArraySize && i < 10; i++) {
//        if (i == 5)
//            snprintf(optionRecord, sizeof(optionRecord), "%s", dec2hex(responseArray[i]).c_str());
//        else
//            snprintf(optionRecord + strlen(optionRecord), sizeof(optionRecord) - strlen(optionRecord),
//                " %s", dec2hex(responseArray[i]).c_str());
//    }
//    app.log_text("Option Record: ", lvlError);
//    app.log_text(optionRecord, lvlError);
//
//    for (int m = 0; m < 27; m++)
//    {
//        checkflag = 1;
//        for (int i = 0; i < 5; i++)
//        {
//            if (DataCCR[m][i] != responseArray[i + 4])
//            {
//                // 新增调试：记录不匹配的字节
//                char mismatchBuf[128];
//                snprintf(mismatchBuf, sizeof(mismatchBuf),
//                    "Mismatch at DataCCR[%d][%d]: Expected %s, Got %s",
//                    m, i, dec2hex(DataCCR[m][i]).c_str(),
//                    dec2hex(responseArray[i + 5]).c_str());
//                app.log_text(mismatchBuf, lvlError);
//                checkflag = 0;
//            }
//        }
//        if (checkflag == 1)
//            break;
//    }
//
//    if (checkflag != 1)
//    {
//        strcpy(issuereason, "Unexpected optionRecord Received!");
//        app.log_text("Unexpected optionRecord Received!", lvlError);
//        return -1;
//    }
//    else
//        app.log_text("Correct optionRecord Received!", lvlOK);
//
//    if (9 != responseArraySize)
//    {
//        strcpy(issuereason, "Invalid Data Length Received!");
//        app.log_text("Error!Invalid Data Length!", lvlError);
//        return -1;
//    }
//
//    return 0;
//}

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
                //printf("SeedArray:%X", SeedArray[i]);
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

s32 CheckPosResponse_SID27_RequestSeed_Warning(u8 SubFunction)
{
    if (0 == Raw_CheckPosResponse_SID27_RequestSeed(SubFunction))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Warning);
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
    u8 SID27[18] = {0};
    SID27[0] = 0x27;
    SID27[1] = SecurityLevel + 1;
    s32 responseArraySize = 100;

    for (int i = 0; i < 16; i++)
        SID27[i + 2] = KeyArray[i];

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID27, 18, responseArray, &responseArraySize) == 0x00) {
            //app.log_text("send diagnostic payload and get response success!", lvlOK);
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

s32 CheckPosResponse_SID27_SendKey_Warning(u8 SecurityLevel)
{
    if (0 == Raw_CheckPosResponse_SID27_SendKey(SecurityLevel))
    {
        TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        TestReport_StepInfo(Warning);
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

s32 Raw_CheckPosResponse_SID29_SF01(u8 returnValue,u8* challengeServer = ChallengeServer,u8* ephemeralPublicKeyServer = EphemeralPublicKeyServer, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("69 01 " + dec2hex(returnValue)).c_str());
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
        TestReport_HTML_StepDescription("ECU give positive response!", Pass);
        if (responseArray[1] != 0x01)
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

        //校验ChallengeServer和EphemeralPublicKeyServer

    }
    else
    {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected  Resposne!", lvlError);
        return -1;
    }
}
s32 CheckPosResponse_SID29_SF01(u8 returnValue)
{
    if (0 == Raw_CheckPosResponse_SID29_SF01(returnValue))
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

s32 Raw_SID29_Authentication_SF02(u8 communicationConfiguration, u16 lengthOfProofOfOwnershipClient, u8* certificateClient, u16 lengthOfChallengeClient, u8* challengeClient, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID29[100] = {};
    s32 responseArraySize = 100;
    SID29[0] = 0x29;
    SID29[1] = 0x02;
    SID29[2] = communicationConfiguration;
    SID29[3] = lengthOfProofOfOwnershipClient >> 8;
    SID29[4] = lengthOfProofOfOwnershipClient & 0xff;
    for (int i = 0; i < lengthOfProofOfOwnershipClient; i++)
        SID29[i + 5] = certificateClient[i];
    SID29[5 + lengthOfProofOfOwnershipClient] = lengthOfChallengeClient >> 8;
    SID29[6 + lengthOfProofOfOwnershipClient] = lengthOfChallengeClient & 0xff;
    for (int i = 0; i < lengthOfChallengeClient; i++)
        SID29[7 + i + lengthOfProofOfOwnershipClient] = challengeClient[i];

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID29, 7 + lengthOfProofOfOwnershipClient + lengthOfChallengeClient, responseArray, &responseArraySize) == 0x00) {
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
s32 SID29_Authentication_SF02(u8 communicationConfiguration, u16 lengthOfProofOfOwnershipClient, u8* certificateClient, u16 lengthOfChallengeClient, u8* challengeClient, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID29_Authentication_SF02(communicationConfiguration, lengthOfProofOfOwnershipClient, certificateClient, lengthOfChallengeClient, challengeClient, AddressMode);
    
    strcpy(requestInfo, ("29 02 " + dec2hex(communicationConfiguration)).c_str());
    return 0;
}

s32 Raw_CheckPosResponse_SID29_SF02(u8 returnValue, u8* challengeServer = ChallengeServer, u8* ephemeralPublicKeyServer = EphemeralPublicKeyServer, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("69 02 " + dec2hex(returnValue)).c_str());
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
        if (responseArray[1] != 0x02)
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

        //校验ChallengeServer和EphemeralPublicKeyServer

    }
    else
    {
        strcpy(issuereason, "Unexpected Response Received!");
        app.log_text("Unexpected  Resposne!", lvlError);
        return -1;
    }
}
s32 CheckPosResponse_SID29_SF02(u8 returnValue)
{
    if (0 == Raw_CheckPosResponse_SID29_SF02(returnValue))
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

s32 Raw_SID29_Authentication_SF03(u8 communicationConfiguration, u16 lengthOfProofOfOwnershipClient, u8* proofOfOwnershipClient, u16 lengthOfEphemeralPublicKeyClient, u8* ephemeralPublicKeyClient, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID29[100] = {};
    s32 responseArraySize = 100;
    SID29[0] = 0x29;
    SID29[1] = 0x03;
    SID29[2] = communicationConfiguration;
    SID29[3] = lengthOfProofOfOwnershipClient >> 8;
    SID29[4] = lengthOfProofOfOwnershipClient & 0xff;
    for (int i = 0; i < lengthOfProofOfOwnershipClient; i++)
        SID29[i + 5] = proofOfOwnershipClient[i];
    SID29[5 + lengthOfProofOfOwnershipClient] = lengthOfEphemeralPublicKeyClient >> 8;
    SID29[6 + lengthOfProofOfOwnershipClient] = lengthOfEphemeralPublicKeyClient & 0xff;
    for (int i = 0; i < lengthOfEphemeralPublicKeyClient; i++)
        SID29[7 + i + lengthOfProofOfOwnershipClient] = ephemeralPublicKeyClient[i];

    if (AddressMode == 0) {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID29, 7 + lengthOfProofOfOwnershipClient + lengthOfEphemeralPublicKeyClient, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID29, 7 + lengthOfProofOfOwnershipClient + lengthOfEphemeralPublicKeyClient, responseArray, &responseArraySize) == 0x00) {
            app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else {
            app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID29_Authentication_SF03(u8 communicationConfiguration, u16 lengthOfProofOfOwnershipClient, u8* proofOfOwnershipClient, u16 lengthOfEphemeralPublicKeyClient, u8* ephemeralPublicKeyClient, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID29_Authentication_SF03(communicationConfiguration, lengthOfProofOfOwnershipClient, ProofOfOwnershipClient, lengthOfEphemeralPublicKeyClient, ephemeralPublicKeyClient, AddressMode);
    strcpy(requestInfo, ("29 03 " + dec2hex(communicationConfiguration)).c_str());
    return 0;
}

s32 Raw_CheckPosResponse_SID29_SF03(u8 returnValue, u16 lengthOfSessionKeyInfo, u8* sessionKeyInfo, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("69 03 " + dec2hex(returnValue)).c_str());

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
        if (responseArray[1] != 0x03)
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

        //校验lengthOfSessionKeyInfo和sessionKeyInfo

    }
    else
    {
        app.log_text("Unexpected  Resposne!", lvlError);
        strcpy(issuereason, "Unexpected Response Received!");
        return -1;
    }
}
s32 CheckPosResponse_SID29_SF03(u8 returnValue, u16 lengthOfSessionKeyInfo, u8* sessionKeyInfo)
{
    if (0 == Raw_CheckPosResponse_SID29_SF03(returnValue, lengthOfSessionKeyInfo, sessionKeyInfo))
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
    s32 responseArraySize = 4092;
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
    std::string hexes[16] = { "0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f" };
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
    strcpy(expectedResponse, ("59 " + dec2hex(SubFunction[0]) + " "+dec2hex(SubFunction[1])).c_str());
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
    switch (SubFunction[1])
    {
        //读取客户端定义状态掩码匹配的DTC（Diagnostic Trouble Code）数量
    case 0x01: {
        s32 dtc_number = (responseArray[4] << 8) + responseArray[5];
        //app.log_text(("读取到的DTC数量为" + std::to_string(dtc_number)).c_str(), Pass);
        strcpy(printChar, "");
        sprintf(printChar, "%s%d", "读取到的DTC数量为", dtc_number);
        TestReport_HTML_StepDescription(printChar);

        break; }
             //读取客户端定义状态掩码匹配的DTC及其掩码
    case 0x02: {
        std::string dtc_hex = dealDTC(responseArray, responseArraySize);
        //app.log_text(("与状态码匹配的DTC为:" + dtc_hex).c_str(), Pass);
        strcpy(printChar, "");
        sprintf(printChar, "%s%s", "与状态码匹配的DTC为:0x", dtc_hex.c_str());
        TestReport_HTML_StepDescription(printChar);
        break;
    }
             //该子功能请求来检索所有捕获的 DTC快照记录的ID信息(所以id支持的快照信息)
    case 0x03: {
        std::string hex_DTC;
        for (int i = 2; i < responseArraySize; i += 4) {
            u32 evdtc = 0;
            for (int a = 0; a < 4; a++) {
                evdtc += responseArray[i + a];
                if (a < 3)
                    evdtc <<= 8;
            }
            hex_DTC += intToHexStr(evdtc) + "/";
        }
        //TestReport_HTML_StepDescription(("DTC快照记录的信息" + hex_DTC).c_str(), Pass);
        strcpy(printChar, "");
        sprintf(printChar, "%s%s", "与状态码匹配的DTC为:0x", hex_DTC.c_str());
        TestReport_HTML_StepDescription(printChar);
    }
             //检索客户端定义DTC掩码的记录数据(快照)如发生某一故障记录DTC时的车速，电源电压状态等
    case 0x04: {
        TestReport_HTML_StepDescription(("DTC的状态掩码为:" + intToHexStr(responseArray[5], 2)).c_str(), Pass);
        //TestReport_HTML_StepDescription(("快照DID的个数"+std::to_string(responseArray[7])).c_str(), Pass);
        break;
    }
             //读取某个DTC及其相关的环境数据
    case 0x06: {
        TestReport_HTML_StepDescription(("DTC的状态掩码为:" + intToHexStr(responseArray[5], 2)).c_str(), Pass);
        //TestReport_HTML_StepDescription(("快照DID的个数" + std::to_string(responseArray[7])).c_str(), Pass);
        break;
    }
             //获取所有DTC的状态位
    case 0x0A: {
        std::string dtc_hex = dealDTC(responseArray, responseArraySize);
        //TestReport_HTML_StepDescription(("所有的DTC及其状态位为:" + dtc_hex).c_str(), Pass);
        strcpy(printChar, "");
        sprintf(printChar, "%s%s", "所有的DTC及其状态位为:", dtc_hex.c_str());
        TestReport_HTML_StepDescription(printChar);
        break;
    }
    case 0x15: {
        std::string dtc_hex = dealDTC(responseArray, responseArraySize);
        TestReport_HTML_StepDescription(("所有的DTC及其状态位为:" + dtc_hex).c_str(), Pass);
        break;
    }
    default:
        break;
    }
    return 0;
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

s32 Raw_CheckPosResponse_DTC_SID19(pu8 SubFunction, pu32 dtc, u8 DTCNum,s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
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
        for (u8 j = 0; i < DTCNum; j++) {
            if (evdtc == dtc[j])
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


s32 CheckPosResponse_DTC_SID19(pu8 SubFunction, pu32 dtc, u8 DTCNum,u8 Astats)
{
    char StepDespriction[500];
    if (Astats == 0)
    {
        if (0 == Raw_CheckPosResponse_DTC_SID19(SubFunction, dtc, DTCNum))
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
        if (0 == Raw_CheckPosResponse_DTC_SID19(SubFunction, dtc,DTCNum))
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

s32 RX_CheckResponse_DTC_SID19_02(u8 Mask, pu32 dtc,u8 DTCNum, u8 Astats = 0) {
    u8 sub1902[2] = {};
    sub1902[0] = 0x02;
    sub1902[1] = Mask;
    return  CheckPosResponse_DTC_SID19(sub1902, dtc, DTCNum,Astats);
}

s32 Raw_CheckPosResponse_SID19_02_DTCNum(pu8 SubFunction, s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
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

    if ((((TpRxLength - 3) % 4) != 0) && TpRxLength > 3) {
        return -1;
    }
    else {
        TestReport_HTML_StepDescription("Find DTC");
    }
    return 0;
}
s32 CheckPosResponse_SID19_02_DTCNum(pu8 SubFunction)
{
    if (0 == Raw_CheckPosResponse_SID19_02_DTCNum(SubFunction))
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
s32 Raw_SID2E_WriteDataById(u8 SubFunction[], u16 SubSize, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID2E[600] = {};
    SID2E[0] = 0x2E;
    for (int i = 0; i < SubSize; i++)
    {
        SID2E[1 + i] = SubFunction[i];
    }
    s32 responseArraySize = 600;
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
s32 SID2E_WriteDataById(u8 SubFunction[], u16 SubSize, u8 AddressMode = 0)
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
/// Service0x2E 按数据标识符写入数据
/// </summary>
/// <param name="SubSize"></param> SubSize         $2E服务子功能长度
/// <param name="SubFunction"></param> SubFunction $2E服务子功能
/// <param name="AddressMode"></param> AddressMode $2E服务寻址方式
s32 Raw_SID2E_WriteDataById_Request(u8 SubFunction[], u16 SubSize, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID2E[600] = {};
    SID2E[0] = 0x2E;
    for (int i = 0; i < SubSize; i++)
    {
        SID2E[1 + i] = SubFunction[i];
    }
    s32 responseArraySize = 600;
    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_send_request(udsHandle, SID2E, SubSize + 1) == 0x00)
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
        if (rtlUIDiagnostics.tstp_send_functional(udsHandle, SID2E, SubSize + 1) == 0x00)
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
s32 SID2E_WriteDataById_Request(u8 SubFunction[], u16 SubSize, u8 AddressMode = 0)
{
    strcpy(requestInfo, "2E");
    for (int i = 0; i < SubSize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }
    Raw_SID2E_WriteDataById_Request(SubFunction, SubSize, AddressMode);

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
    strcpy(expectedResponse,"");
    strcpy(actualResponse,"");
    printf("2E responseArraySize=%d", responseArraySize);
    strcpy(expectedResponse, ("6E " + dec2hex(SubFunction[0]) + " " + dec2hex(SubFunction[1]) ).c_str());
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
            printf("responseArray[%d]=%0X", i, responseArray[i]);
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
    if (responseArray[0] == 0x6E && responseArray[1] == SubFunction[0] && responseArray[2] == SubFunction[1])
    {
        
        app.log_text("Positive Response Received!", lvlOK);
        return 0;
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

    sprintf(requestInfo, "%s%X %s%X %s%X", "3D ", AddLenFID, "", memoryAddress, "", memorySize);
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
            //app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            //app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID34, SubSize + 1, responseArray, &responseArraySize) == 0x00)
        {
            //app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            //app.log_text("send diagnostic payload and get response faliled!", lvlError);
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
        //app.log_text("Positive Response Received!", lvlOK);

        reqSWDL_ID = responseArray[1];
        MaxLength = (responseArray[2] << 8) + responseArray[3];
        strcpy(printChar, "");
        sprintf(printChar, "%s%X", "MaxLength is 0x", MaxLength);
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
        sprintf(printChar, "%s%X", "maxlength is", MaxLength);
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
    sprintf(printChar, "%s%X", "the length of maxNumberOfBlockLength is 0x", responseArray[2]);
    TestReport_HTML_StepDescription(printChar);
    strcpy(printChar, "");
    sprintf(printChar, "%s%X", "maxNumberOfBlockLength is 0x", ((responseArray[3] << 8) + responseArray[4]));
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
    sprintf(printChar, "%s%X", "the length of maxNumberOfBlockLength is 0x", responseArray[1]);
    TestReport_HTML_StepDescription(printChar);
    strcpy(printChar, "");
    sprintf(printChar, "%s%X", "maxNumberOfBlockLength is 0x", ((responseArray[2] << 8) + responseArray[3]));
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
s32 Raw_SID36_Transfer_Data_Different(u8 SequenceNumber, u8 SubFunction[], u64 SubSize, u8 AddressMode=0, u8* responseArray = TpRxData)
{
    u8 SID36[65535] = {};
    SID36[0] = 0x36;
    SID36[1] = SequenceNumber;
    //char SeqNumber[100] = {};
    //snprintf(SeqNumber, 100, "Transfer 36 %02X", SequenceNumber);
    //TestReport_HTML_StepDescription(SeqNumber);
    for (int i = 0; i < SubSize; i++)
    {
        SID36[2 + i] = SubFunction[i];
    }
    s32 responseArraySize = 100;
    if (AddressMode == 0)
    {
        u8 returnCode = 0;
        //returnCode = rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID36, SubSize + 2, responseArray, &responseArraySize);
        returnCode = rtlUIDiagnostics.tstp_send_request(udsHandle, SID36, SubSize + 2);
        if (returnCode == 0x00)
        {
            //printf("transfer data success!");
        }
        else
        {
            printf("transfer data failed,sequence is %d,code is %d", SequenceNumber,returnCode);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID36, SubSize + 2, responseArray, &responseArraySize) == 0x00)
        {
            //app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            //app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID36_Transfer_Data_Different(u8 SequenceNumber, u8 SubFunction[], u64 SubSize, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID36_Transfer_Data_Different(SequenceNumber, SubFunction, SubSize, AddressMode);
    strcpy(requestInfo, "36 ");
    strcat(requestInfo, dec2hex(SequenceNumber).c_str());
    /*for (int i = 0; i < SubSize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }*/
    return 0;
}

s32 Raw_SID36_Transfer_Data(u8 SequenceNumber, u8 SubFunction[], u64 SubSize, u8 AddressMode = 0, u8* responseArray = TpRxData)
{
    u8 SID36[65535] = {};
    SID36[0] = 0x36;
    SID36[1] = SequenceNumber;
    //char SeqNumber[100] = {};
    //snprintf(SeqNumber, 100, "Transfer 36 %02X", SequenceNumber);
    //TestReport_HTML_StepDescription(SeqNumber);
    for (int i = 0; i < SubSize; i++)
    {
        SID36[2 + i] = SubFunction[i];
    }
    s32 responseArraySize = 100;
    if (AddressMode == 0)
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response(udsHandle, SID36, SubSize + 2, responseArray, &responseArraySize) == 0x00)
        {
            //app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            //app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
    else
    {
        if (rtlUIDiagnostics.tstp_request_and_get_response_functional(udsHandle, SID36, SubSize + 2, responseArray, &responseArraySize) == 0x00)
        {
            //app.log_text("send diagnostic payload and get response success!", lvlOK);
            return responseArraySize;
        }
        else
        {
            //app.log_text("send diagnostic payload and get response faliled!", lvlError);
            return -1;
        }
    }
}
s32 SID36_Transfer_Data(u8 SequenceNumber, u8 SubFunction[], u64 SubSize, u8 AddressMode = 0)
{
    TpRxLength = Raw_SID36_Transfer_Data(SequenceNumber, SubFunction, SubSize, AddressMode);
    strcpy(requestInfo, "36 ");
    strcat(requestInfo, dec2hex(SequenceNumber).c_str());
    /*for (int i = 0; i < SubSize; i++)
    {
        strcat(requestInfo, " ");
        strcat(requestInfo, dec2hex(SubFunction[i]).c_str());
    }*/
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
    if (SubFunction == 0xDE) {
        printf("receive 0xDE");
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
        //app.log_text("Positive Response Received!", lvlOK);
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
    char NRCBuffer[100] = {};
    if (0 == Raw_CheckPosResponse_SID36(SubFunction))
    {
        //TestReport_StepInfo(Pass);
        return 0;
    }
    else
    {
        if (TpRxLength > 0 && TpRxData[0] == 0x7F) {
            snprintf(NRCBuffer, 100, "传输数据中断，未收到期望响应，NRC%02X", TpRxData[2]);
            TestReport_HTML_StepDescription(NRCBuffer, Fail);
        }
        else
            TestReport_StepInfo(Fail);
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
    sub1906[0] = 0x04;
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
 
//zy 20251216 19 0A检查
s32 Raw_CheckPosResponse_SID19_0A(s32 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, ("59 0A"));
    //strcpy(actualResponse, );
    if (responseArraySize < 0)
    {
        strcpy(actualResponse, "No Response Received!");
    }
 
    if (responseArraySize < 0)
    {
        TestReport_StepInfo(Fail);
        return -1;
    }

    if (responseArray[0] == 0x7F)
    {

        return -1;
    }

    if (responseArray[0] == 0x59) {
        snprintf(actualResponse,sizeof(actualResponse),"%X %X %X", TpRxData[0], TpRxData[1], TpRxData[2]);
    }
    u8 receivedDTCs[1000][3];
    u16 receivedCount = 0;

    for (u16 i = 3; i < TpRxLength; i += 4)
    {
        if (i + 2 >= TpRxLength)
        {
   /*         snprintf(NRCBuffer, 100, "传输数据中断，未收到期望响应，NRC%02X", TpRxData[2]);*/
            TestReport_HTML_StepDescription("DTC格式错误", Fail);
            return -1;
        }
        receivedDTCs[receivedCount][0] = TpRxData[i];
        receivedDTCs[receivedCount][1] = TpRxData[i + 1];
        receivedDTCs[receivedCount][2] = TpRxData[i + 2];
        receivedCount++;

    }

    if (receivedCount != dtcsize)
    {
        strcpy(printChar, "");
        sprintf(printChar, "故障码数量异常(期望%d，实际%d)!", dtcsize, receivedCount);
        TestReport_HTML_StepDescription(printChar, Warning);
    }

    bool DTC_allMatched = true;
    bool DTC_isMatched[1000] = { false };

    u32 UnexpectDTC[1000] = { 0 };
    u16 unexpectDTC_Count = 0;

    u32 LackDTC[1000] = { 0 };//DTCList中缺失的DTC
    u16 lackDTC_Count = 0;

    for (u16 m = 0; m < dtcsize; m++)
    {
        bool found = false;

        for (u16 r = 0; r < receivedCount; r++)
        {
            if (!DTC_isMatched[r])
            {
                if (DTCList[m].DTC_Byte[0] == receivedDTCs[r][0] && DTCList[m].DTC_Byte[1] == receivedDTCs[r][1] && DTCList[m].DTC_Byte[2] == receivedDTCs[r][2])
                {
                    found = true;
                    DTC_isMatched[r] = true;
                    break;
                }
            }
        }
        if (!found)
        {
            DTC_allMatched = false;
            if (lackDTC_Count < 1000)
            {
                LackDTC[lackDTC_Count] = ((u32)DTCList[m].DTC_Byte[0] << 16) | ((u32)DTCList[m].DTC_Byte[1] << 8) | (u32)DTCList[m].DTC_Byte[2];
                lackDTC_Count++;
            }
        }
    }
    for (u16 r = 0; r < receivedCount; r++)
    {
        if (!DTC_isMatched[r] && unexpectDTC_Count < 1000)
        {
            UnexpectDTC[unexpectDTC_Count] = ((u32)receivedDTCs[r][0] << 16) | ((u32)receivedDTCs[r][1] << 8) | (u32)receivedDTCs[r][2];
            unexpectDTC_Count++;
        }
    }

    if (DTC_allMatched)
    {
        TestReport_HTML_StepDescription("DTC所有故障码正确", Pass);
        return 0;
    }
    else
    {
        if (unexpectDTC_Count > 0)
        {
            char failinfo[1000];
            char unexpectDTCinfo[1000];
            sprintf(failinfo, "存在非预期DTC%d个:", unexpectDTC_Count);
            for (int i = 0; i < unexpectDTC_Count; i++)
            {
                sprintf(unexpectDTCinfo, "0x%X ", UnexpectDTC[i]);
                strcat(failinfo, unexpectDTCinfo);
                if (i == 50)
                {
                    strcat(failinfo, "...");
                    break;
                }

            }
            TestReport_HTML_StepDescription(failinfo, Fail);
        }
        if (lackDTC_Count > 0)
        {
            char failinfo[1000];
            char lackDTCinfo[1000];
            sprintf(failinfo, "存在%d个DTCList中缺失的DTC:", lackDTC_Count);
            for (int i = 0; i < lackDTC_Count; i++)
            {
                sprintf(lackDTCinfo, "0x%X ", LackDTC[i]);
                strcat(failinfo, lackDTCinfo);
                if (i == 200)
                {
                    strcat(failinfo, "...");
                    break;
                }

            }
            TestReport_HTML_StepDescription(failinfo, Fail);
        }
        return -1;
    }
}
s32 CheckPosResponse_SID19_0A()
{
    if (0 == Raw_CheckPosResponse_SID19_0A())
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
    u8 rec[1] = { 0x10};
    char tempAddress[200] = {};
    snprintf(tempAddress, 200, "Erase Address 0x%08X,Length 0x%08X", VbfFileInfo.EraseAddresss, VbfFileInfo.EraseLength);
    TestReport_HTML_StepDescription(tempAddress);
        opt[0] = VbfFileInfo.EraseAddresss >> 24;
        opt[1] = VbfFileInfo.EraseAddresss >> 16;
        opt[2] = VbfFileInfo.EraseAddresss >> 8;
        opt[3] = VbfFileInfo.EraseAddresss;
        opt[4] = VbfFileInfo.EraseLength >> 24;
        opt[5] = VbfFileInfo.EraseLength >> 16;
        opt[6] = VbfFileInfo.EraseLength >> 8;
        opt[7] = VbfFileInfo.EraseLength;
        
        SID31_RoutineControl(0x01, 0xFF00, 8, opt);
        CheckPosResponse_SID31(0x01, 0xFF00, 1, rec);

        if (ifaboart == 1)
            return 0;

    //u8 DID[2] = { 0xF1,0xAA };
    //SID22_ReadDataByIdentifier(DID);
    //CheckPosResponse_SID22(DID, 8);

    return 0;
}

s32 EraseAddress_DisconnectPowerSupply(u8 i = 0, u8 different = 0)
{
    u8 opt[8] = {};
    u8 rec[1] = { 0x10};
    char TempErase[100] = {};

    //按照块地址+长度擦除，实际会回复71 01 FF 00 11
    //opt[0] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 24;
    //opt[1] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 16;
    //opt[2] = VbfFileInfo.VbfDataBlcok[i].StartAddress >> 8;
    //opt[3] = VbfFileInfo.VbfDataBlcok[i].StartAddress;
    //opt[4] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 24;
    //opt[5] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 16;
    //opt[6] = VbfFileInfo.VbfDataBlcok[i].DataLength >> 8;
    //opt[7] = VbfFileInfo.VbfDataBlcok[i].DataLength;

    //按照VBF Header内的EraseAddress擦除
    opt[0] = VbfFileInfo.EraseAddresss >> 24;
    opt[1] = VbfFileInfo.EraseAddresss >> 16;
    opt[2] = VbfFileInfo.EraseAddresss >> 8;
    opt[3] = VbfFileInfo.EraseAddresss;
    opt[4] = VbfFileInfo.EraseLength >> 24;
    opt[5] = VbfFileInfo.EraseLength >> 16;
    opt[6] = VbfFileInfo.EraseLength >> 8;
    opt[7] = VbfFileInfo.EraseLength;
    snprintf(TempErase,100, "Erase Address 0x%08X,Length 0x%08X", VbfFileInfo.EraseAddresss, VbfFileInfo.EraseLength);
    TestReport_HTML_StepDescription(TempErase);
    if(different==0)
        SID31_RoutineControl_EraseMemory(opt,8);
    else
    {
        SID31_RoutineControl(0x01, 0xFF00, 8, opt);
        CheckPosResponse_SID31(0x01, 0xFF00, 1, rec);
    }
    return 0;
}

s32 FileDownload(u8 Dupli36=0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};
    char StepDespriction[256];
    for(int i=0;i<VbfFileInfo.DataBlockNumber;i++)
    {
        sequenceNum = 1;

        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
        log("Parse is %s, Format is 0x%02X", VbfFileInfo.data_format_identifier, subfunc[0]);
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
                    //CANoe报告中带有下列步骤，但是log中没有进行，所以此处直接将每个block最后一个包发送两次
                    //if (remainLength - offset == 1)
                    //{
                    //    SID36_Transfer_Data(sequenceNum, transferData, 0);
                    //}
                    //else if(remainLength - offset <=7)
                    //{
                    //    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset - 1);
                    //}
                    //else if ((remainLength - offset - 6) % 7 == 0)
                    //{
                    //    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset - 7);
                    //}
                    //else
                    //{
                    //    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset - (remainLength - offset - 6) % 7);
                    //}
                    //CheckNonResponse();
                    TestReport_HTML_StepDescription("block中最后一个包，第一次发送");
                    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                    CheckPosResponse_SID36(sequenceNum);
                    TestReport_HTML_StepDescription("block中最后一个包，第二次发送");
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
        snprintf(StepDespriction, sizeof(StepDespriction), "TransferData(0x36) %dth segmentblock", i+1);
        TestReport_HTML_StepDescription(StepDespriction);

        SID37_RequestTransferExit();
        CheckPosResponse_SID37();
    }

    return 0;
}

s32 FileDownload_NRC(char* keyName,pu8 NRC,u8 Dupli36 = 0, u8 eraseFlag = 1)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};
    char StepDespriction[256];

    //char keyvalue[20] = "APP0Path";
    char filename[300] = {};
    vbfparser(keyName);
    filename[0] = '\0';
    if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
        log("Error: Failed to get filename for %s", keyName);
        snprintf(StepDespriction, sizeof(StepDespriction), "Download APP (no filename)");
    }
    else {
        // 安全截断文件名
        size_t len = strlen(filename);
        if (len >= 9) {
            filename[len - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
        }
        else {
            log("Warning: Filename '%s' too short (len=%zu)", filename, len);
            snprintf(StepDespriction, sizeof(StepDespriction), "Download APP (short filename)");
        }
    }
    if(eraseFlag == 1){
        TestReport_HTML_StepDescription(StepDespriction);
        u8 opt[8] = {};
        u8 rec[1] = { 0x10 };
        char tempAddress[200] = {};
        snprintf(tempAddress, 200, "Erase Address 0x%08X,Length 0x%08X", VbfFileInfo.EraseAddresss, VbfFileInfo.EraseLength);
        TestReport_HTML_StepDescription(tempAddress);
        opt[0] = VbfFileInfo.EraseAddresss >> 24;
        opt[1] = VbfFileInfo.EraseAddresss >> 16;
        opt[2] = VbfFileInfo.EraseAddresss >> 8;
        opt[3] = VbfFileInfo.EraseAddresss;
        opt[4] = VbfFileInfo.EraseLength >> 24;
        opt[5] = VbfFileInfo.EraseLength >> 16;
        opt[6] = VbfFileInfo.EraseLength >> 8;
        opt[7] = VbfFileInfo.EraseLength;

        SID31_RoutineControl(0x01, 0xFF00, 8, opt);
        //CheckPosResponse_SID31(0x01, 0xFF00, 1, rec);
        CheckNegResponse(NRC);
    }

    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        sequenceNum = 1;

        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
        log("Parse is %s, Format is 0x%02X", VbfFileInfo.data_format_identifier, subfunc[0]);
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
        if (0 != CheckNegResponse(NRC)) {
            TestReport_HTML_StepDescription("34服务非预期NRC",Fail);
            return -1;
        }
        else {
            TestReport_HTML_StepDescription("34服务收到期望NRC", Pass);
        }
            
        //if (0 != CheckPosResponse_SID34(subfunc))
        //    return -1;

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
                //CheckPosResponse_SID36(sequenceNum);
                if (0 != CheckNegResponse(NRC)) {
                    TestReport_HTML_StepDescription("36服务非预期NRC", Fail);
                    return -1;
                }
                else {
                    TestReport_HTML_StepDescription("36服务收到期望NRC", Pass);
                }
            }

            else
            {
                SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                //CheckPosResponse_SID36(sequenceNum);
                if (0 != CheckNegResponse(NRC)) {
                    TestReport_HTML_StepDescription("36服务非预期NRC", Fail);
                    return -1;
                }
                else {
                    TestReport_HTML_StepDescription("36服务收到期望NRC", Pass);
                }
            }
            sequenceNum++;
        }
        snprintf(StepDespriction, sizeof(StepDespriction), "TransferData(0x36) %dth segmentblock", i + 1);
        TestReport_HTML_StepDescription(StepDespriction);

        SID37_RequestTransferExit();
        //CheckPosResponse_SID37();
        if (0 != CheckNegResponse(NRC)) {
            TestReport_HTML_StepDescription("37服务非预期NRC", Fail);
            return -1;
        }
        else {
            TestReport_HTML_StepDescription("37服务收到期望NRC", Pass);
        }
    }

    return 0;
}

s32 FileDownload_Async(u8 Dupli36 = 0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};
    char StepDespriction[256];
    CheckFlag = true;
    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        sequenceNum = 1;

        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
        log("Parse is %s, Format is 0x%02X", VbfFileInfo.data_format_identifier, subfunc[0]);
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
        memset(rx, 0, sizeof(rx));
        u64 MaxDataLength = MaxLength - 2;
        u64 remainLength = VbfFileInfo.VbfDataBlcok[i].DataLength;
        //rtlUIDiagnostics.tsdiag_transfer_data(udsHandle, VbfFileInfo.VbfDataBlcok[i].BlockData, remainLength+2, 1);
        for (u64 offset = 0; offset <= remainLength; offset += MaxDataLength)
        {
            for (int j = 0; j < MaxDataLength; j++)
            {
                transferData[j] = VbfFileInfo.VbfDataBlcok[i].BlockData[j + offset];
            }
        
            if (remainLength - offset >= MaxDataLength)
            {
                if (sequenceNum <= 2) {
                    SID36_Transfer_Data_Different(sequenceNum, transferData, MaxDataLength);
                }
                else{
                    s64 t, t1;
                    app.get_timestamp(&t);
                    while (1) {
                        if (0 == CheckPosResponse_Queue(sequenceNum - 2)) {
                            SID36_Transfer_Data_Different(sequenceNum, transferData, MaxDataLength);
                            break;
                        }
                        app.get_timestamp(&t1);
                        if (t1 - t > 3000000)
                            return -1;
                    }
                }
            }
        
            else
            {
                if (Dupli36 == 1)
                {
        
                }
                else
                {
                    s64 t, t1;
                    app.get_timestamp(&t);
                    printf("Last Sequence");
                    if(sequenceNum >= 3){
                        while (1) {
                            if (0 == CheckPosResponse_Queue(sequenceNum - 1)) {
                                break;
                            }
                            app.get_timestamp(&t1);
                            if (t1 - t > 3000000)
                                return -1;
                        }
                    }
                    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                    CheckPosResponse_SID36(sequenceNum);
                    app.get_timestamp(&t);
                    while (1) {
                        if (0 == CheckPosResponse_Queue(sequenceNum)) {
                            break;
                        }
                        app.get_timestamp(&t1);
                        if (t1 - t > 3000000)
                            return -1;
                    }
                }
            }
            sequenceNum++;
        }
        snprintf(StepDespriction, sizeof(StepDespriction), "TransferData(0x36) %dth segmentblock", i + 1);
        TestReport_HTML_StepDescription(StepDespriction);
        app.wait(20,"");
        SID37_RequestTransferExit();
        CheckPosResponse_SID37();
    }
    CheckFlag = false;
    return 0;
}


s32 FileDownload_with_decreased_length()
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};
    char StepDespriction[256];
    char filename[300] = {};
    filename[0] = '\0';
    if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
        log("Error: Failed to get filename for File");
        snprintf(StepDespriction, sizeof(StepDespriction), "Download File (no filename)");
    }
    else {
        // 安全截断文件名
        size_t len = strlen(filename);
        if (len >= 9) {
            filename[len - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s", filename);
        }
        else {
            log("Warning: Filename '%s' too short (len=%zu)", filename, len);
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File (short filename)");
        }
    }
    TestReport_HTML_StepDescription(StepDespriction);
    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        sequenceNum = 1;

        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
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

s32 FileDownload_with_VBT_Hash(u8 secondByte = 0, u8 endByte = 0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};

    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        sequenceNum = 1;

        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
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

        for (u64 offset = 0; offset < remainLength; offset += MaxDataLength)
        {
            u64 currentSequenceSize = (remainLength - offset >= MaxDataLength) ?
                MaxDataLength : remainLength - offset;

            for (u64 j = 0; j < currentSequenceSize; j++)
            {
                u64 globalIndex = offset + j;

                if (i == 1)
                {
                    if (secondByte == 1 && globalIndex == 1)
                    {
                        transferData[j] = ~VbfFileInfo.VbfDataBlcok[i].BlockData[globalIndex];
                    }
                    else if (endByte == 1 && globalIndex == VbfFileInfo.VbfDataBlcok[i].DataLength - 1)
                    {
                        transferData[j] = ~VbfFileInfo.VbfDataBlcok[i].BlockData[globalIndex];
                    }
                    else
                    {
                        transferData[j] = VbfFileInfo.VbfDataBlcok[i].BlockData[globalIndex];
                    }
                }
                else
                {
                    transferData[j] = VbfFileInfo.VbfDataBlcok[i].BlockData[globalIndex];
                }
            }

            if (remainLength - offset >= MaxDataLength)
            {
                SID36_Transfer_Data(sequenceNum, transferData, MaxDataLength);
                //CheckPosResponse_SID36(sequenceNum);
                if (0 != CheckPosResponse_SID36(sequenceNum))
                    return -1;
            }

            else
            {
                SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                //CheckPosResponse_SID36(sequenceNum);
                if (0 != CheckPosResponse_SID36(sequenceNum))
                    return -1;
            }
            sequenceNum++;
        }
        SID37_RequestTransferExit();
        CheckPosResponse_SID37();
    }

    return 0;
}

s32 FileDownload_with_Change_Data(u8 secondByte = 0, u8 endByte = 0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};

    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        sequenceNum = 1;

        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
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
            if (secondByte == 1 && sequenceNum == 1 && i == 1) {
                transferData[2] = ~transferData[2];
            }

            if (remainLength - offset >= MaxDataLength)
            {
                SID36_Transfer_Data(sequenceNum, transferData, MaxDataLength);
                //CheckPosResponse_SID36(sequenceNum);
                if (0 != CheckPosResponse_SID36(sequenceNum))
                    return -1;
            }

            else
            {
                SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                //CheckPosResponse_SID36(sequenceNum);
                if (0 != CheckPosResponse_SID36(sequenceNum))
                    return -1;
            }
            sequenceNum++;
        }

        SID37_RequestTransferExit();
        CheckPosResponse_SID37();
    }

    return 0;
}

s32 FileDownload_0x34(u8 dataBlock = 0,u8 posResponse = 1,pu8 NRC = NULL,u8 lengthNRC = 0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};
    char StepDespriction[256];
    

    subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
    log("Parse is %s, Format is 0x%02X", VbfFileInfo.data_format_identifier, subfunc[0]);
    subfunc[1] = 0x44;
    subfunc[2] = VbfFileInfo.VbfDataBlcok[dataBlock].StartAddress >> 24;
    subfunc[3] = VbfFileInfo.VbfDataBlcok[dataBlock].StartAddress >> 16;
    subfunc[4] = VbfFileInfo.VbfDataBlcok[dataBlock].StartAddress >> 8;
    subfunc[5] = VbfFileInfo.VbfDataBlcok[dataBlock].StartAddress;
    subfunc[6] = VbfFileInfo.VbfDataBlcok[dataBlock].DataLength >> 24;
    subfunc[7] = VbfFileInfo.VbfDataBlcok[dataBlock].DataLength >> 16;
    subfunc[8] = VbfFileInfo.VbfDataBlcok[dataBlock].DataLength >> 8;
    subfunc[9] = VbfFileInfo.VbfDataBlcok[dataBlock].DataLength;
    SID34_Request_Download(subfunc, 10);
    if (posResponse == 1) {
        if (0 == Raw_CheckPosResponse_SID34(subfunc)) {
            TestReport_StepInfo(Pass);
            return 0;
        }
    }
    if (lengthNRC > 0) {
        if (0 == Raw_CheckNegResponse(NRC, lengthNRC)) {
            TestReport_StepInfo(Pass);
            return 0;
        }
        else {
            TestReport_StepInfo(Fail);
            return -1;
        }
    }
    if (posResponse == 1) {
        if (0 != Raw_CheckPosResponse_SID34(subfunc)) {
            TestReport_StepInfo(Fail);
            return -1;
        }
    }
    //if (TpRxData[0] == 0x7F && lengthNRC != 0) {
    //    
    //    u8 i = 0;
    //    for (i = 0; i < lengthNRC; i++) {
    //        if (TpRxData[2] == NRC[i]) {
    //            break;
    //        }
    //    }
    //    if (i == lengthNRC) {
    //        return -1;
    //    }
    //    else {
    //        return 0;
    //    }
    //}

    return -1;
}

s32 FileDownload_0x36(u8 blockNumber = 1)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};
    char StepDespriction[256];
    u8 dataBlockNumber = 0;
    if (blockNumber == 1)
        dataBlockNumber = 1;
    else if (blockNumber == 0)
        dataBlockNumber = VbfFileInfo.DataBlockNumber;

    for (int i = 0; i < dataBlockNumber; i++)
    {
        if (dataBlockNumber > 1) {
            FileDownload_0x34(i);
        }
        sequenceNum = 1;
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
                if (0 != CheckPosResponse_SID36(sequenceNum))
                    return -1;
            }

            else
            {
                SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                if (0 != CheckPosResponse_SID36(sequenceNum))
                    return -1;
            }
            sequenceNum++;
        }
        snprintf(StepDespriction, sizeof(StepDespriction), "TransferData(0x36) %dth segmentblock", i + 1);
        TestReport_HTML_StepDescription(StepDespriction);
    }

    return 0;
}

s32 FileDownload_0x37(u8 poResponse = 1,pu8 NRC = NULL,u8 lengthNRC = 0) {
    SID37_RequestTransferExit();
    if (poResponse == 1) {
        if (0 == CheckPosResponse_SID37())
            return 0;
    }
    if (poResponse == 0) {
        return CheckNegResponse(NRC,lengthNRC);
    }
    else if (TpRxData[0] == 0x7F && lengthNRC != 0) {
        u8 i = 0;
        for (i = 0; i < lengthNRC; i++) {
            if (TpRxData[2] == NRC[i]) {
                break;
            }
        }
        if (i >= lengthNRC) {
            TestReport_HTML_StepDescription("No Expect NRC",Fail);
            return -1;
        }
        else {
            TestReport_HTML_StepDescription("Response Expect NRC", Pass);
            return 0;
        }
    }
    return -1;
}

s32 FileDownload_with_checkccr(u8 InCorrect = 0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};
    char tempPrint[100] = {};
    
    if (InCorrect == 0) {
        TestReport_PrintFileName("Download Correct");
        for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
        {
            sequenceNum = 1;

            //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
            subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
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
                snprintf(tempPrint, 100, "Download 36 %02X", sequenceNum);
                TestReport_HTML_StepDescription(tempPrint);
                if (remainLength - offset >= MaxDataLength)
                {
                    SID36_Transfer_Data(sequenceNum, transferData, MaxDataLength);
                    //CheckPosResponse_SID36(sequenceNum);
                    if (0 != CheckPosResponse_SID36(sequenceNum))
                        return -36;
                }

                else
                {
                    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                    //CheckPosResponse_SID36(sequenceNum);
                    if (0 != CheckPosResponse_SID36(sequenceNum))
                        return -36;
                }
                sequenceNum++;

                CheckCCR();
            }

            SID37_RequestTransferExit();
            CheckPosResponse_SID37();
        }
    }
    else if (InCorrect == 1) {
        TestReport_PrintFileName("Downlaod InCorrect");
        for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
        {
            sequenceNum = 1;

            //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
            subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
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
                    transferData[j] = ~VbfFileInfo.VbfDataBlcok[i].BlockData[j + offset];
                }
                snprintf(tempPrint, 100, "Download 36 %02X", sequenceNum);
                TestReport_HTML_StepDescription(tempPrint);
                if (remainLength - offset >= MaxDataLength)
                {
                    SID36_Transfer_Data(sequenceNum, transferData, MaxDataLength);
                    //CheckPosResponse_SID36(sequenceNum);
                    if (0 != CheckPosResponse_SID36(sequenceNum))
                        return -36;
                }

                else
                {
                    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                    //CheckPosResponse_SID36(sequenceNum);
                    if (0 != CheckPosResponse_SID36(sequenceNum))
                        return -36;
                }
                sequenceNum++;
                CheckCCR();
            }

            SID37_RequestTransferExit();
            CheckPosResponse_SID37();
        }
    }

    return 0;
}

s32 FileDownload_RequestSoftwareAndUpdateProcessStatus(u8 Dupli36 = 0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};
    char StepDespriction[256];
    bool requestFlag = true;
    u8 DID1[2] = { 0xD0,0x84 };
    u8 DID2[2] = { 0xD0,0x86 };
    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        sequenceNum = 1;

        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
        log("Parse is %s, Format is 0x%02X", VbfFileInfo.data_format_identifier, subfunc[0]);
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
                if (requestFlag) {
                    SID22_ReadDataByIdentifier(DID1);
                    CheckPosResponse_SID22(DID1);
                    SID22_ReadDataByIdentifier(DID2);
                    CheckPosResponse_SID22(DID2);
                    requestFlag = false;
                }
            }

            else
            {


                if (Dupli36 == 1)
                {
                    //CANoe报告中带有下列步骤，但是log中没有进行，所以此处直接将每个block最后一个包发送两次
                    //if (remainLength - offset == 1)
                    //{
                    //    SID36_Transfer_Data(sequenceNum, transferData, 0);
                    //}
                    //else if(remainLength - offset <=7)
                    //{
                    //    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset - 1);
                    //}
                    //else if ((remainLength - offset - 6) % 7 == 0)
                    //{
                    //    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset - 7);
                    //}
                    //else
                    //{
                    //    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset - (remainLength - offset - 6) % 7);
                    //}
                    //CheckNonResponse();
                    TestReport_HTML_StepDescription("block中最后一个包，第一次发送");
                    SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                    CheckPosResponse_SID36(sequenceNum);
                    TestReport_HTML_StepDescription("block中最后一个包，第二次发送");
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
        snprintf(StepDespriction, sizeof(StepDespriction), "TransferData(0x36) %dth segmentblock", i + 1);
        TestReport_HTML_StepDescription(StepDespriction);

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
        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
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
                //修改为最后一个分片不传输
                if ((AbortAt == 2) && (i == VbfFileInfo.DataBlockNumber - 1))
                    return -1;
                for (int j = 0; j < remainLength - offset; j++)
                {
                    transferData[j + 2] = VbfFileInfo.VbfDataBlcok[i].BlockData[j + offset];
                }

                SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                CheckPosResponse_SID36(sequenceNum);

                //原本为第一个文件的最后一个块刷写，然后返回
                //if (AbortAt == 2)
                //    return -1;
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

s32 FileDownload_AbortPower(u8 AbortAt = 0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;


    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
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
                //修改为最后一个分片不传输
                if ((AbortAt == 2) && (i == VbfFileInfo.DataBlockNumber - 1))
                    return -1;
                for (int j = 0; j < remainLength - offset; j++)
                {
                    transferData[j + 2] = VbfFileInfo.VbfDataBlcok[i].BlockData[j + offset];
                }

                SID36_Transfer_Data(sequenceNum, transferData, remainLength - offset);
                CheckPosResponse_SID36(sequenceNum);

                //原本为第一个文件的最后一个块刷写，然后返回
                //if (AbortAt == 2)
                //    return -1;
            }

            if (AbortAt == 1 && sequenceNum == 1) {
                KL15(off);
                KL30(off);
                TestWait(5000);
                KL30(on);
                KL15(on);
                return -1;
            }

            sequenceNum++;
        }

        SID37_RequestTransferExit();
        CheckPosResponse_SID37();
    }

    return 0;
}

//传输第一个块后校验数字签名
s32 FileDownload_FirstBlockCheckSign(u8 type = 0,pu8 lastECUCoreAssemblyNumber = NULL, pu8 newECUCoreAssemblyNumber = NULL,u8 length = 8) {
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};
    char StepDespriction[256];

    for (int i = 0; i < VbfFileInfo.DataBlockNumber; i++)
    {
        if (0 == i) {
            TestStep("downloade first Block");
        }
        sequenceNum = 1;

        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
        log("Parse is %s, Format is 0x%02X", VbfFileInfo.data_format_identifier, subfunc[0]);
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
        }
        snprintf(StepDespriction, sizeof(StepDespriction), "TransferData(0x36) %dth segmentblock", i + 1);
        TestReport_HTML_StepDescription(StepDespriction);

        SID37_RequestTransferExit();
        CheckPosResponse_SID37();

        if (0 == i) {
            u16 FileSignature = 0x0212;
            u8 ExpectedResponse[2] = { 0x10,0x06 };

            SID31_RoutineControl(0x01, FileSignature, VbfFileInfo.sw_signature_dev.count, VbfFileInfo.sw_signature_dev.array);
            CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse);

            if (type == 1) {
                CheckCCR();
                u8 F1AA[2] = { 0xF1,0xAA };

                TestStep("Read ECU Core Assembly Number");
                SID22_ReadDataByIdentifier(F1AA);
                CheckPosResponse_SID22(F1AA, length);
                StoreArray(newECUCoreAssemblyNumber, TpRxData, length, 0, 3);
                if (1 == CheckArrayDifferent(lastECUCoreAssemblyNumber, newECUCoreAssemblyNumber, length)) {
                    TestReport_HTML_StepDescription("Verify that HH is same as in step 6 and that Length of HH = 8", Pass);
                }
                else {
                    TestReport_HTML_StepDescription("Verify that HH is not same as in step 6", Fail);
                }
            }

        }

    }
}


s32 ActivateSBL()
{
    u16 ActivateSBL =0x0301 ;
    u8 SBLAddress[4] = {};
    u8 ExpectedResponse[1] = {0x10};

    char* pEnd;
    u32 CallAddress = strtol(VbfFileInfo.call, &pEnd, 16);
    SBLAddress[0] = CallAddress >> 24;
    SBLAddress[1] = CallAddress >> 16;
    SBLAddress[2] = CallAddress >> 8;
    SBLAddress[3] = CallAddress;

    SID31_RoutineControl(0x01, ActivateSBL, 4, SBLAddress);
    
    return CheckPosResponse_SID31(0x01, ActivateSBL, 1, ExpectedResponse);
}

s32 CheckSignature()
{
    u16 FileSignature = 0x0212;
    
    u8 ExpectedResponse[2] = { 0x10,0x00 };

    SID31_RoutineControl(0x01, FileSignature, VbfFileInfo.sw_signature_dev.count, VbfFileInfo.sw_signature_dev.array);
    CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse);

    return 0;
}

s32 CheckSignatureProduction()
{
    u16 FileSignature = 0x0212;

    u8 ExpectedResponse[2] = { 0x10,0x00 };

    SID31_RoutineControl(0x01, FileSignature, VbfFileInfo.sw_signature.count, VbfFileInfo.sw_signature.array);
    CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse);

    return 0;
}

s32 CheckSignature_SequenceError(int IsSupportNRC = 0)
{
    u16 FileSignature = 0x0212;

    u8 ExpectedResponse[2] = { 0x10,0x06 };
    u8 NRC[2] = { 0x22,0x31 };
    SID31_RoutineControl(0x01, FileSignature, VbfFileInfo.sw_signature_dev.count, VbfFileInfo.sw_signature_dev.array);
    if (IsSupportNRC == 0)
    {
        CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse);
    }
    else
    {
        if (0 == Raw_CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse))
        {
            TestReport_StepInfo(Pass);
            return 0;
        }
        else if (0 == Raw_CheckNegResponse(NRC))
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

s32 CheckSignatureBlank()
{
    u16 FileSignature = 0x0212;

    u8 ExpectedResponse[2] = { 0x10,0x01 };

    u8 invalidSig[512] = {};

    for (int i = 0; i < VbfFileInfo.sw_signature_dev.count; i++)
    {
        invalidSig[i] = VbfFileInfo.sw_signature_dev.array[i] + 1;
    }

    SID31_RoutineControl(0x01, FileSignature, 0, invalidSig);
    //CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse);
    CheckNegResponse(0x13);
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

s32 CheckInstallationCondition()
{
    u16 InstallationCondition = 0x0250;
    u8 ExpectedResponse[2] = { 0x10,0x00};

    SID31_RoutineControl(0x01, InstallationCondition);
    if (0 == CheckPosResponse_SID31(0x01, InstallationCondition, 2, ExpectedResponse))
    {
        return 0;
    }
    else
        return -1;
}

s32 CheckCancelInstallationCondition()
{
    u16 InstallationCondition = 0x024D;
    u8 ExpectedResponse[2] = { 0x20,0x00 };

    SID31_RoutineControl(0x01, InstallationCondition);
    if (0 == CheckPosResponse_SID31(0x01, InstallationCondition, 2, ExpectedResponse))
    {
        return 0;
    }
    else
        return -1;
}
s32 CheckCancelInstallationCondition(u8 length,pu8 recordData)
{
    u16 InstallationCondition = 0x024D;
    //u8 ExpectedResponse[2] = { 0x20,0x00 };

    SID31_RoutineControl(0x01, InstallationCondition);
    if (0 == CheckPosResponse_SID31(0x01, InstallationCondition, length, recordData))
    {
        return 0;
    }
    else
        return -1;
}

s32 CheckSoftwareSynchronization(u8 status = 0x01)
{
    u16 SoftwareSynchronization = 0x0255;
    u8 Record[1] = { 0x01 };
    u8 ExpectedResponse[1] = { 0x10};
    Record[0] = status;

    SID31_RoutineControl(0x01, SoftwareSynchronization,1,Record);
    if (0 == CheckPosResponse_SID31(0x01, SoftwareSynchronization, 1, ExpectedResponse))
    {
        return 0;
    }
    else
        return -1;
}

s32 CheckFlagActivate()
{
    u16 FlagActivate = 0x0253;
    u8 Record[1] = { 0x01 };
    u8 ExpectedResponse[1] = { 0x10 };

    SID31_RoutineControl(0x01, FlagActivate, 1, Record);
    if (0 == CheckPosResponse_SID31(0x01, FlagActivate, 1, ExpectedResponse))
    {
        return 0;
    }
    else
        return -1;
}

s32 ReadWriteDID(u16 rDID, u32 Length,u8 CheckNRC = 0)
{
    u8 DID[2] = {};
    DID[0] = rDID >> 8;
    DID[1] = rDID & 0xff;

    u8* WriteData = (u8*)malloc(Length + 3);
    u8 nrc[2] = { 0x31,0x11 };

    WriteData[0] = DID[0]; WriteData[1] = DID[1];
    if (CheckNRC == 0) {
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
    }
    else {
        SID22_ReadDataByIdentifier(DID);
        if (0 == CheckNegResponse(nrc))
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
    }
    if (CheckNegResponse(nrc) == 0)
        return 0;
    else
        return -1;
}


s32 ReadWriteDID_Func(u16 rDID, u32 Length, s8 CheckNegResp = 0)
{
    u8 DID[2] = {};
    DID[0] = rDID >> 8;
    DID[1] = rDID & 0xff;

    u8* WriteData = (u8*)malloc(Length + 3);
    u8 nrc[5] = { 0x31,0x11,0x12,0x7E,0x7F};

    WriteData[0] = DID[0]; WriteData[1] = DID[1];
    if (CheckNegResp == 0) {
        SID22_ReadDataByIdentifier(DID,1);
        if (0 == CheckPosResponse_SID22(DID, Length))
        {
            for (int i = 0; i < Length; i++)
            {
                WriteData[i + 2] = TpRxData[i + 3];
            }

            SID2E_WriteDataById(WriteData, Length + 2, 1);
        }
        else
        {
            for (int i = 0; i < 8; i++)
            {
                WriteData[i + 2] = i + 1;
            }

            SID2E_WriteDataById(WriteData, Length + 2, 1);
        }
    }
    else if(CheckNegResp == 1){
        SID22_ReadDataByIdentifier(DID, 1);
        if (0 == CheckNegResponse(nrc))
        {
            for (int i = 0; i < Length; i++)
            {
                WriteData[i + 2] = TpRxData[i + 3];
            }

            SID2E_WriteDataById(WriteData, Length + 2, 1);
        }
        else
        {
            for (int i = 0; i < 8; i++)
            {
                WriteData[i + 2] = i + 1;
            }

            SID2E_WriteDataById(WriteData, Length + 2, 1);
        }
    }
    else if (CheckNegResp == -1) {
        SID22_ReadDataByIdentifier(DID, 1);
        if (0 == CheckNonResponse())
        {
            for (int i = 0; i < Length; i++)
            {
                WriteData[i + 2] = TpRxData[i + 3];
            }

            SID2E_WriteDataById(WriteData, Length + 2, 1);
        }
        else
        {
            for (int i = 0; i < 8; i++)
            {
                WriteData[i + 2] = i + 1;
            }

            SID2E_WriteDataById(WriteData, Length + 2, 1);
        }
    }
    if (0 == CheckNonResponse())
        return 0;
    else
        return -1;
}

s32 CheckProgrammingPrecondition(u8 AddressMode = 0)
{
    u16 ProPrecondition= 0x0206 ;
    u8 ExpectedResponse[2] = { 0x10,01 };

    SID31_RoutineControl(0x01, ProPrecondition,0, NULL,AddressMode);
    CheckPosResponse_SID31(0x01, ProPrecondition, 2, ExpectedResponse);
    return 0;
}

s32 CheckUpdateProgrammingPrecondition(u8 AddressMode = 0)
{
    u16 ProPrecondition = 0x0254;
    u8 ExpectedResponse[2] = { 0x10,01 };

    SID31_RoutineControl(0x01, ProPrecondition, 0, NULL, AddressMode);
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
        TestReport_HTML_StepDescription("ECU未写入公钥！");
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
//    //    printf("%X", ChallengeBytes[i]);
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

    for(int i =0;i<16;i++)
    {
        printf("%X", KeyArray[i]);
    }

    log("111111111111111111111");

    GenKey(ChallengeBytes1, 16, 0x01, "", "", KeyArray, 16, ActualSize);

    for (int i = 0; i < 16; i++)
    {
        printf("%X", KeyArray[i]);
    }

    log("2222222222222222222222");

    GenKey(ChallengeBytes2, 16, 0x01, "", "", KeyArray, 16, ActualSize);

    for (int i = 0; i < 16; i++)
    {
        printf("%X", KeyArray[i]);
    }

    log("333333333333333333333333");
    GenKey(ChallengeBytes3, 16, 0x01, "", "", KeyArray, 16, ActualSize);

    for (int i = 0; i < 16; i++)
    {
        printf("%X", KeyArray[i]);
    }

    FreeLibrary(DLLhandle);
}


s32 SecurityUnlock(u8 SecurityLevel)
{
    app.wait(20, "");
    SID27_SecurityAccess_RequestSeed(SecurityLevel);
    CheckPosResponse_SID27_RequestSeed(SecurityLevel);
    if (SeedArray[0] == 0 && SeedArray[1] == 0 && SeedArray[2] == 0)
        TestReport_HTML_StepDescription("Test fail,seed is all zero!", Fail);

    GenSAKeyExOpt(SecurityLevel);

    SID27_SecurityAccess_SendKey(SecurityLevel);
    return CheckPosResponse_SID27_SendKey(SecurityLevel);
    //return 0;
}

s32 SecurityUnlock_Invalid(u8 SecurityLevel)
{
    app.wait(20, "");
    SID27_SecurityAccess_RequestSeed(SecurityLevel);
    CheckPosResponse_SID27_RequestSeed(SecurityLevel);
    if (SeedArray[0] == 0 && SeedArray[1] == 0 && SeedArray[2] == 0)
        TestReport_HTML_StepDescription("Test fail,seed is all zero!", Fail);

    GenSAKeyExOpt(SecurityLevel);
    KeyArray[15]++;

    SID27_SecurityAccess_SendKey(SecurityLevel);
    return CheckNegResponse(0x35);
    //return 0;
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
    TestReport_HTML_StepDescription("Download SBL");
    char keyvalue[20] = "SBL_Path";
    vbfparser(keyvalue);
    //此处新加，地址擦除，原代码没写。
    //SBL不需要擦除，注释EraseAddress();  2025/4/16 
    //EraseAddress();
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
    //GetIniFilePath(iniptr);
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
        printf("%X", pincode0x01[i-1]);
    }

    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x03");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        /*printf("%s", str);*/
        pincode0x03[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x03[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x05");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x05[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x05[i-1]);
    }

    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x07");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x07[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x07[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x09");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x09[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x09[i-1]);
    }

    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x0B");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x0B[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x0B[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x0D");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x0D[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x0D[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x0F");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x0F[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x0F[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x11");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x11[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x11[i-1]);
    }

    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x13");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x13[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x13[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x15");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x15[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x15[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x17");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x17[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x17[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x19");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x19[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x19[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x1B");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x1B[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x1B[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x1D");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x1D[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x1D[i-1]);
    }
    strcpy(title, "SecurityConstant");
    strcpy(key, "SL0x1F");

    GetINIKeyString(title, key, iniptr, Buffer);

    for (int i = 1; i <= 16; i++) {
        strncpy(str, Buffer + 2 * i, 2);
        //printf("%s", str);
        pincode0x1F[i-1] = strtol(str, &strptr, 16);
        printf("%X", pincode0x1F[i-1]);
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
    printf("resp id = %X", TScanfdResp.FIdentifier);
    TScanfdReq.FIdentifier = TScanfdResp.FIdentifier + 0x100;

    TScanResp.FIdentifier = ECU_Address & 0xfff;
    printf("resp id = %X", TScanResp.FIdentifier);
    TScanReq.FIdentifier = TScanResp.FIdentifier + 0x100;

    appindex = 0;
}


void GetDatabaseType() {
    int DatabaseType = 0;
    if (0 == app.get_system_var_int32("DatabaseType", &DatabaseType)) {
        g_CurrentDBType = (DatabaseType == 0) ? DB_TYPE_DBC : DB_TYPE_ARXML;
        log("Get DatabaseType Successful: is %d", g_CurrentDBType);
    }
    else {
        g_CurrentDBType = DB_TYPE_DBC;
        log("Get DatabaseType Failed: is %d", g_CurrentDBType);
    }
    
}


//std::map<std::string, int> NetworkCluster;

void Pretest()
{
    GetDatabaseType();
    printf("current mp version is 2026.5.30-2:01");
    //app.disconnect();
    //app.mplib_stop("GEEA_CAN_DIAG");
    excelParser();

    
    app.set_system_var_int32("MPLib.GEEA_CAN_DIAG", 1);
    //int ret = app.kill_form("同星工具箱设计开发环境");
    //printf("%d", ret);
    REG();
    //TestReport_Init();
    TestReport_HTML_Init();
    //TestReport_Cover();

    //handle ini
    //SetParameters();

    //test_report.test_openissue(HTMLHandle, "序号,问题分类,测试案例ID,标题,提出时间,问题状态,问题描述,问题严重度,\
//平台,ECU名称,软件版本号,供应商,测试环境,测试部门,附件,责任方,计划完成时间,对策/答案,实际完成时间,测试阶段,问题关闭阶段,测试人员,审核人员,备注");
    
    char keyvalue[100] = "SBL";
    vbfparser(keyvalue);
    int ECUMask;
    ECUMask = (int)strtol(VbfFileInfo.ecu_address, NULL, 16);
    Diag_Request_ID = ECUMask & 0xFF | 0x700;
    Diag_Response_ID = ECUMask & 0xFF | 0x600;
    Diag_Function_ID = 0x7FF;
    if (Diag_Request_ID == 0x701) {
        gCEMFlg = 1;
        strncpy(DUTName,"ZCUD",5);
    }
    else {
        gCEMFlg = 0;
        strncpy(DUTName, "ZCUP", 5);
    }

    if (gCEMFlg) {
        NetworkCluster["ZCU_CANFD2"] = CH2;
        NetworkCluster["ZCU_CANFD1"] = CH3;
        NetworkCluster["ChassisCANFD2"] = CH4;
        NetworkCluster["ZCUD_CAN1"] = CH5;
        NetworkCluster["PropulsionCANFD"] = CH6;
        NetworkCluster["ChassisCANFD1"] = CH7;
        NetworkCluster["ZCUD_CANFD3"] = CH8;
        NetworkCluster["ZCUD_CANFD4"] = CH9;
        NetworkCluster["BodyExposedCAN"] = CH10;
        NetworkCluster_CH[CH2] = "ZCU_CANFD2";
        NetworkCluster_CH[CH3] = "ZCU_CANFD1";
        NetworkCluster_CH[CH4] = "ChassisCANFD2";
        NetworkCluster_CH[CH5] = "ZCUD_CAN1";
        NetworkCluster_CH[CH6] = "PropulsionCANFD";
        NetworkCluster_CH[CH7] = "ChassisCANFD1";
        NetworkCluster_CH[CH8] = "ZCUD_CANFD3";
        NetworkCluster_CH[CH9] = "ZCUD_CANFD4";
        NetworkCluster_CH[CH10] = "BodyExposedCAN";
    }
    else {
        NetworkCluster["ZCU_CANFD2"] = CH2;
        NetworkCluster["ZCU_CANFD1"] = CH3;
        NetworkCluster["ZCUP_CANFD5"] = CH4;
        NetworkCluster["ZCUP_CAN1"] = CH5;
        NetworkCluster_CH[CH2] = "ZCU_CANFD2";
        NetworkCluster_CH[CH3] = "ZCU_CANFD1";
        NetworkCluster_CH[CH4] = "ZCUP_CANFD5";
        NetworkCluster_CH[CH5] = "ZCUP_CAN1";
    }

    Diag_ch = NetworkCluster[TpBusName_Temp];

    pincode0x03[0] = 0x5E;
    pincode0x03[1] = 0x40;
    pincode0x03[2] = 0xDC;
    pincode0x03[3] = 0x91;
    pincode0x03[4] = 0xEC;
    pincode0x03[5] = 0xA1;
    pincode0x03[6] = 0x44;
    pincode0x03[7] = 0xEC;
    pincode0x03[8] = 0x66;
    pincode0x03[9] = 0x3F;
    pincode0x03[10] = 0xCC;
    pincode0x03[11] = 0x2E;
    pincode0x03[12] = 0x6E;
    pincode0x03[13] = 0xD3;
    pincode0x03[14] = 0x46;
    pincode0x03[15] = 0xD8;
    if (Diag_Request_ID == 0x701) {
        pincode0x05[0] = 0x10;
        pincode0x05[1] = 0x54;
        pincode0x05[2] = 0xA8;
        pincode0x05[3] = 0x09;
        pincode0x05[4] = 0x8E;
        pincode0x05[5] = 0xA0;
        pincode0x05[6] = 0x8C;
        pincode0x05[7] = 0x6F;
        pincode0x05[8] = 0x49;
        pincode0x05[9] = 0x25;
        pincode0x05[10] = 0x78;
        pincode0x05[11] = 0x38;
        pincode0x05[12] = 0x2A;
        pincode0x05[13] = 0x11;
        pincode0x05[14] = 0x84;
        pincode0x05[15] = 0x9B;
    }
    else {
        pincode0x05[0] = 0x57;
        pincode0x05[1] = 0xEA;
        pincode0x05[2] = 0xFE;
        pincode0x05[3] = 0xD7;
        pincode0x05[4] = 0x93;
        pincode0x05[5] = 0xDE;
        pincode0x05[6] = 0x70;
        pincode0x05[7] = 0xA8;
        pincode0x05[8] = 0x87;
        pincode0x05[9] = 0xF3;
        pincode0x05[10] = 0xB8;
        pincode0x05[11] = 0x09;
        pincode0x05[12] = 0x1A;
        pincode0x05[13] = 0xDD;
        pincode0x05[14] = 0xFA;
        pincode0x05[15] = 0x75;
    }
    int TestType = 0;
    if (0 != app.get_system_var_int32("TestType", &TestType)) {
        TestType = 1;
    }
    Test_Result = TestOK;
    stepnumber = 0;
    char* s = nullptr;
    char tempPath[500] = { 0 };
    char* pythonResult = nullptr;
    app.get_configuration_file_path(&s);
    snprintf(tempPath, sizeof(tempPath), "%sToolbox\\NM\\common\\json_to_ini_auto.py", s);

    if (gCEMFlg == 1) {
        app.execute_python_script(tempPath, "ZCUD", true, false, &pythonResult);
        sprintf(ini_path, "%sToolbox\\NM\\common\\config_ZCUD.ini", s);// C:\Users\shanqingqing\Desktop\NM_3.0\NM_TEST\common\config.ini
    }
    else {
        app.execute_python_script(tempPath, "ZCUP", true, false, &pythonResult);
        sprintf(ini_path, "%sToolbox\\NM\\common\\config_ZCUP.ini", s);// C:\Users\shanqingqing\Desktop\NM_3.0\NM_TEST\common\config.ini
    }
    char NM_INI_Path[500] = { 0 };
    sprintf(NM_INI_Path, "%sToolbox\\GEELY3\\DB\\PncInfo.ini", s);
    printf("PncInfo.ini path=%s", NM_INI_Path);
    

    if (1 == TestType) {
        //解析SDDB文件
        SDDB_Parser();
        ParserArxml();
        //获取工程项目路径
        app.get_configuration_file_path(&ABSPath);
        std::string E2E_INIPATH = std::string(ABSPath) + E2E_ini_path;
        std::string INIPATH = std::string(ABSPath) + Parameter_inipath;
        GetParameter(E2E_INIPATH);

        ////获取E2E相关信息，解析arxml，生成/解析ini文件
        E2EInfoGet();
    }

    handle_ini(ini_path);
    Read_NM_INI(NM_INI_Path);

    app.get_system_var_int32("SecurityUnlockCycle",&SecurityUnlockCycle);
    printf("SecurityUnlockCycle is %d", SecurityUnlockCycle);
    app.get_system_var_int32("SecurityUnlockTimes", &SecurityUnlockTimes);
    printf("SecurityUnlockTimes is %d", SecurityUnlockTimes);
    //SecurityUnlockCycle = 1;
    //SecurityUnlockTimes = 1;
    BusTypeNow = CANFD;
    DevicesCreat();
    AppWait(500, "");
    gSleepTime = 300000;
    //SetBoardCardInit_Devices();

    FindTRCPath();
   
    //获取测试参数
    
    getCarConfigInfo();
}

void TestFinish()
{
    printf("Start Test Finish");
    //TestPostCondition();
    DevicesInit();

    //BoardCardInit_Devices();

    //TestReport_End();
    UNREG();
    TestReport_HTML_Final();
    s32 v_TestReport_IsGlobalReportEnabled = 0;
    if (0 == app.get_system_var_int32("TestReport_IsGlobalReportEnabled", &v_TestReport_IsGlobalReportEnabled)) {
        if (v_TestReport_IsGlobalReportEnabled == 0) {
            log("get TestReport_IsGlobalReportEnabled is %d", v_TestReport_IsGlobalReportEnabled);
            app.disconnect();
        }
    }
    else {
        log("get TestReport_IsGlobalReportEnabled is %d", v_TestReport_IsGlobalReportEnabled);
    }
    
    printf("Test Finish End");
    app.set_system_var_int32("MPLib.GEEA_CAN_DIAG", 0);
    //app.mplib_stop("GEEA_CAN_DIAG");
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

void TestPrecondition()
{
    strcpy(issuetype, "Diagnostic Protocal");
    Test_Result = TestOK;
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

    PrintCaseNameAndPurpose();

    TestReport_StartLogging();

    TestReport_TableInit();

    BoardCard_Init();
    MainPwr_to_NBATGND(ON);
    //DUT KL30\KL31上电
    KL30(ON);
    KL31(ON);

    SetVoltageAndRead(12, 0,0);
    MainPower_ON();

    NMWakeUp(1);

    TestWait(caseSepTime);

    SendTesterPresent(TP_ON);
    TestWait(stepseptime);

    uds_create();
    SID10_SessionControl(0x01);
    TestWait(1500);
    
    TestReport_HTML_StepDescription("PreCondition:Voltage=12V", Info);
    SetVoltage(12);
    //SimulateTRC(true);
    resultFlag = 0; issueflag = 0;

    app.get_system_var_int32("NVMCount",&NVMCount);
    
    reportOutputFlag = 1;
    StepNumber = 0;
    CheckSoftVersion();
}



void TestPostCondition()
{
    printf("Run Post Condition");
    TestReport_StopLogging();

    TestWait(stepseptime);
    SendTesterPresent(TP_OFF);
    TestWait(stepseptime);

    reportOutputFlag = 0;
    TestWait(1500);
    UNREG();
    uds_delete();
    //SimulateTRC(false);
    NMWakeUp(0);
    com.del_cyclic_messages();
    MainPower_OFF();
    BoardCard_Init();
    printf("Post Condition End");
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


//s32 CheckSoftwareVersion() {
//    u8 DID[2] = { 0xF1,0xAE };
//    int i = 0, j = 0;
//    int resflag = 0;
//
//    for (int k = 0; k < APPFileNum; k++) {
//        _sw_[k].CheckFlag = 0;
//    }
//
//    SID22_ReadDataByIdentifier(DID);
//    if (0 == CheckPosResponse_SID22(DID)) {
//        u8 swnum = TpRxData[3];
//        char temp1[20] = "", temp2[20] = "";
//
//        for(j=0;j<swnum;j++)
//        {
//            temp1[0] = '\0';
//            temp2[0] = '\0';
//            for (i = 0; i < 5; i++) {
//                char num_str[20];
//                sprintf(num_str, "%02X", TpRxData[4 + i + 8 * j]);
//                strcat(temp1, num_str);
//            }
//            temp2[0] = (char)TpRxData[4 + 8 * j + 7];
//
//            for (int k = 0; k < APPFileNum; k++) {
//                if (strcmp(temp1, _sw_[k].sw_part_number) == 0 && strcmp(temp2, _sw_[k].sw_version) == 0) {
//                    _sw_[k].CheckFlag = 1;
//                }
//            }
//        }
//
//        for (int k = 0; k < APPFileNum; k++) {
//            if (_sw_[k].CheckFlag != 1) {
//                resflag++;
//            }
//        }
//    }
//
//    if (resflag == 0) {
//        TestReport_HTML_StepDescription("读取到的软件版本号和写入的软件版本一致！", Pass);
//    }
//    else
//    {
//        TestReport_HTML_StepDescription("读取到的软件版本号和写入的软件版本不一致！", Fail);
//    }
//
//    return 0;
//}

s32 CheckSoftwareVersion() {
    u8 DID[2] = { 0xF1, 0xAE };
    int resflag = 0;
    for (int k = 0; k < APPFileNum; k++) {
        log("Expected: part=%s, version=%s", _sw_[k].sw_part_number, _sw_[k].sw_version);
    }
    // 初始化 CheckFlag
    for (int k = 0; k < APPFileNum; k++) {
        _sw_[k].CheckFlag = 0;
    }

    // 调用 UDS 服务22
    SID22_ReadDataByIdentifier(DID);
    if (0 != CheckPosResponse_SID22(DID)) {
        printf("Error: SID22 negative response or invalid for DID F1 AE");
        TestReport_HTML_StepDescription("UDS服务22读取失败！", Fail);
        return -1;
    }

    // 解析响应数据
    u8 swnum = TpRxData[3];
    if (swnum * 8 + 4 > sizeof(TpRxData)) {
        printf("Error: swnum %d too large for TpRxData", swnum);
        TestReport_HTML_StepDescription("无效的响应数据长度！", Fail);
        return -1;
    }

    char temp1[20], temp2[20];
    int unmatched_response = swnum;
    for (int j = 0; j < swnum; j++) {
        // 解析软件部件号（字节0-4）
        temp1[0] = '\0';
        char num_str[3];
        for (int i = 0; i < 5; i++) {
            sprintf(num_str, "%02X", TpRxData[4 + i + 8 * j]);
            strcat(temp1, num_str);
        }

        // 解析版本号（字节5-7）
        char version_str[4] = { 0 };
        version_str[0] = (char)TpRxData[4 + 8 * j + 5];
        version_str[1] = (char)TpRxData[4 + 8 * j + 6];
        version_str[2] = (char)TpRxData[4 + 8 * j + 7];
        version_str[3] = '\0';

        // 修剪前导空格
        char* p = version_str;
        while (*p == 0x20) p++;
        strcpy(temp2, p);
        if (temp2[0] == '\0') {
            printf("Warning: Empty version after trimming: %02X %02X %02X",
                TpRxData[4 + 8 * j + 5], TpRxData[4 + 8 * j + 6], TpRxData[4 + 8 * j + 7]);
        }

        // 比较
        int matched = 0;
        for (int k = 0; k < APPFileNum; k++) {
            if (strcmp(temp1, _sw_[k].sw_part_number) == 0 && strcmp(temp2, _sw_[k].sw_version) == 0) {
                _sw_[k].CheckFlag = 1;
                matched = 1;
                unmatched_response--;
                break;
            }
        }
        if (!matched) {
            log("Unmatched response version: part=%s, version=%s", temp1, temp2);
        }
    }

    // 检查未匹配的预期版本
    for (int k = 0; k < APPFileNum; k++) {
        if (_sw_[k].CheckFlag != 1) {
            printf("Unmatched expected version: part=%s, version=%s", _sw_[k].sw_part_number, _sw_[k].sw_version);
            resflag++;
        }
    }

    // 响应中多余的版本也算不匹配
    resflag += unmatched_response;

    // 生成报告
    if (resflag == 0) {
        TestReport_HTML_StepDescription("读取到的软件版本号和写入的软件版本一致！", Pass);
    }
    else {
        //TestReport_HTML_StepDescription("读取到的软件版本号和写入的软件版本不一致！", Fail);
        TestReport_HTML_StepDescription("读取到的软件版本号和写入的软件版本不一致！", Warning);
    }

    return (resflag == 0) ? 0 : -1;
}

s32 CheckSoftwareVersion_StoreArray(pu8 softVersionArray,pu8 length) {
    u8 DID[2] = { 0xF1, 0xAE };
    int resflag = 0;
    for (int k = 0; k < APPFileNum; k++) {
        log("Expected: part=%s, version=%s", _sw_[k].sw_part_number, _sw_[k].sw_version);
    }
    // 初始化 CheckFlag
    for (int k = 0; k < APPFileNum; k++) {
        _sw_[k].CheckFlag = 0;
    }

    // 调用 UDS 服务22
    SID22_ReadDataByIdentifier(DID);
    if (0 != CheckPosResponse_SID22(DID)) {
        printf("Error: SID22 negative response or invalid for DID F1 AE");
        TestReport_HTML_StepDescription("UDS服务22读取失败！", Fail);
        return -1;
    }
    for (u8 i = 0; i <= TpRxData[3]*8; ++i) {
        softVersionArray[i] = TpRxData[i + 3];
    }
    *length = TpRxData[3] * 8 + 1;

    // 解析响应数据
    u8 swnum = TpRxData[3];
    if (swnum * 8 + 4 > sizeof(TpRxData)) {
        printf("Error: swnum %d too large for TpRxData", swnum);
        TestReport_HTML_StepDescription("无效的响应数据长度！", Fail);
        return -1;
    }

    char temp1[20], temp2[20];
    int unmatched_response = swnum;
    for (int j = 0; j < swnum; j++) {
        // 解析软件部件号（字节0-4）
        temp1[0] = '\0';
        char num_str[3];
        for (int i = 0; i < 5; i++) {
            sprintf(num_str, "%02X", TpRxData[4 + i + 8 * j]);
            strcat(temp1, num_str);
        }

        // 解析版本号（字节5-7）
        char version_str[4] = { 0 };
        version_str[0] = (char)TpRxData[4 + 8 * j + 5];
        version_str[1] = (char)TpRxData[4 + 8 * j + 6];
        version_str[2] = (char)TpRxData[4 + 8 * j + 7];
        version_str[3] = '\0';

        // 修剪前导空格
        char* p = version_str;
        while (*p == 0x20) p++;
        strcpy(temp2, p);
        if (temp2[0] == '\0') {
            printf("Warning: Empty version after trimming: %02X %02X %02X",
                TpRxData[4 + 8 * j + 5], TpRxData[4 + 8 * j + 6], TpRxData[4 + 8 * j + 7]);
        }

        // 比较
        int matched = 0;
        for (int k = 0; k < APPFileNum; k++) {
            if (strcmp(temp1, _sw_[k].sw_part_number) == 0 && strcmp(temp2, _sw_[k].sw_version) == 0) {
                _sw_[k].CheckFlag = 1;
                matched = 1;
                unmatched_response--;
                break;
            }
        }
        if (!matched) {
            log("Unmatched response version: part=%s, version=%s", temp1, temp2);
        }
    }

    // 检查未匹配的预期版本
    for (int k = 0; k < APPFileNum; k++) {
        if (_sw_[k].CheckFlag != 1) {
            printf("Unmatched expected version: part=%s, version=%s", _sw_[k].sw_part_number, _sw_[k].sw_version);
            resflag++;
        }
    }

    // 响应中多余的版本也算不匹配
    resflag += unmatched_response;

    // 生成报告
    if (resflag == 0) {
        TestReport_HTML_StepDescription("读取到的软件版本号和写入的软件版本一致！", Pass);
    }
    else {
        TestReport_HTML_StepDescription("读取到的软件版本号和写入的软件版本不一致！", Fail);
    }

    return (resflag == 0) ? 0 : -1;
}

void TestSeq_EnterProgrammingSession(u8 AddressMode = 0) {
    TestReport_HTML_StepDescription("EnterProgramming Session");
    //if (0 != CheckCurrentSession(0x01)) {
    //    return;
    //}
    //CheckCurrentSession(0x01);
    if (0x02 == CheckCurrentSession_Warning(0x02, AddressMode)) {
        TestReport_HTML_StepDescription("Current Programming Session");
        return;
    }
    CheckProgrammingPrecondition(AddressMode);

    SID10_SessionControl(0x82, Functional);
    CheckNonResponse();
    TestWait(ECUResetTime);
    SID10_SessionControl(0x02, AddressMode);
    CheckPosResponse_SID10(0x02);
    TestWait(ECUResetTime);
    CheckCurrentSession(0x02, AddressMode);
}

void TestSeq_DownloadSBL(u8 ifactivate = 1, u8 ifDupli36 = 0, u8 Unlock = 1,u8 PublicKey = 1) {
    TestReport_HTML_StepDescription("----Download SBL----");
    char StepDespriction[256];
    //char filename[300] = "";

    char keyvalue[20] = "SBL_Path";
    if(PublicKey == 1)
        ReadPublicKey();
    if(Unlock == 1)
        SecurityUnlock(0x01);
    vbfparser(keyvalue);

    //获取文件名
    //app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
    //filename[strlen(filename) - 9] = '\0';
    snprintf(StepDespriction, sizeof(StepDespriction), "Download SBL_Filename is %s ", filename);
    TestReport_HTML_StepDescription(StepDespriction);


    FileDownload(ifDupli36);
    CheckSignature();
    if (ifactivate == 1)
        ActivateSBL();
}

void TestSeq_DownloadSBL_VBT_Hash(u8 ifactivate = 1, u8 ifDupli36 = 0, u8 Unlock = 1, u8 PublicKey = 1) {
    TestReport_HTML_StepDescription("----Download SBL----");
    char StepDespriction[256];
    char filename[300] = "";

    char keyvalue[20] = "SBL_Path";
    if (PublicKey == 1)
        ReadPublicKey();
    if (Unlock == 1)
        SecurityUnlock(0x01);
    vbfparser(keyvalue);

    //获取文件名
    app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
    filename[strlen(filename) - 9] = '\0';
    snprintf(StepDespriction, sizeof(StepDespriction), "Download SBL_Filename is %s ", filename);
    TestReport_HTML_StepDescription(StepDespriction);


    FileDownload_with_VBT_Hash(0,1);
    CheckSignature();
    if (ifactivate == 1)
        ActivateSBL();
}

void TestSeq_DownloadSBL_Abort(u8 ifactivate = 1, u8 ifDupli36 = 0, u8 Unlock = 1, u8 PublicKey = 1) {
    TestReport_HTML_StepDescription("----Download SBL----");
    char StepDespriction[256];
    char filename[300] = "";

    char keyvalue[20] = "SBL_Path";
    if (PublicKey == 1)
        ReadPublicKey();
    if (Unlock == 1)
        SecurityUnlock(0x01);
    vbfparser(keyvalue);

    //获取文件名
    app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
    filename[strlen(filename) - 9] = '\0';
    snprintf(StepDespriction, sizeof(StepDespriction), "Download SBL_Filename is %s ", filename);
    TestReport_HTML_StepDescription(StepDespriction);


    FileDownload_AbortPower(ifDupli36);
    CheckSignature();
    if (ifactivate == 1)
        ActivateSBL();
}
void TestSeq_DownloadSBL_Async(u8 ifactivate = 1, u8 ifDupli36 = 0, u8 Unlock = 1,u8 PublicKey = 1) {
    TestReport_HTML_StepDescription("----Download SBL----");
    char StepDespriction[256];
    char filename[300] = "";

    char keyvalue[20] = "SBL_Path";
    if(PublicKey == 1)
        ReadPublicKey();
    if(Unlock == 1)
        SecurityUnlock(0x01);
    vbfparser(keyvalue);

    //获取文件名
    app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
    filename[strlen(filename) - 9] = '\0';
    snprintf(StepDespriction, sizeof(StepDespriction), "Download SBL_Filename is %s ", filename);
    TestReport_HTML_StepDescription(StepDespriction);


    FileDownload_Async(ifDupli36);
    CheckSignature();
    if (ifactivate == 1)
        ActivateSBL();
}

void TestSeq_DownloadAPP(u8 ifDupli36 = 0, u8 ifAbort = 0, u8 abortat = 0)
{
    TestReport_HTML_StepDescription("----Download APP----");

    char keyvalue[20] = "APP0Path";

    for (int i = 0; i < APPFileNum; i++)
    {
        char StepDespriction[256];
        char filename[300] = {};
        vbfparser(keyvalue);
        //获取文件名
        //app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
        //filename[strlen(filename) - 9] = '\0';
        //snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s ", filename);
        //TestReport_HTML_StepDescription(StepDespriction);
        filename[0] = '\0';
        if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
            log("Error: Failed to get filename for %s", keyvalue);
            snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
        }
        else {
            // 安全截断文件名
            size_t len = strlen(filename);
            if (len >= 9) {
                filename[len - 9] = '\0';
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
            }
            else {
                log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
            }
        }
        TestReport_HTML_StepDescription(StepDespriction);
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

void TestSeq_DownloadAPP_VBT_Hash(u8 ifDupli36 = 0, u8 ifAbort = 0, u8 abortat = 0)
{
    TestReport_HTML_StepDescription("----Download APP----");

    char keyvalue[20] = "APP0Path";

    for (int i = 0; i < APPFileNum; i++)
    {
        char StepDespriction[256];
        char filename[300] = {};
        vbfparser(keyvalue);
        //获取文件名
        //app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
        //filename[strlen(filename) - 9] = '\0';
        //snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s ", filename);
        //TestReport_HTML_StepDescription(StepDespriction);
        filename[0] = '\0';
        if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
            log("Error: Failed to get filename for %s", keyvalue);
            snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
        }
        else {
            // 安全截断文件名
            size_t len = strlen(filename);
            if (len >= 9) {
                filename[len - 9] = '\0';
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
            }
            else {
                log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
            }
        }
        TestReport_HTML_StepDescription(StepDespriction);
        EraseAddress();
        if (ifAbort == 1 && abortat == i)
        {
            return;
        }
        FileDownload_with_VBT_Hash(0,1);
        u16 FileSignature = 0x0212;
        u8 ExpectedResponse[2] = { 0x10,0x04 };
        SID31_RoutineControl(0x01, FileSignature, VbfFileInfo.sw_signature_dev.count, VbfFileInfo.sw_signature_dev.array);
        CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse);
        keyvalue[3]++;
    }
}

void TestSeq_DownloadAPP_Async(u8 ifDupli36 = 0, u8 ifAbort = 0, u8 abortat = 0)
{
    TestReport_HTML_StepDescription("----Download APP----");

    char keyvalue[20] = "APP0Path";

    for (int i = 0; i < APPFileNum; i++)
    {
        char StepDespriction[256];
        char filename[300] = {};
        vbfparser(keyvalue);
        //获取文件名
        //app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
        //filename[strlen(filename) - 9] = '\0';
        //snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s ", filename);
        //TestReport_HTML_StepDescription(StepDespriction);
        filename[0] = '\0';
        if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
            log("Error: Failed to get filename for %s", keyvalue);
            snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
        }
        else {
            // 安全截断文件名
            size_t len = strlen(filename);
            if (len >= 9) {
                filename[len - 9] = '\0';
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
            }
            else {
                log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
            }
        }
        TestReport_HTML_StepDescription(StepDespriction);
        EraseAddress();
        if (ifAbort == 1 && abortat == i)
        {
            return;
        }
        FileDownload_Async(ifDupli36);
        CheckSignature();
        keyvalue[3]++;
    }
}

void TestSeq_DownloadAPP_NRC(pu8 NRC,u8 ifDupli36 = 0, u8 ifAbort = 0, u8 abortat = 0)
{
    TestReport_HTML_StepDescription("----Download APP----");

    char keyvalue[20] = "APP0Path";

    for (int i = 0; i < APPFileNum; i++)
    {
        FileDownload_NRC(keyvalue,NRC,ifDupli36);
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
        TestReport_PrintFileName("Download File ");
        EraseAddress();
        if (abortat == 1 && i == 0) {
            if (0 != FileDownload_Abort(abortat))
            {
                return;
            }
        }
        else if (abortat == 2 && i == APPFileNum - 1) {
            if (0 != FileDownload_Abort(abortat))
            {
                return;
            }
        }

        if (0 != FileDownload_Abort(abortat))
        {
            return;
        }
        CheckSignature();
        keyvalue[3]++;
    }
}

void TestSeq_FileDownload_Full_Sequence()
{
    TestReport_HTML_StepDescription("Full Sequence Download");
    TestSeq_EnterProgrammingSession();
    //CheckCurrentSession(0x01);

    //CheckProgrammingPrecondition();

    //SID10_SessionControl(0x82);
    //CheckNonResponse();
    //TestWait(ECUResetTime);

    //SID10_SessionControl(0x02);
    //CheckPosResponse_SID10(0x02);

    //CheckCurrentSession(0x02);

    u8 DID1[2] = { 0xED,0x20 };
    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    u8 DID2[2] = { 0xF1,0xAA };
    SID22_ReadDataByIdentifier(DID2);
    CheckPosResponse_SID22(DID2);

    TestSeq_DownloadSBL(0);

    u8 DID3[2] = { 0xF1,0xA2 };
    SID22_ReadDataByIdentifier(DID3);
    //CheckPosResponse_SID22(DID3);
    //u8 NRC[2] = { 0X31 };
    CheckNegResponse(0x31);

    ActivateSBL();

    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    SID22_ReadDataByIdentifier(DID3);
    CheckPosResponse_SID22(DID3, 8);

    char keyvalue[20] = "APP0Path";

    for (int i = 0; i < APPFileNum; i++)
    {
        vbfparser(keyvalue);
        TestReport_PrintFileName("Downlaod");
        EraseAddress();
        FileDownload();
        CheckSignature();
        keyvalue[3]++;
    }

    if (0 != CheckCompleteAndCompatible()) {
        strcpy(issuereason, "");
        TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
    }

    SID11_ECUReset(0x81);
    CheckNonResponse();
    TestWait(ECUResetTime);

    CheckCurrentSession(0x01);

    RX_SID19_ReportDTCByStatusMask(0x08);
    RX_CheckPosResponse_SID19_02(0xFF);

    CheckSoftwareVersion();

    CheckCurrentSession(0x01);

    PowerReset(30000);
    TestWait(ECUResetTime);

    CheckCurrentSession(0x01);

    RX_SID19_ReportDTCByStatusMask(0x09);
    RX_CheckPosResponse_SID19_02(0xFF);

    CheckSoftwareVersion();

    CheckCurrentSession(0x01);

    SID10_SessionControl(0x03);
    CheckPosResponse_SID10(0x03);

    CheckCurrentSession(0x03);

    SID10_SessionControl(0x82);
    CheckNonResponse();
    TestWait(ECUResetTime);

    SID10_SessionControl(0x02);
    CheckPosResponse_SID10(0x02);

    CheckCurrentSession(0x02);

    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    SID22_ReadDataByIdentifier(DID2);
    CheckPosResponse_SID22(DID2);

    TestSeq_DownloadSBL(0);

    SID22_ReadDataByIdentifier(DID3);
    //CheckPosResponse_SID22(DID3);
    CheckNegResponse(0x31);

    ActivateSBL();

    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    SID22_ReadDataByIdentifier(DID3);
    CheckPosResponse_SID22(DID3, 8);

    strcpy(keyvalue, "APP0Path");

    for (int i = 0; i < APPFileNum; i++)
    {
        vbfparser(keyvalue);
        TestReport_PrintFileName("Downlaod");
        EraseAddress();
        FileDownload();
        CheckSignature();
        keyvalue[3]++;
    }

    if (0 != CheckCompleteAndCompatible()) {
        strcpy(issuereason, "");
        TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
    }

    SID11_ECUReset(0x81);
    CheckNonResponse();
    TestWait(ECUResetTime);

    CheckCurrentSession(0x01);

    RX_SID19_ReportDTCByStatusMask(0x08);
    RX_CheckPosResponse_SID19_02(0xFF);

    CheckSoftwareVersion();
}

void TestSeq_FileDownload_Full_Sequence_Decompress()
{
    TestReport_HTML_StepDescription("Full Sequence Download Incompress");
    TestSeq_EnterProgrammingSession();
    //CheckCurrentSession(0x01);

    //CheckProgrammingPrecondition();

    //SID10_SessionControl(0x82);
    //CheckNonResponse();
    //TestWait(ECUResetTime);

    //SID10_SessionControl(0x02);
    //CheckPosResponse_SID10(0x02);

    //CheckCurrentSession(0x02);

    u8 DID1[2] = { 0xED,0x20 };
    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    u8 DID2[2] = { 0xF1,0xAA };
    SID22_ReadDataByIdentifier(DID2);
    CheckPosResponse_SID22(DID2);

    TestSeq_DownloadSBL(0);

    u8 DID3[2] = { 0xF1,0xA2 };
    SID22_ReadDataByIdentifier(DID3);
    //CheckPosResponse_SID22(DID3);
    //u8 NRC[2] = { 0X31 };
    //CheckNegResponse(NRC);
    CheckNegResponse(0x31);

    ActivateSBL();

    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    SID22_ReadDataByIdentifier(DID3);
    CheckPosResponse_SID22(DID3, 8);

    char keyvalue[30] = "APP0Path_Decompress";

    for (int i = 0; i < APPFileNum; i++)
    {
        vbfparser(keyvalue);
        TestReport_PrintFileName("Downlaod Decompress");
        EraseAddress();
        FileDownload();
        CheckSignature();
        keyvalue[3]++;
    }

    if (0 != CheckCompleteAndCompatible()) {
        strcpy(issuereason, "");
        TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
    }

    SID11_ECUReset(0x81);
    CheckNonResponse();
    TestWait(ECUResetTime);

    CheckCurrentSession(0x01);

    RX_SID19_ReportDTCByStatusMask(0x08);
    RX_CheckPosResponse_SID19_02(0xFF);

    CheckSoftwareVersion();

    CheckCurrentSession(0x01);

    PowerReset(30000);
    TestWait(ECUResetTime);

    CheckCurrentSession(0x01);

    RX_SID19_ReportDTCByStatusMask(0x09);
    RX_CheckPosResponse_SID19_02(0xFF);

    CheckSoftwareVersion();

    CheckCurrentSession(0x01);

    SID10_SessionControl(0x03);
    CheckPosResponse_SID10(0x03);

    CheckCurrentSession(0x03);

    SID10_SessionControl(0x82);
    CheckNonResponse();
    TestWait(ECUResetTime);

    SID10_SessionControl(0x02);
    CheckPosResponse_SID10(0x02);

    CheckCurrentSession(0x02);

    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    SID22_ReadDataByIdentifier(DID2);
    CheckPosResponse_SID22(DID2);

    TestSeq_DownloadSBL(0);

    SID22_ReadDataByIdentifier(DID3);
    //CheckPosResponse_SID22(DID3);
    CheckNegResponse(0x31);

    ActivateSBL();

    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    SID22_ReadDataByIdentifier(DID3);
    CheckPosResponse_SID22(DID3, 8);

    strcpy(keyvalue, "APP0Path_Decompress");

    for (int i = 0; i < APPFileNum; i++)
    {
        vbfparser(keyvalue);
        TestReport_PrintFileName("Downlaod Decompress");
        EraseAddress();
        FileDownload();
        CheckSignature();
        keyvalue[3]++;
    }

    if (0 != CheckCompleteAndCompatible()) {
        strcpy(issuereason, "");
        TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
    }

    SID11_ECUReset(0x81);
    CheckNonResponse();
    TestWait(ECUResetTime);

    CheckCurrentSession(0x01);

    RX_SID19_ReportDTCByStatusMask(0x08);
    RX_CheckPosResponse_SID19_02(0xFF);

    CheckSoftwareVersion();
}

void TestSeq_DownloadAPP_RequestSID22(u8 ifDupli36 = 0, u8 ifAbort = 0, u8 abortat = 0)
{
    TestReport_HTML_StepDescription("----Download APP----");

    char keyvalue[20] = "APP0Path";

    for (int i = 0; i < APPFileNum; i++)
    {
        char StepDespriction[256];
        char filename[300] = {};
        vbfparser(keyvalue);
        //获取文件名
        //app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
        //filename[strlen(filename) - 9] = '\0';
        //snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s ", filename);
        //TestReport_HTML_StepDescription(StepDespriction);
        filename[0] = '\0';
        if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
            log("Error: Failed to get filename for %s", keyvalue);
            snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
        }
        else {
            // 安全截断文件名
            size_t len = strlen(filename);
            if (len >= 9) {
                filename[len - 9] = '\0';
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
            }
            else {
                log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
            }
        }
        TestReport_HTML_StepDescription(StepDespriction);
        EraseAddress();
        if (ifAbort == 1 && abortat == i)
        {
            return;
        }
        FileDownload_RequestSoftwareAndUpdateProcessStatus(ifDupli36);
        CheckSignature();
        keyvalue[3]++;
    }
}

//add function by WYB
//下载文件，到指定的数据块就停止
s32 FileDownload_Abort_until_block(u8 Abortsize = 0)
{
    u8 subfunc[20] = {};
    u8 sequenceNum = 1;
    u8 transferData[65535] = {};

    for(int i=0;i<(Abortsize+1);i++)
    {
        sequenceNum = 1;

        //subfunc[0] = atoi(VbfFileInfo.data_format_identifier);
        subfunc[0] = (int)strtol(VbfFileInfo.data_format_identifier, NULL, 16);
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


// 计算阶乘
int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

void swap(char a[][20], int i, int j) {
    char temp[20];
    strcpy(temp, a[i]);
    strcpy(a[i], a[j]);
    strcpy(a[j], temp);
}

void FullSqueenceDownload_Disorder(char paths[][20], int start, int n, int* perm_count, int total_perms) {
    if (start == n - 1) {
        char StepDespriction[100] = {};
        // 增加排列计数
        (*perm_count)++;
        snprintf(StepDespriction, 100, "乱序刷写第%d次", *perm_count);
        TestReport_HTML_StepDescription(StepDespriction);
        printf("Testing permutation %d: ", *perm_count);
        for (int i = 0; i < n; i++) {
            printf("%s ", paths[i]);
        }
        printf("\n");

        SID10_SessionControl(0x82);
        CheckNonResponse();
        TestWait(ECUResetTime);

        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);

        CheckCurrentSession(0x02);

        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        u8 NRC[1] = { 0x31 };
        SID22_ReadDataByIdentifier(DID3);
        //CheckPosResponse_SID22(DID3);
        CheckNegResponse(NRC);

        ActivateSBL();

        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        // 对当前排列执行刷写操作
        for (int i = 0; i < n; i++) {
            printf("Parser File Name is %s\n", paths[i]);
            vbfparser(paths[i]);
            TestReport_PrintFileName("Download APP");
            EraseAddress();
            FileDownload();
            CheckSignature();
        }
        if (0 != CheckCompleteAndCompatible()) {
            strcpy(issuereason, "");
            TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckPosResponse_SID19_02(0xFF);

        CheckSoftwareVersion();

        CheckCurrentSession(0x01);

        PowerReset(30000);
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckPosResponse_SID19_02(0xFF);

        CheckSoftwareVersion();

        CheckCurrentSession(0x01);

        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);

        CheckCurrentSession(0x03);

        SID10_SessionControl(0x82);
        CheckNonResponse();
        TestWait(ECUResetTime);

        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);

        CheckCurrentSession(0x02);

        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        SID22_ReadDataByIdentifier(DID3);
        //CheckPosResponse_SID22(DID3);
        CheckNegResponse(NRC);

        ActivateSBL();

        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);
        // 对当前排列执行刷写操作
        for (int i = 0; i < n; i++) {
            printf("Parser File Name is %s\n", paths[i]);
            vbfparser(paths[i]);
            TestReport_PrintFileName("Download APP");
            EraseAddress();
            FileDownload();
            CheckSignature();
        }
        if (0 != CheckCompleteAndCompatible()) {
            strcpy(issuereason, "");
            TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckPosResponse_SID19_02(0xFF);

        CheckSoftwareVersion();

    }
    else {
        for (int i = start; i < n; i++) {
            swap(paths, start, i);
            FullSqueenceDownload_Disorder(paths, start + 1, n, perm_count, total_perms);
            swap(paths, start, i); // 恢复原始顺序
        }
    }
}

s32 CheckArrayDifferent(pu8 array1,pu8 array2,u8 length) {
    char TempDescription[200];
    u8 i = 0;
    for ( i = 0; i < length; i++) {
        if (array1[i] != array2[i]) {
            return -1;
        }
        if (i == length - 1)
            return 1;
    }
    return 0;
}

s32 CheckAppAndNMMessages() {
    if (NM_Flag == 1) {
        TestReport_HTML_StepDescription("NM Messages is Normal",Pass);
    }
    else {
        TestReport_HTML_StepDescription("NM Messages is Abnormal", Fail);
    }
    if (APP_Flag == 1) {
        TestReport_HTML_StepDescription("APP Messages is Normal",Pass);
    }
    else {
        TestReport_HTML_StepDescription("APP Messages is Abnormal", Fail);
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

s32 DiagnosticRequestNoResponse(pu8 request, u8 length, u8 addressModel = 0) {
    strcpy(requestInfo, (" " + dec2hex(request[0]) + " " + dec2hex(request[1])).c_str());
    strcpy(expectedResponse, "ECU NO Response!");
    strcpy(actualResponse, "No Response Received!");
    if (0 == addressModel) {
        if (0 == rtlUIDiagnostics.tstp_send_request(udsHandle, request, length)) {
            TestReport_StepInfo(Pass);
            return 0;
        }
        else {
            TestReport_StepInfo(Fail);
            return -1;
        }
    }
    else {
        if (0 == rtlUIDiagnostics.tstp_send_functional(udsHandle, request, length)) {
            TestReport_StepInfo(Pass);
            return 0;
        }
        else {
            TestReport_StepInfo(Fail);
            return -1;
        }
    }
    return 0;
}


s32 OTA_Init()
{
    TestSeq_EnterProgrammingSession();

    TestReport_HTML_StepDescription("测试前提:下载SBL，并不激活");
    TestSeq_DownloadSBL(0);

    //未激活后处于编程会话
    SID11_ECUReset(0x81);
    CheckNonResponse();
    return 0;
}
s32 OTA_SessionControl(u8 SubFunction, u8 AddressMode = 0, u64 waittime = 1000, int send_num = 3)
{
    u8 request[2] = { 0x10 };
    request[1] = SubFunction;
    for (int i = 0; i < send_num; i++)
    {
        //SID10_SessionControl(SubFunction, AddressMode);
        //if ((SubFunction & 0xF0) == 0x80)
        //{
        //    CheckNonResponse();
        //}
        //else
        //{
        //    CheckPosResponse_SID10(SubFunction);
        //}
        DiagnosticRequestNoResponse(request, 2);

        TestWait(waittime);
    }

    return 0;

}

s32 OTA_ControlDTCSetting(u8 SubFunction, u8 AddressMode = 0, u64 waittime = 1000, int send_num = 3)
{
    u8 request_0x85[2] = { 0x85,0x82 };
    for (int i = 0; i < send_num; i++)
    {
        DiagnosticRequestNoResponse(request_0x85, 2);
        //SID85_ControlDTCSetting(SubFunction, AddressMode);
        //if ((SubFunction & 0xF0) == 0x80)
        //{
        //    CheckNonResponse();
        //}
        //else
        //{
        //    CheckPosResponse_SID85(SubFunction);
        //}
        TestWait(waittime);
    }

    return 0;

}

s32 OTA_CommunicationControl(u8 SubFunction, u8 CommunicationType, u8 AddressMode = 0, u64 waittime = 1000, int send_num = 3)
{
    u8 request_0x10[2] = { 0x10,0x83 };
    for (int i = 0; i < send_num; i++)
    {
        DiagnosticRequestNoResponse(request_0x10, 2);
        //SID28_CommunicationControl(SubFunction, CommunicationType, AddressMode);
        //if ((SubFunction & 0xF0) == 0x80)
        //{
        //    CheckNonResponse();
        //}
        //else
        //{
        //    CheckPosResponse_SID28(SubFunction);
        //}
        TestWait(waittime);
    }

    return 0;

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

void SimVehMtnSpd()
{
    int i, j, n;
    int sigLength;
    int sigLength_d;
    int sigLength_m;
    u8 Data[100];
    int dataLen;
    u16 dataID;
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

    if (gVehSpdfdFlag)
    {
        Sim_VehSpd.set_is_brs(true);
        Sim_VehSpd.set_is_edl(true);
    }
    else
    {
        Sim_VehSpd.set_is_brs(false);
        Sim_VehSpd.set_is_edl(false);
    }
    Sim_VehSpd.FIdxChn = NetworkCluster[VehSpdLgt.cluster];
    Sim_VehSpd.FIdentifier = VehSpdLgt.slotID;
    Sim_VehSpd.FDLC = DataLengthToDLC[VehSpdLgt.dlc];
    for (i = 0; i < VehSpdLgt.dlc; i++)
    {
        Sim_VehSpd.FData[i] = Assignment_data[i];
    }
    counterVehSpd++;
}

void SimVehMtnSpd_Creat() {
    SimVehMtnSpd();
    com.add_cyclic_message_canfd(&Sim_VehSpd, 50);
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

void SimEngStsCAN()
{
    int i, j, n;
    int sigLength;
    int sigLength_d;
    int sigLength_m;
    u8 Data[100];
    int dataLen;
    u32 dataID;
    u32 CRC;
    for (j = 2; j < EngSts[0].sigNum; j++)
    {
        EngSts[0].e2eSignalList[j].sigValue = 0x0;
    }
    EngSts[0].e2eSignalList[2].sigValue = 0x1;

    if (cntrEngSts == 0xF)
    {
        cntrEngSts = 0;
    }

    Data[0] = cntrEngSts;////
    dataLen = 1;

    for (j = 2; j < EngSts[0].sigNum; j++)
    {
        sigLength = EngSts[0].e2eSignalList[j].sigLength;
        sigLength_d = sigLength / 8;
        sigLength_m = sigLength % 8;
        if (sigLength_m > 0)
        {
            sigLength_d = sigLength_d + 1;
        }

        for (n = 0; n < sigLength_d; n++)
        {
            Data[dataLen] = EngSts[0].e2eSignalList[j].sigValue >> (n * 8);

            dataLen++;
        }
    }

    dataID = EngSts[0].dataID;
    CRC = E2E_Check(dataID, Data, dataLen);

    EngSts[0].e2eSignalList[0].sigValue = CRC;
    EngSts[0].e2eSignalList[2].sigValue = cntrEngSts;

    Signal_assignment_EngSts(0);////
    if (gCEMFlg)
    {
        Sim_EngSts_CAN.FIdxChn = NetworkCluster[EngSts[0].cluster];
        Sim_EngSts_CAN.FIdentifier = EngSts[0].slotID;
        Sim_EngSts_CAN.FDLC = EngSts[0].dlc;
        for (i = 0; i < EngSts[0].dlc; i++)
        {
            Sim_EngSts_CAN.FData[i] = Assignment_data[i];
        }
        //com.add_cyclic_message_can(&Sim_EngSts_CAN, 50);
    }

    cntrEngSts++;
}

void SimEngStsCAN_Creat() {
    if (gCEMFlg) {
        SimEngStsCAN();
        com.add_cyclic_message_can(&Sim_EngSts_CAN, 50);
    }
}

void SimIDcDcActLoSideCAN()
{
    int i, j, n;
    int sigLength;
    int sigLength_d;
    int sigLength_m;
    u8 Data[100];
    int dataLen;
    u32 dataID;
    u32 CRC;
    for (j = 2; j < EngSts[1].sigNum; j++)
    {
        EngSts[1].e2eSignalList[j].sigValue = 0x0;
    }
    EngSts[1].e2eSignalList[2].sigValue = 0x0;

    if (cntrIDcDcActLoSide == 0xF)
    {
        cntrIDcDcActLoSide = 0;
    }

    Data[0] = cntrIDcDcActLoSide;////
    dataLen = 1;

    for (j = 2; j < EngSts[1].sigNum; j++)
    {
        sigLength = EngSts[1].e2eSignalList[j].sigLength;
        sigLength_d = sigLength / 8;
        sigLength_m = sigLength % 8;
        if (sigLength_m > 0)
        {
            sigLength_d = sigLength_d + 1;
        }

        for (n = 0; n < sigLength_d; n++)
        {
            Data[dataLen] = EngSts[1].e2eSignalList[j].sigValue >> (n * 8);

            dataLen++;
        }
    }
    dataID = EngSts[1].dataID;
    CRC = E2E_Check(dataID, Data, dataLen);

    EngSts[1].e2eSignalList[1].sigValue = CRC;
    EngSts[1].e2eSignalList[0].sigValue = cntrIDcDcActLoSide;

    Signal_assignment_EngSts(1);////


    if (gCEMFlg)
    {
        Sim_IDcDcActLoSide_CAN.FIdxChn = NetworkCluster[EngSts[1].cluster];
        Sim_IDcDcActLoSide_CAN.FIdentifier = EngSts[1].slotID;
        Sim_IDcDcActLoSide_CAN.FDLC = EngSts[1].dlc;
        for (i = 0; i < EngSts[1].dlc; i++)
        {
            Sim_IDcDcActLoSide_CAN.FData[i] = Assignment_data[i];
        }
        //com.add_cyclic_message_can(&Sim_IDcDcActLoSide_CAN, 50);
    }

    cntrIDcDcActLoSide++;
}


void SimIDcDcActLoSideCAN_Creat() {
    if (gCEMFlg)
    {
        SimIDcDcActLoSideCAN();
        com.add_cyclic_message_can(&Sim_IDcDcActLoSide_CAN, 50);
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


void SimVehMtnGlbSafeCANFD()
{
    int i, j, n;
    int sigLength;
    int sigLength_d;
    int sigLength_m;
    u8 Data[100];
    int dataLen;
    u8 dataID;
    u8 CRC;
    //信号值初始化
    for (j = 2; j < VehGlbSafe.sigNum; j++)
    {
        VehGlbSafe.e2eSignalList[j].sigValue = 0x0;
    }
    VehGlbSafe.e2eSignalList[2].sigValue = gCarMode;
    VehGlbSafe.e2eSignalList[7].sigValue = gEPLvl;
    VehGlbSafe.e2eSignalList[9].sigValue = gUagMode;


    if (counterVehMnGlSa == 0xF)
    {
        counterVehMnGlSa = 0;
    }

    Data[0] = counterVehMnGlSa;
    dataLen = 1;

    for (j = 2; j < VehGlbSafe.sigNum; j++)
    {
        sigLength = VehGlbSafe.e2eSignalList[j].sigLength;
        sigLength_d = sigLength / 8;
        sigLength_m = sigLength % 8;
        if (sigLength_m > 0)
        {
            sigLength_d = sigLength_d + 1;
        }

        for (n = 0; n < sigLength_d; n++)
        {
            Data[dataLen] = VehGlbSafe.e2eSignalList[j].sigValue >> (n * 8);

            dataLen++;
        }
    }

    dataID = VehGlbSafe.dataID;
    CRC = E2E_Check(dataID, Data, dataLen);

    VehGlbSafe.e2eSignalList[0].sigValue = CRC;
    VehGlbSafe.e2eSignalList[1].sigValue = counterVehMnGlSa;

    Signal_assignment_CANFD_UsageMode();

    Sim_UsgMode_CANFD.FIdxChn = NetworkCluster[VehGlbSafe.cluster];
    Sim_UsgMode_CANFD.FIdentifier = VehGlbSafe.slotID;
    Sim_UsgMode_CANFD.FDLC = 15;
    Sim_UsgMode_CANFD.FFDProperties = 0x1;  //isfd

    for (i = 0; i < VehGlbSafe.dlc; i++)
    {
        Sim_UsgMode_CANFD.FData[i] = Assignment_data[i];
    }
    //com.add_cyclic_message_canfd(&Sim_UsgMode_CANFD, 10);

    counterVehMnGlSa++;
}

void SimVehMtnGlbSafeCANFD_Creat() {
    SimVehMtnGlbSafeCANFD();
    com.add_cyclic_message_canfd(&Sim_UsgMode_CANFD, 10);
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

void SimVehBatOnCANFD()
{
    int i, j, n;
    Sim_VehBat_CANFD.FIdxChn = NetworkCluster[VehBattU.cluster];
    Sim_VehBat_CANFD.FIdentifier = VehBattU.slotID;
    Sim_VehBat_CANFD.FDLC = 15;
    Sim_VehBat_CANFD.FFDProperties = 0x1;  //isfd

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
}

void SimVehBat(u8 voltage)
{
    gVehBatVotg = voltage;
    // 检查 cluster 字符串中是否包含 "CANFD"
    if (strstr(VehBattU.cluster, "CANFD") != NULL) {
        gTRCfdFlag = 1;
    }
    else {
        gTRCfdFlag = 0;
    }
    if (gTRCfdFlag)
        SimVehBatOnCANFD_Creat();
    else
        SimVehBatOnCAN_Creat();
}

void FulfilTRC()
{
    if (gCEMFlg == 1)
    {
        //仿真报文
        //SimVehMtnSpdCANFD_Creat();
        //SimVehMtnSpd_Creat();
        //SimEngStsCAN_Creat();
        //SimIDcDcActLoSideCAN_Creat();
        SimulateTRC(false);

        SID10_SessionControl(0x01);
        CheckPosResponse_SID10(0x01);

        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);

        SecurityUnlock(0x03);
        SimulateTRC(true);
    }
    else
    {
        /*SimulateTRC(false);
        SimVehMtnGlbSafeCANFD_Creat();
        SimVehBat(gUbat12);*/
        SimulateTRC(true);
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
        com.can_rbs_set_signal_value_by_element(NetworkCluster[g_VehModMngtGlbSafe1_path.FDBName], g_VehModMngtGlbSafe1_path.FDBName, g_VehModMngtGlbSafe1_path.FECUName, g_VehModMngtGlbSafe1_path.FMsgName, g_VehModMngtGlbSafe1_path.FSignalName, gUagMode);
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
        snprintf(StepDespriction, sizeof(StepDespriction), "切换Carmode为%d,Usgmode为0x%X,EIPwrLvl为%d", carmode, usgmode, EIPwrLvl);
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

double SetVolatgeAndRead(double voltage, u8 flag = 1, u32 waitTime = 500) {
    double currentVol = 0;
    snprintf(StepDespriction, sizeof(StepDespriction), "Set Voltage is %.3lfV", voltage);
    TestReport_HTML_StepDescription(StepDespriction);
    main_power::instance()->Set_Voltage(voltage);

    if (flag) {
        app.wait(waitTime, "wait for read voltage");
        currentVol = main_power::instance()->Read_Val();
        snprintf(StepDespriction, sizeof(StepDespriction), "Current Voltage is %.3lfV", currentVol);
        TestReport_HTML_StepDescription(StepDespriction);
        return currentVol;
    }
    return 0;
}

double GetVolatge(u32 waitTime = 500) {
    double currentVol = 0;
    app.wait(waitTime, "wait for read voltage");
    currentVol = main_power::instance()->Read_Val();
    snprintf(StepDespriction, sizeof(StepDespriction), "Current Voltage is %.3lfV", currentVol);
    TestReport_HTML_StepDescription(StepDespriction);
    return currentVol;
}

//s32 TestWaitForCurInRange(float current, long time)
//{
//    int i;
//    float Cur;
//    char StepDespriction[256];
//
//    for (i = 0; i < (time / 1000); i++)
//    {
//        Cur = main_power::instance()->Read_Cur();
//        snprintf(StepDespriction, sizeof(StepDespriction), "The current is %5.3fmA", Cur * 1000);
//        TestReport_HTML_StepDescription(StepDespriction, Info);
//        AppWait(1000, "");
//        if (Cur * 1000 <= current)
//        {
//            snprintf(StepDespriction, sizeof(StepDespriction), "The current is %5.3fmA", Cur * 1000);
//            TestReport_HTML_StepDescription(StepDespriction, Pass);
//            break;
//        }
//
//    }
//    if (i == (time / 1000))
//    {
//        snprintf(StepDespriction, sizeof(StepDespriction), "Time out! The current is %5.3fmA, expected is %5.3fmA", Cur * 1000, current);
//        TestReport_HTML_StepDescription(StepDespriction, Fail);
//        return -1;
//    }
//    else
//        return 0;
//}

s32 TestWaitForCurInRange(float current, long time_ms)
{
    const long WAIT_INTERVAL_MS = 1000;  // 每次等待1秒
    long total_wait_ms = 0;

    while (total_wait_ms < time_ms)
    {
        float cur_ma = main_power::instance()->Read_Cur() * 1000.0f;

        if (cur_ma <= current)
        {
            // PASS 逻辑
            float cur_ma = main_power::instance()->Read_Cur() * 1000.0f;
            char step_desc[256];
            snprintf(step_desc, sizeof(step_desc),
                "PASS: Current %5.3fmA <= %5.3fmA", cur_ma, current);
            TestInfoPrintPass(step_desc, Pass);
            return 0;
        }

        // 等待间隔
        long wait_time = WAIT_INTERVAL_MS;
        if (total_wait_ms + wait_time > time_ms) {
            wait_time = time_ms - total_wait_ms;
        }

        if (wait_time > 0) {
            AppWait(wait_time, "");
            total_wait_ms += wait_time;
        }
    }

    float cur_ma = main_power::instance()->Read_Cur() * 1000.0f;
    char step_desc[256];
    snprintf(step_desc, sizeof(step_desc),
        "FAIL: Timeout! Current %5.3fmA > %5.3fmA", cur_ma, current);
    TestInfoPrintFail(step_desc, Fail);
    return -1;
}

int check_if_support_0x27()
{
    int supportServiceNum = -1;
    for (int i = 0; i < sizeof(SupportServices_Security); i++)
    {
        if (SupportServices_Security[i].service == 0x27)
        {
            supportServiceNum = i;
            log("支持0x27服务");
            break;
        }
    }

    if (supportServiceNum == -1)
        TestReport_HTML_StepDescription("不支持27服务");

    return supportServiceNum;
}
u8* return_level_levelx(u8 j, s8 supportServiceNum, u8 session)
{
    u8 level_levelx[2] = { 0 };
    u8 level = 0, levelx = 0;
    level = 0;
    levelx = 0;
    if (supportServiceNum == -1)
        return level_levelx;
    if (session == 1)
    {
        if (SupportServices_Security[supportServiceNum].subfunc[j].Sup_Def == 1)
        {
            for (u8 k = j + 1; k < SupportServices_Security[supportServiceNum].subfuncsize; k++)
            {
                level = SupportServices_Security[supportServiceNum].subfunc[j].Subfunc;
                if (SupportServices_Security[supportServiceNum].subfunc[k].Sup_Def == 1)
                {
                    levelx = SupportServices_Security[supportServiceNum].subfunc[k].Subfunc;
                    break;
                }
                break;
            }
        }

    }
    else if (session == 2)
    {
        if (SupportServices_Security[supportServiceNum].subfunc[j].Sup_PBL == 1)
        {
            for (u8 k = j + 1; k < SupportServices_Security[supportServiceNum].subfuncsize; k++)
            {
                level = SupportServices_Security[supportServiceNum].subfunc[j].Subfunc;
                if (SupportServices_Security[supportServiceNum].subfunc[k].Sup_PBL == 1)
                {
                    levelx = SupportServices_Security[supportServiceNum].subfunc[k].Subfunc;
                    break;
                }
                break;
            }
        }
    }
    else if (session == 3)
    {
        if (SupportServices_Security[supportServiceNum].subfunc[j].Sup_Ext == 1)
        {
            for (u8 k = j + 1; k < SupportServices_Security[supportServiceNum].subfuncsize; k++)
            {
                level = SupportServices_Security[supportServiceNum].subfunc[j].Subfunc;
                if (SupportServices_Security[supportServiceNum].subfunc[k].Sup_Ext == 1)
                {
                    levelx = SupportServices_Security[supportServiceNum].subfunc[k].Subfunc;
                    break;
                }
                break;
            }
        }
    }
    else
        TestReport_HTML_StepDescription("27服务会话错误", Fail);

    level_levelx[0] = level;
    level_levelx[1] = levelx;
    return level_levelx;
}

s32 Raw_CheckNVM_SID22(u8 DID[2], u8 CheckData = 0, u8 Data = 0,s64 responseArraySize = TpRxLength, u8* responseArray = TpRxData)
{
    strcpy(expectedResponse, "");
    strcpy(actualResponse, "");
    printf("responseArraySize=%d", responseArraySize);
    printf("TpRxLength=%d", TpRxLength);
    if (CheckData==1) {
        strcpy(expectedResponse, ("62 " + dec2hex(DID[0]) + " " + dec2hex(DID[1]) + " " + dec2hex(Data)).c_str());
    }
    else {
        strcpy(expectedResponse, ("62 " + dec2hex(DID[0]) + " " + dec2hex(DID[1])).c_str());
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
    if (CheckData == 1) {
        if (responseArray[3] == Data) {
            return 0;
        }
        else {
            strcpy(issuereason, "Unexpected WriteData!");
            return -1;
        }
    }


    return 0;
}

s32 CheckNVM_SID22(u8 DID[2], u8 CheckData=0,u8 Data = 0)
{
    if (0 == Raw_CheckNVM_SID22(DID, CheckData,Data))
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



//LIN Schedule
std::vector<TLIN>Diag_RX_Lin_V;

std::vector<TLIN>Diag_TX_Lin_V;

#include <unordered_set>
#include <vector>
#include <chrono>
#include <algorithm>
#include <map>
// 精简的ZCUD报文特征结构
struct ZCUDSignature {
    u8 identifier;
    u8 dlc;

    bool operator==(const ZCUDSignature& other) const {
        return identifier == other.identifier && dlc == other.dlc;
    }
};

// 哈希函数特化
namespace std {
    template<> struct hash<ZCUDSignature> {
        size_t operator()(const ZCUDSignature& s) const {
            return hash<u8>()(s.identifier) ^ hash<u8>()(s.dlc);
        }
    };
}

typedef std::pair<u8, u8> MsgKey; // ID和DLC的组合作为key

class ZCUDChecker {
private:
    // 按通道存储ZCUD特征
    std::map<u8, std::unordered_set<ZCUDSignature>> channel_zcud_signatures;
    // 存储通道索引到数据库名的映射
    std::map<u8, std::string> channel_db_names;
    const char* m_ecu_name;  // 要搜索的ECU名称

public:
    ZCUDChecker(const char* ecu_name = "ZCUD") : m_ecu_name(ecu_name) {}

    // 初始化ZCUD特征
    void init() {
        s32 n = 0;
        if (0 == app.db_get_lin_database_count(&n)) {
            for (s32 i = 0; i < n; i++) {
                TDBProperties db = { i };
                if (0 == app.db_get_lin_database_properties_by_index(&db)) {
                    std::string db_name = db.FName;

                    // 解析数据库支持的通道掩码
                    u64 channel_mask = db.FSupportedChannelMask;
                    for (u8 channel = 0; channel < 64; channel++) {  // 假设最多64个通道
                        if (channel_mask & (1ULL << channel)) {
                            // 存储通道索引到数据库名的映射
                            channel_db_names[channel] = db_name;

                            // 为该通道初始化ZCUD特征集合
                            std::unordered_set<ZCUDSignature>& signatures = channel_zcud_signatures[channel];

                            for (s32 j = 0; j < db.FECUCount; j++) {
                                TDBECUProperties ecu = { i, j };
                                if (0 == app.db_get_lin_ecu_properties_by_index(&ecu)) {
                                    if (strcmp(ecu.FName, m_ecu_name) == 0) {
                                        // 分别处理TX和RX报文
                                        for (s32 k = 0; k < ecu.FTxFrameCount; k++) {
                                            TDBFrameProperties msg = { i, j, k, 1 }; // TX方向
                                            if (0 == app.db_get_lin_frame_properties_by_index(&msg)) {
                                                log_hint("通道 %d - 数据库 %s: Tx Msg %d %s Id=0x%03X DLC=%d",
                                                    channel, db_name.c_str(), k + 1, msg.FName, msg.FLINIdentifier, msg.FLINDLC);
                                                signatures.insert({
                                                    static_cast<u8>(msg.FLINIdentifier),
                                                    static_cast<u8>(msg.FLINDLC)
                                                    });
                                            }
                                        }
                                        for (s32 k = 0; k < ecu.FRxFrameCount; k++) {
                                            TDBFrameProperties msg = { i, j, k, 0 }; // RX方向
                                            if (0 == app.db_get_lin_frame_properties_by_index(&msg)) {
                                                log_hint("通道 %d - 数据库 %s: Rx Msg %d %s Id=0x%03X DLC=%d",
                                                    channel, db_name.c_str(), k + 1, msg.FName, msg.FLINIdentifier, msg.FLINDLC);
                                                signatures.insert({
                                                    static_cast<u8>(msg.FLINIdentifier),
                                                    static_cast<u8>(msg.FLINDLC)
                                                    });
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

    // 获取当前搜索的ECU名称
    const char* get_ecu_name() const { return m_ecu_name; }

    // 判断报文是否属于ZCUD
    bool is_zcud_message(const TLIN& msg) {
        u8 channel = msg.FIdxChn;

        // 检查该通道是否有对应的ZCUD特征集合
        if (channel_zcud_signatures.find(channel) == channel_zcud_signatures.end()) {
            return false;
        }

        const std::unordered_set<ZCUDSignature>& signatures = channel_zcud_signatures[channel];
        bool tx_flag = (msg.FProperties & MASK_LINProp_DIR_TX) != 0;

        if (!tx_flag && msg.FDLC == 0) {
            // 检查数据库中是否存在该ID（不检查DLC）
            for (const auto& sig : signatures) {
                if (sig.identifier == msg.FIdentifier) {
                    return true;
                }
            }
            return false;
        }

        // 正常情况检查ID和DLC
        return signatures.count({ msg.FIdentifier, msg.FDLC }) > 0;
    }

    // 获取通道到数据库名的映射
    std::map<u8, std::string> get_channel_db_names() const {
        return channel_db_names;
    }

    // 检查特定通道是否有ZCUD报文定义
    bool has_zcud_for_channel(u8 channel) const {
        return channel_zcud_signatures.find(channel) != channel_zcud_signatures.end() &&
            !channel_zcud_signatures.at(channel).empty();
    }

    // 方法1: 设置特定通道的数据库名
    void set_channel_db_name(u8 channel, const std::string& db_name) {
        channel_db_names[channel] = db_name;
    }

    // 方法2: 批量设置通道数据库名
    void set_channel_db_names(const std::map<u8, std::string>& names) {
        for (const auto& pair : names) {
            channel_db_names[pair.first] = pair.second;
        }
    }

    // 方法3: 清除特定通道的数据库名
    void clear_channel_db_name(u8 channel) {
        auto it = channel_db_names.find(channel);
        if (it != channel_db_names.end()) {
            channel_db_names.erase(it);
        }
    }

    // 方法4: 清除所有通道的数据库名
    void clear_all_channel_db_names() {
        channel_db_names.clear();
    }
};

// ZCUP报文检测类，继承自ZCUDChecker，搜索ECU名称为"ZCUP"
class ZCUPChecker : public ZCUDChecker {
public:
    ZCUPChecker() : ZCUDChecker("ZCUP") {}
};

/*
//#define MAX_CHANNELS 7
//native_int CANMessageOBJ;
//int CurrentCheckChannel = -1;
// 4. 回调函数（已优化，添加时间戳记录）
void __stdcall OnCANMessageReceived(const pnative_int pobj, const PCANFD ACANFD) {
    // 仅处理ZCUD节点的TX报文
    if (ACANFD->FIdxChn >= 0 && ACANFD->FIdxChn < MAX_CHANNELS) {
        // 检查报文ID是否在该通道的合法列表中
        bool isValid = false;
        //for (uint32_t id : expectedCycles[ACANFD->FIdxChn].keys()) {
        //    if (id == ACANFD->FIdentifier) {
        //        isValid = true;
        //        break;
        //    }
        //}
    //    std::vector<unsigned int> keys;
    //    for (const auto& pair : expectedCycles[ACANFD->FIdxChn]) {
    //        keys.push_back(pair.first); // 提取键
    //    }

    //    if (isValid) {
    //        // 记录时间戳（毫秒级）
    //        s64 currentTime;
    //        app.get_timestamp(&currentTime);
    //        channelTimestamps[ACANFD->FIdxChn][ACANFD->FIdentifier].push_back(currentTime);
    //    }
    //    else {
    //        // 13. 生成错误日志（注意：实际测试框架可能需要记录到结果）
    //        log_nok("Invalid TX message from ZCUD node on channel %d: 0x%03X",
    //            ACANFD->FIdxChn + 1, ACANFD->FIdentifier);
    //        resultFlag = 1; // 标记测试失败
    //    }
    //}
}


*/

s8 CheckCommunication(u16 checkTime, s64 tolerabRecoveryleTime, u8 CheckChannel = 99) {
    char StepDescription[500] = {};
    s64 lastTime, currentTime = 0;
    //s32 v;
    if (CheckChannel == 99) {
        snprintf(StepDescription, sizeof(StepDescription), "Check DUT Communication");
        TestReport_HTML_StepDescription(StepDescription, Info);
    }
    else {
        snprintf(StepDescription, sizeof(StepDescription), "Check Channel CH%d Communication", CheckChannel + 1);
        TestReport_HTML_StepDescription(StepDescription, Info);
    }
    //app.set_system_var_int32("CheckCom", 0);
    //printf("set CheckCom 0");
    APP_Flag = 0;
    NM_Flag = 0;

    app.get_timestamp(&lastTime);
    while (1) {
        app.get_timestamp(&currentTime);
        
        if ((currentTime - lastTime) / 1000 > checkTime) {
            TestReport_HTML_StepDescription("DUT Communication Is Not Normal", Fail);
            return -1;
        }
        else if (1 == NM_Flag || 1 == APP_Flag) {

            if (((currentTime - lastTime) / 1000 < checkTime) && CheckChannel == 99) {
                TestReport_HTML_StepDescription("DUT Communication Is Normal", Info);
                return 0;
            }
            else if (abs((currentTime - lastTime) / 1000 - checkTime) < tolerabRecoveryleTime) {
                TestReport_HTML_StepDescription("DUT Communication Is Normal", Info);
                return 0;
            }
            else {
                TestReport_HTML_StepDescription("The DUT's recovery time for communication is shorter or longer than defined.", Fail);
                return 0;
            }
        }
    }

    /*if (v == 1) {
        TestReport_HTML_StepDescription("DUT Communication Is Normal", Info);
        return 0;
    }
    else {
        TestReport_HTML_StepDescription("DUT Communication Is Not Normal", Fail);
        return -1;
    }
    return 0;*/
}

s32 NM_NextOperation()
{
    SID10_SessionControl(0x03);
    CheckPosResponse_SID10(0x03);
    SecurityUnlock(0x03);
    UsgModSwtich(0x1);
    EPLvlSwitch(0x0);
    AppWait(1000, "");
    SendTesterPresent(TP_OFF);
    SimulateTRC(false);
    NMWakeUp(0);
    com.del_cyclic_messages();
    KL15(off);
    AppWait(2000, "");
    if (0 != TestWaitForCurInRange(gLowPwrCur, gSleepTime)) {
        return -1;
    }

    AppWait(2000, "");
    KL15(on);
    NMWakeUp(1);
    SendTesterPresent(TP_ON);
    AppWait(10000, "");
    FulfilTRC();
    AppWait(1000, "");
    //TRCSwitch(0x0, 0xD, 0x0);
    //CarModSwtich(0x0);
    //UsgModSwtich(0xD);
    //EPLvlSwitch(0x0);
    return 0;
}

void NM_NextOperation_NoSwitch()
{
    //UsgModSwtich(0x1);
    //EPLvlSwitch(0x0);
    app.wait(1000, "");
    SendTesterPresent(TP_OFF);
    SimulateTRC(false);
    NMWakeUp(0);
    com.del_cyclic_messages();
    KL15(off);
    app.wait(2000, "");
    TestWaitForCurInRange(gLowPwrCur, gSleepTime);

    app.wait(2000, "");
    KL15(on);
    NMWakeUp(1);
    SendTesterPresent(TP_ON);
    app.wait(10000, "");
    //FulfilTRC();
    //app.wait(1000, "");
    //TRCSwitch(0x0, 0xD, 0x0);
    //CarModSwtich(0x0);
    //UsgModSwtich(0xD);
    //EPLvlSwitch(0x0);
}

s32 calculateResult_C00A(pu8 array, bool state, u32 baseCount, u32 checkCount) {
    u32 readCount = 10;
    u32 passCount = 0;

    passCount = ((u32)array[3] << 24) | ((u32)array[4] << 16) |
        ((u32)array[5] << 8) | (u32)array[6];

    if (!state) {
        readCount = 5;
    }

    if (checkCount != passCount - baseCount) {
        TestInfoPrintFail("0xC00A Read Pass Count Is Not Correct");
        return -1;
    }

    for (int i = 0; i < readCount; i++) {
        int offset = 11 + i * 12;
        u8 expectedByte5 = state ? 0x81 : 0x01;

        if (array[offset + 4] != expectedByte5) {
            TestInfoPrintFail("0xC00A Data Format Error: Byte 5 incorrect");
            return -2;
        }

        for (int j = 1; j < 8; j++) {
            if (array[offset + 4 + j] != 0xFF) {
                TestInfoPrintFail("0xC00A Data Format Error: Bytes 6-12 not 0xFF");
                return -3;
            }
        }
    }

    u32 startTestNum = baseCount + 1;
    u32 endTestNum = passCount;

    for (u32 testNum = startTestNum; testNum < endTestNum; testNum++) {
        u32 containerIdx1 = (testNum - 1) % readCount;
        u32 containerIdx2 = testNum % readCount;

        int offset1 = 11 + containerIdx1 * 12;
        int offset2 = 11 + containerIdx2 * 12;

        u32 timestamp1 = ((u32)array[offset1] << 24) | ((u32)array[offset1 + 1] << 16) |
            ((u32)array[offset1 + 2] << 8) | (u32)array[offset1 + 3];
        u32 timestamp2 = ((u32)array[offset2] << 24) | ((u32)array[offset2 + 1] << 16) |
            ((u32)array[offset2 + 2] << 8) | (u32)array[offset2 + 3];

        if (timestamp1 >= timestamp2) {
            TestInfoPrintFail("0xC00A Timestamp Not Strictly Increasing");
            return -4;
        }
    }

    return 0;
}

s32 calculateResult_C00B(pu8 array, u32 baseCount, u32 checkCount) {
    u32 readCount = 20;
    u32 passCount = 0;

    passCount = ((u32)array[3] << 24) | ((u32)array[4] << 16) |
        ((u32)array[5] << 8) | (u32)array[6];


    if (checkCount != passCount - baseCount) {
        TestInfoPrintFail("0xC00B Read Pass Count Is Not Correct");
        return -1;
    }

    for (int i = 0; i < readCount; i++) {
        int offset = 11 + i * 12;
        u8 expectedByte5 = 0x01;

        if (array[offset + 4] != expectedByte5) {
            TestInfoPrintFail("0xC00B Data Format Error: Byte 5 incorrect");
            return -2;
        }

        for (int j = 1; j < 4; j++) {
            if (array[offset + 4 + j] != 0xFF) {
                TestInfoPrintFail("0xC00B Data Format Error: Bytes 6-8 not 0xFF");
                return -3;
            }
        }
    }

    u32 startTestNum = baseCount + 1;
    u32 endTestNum = passCount;

    for (u32 testNum = startTestNum; testNum < endTestNum; testNum++) {
        u32 containerIdx1 = (testNum - 1) % readCount;
        u32 containerIdx2 = testNum % readCount;

        int offset1 = 11 + containerIdx1 * 12;
        int offset2 = 11 + containerIdx2 * 12;

        u32 timestamp1 = ((u32)array[offset1] << 24) | ((u32)array[offset1 + 1] << 16) |
            ((u32)array[offset1 + 2] << 8) | (u32)array[offset1 + 3];
        u32 timestamp2 = ((u32)array[offset2] << 24) | ((u32)array[offset2 + 1] << 16) |
            ((u32)array[offset2 + 2] << 8) | (u32)array[offset2 + 3];

        if (timestamp1 >= timestamp2) {
            TestInfoPrintFail("0xC00B Timestamp Not Strictly Increasing");
            return -4;
        }
    }

    return 0;
}
/*
int FindSignalPath(const char* dut_name, const char* signalName, TSignalPath* out_path, int CHANNEL_PRIORITY_COUNT, const u32 CHANNEL_PRIORITY[]) {
    s32 i, j, k, m, n;
    int prio_idx;

    printf("[调试] 进入 FindSignalPath 函数\n");
    printf("[调试] 查找参数: dut_name=%s, signalName=%s\n", dut_name ? dut_name : "NULL", signalName ? signalName : "NULL");
    printf("[调试] 通道优先级数量: %d\n", CHANNEL_PRIORITY_COUNT);

    // 参数检查
    if (!dut_name || !out_path) {
        printf("[调试] 参数错误: dut_name=%p, out_path=%p\n", dut_name, out_path);
        return -1;
    }

    // 按优先级顺序遍历数据库
    for (prio_idx = 0; prio_idx < CHANNEL_PRIORITY_COUNT; prio_idx++) {
        printf("[调试] ========== 优先级级别 %d/%d ==========\n", prio_idx + 1, CHANNEL_PRIORITY_COUNT);

        u32 target_channel = CHANNEL_PRIORITY[prio_idx];
        printf("[调试] 目标通道: %d (0x%X)\n", target_channel, target_channel);

        // 遍历所有数据库
        if (0 != app.db_get_can_database_count(&n)) {
            printf("[调试] 获取数据库数量失败，跳过本次优先级\n");
            continue;
        }
        printf("[调试] 数据库总数: %d\n", n);

        for (i = 0; i < n; i++) {
            printf("[调试] --- 处理数据库索引 %d/%d ---\n", i + 1, n);

            TDBProperties db;
            db.FDBIndex = i;
            if (0 != app.db_get_can_database_properties_by_index(&db)) {
                printf("[调试] 获取数据库 %d 属性失败，跳过\n", i);
                continue;
            }
            printf("[调试] 数据库名称: %s\n", db.FName);
            printf("[调试] 数据库通道掩码: 0x%X\n", db.FSupportedChannelMask);

            // **关键点1**：检查数据库是否支持目标通道
            if (NetworkCluster[db.FName] != target_channel) {
                printf("[调试] 数据库 %s 不在当前优先级通道上 (NetworkCluster=%d, 目标=%d)，跳过\n",
                    db.FName, NetworkCluster[db.FName], target_channel);
                continue;
            }
            printf("[调试] 数据库 %s 匹配当前优先级通道\n", db.FName);

            // 遍历ECU查找DUT
            printf("[调试] ECU总数: %d\n", db.FECUCount);
            for (j = 0; j < db.FECUCount; j++) {
                printf("[调试]   处理ECU索引 %d/%d\n", j + 1, db.FECUCount);

                TDBECUProperties ecu;
                ecu.FDBIndex = i;
                ecu.FECUIndex = j;
                if (0 != app.db_get_can_ecu_properties_by_index(&ecu)) {
                    printf("[调试]   获取ECU %d 属性失败，跳过\n", j);
                    continue;
                }
                printf("[调试]   ECU名称: %s\n", ecu.FName);

                // **关键点2**：匹配DUT名称 - 修正后的逻辑
                if (strcmp(ecu.FName, dut_name) != 0) {
                    printf("[调试]   ECU名称不匹配 DUT (期望:%s, 实际:%s)，跳过\n", dut_name, ecu.FName);
                    continue;
                }
                printf("[调试]   ECU名称匹配 DUT: %s\n", dut_name);

                // **关键点3**：只找Tx方向的报文（DUT发送的）
                printf("[调试]   Tx报文数量: %d\n", ecu.FTxFrameCount);
                for (k = 0; k < ecu.FTxFrameCount; k++) {
                    printf("[调试]     处理Tx报文索引 %d/%d\n", k + 1, ecu.FTxFrameCount);

                    TDBFrameProperties msg;
                    msg.FDBIndex = i;
                    msg.FECUIndex = j;
                    msg.FFrameIndex = k;
                    msg.FIsTx = 1;
                    if (0 != app.db_get_can_frame_properties_by_index(&msg)) {
                        printf("[调试]     获取报文 %d 属性失败，跳过\n", k);
                        continue;
                    }
                    printf("[调试]     报文名称: %s\n", msg.FName);
                    printf("[调试]     报文ID: 0x%X\n", msg.FCANIdentifier);
                    printf("[调试]     信号数量: %d\n", msg.FSignalCount);

                    // 遍历信号
                    for (m = 0; m < msg.FSignalCount; m++) {
                        TDBSignalProperties sgn;
                        sgn.FDBIndex = i;
                        sgn.FECUIndex = j;
                        sgn.FFrameIndex = k;
                        sgn.FSignalIndex = m;
                        sgn.FIsTx = 1;
                        if (0 != app.db_get_can_signal_properties_by_index(&sgn)) {
                            printf("[调试]       获取信号 %d 属性失败，跳过\n", m);
                            continue;
                        }
                        printf("[调试]       信号名称: %s\n", sgn.FName);

                        // **关键点4**：匹配信号名
                        if (strcmp(sgn.FName, signalName) == 0) {
                            printf("[调试]       *** 找到匹配信号！***\n");

                            // 填充路径结构
                            out_path->FDBIndex = i;
                            out_path->FECUIndex = j;
                            out_path->FFrameIndex = k;
                            out_path->FSignalIndex = m;
                            out_path->FIsTx = 1;

                            strncpy(out_path->FDBName, db.FName, sizeof(out_path->FDBName) - 1);
                            out_path->FDBName[sizeof(out_path->FDBName) - 1] = '\0';

                            strncpy(out_path->FECUName, ecu.FName, sizeof(out_path->FECUName) - 1);
                            out_path->FECUName[sizeof(out_path->FECUName) - 1] = '\0';

                            strncpy(out_path->FMsgName, msg.FName, sizeof(out_path->FMsgName) - 1);
                            out_path->FMsgName[sizeof(out_path->FMsgName) - 1] = '\0';

                            strncpy(out_path->FSignalName, sgn.FName, sizeof(out_path->FSignalName) - 1);
                            out_path->FSignalName[sizeof(out_path->FSignalName) - 1] = '\0';

                            out_path->FFrameIndentifier = msg.FCANIdentifier;
                            out_path->FFoundChannel = target_channel;

                            printf("[调试]       ========================================\n");
                            printf("[调试]       路径找到成功！\n");
                            printf("[调试]       数据库[%s] 索引=%d\n", db.FName, i);
                            printf("[调试]       ECU[%s] 索引=%d\n", ecu.FName, j);
                            printf("[调试]       报文[%s] 索引=%d\n", msg.FName, k);
                            printf("[调试]       信号[%s] 索引=%d\n", sgn.FName, m);
                            printf("[调试]       通道=0x%X (优先级%d)\n", target_channel, prio_idx);
                            printf("[调试]       ========================================\n");

                            return 0;  // 成功找到
                        }
                    }
                }
            }
        }
    }

    printf("[调试] ========================================\n");
    printf("[调试] 查找失败！\n");
    printf("[调试] 未找到 DUT[%s] 的信号 %s\n", dut_name ? dut_name : "NULL", signalName ? signalName : "NULL");
    printf("[调试] 已遍历所有优先级通道和数据库\n");
    printf("[调试] ========================================\n");

    return -1;
}
*/
 //搜索匹配函数
int FindSignalPath(const char* dut_name, const char* signalName, TSignalPath* out_path, int CHANNEL_PRIORITY_COUNT, const u32 CHANNEL_PRIORITY[]) {
    s32 i, j, k, m, n;
    int prio_idx;

    if (!dut_name || !out_path) return -1;

    // 按优先级顺序遍历数据库
    for (prio_idx = 0; prio_idx < CHANNEL_PRIORITY_COUNT; prio_idx++) {
        u32 target_channel = CHANNEL_PRIORITY[prio_idx];

        // 遍历所有数据库
        if (0 != app.db_get_can_database_count(&n)) continue;

        for (i = 0; i < n; i++) {
            TDBProperties db;
            db.FDBIndex = i;
            if (0 != app.db_get_can_database_properties_by_index(&db)) continue;

            // **关键点1**：检查数据库是否支持目标通道
            if (NetworkCluster[db.FName] != target_channel) {
                log("NetworkCluster[%s] = %d, target_channel is %d,非优先级通道", db.FName, NetworkCluster[db.FName], target_channel);
                continue;  // 此数据库不在当前优先级通道上
            }
            log("NetworkCluster[%s] = %d, target_channel is %d,优先级通道", db.FName, NetworkCluster[db.FName], target_channel);
            // 遍历ECU查找DUT
            for (j = 0; j < db.FECUCount; j++) {
                TDBECUProperties ecu;
                ecu.FDBIndex = i;
                ecu.FECUIndex = j;
                if (0 != app.db_get_can_ecu_properties_by_index(&ecu)) continue;

                // **关键点2**：匹配DUT名称
                //if (strcmp(ecu.FName, dut_name) != 0) continue;
                if (strcmp(ecu.FName, dut_name) == 0) continue;

                // **关键点3**：只找Tx方向的报文（DUT发送的）
                // 遍历Tx报文
                for (k = 0; k < ecu.FTxFrameCount; k++) {
                    TDBFrameProperties msg;
                    msg.FDBIndex = i;
                    msg.FECUIndex = j;
                    msg.FFrameIndex = k;
                    msg.FIsTx = 1;
                    if (0 != app.db_get_can_frame_properties_by_index(&msg)) continue;

                    // 遍历信号
                    for (m = 0; m < msg.FSignalCount; m++) {
                        TDBSignalProperties sgn;
                        sgn.FDBIndex = i;
                        sgn.FECUIndex = j;
                        sgn.FFrameIndex = k;
                        sgn.FSignalIndex = m;
                        sgn.FIsTx = 1;
                        if (0 != app.db_get_can_signal_properties_by_index(&sgn)) continue;

                        // **关键点4**：匹配信号名
                        if (strcmp(sgn.FName, signalName) == 0) {
                            // 找到！填充路径结构
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
                            return 0;  // 成功找到
                        }
                    }
                }
            }
        }
    }

    log("未找到DUT[%s]的 %s 信号路径", dut_name, signalName);
    return -1;
}
/*
s32 StartCANRBS(TSignalPath* path, bool state = true) {
    s32 ret = 0;  // 保存返回值
    printf("[调试] 进入 StartCANRBS 函数\n");
    printf("[调试] 参数 state = %d\n", state);
    printf("[调试] 信号名称: %s\n", path->FSignalName);

    if (NetworkCluster[path->FDBName] == path->FFoundChannel) {
        printf("[调试] 当前查找通道一致\n");
    }
    else {
        printf("[调试] 当前查找 NetworkCluster[%s] = CH%d\n", path->FDBName, NetworkCluster[path->FDBName] + 1);
        printf("[调试] 当前查找 FFoundChannel[%s] = CH%d\n", path->FDBName, path->FFoundChannel + 1);
    }

    printf("[调试] 开始执行 CAN RBS 激活/停用操作\n");
    if (state) {
        printf("[调试] 激活模式: 激活网络、节点和报文\n");
        ret = com.can_rbs_activate_network_by_name(NetworkCluster[path->FDBName], true, path->FDBName, false);
        printf("[调试] 网络激活完成: %s, 返回值=%d\n", path->FDBName, ret);
        if (ret != 0) goto EXIT;

        ret = com.can_rbs_activate_node_by_name(NetworkCluster[path->FDBName], true, path->FDBName, path->FECUName, false);
        printf("[调试] 节点激活完成: %s, 返回值=%d\n", path->FECUName, ret);
        if (ret != 0) goto EXIT;

        ret = com.can_rbs_activate_message_by_name(NetworkCluster[path->FDBName], true, path->FDBName, path->FECUName, path->FMsgName);
        printf("[调试] 报文激活完成: %s, 返回值=%d\n", path->FMsgName, ret);
        if (ret != 0) goto EXIT;
    }
    else {
        printf("[调试] 停用模式: 停用报文、节点和网络\n");
        ret = com.can_rbs_activate_message_by_name(NetworkCluster[path->FDBName], false, path->FDBName, path->FECUName, path->FMsgName);
        printf("[调试] 报文停用完成: %s, 返回值=%d\n", path->FMsgName, ret);
        if (ret != 0) goto EXIT;

        ret = com.can_rbs_activate_node_by_name(NetworkCluster[path->FDBName], false, path->FDBName, path->FECUName, false);
        printf("[调试] 节点停用完成: %s, 返回值=%d\n", path->FECUName, ret);
        if (ret != 0) goto EXIT;

        ret = com.can_rbs_activate_network_by_name(NetworkCluster[path->FDBName], false, path->FDBName, false);
        printf("[调试] 网络停用完成: %s, 返回值=%d\n", path->FDBName, ret);
        if (ret != 0) goto EXIT;
    }

    printf("[调试] 当前 gCEMFlg = %d\n", gCEMFlg);

    if (gCEMFlg == 1) {
        printf("[调试] 仿真模式分支 (gCEMFlg == 1)\n");

        if (strncmp(path->FSignalName, "VehSpdLgtA", 10) == 0) {
            printf("[调试] 匹配信号: VehSpdLgtA (车速信号)\n");
            printf("[调试] 设置 VehSpdLgt_UB = 1\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehSpdLgt_UB", 1);
            printf("[调试] 设置 VehSpdLgt_UB 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 VehSpdLgtQf = 3\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehSpdLgtQf", 3);
            printf("[调试] 设置 VehSpdLgtQf 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 %s = %d (gVehSpd)\n", path->FSignalName, gVehSpd);
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gVehSpd);
            printf("[调试] 设置 %s 返回值=%d\n", path->FSignalName, ret);
        }
        else if (strncmp(path->FSignalName, "VehMtnStVehMtnSt", 30) == 0) {
            printf("[调试] 匹配信号: VehMtnStVehMtnSt (车辆运动状态)\n");
            printf("[调试] 设置 VehMtnSt_UB = 1\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehMtnSt_UB", 1);
            printf("[调试] 设置 VehMtnSt_UB 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 %s = 3\n", path->FSignalName);
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, 3);
            printf("[调试] 设置 %s 返回值=%d\n", path->FSignalName, ret);
        }
        else if (strncmp(path->FSignalName, "EngSt1WdStsEngSt1WdSts", 30) == 0) {
            printf("[调试] 匹配信号: EngSt1WdStsEngSt1WdSts (发动机状态)\n");
            printf("[调试] 设置 EngSt1WdSts_UB = 1\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "EngSt1WdSts_UB", 1);
            printf("[调试] 设置 EngSt1WdSts_UB 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 %s = 1\n", path->FSignalName);
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, 1);
            printf("[调试] 设置 %s 返回值=%d\n", path->FSignalName, ret);
        }
        else if (strncmp(path->FSignalName, "IDcDcActLoSideIDcDcActLoSide", 30) == 0) {
            printf("[调试] 匹配信号: IDcDcActLoSide (DCDC 状态)\n");
            printf("[调试] 设置 IDcDcActLoSide_UB = 1\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "IDcDcActLoSide_UB", 1);
            printf("[调试] 设置 IDcDcActLoSide_UB 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 %s = 0\n", path->FSignalName);
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, 0);
            printf("[调试] 设置 %s 返回值=%d\n", path->FSignalName, ret);
        }
        else {
            printf("[调试] 警告: 未匹配到任何已知信号, 信号名称=%s\n", path->FSignalName);
        }
    }
    else {
        printf("[调试] 非仿真模式分支 (gCEMFlg != 1)\n");

        if (strncmp(path->FSignalName, "VehSpdLgtA", 10) == 0) {
            printf("[调试] 匹配信号: VehSpdLgtA (车速信号)\n");
            printf("[调试] 设置 VehSpdLgt_UB = 1\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehSpdLgt_UB", 1);
            printf("[调试] 设置 VehSpdLgt_UB 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 VehSpdLgtQf = 3\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehSpdLgtQf", 3);
            printf("[调试] 设置 VehSpdLgtQf 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 %s = %d (gVehSpd)\n", path->FSignalName, gVehSpd);
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gVehSpd);
            printf("[调试] 设置 %s 返回值=%d\n", path->FSignalName, ret);
        }
        else if (strncmp(path->FSignalName, "VehModMngtGlbSafe1UsgModSts", 30) == 0) {
            printf("[调试] 匹配信号: VehModMngtGlbSafe1UsgModSts (整车模式管理)\n");
            printf("[调试] 设置 VehModMngtGlbSafe1_UB = 1\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehModMngtGlbSafe1_UB", 1);
            printf("[调试] 设置 VehModMngtGlbSafe1_UB 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 VehModMngtGlbSafe1CarModSts1 = %d (gCarMode)\n", gCarMode);
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehModMngtGlbSafe1CarModSts1", gCarMode);
            printf("[调试] 设置 VehModMngtGlbSafe1CarModSts1 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 VehModMngtGlbSafe1PwrLvlElecMai = %d (gEPLvl)\n", gEPLvl);
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehModMngtGlbSafe1PwrLvlElecMai", gEPLvl);
            printf("[调试] 设置 VehModMngtGlbSafe1PwrLvlElecMai 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 %s = %d (gUagMode)\n", path->FSignalName, gUagMode);
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gUagMode);
            printf("[调试] 设置 %s 返回值=%d\n", path->FSignalName, ret);
        }
        else if (strncmp(path->FSignalName, "VehBattUSysU", 30) == 0) {
            printf("[调试] 匹配信号: VehBattUSysU (蓄电池电压)\n");
            printf("[调试] 设置 VehBattU_UB = 1\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehBattU_UB", 1);
            printf("[调试] 设置 VehBattU_UB 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 VehBattUSysUQf = 3\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehBattUSysUQf", 3);
            printf("[调试] 设置 VehBattUSysUQf 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 %s = %d (gUbat12_Phy)\n", path->FSignalName, gUbat12_Phy);
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gUbat12_Phy);
            printf("[调试] 设置 %s 返回值=%d\n", path->FSignalName, ret);
        }
        else if (strncmp(path->FSignalName, "CarTiGlb", 30) == 0) {
            printf("[调试] 匹配信号: CarTiGlb (全局时间)\n");
            printf("[调试] 设置 CarTiGlb_UB = 1\n");
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "CarTiGlb_UB", 1);
            printf("[调试] 设置 CarTiGlb_UB 返回值=%d\n", ret);
            if (ret != 0) goto EXIT;

            printf("[调试] 设置 %s = %d (gCarTiGlb_Sim)\n", path->FSignalName, gCarTiGlb_Sim);
            ret = com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gCarTiGlb_Sim);
            printf("[调试] 设置 %s 返回值=%d\n", path->FSignalName, ret);
        }
        else {
            printf("[调试] 警告: 未匹配到任何已知信号, 信号名称=%s\n", path->FSignalName);
        }
    }

EXIT:
    if (ret == 0) {
        printf("[调试] StartCANRBS 函数执行成功，返回 0\n");
    }
    else {
        printf("[调试] StartCANRBS 函数执行失败，返回 %d\n", ret);
    }
    return ret;
}
*/
s32 StartCANRBS(TSignalPath* path, bool state = true) {
    if (NetworkCluster[path->FDBName] == path->FFoundChannel) {
        printf("当前查找通道一致");
    }
    else {
        printf("当前查找NetworkCluster[%s] = CH%d\n", path->FDBName, NetworkCluster[path->FDBName] + 1);
        printf("当前查找FFoundChannel[%s] = CH%d", path->FDBName, path->FFoundChannel + 1);
    }
    if (state) {
        com.can_rbs_activate_network_by_name(NetworkCluster[path->FDBName], true, path->FDBName, false);
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
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gVehSpd);
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
        if (strncmp(path->FSignalName, "VehSpdLgtA", 10) == 0) {
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehSpdLgt_UB", 1);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehSpdLgtQf", 3);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gVehSpd);
        }
        else if (strncmp(path->FSignalName, "VehModMngtGlbSafe1UsgModSts", 30) == 0) {
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehModMngtGlbSafe1_UB", 1);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehModMngtGlbSafe1CarModSts1", gCarMode);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehModMngtGlbSafe1PwrLvlElecMai", gEPLvl);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gUagMode);
        }
        else if (strncmp(path->FSignalName, "VehBattUSysU", 30) == 0) {
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehBattU_UB", 1);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "VehBattUSysUQf", 3);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gUbat12_Phy);
        }
        else if (strncmp(path->FSignalName, "CarTiGlb", 30) == 0) {
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, "CarTiGlb_UB", 1);
            com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, gCarTiGlb_Sim);
        }
    }
    return 0;
}

s32 UpdateCarTiGlbRBS(TSignalPath* path,u32 value) {
    com.can_rbs_set_signal_value_by_element(NetworkCluster[path->FDBName], path->FDBName, path->FECUName, path->FMsgName, path->FSignalName, value);
    return 0;
}

s32 UpdateValueTimer(bool status) {
    if (status) {
        NewTimer1.start();
        gCarTiGlb_Sim = 0;
    }
    else {
        NewTimer1.stop();
    }
    return 0;
}

s32 SimulateTRC(bool state) {
    if (state) {
        com.can_rbs_start();
        if (gCEMFlg) {
            if (g_VehSpdLgt_found) {
                StartCANRBS(&g_VehSpdLgt_path);//0x9f chassiscanfd2 BBM
            }
            if (g_EngSt1WdSts_found) {
                StartCANRBS(&g_EngSt1WdSts_path);//0x4b proplusioncan XCU
            }
            if (g_IDcDcActLoSide_found) {
                StartCANRBS(&g_IDcDcActLoSide_path);//0x149 proplusiocan XCU
            }
            if (g_VehMtnSt_found) {
                StartCANRBS(&g_VehMtnSt_path);//0x76 proplusioncan ZCUD
            }
        }
        else
        {
            if (g_VehSpdLgt_found) {
                StartCANRBS(&g_VehSpdLgt_path);
            }
            if (g_VehModMngtGlbSafe1_found) {
                StartCANRBS(&g_VehModMngtGlbSafe1_path);
            }
            if (g_VehUbat_found) {
                StartCANRBS(&g_VehUbat_path);
            }
            if (g_CarTiGlb_found) {
                StartCANRBS(&g_CarTiGlb_path);
            }
        }
    }
    else {
        if (gCEMFlg) {
            if (g_VehSpdLgt_found) {
                StartCANRBS(&g_VehSpdLgt_path, false);
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
            if (g_VehSpdLgt_found) {
                StartCANRBS(&g_VehSpdLgt_path, false);
            }
            if (g_VehModMngtGlbSafe1_found) {
                StartCANRBS(&g_VehModMngtGlbSafe1_path, false);
            }
            if (g_VehUbat_found) {
                StartCANRBS(&g_VehUbat_path, false);
            }
            if (g_CarTiGlb_found) {
                StartCANRBS(&g_CarTiGlb_path, false);
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
        g_VehSpdLgt_found = (0 == FindSignalPath(DUTName, "VehSpdLgtA", &g_VehSpdLgt_path, VehSpd_CHANNEL_PRIORITY_COUNT, VehSpd_CHANNEL_PRIORITY));
        g_VehModMngtGlbSafe1_found = (0 == FindSignalPath(DUTName, "VehModMngtGlbSafe1UsgModSts", &g_VehModMngtGlbSafe1_path, VehMtn_CHANNEL_PRIORITY_COUNT, VehMtn_CHANNEL_PRIORITY));
        g_VehUbat_found = (0 == FindSignalPath(DUTName, "VehBattUSysU", &g_VehUbat_path, VehMtn_CHANNEL_PRIORITY_COUNT, VehMtn_CHANNEL_PRIORITY));
        g_CarTiGlb_found = (0 == FindSignalPath(DUTName, "CarTiGlb", &g_CarTiGlb_path, CarTiGlb_CHANNEL_PRIORITY_COUNT, CarTiGlb_CHANNEL_PRIORITY));
    }
    return 0;
}


s32 FullFileDownload() {

    TestSeq_EnterProgrammingSession();

    u8 DID1[2] = { 0xED,0x20 };
    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    u8 DID2[2] = { 0xF1,0xAA };
    SID22_ReadDataByIdentifier(DID2);
    CheckPosResponse_SID22(DID2);

    TestSeq_DownloadSBL(0);

    u8 DID3[2] = { 0xF1,0xA2 };
    //SID22_ReadDataByIdentifier(DID3);
    //CheckNegResponse(0x31);

    if (0 != ActivateSBL()) {
        com.del_cyclic_messages();
        TestPostCondition();
        return Test_Result;
    }

    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    SID22_ReadDataByIdentifier(DID3);
    CheckPosResponse_SID22(DID3, 8);

    char keyvalue[20] = "APP0Path";
    TestReport_HTML_StepDescription("----Download APP----");
    vbfparser(keyvalue);
    for (int i = 0; i < APPFileNum; i++)
    {
        char StepDespriction[256];
        char filename[300] = "";
        vbfparser(keyvalue);
        app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
        filename[strlen(filename) - 9] = '\0';
        snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
        TestReport_HTML_StepDescription(StepDespriction);
        EraseAddress();
        FileDownload();
        TestReport_HTML_StepDescription("----Check Signature----");
        CheckSignature();
        keyvalue[3]++;
    }

    if (0 != CheckCompleteAndCompatible()) {
        strcpy(issuereason, "");
        TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
    }

    SID11_ECUReset(0x81);
    CheckNonResponse();
    TestWait(ECUResetTime);

    CheckCurrentSession(0x01);

    RX_SID19_ReportDTCByStatusMask(0x08);
    RX_CheckPosResponse_SID19_02(0xFF);
    return 0;
}

s32 FullFileDownload_VersionA() {

    TestSeq_EnterProgrammingSession();

    u8 DID1[2] = { 0xED,0x20 };
    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    u8 DID2[2] = { 0xF1,0xAA };
    SID22_ReadDataByIdentifier(DID2);
    CheckPosResponse_SID22(DID2);

    TestReport_HTML_StepDescription("----Download SBL----");
    char StepDespriction[256];
    //char filename[300] = "";

    char keyvalue[20] = "SBL_Path_VersionA";
    ReadPublicKey();
    SecurityUnlock(0x01);
    vbfparser(keyvalue);

    //获取文件名
    //app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
    //filename[strlen(filename) - 9] = '\0';
    snprintf(StepDespriction, sizeof(StepDespriction), "Download SBL_Filename is %s ", filename);
    TestReport_HTML_StepDescription(StepDespriction);

    FileDownload(0);
    CheckSignature();

    u8 DID3[2] = { 0xF1,0xA2 };
    //SID22_ReadDataByIdentifier(DID3);
    //CheckNegResponse(0x31);

    if (0 != ActivateSBL()) {
        com.del_cyclic_messages();
        TestPostCondition();
        return Test_Result;
    }

    SID22_ReadDataByIdentifier(DID1);
    CheckPosResponse_SID22(DID1);

    SID22_ReadDataByIdentifier(DID3);
    CheckPosResponse_SID22(DID3, 8);

    char keyvalue_APP[20] = "APP0Path_VersionA";
    TestReport_HTML_StepDescription("----Download APP----");
    vbfparser(keyvalue_APP);
    for (int i = 0; i < APPFileNum; i++)
    {
        char StepDespriction[256];
        char filename[300] = "";
        vbfparser(keyvalue_APP);
        app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
        filename[strlen(filename) - 9] = '\0';
        snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
        TestReport_HTML_StepDescription(StepDespriction);
        EraseAddress();
        FileDownload();
        TestReport_HTML_StepDescription("----Check Signature----");
        CheckSignature();
        keyvalue_APP[3]++;
    }

    if (0 != CheckCompleteAndCompatible()) {
        strcpy(issuereason, "");
        TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
    }

    SID11_ECUReset(0x81);
    CheckNonResponse();
    TestWait(ECUResetTime);

    CheckCurrentSession(0x01);

    RX_SID19_ReportDTCByStatusMask(0x08);
    RX_CheckPosResponse_SID19_02(0xFF);
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

double SetVoltageAndRead(double voltage, u8 flag = 1, u32 waitTime = 200) {
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

s32 CheckLINWakeUp() {
    int LINChannelCount = 0;
    app.get_lin_channel_count(&LINChannelCount);
    FirstNMMessageFlg = true;
    CheckLINMessageFlg = true;
    FirstNMMessageTime = 0;
    double deltaTime = 0;
    memset(FirstLINMessageTime, 0, sizeof(FirstLINMessageTime));
    NMWakeUp(1);
    AppWait(5000, "");
    CheckLINMessageFlg = false;
    for (int processIndex = 0; processIndex < LINChannelCount; processIndex++) {
        deltaTime = (FirstLINMessageTime[processIndex] - FirstNMMessageTime) / 1000.0;
        if (deltaTime > WakeUpTime) {
            TestInfoPrint("LIN CH%d WakeUp Time is %.2lf", deltaTime);

        }
        else if (deltaTime < 0) {
            TestInfoPrintFail("LIN CH%d No Message,Delta Time < 0");
        }
        else {
            if (0 == FirstLINMessageTime[processIndex]) {
                TestInfoPrintFail("LIN CH%d No Message");
            }
            else if (0 == FirstNMMessageTime) {
                TestInfoPrintFail("No WakeuUp Message");
            }
            else {
                TestInfoPrintPass("LIN CH%d WakeUp Time is %.2lf", deltaTime);
            }
        }
    }
    return 0;
}

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
                    strncpy(tempName, varName, 100);
                    if (0 == app.excel_get_cell_value(excelObj, i, row, 1, &varValue)) {
                        char tempValue[100] = { 0 };
                        strncpy(tempValue, varValue, 100);
                        size_t nameLen = strlen(tempName);
                        size_t valueLen = strlen(tempValue);

                        
                        if (nameLen > 0 && strncmp(tempName, "TestEngineer", nameLen) == 0) {
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
                            printf("字符串内容: '%s'\n", main_power::instance()->main_power_Serial);

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
                            snprintf(CarConfigBus_Temp, sizeof(CarConfigBus_Temp), "%s", tempValue);
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
                    varValue = nullptr;
                }
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
                }
            }
        }

        else if (strncmp(tempName, "CarConfig", 50) == 0) {
            app.log_text("Processing CarConfig sheet", lvlOK);

            s32 rowCount = 0, colCount = 0;
            if (0 != app.excel_get_cell_count(excelObj, i, &rowCount, &colCount)) {
                app.log_text("Failed to get cell count for CarConfig sheet", lvlError);
                continue;
            }
            ActualCarConfigCount = rowCount - 1;
            for (s32 row = 1; row < rowCount; row++) {
                char* tempValue = nullptr;
                char* tempPosition = nullptr;
                if (0 == app.excel_get_cell_value(excelObj, i, row, 0, &tempPosition) && tempPosition) {
                    int CCPosition;
                    CCPosition = atoi(tempPosition);
                    recordCarConfigDataList[row].CCPPsn = CCPosition;
                    if (0 == app.excel_get_cell_value(excelObj, i, row, 1, &tempValue)) {
                        int CCValue;
                        CCValue = strtol(tempValue,nullptr,0);
                        recordCarConfigDataList[row].CCPValue = CCValue;
                        printf("Get CCP[%d] = 0x%X,Record CCP[%d] = 0x%X", CCPosition, CCValue, recordCarConfigDataList[row].CCPPsn, recordCarConfigDataList[row].CCPValue);
                    }
                }
            }
        }
    }
    app.excel_unload(excelObj);
    return 0;
}
/*
s32 calculateNIST(char* configPath,const char* logPath) {
    char tempPath[500] = { 0 };
    char* runResult = nullptr;
    snprintf(tempPath,sizeof(tempPath),"%sToolbox\\NM\\common\\seed_randomness_test.py", configPath);
    app.execute_python_script(tempPath, logPath, true,false,&runResult);
    printf("Seed Randomness Result is %s", runResult);
    //if (strstr(runResult, "通过") != nullptr) {
    //    TestInfoPrintPass("NIST随机度检测通过");
    //}
    //else {
    //    TestInfoPrintFail("NIST随机度检测失败");
    //}
    return 0;
}
*/
s32 calculateNIST(char* configPath, const char* logPath) {
    char tempPath[500] = { 0 };
    char* runResult = nullptr;
    snprintf(tempPath, sizeof(tempPath), "%sToolbox\\NM\\common\\seed_randomness_test.py", configPath);

    app.execute_python_script(tempPath, logPath, true, false, &runResult);

    if (runResult != nullptr) {
        std::vector<std::string> logLines;
        std::string resultStr(runResult);
        std::stringstream ss(resultStr);
        std::string line;

        while (std::getline(ss, line, '\n')) {
            if (!line.empty() && line[line.size() - 1] == '\r') {
                line.erase(line.size() - 1);
            }
            logLines.push_back(line);
        }

        bool isPass = false;
        bool isFail = false;

        for (size_t i = 0; i < logLines.size(); i++) {
            TestInfoPrint("%s",logLines[i].c_str());

            if (logLines[i].find("[Pass]") != std::string::npos) {
                isPass = true;
            }
            if (logLines[i].find("[Fail]") != std::string::npos) {
                isFail = true;
            }
        }

        if (isPass) {
            TestInfoPrintPass("NIST 随机度检测通过");
        }
        else if (isFail) {
            TestInfoPrintFail("NIST 随机度检测失败");
        }
        else {
            TestInfoPrint("NIST 检测结束，未识别到明确结论标签");
        }

    }
    else {
        TestInfoPrintFail("NIST 检测异常：Python 脚本无返回内容");
    }

    return 0;
}
std::string findFileByExtension(const std::string& directory, const std::string& extension) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        printf("directory is %s", directory.c_str());
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            return entry.path().string();
        }
    }
    return "";
}

s32 ParserArxml() {
    char* s = nullptr;
    char tempPath[500] = { 0 };
    char tempPath2[500] = { 0 };
    char tempPath3[1000] = { 0 };
    app.get_configuration_file_path(&s);
    sprintf(tempPath, "%sToolbox\\GEELY3\\DB\\", s);
    //std::string path = "./"; // 当前文件夹
    std::string ext = ".arxml"; // 目标扩展名
    std::string fileName = findFileByExtension((std::string)(tempPath), ext);
    if (!fileName.empty()) {
        printf("找到文件: %s", fileName);
    }
    else {
        printf("未找到指定后缀的文件");
    }
    sprintf(tempPath2, "%sToolbox\\NM\\common\\NM_PNC.json", s);
    snprintf(tempPath3,sizeof(tempPath3), "\"\"%sTCOMParser.exe\" -t -v -i \"%s\" -o \"%s\"\"", tempPath,fileName.c_str(), tempPath2);
    printf("%s", tempPath3);
    int ret = system(tempPath3);
    printf("system return:%d", ret);
    return 0;
}

/**
 * @brief 校验循环覆盖存储合法性
 * @param arrayData 数组起始指针 (跳过Header部分，直接指向Time1)
 * @param recordCount 记录条数 (例如 20)
 * @param recordSize 每条记录的大小 (例如 7 或 9)
 * @return s32 0: 正常, -1: 逻辑错误
 */
s32 checkCircularStorage(pu8 arrayData, u32 recordCount, u32 recordSize) {
    u32 i;
    u32 discontinuityCount = 0; // 不连续（反转点）的次数
    u32 firstValidIndex = 0xFFFFFFFF;
    u32 lastValidIndex = 0xFFFFFFFF;

    // 1. 找到第一个和最后一个非空(非全FF)的索引
    for (i = 0; i < recordCount; i++) {
        // 检查这一组数据是否全为 0xFF (假设Time记录首字节非FF即为有效)
        if (arrayData[i * recordSize] != 0xFF) {
            if (firstValidIndex == 0xFFFFFFFF) firstValidIndex = i;
            lastValidIndex = i;
        }
    }

    // 如果全是 0xFF，说明还没开始写，是合法的
    if (firstValidIndex == 0xFFFFFFFF) return 0;

    // 2. 遍历有效区间，查找“突变点”
    // 注意：循环覆盖的特性决定了它在物理存储上最多只有一个下降沿
    for (i = 0; i < recordCount; i++) {
        u32 curr = i;
        u32 next = (i + 1) % recordCount;

        pu8 currData = &arrayData[curr * recordSize];
        pu8 nextData = &arrayData[next * recordSize];

        // 只在两个都是有效数据时进行比较
        if (currData[0] != 0xFF && nextData[0] != 0xFF) {
            // 如果 当前数据 > 下一个数据，说明找到了覆盖的交界处
            if (memcmp(currData, nextData, recordSize) > 0) {
                discontinuityCount++;
            }
        }
    }

    // 3. 结果判断：
    // 在循环存储中，递减的情况（从最大值跳回最小值）只能出现 0 次（未填满）或 1 次（已填满并覆盖）
    if (discontinuityCount <= 1) {
        return 0; // 合法
    }
    else {
        return -1; // 非法：出现了多次数值回跳，存储逻辑混乱
    }
}

/**
 * @brief 基于写入计数的循环覆盖校验
 * @param arrayData  数据区首地址（跳过Header）
 * @param totalCount 外部传入的总写入次数 (例如 24)
 * @param maxRecords 队列总容量 (例如 20)
 * @param recordSize 每条记录字节数 (7 或 9)
 * @return s32 0: 正常, -1: 逻辑异常
 */
s32 checkByCount(pu8 arrayData, u32 totalCount, u32 maxRecords, u32 recordSize) {
    if (totalCount == 0) return 0;

    u32 currIdx = (totalCount - 1) % maxRecords; 
    u32 nextIdx = totalCount % maxRecords;

    pu8 currData = &arrayData[currIdx * recordSize];
    pu8 nextData = &arrayData[nextIdx * recordSize];


    if (totalCount <= maxRecords) {

        if (currData[0] == 0xFF) return -1;

        if (totalCount < maxRecords) {
            if (nextData[0] != 0xFF) return -1;
        }
    }

    else {
        if (memcmp(currData, nextData, recordSize) <= 0) {
            return -1;
        }
    }

    return 0;
}

s32 recordCountAndData(u32 totalCount, pu8 arrayData, int recordCount, int recordSize, pu8 recordTime) {
    int modIndex = 0;
    printf("debug 1,totalCount %d;recordCount %d;recordSize %d;", totalCount, recordCount, recordSize);
    if (totalCount == 0) {
        modIndex = recordCount;
    }
    else {
        modIndex = totalCount % recordCount;
        if (modIndex == 0) modIndex = recordCount;
    }
    printf("debug 2,modIndex %d", modIndex);
    pu8 currData = &arrayData[(modIndex - 1) * recordSize];
    memcpy(recordTime, currData, recordSize);

    TestInfoPrint("Record Index: %d", modIndex);

    char hexBuf[128] = { 0 };
    for (int i = 0; i < recordSize && i < 32; i++) {
        sprintf(&hexBuf[i * 3], "%02X ", recordTime[i]);
    }
    TestInfoPrint("RecordTime: %s", hexBuf);

    return 0;
}

s32 ReadAndRecordC00B(pu32 currentCount, int recordCount, int recordSize, pu8 recordTime) {
    u8 DID_C00B[2] = { 0xC0,0x0B };
    u8 DID_Store[168] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C00B);
    if (0 == CheckPosResponse_SID22(DID_C00B)) {
        StoreArray(DID_Store, TpRxData, 168, 0, 0);
    }
    uint8_t* p = DID_Store + 4;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    if (currentCount != NULL) {
        *currentCount = Count;
    }
    TestInfoPrint("C00B Current Read Count is %d", *currentCount);
    return recordCountAndData(*currentCount, DID_Store + 8, recordCount, recordSize, recordTime);
}

s32 ReadAndRecordC00C(pu32 currentCount, int recordCount, int recordSize, pu8 recordTime) {
    u8 DID_C00C[2] = { 0xC0,0x0C };
    u8 DID_Store[272] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C00C);
    if (0 == CheckPosResponse_SID22(DID_C00C)) {
        StoreArray(DID_Store, TpRxData, 272, 0, 0);
    }
    uint8_t* p = DID_Store + 4;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    if (currentCount != NULL) {
        *currentCount = Count;
    }
    TestInfoPrint("C00C Current Read Count is %d", *currentCount);
    return recordCountAndData(*currentCount, DID_Store + 12, recordCount, recordSize, recordTime);
}


s32 ReadAndRecordC00E(pu32 currentCount, int recordCount, int recordSize, pu8 recordTime) {
    u8 DID_C00E[2] = { 0xC0,0x0E };
    u8 DID_Store[148] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C00E);
    if (0 == CheckPosResponse_SID22(DID_C00E)) {
        StoreArray(DID_Store, TpRxData, 148, 0, 0);
    }
    uint8_t* p = DID_Store + 4;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    if (currentCount != NULL) {
        *currentCount = Count;
    }
    TestInfoPrint("C00E Current Read Pass Count is %d", *currentCount);
    return recordCountAndData(*currentCount, DID_Store + 8, recordCount, recordSize, recordTime);
}

s32 ReadAndRecordC00F(pu32 currentCount, int recordCount, int recordSize, pu8 recordTime) {
    u8 DID_C00F[2] = { 0xC0,0x0F };
    u8 DID_Store[188] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C00F);
    if (0 == CheckPosResponse_SID22(DID_C00F)) {
        StoreArray(DID_Store, TpRxData, 188, 0, 0);
    }
    uint8_t* p = DID_Store + 4;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    if (currentCount != NULL) {
        *currentCount = Count;
    }
    TestInfoPrint("C00F Current Read Count is %d", *currentCount);
    return recordCountAndData(*currentCount, DID_Store + 8, recordCount, recordSize, recordTime);
}


s32 PassReadAndRecordC00A(pu32 currentCount,int recordCount, int recordSize, pu8 recordTime) {
    u8 DID_C00A[2] = { 0xC0,0x0A };
    u8 DID_Store[192] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C00A);
    if (0 == CheckPosResponse_SID22(DID_C00A)) {
        StoreArray(DID_Store, TpRxData, 192, 0, 0);
    }
    uint8_t* p = DID_Store + 4;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    if (currentCount != NULL) {
        *currentCount = Count;
    }
    //u32 count = *(u32*)(DID_Store + 4);
    TestInfoPrint("Current Read Pass Count is %d", *currentCount);
    return recordCountAndData(*currentCount, DID_Store + 12,recordCount, recordSize, recordTime);
}

s32 FailReadAndRecordC00A(pu32 currentCount, int recordCount, int recordSize, pu8 recordTime) {
    u8 DID_C00A[2] = { 0xC0,0x0A };
    u8 DID_Store[192] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C00A);
    if (0 == CheckPosResponse_SID22(DID_C00A)) {
        StoreArray(DID_Store, TpRxData, 192, 0, 0);
    }
    uint8_t* p = DID_Store + 8;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    if (currentCount != NULL) {
        *currentCount = Count;
    }
    TestInfoPrint("Current Read Fail Count is %d", *currentCount);
    return recordCountAndData(*currentCount, DID_Store + 102, recordCount, recordSize, recordTime);
}

s32 PassReadAndRecordC012(pu32 currentCount, int recordCount, int recordSize, pu8 recordTime) {
    u8 DID_C012[2] = { 0xC0,0x12 };
    u8 DID_Store[312] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C012);
    if (0 == CheckPosResponse_SID22(DID_C012)) {
        StoreArray(DID_Store, TpRxData, 312, 0, 0);
    }

    uint8_t* p = DID_Store + 4;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    if (currentCount != NULL) {
        *currentCount = Count;
    }

    TestInfoPrint("Current Read Pass Count is %u", *currentCount);

    return recordCountAndData(*currentCount, DID_Store + 12, recordCount, recordSize, recordTime);
}

s32 FailReadAndRecordC012(pu32 currentCount, int recordCount, int recordSize, pu8 recordTime) {
    u8 DID_C012[2] = { 0xC0,0x12 };
    u8 DID_Store[312] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C012);
    if (0 == CheckPosResponse_SID22(DID_C012)) {
        StoreArray(DID_Store, TpRxData, 312, 0, 0);
    }
    uint8_t* p = DID_Store + 8;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    if (currentCount != NULL) {
        *currentCount = Count;
    }
    TestInfoPrint("Current Read Fail Count is %d", *currentCount);
    return recordCountAndData(*currentCount, DID_Store + 162, recordCount, recordSize, recordTime);
}


s32 checkC00A(pu8 arrayData,bool isPass ) {
    //传入arrayData需筛除Header(62 C0 XX XX)，只保留Time Data
    // 
    // C012共312Bytes
    // 
    // 3Bytes 62 C0 0F
    // 1Byte Type
    // 4Bytes Count
    // 9Bytes Time1
    // 9Bytes Time2
    // 9Bytes Time3
    // .....
    // .....
    // 9Bytes Time20

    //return checkCircularStorage(arrayData, 20, 9);

    int countIndex = 0, dataIndex = 0;
    if (isPass) {
        countIndex = 4;
        dataIndex = 12;
    }
    else {
        countIndex = 8;
        dataIndex = 102;
    }
    uint8_t* p = arrayData + countIndex;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);
    TestInfoPrint("Current Read Count is %d", Count);
    return checkByCount(arrayData + dataIndex, Count, 10, 9);
}

s32 checkC00B(pu8 arrayData,int recordCount,int recordSize) {
    //C00E共148Bytes
    // 3Bytes 62 C0 0E
    // 1Byte Type
    // 4Bytes Count
    // 7Bytes Time1
    // 7Bytes Time2
    // 7Bytes Time3
    // .....
    // .....
    // 7Bytes Time20


    uint8_t* p = arrayData + 4;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    return checkByCount(arrayData + 8, Count, recordCount, recordSize);
}

s32 checkC00C(pu8 arrayData, int recordCount, int recordSize) {
    //C00E共148Bytes
    // 3Bytes 62 C0 0E
    // 1Byte Type
    // 4Bytes Count
    // 7Bytes Time1
    // 7Bytes Time2
    // 7Bytes Time3
    // .....
    // .....
    // 7Bytes Time20

    //return checkCircularStorage(arrayData, 20, 7);

    uint8_t* p = arrayData + 4;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);


    return checkByCount(arrayData + 12, Count, recordCount, recordSize);
}


s32 checkC00E(int recordCount, int recordSize) {
    //C00E共148Bytes
    // 3Bytes 62 C0 0E
    // 1Byte Type
    // 4Bytes Count
    // 7Bytes Time1
    // 7Bytes Time2
    // 7Bytes Time3
    // .....
    // .....
    // 7Bytes Time20
    
    //return checkCircularStorage(arrayData, 20, 7);

    // 从 arrayData 中解析出外部 Count (4字节)
    u8 DID_C00E[2] = { 0xC0,0x0E };
    u8 DID_Store[148] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C00E);
    if (0 == CheckPosResponse_SID22(DID_C00E)) {
        StoreArray(DID_Store, TpRxData, 148, 0, 0);
    }

    uint8_t* p = DID_Store + 4;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    return checkByCount(DID_Store + 8, Count, recordCount, recordSize);
}

s32 checkC00F(int recordCount, int recordSize) {
    //传入arrayData需筛除Header，只保留Time Data
    //C00F共188Bytes
    // 3Bytes 62 C0 0F
    // 1Byte Type
    // 4Bytes Count
    // 9Bytes Time1
    // 9Bytes Time2
    // 9Bytes Time3
    // .....
    // .....
    // 9Bytes Time20
    u8 DID_C00F[2] = { 0xC0,0x0F };
    u8 DID_Store[188] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C00F);
    if (0 == CheckPosResponse_SID22(DID_C00F)) {
        StoreArray(DID_Store, TpRxData, 188, 0, 0);
    }
    uint8_t* p = DID_Store + 4;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);

    return checkByCount(DID_Store + 8, Count, recordCount, recordSize);
}

s32 checkC012(bool isPass) {
    //传入arrayData需筛除Header(62 C0 XX XX)，只保留Time Data
    // 
    // C012共312Bytes
    // 
    // 3Bytes 62 C0 0F
    // 1Byte Type
    // 4Bytes Count
    // 9Bytes Time1
    // 9Bytes Time2
    // 9Bytes Time3
    // .....
    // .....
    // 9Bytes Time20

    //return checkCircularStorage(arrayData, 20, 9);
    u8 DID_C012[2] = { 0xC0,0x12 };
    u8 DID_Store[312] = { 0x0 };

    SID22_ReadDataByIdentifier(DID_C012);
    if (0 == CheckPosResponse_SID22(DID_C012)) {
        StoreArray(DID_Store, TpRxData, 312, 0, 0);
    }
    int countIndex = 0, dataIndex = 0;
    if (isPass) {
        countIndex = 4;
        dataIndex = 12;
    }
    else {
        countIndex = 8;
        dataIndex = 162;
    }
    u32 count = *(u32*)(DID_Store + countIndex);
    uint8_t* p = DID_Store + countIndex;

    u32 Count = ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        ((uint32_t)p[3] << 0);
    TestInfoPrint("Current Read Count is %d", Count);
    return checkByCount(DID_Store + dataIndex, Count, 10, 15);
}

bool isAllFF(pu8 data, int size) {
    for (int i = 0; i < size; i++) {
        if (data[i] != 0xFF) return false;
    }
    return true;
}

s32 checkInCrease(u32 currentCount, u32 lastCount, pu8 currentRecordTime, pu8 lastRecordTime, int recordSize, int deltaValue = 1) {
    if (deltaValue == (currentCount - lastCount)) {
        TestInfoPrintPass("Count is InCreasing: Curr=%u  Last=%u", currentCount, lastCount);
    }
    else {
        TestInfoPrintFail("Count Error: Curr=%u  Last=%u  Expected Delta=%d", currentCount, lastCount, deltaValue);
    }

    bool lastIsInvalid = isAllFF(lastRecordTime, recordSize);

    if (lastIsInvalid) {
        TestInfoPrintPass("Timestamp: Last data is All 0xFF (Initial State)  Skipping comparison.");
        return 0;
    }
    int result = memcmp(currentRecordTime, lastRecordTime, recordSize);

    if (result > 0) {
        if (deltaValue > 0) {
            TestInfoPrintPass("Timestamp is InCreasing (Normal)");
        }
        else {
            //TestInfoPrintFail("Timestamp increased but Count did not");
            TestInfoPrintFail("Timestamp increased");
        }
        return 0;
    }
    else if (result < 0) {
        TestInfoPrintFail("Timestamp is Not Correct (Decreased)");
        return -1;
    }
    else {
        if (0 == deltaValue) {
            TestInfoPrintPass("Timestamp is Unchanged (Expected)");
            return 0;
        }
        else {
            //TestInfoPrintFail("Timestamp is Unchanged but Count increased");
            TestInfoPrintFail("Timestamp is Unchanged");
            return -1;
        }
    }
}

s32 CheckSoftVersion() {

    TestReport_HTML_StepDescription("切换至编程会话");

    SID10_SessionControl(ProgrammingSession);
    CheckPosResponse_SID10(ProgrammingSession);

    CheckCurrentSession(ProgrammingSession);

    TestReport_HTML_StepDescription("检查Boot版本号");

    u8 DID_D94B[2] = { 0xD9, 0x4B };
    u8 D94B_Version[64] = { 0 };

    SID22_ReadDataByIdentifier(DID_D94B);
    if (0 == CheckPosResponse_SID22(DID_D94B)) {
        u32 actualLen = TpRxLength - 3;

        if (actualLen >= sizeof(D94B_Version)) {
            actualLen = sizeof(D94B_Version) - 1;
        }

        if (actualLen > 0) {
            StoreArray(D94B_Version, TpRxData, actualLen, 3, 0);
            D94B_Version[actualLen] = '\0';

            snprintf(StepDespriction, sizeof(StepDespriction), "当前版本号为：%s", D94B_Version);
            TestReport_HTML_StepDescription(StepDespriction);
        }
        else {
            TestInfoPrintFail("版本号数据为空");
        }
    }
    else {
        TestInfoPrintFail("版本号读取失败");
    }

    TestReport_HTML_StepDescription("读取ECU Core Assembly Part Number");
    u8 DID_F1AA[2] = { 0xF1, 0xAA };
    u8 F1AA_Version[64] = { 0 };
    SID22_ReadDataByIdentifier(DID_F1AA);
    CheckPosResponse_SID22(DID_F1AA);

    TestReport_HTML_StepDescription("读取PBL Software Part Number");
    u8 DID_F1A5[2] = { 0xF1, 0xA5 };
    u8 F1A5_Version[64] = { 0 };
    SID22_ReadDataByIdentifier(DID_F1A5);
    CheckPosResponse_SID22(DID_F1A5);

    TestReport_HTML_StepDescription("切换至默认会话");

    SID10_SessionControl(DefaultSession);
    CheckPosResponse_SID10(DefaultSession);

    CheckCurrentSession(DefaultSession);

    TestReport_HTML_StepDescription("检查APP版本号");

    SID22_ReadDataByIdentifier(DID_D94B);
    if (0 == CheckPosResponse_SID22(DID_D94B)) {
        u32 actualLen = TpRxLength - 3;

        if (actualLen >= sizeof(D94B_Version)) {
            actualLen = sizeof(D94B_Version) - 1;
        }

        if (actualLen > 0) {
            StoreArray(D94B_Version, TpRxData, actualLen, 3, 0);
            D94B_Version[actualLen] = '\0';

            snprintf(StepDespriction, sizeof(StepDespriction), "当前版本号为：%s", D94B_Version);
            TestReport_HTML_StepDescription(StepDespriction);
        }
        else {
            TestInfoPrintFail("APP版本号数据为空");
        }
    }
    else {
        TestInfoPrintFail("版本号读取失败");
    }
    return 0;
}


// ========================================================================
// Helper: CAN 网络拓扑配置
// ========================================================================
struct CANNetworkTopology {
    std::map<std::string, int> nameToChannel;
    std::map<int, std::string> channelToName;
    std::vector<int> channelsToTest;
};

static CANNetworkTopology GetCANNetworkTopology() {
    CANNetworkTopology topo;
    if (gCEMFlg) {
        topo.nameToChannel = {
            {"ZCU_CANFD2", CH2}, {"ZCU_CANFD1", CH3}, {"ChassisCANFD2", CH4},
            {"ZCUD_CAN1", CH5}, {"PropulsionCANFD", CH6}, {"ChassisCANFD1", CH7},
            {"ZCUD_CANFD3", CH8}, {"ZCUD_CANFD4", CH9}, {"BodyExposedCAN", CH10}
        };
        topo.channelToName = {
            {CH2, "ZCU_CANFD2"}, {CH3, "ZCU_CANFD1"}, {CH4, "ChassisCANFD2"},
            {CH5, "ZCUD_CAN1"}, {CH6, "PropulsionCANFD"}, {CH7, "ChassisCANFD1"},
            {CH8, "ZCUD_CANFD3"}, {CH9, "ZCUD_CANFD4"}, {CH10, "BodyExposedCAN"}
        };
        topo.channelsToTest = { CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9, CH10 };
    }
    else {
        topo.nameToChannel = {
            {"ZCU_CANFD2", CH2}, {"ZCU_CANFD1", CH3},
            {"ZCUP_CANFD5", CH4}, {"ZCUP_CAN1", CH5}
        };
        topo.channelToName = {
            {CH2, "ZCU_CANFD2"}, {CH3, "ZCU_CANFD1"},
            {CH4, "ZCUP_CANFD5"}, {CH5, "ZCUP_CAN1"}
        };
        topo.channelsToTest = { CH2, CH3, CH4, CH5 };
    }
    return topo;
}

// ========================================================================
// 回调收集报文信息的全局变量（原在 TestCase_CAN_NM.h，移至此处以解决 include order 问题）
// ========================================================================
#include <vector>
static std::unordered_map<int, std::unordered_map<u64, std::vector<double>>> g_receivedTimestampsPerChannel;
static std::unordered_map<int, std::set<u64>> g_receivedIdsPerChannel;
static std::unordered_map<int, std::vector<double>> g_errorFrameTimestampsPerChannel;
static bool g_isCollecting = false;
static native_int g_checkObj;

char StepDesc[1000] = { 0 };

void __stdcall OnCANMessageForCheck(const pnative_int pobj, const PCANFD ACANFD)
{
    if (!g_isCollecting) return;

    TCANFD frame = *ACANFD;
    if (frame.get_is_tx()) return;

    int ch = frame.FIdxChn;
    double ts = frame.FTimeUs / 1000.0;

    if (frame.get_is_err()) {
        g_errorFrameTimestampsPerChannel[ch].push_back(ts);
        return;
    }

    u64 id = frame.FIdentifier;
    if (id >= 0x500) return;

    g_receivedTimestampsPerChannel[ch][id].push_back(ts);
    g_receivedIdsPerChannel[ch].insert(id);
}

inline void REG_Check()
{
    com.register_event_canfd(&g_checkObj, OnCANMessageForCheck);
}

inline void UNREG_Check()
{
    com.unregister_event_canfd(&g_checkObj, OnCANMessageForCheck);
}

// ========================================================================
// Helper: 期望报文信息（从数据库解析）
// ========================================================================
struct CANMessageExpectations {
    std::unordered_map<int, std::unordered_map<s32, u16>> cyclesPerChannel;   // ch -> (ID -> cycle ms)
    std::unordered_map<int, std::set<u64>> eventIdsPerChannel;
    std::unordered_map<int, std::set<u64>> periodicIdsPerChannel;
    std::unordered_map<int, std::set<u64>> allIdsPerChannel;
};

static CANMessageExpectations ParseDatabaseExpectations(const CANNetworkTopology& topo) {
    CANMessageExpectations exp;
    int dbCount = 0;
    if (0 != app.db_get_can_database_count(&dbCount)) return exp;

    log_ok("CAN database count = %d", dbCount);

    for (int dbIdx = 0; dbIdx < dbCount; dbIdx++) {
        TDBProperties db;
        db.FDBIndex = dbIdx;
        if (0 != app.db_get_can_database_properties_by_index(&db)) continue;

        auto it = topo.nameToChannel.find(db.FName);
        if (it == topo.nameToChannel.end()) {
            printf("跳过未配置的网络: %s\n", db.FName);
            continue;
        }
        int ch = it->second;
        log_ok("[%d] Database: %s (CH%d)", dbIdx + 1, db.FName, ch + 1);

        for (int ecuIdx = 0; ecuIdx < db.FECUCount; ecuIdx++) {
            TDBECUProperties ecu;
            ecu.FDBIndex = dbIdx;
            ecu.FECUIndex = ecuIdx;
            if (0 != app.db_get_can_ecu_properties_by_index(&ecu)) continue;

            bool isTarget = gCEMFlg
                ? (strstr(ecu.FName, "ZCUD") != nullptr)
                : (strstr(ecu.FName, "ZCUP") != nullptr);
            if (!isTarget) continue;

            for (int msgIdx = 0; msgIdx < ecu.FTxFrameCount; msgIdx++) {
                TDBFrameProperties msg;
                msg.FDBIndex = dbIdx;
                msg.FECUIndex = ecuIdx;
                msg.FFrameIndex = msgIdx;
                msg.FIsTx = 1;
                if (0 != app.db_get_can_frame_properties_by_index(&msg)) continue;

                u16 cycle = 0;
                if (g_CurrentDBType == DB_TYPE_ARXML) {
                    TDBPDUProperties pdu;
                    pdu.FDBIndex = dbIdx; pdu.FECUIndex = ecuIdx;
                    pdu.FFrameIndex = msgIdx; pdu.FPDUIndex = 0; pdu.FIsTx = 1;
                    if (0 != app.db_get_can_pdu_properties_by_index(&pdu)) continue;
                    cycle = pdu.FCycleTimeMs;
                }
                else {
                    cycle = msg.FCycleTimeMs;
                }

                if (msg.FCANIdentifier >= 0x500 || msg.FCANIdentifier < 0) continue;

                if (cycle > 0 && cycle < 10000) {
                    exp.cyclesPerChannel[ch][msg.FCANIdentifier] = cycle;
                    exp.periodicIdsPerChannel[ch].insert(msg.FCANIdentifier);
                    exp.allIdsPerChannel[ch].insert(msg.FCANIdentifier);
                    log_ok("    Added Periodic TX Msg: ID=0x%03X, Cycle=%u ms", msg.FCANIdentifier, cycle);
                }
                else if (cycle == 0) {
                    exp.eventIdsPerChannel[ch].insert(msg.FCANIdentifier);
                    exp.allIdsPerChannel[ch].insert(msg.FCANIdentifier);
                    log_ok("    Added Event TX Msg: ID=0x%03X (Cycle=0)", msg.FCANIdentifier);
                }
            }
        }
        log_ok("网络 %s 期望的周期报文数: %zu, 事件报文数: %zu",
            db.FName,
            exp.periodicIdsPerChannel[ch].size(),
            exp.eventIdsPerChannel[ch].size());
    }
    return exp;
}

// ========================================================================
// Helper: 收集总线报文
// ========================================================================
static void CollectCANMessages(int waitMs) {
    g_receivedTimestampsPerChannel.clear();
    g_receivedIdsPerChannel.clear();
    g_errorFrameTimestampsPerChannel.clear();
    g_isCollecting = true;

    //com.tsfifo_enable_receive_error_frames();
    REG_Check();
    printf("开始通过回调收集报文信息（已开启错误帧检测）...\n");
    TestWait(waitMs);
    UNREG_Check();
    //com.tsfifo_disable_receive_error_frames();

    g_isCollecting = false;
    printf("停止收集报文信息。\n");
}

// ========================================================================
// Helper: 单通道 ID 一致性检查
// ========================================================================
static bool CheckIDConsistency(int ch, const std::string& chName,
    const CANMessageExpectations& exp,
    const std::set<u64>& receivedIds,
    std::vector<u64>& outMissingIds) {
    // 传入 const& 不可用 operator[]，用 at()
    const auto& expPeriodic = exp.periodicIdsPerChannel.at(ch);
    const auto& expAll = exp.allIdsPerChannel.at(ch);

    snprintf(StepDesc, sizeof(StepDesc), "ID一致性检查 - 期望周期报文: %zu 条, 实际接收到: %zu 条",
        expPeriodic.size(), receivedIds.size());
    TestReport_HTML_StepDescription(StepDesc);

    // 缺的周期报文
    outMissingIds.clear();
    for (u64 id : expPeriodic) {
        if (receivedIds.find(id) == receivedIds.end())
            outMissingIds.push_back(id);
    }

    // 多余的报文
    std::vector<u64> extraIds;
    for (u64 id : receivedIds) {
        if (expAll.find(id) == expAll.end())
            extraIds.push_back(id);
    }

    bool hasError = false;

    if (!outMissingIds.empty()) {
        std::string s = "缺少的周期报文ID: ";
        char buf[16];
        for (u64 id : outMissingIds) {
            snprintf(buf, sizeof(buf), "0x%03X ", (u32)id);
            s += buf;
        }
        log_nok("[%s] %s", chName.c_str(), s.c_str());
        TestReport_HTML_StepDescription(s.c_str(), Fail);
        hasError = true;
    }

    if (!extraIds.empty()) {
        const auto& expEvent = exp.eventIdsPerChannel.at(ch);
        std::vector<u64> trueExtra, eventAppeared;
        for (u64 id : extraIds) {
            if (expEvent.find(id) != expEvent.end())
                eventAppeared.push_back(id);
            else
                trueExtra.push_back(id);
        }

        if (!eventAppeared.empty()) {
            char buf[512] = "检测到的事件报文ID: ";
            char tmp[16];
            for (u64 id : eventAppeared) {
                snprintf(tmp, sizeof(tmp), "0x%X ", (u32)id);
                strcat(buf, tmp);
            }
            log_hint("[%s] %s", chName.c_str(), buf);
            TestReport_HTML_StepDescription(buf);
        }
        if (!trueExtra.empty()) {
            char buf[512] = "多发的未知报文ID: ";
            char tmp[16];
            for (u64 id : trueExtra) {
                snprintf(tmp, sizeof(tmp), "0x%X ", (u32)id);
                strcat(buf, tmp);
            }
            log_nok("[%s] %s", chName.c_str(), buf);
            TestReport_HTML_StepDescription(buf, Fail);
            hasError = true;
        }
    }

    if (!hasError) {
        log_ok("[%s] ID检查通过：期望 %zu 条周期报文，实际接收到 %zu 条",
            chName.c_str(), expPeriodic.size(), receivedIds.size());
        snprintf(StepDesc, sizeof(StepDesc), "ID检查通过：期望 %zu 条周期报文，实际 %zu 条",
            expPeriodic.size(), receivedIds.size());
        TestReport_HTML_StepDescription(StepDesc, Pass);
    }
    return hasError;
}

// ========================================================================
// Helper: 单通道周期检查
// ========================================================================
static bool CheckCycleTimes(int ch, const std::string& chName,
    const CANMessageExpectations& exp,
    const std::unordered_map<u64, std::vector<double>>& receivedTimestamps) {
    constexpr double TOLERANCE = 0.1;
    bool hasError = false;

    for (const auto& [id, expectedCycleMs] : exp.cyclesPerChannel.at(ch)) {
        auto tsIt = receivedTimestamps.find(id);
        if (tsIt == receivedTimestamps.end()) continue;

        const auto& rawTs = tsIt->second;
        if (rawTs.size() < 2) {
            log_hint("CH%d ID 0x%X: 帧数不足(%zu帧)", ch, (u32)id, rawTs.size());
            continue;
        }

        std::vector<double> sorted = rawTs;
        std::sort(sorted.begin(), sorted.end());

        std::vector<double> diffs;
        for (size_t i = 1; i < sorted.size(); i++) {
            double d = sorted[i] - sorted[i - 1];
            if (d > 0) diffs.push_back(d);
        }
        if (diffs.empty()) continue;

        double avg = std::accumulate(diffs.begin(), diffs.end(), 0.0) / diffs.size();
        double lower = expectedCycleMs * (1.0 - TOLERANCE);
        double upper = expectedCycleMs * (1.0 + TOLERANCE);
        double minCycle = *std::min_element(diffs.begin(), diffs.end());
        double maxCycle = *std::max_element(diffs.begin(), diffs.end());

        char desc[256];
        snprintf(desc, sizeof(desc), "ID 0x%X: 最小=%.1fms 最大=%.1fms, 期望=%.1fms, 平均=%.1fms",
            (u32)id, minCycle, maxCycle, (double)expectedCycleMs, avg);

        if (avg >= lower && avg <= upper) {
            log_ok("CH%d %s: 周期正常", ch, desc);
            snprintf(StepDesc, sizeof(StepDesc), "%s", desc);
            TestReport_HTML_StepDescription(StepDesc, Pass);
        }
        else if (id == 102 || id == 133 || id == 614) {
            log_warning("CH%d %s: 混合型报文", ch, desc);
            snprintf(StepDesc, sizeof(StepDesc), "%s", desc);
            TestReport_HTML_StepDescription(StepDesc, Warning);
        }
        else {
            log_nok("CH%d %s: 周期超出容差[%.1f-%.1f]", ch, desc, lower, upper);
            snprintf(StepDesc, sizeof(StepDesc), "%s", desc);
            TestReport_HTML_StepDescription(StepDesc, Fail);
            hasError = true;
        }
    }
    return hasError;
}


s32 FindCarConfigPath() {
    g_CarConfig1_found = (0 == FindSignalPath(DUTName, "VehCfgPrm_UB", &g_CarConfig1_path, CarConfig_CHANNEL_PRIORITY_COUNT, CarConfig_CHANNEL_PRIORITY));
    g_CarConfig2_found = (0 == FindSignalPath(DUTName, "VehCfgPrmExt_UB", &g_CarConfig2_path, CarConfig_CHANNEL_PRIORITY_COUNT, CarConfig_CHANNEL_PRIORITY));
    g_CarConfig3_found = (0 == FindSignalPath(DUTName, "VehCfgPrmExt2_UB", &g_CarConfig3_path, CarConfig_CHANNEL_PRIORITY_COUNT, CarConfig_CHANNEL_PRIORITY));
    return 0;
}


bool IsMatchingArxmlSignal(const char* currentSgnName, const char* baseName) {
    if (g_CurrentDBType == DB_TYPE_DBC) {
        return strcmp(currentSgnName, baseName) == 0;
    }
    std::string pattern = "^is" + std::string(baseName) + "(_[0-9]+)?$";
    try {
        std::regex re(pattern);
        return std::regex_match(currentSgnName, re);
    }
    catch (...) { return false; }
}

std::string ExtractArxmlSuffix(const char* fullSignalName) {
    std::string s(fullSignalName);
    size_t lastUnderscore = s.find_last_of('_');
    if (lastUnderscore != std::string::npos && lastUnderscore < s.length() - 1) {
        std::string suffix = s.substr(lastUnderscore + 1);
        if (std::all_of(suffix.begin(), suffix.end(), ::isdigit)) {
            return suffix;
        }
    }
    return "";
}

std::string ExtractGroupName(const char* fullSignalName, EnumDBType type) {
    std::string s(fullSignalName);
    if (type == DB_TYPE_DBC) return s;

    if (s.find("is") == 0) {
        s = s.substr(2);
    }

    size_t lastUnderscore = s.find_last_of('_');
    if (lastUnderscore != std::string::npos) {
        std::string suffix = s.substr(lastUnderscore + 1);
        if (!suffix.empty() && std::all_of(suffix.begin(), suffix.end(), ::isdigit)) {
            s = s.substr(0, lastUnderscore);
        }
    }
    return s;
}

std::string ExtractUBGroupName(const char* fullSignalName, EnumDBType type) {
    std::string s(fullSignalName);
    if (type == DB_TYPE_DBC) return s;

    if (s.find("is") == 0 || s.find("ig") == 0) {
        s = s.substr(2);
    }
    size_t grpUbPos = s.find("_Grp_UB");
    if (grpUbPos != std::string::npos) {
        size_t underscoreBeforeGrp = s.rfind('_', grpUbPos - 1);
        if (underscoreBeforeGrp != std::string::npos && underscoreBeforeGrp < grpUbPos) {
            std::string numPart = s.substr(underscoreBeforeGrp + 1, grpUbPos - underscoreBeforeGrp - 1);
            if (!numPart.empty() && std::all_of(numPart.begin(), numPart.end(), ::isdigit)) {
                s = s.substr(0, underscoreBeforeGrp);
            }
            else {
                s = s.substr(0, grpUbPos);
            }
        }
        else {
            s = s.substr(0, grpUbPos);
        }
    }
    size_t lastUnderscore = s.find_last_of('_');
    if (lastUnderscore != std::string::npos) {
        std::string suffix = s.substr(lastUnderscore + 1);
        if (!suffix.empty() && std::all_of(suffix.begin(), suffix.end(), ::isdigit)) {
            s = s.substr(0, lastUnderscore);
        }
    }

    return s;
}

s32 getCarConfigInfo() {
    s32 i, j, k, m, n;
    //CarConfigSignalGroup.clear();

    if (0 != app.db_get_can_database_count(&n)) return -1;
    log_ok("CAN database count = %d", n);

    for (i = 0; i < n; i++) {
        TDBProperties db; db.FDBIndex = i;
        if (0 != app.db_get_can_database_properties_by_index(&db)) continue;
        log_ok("Checking DB: %s, target is: %s", db.FName, CarConfigBus_Temp);
        if (0 != strcmp(db.FName, CarConfigBus_Temp)) continue;
        printf("Find Target DB");

        for (j = 0; j < db.FECUCount; j++) {
            TDBECUProperties ecu; ecu.FDBIndex = i; ecu.FECUIndex = j;
            if (0 != app.db_get_can_ecu_properties_by_index(&ecu)) continue;

            for (k = 0; k < ecu.FTxFrameCount; k++) {
                TDBFrameProperties msg; msg.FDBIndex = i; msg.FECUIndex = j; msg.FFrameIndex = k; msg.FIsTx = 1;
                if (0 != app.db_get_can_frame_properties_by_index(&msg)) continue;

                int pduCount = (g_CurrentDBType == DB_TYPE_ARXML) ? 1 : 1; // 如有多 PDU 可改为 msg.FPDUCount

                for (int pIdx = 0; pIdx < pduCount; pIdx++) {
                    int signalCount = msg.FSignalCount;

                    TDBPDUProperties pdu;
                    if (g_CurrentDBType == DB_TYPE_ARXML) {
                        pdu.FDBIndex = i; pdu.FECUIndex = j; pdu.FFrameIndex = k; pdu.FPDUIndex = pIdx; pdu.FIsTx = 1;
                        if (0 != app.db_get_can_pdu_properties_by_index(&pdu)) continue;
                        signalCount = pdu.FSignalCount;
                    }

                    for (m = 0; m < signalCount; m++) {
                        TDBSignalProperties sgn;
                        sgn.FDBIndex = i; sgn.FECUIndex = j; sgn.FFrameIndex = k; sgn.FSignalIndex = m; sgn.FIsTx = 1;
                        if (g_CurrentDBType == DB_TYPE_ARXML) sgn.FPDUIndex = pIdx;
                        if (0 != app.db_get_can_signal_properties_by_index(&sgn)) continue;

                        char tempSgnIDName[128] = { 0 };
                        char tempSgnByteName[128] = { 0 };

                        for (int groupIndex = 0; groupIndex < CarConfigSignalGroup.size(); groupIndex++) {
                            const char* baseGroup = CarConfigSignalGroup[groupIndex].c_str();
                            snprintf(tempSgnIDName, sizeof(tempSgnIDName), "%sBlkIDBytePosn1", baseGroup);
                            snprintf(tempSgnByteName, sizeof(tempSgnByteName), "%sCCPBytePosn", baseGroup);

                            if ((CarConfigSignalGroup.size() - 1) == groupIndex && g_CCPIsCanFD) {
                                bool isUB = false;
                                if (g_CurrentDBType == DB_TYPE_DBC) {
                                    char expectedUB[128]; snprintf(expectedUB, sizeof(expectedUB), "%s_UB", baseGroup);
                                    isUB = (0 == strcmp(sgn.FName, expectedUB));
                                }
                                else {
                                    std::string extracted = ExtractUBGroupName(sgn.FName, g_CurrentDBType);

                                    size_t pos = extracted.find(baseGroup);
                                    isUB = (extracted == baseGroup);
                                }

                                if (isUB) {
                                    carConfigData[groupIndex].DBName = std::string(db.FName);
                                    carConfigData[groupIndex].ECUName = std::string(ecu.FName);
                                    carConfigData[groupIndex].FrameName = std::string(msg.FName);
                                    carConfigData[groupIndex].FFrameIdentifier = msg.FCANIdentifier;
                                    carConfigData[groupIndex].UBbit = sgn.FCANSignal.FStartBit;
                                    printf("DBName is %s, ECUName is %s, FrameName is %s, FFrameIdentifier is 0x%X, UBbit is %d (UB:%s)\n",
                                        carConfigData[groupIndex].DBName.c_str(), carConfigData[groupIndex].ECUName.c_str(),
                                        carConfigData[groupIndex].FrameName.c_str(), carConfigData[groupIndex].FFrameIdentifier,
                                        carConfigData[groupIndex].UBbit, sgn.FName);
                                }

                                for (u8 listOfNodvIndex = 1; listOfNodvIndex <= 8; listOfNodvIndex++) {
                                    char expectedBase[128];
                                    snprintf(expectedBase, sizeof(expectedBase), "%s%d", baseGroup, listOfNodvIndex);

                                    if (IsMatchingArxmlSignal(sgn.FName, expectedBase)) {
                                        carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].StartBit = sgn.FCANSignal.FStartBit;
                                        carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].Length = sgn.FCANSignal.FLength;
                                        carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].SignalName = std::string(sgn.FName);
                                        printf("Signal is %s, Length is %d, startBit is %d\n",
                                            carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].SignalName.c_str(),
                                            carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].Length,
                                            carConfigData[groupIndex].carConfigSignalGroupData[listOfNodvIndex - 1].StartBit);
                                    }
                                }
                                continue;
                            }
                            bool isUB = false;
                            if (g_CCPIsCanFD) {
                                if (g_CurrentDBType == DB_TYPE_DBC) {
                                    char expectedUB[128]; snprintf(expectedUB, sizeof(expectedUB), "%s_UB", baseGroup);
                                    isUB = (0 == strcmp(sgn.FName, expectedUB));
                                }
                                else {
                                    std::string extracted = ExtractUBGroupName(sgn.FName, g_CurrentDBType);
                                    isUB = (extracted == baseGroup);
                                }
                            }
                            else {
                                isUB = IsMatchingArxmlSignal(sgn.FName, tempSgnIDName);
                            }

                            if (isUB) {
                                carConfigData[groupIndex].DBName = std::string(db.FName);
                                carConfigData[groupIndex].ECUName = std::string(ecu.FName);
                                carConfigData[groupIndex].FrameName = std::string(msg.FName);
                                carConfigData[groupIndex].FFrameIdentifier = msg.FCANIdentifier;
                                carConfigData[groupIndex].UBbit = sgn.FCANSignal.FStartBit;
                                carConfigData[groupIndex].UBSignal = std::string(sgn.FName);
                                printf("DBName is %s, ECUName is %s, FrameName is %s, FFrameIdentifier is 0x%X, UBbit is %d (UB:%s)\n",
                                    carConfigData[groupIndex].DBName.c_str(), carConfigData[groupIndex].ECUName.c_str(),
                                    carConfigData[groupIndex].FrameName.c_str(), carConfigData[groupIndex].FFrameIdentifier,
                                    carConfigData[groupIndex].UBbit, sgn.FName);
                            }

                            if (IsMatchingArxmlSignal(sgn.FName, tempSgnIDName)) {
                                carConfigData[groupIndex].carConfigSignalGroupData[0].StartBit = sgn.FCANSignal.FStartBit;
                                carConfigData[groupIndex].carConfigSignalGroupData[0].Length = sgn.FCANSignal.FLength;
                                carConfigData[groupIndex].carConfigSignalGroupData[0].SignalName = std::string(sgn.FName);
                                printf("Signal is %s, Length is %d, startBit is %d\n",
                                    carConfigData[groupIndex].carConfigSignalGroupData[0].SignalName.c_str(),
                                    carConfigData[groupIndex].carConfigSignalGroupData[0].Length,
                                    carConfigData[groupIndex].carConfigSignalGroupData[0].StartBit);
                            }

                            std::string groupNameExtracted = ExtractGroupName(sgn.FName, g_CurrentDBType);
                            if (groupNameExtracted.find(tempSgnByteName) != std::string::npos) {
                                for (u8 carConfigSignalIndex = 1; carConfigSignalIndex < 8; carConfigSignalIndex++) {
                                    char expectedByteName[128] = { 0 };
                                    snprintf(expectedByteName, sizeof(expectedByteName), "%sCCPBytePosn%d", baseGroup, carConfigSignalIndex + 1);

                                    if (IsMatchingArxmlSignal(sgn.FName, expectedByteName)) {
                                        carConfigData[groupIndex].carConfigSignalGroupData[carConfigSignalIndex].StartBit = sgn.FCANSignal.FStartBit;
                                        carConfigData[groupIndex].carConfigSignalGroupData[carConfigSignalIndex].Length = sgn.FCANSignal.FLength;
                                        carConfigData[groupIndex].carConfigSignalGroupData[carConfigSignalIndex].SignalName = std::string(sgn.FName);
                                        printf("Signal is %s, Length is %d, startBit is %d\n",
                                            carConfigData[groupIndex].carConfigSignalGroupData[carConfigSignalIndex].SignalName.c_str(),
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
    return 0;
}

s32 setCarConfigValue(bool state) {
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
        for (int index = 0; index < ActualCarConfigCount; index++)
        {
            if (recordCarConfigDataList[index].CCPPsn <= 504)
            {
                if (gCCPData[recordCarConfigDataList[index].CCPPsn - 1] != recordCarConfigDataList[index].CCPValue)
                {
                    gCCPData[recordCarConfigDataList[index].CCPPsn - 1] = recordCarConfigDataList[index].CCPValue;
                }
            }
            else if (505 <= recordCarConfigDataList[index].CCPPsn && recordCarConfigDataList[index].CCPPsn < 1000)
            {
                if (gCCPDataExt[recordCarConfigDataList[index].CCPPsn - 505] != recordCarConfigDataList[index].CCPValue)
                {
                    gCCPDataExt[recordCarConfigDataList[index].CCPPsn - 505] = recordCarConfigDataList[index].CCPValue;
                }
            }
            else if (1256 <= recordCarConfigDataList[index].CCPPsn && recordCarConfigDataList[index].CCPPsn <= 1760)
            {
                if (gCCPDataExt2[recordCarConfigDataList[index].CCPPsn - 1256] != recordCarConfigDataList[index].CCPValue)
                {
                    gCCPDataExt2[recordCarConfigDataList[index].CCPPsn - 1256] = recordCarConfigDataList[index].CCPValue;
                }
                //printf("gCCPDataExt2[%d] = %d;", recordCarConfigDataList[index].CCPPsn - 1256, gCCPDataExt2[recordCarConfigDataList[index].CCPPsn - 1256]);
            }
            else
            {
                for (u8 i = 0; i < 8; i++) {
                    gCCPDataListOfNodAv[i] = 0xFFFFFFFF;
                }
            }

            if (recordCarConfigDataList[index].CCPPsn <= 504)
            {
                if (gCCPData[recordCarConfigDataList[index].CCPPsn - 1] != recordCarConfigDataList[index].CCPValue)
                {
                    gCCPData[recordCarConfigDataList[index].CCPPsn - 1] = recordCarConfigDataList[index].CCPValue;
                }
            }
            else if (505 <= recordCarConfigDataList[index].CCPPsn && recordCarConfigDataList[index].CCPPsn < 1000)
            {
                if (gCCPDataExt[recordCarConfigDataList[index].CCPPsn - 505] != recordCarConfigDataList[index].CCPValue)
                {
                    gCCPDataExt[recordCarConfigDataList[index].CCPPsn - 505] = recordCarConfigDataList[index].CCPValue;
                }
            }
            else if (1256 <= recordCarConfigDataList[index].CCPPsn && recordCarConfigDataList[index].CCPPsn <= 1760)
            {
                if (gCCPDataExt2[recordCarConfigDataList[index].CCPPsn - 1256] != recordCarConfigDataList[index].CCPValue)
                {
                    gCCPDataExt2[recordCarConfigDataList[index].CCPPsn - 1256] = recordCarConfigDataList[index].CCPValue;
                }
                //printf("gCCPDataExt2[%d] = %d;", recordCarConfigDataList[index].CCPPsn2 - 1256, gCCPDataExt2[recordCarConfigDataList[index].CCPPsn2 - 1256]);
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
        TCANFD CANFDCarConfig, CarConfigListOfNodv;
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
        CarConfigListOfNodv.FData[carConfigData[3].UBbit / 8] |= (1 << carConfigData[3].UBbit % 8);
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
    if (index_cc1 >= 9)
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


    setCarConfigValue(state);

    CCPTimer.stop();
    CCPTimer.start();
    CCPTimer.set_interval(50);
    log("开始仿真CCP");
    carConfigEndFlg = false;
    app.get_timestamp(&lastTime);
    while ((nowTime - lastTime) <= 30000000) {
        app.get_timestamp(&nowTime);
        AppWait(1000, "");
        if (carConfigEndFlg) {
            break;
        }
    }
    CCPTimer.stop();
    return 0;
}

s32 StartCarConfig(bool state = true) {
    FulfilTRC();
    app.wait(5000, "");
    UsgModSwtich(0x01);

    TRCSwitch(0x0, 0xD, 0x0);
    simulateCCP(state);
    UsgModSwtich(0x01);
    return 0;
}

s32 Pretestcase()
{
    try {
        strcpy(TestCaseName, "Pretestcase");
        strcpy(TestScopeName, "Pretestcase");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        StartCarConfig();
        app.create_system_var("CheckCom", svtInt32, "0", "this is a check for DUT's wakeup");
        app.create_system_var("ResumCount", svtInt32, "0", "this is a count for busoff recovery times");
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}
#endif // !TEST_CASE_DIAG_API