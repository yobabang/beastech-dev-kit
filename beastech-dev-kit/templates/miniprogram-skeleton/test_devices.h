#ifndef TEST_DEVICES
#define TEST_DEVICES
//#pragma comment(lib,"ws2_32.lib")
#include<string>
#include <TSMaster.h>
#include "MPLibrary.h"
#include <vector>
#include "TestReport.h"
#include "test_common.h"

/*---------------------------参数---------------------------*/
//Pico Type          
#define PICO3000              0
#define PICO4000              1
#define PICO5000              2
//Range
#define RANGE_2_MV_div        0
#define RANGE_4_MV_div        1
#define RANGE_10_MV_div       2
#define RANGE_25_MV_div       3
#define RANGE_40_MV_div       4
#define RANGE_100_MV_div      5
#define RANGE_200_MV_div      6
#define RANGE_400_MV_div      7
#define RANGE_1_V_div         8
#define RANGE_2_V_div         9
#define RANGE_4_V_div         10
#define RANGE_10_V_div        11
//Resolution
#define PS_DR_8BIT            0
#define PS_DR_12BIT           1
#define PS_DR_14BIT           2
#define PS_DR_15BIT           3
#define PS_DR_16BIT           4
//Trigger Direction          
#define PS3000A_ABOVE               0
#define PS3000A_BELOW               1
#define PS3000A_RISING              2
#define PS3000A_FALLING             3
#define PS3000A_RISING_OR_FALLING   4
#define PS3000A_ABOVE_LOWER         5
#define PS3000A_BELOW_LOWER         6
#define PS3000A_RISING_LOWER        7
#define PS3000A_FALLING_LOWER       8
#define PS3000A_POSITIVE_RUNT       9
#define PS3000A_NEGATIVE_RUNT       10
#define PS3000A_NONE                11
#define PICO_DATA_MAX               100000

//CAN一致性板卡内部FDCAN1波特率设置枚举
#define CAN_500K_1M 0x00
#define CAN_500K_2M 0x01
#define CAN_500K_4M 0x03
//typedef enum 
//{
//    CAN_500K_1M,    // 0x00
//    CAN_500K_2M,    // 0x01
//    CAN_500K_3M,    // 0x02
//    CAN_500K_4M,    // 0x03
//    CAN_500K_5M,    // 0x04
//    CAN_500K_6M,
//    CAN_500K_8M,    // 0x06
//    CAN_500K_10M,
//    CAN_250K_500K,  // 0x08
//    CAN_250K_833K,
//    CAN_250K_1M,
//    CAN_250K_1M5,
//    CAN_250K_2M,
//    CAN_250K_3M,
//    CAN_250K_4M,
//    CAN_1000K_4M,   // 0x0f
//    CAN_1000K_8M,
//    CAN_125K_500K   // 0x11
//} CAN_BITTIME_SETUP;

u32 KL30ON = 0, KL31ON = 0;
u32 BoardCH = CH1;
/*----------------------------------------------设备/板卡函数库----------------------------------------*/
//注册类定义
template <typename T>
class devices
{
public:
    static T* instance()
    {
        static T object;
        return &object;
    }
};

inline void AppWait(const s32 ATimeMs, const char* AMsg) {
    app.wait(ATimeMs, AMsg);
    //检查是否有手动停止测试
    if (0 != app.is_connected()) {
        throw test_terminate();
    }
}

/*--------------------主电源函数库----------------------*/
//主电源
class main_power : public devices<main_power>
{
public: char main_power_Serial[1024] = 
    //"USB0::0xFFFF::0x6500::800630011806910029::0::INSTR";
    "USB0::0xFFFF::0x6500::800630011797510024::0::INSTR";

private:
    char Set_voltage_command[100] = "VOLT ";
    char Set_current_command[100] = "CURR ";
    char Set_on_command[100] = "OUTP ON";
    char Set_off_command[100] = "OUTP OFF";
    char Read_voltage_command[100] = "MEAS:VOLT?";
    char Read_current_command[100] = "MEAS:CURR?";
    char IDN[100] = "*IDN?";//读取id

    const char buffer[1024] = { 0 };
    const s32 bufferLen = 1024;
    s32 main_power_handle = -1;
public:
    main_power()
    {
    }
    ~main_power()
    {
    }

    s32 DeviceCreat()
    {
        s32 res = -1;
        //if (main_power_handle == -1)
        //{
        res = rtlTSInstrument.visa_create_device(&main_power_handle, VT_AGILENT_VISA, main_power_Serial);
        //}
        //else
        //{
        //    res = 0;
        //}
        return res;
    }

    s32 Set_Voltage(double voltage, double current_limit = 3)
    {
        char buffer_VC[1024] = "";

        sprintf(buffer_VC, "%s%s", Set_voltage_command, std::to_string(voltage).c_str());
        rtlTSInstrument.visa_send(main_power_handle, buffer_VC);

        sprintf(buffer_VC, "");
        sprintf(buffer_VC, "%s%s", Set_current_command, std::to_string(current_limit).c_str());
        return rtlTSInstrument.visa_send(main_power_handle, buffer_VC);
    }

    s32 Power_On()
    {
        return rtlTSInstrument.visa_send(main_power_handle, Set_on_command);
    }

    s32 Power_Off()
    {
        return rtlTSInstrument.visa_send(main_power_handle, Set_off_command);
    }

    double Read_Val()
    {
        rtlTSInstrument.visa_query_bin(main_power_handle, Read_voltage_command, buffer, &bufferLen);
        return strtod(buffer, NULL);
    }

    double Read_Cur()
    {
        rtlTSInstrument.visa_query_bin(main_power_handle, Read_current_command, buffer, &bufferLen);
        return strtod(buffer, NULL);
    }

};

/*--------------------双极性电源函数库-------------------------*/
class bipolar_power : public devices<bipolar_power>
{
public: char bipolar_power_Serial[1024] = { 0 };
private:
    char Set_voltage_command[100] = "VOLT ";
    char Set_current_command[100] = "CURR ";
    char Set_on_command[100] = "OUTP ON";
    char Set_off_command[100] = "OUTP OFF";
    char Read_voltage_command[100] = "MEAS:VOLT?";
    char Read_current_command[100] = "MEAS:CURR?";
    char IDN[100] = "*IDN?";//读取id
    char CLEar[100] = "OUTPut:PROTection:CLEar";//清除故障

    s32 bipolar_power_handle = -1;
    const char* buffer = "";
    const s32 bufferLen = 1024;
public:
    bipolar_power()
    {
    }
    ~bipolar_power()
    {
    }

    s32 DeviceCreat()
    {
        s32 res = -1;
        //if (bipolar_power_handle == -1)
        //{
        res = rtlTSInstrument.visa_create_device(&bipolar_power_handle, VT_AGILENT_VISA, bipolar_power_Serial);
        //}
        //else
        //{
        //    res = 0;
        //}
        return res;
    }

    s32 Set_Voltage(double voltage, double current_limit)
    {
        char buffer_VC[1024] = "";

        sprintf(buffer_VC, "%s%s", Set_voltage_command, std::to_string(voltage).c_str());
        rtlTSInstrument.visa_send(bipolar_power_handle, buffer_VC);

        sprintf(buffer_VC, "");
        sprintf(buffer_VC, "%s%s", Set_current_command, std::to_string(current_limit).c_str());
        return rtlTSInstrument.visa_send(bipolar_power_handle, buffer_VC);
    }

    s32 Power_On()
    {
        return rtlTSInstrument.visa_send(bipolar_power_handle, Set_on_command);
    }

    s32 Power_Off()
    {
        return rtlTSInstrument.visa_send(bipolar_power_handle, Set_off_command);
    }

    double Read_Val()
    {
        rtlTSInstrument.visa_query_bin(bipolar_power_handle, Read_voltage_command, buffer, &bufferLen);
        return strtod(buffer, NULL);
    }

    double Read_Cur()
    {
        rtlTSInstrument.visa_query_bin(bipolar_power_handle, Read_current_command, buffer, &bufferLen);
        return strtod(buffer, NULL);
    }

    s32 Clear_Error()
    {
        return rtlTSInstrument.visa_send(bipolar_power_handle, CLEar);
    }
};

/*--------------------数字万用表函数库------------------*/
//class rigol_multimeter :public devices<rigol_multimeter>
//{
//
//private:
//	char Read_voltage_AC_command[100] = "MEAS:VOLT:AC?";
//	char Read_voltage_DC_command[100] = "MEAS:VOLT:DC?";
//	char Read_current_AC_command[100] = "MEAS:CURR:AC?";
//	char Read_current_DC_command[100] = "MEAS:CURR:DC?";
//	char Read_resistance_command[100] = "MEAS:RES?";
//	char IDN[100] = "*IDN?";//读取id
//
//	s32 rigol_handle = -1;
//	const char* buffer = "";
//	const s32 bufferLen = 1024;
//public:
//	rigol_multimeter()
//	{
//	};
//	~rigol_multimeter()
//	{
//	};
//
//    s32 DeviceCreat()
//    {
//        s32 res = -1;
//        //if (rigol_handle == -1)
//        //{
//            res = rtlTSInstrument.visa_create_device(&rigol_handle, VT_AGILENT_VISA, TestPara.MultimeterSerial.c_str());
//        //}
//        //else
//        //{
//        //    res = 0;
//        //}
//        return res;
//	}
//
//	double Read_Resistance()
//	{
//		if (rigol_handle == -1)
//		{
//			TestInfoPrintFail("万用表未连接");
//			return 0;
//		}
//		rtlTSInstrument.visa_query(rigol_handle, "*IDN?", &buffer);
//		app.wait(2000, "");
//		rtlTSInstrument.visa_query_bin(rigol_handle, Read_resistance_command, buffer, &bufferLen);
//		return strtod(buffer, NULL);
//	}
//
//	double Read_Current_DC()
//	{
//		if (rigol_handle == -1)
//		{
//			TestInfoPrintFail("万用表未连接");
//			return 0;
//		}
//		rtlTSInstrument.visa_query_bin(rigol_handle, Read_current_DC_command, buffer, &bufferLen);
//		return strtod(buffer, NULL);
//	}
//
//	double Read_Current_AC()
//	{
//		if (rigol_handle == -1)
//		{
//			TestInfoPrintFail("万用表未连接");
//			return 0;
//		}
//		rtlTSInstrument.visa_query_bin(rigol_handle, Read_current_AC_command, buffer, &bufferLen);
//		return strtod(buffer, NULL);
//	}
//
//	double Read_Voltage_DC()
//	{
//		if (rigol_handle == -1)
//		{
//			TestInfoPrintFail("万用表未连接");
//			return 0;
//		}
//		rtlTSInstrument.visa_query_bin(rigol_handle, Read_voltage_DC_command, buffer, &bufferLen);
//    return strtod(buffer, NULL);
//	}
//
//	double Read_Voltage_AC()
//	{
//		if (rigol_handle == -1)
//		{
//			TestInfoPrintFail("万用表未连接");
//			return 0;
//		}
//		rtlTSInstrument.visa_query_bin(rigol_handle, Read_voltage_AC_command, buffer, &bufferLen);
//		return strtod(buffer, NULL);
//	}
//};

