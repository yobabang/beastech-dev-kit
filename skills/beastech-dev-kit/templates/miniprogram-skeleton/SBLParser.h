#ifndef SBLParser
#define SBLParser

#include <TSMaster.h>

#define LEN 512

struct _VbfFileHeader
{
    u32 FStartAddress;
    u32 FSize;          //Size of real bytes
    u32 FCheckSum;
    u32 FBlockSize;     //Length of Block >= FSize
    u8  FType;          //0: binaray data; 1: string Key
    u8  FTypeReserved;
    u16 FTypeReserved1; //FReserved2: UInt32;
    u32 FReserved3;
    u32 FReserved4;
    u32 FReserved5;

} VbfFileHeader, * PVbfFileHeader;

struct SofewareNumber {
    char sw_part_number[20];
    char sw_version[10];
    u8 CheckFlag;
} _sw_[10];

u8 APPFileNum = 1;
u8 appindex = 0;

struct u8_array
{
    u8  array[LEN];
    s32 count;
};

struct DataBlcok
{
    u32 StartAddress;   //数据块起始地址
    u32 DataLength;     //数据块长度
    struct u8_array DataBlockCheckSum;       //数据块校验和
    u8  BlockData[16777215];      //数据块数据
};

struct vbfinfo
{
    u32              StartAddress;                     //数据文件起始地址
    u32              DataLength;                       //数据文件长度
    struct u8_array  allcheckcsum;                     //文件的校验和crc32
    u8               DataBlockNumber;                  //数据块数量
    struct DataBlcok VbfDataBlcok[10];                 //数据块
    char             sw_part_number[20];                   //软件版本号
    char             sw_version[10];                       //软件版本
    char             sw_part_type[10];                     //软件类型
    char             data_format_identifier[10];           //数据格式标识符
    char             ecu_address[10];                      //ecu地址
    char             file_checksum[10];                    //文件校验和
    char             call[10];                             //
    char             verification_block_start[10];         //验证块起始地址
    char             verification_block_length[10];        //验证块长度
    char             verification_block_root_hash[200];     //验证块根哈希
    struct u8_array  sw_signature_dev;                 //研发签名
}   VbfFileInfo;

int GetIniKeyString(char* title, char* key, char* filename, char* buf)
{
    FILE* fp;
    int  flag = 0;
    char sTitle[64], * wTmp;
    char sLine[1024];
    sprintf(sTitle, "[%s]", title);

    if (NULL == (fp = fopen(filename, "r"))) {
        app.log_text("无法打开配置文件！", lvlError);
        return -1;
    }
    while (NULL != fgets(sLine, 1024, fp)) {
        // 这是注释行  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准  
                sLine[strlen(sLine) - 1] = '\0';
                fclose(fp);
                while (*(wTmp + 1) == ' ') {
                    wTmp++;
                }
                strcpy(buf, wTmp + 1);
                return 0;
            }
        }
        else {
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准  
                flag = 1; // 找到标题位置  
            }
        }
    }
    fclose(fp);
    return -1;
}

int PutIniKeyString(char* title, char* key, char* val, char* filename)
{
    FILE* fpr, * fpw;
    int  flag = 0;
    char sLine[1024], sTitle[32], * wTmp;
    sprintf(sTitle, "[%s]", title);
    if (NULL == (fpr = fopen(filename, "r")))
        return -1;// 读取原文件  
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// 写入临时文件        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // 如果找到要修改的那一行，则不会执行内部的操作  
            wTmp = strchr(sLine, '=');
            if ((NULL != wTmp) && (1 == flag)) {
                if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准 
                    flag = 2;// 更改值，方便写入文件  
                    sprintf(wTmp + 1, " %s\n", val);
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准
                    flag = 1; // 找到标题位置  
                }
            }
        }
        fputs(sLine, fpw); // 写入临时文件 
    }
    fclose(fpr);
    fclose(fpw);
    sprintf(sLine, "%s.tmp", filename);
    return rename(sLine, filename);// 将临时文件更新到原文件  
}

void GetIniFilePath(char** path)
{
    app.get_configuration_file_path(path);

    strcat(*path, "Toolbox\\GEELY3\\Parameters.ini");

    //log("parameter path is %s", *path);
}

void GetHexFilePath(char* vbfpath, char** hexpath)
{
    try {

        printf("GetHexFilePath");
        app.set_system_var_string("Diagnostic0.下载文件_DataFile", vbfpath);
        app.wait(3000, "");
        char* s;
        app.get_configuration_file_path(&s);

        //strcat(*hexpath, "Diagnostic\\Diagnostic\\");

        char temp[512] = {};

        app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &temp[0]);
        sprintf(*hexpath, "%sDiagnostic\\Diagnostic\\%s", s, temp);
        //strcpy(*hexpath, s);

        log("hex file path is %s", *hexpath);
        printf("End GetHexFilePath");
    }
    catch (...) { log_nok("GetHexFilePath Crash"); app.terminate_application(); }
}

