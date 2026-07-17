#ifndef SBLParser
#define SBLParser

#include <TSMaster.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <map>
#include <cstring> // for strcpy
namespace fs = std::filesystem;

std::map< std::string, int> appFileToIndex = {
    {"SBL_Path", 0},
    {"SBL_Path_VersionA", 0},
    {"Boot_Path", 0},
    {"APP0Path", 0},
    {"APP1Path", 1},
    {"APP2Path", 2},
    {"APP3Path", 3},
    {"APP4Path", 4},
    {"APP5Path", 5},
    {"APP6Path", 6},
    {"APP7Path", 7},
    {"APP8Path", 8},
    {"APP0Path_Decompress", 0},
    {"APP1Path_Decompress", 1},
    {"APP2Path_Decompress", 2},
    {"APP3Path_Decompress", 3},
    {"APP4Path_Decompress", 4},
    {"APP5Path_Decompress", 5},
    {"APP6Path_Decompress", 6},
    {"APP7Path_Decompress", 7},
    {"APP8Path_Decompress", 8},
    {"APP0Path_VersionA", 0},
    {"APP1Path_VersionA", 1},
    {"APP2Path_VersionA", 2},
    {"APP3Path_VersionA", 3},
    {"APP4Path_VersionA", 4},
    {"APP5Path_VersionA", 5},
    {"APP6Path_VersionA", 6},
    {"APP7Path_VersionA", 7},
    {"APP8Path_VersionA", 8},
    {"APP0Path_VersionB", 0},
    {"APP1Path_VersionB", 1},
    {"APP2Path_VersionB", 2},
    {"APP3Path_VersionB", 3},
    {"APP4Path_VersionB", 4},
    {"APP5Path_VersionB", 5},
    {"APP6Path_VersionB", 6},
    {"APP7Path_VersionB", 7},
    {"APP8Path_VersionB", 8},
    {"APP0Path_Production", 0},
    {"APP1Path_Production", 1},
    {"APP2Path_Production", 2},
    {"APP3Path_Production", 3},
    {"APP4Path_Production", 4},
    {"APP5Path_Production", 5},
    {"APP6Path_Production", 6},
    {"APP7Path_Production", 7},
    {"APP8Path_Production", 8}
};

#define LEN 512

u8 productionSign = 0;
u8 SBLSupported = 1;
char filename[500] = {};

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

int APPFileNum = 7;
char getFilePath[10][300] = {};
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

#define MAX_BLOCKS 30
struct vbfinfo
{
    u32              StartAddress;                     //数据文件起始地址
    u32              DataLength;                       //数据文件长度
    struct u8_array  allcheckcsum;                     //文件的校验和crc32
    u8               DataBlockNumber;                  //数据块数量
    struct DataBlcok VbfDataBlcok[MAX_BLOCKS];                 //数据块
    u32              EraseAddresss;                    //数据文件擦除地址
    u32              EraseLength;                      //数据文件擦除长度
    char             sw_part_number[20];                   //软件版本号
    char             sw_version[10];                       //软件版本
    char             sw_part_type[10];                     //软件类型
    char             data_format_identifier[10];           //数据格式标识符
    char             ecu_address[10];                      //ecu地址
    char             file_checksum[10];                    //文件校验和
    char             call[100];                             //文件刷写起始地址
    char             verification_block_start[100];         //验证块起始地址
    char             verification_block_length[100];        //验证块长度
    char             verification_block_root_hash[500];     //验证块根哈希
    struct u8_array  sw_signature_dev;                 //研发签名
    struct u8_array  sw_signature;                 //生产签名
}   VbfFileInfo;