class rigol_multimeter :public devices<rigol_multimeter>
{
public: char rigol_multimeter_Serial[1024] = { 0 };
private:
    std::string Read_voltage_AC_command = "MEAS:VOLT:AC?";
    std::string Read_voltage_DC_command = "MEAS:VOLT:DC?";
    std::string Read_current_AC_command = "MEAS:CURR:AC?";
    std::string Read_current_DC_command = "MEAS:CURR:DC?";
    std::string Read_resistance_command = "MEAS:RES?";
    char IDN[100] = "*IDN?";//读取id

    s32 rigol_handle = -1;
    /*const char* buffer = "";
    const s32 bufferLen = 1024;*/
public:
    char buffer[1024] = "";
    const char* buffer1 = buffer;
    rigol_multimeter()
    {
    };
    ~rigol_multimeter()
    {
    };

    s32 DeviceCreat()
    {
        s32 res = -1;
        //if (rigol_handle == -1)
        //{
        res = rtlTSInstrument.visa_create_device(&rigol_handle, VT_AGILENT_VISA, rigol_multimeter_Serial);
        //}
        //else
        //{
        //    res = 0;
        //}
        return res;
    }

    double Read_Resistance()
    {
        if (rigol_handle == -1)
        {
            TestInfoPrintFail("万用表未连接");
            return 0;
        }
        //rtlTSInstrument.visa_query(rigol_handle, "*IDN?", &buffer);
        app.wait(2000, "");
        rtlTSInstrument.visa_query(rigol_handle, Read_resistance_command.c_str(), &buffer1);
        return strtod(buffer1, NULL);
    }

    double Read_Current_DC()
    {
        if (rigol_handle == -1)
        {
            TestInfoPrintFail("万用表未连接");
            return 0;
        }
        rtlTSInstrument.visa_query(rigol_handle, Read_current_DC_command.c_str(), &buffer1);
        return strtod(buffer1, NULL);
    }

    double Read_Current_AC()
    {
        if (rigol_handle == -1)
        {
            TestInfoPrintFail("万用表未连接");
            return 0;
        }
        rtlTSInstrument.visa_query(rigol_handle, Read_current_AC_command.c_str(), &buffer1);
        return strtod(buffer1, NULL);
    }

    double Read_Voltage_DC()
    {
        if (rigol_handle == -1)
        {
            TestInfoPrintFail("万用表未连接");
            return 0;
        }
        rtlTSInstrument.visa_query(rigol_handle, Read_voltage_DC_command.c_str(), &buffer1);
        return strtod(buffer1, NULL);
    }

    double Read_Voltage_AC()
    {
        if (rigol_handle == -1)
        {
            TestInfoPrintFail("万用表未连接");
            return 0;
        }
        rtlTSInstrument.visa_query(rigol_handle, Read_voltage_AC_command.c_str(), &buffer1);
        return strtod(buffer1, NULL);
    }
};

