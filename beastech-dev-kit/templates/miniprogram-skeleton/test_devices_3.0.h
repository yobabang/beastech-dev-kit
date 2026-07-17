#ifndef TEST_DEVICES_3
#define TEST_DEVICES_3
/*---------------------------参数---------------------------*/
#define BoardCardCH CH1

#include <TSMaster.h>
#include "MPLibrary.h"
#include <vector>
#include "TestReport.h"


//定义上下电参数
enum Power { on, off };

//VISA
#define VT_AGILENT_VISA 1
#define VT_NORMAL_VISA 0
//设备类模板，注册类实例
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

//主电源
class main_power : public devices<main_power>
{
    //主电源控制字符串
    char main_power_Serial[1024] =
    "USB0::0xFFFF::0x6500::800630011797510024::0::INSTR";
    //"USB0::0xFFFF::0x6500::800630011787120122::0::INSTR";//geely-电子电器
    //"USB0::0xFFFF::0x6500::800630011787020031::0::INSTR";
    //"USB0::0xFFFF::0x6500::800630011797310005::0::INSTR";//公司
private:
    char Set_voltage_command[100] = "VOLT ";
    char Set_current_command[100] = "CURR ";
    char Set_on_command[100] = "OUTP ON";
    char Set_off_command[100] = "OUTP OFF";
    char Read_voltage_command[100] = "MEAS:VOLT?";
    char Read_current_command[100] = "MEAS:CURR?";
    char IDN[100] = "*IDN?";//读取id
    //const char* buffer = "";
    //char* buffer = (char*)malloc(1000);
    char buffer[1000] = { 0 };
    int bufferLen = 1000;
    s32 main_power_handle = -1;
public:
    main_power()
    {

    }
    ~main_power()
    {
    }

    bool DeviceCreat()
    {
        bool istrue = true;
        int res = -1;
        res = rtlTSInstrument.visa_create_device(&main_power_handle, VT_AGILENT_VISA, main_power_Serial);
        if (res == 0)
        {
            app.log_text("主电源连接成功", lvlOK);
            istrue = true;
        }
        else
        {
            app.log_text("主电源连接失败", lvlError);
            istrue = false;
        }
        return istrue;
    }

    bool Set_Voltage(double voltage = 12, double current_limit = 3)
    {
        char buffer_VC[1024] = "";
        log("Set MainPower Voltage %.2lfV", voltage);
        sprintf(buffer_VC, "%s%s", Set_voltage_command, std::to_string(voltage).c_str());
        rtlTSInstrument.visa_send(main_power_handle, buffer_VC);

        sprintf(buffer_VC, "");
        sprintf(buffer_VC, "%s%s", Set_current_command, std::to_string(current_limit).c_str());
        rtlTSInstrument.visa_send(main_power_handle, buffer_VC);

        return rtlTSInstrument.visa_send(main_power_handle, Set_on_command);
    }

    bool Power_Off()
    {
        return rtlTSInstrument.visa_send(main_power_handle, Set_off_command);
    }

    bool Power_ON()
    {
        return rtlTSInstrument.visa_send(main_power_handle, Set_on_command);
    }
    double Read_Val()
    {
        char resultBuffer[1000];
        bufferLen = 1000;
        rtlTSInstrument.visa_query_bin(main_power_handle, Read_voltage_command, resultBuffer, &bufferLen);

        if (bufferLen >= 1000) bufferLen = 999;
        resultBuffer[bufferLen] = '\0';
        double vol = strtod(resultBuffer, NULL);
        //log("Read_Val is %.3lf", vol);
        return vol;
    }


    double Read_Cur()
    {
        char resultBuffer[1000];
        bufferLen = 1000;
        rtlTSInstrument.visa_query_bin(main_power_handle, Read_current_command, resultBuffer, &bufferLen);
        if (bufferLen >= 1000) bufferLen = 999;
        resultBuffer[bufferLen] = '\0';
        double cur = strtod(resultBuffer, NULL);
        log("Read_Cur is %.3lf", cur);
        return cur;
    }

};


//板卡
class boardcard :public devices<boardcard>
{
public:
    boardcard()
    {
        InitRelays();
    }
    ~boardcard()
    {
        InitRelays();
    }