int countFilesInDirectory(const std::string& dirPath) {
    int count = 0;
    try {
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                count++;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    APPFileNum = count;
    printf("fileCount is %d", count);
    return count;
}

int listFilesInDirectory(const std::string& dirPath) {
    int fileCount = countFilesInDirectory(dirPath);
    if (fileCount > 10) {
        printf("Too many files! Maximum supported: 10, found: %d\n", fileCount);
        return -1;
    }

    int index = 0;
    try {
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();

                if (filename.size() >= 300) {
                    std::cerr << "Filename too long: " << filename << std::endl;
                    continue;
                }

                printf("文件名: %s\n", filename.c_str());
                //strcat_s(getFilePath[index],sizeof(getFilePath[index]),configPath);
                snprintf(getFilePath[index], sizeof(getFilePath[index]), "%s\\%s", dirPath.c_str(), filename.c_str());
                getFilePath[fileCount][sizeof(getFilePath[0]) - 1] = '\0'; // 确保结尾
                //strcpy(getFilePath[index], filename.c_str());
                index++;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error listing directory: " << e.what() << std::endl;
        return -1;
    }
    return index;
}

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

    //strcat(*path, "Toolbox\\GEELY3\\Parameters.ini");
    strcat(*path, "01_Input\\SDDB\\write_sddb.ini");

    log("parameter path is %s", *path);
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
    printf("ReadHexFile");
    FILE* file = fopen(path, "rb"); // 打开hex文件

    if (file == NULL) {
        log("Error opening file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    //printf("%x%x%x", fileSize & 0xff, fileSize & 0xff00, fileSize & 0xff0000);
    fseek(file, 0, SEEK_SET);

    //printf("%x%x%x", ftell(file) & 0xff, ftell(file) & 0xff00, ftell(file) & 0xff0000);

    VbfFileInfo.DataBlockNumber = 0;

    while (fread(&VbfFileHeader, sizeof(VbfFileHeader), 1, file) == 1) {

        //printf("%x %x %x", ftell(file)&0xff, ftell(file) & 0xff00, ftell(file) & 0xff0000);

        //printf("FStartAddress=%x\n", VbfFileHeader.FStartAddress);
        VbfFileInfo.VbfDataBlcok[VbfFileInfo.DataBlockNumber].StartAddress = VbfFileHeader.FStartAddress;

        //printf("blocksize is %x\n", VbfFileHeader.FSize);
        VbfFileInfo.VbfDataBlcok[VbfFileInfo.DataBlockNumber].DataLength = VbfFileHeader.FSize;

        if (VbfFileHeader.FType != 0) {
            // 如果不是数据块类型，跳过本次循环
            fseek(file, VbfFileHeader.FBlockSize, SEEK_CUR);
            continue;
        }

        // 动态分配 hexbuffer 的内存空间
        u8* hexbuffer = (u8*)malloc(VbfFileHeader.FSize * 2);
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
            free(hexbuffer);
            fclose(file);
            return;
        }

        //printf("hexbuffer = %x  %x\n", hexbuffer[0], hexbuffer[1]);

        for (u32 j = 0; j < VbfFileHeader.FSize; j++) {
            VbfFileInfo.VbfDataBlcok[VbfFileInfo.DataBlockNumber].BlockData[j] = hexbuffer[j];
        }

        VbfFileInfo.DataBlockNumber++;

        free(hexbuffer);
    }

    fclose(file); // 关闭文件
    printf("End ReadHexFile");
    return;
}

void vbfparser(char* vbfIniKeyValue)
{
    char title[100] = "", key[100] = "";
    char buffer[300] = "";
    char parameterpath[300] = "";
    char* par = parameterpath;
    char vbfpath[500] = "";
    char hexpath[500] = "";
    char* hex = hexpath;
    char sysvar[500] = "";
    //char filename[1000] = "";
    char* configPath = NULL;

    //获取ini文件路径
    //GetIniFilePath(par);
    //app.get_configuration_file_path(&par);

    //strcat(par, "Toolbox\\GEELY3\\Parameters.ini");

    //printf("parameter path is %s", par);

    app.wait(1000, "");

    /*
    //获取SBL路径
    //sprintf(title, "DiagUserDefine");
    sprintf(title, "SWDLParameters");
    sprintf(key, vbfIniKeyValue);
    GetIniKeyString(title, key, par, buffer);
    sprintf(vbfpath, buffer);
    printf("vbfpath is %s", vbfpath);
    */
    app.get_configuration_file_path(&configPath);

    if (strstr(vbfIniKeyValue, "SBL_Path_VersionA") != nullptr) {
        snprintf(vbfpath, sizeof(vbfpath), "%s01_Input\\SBL_VersionA", configPath);
        listFilesInDirectory(vbfpath);
        snprintf(vbfpath, sizeof(vbfpath), "%s", getFilePath[appFileToIndex[vbfIniKeyValue]]);
        printf("当前解析APP_Decompress路径: %s\n", vbfpath);
    }
    else if (strstr(vbfIniKeyValue, "SBL_Path_VersionB") != nullptr) {
        snprintf(vbfpath, sizeof(vbfpath), "%s01_Input\\SBL_VersionB", configPath);
        listFilesInDirectory(vbfpath);
        snprintf(vbfpath, sizeof(vbfpath), "%s", getFilePath[appFileToIndex[vbfIniKeyValue]]);
        printf("当前解析APP_Decompress路径: %s\n", vbfpath);
    }
    else if (strstr(vbfIniKeyValue, "SBL") != nullptr) {
        snprintf(vbfpath, sizeof(vbfpath), "%s01_Input\\SBL", configPath);
        //vbfpath[sizeof(vbfpath)] = '\0'; // 确保结尾
        printf("SBL路径: %s\n", vbfpath);
        listFilesInDirectory(vbfpath);
        printf("sizeof=%d", sizeof(vbfpath));
        snprintf(vbfpath, sizeof(vbfpath), "%s", getFilePath[0]);
        printf("文件路径: %s\n", vbfpath);
    }
    else if (strstr(vbfIniKeyValue, "Decompress") != nullptr) {
        snprintf(vbfpath, sizeof(vbfpath), "%s01_Input\\APP_Decompress", configPath);
        listFilesInDirectory(vbfpath);
        snprintf(vbfpath, sizeof(vbfpath), "%s", getFilePath[appFileToIndex[vbfIniKeyValue]]);
        printf("当前解析APP_Decompress路径: %s\n", vbfpath);
    }
    else if (strstr(vbfIniKeyValue, "VersionA") != nullptr) {
        snprintf(vbfpath, sizeof(vbfpath), "%s01_Input\\APP_VersionA", configPath);
        listFilesInDirectory(vbfpath);
        snprintf(vbfpath, sizeof(vbfpath), "%s", getFilePath[appFileToIndex[vbfIniKeyValue]]);
        printf("当前解析VersionA路径: %s\n", vbfpath);
    }
    else if (strstr(vbfIniKeyValue, "VersionB") != nullptr) {
        snprintf(vbfpath, sizeof(vbfpath), "%s01_Input\\APP_VersionB", configPath);
        listFilesInDirectory(vbfpath);
        snprintf(vbfpath, sizeof(vbfpath), "%s", getFilePath[appFileToIndex[vbfIniKeyValue]]);
        printf("当前解析VersionA路径: %s\n", vbfpath);
    }
    else if (strstr(vbfIniKeyValue, "Production") != nullptr) {
        snprintf(vbfpath, sizeof(vbfpath), "%s01_Input\\APP_Production", configPath);
        listFilesInDirectory(vbfpath);
        snprintf(vbfpath, sizeof(vbfpath), "%s", getFilePath[appFileToIndex[vbfIniKeyValue]]);
        printf("当前解析APP_Production路径: %s\n", vbfpath);
    }
    else if (strstr(vbfIniKeyValue, "APP") != nullptr) {
        snprintf(vbfpath, sizeof(vbfpath), "%s01_Input\\APP", configPath);
        listFilesInDirectory(vbfpath);
        snprintf(vbfpath, sizeof(vbfpath), "%s", getFilePath[appFileToIndex[vbfIniKeyValue]]);
        printf("当前解析APP路径: %s\n", vbfpath);
    }
    else if (strstr(vbfIniKeyValue, "Boot") != nullptr) {
        snprintf(vbfpath, sizeof(vbfpath), "%s01_Input\\Boot", configPath);
        listFilesInDirectory(vbfpath);
        snprintf(vbfpath, sizeof(vbfpath), "%s", getFilePath[appFileToIndex[vbfIniKeyValue]]);
        printf("当前解析Boot自更新路径: %s\n", vbfpath);
    }
    //free(configPath);
    vbfpath[sizeof(vbfpath) - 1] = '\0';
    app.wait(1000, "");

    //解析vbf文件并获取hex文件路径
    GetHexFilePath(vbfpath, &hex);
    app.wait(1000, "");
    //printf("path is %s", vbfpath);

    //获取文件名
    app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
    if (strlen(filename) > 8) {
        filename[strlen(filename) - 9] = '\0';
    }


    //printf("filename = %s", filename);
    // 
    //获取vbf起始地址
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":StartAddress");
    app.get_system_var_uint32(sysvar, &VbfFileInfo.StartAddress);
    //log("File %s start address is %2x", filename, VbfFileInfo.StartAddress);

    //获取vbf数据文件长度
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":DataLength");
    app.get_system_var_uint32(sysvar, &VbfFileInfo.DataLength);
    //log("File %s start length is %2x", filename, VbfFileInfo.DataLength);


    //获取数据块数据
    //修改数据块个数，包括VbfFileInfo内的VbfDataBlcok个数为MAX_BLOCKS，原本为10
    //自研ZCU VBF中出现20个block
    for (int i = 0; i < MAX_BLOCKS; i++)
    {

        sprintf(sysvar, "%s%s%s%d", "Diagnostic0.", filename, ":StartAddress[Block]", i);
        app.get_system_var_uint32(sysvar, &VbfFileInfo.VbfDataBlcok[i].StartAddress);

        if (VbfFileInfo.VbfDataBlcok[i].StartAddress == 0)
        {
            //VbfFileInfo.DataBlockNumber = i + 1;
            //不确定上面为何对DataBlockNumber + 1，先注释
            VbfFileInfo.DataBlockNumber = i;
            printf("BlockNum is %d", VbfFileInfo.DataBlockNumber);
            break;
        }

        //log("File %s datablock %d start address is %2x",filename,  i, VbfFileInfo.VbfDataBlcok[i].StartAddress);

        sprintf(sysvar, "%s%s%s%d", "Diagnostic0.", filename, ":DataLength[Block]", i);
        app.get_system_var_uint32(sysvar, &VbfFileInfo.VbfDataBlcok[i].DataLength);
        //log("File %s datablock %d data length is %2x", filename, i, VbfFileInfo.VbfDataBlcok[i].DataLength);


        sprintf(sysvar, "%s%s%s%d", "Diagnostic0.", filename, ":CheckSum[Block]", i);
        app.get_system_var_uint8_array(sysvar, LEN, &VbfFileInfo.VbfDataBlcok[i].DataBlockCheckSum.count, VbfFileInfo.VbfDataBlcok[i].DataBlockCheckSum.array);
        //for (int k = 0; k < VbfFileInfo.VbfDataBlcok[i].DataBlockCheckSum.count; k++)
        //    log("File %s datablock %d checksum is %2x", filename, i, VbfFileInfo.VbfDataBlcok[i].DataBlockCheckSum.array[k]);
    }

    //获取块数据
    ReadHexFile(hex);

    //获取数据校验和
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":CheckSum");
    app.get_system_var_uint8_array(sysvar, LEN, &VbfFileInfo.allcheckcsum.count, VbfFileInfo.allcheckcsum.array);
    //for (int k = 0; k < VbfFileInfo.allcheckcsum.count; k++)
    //    log("File %s checksum is %2x", filename,VbfFileInfo.allcheckcsum.array[k]);

    //获取软件版本号
    char str[100];
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":sw_part_number");
    app.get_system_var_string(sysvar, LEN, str);
    sscanf(str, "\"%[^\"]\"", &VbfFileInfo.sw_part_number);
    //log("File %s partnumber is %s", filename,VbfFileInfo.sw_part_number);


    //获取软件版本
    str[0] = '\0';
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":sw_version");
    app.get_system_var_string(sysvar, LEN, str);
    sscanf(str, "\"%[^\"]\"", &VbfFileInfo.sw_version);
    //log("File %s version is %s", filename,VbfFileInfo.sw_version);


    ////获取软件类型
    //sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":sw_part_type");
    //app.get_system_var_string(sysvar, LEN, VbfFileInfo.sw_part_type);
    //log("File %s sw_part_type is %s", filename, VbfFileInfo.sw_part_type);
    //if (strcmp(VbfFileInfo.sw_part_type, "SBL") != 0){

    //    int found = 0;
    //    for (int i = 0; i < appindex; i++) {
    //        if (strcmp(_sw_[i].sw_part_number, VbfFileInfo.sw_part_number) == 0 &&
    //            strcmp(_sw_[i].sw_version, VbfFileInfo.sw_version) == 0) {
    //            found = 1;
    //            break;
    //        }
    //    }
    //    // 如果未找到匹配项，添加新条目
    //    if (!found) {
    //        strcpy(_sw_[appindex].sw_part_number, VbfFileInfo.sw_part_number);
    //        strcpy(_sw_[appindex].sw_version, VbfFileInfo.sw_version);
    //        appindex++;
    //    }

    //    //获取数据文件擦除地址和长度
    //    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":erase");
    //    str[0] = '\0';
    //    app.get_system_var_string(sysvar, LEN, str);

    //    // 使用 sscanf 提取两个十六进制数
    //    if (sscanf(str, "{{0x%x,0x%x", &VbfFileInfo.EraseAddresss, &VbfFileInfo.EraseLength) == 2) {
    //        log("EraseAddresss is 0x%08X,EraseLength is 0x%08X", VbfFileInfo.EraseAddresss, VbfFileInfo.EraseLength);
    //    }
    //    else {
    //        log("Failed to extract hex values.");
    //    }
    //}
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":sw_part_type");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.sw_part_type);
    if (VbfFileInfo.sw_part_type[0] == '\0') {
        log("Error: sw_part_type is empty for file %s", filename);
        return;
    }
    //log("File %s sw_part_type is %s", filename, VbfFileInfo.sw_part_type);
    if (strcmp(VbfFileInfo.sw_part_type, "SBL") != 0) {
        char str[256];

        // 检查 _sw_ 数组容量
        if (appindex >= APPFileNum) {
            appindex = 0;
            /*log("Error: _sw_ array full, cannot add part=%s, version=%s", VbfFileInfo.sw_part_number, VbfFileInfo.sw_version);
            return;*/
        }

        // 检查是否已存在相同的 (sw_part_number, sw_version)
        int found = 0;
        for (int i = 0; i < appindex; i++) {
            if (strcmp(_sw_[i].sw_part_number, VbfFileInfo.sw_part_number) == 0 &&
                strcmp(_sw_[i].sw_version, VbfFileInfo.sw_version) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (VbfFileInfo.sw_part_number[0] == '\0' || VbfFileInfo.sw_version[0] == '\0') {
                log("Error: Invalid sw_part_number or sw_version for file %s", filename);
                return;
            }
            strcpy(_sw_[appindex].sw_part_number, VbfFileInfo.sw_part_number);
            strcpy(_sw_[appindex].sw_version, VbfFileInfo.sw_version);
            appindex++;
            //log("Added APP file %s to _sw_[%d]: part=%s, version=%s", filename, appindex - 1, VbfFileInfo.sw_part_number, VbfFileInfo.sw_version);
        }

        // 获取擦除地址和长度
        //sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":erase");
        //str[0] = '\0';
        //app.get_system_var_string(sysvar, LEN, str);
        //if (str[0] == '\0') {
        //    log("Error: erase attribute is empty for file %s", filename);
        //    return ;
        //}

        //if (sscanf(str, "{{0x%x,0x%x", &VbfFileInfo.EraseAddresss, &VbfFileInfo.EraseLength) == 2) {
        //    log("File %s: EraseAddress=0x%08X, EraseLength=0x%08X", filename, VbfFileInfo.EraseAddresss, VbfFileInfo.EraseLength);
        //}
        //else {
        //    log("Error: Failed to parse erase attribute '%s' for file %s", str, filename);
        //    return;
        //}
        sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":erase");
        str[0] = '\0';
        app.get_system_var_string(sysvar, LEN, str);
        //log("sysvar: %s, str: %s", sysvar, str);

        if (str[0] == '\0') {
            log("Failed to get erase data: str is empty.");
        }
        else {
            // 使用灵活的格式匹配，允许任意空格和括号
            int result = sscanf(str, "%*[{ ]0x%x%*[, ]0x%x%*[ }]", &VbfFileInfo.EraseAddresss, &VbfFileInfo.EraseLength);
            if (result == 2) {
                log("EraseAddresss is 0x%08X, EraseLength is 0x%08X", VbfFileInfo.EraseAddresss, VbfFileInfo.EraseLength);
            }
            else {
                log("sscanf failed, parsed %d values, str: %s", result, str);
            }
        }
    }

    //获取数据格式标识符
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":data_format_identifier");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.data_format_identifier);
    log("File %s data_format_identifier is %s", filename, VbfFileInfo.data_format_identifier);

    //获取ecu地址
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":ecu_address");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.ecu_address);
    //log("File %s ecu_address is %s", filename, VbfFileInfo.ecu_address);

    //获取文件校验和
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":file_checksum");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.file_checksum);
    //log("File %s file_checksum is %s", filename, VbfFileInfo.file_checksum);

    //获取call
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":call");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.call);
    //log("File %s call is %s", filename, VbfFileInfo.call);

    //获取验证块起始地址
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":verification_block_start");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.verification_block_start);
    //log("File %s verification_block_start is %s", filename, VbfFileInfo.verification_block_start);

    //获取验证块数据长度
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":verification_block_length");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.verification_block_length);
    //log("File %s verification_block_length is %s", filename, VbfFileInfo.verification_block_length);

    //获取验证块根哈希
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":verification_block_root_hash");
    app.get_system_var_string(sysvar, LEN, VbfFileInfo.verification_block_root_hash);
    //log("File %s verification_block_root_hash is %s", filename, VbfFileInfo.verification_block_root_hash);

    //获取软件研发签名
    sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":sw_signature_dev");
    app.get_system_var_uint8_array(sysvar, LEN, &VbfFileInfo.sw_signature_dev.count, VbfFileInfo.sw_signature_dev.array);

    //获取软件生产签名
    if (1 == productionSign) {
        sprintf(sysvar, "%s%s%s", "Diagnostic0.", filename, ":sw_signature");
        app.get_system_var_uint8_array(sysvar, LEN, &VbfFileInfo.sw_signature.count, VbfFileInfo.sw_signature.array);
    }

}












#endif  // !SBLParser