/*--------------------CAN一致性测试板卡函数库------------------*/
class boardcard : public devices<boardcard>
{
private:
    union float_to_char
    {
        float value;
        u8 bytes[4];
    };
public:
    boardcard()
    {
        InitRelays();
    }
    ~boardcard()
    {
        InitRelays();
    }
    //继电器控制编号
    std::vector<u8> Small = { 32,38,41,42,66,69,71,78,92,94,96,116,119,121,141,144,146,148,159,161,167,168,179,180,182,185,186,189,192,193,194,196,197,214,215,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,242 };
    std::vector<u8> Big = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,29,33,34,35,44,48,49,51,55,56,59,62,63,74,77,79,80,81,84,85,87,88,90,91
                    ,93,97,99,102,103,105,106,107,109,110,112,113,114,118,120,124,127,128,131,134,135,137,142,145,149,150,151,152,153,154,155,156,157,169,199,200,201,202,
                    203,204,205,206,207,208,209,210,211,212,213,216 };
    void BoardCardSendMessage(u8 RelayNum, u8 data)
    {
        if (count(Big.begin(), Big.end(), RelayNum))
        {
            TCAN f0 = { CH1,0x5,8,0,0x71700,0,{0x05, RelayNum, data, 0x00, 0x00, 0x00, 0x00, 0x00} };
            com.transmit_can_async(&f0);
            app.wait(0, "");
        }
        if (count(Small.begin(), Small.end(), RelayNum))
        {
            TCAN f0 = { CH1,0x5,8,0,0x71600,0,{0x05, RelayNum, data, 0x00, 0x00, 0x00, 0x00, 0x00} };
            com.transmit_can_async(&f0);
            app.wait(0, "");
        }
    }
    void Set_Resistance(u8 ch, float res)
    {
        union float_to_char float_num;
        float_num.value = res;
        TCAN f0 = { CH1, 0x5, 8, 0, 0x10071027, 0, {0x27, 0x05, ch, float_num.bytes[0], float_num.bytes[1], float_num.bytes[2], float_num.bytes[3], 0x00} };
        com.transmit_can_async(&f0);
        app.wait(0, "");
    }
    void Set_Capacitance(u8 ch, float cap)
    {
        union float_to_char float_num;
        float_num.value = cap;
        TCAN f0 = { CH1, 0x5, 8, 0, 0x71100, 0, {0x2F, 0x05, ch, float_num.bytes[0], float_num.bytes[1], float_num.bytes[2], float_num.bytes[3], 0x00} };
        com.transmit_can_async(&f0);
        app.wait(0, "");
    }
    //继电器初始化
    void InitRelays()
    {
        for (s32 i = 1; i <= 242; i++)
        {
            BoardCardSendMessage(i, 0);
            //5ms一条报文，避免报文溢出，同时降低板卡继电器电源压力
            app.wait(5, "");
        }
    }
    //电阻电容矩阵通道初始化
    void Init_ResCapCH()
    {
        for (s32 i = 0; i < 3; i++)
        {
            Set_Resistance(i, 40958); //最大电阻值16383*5/2=40957.5
            app.wait(5, "");
        }
        for (s32 i = 0; i < 3; i++)
        {
            Set_Capacitance(i, 0);
            app.wait(5, "");
        }
    }
    //----------------DUT接口配置---------------
    //DUT KL30连接内部BAT
    void KL30_to_NBAT(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(200, 1);
            BoardCardSendMessage(201, 0);
            if (KL31ON)
            {
                BoardCardSendMessage(18, 1);// MainPwr FB+/- 连接反馈FBBAT(DUT KL30)和反馈FBGND(DUT KL31_1)或内部地NGND
            }
            KL30ON = 1;
        }
        else
        {
            KL30ON = 0;
            BoardCardSendMessage(18, 0);// MainPwr FB+/- 连接 MainPwr VCC/GND，避免电源电压补偿错误，下次上电烧坏继电器线路！！
            BoardCardSendMessage(200, 0);
            BoardCardSendMessage(201, 0);
        }
    }
    //DUT KL30连接测试线1
    void KL30_to_Line1(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(156, 1);
            BoardCardSendMessage(146, 0);
            BoardCardSendMessage(145, 0);
        }
        else
        {
            BoardCardSendMessage(156, 0);
            BoardCardSendMessage(146, 0);
            BoardCardSendMessage(145, 0);
        }
    }
    //DUT KL30连接测试线2
    void KL30_to_Line2(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(156, 1);
            BoardCardSendMessage(146, 0);
            BoardCardSendMessage(145, 1);
        }
        else
        {
            BoardCardSendMessage(156, 0);
            BoardCardSendMessage(146, 0);
            BoardCardSendMessage(145, 0);
        }
    }
    //DUT KL30连接测试线3
    void KL30_to_Line3(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(156, 1);
            BoardCardSendMessage(146, 1);
            BoardCardSendMessage(145, 0);
        }
        else
        {
            BoardCardSendMessage(156, 0);
            BoardCardSendMessage(146, 0);
            BoardCardSendMessage(145, 0);
        }
    }
    //DUT KL30连接测试线4
    void KL30_to_Line4(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(156, 1);
            BoardCardSendMessage(146, 1);
            BoardCardSendMessage(145, 1);
        }
        else
        {
            BoardCardSendMessage(156, 0);
            BoardCardSendMessage(146, 0);
            BoardCardSendMessage(145, 0);
        }
    }
    //DUT KL31-1连接内部GND
    void KL31_to_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(212, 1);
            BoardCardSendMessage(209, 1);
            BoardCardSendMessage(203, 1);
            BoardCardSendMessage(199, 0);
            BoardCardSendMessage(6, 0); // 选择反馈地FBGND
            if (KL30ON)
            {
                BoardCardSendMessage(18, 1);// MainPwr FB+/- 连接反馈FBBAT(DUT KL30)和反馈FBGND(DUT KL31_1)
            }
            KL31ON = 1;
        }
        else
        {
            KL31ON = 0;
            BoardCardSendMessage(18, 0);// MainPwr FB+/- 连接 MainPwr VCC/GND，避免电源电压补偿错误，下次上电烧坏继电器线路！！
            BoardCardSendMessage(6, 0); // 继电器恢复默认状态，选择反馈地FBGND
            BoardCardSendMessage(212, 0);
            BoardCardSendMessage(209, 0);
            BoardCardSendMessage(203, 0);
            BoardCardSendMessage(199, 0);
        }
    }
    //DUT KL31-1连接地偏BAT
    void KL31_1_to_ShiftBAT(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(212, 1);
            BoardCardSendMessage(199, 1);
            AppWait(500, "");//延迟取消主电源反馈
            BoardCardSendMessage(18, 0);// 取消MainPwr FB+/- 连接反馈FBBAT(DUT KL30)和内部地NGND
        }
        else
        {
            BoardCardSendMessage(18, 0);
            AppWait(500, "");//延迟取消主电源反馈
            BoardCardSendMessage(212, 0);
            BoardCardSendMessage(199, 0);
        }
    }
    //DUT KL31-2连接内部GND
    void KL31_2_to_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(209, 1);
            BoardCardSendMessage(199, 0);
        }
        else
        {
            BoardCardSendMessage(209, 0);
            BoardCardSendMessage(199, 0);
        }
    }
    //DUT KL31-3连接内部GND
    void KL31_3_to_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(203, 1);
            BoardCardSendMessage(199, 0);
        }
        else
        {
            BoardCardSendMessage(203, 0);
            BoardCardSendMessage(199, 0);
        }
    }
    //KL15接到内部BAT
    void KL15_to_NBAT(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(210, 1);
            BoardCardSendMessage(205, 0);
        }
        else
        {
            BoardCardSendMessage(210, 0);
            BoardCardSendMessage(205, 0);
        }
    }
    //KL15接到唤醒BAT
    void KL15_to_WakeBAT(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(210, 1);
            BoardCardSendMessage(205, 1);
        }
        else
        {
            BoardCardSendMessage(210, 0);
            BoardCardSendMessage(205, 0);
        }
    }
    //KLR接到内部BAT
    void KLR_to_NBAT(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(206, 1);
            BoardCardSendMessage(208, 0);
        }
        else
        {
            BoardCardSendMessage(206, 0);
            BoardCardSendMessage(208, 0);
        }
    }
    //KLR接到唤醒BAT
    void KLR_to_WakeBAT(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(206, 1);
            BoardCardSendMessage(208, 1);
        }
        else
        {
            BoardCardSendMessage(206, 0);
            BoardCardSendMessage(208, 0);
        }
    }
    //WAKEUP接到内部BAT
    void WAKE_UP_to_NBAT(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(213, 1);
            BoardCardSendMessage(214, 0);
        }
        else
        {
            BoardCardSendMessage(213, 0);
            BoardCardSendMessage(214, 0);
        }
    }
    //WAKEUP接到内部GND
    void WAKE_UP_to_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(220, 1);
        }
        else
        {
            BoardCardSendMessage(220, 0);
        }
    }
    //WAKEUP接到唤醒BAT
    void WAKE_UP_to_WakeBAT(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(213, 1);
            BoardCardSendMessage(214, 1);
        }
        else
        {
            BoardCardSendMessage(213, 0);
            BoardCardSendMessage(214, 0);
        }
    }
    //DUT CAN_H连接到内部CAN_H
    void CAN_H_to_NCAN_H(CtrlStatus status) {
        if (ON == status)
        {
            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);
        }
        else
        {
            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 0);
        }
    }
    //DUT CAN_L连接到内部CAN_L
    void CAN_L_to_NCAN_L(CtrlStatus status) {
        if (ON == status)
        {
            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);
        }
        else
        {
            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 0);
            BoardCardSendMessage(44, 0);
        }
    }
    //DUT CAN_GND连接内部CAN_GND
    void CAN_GND_to_NCAN_GND(CtrlStatus status) {
        if (ON == status) {
            BoardCardSendMessage(48, 0);
            BoardCardSendMessage(49, 1);
            BoardCardSendMessage(51, 1);
        }
        else {
            BoardCardSendMessage(48, 0);
            BoardCardSendMessage(49, 0);
            BoardCardSendMessage(51, 0);
        }

    }
    //DUT CAN_GND连接内部GND
    void CAN_GND_to_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(32, 1);
        }
        else
        {
            BoardCardSendMessage(32, 0);
        }
    }
    //----------------电源接口配置---------------
    //主电源
    //主电源MainPwr连接内部BAT、GND
    void MainPwr_to_NBAT_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(17, 1);
            BoardCardSendMessage(216, 1);
        }
        else
        {
            BoardCardSendMessage(17, 0);
            BoardCardSendMessage(216, 0);
        }
    }
    //主电源MainPwr连接内部BAT、MainPwr的GND先串联数字万用表DMM后再连接内部GND
    void MainPwr_to_DMM_to_NBAT_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(17, 1);
            BoardCardSendMessage(216, 0);
            BoardCardSendMessage(211, 1);
        }
        else
        {
            BoardCardSendMessage(17, 0);
            BoardCardSendMessage(216, 0);
            BoardCardSendMessage(211, 0);
        }
    }
    //主电源MainPwr GND连接SinkPwr GND
    void MainPwr_GND_to_SinkPwr_GND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(229, 1);
        }
        else
        {
            BoardCardSendMessage(229, 0);
        }
    }
    //电流型电源
    //电流型电源SinkPwr连接地偏BAT和内部GND
    void SinkPwr_to_ShiftBAT_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(21, 1);
        }
        else
        {
            BoardCardSendMessage(21, 0);
        }
    }
    //电流型电源SinkPwr连接反馈GND(DUT KL31_1)和内部GND
    void SinkPwr_FB_to_FBGND_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(20, 1);
        }
        else
        {
            BoardCardSendMessage(20, 0);
        }
    }
    //信号电源
    //信号电源SigPwr连接唤醒BAT和内部GND
    void SigPwr_to_WakeBAT_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(7, 0);
            BoardCardSendMessage(8, 0);
            BoardCardSendMessage(19, 1);
        }
        else
        {
            BoardCardSendMessage(7, 0);
            BoardCardSendMessage(8, 0);
            BoardCardSendMessage(19, 0);
        }
    }
    //信号电源SigPwr输入端电源口VCC+/-选择
    void SigPwr_VCCH_or_VCCL(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(8, 0); // VCC+
        }
        else
        {
            BoardCardSendMessage(8, 1); // VCC-
        }
    }
    //信号电源SigPwr正极接内部NCAN_H
    void SigPwr_H_to_NCAN_H(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(180, 0);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 1);
        }
        else
        {
            BoardCardSendMessage(180, 0);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 0);
        }
    }
    //信号电源SigPwr正极接内部NCAN_L
    void SigPwr_H_to_NCAN_L(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(180, 0);
            BoardCardSendMessage(179, 1);
            BoardCardSendMessage(182, 0);
        }
        else
        {
            BoardCardSendMessage(180, 0);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 0);
        }
    }
    //信号电源SigPwr负极GND接内部NCAN_H
    void SigPwr_GND_to_NCAN_H(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(186, 0);
            BoardCardSendMessage(185, 0);
            BoardCardSendMessage(189, 1);
        }
        else
        {
            BoardCardSendMessage(186, 0);
            BoardCardSendMessage(185, 0);
            BoardCardSendMessage(189, 0);
        }
    }
    //信号电源SigPwr负极GND接内部NCAN_L
    void SigPwr_GND_to_NCAN_L(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(186, 0);
            BoardCardSendMessage(185, 1);
            BoardCardSendMessage(189, 0);
        }
        else
        {
            BoardCardSendMessage(186, 0);
            BoardCardSendMessage(185, 0);
            BoardCardSendMessage(189, 0);
        }
    }
    //信号电源SigPwr负极GND接内部NCAN_GND
    void SigPwr_GND_to_NCAN_GND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(186, 0);
            BoardCardSendMessage(185, 1);
            BoardCardSendMessage(189, 1);
        }
        else
        {
            BoardCardSendMessage(186, 0);
            BoardCardSendMessage(185, 0);
            BoardCardSendMessage(189, 0);
        }
    }
    //电流源
    //电流源正极串联200Ω、负极串联电阻矩阵通道ch2后，接内部NCAN
    void CurrentSrc_to_Res2_to_NCAN(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(167, 1);
            BoardCardSendMessage(168, 1);
        }
        else
        {
            BoardCardSendMessage(167, 0);
            BoardCardSendMessage(168, 0);
        }
    }
    //----------------示波器接口配置---------------
    //示波器CH1连接到内部NCAN_H
    void Scope_CH1_to_NCAN_H(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(120, 0);
            BoardCardSendMessage(121, 0);
            BoardCardSendMessage(124, 1);
        }
        else
        {
            BoardCardSendMessage(120, 0);
            BoardCardSendMessage(121, 0);
            BoardCardSendMessage(124, 0);
        }
    }
    //示波器CH2连接到内部NCAN_L
    void Scope_CH2_to_NCAN_L(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(127, 0);
            BoardCardSendMessage(128, 1);
            BoardCardSendMessage(131, 0);
        }
        else
        {
            BoardCardSendMessage(127, 0);
            BoardCardSendMessage(128, 0);
            BoardCardSendMessage(131, 0);
        }
    }
    //示波器CH3连接到KL30
    void Scope_CH3_to_KL30(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(3, 0);
            BoardCardSendMessage(13, 0);
            BoardCardSendMessage(12, 0);
            BoardCardSendMessage(25, 1);
        }
        else
        {
            BoardCardSendMessage(3, 0);
            BoardCardSendMessage(13, 0);
            BoardCardSendMessage(12, 0);
            BoardCardSendMessage(25, 0);
        }
    }
    //示波器CH3连接到KL15
    void Scope_CH3_to_KL15(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(9, 0);
            BoardCardSendMessage(13, 1);
            BoardCardSendMessage(12, 0);
            BoardCardSendMessage(25, 1);
        }
        else
        {
            BoardCardSendMessage(9, 0);
            BoardCardSendMessage(13, 0);
            BoardCardSendMessage(12, 0);
            BoardCardSendMessage(25, 0);
        }
    }
    //示波器CH3和GND分别连接到DUT CAN_RXD和MCU_GND
    void Scope_CH3_to_CAN_RXD(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(219, 1);
        }
        else
        {
            BoardCardSendMessage(219, 0);
        }
    }
    //示波器CH4和GND分别连接到DUT CAN_TXD和MCU_GND
    void Scope_CH4_to_CAN_TXD(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(218, 1);
        }
        else
        {
            BoardCardSendMessage(218, 0);
        }
    }
    //SCOPE_CH4连接触发输出I/O1
    void Scope_CH4_to_IO1(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(27, 1);  //触发 I/O1-->Scope_ch4

        }
        else
        {
            BoardCardSendMessage(27, 0);
        }
    }
    //I/O1对应的5V隔离1GND通过测试线4连接内部CAN_GND
    void N1GND_to_Line4_to_NCAN_GND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(29, 1);   //5V隔离1GND --> 测试线4
            BoardCardSendMessage(155, 1);
            BoardCardSendMessage(142, 1);
            BoardCardSendMessage(141, 1);
            BoardCardSendMessage(91, 1);  //测试线4接内部CAN_GND
            BoardCardSendMessage(90, 1);
        }
        else
        {
            BoardCardSendMessage(29, 0);
            BoardCardSendMessage(155, 0);
            BoardCardSendMessage(142, 0);
            BoardCardSendMessage(141, 0);
            BoardCardSendMessage(91, 0);
            BoardCardSendMessage(90, 0);
        }
    }
    //示波器GND连接到内部CAN_GND
    void Scope_GND_to_NCAN_GND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(134, 0);
            BoardCardSendMessage(135, 1);
            BoardCardSendMessage(137, 1);
            BoardCardSendMessage(24, 1);//fzj add
            BoardCardSendMessage(32, 1);//fzj add
        }
        else
        {
            BoardCardSendMessage(134, 0);
            BoardCardSendMessage(135, 0);
            BoardCardSendMessage(137, 0);
            BoardCardSendMessage(24, 0);//fzj add
            BoardCardSendMessage(32, 0);//fzj add
        }
    }
    //示波器GND连接到内部GND
    void Scope_GND_to_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(24, 1);
        }
        else
        {
            BoardCardSendMessage(24, 0);
        }
    }
    //----------------数字万用表接口配置---------------
    //数字万用表DMM电流测量档位选择A档、mA档
    void DMM_I_A_or_mA(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(218, 0); // I_A
        }
        else
        {
            BoardCardSendMessage(218, 1); // I_mA
        }
    }
    //数字万用表DMM正负极端口连接Wakeup和内部GND
    void DMM_to_WAKEUP_NGND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(2, 1);
            BoardCardSendMessage(4, 0);
            BoardCardSendMessage(22, 1);
            BoardCardSendMessage(16, 1);
        }
        else
        {
            BoardCardSendMessage(2, 0);
            BoardCardSendMessage(4, 0);
            BoardCardSendMessage(22, 0);
            BoardCardSendMessage(16, 0);
        }
    }
    //数字万用表DMM正负极端口连接反馈BAT(DUT KL30)和反馈GND(DUT KL31_1)
    void DMM_to_KL30_KL31(CtrlStatus status) {
        if (ON == status)
        {
            BoardCardSendMessage(1, 0);
            BoardCardSendMessage(4, 1);
            BoardCardSendMessage(22, 1);
            BoardCardSendMessage(16, 0);
        }
        else
        {
            BoardCardSendMessage(1, 0);
            BoardCardSendMessage(4, 0);
            BoardCardSendMessage(22, 0);
            BoardCardSendMessage(16, 0);
        }

    }
    //数字万用表DMM正极端口连接到内部NCAN_H
    void DMM_H_to_NCAN_H(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(55, 0);
            BoardCardSendMessage(56, 0);
            BoardCardSendMessage(59, 1);
        }
        else
        {
            BoardCardSendMessage(55, 0);
            BoardCardSendMessage(56, 0);
            BoardCardSendMessage(59, 0);
        }
    }
    //数字万用表DMM正极端口连接到内部NCAN_L
    void DMM_H_to_NCAN_L(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(55, 0);
            BoardCardSendMessage(56, 1);
            BoardCardSendMessage(59, 0);
        }
        else
        {
            BoardCardSendMessage(55, 0);
            BoardCardSendMessage(56, 0);
            BoardCardSendMessage(59, 0);
        }
    }
    //数字万用表DMM负极端口连接到内部NCAN_L
    void DMM_L_to_NCAN_L(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 1);
            BoardCardSendMessage(66, 0);
        }
        else
        {
            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 0);
            BoardCardSendMessage(66, 0);
        }
    }
    //数字万用表DMM负极端口连接到内部NCAN_GND
    void DMM_L_to_NCAN_GND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 1);
            BoardCardSendMessage(66, 1);
        }
        else
        {
            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 0);
            BoardCardSendMessage(66, 0);
        }
    }
    //数字万用表DMM电流端口连接内部NCAN_H
    void DMM_I_to_NCAN_H(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(69, 0);
            BoardCardSendMessage(71, 0);
            BoardCardSendMessage(74, 1);
        }
        else
        {
            BoardCardSendMessage(69, 0);
            BoardCardSendMessage(71, 0);
            BoardCardSendMessage(74, 0);
        }
    }
    //数字万用表DMM电流端口连接内部NCAN_L
    void DMM_I_to_NCAN_L(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(69, 0);
            BoardCardSendMessage(71, 1);
            BoardCardSendMessage(74, 0);
        }
        else
        {
            BoardCardSendMessage(69, 0);
            BoardCardSendMessage(71, 0);
            BoardCardSendMessage(74, 0);
        }
    }
    //--------------CAN一致性测试板卡内部CAN线连接、负载配置-------------
    //CAN工具测试通道x通过CAN1接口连接内部CAN
    void Tool_1CAN_to_NCAN(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(157, 1);
        }
        else
        {
            BoardCardSendMessage(157, 0);
        }
    }
    //CAN干扰仪通过CAN2接口连接内部CAN
    void Tool_2CAN_to_NCAN(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(77, 1);
        }
        else
        {
            BoardCardSendMessage(77, 0);
        }
    }
    //CAN一致性板卡内部预留的FDCAN1连接内部CAN，可用于SelfACK和特定报文Trigger Out到I/O口方波
    void FDCAN1_to_NCAN(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(242, 1);
        }
        else
        {
            BoardCardSendMessage(242, 0);
        }
    }
    //前面板诊断CAN口连接内部CAN
    void EXTCAN_to_NCAN(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(192, 1);
        }
        else
        {
            BoardCardSendMessage(192, 0);
        }
    }
    //内部NCAN负载
    //内部CAN的终端电阻设置 CANTerminalR { Res0, Res60, Res120 }
    void NCAN_Terminal_R_Config(CANTerminalR trload)
    {
        switch (trload)
        {
        case Res0:   //不添加电阻
            BoardCardSendMessage(196, 0);
            BoardCardSendMessage(197, 0);
            break;
        case Res60:  //电阻60Ω
            BoardCardSendMessage(196, 1);
            BoardCardSendMessage(197, 1);
            break;
        case Res120:  //电阻120Ω
            BoardCardSendMessage(196, 1);
            BoardCardSendMessage(197, 0);
            break;
        default:
            break;
        }
    }
    //内部CAN的电阻电容负载电路设置
    void NCAN_Rx_C1C2C3_Config(CANRCLoad rcload)
    {
        switch (rcload)
        {
        case Rx0C0:    //Rx不接，电容负载C1/C2/C3不接
            BoardCardSendMessage(150, 0); //Rx，通道1
            BoardCardSendMessage(144, 0); //C1，通道2
            BoardCardSendMessage(148, 0); //C2，通道3
            BoardCardSendMessage(149, 0); //C3，通道1
            break;
        case Rx0Cmin:    //Rx不接，电容负载最小C1=100pF，C2=100pF，C3=0pF不接
            BoardCardSendMessage(150, 0);
            BoardCardSendMessage(144, 1);
            BoardCardSendMessage(148, 1);
            BoardCardSendMessage(149, 0);
            break;
        case Rx0Cmax:    //Rx不接，电容负载最大C1=4.7nF，C2=4.7nF，C3=3.3nF
            BoardCardSendMessage(150, 0);
            BoardCardSendMessage(144, 1);
            BoardCardSendMessage(148, 1);
            BoardCardSendMessage(149, 1);
            break;
        case Rx180Cmin:  //Rx=180Ω，电容负载最小C1=100pF，C2=100pF，C3=0pF不接
            BoardCardSendMessage(150, 1);
            BoardCardSendMessage(144, 1);
            BoardCardSendMessage(148, 1);
            BoardCardSendMessage(149, 0);
            break;
        case Rx180Cmax:  //Rx=180Ω，电容负载最大C1=4.7nF，C2=4.7nF，C3=3.3nF
            BoardCardSendMessage(150, 1);
            BoardCardSendMessage(144, 1);
            BoardCardSendMessage(148, 1);
            BoardCardSendMessage(149, 1);
            break;
        default:
            break;
        }
    }
    //--------------CAN一致性测试板卡内部CAN线故障模拟-------------
    //NCAN_H和NCAN_L短接
    void NCAN_H_to_NCAN_L(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(153, 1);
        }
        else
        {
            BoardCardSendMessage(153, 0);
        }
    }
    //NCAN_H和NCAN_GND短接
    void NCAN_H_to_NCAN_GND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(152, 1);
        }
        else
        {
            BoardCardSendMessage(152, 0);
        }
    }
    //NCAN_L和NCAN_GND短接
    void NCAN_L_to_NCAN_GND(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(151, 1);
        }
        else
        {
            BoardCardSendMessage(151, 0);
        }
    }
    //NCAN_H连接测试线1
    void NCAN_H_to_Line1(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(81, 0);
            BoardCardSendMessage(80, 1);
        }
        else
        {
            BoardCardSendMessage(81, 0);
            BoardCardSendMessage(80, 0);
        }
    }
    //NCAN_L连接测试线1
    void NCAN_L_to_Line1(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(81, 1);
            BoardCardSendMessage(80, 0);
        }
        else
        {
            BoardCardSendMessage(81, 0);
            BoardCardSendMessage(80, 0);
        }
    }
    //DUT CAN_HL与内部NCAN_HL交叉相连
    void CAN_HL_Cross_NCAN_HL(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 1);
            BoardCardSendMessage(38, 0);

            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 0);
            BoardCardSendMessage(44, 1);
        }
        else
        {
            CAN_L_to_NCAN_L(ON);
            CAN_H_to_NCAN_H(ON);
        }
    }

    //电流源接CANH
    void NCAN_HL_to_CurrentSrc(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(168, 1);
            BoardCardSendMessage(167, 1);
        }
        else
        {
            BoardCardSendMessage(168, 0);
            BoardCardSendMessage(167, 0);
        }
    }

    //电流源接CANL
    void NCAN_LH_to_CurrentSrc(CtrlStatus status)
    {
        if (ON == status)
        {
            BoardCardSendMessage(169, 1);
            BoardCardSendMessage(167, 1);
        }
        else
        {
            BoardCardSendMessage(169, 0);
            BoardCardSendMessage(167, 0);
        }
    }
};

