#ifndef _TSDISTURB_H_
#define _TSDISTURB_H_

#include "TSMaster.h"

typedef s32 disturb_handle;

#define FILTER_CHAR_NULL ('\0')
#define FILTER_CHAR_IRRELEVANT ('-1')
#define FILTER_CHAR_DOMINANT ('0')
#define FILTER_CHAR_RECESSIVE ('1')

#define DELAY_TRANSCEIVER_MCP2558 70, 110
#define DELAY_TRANSCEIVER_TJA1051 60, 65

enum disturb_seq_type : u16
{
    DISTURBANCE_SEQUENCE,
    DISTURBANCE_CAN_FRAME
};

enum disturb_trigger_type : u16
{
    TRIGGER_FRAME,
    TRIGGER_DIN_PORT,
    TRIGGER_ERROR_FRAME
};

typedef enum {
    PORT_LOW = 0,
    PORT_1,    // 对应 端口号
    PORT_2,
    PORT_3,
}port_num_t;

    typedef enum {
    SOF_FILED_FLAG,
    ARBITRARY_FIELD_FLAG,
    CONTROL_FIELD_FLAG,
    DATA_FIELD_FLAG,
    CRC_FIELD_FLAG,
    ACK_FIELD_FLAG,
    EOF_FIELD_FLAG,
    INTERFRAME_FLAG,
    ERROR_FIELD_FLAG
}Frame_Field_t;   //干扰场选择

    typedef struct {
    Frame_Field_t Frame_Field;
    unsigned int Pos;     //干扰第几位
    unsigned int Stuff_Pos;  //填充位第几位
}Disturb_Frame_config_t;

    typedef enum {
    CLASSIC_DATA_FRAME,
    EXTENDED_DATA_FRAME,
    CLASSIC_REMOTE_FRAME,
    EXTENDED_REMOTE_FRAME,
    FD_DATA_FRAME,
    FD_EXTENDED_FRAME,
    USER_DEFINED
}Frame_Type_t;

typedef struct {
    unsigned int BseID;
    unsigned int BseID_MASK;
    unsigned int RTRorRES;
    unsigned int RTRorRES_MASK;
    unsigned int IDE;
    unsigned int IDE_MASK;
    unsigned int ExrendedID;
    unsigned int ExrendedID_MASK;
    unsigned int RTRorRES_EXT;
    unsigned int RTRorRES_EXT_MASK;
}Arbitr_Field_t;

typedef struct {
    unsigned int DataWord[16];
    unsigned int DataWordMask[16];
}Data_Field_t;

typedef struct {
    unsigned int R1orFDF;
    unsigned int R1orFDF_MASK;
    unsigned int R0;
    unsigned int R0_MASK;
    unsigned int BRS;
    unsigned int BRS_MASK;
    unsigned int ESI;
    unsigned int ESI_MASK;
    unsigned int DLC;
    unsigned int DLC_MASK;
}Control_Feild_t;

typedef struct {
    unsigned int SBC;
    unsigned int SBC_MASK;
    unsigned int CRC;
    unsigned int CRC_MASK;
    unsigned int CRC_DEL;
    unsigned int CRC_DEL_MASK;
}Crc_Feild_t;

typedef struct {
    unsigned int ACK;
    unsigned int ACK_MASK;
    unsigned int ACK_DEL;
    unsigned int ACK_DEL_MASK;
}Ack_Feild_t;

typedef struct {
    Frame_Type_t Frame_Type;
    Arbitr_Field_t Arbitr_Field;
    Control_Feild_t Control_Feild;
    Data_Field_t Data_Field;
    Crc_Feild_t Crc_Feild;
    Ack_Feild_t Ack_Feild;
}First_Filter_t;     //一层过滤器数据结构

typedef struct canfd_filter__
{
    char ID_base[11];
    char remote_base;
    char IDE;
    char ID_extended[18];
    char remote_extended;
    char FDF;
    char reserved;
    char BRS;
    char ESI;
    char DLC[4];
    char payload[512];
    //char stuff_count[4];
    char CRC[21];
    char CRC_delimiter;
    char ACK_slot;
    char ACK_delimiter;
    //char end_of_frame[7];
} canfd_filter;

typedef struct {
    int bit_num;     //序列中有多少BIT
    unsigned int Frame_format[20];  //表示序列的高低电平
    int div;        //表示1bit长度
}sequence_config_t;

typedef enum {
    FD_0_SEND_FRAME = 0,   //BRS = 0 的 FD帧
    FD_1_SEND_FRAME = 1,   //BRS  = 1 的 FD整
    CLASSIC_SEND_FRAME = 2  // 经典CAN帧
}TX_Frame_Format_t;

struct fd_tx_frame_config_t {
    TX_Frame_Format_t TX_Frame_Format;
    unsigned int TX_ID;
    unsigned int TX_Date1;
};

struct tx_frame_config_t {
    struct fd_tx_frame_config_t fd_tx_frame_config;
    unsigned int Count;    //发送次数
};

typedef struct disturbance_trigger_port__
{
    port_num_t port;
    bool sel_edge;
} disturbance_trigger_port;

typedef struct disturbance_repetitions__
{
    //cycle
    unsigned int Cycles = 1;
    unsigned int Cycles_hold = 0;
    unsigned int Repetitions = 1;
    unsigned int Repetitions_hold = 0;
} disturbance_repetitions;

typedef struct disturbance_options__
{
    bool disturb_dominant;
    bool missbit_trigger;
    bool rx_direction;
    bool tx_direction;
} disturbance_options;

typedef struct th7011_disturbance_trigger__
{
    disturb_trigger_type trigger_type;
    u16 size;
    Disturb_Frame_config_t start_pos;
    union
    {
        First_Filter_t frame;
        disturbance_trigger_port din;
    } trigger_content;
} can_disturbance_trigger;

typedef struct th7011_disturbance_sequence__
{
    disturb_seq_type seq_type;
    u16 size;
    union
    {
        tx_frame_config_t frame;
        sequence_config_t sequence;
    } seq_content;
} can_disturbance_sequence;

#endif