void ReadHexFile(char* path)
{
    log("%s", path);

    FILE* file = fopen(path, "rb"); // 打开hex文件

    if (file == NULL) {
        log("Error opening file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    printf("%x%x%x", fileSize & 0xff, fileSize & 0xff00, fileSize & 0xff0000);
    fseek(file, 0, SEEK_SET);
 
    printf("%x%x%x", ftell(file) & 0xff, ftell(file) & 0xff00, ftell(file) & 0xff0000);

    VbfFileInfo.DataBlockNumber = 0;

    while (fread(&VbfFileHeader, sizeof(VbfFileHeader), 1, file) == 1) {

        printf("%x %x %x", ftell(file)&0xff, ftell(file) & 0xff00, ftell(file) & 0xff0000);

        printf("FStartAddress=%x\n", VbfFileHeader.FStartAddress);
        VbfFileInfo.VbfDataBlcok[VbfFileInfo.DataBlockNumber].StartAddress = VbfFileHeader.FStartAddress;

        printf("blocksize is %x\n", VbfFileHeader.FSize);
        VbfFileInfo.VbfDataBlcok[VbfFileInfo.DataBlockNumber].DataLength = VbfFileHeader.FSize;

        if (VbfFileHeader.FType != 0) {
            // 如果不是数据块类型，跳过本次循环
            fseek(file,VbfFileHeader.FBlockSize,SEEK_CUR);
            continue;
        }

        // 动态分配 hexbuffer 的内存空间
        u8* hexbuffer = (u8*)malloc(VbfFileHeader.FSize*2);
        if (hexbuffer == NULL) {
            printf("Failed to allocate memory.\n");
            fclose(file);
            return;
        }

        if (fread(hexbuffer, 1, VbfFileHeader.FBlockSize, file) != VbfFileHeader.FBlockSize) {
            printf("Error reading data");  printf("%x %x %x", ftell(file) & 0xff, ftell(file) & 0xff00, ftell(file) & 0xff0000);
            if (feof(file)) {
                printf("End of file reached.\n"); // 到达文件结尾
            }
            else if (ferror(file)) {
                printf("Error reading file.\n"); // 读取文件时发生错误
            }
            fclose(file);
            return;
        }

        printf("hexbuffer = %x  %x\n", hexbuffer[0], hexbuffer[1]);

        for (u32 j = 0; j < VbfFileHeader.FSize; j++) {
            VbfFileInfo.VbfDataBlcok[VbfFileInfo.DataBlockNumber].BlockData[j] = hexbuffer[j];
        }

        VbfFileInfo.DataBlockNumber++;

        free(hexbuffer);
    }

    fclose(file); // 关闭文件

    return;
}

void vbfparser(char* vbfIniKeyValue)
{
    char title[100] = "", key[100] = "";
    char buffer[300] = "";
    char parameterpath[300] = "";
    char* par = parameterpath;
    char vbfpath[300] = "";
    char hexpath[300] = "";
    char* hex = hexpath;
    char sysvar[300] = "";
    char filename[300] = "";
    u8 indexflag = 0;

    //获取ini文件路径
    GetIniFilePath(&par);

    app.wait(1000, "");

    //获取SBL路径
    sprintf(title, "DiagUserDefine");
    sprintf(key, vbfIniKeyValue);
    GetIniKeyString(title, key, par, buffer);
    sprintf(vbfpath, buffer);
    printf("vbfpath is &s", vbfpath);


    app.wait(1000, "");

    //解析vbf文件并获取hex文件路径
    GetHexFilePath(vbfpath, &hex);
    app.wait(1000, "");
    printf("path is %s", vbfpath);

    //获取文件名
    app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
    filename[strlen(filename) - 9] = '\0';

    //获取vbf起始地址
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":StartAddress");
    app.get_system_var_uint32(sysvar, &VbfFileInfo.StartAddress);
    log("SBL start address is %2x", VbfFileInfo.StartAddress);

    //获取vbf数据文件长度
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":DataLength");
    app.get_system_var_uint32(sysvar, &VbfFileInfo.DataLength);
    log("SBL start length is %2x", VbfFileInfo.DataLength);


    //获取数据块数据
    for (int i = 0; i < 10; i++)
    {

        sprintf(sysvar, "%s%s%s%d", "Diagnostic0.", filename, ":StartAddress[Block]", i);
        app.get_system_var_uint32(sysvar, &VbfFileInfo.VbfDataBlcok[i].StartAddress);

        if (VbfFileInfo.VbfDataBlcok[i].StartAddress == 0)
        {
            VbfFileInfo.DataBlockNumber = i + 1;
            break;
        }

        log("SBL datablock %d start address is %2x", i, VbfFileInfo.VbfDataBlcok[i].StartAddress);

        sprintf(sysvar, "%s%s%s%d", "Diagnostic0.", filename, ":DataLength[Block]", i);
        app.get_system_var_uint32(sysvar, &VbfFileInfo.VbfDataBlcok[i].DataLength);
        log("SBL datablock %d data length is %2x", i, VbfFileInfo.VbfDataBlcok[i].DataLength);


        sprintf(sysvar, "%s%s%s%d", "Diagnostic0.", filename, ":CheckSum[Block]", i);
        app.get_system_var_uint8_array(sysvar, LEN, &VbfFileInfo.VbfDataBlcok[i].DataBlockCheckSum.count, VbfFileInfo.VbfDataBlcok[i].DataBlockCheckSum.array);
        for (int k = 0; k < VbfFileInfo.VbfDataBlcok[i].DataBlockCheckSum.count; k++)
            log("SBL datablock %d checksum is %2x", i, VbfFileInfo.VbfDataBlcok[i].DataBlockCheckSum.array[k]);
    }

    //获取块数据
    ReadHexFile(hex);


    //获取数据校验和
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":CheckSum");
    app.get_system_var_uint8_array(sysvar, LEN, &VbfFileInfo.allcheckcsum.count, VbfFileInfo.allcheckcsum.array);
    for (int k = 0; k < VbfFileInfo.allcheckcsum.count; k++)
        log("SBL checksum is %2x", VbfFileInfo.allcheckcsum.array[k]);

    //获取软件版本号
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":sw_part_number");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.sw_part_number);
    log("SBL SW partnumber is %s", VbfFileInfo.sw_part_number);


    //获取软件版本
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":sw_version");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.sw_version);
    log("SBL SW version is %s", VbfFileInfo.sw_version);


    //获取软件类型
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":sw_part_type");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.sw_part_type);
    log("SBL SW sw_part_type is %s", VbfFileInfo.sw_part_type);
    if (strcmp(VbfFileInfo.sw_part_type, "SBL") != 0){

        for (int i = 0; i < appindex+1; i++)
        {
            if (0 != strcmp(_sw_[i].sw_part_number, VbfFileInfo.sw_part_number) && strcpy(_sw_[i].sw_version, VbfFileInfo.sw_version) != 0)
            {
                indexflag = 1;
            }
            else
                indexflag = 0;
        }
        if(indexflag==1)
        {
            strcpy(_sw_[appindex].sw_part_number, VbfFileInfo.sw_part_number);
            strcpy(_sw_[appindex].sw_version, VbfFileInfo.sw_version);
            appindex++;
        }
    }

    //获取数据格式标识符
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":data_format_identifier");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.data_format_identifier);
    log("SBL SW data_format_identifier is %s", VbfFileInfo.data_format_identifier);

    //获取ecu地址
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":ecu_address");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.ecu_address);
    log("SBL SW ecu_address is %s", VbfFileInfo.ecu_address);

    //获取文件校验和
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":file_checksum");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.file_checksum);
    log("SBL SW file_checksum is %s", VbfFileInfo.file_checksum);

    //获取call
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":call");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.call);
    log("SBL SW call is %s", VbfFileInfo.call);

    //获取验证块起始地址
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":verification_block_start");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.verification_block_start);
    log("SBL SW verification_block_start is %s", VbfFileInfo.verification_block_start);

    //获取验证块数据长度
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":verification_block_length");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.verification_block_length);
    log("SBL SW verification_block_length is %s", VbfFileInfo.verification_block_length);

    //获取验证块根哈希
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":verification_block_root_hash");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.verification_block_root_hash);
    log("SBL SW verification_block_root_hash is %s", VbfFileInfo.verification_block_root_hash);

    //获取软件研发签名
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":sw_signature_dev");
    app.get_system_var_uint8_array(sysvar, LEN, &VbfFileInfo.sw_signature_dev.count, VbfFileInfo.sw_signature_dev.array);
    for (int k = 0; k < VbfFileInfo.allcheckcsum.count; k++)
        log("SBL checksum is %2x", VbfFileInfo.sw_signature_dev.array[k]);

}












#endif  // !SBLParser