/*---------------------------------------------------函数---------------------------------------------------*/
/*--------------------示波器Picoscope----------------*/
/*
s32 Picoscope_Create_New_Device(s32* ADeviceHandle, const s32 ADeviceType, const char* ASerialNo)
{
    return Instr_Scope_Picoscope.picoscope_create_new_device(ADeviceHandle, ADeviceType, ASerialNo);
}
s32 Picoscope_Connect_Device(s32 ADeviceHandle, const char* ASerialNo)
{
    return Instr_Scope_Picoscope.picoscope_connect_device(ADeviceHandle, ASerialNo);
}
s32 Picoscope_Delete_Device(s32 ADeviceHandle)
{
    return Instr_Scope_Picoscope.picoscope_delete_device(ADeviceHandle);
}
s32 Picoscope_Switch_Device(s32 ADeviceHandle, const s32 ADeviceType)
{
    return Instr_Scope_Picoscope.picoscope_switch_device(ADeviceHandle, ADeviceType);
}
s32 Picoscope_Set_Resolution(s32 ADeviceHandle, const s32 AResolution)
{
    return Instr_Scope_Picoscope.picoscope_set_resolution(ADeviceHandle, AResolution);
}
s32 Picoscope_Get_Interval_Nanoseconds(s32 ADeviceHandle, const float* ANanoseconds)
{
    return Instr_Scope_Picoscope.picoscope_get_interval_nanoseconds(ADeviceHandle, ANanoseconds);
}
s32 Picoscope_Get_Version(char** AVersion)
{
    return Instr_Scope_Picoscope.picoscope_get_version(AVersion);
}
s32 Picoscope_Set_Timebase(const s32 ADeviceHandle, const s32 AScreenPointCount, const double ASecondsPerDiv)
{
    return Instr_Scope_Picoscope.picoscope_set_timebase(ADeviceHandle, AScreenPointCount, ASecondsPerDiv);
}
s32 Picoscope_Set_Channel(const s32 ADeviceHandle, const s32 AChn, const s32 AOnOff, const u8 A10X, const u8 AIsDC, const u8 ARange, const double AOffset)
{
    return Instr_Scope_Picoscope.picoscope_set_channel(ADeviceHandle, AChn, AOnOff, A10X, AIsDC, ARange, AOffset);
}
s32 Picoscope_Set_Trigger(const s32 ADeviceHandle, const s32 AChn, const u8 AEnabledTrigger, const float ATriggerVoltage, const u8 AChannelTriggerDirection, const u32 AChannelTriggerDelay, const s16 AChannelTriggerAutoMS, const float APreTriggerRatio, const bool AIsSingleTrigger)
{
    return Instr_Scope_Picoscope.picoscope_set_trigger(ADeviceHandle, AChn, AEnabledTrigger, ATriggerVoltage, AChannelTriggerDirection, AChannelTriggerDelay, AChannelTriggerAutoMS, APreTriggerRatio, AIsSingleTrigger);
}
s32 Picoscope_Set_Run(const s32 ADeviceHandle)
{
    return Instr_Scope_Picoscope.picoscope_set_run(ADeviceHandle);
}
s32 Picoscope_Set_Stop(const s32 ADeviceHandle)
{
    return Instr_Scope_Picoscope.picoscope_set_stop(ADeviceHandle);
}
s32 Picoscope_Get_Screen_Datas(const s32 ADeviceHandle, const s32 AChn, const float* ADataBuffer, s32* ABuffsize)
{
    return Instr_Scope_Picoscope.picoscope_get_screen_datas(ADeviceHandle, AChn, ADataBuffer, ABuffsize);
}
s32 Picoscope_Get_Screenshot(const s32 ADeviceHandle, const char* AImageFileName, s32 Astyle)
{
    return Instr_Scope_Picoscope.picoscope_get_screenshot(ADeviceHandle, AImageFileName, Astyle);
}
s32 Picoscope_Reset_Connection(const s32 ADeviceHandle)
{
    return Instr_Scope_Picoscope.picoscope_reset_connection(ADeviceHandle);
}
s32 Picoscope_Start_Minmax_Measurement(const s32 ADeviceHandle, const s32 AChn)
{
    return Instr_Scope_Picoscope.picoscope_start_minmax_measurement(ADeviceHandle, AChn);
}
s32 Picoscope_Stop_Minmax_Measurement(const s32 ADeviceHandle, const s32 AChn)
{
    return Instr_Scope_Picoscope.picoscope_stop_minmax_measurement(ADeviceHandle, AChn);
}
s32 Picoscope_Get_Minmax_Measurement(const s32 ADeviceHandle, const s32 AChn, const double* AMinValue, const double* AMaxValue)
{
    return Instr_Scope_Picoscope.picoscope_get_minmax_measurement(ADeviceHandle, AChn, AMinValue, AMaxValue);
}
*/
/*--------------------主电源IT65XX-------------------*/
//创建设备连接
s32 MainPower_Create_Device() //原生VISA无Delete Device操作，请在TestFinish中关闭所有VISA设备连接 Visa_Finalize_Devices()
{
    return  main_power::instance()->DeviceCreat();
}
//设置输出电压和最大电流
s32 MainPower_Set_VoltageCurrent(double V, double I)
{
    return main_power::instance()->Set_Voltage(V, I);
}
//电源上电
s32 MainPower_ON()
{
    TestReport_HTML_StepDescription("MainPower_ON");
    return main_power::instance()->Power_On();
}
//电源断电
s32 MainPower_OFF()
{
    TestReport_HTML_StepDescription("MainPower_OFF");
    return main_power::instance()->Power_Off();
}
//获取当前电压
double MainPower_Get_Voltage()
{
    return main_power::instance()->Read_Val();
}
//获取当前电流
double MainPower_Get_Current()
{
    return main_power::instance()->Read_Cur();
}
/*--------------------双极性电源IT6432-------------------*/
//创建设备连接
s32 BipolarPower_Create_Device() //原生VISA无Delete Device操作，请在TestFinish中关闭所有VISA设备连接 Visa_Finalize_Devices()
{
    return bipolar_power::instance()->DeviceCreat();
}
//设置当前电压和最大电流
s32 BipolarPower_Set_VoltageCurrent(double V, double I)
{
    return bipolar_power::instance()->Set_Voltage(V, I);
}
//电源上电
s32 BipolarPower_ON()
{
    bipolar_power::instance()->Clear_Error();
    return bipolar_power::instance()->Power_On();
}
//电源断电
s32 BipolarPower_OFF()
{
    bipolar_power::instance()->Clear_Error();
    return bipolar_power::instance()->Power_Off();
}
//读取当前电压
double BipolarPower_Get_Voltage()
{
    return bipolar_power::instance()->Read_Val();
}
//读取当前电流
double BipolarPower_Get_Current()
{
    return bipolar_power::instance()->Read_Cur();
}
//清除故障
s32 BipolarPower_Clear_Error()
{
    return bipolar_power::instance()->Clear_Error();
}