    //短接
    std::vector<u8> Small = { 32,38,41,42,66,69,71,78,92,94,96,116,119,121,141,144,146,148,159,161,167,168,179,180,182,185,186,189,192,193,194,196,197,214,215,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,242 };
    std::vector<u8> Big = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,29,33,34,35,44,48,49,51,55,56,59,62,63,74,77,79,80,81,84,85,87,88,90,91
                    ,93,97,99,102,103,105,106,107,109,110,112,113,114,118,120,124,127,128,131,134,135,137,142,145,149,150,151,152,153,154,155,156,157,199,200,201,202,
                    203,204,205,206,207,208,209,210,211,212,213,216 };
    void BoardCardSendMessage(u8 RelayNum, u8 status)
    {

        if (count(Big.begin(), Big.end(), RelayNum))
        {
            TCAN f0 = { BoardCardCH,0x5,8,0,0x71700,0,{0x05, RelayNum, status, 0x00, 0x00, 0x00, 0x00, 0x00} };
            com.transmit_can_async(&f0);
            app.wait(0, "");
        }
        if (count(Small.begin(), Small.end(), RelayNum))
        {
            TCAN f0 = { BoardCardCH,0x5,8,0,0x71600,0,{0x05, RelayNum, status, 0x00, 0x00, 0x00, 0x00, 0x00} };
            com.transmit_can_async(&f0);
            app.wait(0, "");
        }

    }
    //继电器初始化
    void InitRelays()
    {
        for (int i = 1; i <= 242; i++)
        {
            /*if ((196 == i)||(197==i)||(150==i)||(149==i)) {
                BoardCardSendMessage(i, 1);
                continue;
            }*/
            BoardCardSendMessage(i, 0);

        }

    }
    //诊断
    void CAN_EXT_to_PC(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(192, 1);
            BoardCardSendMessage(77, 1);
        }
        else
        {
            BoardCardSendMessage(192, 0);
            BoardCardSendMessage(77, 0);
        }
    }
    //CAN_H和CAN_L之间短接
    void CAN_H_to_CAN_L_SendMessage(u8 status)
    {
        /*TCAN f0 = { 0,0x5,8,0,0x00071700,0,{0x05,34,status,0x00,0x00,0x00,0x00,0x00} };
        com.transmit_can_async(&f0);
        app.wait(0, "");*/
        if (1 == status)
        {
            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);

            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 0);
            BoardCardSendMessage(44, 1);
        }
        else
        {
            BoardCardSendMessage(38, 1);
            BoardCardSendMessage(44, 1);
        }
    }
    //CAN_H接地，内部GND接口没有内部GND那根线导致没办法使用
    void CAN_H_to_GND(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);

            BoardCardSendMessage(81, 0);
            BoardCardSendMessage(80, 1);

            BoardCardSendMessage(24, 1);

            BoardCardSendMessage(134, 1);
            BoardCardSendMessage(135, 0);
            BoardCardSendMessage(137, 0);
            /* BoardCardSendMessage(186,0);
               BoardCardSendMessage(185,0);
               BoardCardSendMessage(189,1);*/

        }
        else
        {
            BoardCardSendMessage(81, 0);
            BoardCardSendMessage(80, 0);
            BoardCardSendMessage(24, 0);

            BoardCardSendMessage(134, 0);
            BoardCardSendMessage(135, 0);
            BoardCardSendMessage(137, 0);
        }
    }
    //CAN_L接地 
    void CAN_L_to_GND(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);

            BoardCardSendMessage(81, 1);
            BoardCardSendMessage(80, 0);

            BoardCardSendMessage(24, 1);

            BoardCardSendMessage(134, 1);
            BoardCardSendMessage(135, 0);
            BoardCardSendMessage(137, 0);
            /*BoardCardSendMessage(186, 0);
            BoardCardSendMessage(185, 0);
            BoardCardSendMessage(189, 1);*/
        }
        else
        {
            BoardCardSendMessage(81, 0);
            BoardCardSendMessage(80, 0);

            BoardCardSendMessage(24, 0);

            BoardCardSendMessage(134, 0);
            BoardCardSendMessage(135, 0);
            BoardCardSendMessage(137, 0);

        }
    }
    //CAN_H接主电源
    void MainPwr_H_to_CAN_H(u8 status)
    {
        if (1 == status) {
            //BAT---->测1
            BoardCardSendMessage(156, 1);
            BoardCardSendMessage(146, 0);
            BoardCardSendMessage(145, 0);
            //测1---->内can_h
            BoardCardSendMessage(81, 0);
            BoardCardSendMessage(80, 1);
        }
        else {
            BoardCardSendMessage(156, 0);
            BoardCardSendMessage(81, 0);
            BoardCardSendMessage(80, 0);
        }
    }
    //CAN_L接主电源
    void CAN_L_to_MainPwr_H(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(156, 1);
            BoardCardSendMessage(146, 0);
            BoardCardSendMessage(145, 0);

            BoardCardSendMessage(81, 1);
            BoardCardSendMessage(80, 0);

            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);
        }
        else
        {
            BoardCardSendMessage(156, 0);
            BoardCardSendMessage(81, 0);
            BoardCardSendMessage(80, 0);
        }
    }
    //副电源接电阻接CAN_H
    //副电源接电阻接CAN_L
    //副电源接电阻接地
    //副电源正极接CAN_H 
    void SigPwr_H_to_CAN_H(u8 status)
    {
        if (1 == status)
        {
            //8,180,179,182
            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);

            BoardCardSendMessage(8, 1);
            BoardCardSendMessage(180, 0);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 1);
        }
        else
        {
            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 0);

            BoardCardSendMessage(8, 0);
            BoardCardSendMessage(180, 0);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 0);
        }
    }

    //副电源正极接CAN_L 
    void SigPwr_H_to_CAN_L(u8 status)
    {
        if (1 == status)
        {
            //8,180,179,182
            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);

            BoardCardSendMessage(8, 1);
            BoardCardSendMessage(180, 0);
            BoardCardSendMessage(179, 1);
            BoardCardSendMessage(182, 0);
        }
        else
        {
            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 0);
            BoardCardSendMessage(44, 0);

            BoardCardSendMessage(8, 0);
            BoardCardSendMessage(180, 0);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 0);
        }
    }
    //副电源正极接地 目前可以通过新增的继电器直接打到gnd
    void SigPwr_H_to_GND(u8 status)
    {
        if (1 == status)
        {

            BoardCardSendMessage(48, 0);
            BoardCardSendMessage(49, 0);
            BoardCardSendMessage(51, 1);

            BoardCardSendMessage(8, 1);
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
    //地偏电压
    void offset_of_ground(u8 status) {
        if (status == 1) {
            BoardCardSendMessage(203, 1);
            BoardCardSendMessage(199, 1);
            BoardCardSendMessage(21, 1);
            BoardCardSendMessage(19, 0);


        }
        else
        {
            BoardCardSendMessage(203, 0);
            BoardCardSendMessage(199, 0);
            BoardCardSendMessage(21, 0);
        }
    }
    //测试地偏电压
    void offset_read(u8 status) {
        if (status == 1) {
            BoardCardSendMessage(22, 1);
            BoardCardSendMessage(4, 1);
            BoardCardSendMessage(1, 1);
            BoardCardSendMessage(16, 1);
        }
        else {
            BoardCardSendMessage(22, 0);
            BoardCardSendMessage(4, 0);
            BoardCardSendMessage(1, 0);
            BoardCardSendMessage(16, 0);

        }

    }
    //示波器接CAN_H 
    void oscillograph1_to_CAN_H(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(120, 0);
            BoardCardSendMessage(121, 0);
            BoardCardSendMessage(124, 1);

            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);


        }
        else
        {
            BoardCardSendMessage(120, 1);
        }
    }
    //示波器接CAN_L 
    void oscillograph2_to_CAN_L(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(127, 0);
            BoardCardSendMessage(128, 1);
            BoardCardSendMessage(131, 0);

            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);


        }
        else
        {
            BoardCardSendMessage(127, 1);
        }

    }
    //示波器接主电源
    void oscillograph3_to_MainPwr(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(25, 1);
            BoardCardSendMessage(12, 0);
            BoardCardSendMessage(13, 0);
            BoardCardSendMessage(3, 0);


        }
        else
        {
            BoardCardSendMessage(25, 0);
        }
    }
    //示波器接副电源
    void oscillograph4_to_SigPwr(u8 status)
    {
    }
    //示波器连接
    void oscillograph_connect(u8 status)
    {
        oscillograph1_to_CAN_H(status);
        oscillograph2_to_CAN_L(status);
        oscillograph3_to_MainPwr(status);
        oscillograph4_to_SigPwr(status);

    }

    //示波器地口接内部CAN_GND
    void oscillograph_to_GND(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(134, 0);
            BoardCardSendMessage(135, 1);
            BoardCardSendMessage(137, 1);
        }
        else
        {
            BoardCardSendMessage(135, 0);
            BoardCardSendMessage(137, 0);
        }
    }
    void wakeup_to_res_to_gnd(u8 status) {
        if (1 == status) {
            boardcard::instance()->BoardCardSendMessage(5, 1);
            boardcard::instance()->BoardCardSendMessage(10, 1);
            boardcard::instance()->BoardCardSendMessage(23, 1);
        }
        else
        {
            boardcard::instance()->BoardCardSendMessage(5, 0);
            boardcard::instance()->BoardCardSendMessage(10, 0);
            boardcard::instance()->BoardCardSendMessage(23, 0);
        }
    }
    //地偏接地
    /*
    void SigPwr_to_GND(u8 status)
    {
       if (1 == status)
        {
            BoardCardSendMessage(180,0);
            BoardCardSendMessage(179,0);
            BoardCardSendMessage(182,1);

            BoardCardSendMessage(48, 0);
            BoardCardSendMessage(49, 0);
            BoardCardSendMessage(51, 1);
        }
        else
        {
            BoardCardSendMessage(180,1);
        }
    }
    */
    //万用表接主电源 
    void multr_to_MainPwr(u8 status)
    {
    }
    //万用表接副电源
    void multr_to_SigPwr(u8 status)
    {
    }
    //万用表接CAN_H
    void multr_to_CAN_H(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(55, 0);
            BoardCardSendMessage(56, 0);
            BoardCardSendMessage(59, 1);

            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);
        }
        else
        {
            BoardCardSendMessage(59, 0);


            BoardCardSendMessage(38, 0);
        }
    }
    void multr_to_CAN_L(u8 status) {
        if (1 == status)
        {
            BoardCardSendMessage(55, 0);
            BoardCardSendMessage(56, 1);
            BoardCardSendMessage(59, 0);

            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);
        }
        else {
            BoardCardSendMessage(55, 0);
            BoardCardSendMessage(56, 0);
            BoardCardSendMessage(59, 0);

            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 0);
            BoardCardSendMessage(44, 0);
        }

    };
    //万用表接CAN_L
    void multr_GND_to_CAN_L(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 1);
            BoardCardSendMessage(66, 0);

            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);
        }
        else
        {
            BoardCardSendMessage(62, 1);
        }
    }
    void multr_GND_to_CAN_H(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 0);
            BoardCardSendMessage(66, 1);


        }
        else
        {
            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 0);
            BoardCardSendMessage(66, 0);

        }
    }
    void multr_I_to_CAN_H(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(69, 0);
            BoardCardSendMessage(71, 0);
            BoardCardSendMessage(74, 1);

            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);
        }
        else
        {
            BoardCardSendMessage(69, 0);
            BoardCardSendMessage(71, 0);
            BoardCardSendMessage(74, 0);

            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 0);
        }
    };
    void multr_I_to_CAN_L(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(69, 0);
            BoardCardSendMessage(71, 1);
            BoardCardSendMessage(74, 0);

            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);
        }
        else
        {
            BoardCardSendMessage(69, 1);
        }
    };
    //示波器GND连接到内部CAN_GND
    void multr_GND_to_GND(u8 status) {
        if (1 == status)
        {
            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 1);
            BoardCardSendMessage(66, 1);

            BoardCardSendMessage(48, 0);
            BoardCardSendMessage(49, 1);
            BoardCardSendMessage(51, 1);
        }
        else
        {
            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 0);
            BoardCardSendMessage(66, 0);

            BoardCardSendMessage(48, 0);
            BoardCardSendMessage(49, 0);
            BoardCardSendMessage(51, 0);
        }
    };

    //万用表测电阻
    void multr_mea_resistive(u8 status)
    {
    }
    //万用表测电流
    void multr_mea_current(u8 status)
    {
    }
    //万用表测电平
    void multr_mea_level(u8 status)
    {
    }
    //J3: 1CAN_H 1CAN_L / 2CAN_H 2CAN_L
    //CAN_H连接到内部CAN_H
    void CAN_H_to_NCAN_H(u8 status) {
        if (1 == status)
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
    //CAN_L连接到内部CAN_L
    void CAN_L_to_NCAN_L(u8 status) {
        if (1 == status)
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
    //CAN_H连接PC端
    void CAN_H_to_PC(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);
            //BoardCardSendMessage(157, 1);
        }
        else
        {
            BoardCardSendMessage(34, 1);
        }
    }
    //CAN_L连接PC端
    void CAN_L_to_PC(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);
            BoardCardSendMessage(157, 1);
        }
        else
        {
            BoardCardSendMessage(41, 1);
        }
    }
    void CAN_GND_to_in_CAN_GND(u8 status) {
        if (1 == status) {
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
    //CAN连接PC端
    void CAN_to_PC(u8 status)
    {
        CAN_H_to_PC(status);
        CAN_L_to_PC(status);
        CAN_GND_to_in_CAN_GND(status);
    }
    //KL31(GND3)连接到内部GND
    void KL31GND_to_NGND(u8 status)
    {
        if (1 == status)
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

    //KL30连接主电源
    void KL30_H_to_MainPwr(u8 status)
    {
        if (1 == status)
        {
            //继电器200号？
            BoardCardSendMessage(200, 1);
            BoardCardSendMessage(201, 0);
            BoardCardSendMessage(17, 1);
            BoardCardSendMessage(18, 1);
        }
        else
        {
            BoardCardSendMessage(200, 0);
            BoardCardSendMessage(18, 0);
            //  BoardCardSendMessage(17, 0);
        }
    }
    //**
    void KL30_L_to_MainPwr(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(203, 1);
            BoardCardSendMessage(199, 0);
            BoardCardSendMessage(32, 1);//和内部can_h未接线
            BoardCardSendMessage(216, 1);
            BoardCardSendMessage(17, 1);
            BoardCardSendMessage(18, 1);
        }
        else
        {
            BoardCardSendMessage(203, 0);
            BoardCardSendMessage(216, 0);
            BoardCardSendMessage(18, 0);
        }
    }
    void KL30_to_MainPwr(u8 status)
    {
        KL30_H_to_MainPwr(status);
        KL30_L_to_MainPwr(status);

    }
    //wakeup_to_GND
    void wakeup_to_GND(u8 status)
    {
        if (status == 1) {
            boardcard::instance()->BoardCardSendMessage(5, 1);
            boardcard::instance()->BoardCardSendMessage(10, 1);
            boardcard::instance()->BoardCardSendMessage(23, 1);
        }
        else {

            boardcard::instance()->BoardCardSendMessage(5, 0);
            boardcard::instance()->BoardCardSendMessage(10, 0);
            boardcard::instance()->BoardCardSendMessage(23, 0);
        }
    }
    //wakeup_to_SigPwr_H
    void wakeup_H_to_SigPwr_H(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(8, 0);
            BoardCardSendMessage(19, 1);
            BoardCardSendMessage(7, 0);
            BoardCardSendMessage(214, 1);
            BoardCardSendMessage(213, 1);
        }
        else
        {
            BoardCardSendMessage(8, 1);
            BoardCardSendMessage(19, 0);
            BoardCardSendMessage(7, 0);
            BoardCardSendMessage(214, 0);
            BoardCardSendMessage(213, 0);
        }
    }
    //wakup
    void WakeUp_connect(u8 status)
    {
        wakeup_to_GND(status);
        wakeup_H_to_SigPwr_H(status);
    }
    //wake--->i
    void WakeUp_to_I_to_GND(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(33, 1);

            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 1);
            BoardCardSendMessage(66, 1);
        }
        else {
            BoardCardSendMessage(33, 0);

            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 0);
            BoardCardSendMessage(66, 0);
        }
    }
    //wake-->I--->SigPwr
    void WakeUp_to_I_to_SigPwr_H(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(33, 1);

            BoardCardSendMessage(8, 1);
            BoardCardSendMessage(180, 1);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 0);

            BoardCardSendMessage(62, 1);
            BoardCardSendMessage(63, 0);
            BoardCardSendMessage(66, 0);
        }
        else
        {
            BoardCardSendMessage(33, 0);

            BoardCardSendMessage(8, 0);
            BoardCardSendMessage(180, 0);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 0);

            BoardCardSendMessage(62, 0);
            BoardCardSendMessage(63, 0);
            BoardCardSendMessage(66, 0);
        }
    }
    //Wake-->内部BAT
    void WakeUp_h_to_Pow(u8 status) {
        if (1 == status)
        {
            BoardCardSendMessage(213, 1);
            BoardCardSendMessage(214, 0);

        }
        else
        {
            BoardCardSendMessage(213, 0);
        }

    }
    //WakeUpline 线到示波器3
    void WakeUp_to_oscillograph3(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(25, 1);
            BoardCardSendMessage(12, 0);
            BoardCardSendMessage(13, 1);
            BoardCardSendMessage(9, 1);

            BoardCardSendMessage(24, 1);
        }
        else
        {
            BoardCardSendMessage(25, 0);
            BoardCardSendMessage(24, 0);
        }
    }
    //CAN_H_to_IO_to_CAN_L
    void CAN_H_to_IO_to_CAN_L(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);

            BoardCardSendMessage(153, 1);

            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);
        }
        else
        {

            BoardCardSendMessage(153, 0);

            BoardCardSendMessage(153, 0);

        }

    }
    //CAN_H_to_VarRes_CAN_L,电阻分别是ch2、ch3
    void CAN_H_to_VarRes_CAN_L(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(93, 1);
            BoardCardSendMessage(154, 1);
            BoardCardSendMessage(92, 1);
        }
        else
        {
            BoardCardSendMessage(93, 0);
            BoardCardSendMessage(154, 0);
            BoardCardSendMessage(92, 0);
        }
    }
    //KL15接到内部BAT
    void KL15_to_BAT(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(210, 1);
            BoardCardSendMessage(205, 0);
        }
        else
        {
            BoardCardSendMessage(210, 0);

        }
    }
    //KLR接到内部BAT
    void KLR_to_BAT(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(206, 1);
            BoardCardSendMessage(208, 0);
        }
        else
        {
            BoardCardSendMessage(206, 0);
        }
    }

    //CAN_L过电阻接到电流表的正
    void CAN_L_to_res_to_I(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);

            BoardCardSendMessage(81, 1);
            BoardCardSendMessage(80, 0);

            BoardCardSendMessage(69, 1);
            BoardCardSendMessage(71, 0);
            BoardCardSendMessage(74, 0);
        }
        else
        {
            BoardCardSendMessage(42, 0);
            BoardCardSendMessage(81, 0);
            BoardCardSendMessage(69, 0);
        }
    }
    void SigPwr_L_to_GND(u8 status)
    {

        if (1 == status) {
            BoardCardSendMessage(203, 1);
            BoardCardSendMessage(199, 0);
            BoardCardSendMessage(19, 1);

        }
        else {
            BoardCardSendMessage(203, 0);
            BoardCardSendMessage(199, 0);
            BoardCardSendMessage(19, 0);
        }
    }
    //副电源地线接CAN_H
    void SigPwr_L_to_CAN_H(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);

            BoardCardSendMessage(87, 0);
            BoardCardSendMessage(88, 1);

            BoardCardSendMessage(186, 1);
            BoardCardSendMessage(185, 1);
            BoardCardSendMessage(189, 0);
        }
        else
        {
            BoardCardSendMessage(186, 1);
        }
    }
    //副电源正极接电阻接CAN_L
    //sig接到测试线2
    //can_l接到测试线1
    //电阻连接测试线1'2
    void SigPwr_H_to_res1_to_CAN_L(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(8, 0);
            BoardCardSendMessage(180, 1);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 1);

            BoardCardSendMessage(96, 1);
            BoardCardSendMessage(99, 0);
            BoardCardSendMessage(107, 1);
            BoardCardSendMessage(97, 0);

            BoardCardSendMessage(41, 1);
            BoardCardSendMessage(42, 0);
            BoardCardSendMessage(44, 0);
        }
        else
        {
            BoardCardSendMessage(8, 1);
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
            BoardCardSendMessage(8, 0);
            BoardCardSendMessage(180, 1);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 1);

            BoardCardSendMessage(96, 1);
            BoardCardSendMessage(99, 0);
            BoardCardSendMessage(107, 1);

            BoardCardSendMessage(100, 0);
            BoardCardSendMessage(97, 0);

            BoardCardSendMessage(34, 1);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 0);
        }
        else
        {
            BoardCardSendMessage(8, 1);
        }
    }


    void SigPwr_L_to_CAN_L(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(186, 0);
            BoardCardSendMessage(185, 1);
            BoardCardSendMessage(189, 0);

            BoardCardSendMessage(41, 0);
            BoardCardSendMessage(42, 1);
            BoardCardSendMessage(44, 0);
        }
        else
        {
            BoardCardSendMessage(186, 1);
        }
    }
    void current_source_connnect(u8 status) {
        if (1 == status) {
            BoardCardSendMessage(168, 1);
            BoardCardSendMessage(167, 1);

        }
        else {
            BoardCardSendMessage(168, 0);
            BoardCardSendMessage(167, 0);

        }
    }
    //副电源接电阻接CAN_H (3通道可变电阻矩阵-CH3)
    void SigPwr_H_to_res2_to_CAN_H(u8 status)
    {
        if (1 == status)
        {
            BoardCardSendMessage(8, 0);
            BoardCardSendMessage(180, 1);
            BoardCardSendMessage(179, 0);
            BoardCardSendMessage(182, 0);

            BoardCardSendMessage(94, 1);
            BoardCardSendMessage(106, 0);
            BoardCardSendMessage(103, 0);

            BoardCardSendMessage(102, 0);
            BoardCardSendMessage(105, 1);

            BoardCardSendMessage(87, 0);
            BoardCardSendMessage(88, 1);

            BoardCardSendMessage(34, 0);
            BoardCardSendMessage(35, 0);
            BoardCardSendMessage(38, 1);
        }
        else
        {
            BoardCardSendMessage(88, 0);
        }
    }

};

