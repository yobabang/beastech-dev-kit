#include "SysAndCan.h"
#include "TSMaster.h"
// 函数定义
// void canEnableStatistics(long channel, long flag) {
//     // 实现逻辑
// }
typedef unsigned __int32 u32 ;

typedef enm FrFrame frFrame;
void canEnableStatistics(long flag) {
    if(flag) {
        com.enable_bus_statistics(true);
    } else {
        com.enable_bus_statistics(false);
    }
} 

void frSetKeySlot(long channel, long channelMask, long keySlotIndex, long keySlotId, long keySlotUsage) {

}
long frSetMode(int channel, int channelMask, u32 mode) {

}

long frSetPOCState(long channel, long ccNumber, long pocState) {

}

long frSetPayloadLengthInByte(frFrame var, word dlc) {

}

void frSetPduFilter(frPDU message, long mode) {

}

void frSetSendFrame(<frame name>, u32 flags) {

}

//void frSetSendFrame(int slotId, int channelMask, int len, int cycleStart, int cycleRepetition, u32 flags, int channel) {}


//void frSetSendFrame(<frame object>) {}

void frSetSendGroup(int slotId, int channelMask, int groupNo, int cycleStart, int cycleRepetition, long flags, int channel) {

}

void frSetSendPDU(<PDU object>) {

}

long frSetTrigger(long channel, long portNo) {

}

void frUpdatePDU(frPDU pdu, u32 flags, int updateCounter) {

}

long frUpdateStatFrame(frFrame frame) {


}

void resetFlexRayCC(int channel) {

}

long resetFlexRayCCEx(int channel, int wuChMask, int wuCount, int wuTxIdle, int wuTxLow, char[] cfg) {

}

long setSignalStartValues(frFrame frame) {}//form 1

//long setSignalStartValues(frFrame frame, byte uninitializedData){}//form 2

//long setSignalStartValues(frPDU pdu){}//form 3

//long setSignalStartValues(frPDU pdu, byte uninitializedData){}//form 4

void output(frSlot fexRaySlot) {}//form 1


//void output(frFrame fexFrame){}//form 2

//void output(frstartCycle flexstartCycle){}//form 3