/*--------------------数字万用表34461A-------------------*/
//数字万用表初始化
s32 DMM_Create_Device() //原生VISA无Delete Device操作，请在TestFinish中关闭所有VISA设备连接 Visa_Finalize_Devices()
{
    return rigol_multimeter::instance()->DeviceCreat();
}
double DMM_Get_Resistance()
{
    return rigol_multimeter::instance()->Read_Resistance();
}
double DMM_Get_DCCurrent()
{
    return rigol_multimeter::instance()->Read_Current_DC();
}
double DMM_Get_ACCurrent()
{
    return rigol_multimeter::instance()->Read_Current_AC();
}
double DMM_Get_DCVoltage()
{
    return rigol_multimeter::instance()->Read_Voltage_DC();
}
double DMM_Get_ACVoltage()
{
    return rigol_multimeter::instance()->Read_Voltage_AC();
}

/*--------------------CAN一致性测试板卡-------------------*/
//CAN一致性板卡初始化
void BoardCard_Init()
{
    KL30ON = 0;
    KL31ON = 0;
    boardcard::instance()->InitRelays();
    boardcard::instance()->Init_ResCapCH();
}
//设置电阻三通道，范围限于基于14个基础参数进行任意相加：2.5/5/10/20/40/80/160/320/640/1280/2560/5120/10220/20495Ω
void ResCapCH_Set_Resistance(u8 ch, float res)
{
    boardcard::instance()->Set_Resistance(ch, res);
}
//设置电容三通道，范围基于14个基础参数进行任意相加：12/27/47/100/200/400/800/1600/3200/6400/12800/22000/47000/100000pF
void ResCapCH_Set_Capacitance(u8 ch, float cap)
{
    boardcard::instance()->Set_Capacitance(ch, cap);
}