//短路
void Short_CANH_To_CANL_Devices()
{
    boardcard::instance()->CAN_H_to_CAN_L_SendMessage(1);
}

//短路恢复
void ShortRecovery_Devices()
{
    boardcard::instance()->CAN_H_to_CAN_L_SendMessage(0);
}
/*---------------------------------------------------函数---------------------------------------------------*/


//设置供电电压
void SetVoltage(double V)
{
    main_power::instance()->Set_Voltage(V);
}

//设置供电电压
double GetVoltage()
{
    app.wait(1000, "wait Read Voltage");
    return main_power::instance()->Read_Val();
}

//KL30上下电
void KL30(enum Power status)
{
    if (status == off)
    {
        boardcard::instance()->KL30_to_MainPwr(0);
        TestReport_HTML_StepDescription("KL30_OFF");

    }
    else if (status == on)
    {
        boardcard::instance()->KL30_to_MainPwr(1);
        TestReport_HTML_StepDescription("KL30_ON");
    }
    else
    {
        app.log_text("KL30上电函数出现未知参数，请检查", lvlWarning);
    }
}

//KL15上下电
void KL15(enum Power status)
{
    if (status == off)
    {
        boardcard::instance()->KL15_to_BAT(0);
        TestReport_HTML_StepDescription("KL15_OFF");

    }
    else if (status == on)
    {
        boardcard::instance()->KL15_to_BAT(1);
        TestReport_HTML_StepDescription("KL15_ON");
    }
    else
    {
        app.log_text("KL15上电函数出现未知参数，请检查", lvlWarning);
    }
}

//板卡初始化
void BoardCardInit_Devices()
{
    boardcard::instance()->InitRelays();
}

//CAN_H和CAN_L断开外部电路
inline void Break_CAN_to_PC_Devices()
{
    boardcard::instance()->CAN_to_PC(0);
}

//CAN_H和CAN_L连接外部电路
inline void CAN_to_PC_Devices()
{
    boardcard::instance()->CAN_to_PC(1);
}

//测试用例前板卡的初始化
void SetBoardCardInit_Devices()
{
    //板卡初始化
    boardcard::instance()->InitRelays();

    //电源连接
    boardcard::instance()->KL30_to_MainPwr(1);

    //CAN通道配置
    boardcard::instance()->CAN_to_PC(1);
}

//设备初始化
void DevicesInit()
{
    main_power::instance()->Power_Off();
    app.log_text("Init Power Off", lvlInfo);
    rtlTSInstrument.visa_finalizelib();
}

#endif