//----------------DUT接口配置---------------
//KL30上下电
void KL30(CtrlStatus status)
{
    boardcard::instance()->KL30_to_NBAT(status);
}
//KL31上下电
void KL31(CtrlStatus status)
{
    boardcard::instance()->KL31_to_NGND(status);
}
//KL31连接地偏电
void KL31_to_SinkPwr(CtrlStatus status)
{
    //DUT KL31-1连接地偏BAT
    boardcard::instance()->KL31_1_to_ShiftBAT(status);
    //电流型电源SinkPwr连接地偏BAT和内部GND
    boardcard::instance()->SinkPwr_to_ShiftBAT_NGND(status);
}
//KL15上下电
void KL15(CtrlStatus status)
{
    boardcard::instance()->KL15_to_NBAT(status);
}
//KLR上下电
void KLR(CtrlStatus status)
{
    boardcard::instance()->KLR_to_NBAT(status);
}
//WAKEUP上下高电
void WAKEUP_to_NBAT(CtrlStatus status)
{
    boardcard::instance()->WAKE_UP_to_NBAT(status);
}
//WAKEUP上下低电
void WAKEUP_to_NGND(CtrlStatus status)
{
    boardcard::instance()->WAKE_UP_to_NGND(status);
}
//DUT CAN_H连接到内部CAN_H
void CANH_to_NCANH(CtrlStatus status)
{
    boardcard::instance()->CAN_H_to_NCAN_H(status);
}
//DUT CAN_H连接到内部CAN_L
void CANH_to_NCANL(CtrlStatus status)
{
    boardcard::instance()->NCAN_H_to_NCAN_L(status);
}
//DUT CAN_L连接到内部CAN_L
void CANL_to_NCANL(CtrlStatus status)
{
    boardcard::instance()->CAN_L_to_NCAN_L(status);
}
//DUT CAN_GND连接到内部CAN_GND
void CANGND_to_NCANGND(CtrlStatus status)
{
    boardcard::instance()->CAN_GND_to_NCAN_GND(status);
}
//DUT CAN连接到内部CAN
void CAN_to_NCAN(CtrlStatus status)
{
    boardcard::instance()->CAN_H_to_NCAN_H(status);
    boardcard::instance()->CAN_L_to_NCAN_L(status);
    boardcard::instance()->CAN_GND_to_NCAN_GND(status);
}
//DUT CAN_GND连接到内部GND
void CANGND_to_NGND(CtrlStatus status)
{
    boardcard::instance()->CAN_GND_to_NGND(status);
}

//----------------电源接口配置---------------
//主电源MainPwr连接内部BAT、GND
void MainPwr_to_NBATGND(CtrlStatus status)
{
    boardcard::instance()->MainPwr_to_NBAT_NGND(status);
}
//信号电源SigPwr连接唤醒BAT和内部GND
void SigPwr_to_WakeBATNGND(CtrlStatus status)
{
    boardcard::instance()->SigPwr_to_WakeBAT_NGND(status);
}
//信号电源SigPwr输入端电源口VCC+/-选择
void SigPwr_VCCHorVCCL(CtrlStatus status)
{
    boardcard::instance()->SigPwr_VCCH_or_VCCL(status);
}
//信号电源SigPwr接内部NCAN_H
void SigPwr_to_NCANH(CtrlStatus status)
{
    boardcard::instance()->SigPwr_H_to_NCAN_H(status);
}
//信号电源SigPwr接内部NCAN_L
void SigPwr_to_NCANL(CtrlStatus status)
{
    boardcard::instance()->SigPwr_H_to_NCAN_L(status);
}
//信号电源SigPwr接内部NCAN_GND
void SigPwrGND_to_NCANGND(CtrlStatus status)
{
    boardcard::instance()->SigPwr_GND_to_NCAN_GND(status);
}
//电流源正极串联200Ω、负极串联电阻矩阵通道ch2后，接内部NCAN
void CurrentSrc_200Res2_NCAN(CtrlStatus status)
{
    boardcard::instance()->CurrentSrc_to_Res2_to_NCAN(status);
}

//----------------示波器接口配置---------------
//示波器CH1/CH2/GND连接内部CANH/CANL/CANGND
void ScopeCH12_to_NCANHL(CtrlStatus status)
{
    boardcard::instance()->Scope_CH1_to_NCAN_H(status);
    boardcard::instance()->Scope_CH2_to_NCAN_L(status);
    boardcard::instance()->Scope_GND_to_NCAN_GND(status);
}
//示波器CH3连接KL30
void ScopeCH3_to_KL30(CtrlStatus status)
{
    boardcard::instance()->Scope_CH3_to_KL30(status);
    boardcard::instance()->Scope_GND_to_NGND(status);
}
//示波器CH3连接KL15
void ScopeCH3_to_KL15(CtrlStatus status)
{
    boardcard::instance()->Scope_CH3_to_KL15(status);
    boardcard::instance()->Scope_GND_to_NGND(status);
}
//示波器CH3连接CAN_RXD
void ScopeCH3_to_CANRXD(CtrlStatus status)
{
    boardcard::instance()->Scope_CH3_to_CAN_RXD(status);
}
//示波器CH4连接CAN_TXD
void ScopeCH4_to_CANTXD(CtrlStatus status)
{
    boardcard::instance()->Scope_CH4_to_CAN_TXD(status);
}
//示波器CH4连接I/O1
void ScopeCH4_to_IO1(CtrlStatus status)
{
    boardcard::instance()->Scope_CH4_to_IO1(status);
    boardcard::instance()->N1GND_to_Line4_to_NCAN_GND(status);
    boardcard::instance()->Scope_GND_to_NCAN_GND(status);
}
//示波器GND连接内部NGND
void ScopeGND_to_NGND(CtrlStatus status)
{
    boardcard::instance()->Scope_GND_to_NGND(status);
}

//----------------数字万用表接口配置---------------
//数字万用表DMM正负极端口连接Wakeup和内部GND
void DMM_to_WAKEUPNGND(CtrlStatus status)
{
    boardcard::instance()->DMM_to_WAKEUP_NGND(status);
}
//数字万用表DMM正负极端口连接反馈BAT(DUT KL30)和反馈GND(DUT KL31_1)
void DMM_to_KL30KL31(CtrlStatus status)
{
    boardcard::instance()->DMM_to_KL30_KL31(status);
}
//数字万用表DMM连接到内部NCAN_H和内部NCAN_GND
void DMM_to_NCANH(CtrlStatus status)
{
    boardcard::instance()->DMM_H_to_NCAN_H(status);
    boardcard::instance()->DMM_L_to_NCAN_GND(status);
}
//数字万用表DMM连接到内部NCAN_L和内部NCAN_GND
void DMM_to_NCANL(CtrlStatus status)
{
    boardcard::instance()->DMM_H_to_NCAN_L(status);
    boardcard::instance()->DMM_L_to_NCAN_GND(status);
}
//数字万用表DMM连接到内部NCAN_H和内部NCAN_L
void DMM_to_NCANHL(CtrlStatus status)
{
    boardcard::instance()->DMM_H_to_NCAN_H(status);
    boardcard::instance()->DMM_L_to_NCAN_L(status);
}
//--------------CAN一致性测试板卡内部CAN线连接、负载配置-------------
//CAN工具测试通道连接内部CAN
void Tool1CAN_to_NCAN(CtrlStatus status)
{
    boardcard::instance()->Tool_1CAN_to_NCAN(status);
}
//CAN干扰仪连接内部CAN
void Tool2CAN_to_NCAN(CtrlStatus status)
{
    boardcard::instance()->Tool_2CAN_to_NCAN(status);
}
/* CAN一致性板卡内部预留的FDCAN1通道可用于SelfACK和特定报文Trigger Out到I/O口方波
   SelfACK实现办法：FDCAN1连接NCAN，FDCAN1波特率设置(+GND连接)
   报文Trigger方法：FDCAN1连接NCAN，FDCAN1 Trigger设置，I/O1连接示波器ch4(+GND连接)
*/
//内部FDCAN1_H/L/GND连接内部NCAN_H/L/GND
void NFDCAN1_to_NCAN(CtrlStatus status)
{
    boardcard::instance()->FDCAN1_to_NCAN(status);
}
//内部CAN负载设置
//内部CAN的终端电阻设置
void NCAN_TerminalR_Config(CANTerminalR trload)
{
    boardcard::instance()->NCAN_Terminal_R_Config(trload);
}
//内部CAN的电阻Rx/电容C1C2C3负载电路配置，不含终端电阻
void NCAN_RxC1C2C3_Config(CANRCLoad rcload)
{
    boardcard::instance()->NCAN_Rx_C1C2C3_Config(rcload);
}

//--------------CAN一致性测试板卡内部CAN线故障模拟-------------
//内部CAN_H和CAN_L短接
void NCANH_to_NCANL(CtrlStatus status)
{
    boardcard::instance()->NCAN_H_to_NCAN_L(status);
}
//内部CAN_H和内部GND短接
void NCANH_to_NGND(CtrlStatus status)
{
    boardcard::instance()->NCAN_H_to_NCAN_GND(status);
    boardcard::instance()->CAN_GND_to_NCAN_GND(status);
    boardcard::instance()->CAN_GND_to_NGND(status);
}
//内部CAN_L和内部GND短接
void NCANL_to_NGND(CtrlStatus status)
{
    boardcard::instance()->NCAN_L_to_NCAN_GND(status);
    boardcard::instance()->CAN_GND_to_NCAN_GND(status);
    boardcard::instance()->CAN_GND_to_NGND(status);
}
//内部CAN_HL和内部GND短接
void NCANHL_to_NGND(CtrlStatus status)
{
    boardcard::instance()->NCAN_H_to_NCAN_GND(status);
    boardcard::instance()->NCAN_L_to_NCAN_GND(status);
    boardcard::instance()->CAN_GND_to_NCAN_GND(status);
    boardcard::instance()->CAN_GND_to_NGND(status);
}
//内部CAN_H和KL30短接
void NCANH_to_KL30(CtrlStatus status)
{
    boardcard::instance()->NCAN_H_to_Line1(status);
    boardcard::instance()->KL30_to_Line1(status);
}
//内部CAN_L和KL30短接
void NCANL_to_KL30(CtrlStatus status)
{
    boardcard::instance()->NCAN_L_to_Line1(status);
    boardcard::instance()->KL30_to_Line1(status);
}
//内部CAN_HL和KL30短接
void NCANHL_to_KL30(CtrlStatus status)
{
    boardcard::instance()->NCAN_H_to_Line1(status);
    boardcard::instance()->NCAN_H_to_NCAN_L(status);
    boardcard::instance()->KL30_to_Line1(status);
}
//CAN_HL与内部NCAN_HL交叉相连
void CANHL_Cross_NCANHL(CtrlStatus status)
{
    boardcard::instance()->CAN_HL_Cross_NCAN_HL(status);
}
//NCAN_HL连接到CurrentSource电流源
void NCANHL_to_CurrentSrc(CtrlStatus status)
{
    boardcard::instance()->NCAN_HL_to_CurrentSrc(status);
}
//NCAN_LH连接到CurrentSource电流源
void NCANLH_to_CurrentSrc(CtrlStatus status)
{
    boardcard::instance()->NCAN_LH_to_CurrentSrc(status);
}

/*--------------------VISA设备操作------------------*/
//关闭所有VISA设备
void Visa_Finalize_Devices()
{
    rtlTSInstrument.visa_finalizelib();
}

///*--------------------DSOX3024G示波器------------------*/
//int number = 100;
//char* resultStr = NULL;
//s32 dso3024handle = 0;
////连接示波器
//void Scope_Create() {
//    //bool isScopeReady = Instr_Scope_AgilentDSOX3024.ts_create_scope(&dso3024handle, "USB0::0x2A8D::0x1766::MY64190156::0::INSTR");//楼下机柜
//    bool isScopeReady = Instr_Scope_AgilentDSOX3024.ts_create_scope(&dso3024handle, "USB0::0x2A8D::0x1766::MY64190152::0::INSTR");//客户现场机柜
//    if (isScopeReady == 0)
//    {
//        app.log_text("示波器连接成功！", lvlOK);
//    }
//    else
//    {
//        app.log_text("示波器连接失败！", lvlError);
//    }
//    app.wait(1000, "");
//
//
//    test.log_info("连接成功", lvlOK);
//    //读取示波器信息
//    Instr_Scope_AgilentDSOX3024.ts_scope_query(dso3024handle, "*IDN?", (const char**)&resultStr);
//    //TestInfoPrint("-------------%d", dso3024handle);调试用
//}
//
////断开示波器
//void Scope_Delete() {
//    bool isScopeDelete = Instr_Scope_AgilentDSOX3024.ts_delete_scope(dso3024handle);
//    if (isScopeDelete == 0)
//    {
//        app.log_text("示波器断开成功！", lvlOK);
//    }
//    else
//    {
//        app.log_text("示波器断开失败！", lvlError);
//    }
//    app.wait(1000, "");
//}
//
////重置示波器
//void Scope_reset() {
//    bool isScopeReset;
//    isScopeReset = Instr_Scope_AgilentDSOX3024.ts_scope_reset(dso3024handle);
//    if (isScopeReset == 0)
//    {
//        app.log_text("示波器重置成功！", lvlOK);
//    }
//    else
//    {
//        app.log_text("示波器重置失败！", lvlError);
//    }
//    app.wait(1000, "");
//}
//
////设置示波器通道
//void set_Scope_Channel(ScopeChannel channel, ScopeChannelState state, double timebase, double rangeValue) {
//    bool isScopeOpen;
//    if (state == OPEN)
//    {
//        isScopeOpen = Instr_Scope_AgilentDSOX3024.ts_scope_set_channel(dso3024handle, channel, 1);
//        if (isScopeOpen == 0)
//        {
//            app.log_text((std::to_string(channel) + "通道打开成功！").c_str(), lvlOK);
//        }
//        else
//        {
//            app.log_text((std::to_string(channel) + "通道打开失败！").c_str(), lvlError);
//        }
//
//        Instr_Scope_AgilentDSOX3024.ts_scope_set_timebase_range(dso3024handle, timebase);
//        Instr_Scope_AgilentDSOX3024.ts_scope_set_vertical_range(dso3024handle, channel, rangeValue);
//    }
//    else
//    {
//        isScopeOpen = Instr_Scope_AgilentDSOX3024.ts_scope_set_channel(dso3024handle, channel, 0);
//        if (isScopeOpen == 0)
//        {
//            app.log_text((std::to_string(channel) + "通道关闭成功！").c_str(), lvlOK);
//        }
//        else
//        {
//            app.log_text((std::to_string(channel) + "通道关闭失败！").c_str(), lvlError);
//        }
//    }
//    app.wait(1000, "");
//}
//
////设置差分电压通道
//void set_Scope_MathChannel()
//{
//    Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":FUNCtion:OPERation SUBTract");
//}
//
//
////示波器触发类型以及触发电压
//void set_Scope_TriggerMode(ScopeChannel channel, ScopeTriggerType triggerType, double triggerVoltage) {
//    bool isScopeTriggerMode, isScopeTriggerVoltage;
//    isScopeTriggerMode = Instr_Scope_AgilentDSOX3024.ts_scope_set_trigger_edge_slope(dso3024handle, triggerType);//上升沿触发
//    app.wait(500, "");
//    isScopeTriggerVoltage = Instr_Scope_AgilentDSOX3024.ts_scope_set_trigger_edge_level(dso3024handle, channel, triggerVoltage);//设置触发电压
//    if (isScopeTriggerMode == 0 && isScopeTriggerVoltage == 0)
//    {
//        app.log_text("示波器通道触发设置成功！", lvlOK);
//
//    }
//    else
//    {
//        app.log_text("示波器重置失败！", lvlError);
//    }
//
//    app.wait(1000, "");
//}
//
////示波器强制触发
//void set_Scope_Force_Trigger() {
//    if (Instr_Scope_AgilentDSOX3024.ts_scope_force_trigger(dso3024handle) == 0x00)
//        app.log_text("Force trigger success", lvlOK);
//    else
//        app.log_text("Force trigger failed", lvlError);
//}
//
//void set_Scope_PWIDthMeasure(int channel)
//{
//    if (0 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:PWIDth CHANnel1");
//    }
//    else if (1 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:PWIDth CHANnel2");
//    }
//    else if (2 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:PWIDth MATH");
//    }
//    else {
//        app.log_text("未定义通道", lvlError);
//    }
//}
//
//void set_Scope_RISetime(int channel)
//{
//    if (0 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:RISetime CHANnel1");
//    }
//    else if (1 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:RISetime CHANnel2");
//    }
//    else if (2 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:RISetime MATH");
//    }
//    else {
//        app.log_text("未定义通道", lvlError);
//    }
//}
//
//void set_Scope_FALLtime(int channel)
//{
//    if (0 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:FALLtime CHANnel1");
//    }
//    else if (1 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:FALLtime CHANnel2");
//    }
//    else if (2 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:FALLtime MATH");
//    }
//    else {
//        app.log_text("未定义通道", lvlError);
//    }
//}
//
//void set_Scope_NWIDthMeasure(int channel)
//{
//    if (0 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:NWIDth CHANnel1");
//    }
//    else if (1 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:NWIDth CHANnel2");
//    }
//    else if (2 == channel) {
//        Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":MEASure:PWIDth MATH");
//    }
//    else {
//        app.log_text("未定义通道", lvlError);
//    }
//}
//
////设置示波器配置文件（方便某些测试项）
//void set_Scope_Configuration(ScopeConfigurationFile conf) {
//    bool isConfigSuccess = 0;
//    switch (conf)
//    {
//    case RCL_4:
//        isConfigSuccess = Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, "*RCL 4");
//        break;
//    case RCL_5:
//        isConfigSuccess = Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, "*RCL 5");
//        break;
//    case RCL_6:
//        isConfigSuccess = Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, "*RCL 6");
//        break;
//    case RCL_7:
//        isConfigSuccess = Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, "*RCL 7");
//        break;
//    case RCL_8:
//        isConfigSuccess = Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, "*RCL 8");
//        break;
//    case RCL_9:
//        isConfigSuccess = Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, "*RCL 9");
//        break;
//    default:
//        app.log_text("请输入一个配置文件的参数！", lvlError);
//        break;
//    }
//    if (isConfigSuccess == 0)
//    {
//        app.log_text("示波器配置文件设置成功！", lvlOK);
//    }
//    else
//    {
//        app.log_text("示波器配置文件配置失败！", lvlError);
//    }
//    app.wait(1000, "");
//}
//
////添加示波器测量项
//void set_Scope_Measure(ScopeChannel channel, ScopeMeasurementType measureType) {
//    bool isSetSuccess = 0;
//    isSetSuccess = Instr_Scope_AgilentDSOX3024.ts_scope_add_measurement(dso3024handle, channel, measureType);
//    if (isSetSuccess == 0)
//    {
//        //app.log_text("测量项添加成功！", lvlOK);
//    }
//    else
//    {
//        app.log_text("测量项添加失败！", lvlError);
//    }
//    app.wait(1000, "");
//}
//
////回读已添加测量项的结果
////每个测量项回读6个结果，并按顺序写入数组：当前值，最小值，最大值，平均值，标准差，计数
////@number:代表读取到的结果个数，默认100个:SCOPE_RESULT_ARRAY_SIZE
////最后的0代表是否按表格输出结果，一般为0
//DoubleArray read_Scope_Measurement() {
//    double resultArray[SCOPE_RESULT_ARRAY_SIZE];
//    DoubleArray arr;
//    bool isReadSuccess = 0;
//    memset(&arr, 0, sizeof(DoubleArray));
//    isReadSuccess = Instr_Scope_AgilentDSOX3024.ts_scope_read_measurements(dso3024handle, resultArray, &number, 0);
//    if (isReadSuccess == 0)
//    {
//        app.log_text("测试项读取成功！", lvlOK);
//        for (int i = 0; i < SCOPE_RESULT_ARRAY_SIZE; i++)
//        {
//            //arr.datasArray[i] = resultArray[i];
//            arr.datasArray[i] = resultArray[i];
//        }
//        return arr;
//    }
//    else
//    {
//        app.log_text("测试项读取失败！", lvlError);
//        return arr;
//    }
//    //DoubleArray result = read_Scope_Measurement();
//    app.wait(1000, "");
//}
//
////暂停波形
//void stop_Scope() {
//    bool isStop = 0;
//    isStop = Instr_Scope_AgilentDSOX3024.ts_scope_set_stop(dso3024handle);
//    if (isStop == 0)
//    {
//        app.log_text("示波器暂停成功", lvlOK);
//    }
//    else
//    {
//        app.log_text("示波器暂停失败", lvlError);
//    }
//    app.wait(1000, "");
//}
//
////保存示波器屏幕截图到目录
//void save_Scope_ScreenShot(std::string picname) {
//    bool isSaveSuccess = 0;
//    char ScopePicName[1024];
//    sprintf(ScopePicName, "%s\\ReportData\\%s.png", ReportPath, picname.c_str());
//
//    //TestInfoPrint("---------------%s", ScopePicName);
//    isSaveSuccess = Instr_Scope_AgilentDSOX3024.ts_scope_capture_screen(dso3024handle, ScopePicName, 1);
//    if (isSaveSuccess == 0)
//    {
//        //TestInfoPrintPass("图片保存成功，路径：%s", ScopePicName);
//        app.log_text("图片保存成功", lvlOK);
//    }
//    app.wait(1000, "");
//}
//
////设置电压阈 值（如电压40%-60%）
//void CAN_Scope_THResholds(double Vmax, double Vmid, double Vmin) {
//    char cmd[50];  // 缓冲区大小需大于最终字符串长度（含结尾的'\0'）
//    double threshold1 = Vmax;
//    double threshold2 = Vmid;
//    double threshold3 = Vmin;
//    snprintf(cmd, sizeof(cmd), ":MEASure:DEFine THResholds,PERCent,%.1f,%.1f,%.1f\n", threshold1, threshold2, threshold3);
//    Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, cmd);
//
//
//}
//
////下降沿时间（直接返回测试结果）us
//double Scope_FalltimeMeasure(ScopeChannel channel) {
//    double T_Falltime;
//    DoubleArray resArray;
//    set_Scope_Measure(channel, FALLtime);
//    app.wait(200, "");
//    resArray = read_Scope_Measurement();
//    T_Falltime = resArray.datasArray[0];
//    return T_Falltime;
//}
//
////上升沿时间（直接返回测试结果）us
//double Scope_RisetimeMeasure(ScopeChannel channel) {
//    double T_Risetime;
//    DoubleArray resArray;
//    set_Scope_Measure(channel, RISetime);
//    app.wait(200, "");
//    resArray = read_Scope_Measurement();
//    T_Risetime = resArray.datasArray[0];
//    return T_Risetime;
//}
//
////根据设定好的阈值，获取电压差
//double Scope_YDelta() {
//    char* resultStr = NULL;
//    double res;
//    Instr_Scope_AgilentDSOX3024.ts_scope_query(dso3024handle, ":MARKer:YDELta?", (const char**)&resultStr);
//    res = strtod(resultStr, NULL);
//    return res;
//}
//
////根据设定好的阈值，获取时间差(S)
//double Scope_XDelta() {
//    char* resultStr = NULL;
//    double res;
//    Instr_Scope_AgilentDSOX3024.ts_scope_query(dso3024handle, ":MARKer:XDELta?", (const char**)&resultStr);
//    res = strtod(resultStr, NULL);
//    return res;
//}
//
//void set_Scope_DElay_TriggerMode(std::string DELayTIME) {
//    Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":TRIGger:DELay:ARM:SLOPe NEGative");
//    Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":TRIGger:DELay:ARM:SOURce CHANnel1");
//    std::string command = ":TRIGger:DELay:TDELay:TIME " + DELayTIME;
//    Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, command.c_str());
//    Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":TRIGger:DELay:TRIGger:SLOPe POSitive");
//    Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":TRIGger:DELay:TRIGger:SOURce CHANnel2");
//    Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":TRIGger:LEVel:ASETup");
//    Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":TRIGger:MODE DELay");
//
//}
//
//void set_Scope_SINGle() {
//    Instr_Scope_AgilentDSOX3024.ts_scope_send(dso3024handle, ":SINGle");
//}


//副电源正极接电阻接CAN_L
    //sig接到测试线2
    //can_l接到测试线1
    //电阻连接测试线1'2
void SigPwr_H_to_res1_to_CAN_L(u8 status)
{
    if (1 == status)
    {
        boardcard::instance()->BoardCardSendMessage(8, 0);
        boardcard::instance()->BoardCardSendMessage(180, 1);
        boardcard::instance()->BoardCardSendMessage(179, 0);
        boardcard::instance()->BoardCardSendMessage(182, 1);

        boardcard::instance()->BoardCardSendMessage(96, 1);
        boardcard::instance()->BoardCardSendMessage(99, 0);
        boardcard::instance()->BoardCardSendMessage(107, 1);

        boardcard::instance()->BoardCardSendMessage(100, 0);
        boardcard::instance()->BoardCardSendMessage(97, 0);

        boardcard::instance()->BoardCardSendMessage(41, 1);
        boardcard::instance()->BoardCardSendMessage(42, 0);
        boardcard::instance()->BoardCardSendMessage(44, 0);
    }
    else
    {
        boardcard::instance()->BoardCardSendMessage(8, 0);
        boardcard::instance()->BoardCardSendMessage(180, 0);
        boardcard::instance()->BoardCardSendMessage(179, 0);
        boardcard::instance()->BoardCardSendMessage(182, 0);

        boardcard::instance()->BoardCardSendMessage(96, 0);
        boardcard::instance()->BoardCardSendMessage(99, 0);
        boardcard::instance()->BoardCardSendMessage(107, 0);

        boardcard::instance()->BoardCardSendMessage(100, 0);
        boardcard::instance()->BoardCardSendMessage(97, 0);

        boardcard::instance()->BoardCardSendMessage(41, 0);
        boardcard::instance()->BoardCardSendMessage(42, 0);
        boardcard::instance()->BoardCardSendMessage(44, 0);
    }
}

//副电源正极接电阻接CAN_H,可变电阻矩阵ch2
//sig接测试线2
//CAN_H接测试线1
//电阻连接两端
void SigPwr_H_to_res1_to_CAN_H(u8 status)
{
    if (1 == status)
    {
        boardcard::instance()->BoardCardSendMessage(8, 0);
        boardcard::instance()->BoardCardSendMessage(180, 1);
        boardcard::instance()->BoardCardSendMessage(179, 0);
        boardcard::instance()->BoardCardSendMessage(182, 1);

        boardcard::instance()->BoardCardSendMessage(96, 1);
        boardcard::instance()->BoardCardSendMessage(99, 0);
        boardcard::instance()->BoardCardSendMessage(107, 1);

        boardcard::instance()->BoardCardSendMessage(100, 0);
        boardcard::instance()->BoardCardSendMessage(97, 0);

        boardcard::instance()->BoardCardSendMessage(34, 1);
        boardcard::instance()->BoardCardSendMessage(35, 0);
        boardcard::instance()->BoardCardSendMessage(38, 0);
    }
    else
    {
        boardcard::instance()->BoardCardSendMessage(8, 0);
        boardcard::instance()->BoardCardSendMessage(180, 0);
        boardcard::instance()->BoardCardSendMessage(179, 0);
        boardcard::instance()->BoardCardSendMessage(182, 0);

        boardcard::instance()->BoardCardSendMessage(96, 0);
        boardcard::instance()->BoardCardSendMessage(99, 0);
        boardcard::instance()->BoardCardSendMessage(107, 0);

        boardcard::instance()->BoardCardSendMessage(100, 0);
        boardcard::instance()->BoardCardSendMessage(97, 0);

        boardcard::instance()->BoardCardSendMessage(34, 0);
        boardcard::instance()->BoardCardSendMessage(35, 0);
        boardcard::instance()->BoardCardSendMessage(38, 0);
    }
}

void Add_ResisnceToCAN(float res) {
    if (res != 0) {
        boardcard::instance()->BoardCardSendMessage(150, 1);
        ResCapCH_Set_Resistance(1, res);
    }
    else {
        boardcard::instance()->BoardCardSendMessage(150, 0);
        ResCapCH_Set_Resistance(1, 40958);
    }
}

//设置供电电压
void SetVoltage(double V, double limitCurrent = 3)
{
    TestInfoPrint("Set Voltage is %.3lfV", V);
    main_power::instance()->Set_Voltage(V, limitCurrent);
}

void KL30(enum Power stuts) {
    if (off == stuts) {
        SetVoltage(0, 0);
        MainPower_OFF();
       // BoardCard_Init();
    }
    else if(on == stuts) {
        SetVoltage(12, 3);
        MainPower_ON();
        //printf("Read Voltage is %.2lf", main_power::instance()->Read_Val());
    }
}

void KL15(enum Power stuts) {

}

s32 DevicesCreat() //原生VISA无Delete Device操作，请在TestFinish中关闭所有VISA设备连接 Visa_Finalize_Devices()
{
    return  main_power::instance()->DeviceCreat();
}

void DevicesInit()
{
    rtlTSInstrument.visa_finalizelib();
}



//设置供电电压
double GetVoltage()
{
    AppWait(1000, "wait Read Voltage");
    return main_power::instance()->Read_Val();
}


//短路
void Short_CANH_To_CANL_Devices()
{
    //boardcard::instance()->CAN_H_to_CAN_L_SendMessage(1);
}

//短路恢复
void ShortRecovery_Devices()
{
    //boardcard::instance()->CAN_H_to_CAN_L_SendMessage(0);
}

#endif
