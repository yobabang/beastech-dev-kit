#ifndef PDXParser
#define PDXParser

#include <TSMaster.h>
#include "cJSON.h"

/*PDX参数清单*/

u8 MultiDIDNum = 5;//支持最大同时读取的DID数量
u8 enterprogType = 0;//进入编程会话的方式

u32 ECU_Address = 0x163b;
u8 AvaliableMask = 0xFF;
u32 ECUResetTime = 1500;

struct AvaliableSubfunctions
{
    u8 Subfunc;
    u8 Sup_Def;
    u8 Sup_Ext;
    u8 Sup_PBL;
    u8 Sup_SBL;
};

//支持的服务和子功能
struct AvailableService
{
	u8 service;
    AvaliableSubfunctions subfunc[30];
    u8 subfuncsize;
};



//可读DID
struct AvailableReadDID
{
	u32 DID;
	u16 DIDLength;
	u8 Sup_Def;
	u8 Sup_Ext;
	u8 Sup_PBL;
	u8 Sup_SBL;
    u8 Writable;
	u16 Seclvl_Ext;
	u16 Seclvl_PBL;
};

//可写DID
struct AvailableWriteDID
{
	u32 DID;
	u16 DIDLength;
	u8 Sup_Def;
	u8 Sup_Ext;
	u8 Sup_PBL;
	u8 Sup_SBL;
	u16 Seclvl_Ext;
	u16 Seclvl_PBL;
};

//支持I/O控制的DID
struct Available2FDID
{
	u32 DID;
	u16 DIDLength;
	u8 Sup_Ext;
	u16 Seclvl_Ext;
};

//支持的例程
struct AvailableRID
{
	u32 RID;
    u8 RoutineType;
	u8 Sup_Ext;
    u8 Sup_PBL;
	u8 Seclvl_Ext;
    u8 Seclvl_PBL;
    u16 RecordLength;
};

//支持的DTC
struct AvailableDTC
{
	u64 DTC;
	u8  Status;
};


AvailableService SupportServices[40] = {
    { 0x10, {{0x01,1,1,1,1},{0x02,1,1,1,1},{0x03,1,1,0,0}} ,1 },
    { 0x11, {{0x01,1,1,1,1},{0x03,1,1,1,1}},1 },
    { 0x14,{},1},
    { 0x19,{{0x02,1,1,0,0},{0x03,1,1,0,0},{0x04,1,1,0,0},{0x06,1,1,0,0},{0x0A}},1},
    { 0x22,{},1},
    { 0x27,{{0x01,0,0,1,1},{0x05,0,1,0,0}},2}
};

u8 servicesize = 40;

AvailableReadDID ReadDIDList[50] = {
	{ 0xF186, 1, 1, 1, 1,1, 0x01, 0x03 },
	{ 0xF186, 1, 1, 1, 1,1, 0x01, 0x03 },
};

u8 readdidsize = 50;

AvailableWriteDID WriteDIDList[50] = {
	{ 0xF186, 1, 1, 1, 1, 0x01, 0x03 },
	{ 0xF186, 1, 1, 1, 1, 0x01, 0x03 },
};

u8 writedidsize = 50;

Available2FDID IODIDList[20] = {
	{ 0xF186, 1, 0x03 },
	{ 0xF186, 1, 0x03 },
};

u8 iodidsize = 20;

AvailableRID RIDList[50] = {
	{ 0xF186,1,1,1,0,0,0},
	{ 0x0212,1,1,1,0,0x01,256},
};

u8 ridsize = 50;

struct AvailableDTC DTCList[100] = {
	{0xD01C2F,0x01},
};

u8 dtcsize = 100;


//从ini文件获取信息
int GetINIKeyString(char* title, char* key, char* filename, char* buf)
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

int PutINIKeyString(char* title, char* key, char* val, char* filename)
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

    remove(filename);

    return rename(sLine, filename);// 将临时文件更新到原文件  
}

void GetSDDBJSONFilePath(char** path)
{
    app.get_configuration_file_path(path);

    strcat(*path, "Toolbox\\GEELY3\\sddb.json");

    //log("json path is %s", *path);
}

//通过面板的用户配置来修改json文件
int ini_json_parser() {
    char ini_path[200] = "";
    char test_json[200] = "";

    char configurationpath[200];
    char* cfgpath = configurationpath;
    char title[20], key[20],buf[200];

    //读取ini文件
    app.get_configuration_file_path(&cfgpath);
    sprintf(ini_path,"%s%s",cfgpath, "Toolbox\\PythonTestSystem\\Parameters.ini");

    // 打开test_JSON文件进行读取
    sprintf(test_json, "%s%s", cfgpath, "Toolbox\\PythonTestSystem\\test.json");

    FILE* originalFile = fopen(test_json, "r+");
    if (originalFile == NULL) {
        log("Error opening the original file.");
        return -1;
    }


    // 读取JSON数据
    fseek(originalFile, 0, SEEK_END);
    long fileSize = ftell(originalFile);
    fseek(originalFile, 0, SEEK_SET);

    // 检查文件大小
    if (fileSize <= 0) {
        printf("Error: File is empty or cannot determine file size.\n");
        fclose(originalFile); // 关闭文件
        return -1; // 退出函数或程序
    }

    // 动态分配内存并检查分配是否成功
    char* test_jsonData = (char*)malloc(fileSize + 1);
    if (!test_jsonData) {
        printf("Error: Memory allocation failed.\n");
        fclose(originalFile); // 关闭文件
        return -1; // 退出函数或程序
    }

    // 读取文件内容并检查读取是否成功
    size_t bytesRead = fread(test_jsonData, 1, fileSize, originalFile);
    if (bytesRead > fileSize) {
        printf("Error: Failed to read file contents.\n");
        fclose(originalFile); // 关闭文件
        free(test_jsonData); // 释放分配的内存
        return -1; // 退出函数或程序
    }


    // 解析JSON数据
    cJSON* root = cJSON_Parse(test_jsonData);
    if (root == NULL) {
        log("Error parsing JSON data.");
        return -1;
    }

    cJSON* Items = cJSON_GetObjectItem(root, "Items");	//解析数组
    int Items_size = cJSON_GetArraySize(Items);	//获取数组成员个数 

    //读取是否测试SID2F
    sprintf(title, "%s", "DiagUserDefine"); sprintf(key, "%s", "Test_SID2F");
    GetINIKeyString(title, key, ini_path, buf);
    if (atoi(buf) == 0)
    {  
        for (int i = 0; i < Items_size; i++)
        {
            cJSON* Item = cJSON_GetArrayItem(Items,i);//解析数组中的对象
            if (Item == NULL || Item->type != cJSON_Object) continue; // 确保 Item 不为空并且是一个对象

            cJSON* Sub_Items = cJSON_GetObjectItem(Item, "Items");//解析数组中的对象

            if (Sub_Items == NULL || Sub_Items->type != cJSON_Array) continue;
           
            int Sub_Items_size = cJSON_GetArraySize(Sub_Items);	//获取数组成员个数 

            // 从后往前遍历子数组，以防止删除元素导致的索引错位
            for (int j = Sub_Items_size - 1; j >= 0; j--)
            {
                cJSON* Sub_Item = cJSON_GetArrayItem(Sub_Items, j); // 解析子数组中的对象
                if (Sub_Item == NULL || Sub_Item->type != cJSON_Object) continue; // 确保 Sub_Item 不为空并且是一个对象

                cJSON* name = cJSON_GetObjectItem(Sub_Item, "Name"); // 获取对象中的属性 "Name"
                if (name != NULL && name->type == cJSON_String && strstr(name->valuestring, "Service0x2F") != NULL)
                {
                    cJSON_DeleteItemFromArray(Sub_Items, j); // 删除满足条件的元素
                }
            }
          
        }
    }

    //读取是否测试SID31
    sprintf(title, "%s", "DiagUserDefine"); sprintf(key, "%s", "Test_SID31");
    GetINIKeyString(title, key, ini_path, buf);
    if (atoi(buf) == 0)
    {
        for (int i = 0; i < Items_size; i++)
        {
            cJSON* Item = cJSON_GetArrayItem(Items, i);//解析数组中的对象
            if (Item == NULL || Item->type != cJSON_Object) continue; // 确保 Item 不为空并且是一个对象

            cJSON* Sub_Items = cJSON_GetObjectItem(Item, "Items");//解析数组中的对象

            if (Sub_Items == NULL || Sub_Items->type != cJSON_Array) continue;
            
            int Sub_Items_size = cJSON_GetArraySize(Sub_Items);	//获取数组成员个数 

            // 从后往前遍历子数组，以防止删除元素导致的索引错位
            for (int j = Sub_Items_size - 1; j >= 0; j--)
            {
                cJSON* Sub_Item = cJSON_GetArrayItem(Sub_Items, j); // 解析子数组中的对象
                if (Sub_Item == NULL || Sub_Item->type != cJSON_Object) continue; // 确保 Sub_Item 不为空并且是一个对象

                cJSON* name = cJSON_GetObjectItem(Sub_Item, "Name"); // 获取对象中的属性 "Name"
                if (name != NULL && name->type == cJSON_String && strstr(name->valuestring, "Service0x31") != NULL)
                {
                    cJSON_DeleteItemFromArray(Sub_Items, j); // 删除满足条件的元素
                }
            }
            
        }
    }

    //读取是否测试SPRMIB
    sprintf(title, "%s", "DiagUserDefine"); sprintf(key, "%s", "Test_SPRMIB");
    GetINIKeyString(title, key, ini_path, buf);
    if (atoi(buf) == 0)
    {
        for (int i = 0; i < Items_size; i++)
        {
            cJSON* Item = cJSON_GetArrayItem(Items, i);//解析数组中的对象
            if (Item == NULL || Item->type != cJSON_Object) continue; // 确保 Item 不为空并且是一个对象

            cJSON* Sub_Items = cJSON_GetObjectItem(Item, "Items");//解析数组中的对象

            if (Sub_Items == NULL || Sub_Items->type != cJSON_Array) continue;
            int Sub_Items_size = cJSON_GetArraySize(Sub_Items);	//获取数组成员个数 

            for (int j = 0; j < Sub_Items_size; j++)
            {
                cJSON* Sub_Item = cJSON_GetArrayItem(Sub_Items, j);//解析数组中的对象
                if (Sub_Item == NULL || Sub_Item->type != cJSON_Object) continue;

                cJSON* Sub_Sub_Items = cJSON_GetObjectItem(Sub_Item, "Items");
                if (Sub_Sub_Items == NULL || Sub_Sub_Items->type != cJSON_Array) continue;

                int Sub_Sub_Items_size = cJSON_GetArraySize(Sub_Sub_Items);

                for (int k = Sub_Sub_Items_size - 1; k >= 0; k--)
                {
                    cJSON* Sub_Sub_Item = cJSON_GetArrayItem(Sub_Sub_Items, k); // 解析子数组中的对象
                    if (Sub_Sub_Item == NULL || Sub_Sub_Item->type != cJSON_Object) continue; // 确保 Sub_Item 不为空并且是一个对象

                    cJSON* name = cJSON_GetObjectItem(Sub_Sub_Item, "Name"); // 获取对象中的属性 "Name"

                    if (name != NULL && name->type == cJSON_String && strstr(name->valuestring, "SPRMIB") != NULL)
                    {
                        printf("%s %s", name->valuestring, strstr(name->valuestring, "SPRMIB"));
                        cJSON_DeleteItemFromArray(Sub_Sub_Items, k); // 删除满足条件的元素
                    }
                }

            }
        }
    }

    

    //读取是否测试功能寻址
    sprintf(title, "%s", "DiagUserDefine"); sprintf(key, "%s", "Test_FuncAddr");
    GetINIKeyString(title, key, ini_path, buf);
    if (atoi(buf) == 0)
    {
        for (int i = 0; i < Items_size; i++)
        {
            cJSON* Item = cJSON_GetArrayItem(Items, i);//解析数组中的对象
            if (Item == NULL || Item->type != cJSON_Object) continue; // 确保 Item 不为空并且是一个对象

            cJSON* Sub_Items = cJSON_GetObjectItem(Item, "Items");//解析数组中的对象

            if (Sub_Items == NULL || Sub_Items->type != cJSON_Array) continue;
            int Sub_Items_size = cJSON_GetArraySize(Sub_Items);	//获取数组成员个数 

            for (int j = 0; j < Sub_Items_size; j++)
            {
                cJSON* Sub_Item = cJSON_GetArrayItem(Sub_Items, j);//解析数组中的对象
                if (Sub_Item == NULL || Sub_Item->type != cJSON_Object) continue;

                cJSON* Sub_Sub_Items = cJSON_GetObjectItem(Sub_Item, "Items");
                if (Sub_Sub_Items == NULL || Sub_Sub_Items->type != cJSON_Array) continue;

                int Sub_Sub_Items_size = cJSON_GetArraySize(Sub_Sub_Items);

                for (int k = Sub_Sub_Items_size - 1; k >= 0; k--)
                {
                    cJSON* Sub_Sub_Item = cJSON_GetArrayItem(Sub_Sub_Items, k); // 解析子数组中的对象
                    if (Sub_Sub_Item == NULL || Sub_Sub_Item->type != cJSON_Object) continue; // 确保 Sub_Item 不为空并且是一个对象

                    cJSON* name = cJSON_GetObjectItem(Sub_Sub_Item, "Name"); // 获取对象中的属性 "Name"
                    if (name != NULL && name->type == cJSON_String && strstr(name->valuestring, "Func") != NULL)
                    {
                        cJSON_DeleteItemFromArray(Sub_Sub_Items, k); // 删除满足条件的元素
                    }
                }

            }
        }
    }

    originalFile = fopen(test_json, "w+");
    if (originalFile == NULL) {
        log("Error opening the original file.");
        return -1;
    }

    // 将修改后的JSON数据写入文件
    fseek(originalFile, 0, SEEK_SET);
    fputs(cJSON_Print(root), originalFile);

    // 释放资源

    fclose(originalFile);
    cJSON_Delete(root);
    free(test_jsonData);

    return 0;
}

//通过pdx_json来修改test_json
int json_json_parser() {
    char pdx_json[200] = "";
    char test_json[200] = "";
    unsigned int exist_flag = 0;

    // 读取pdx_JSON文件名
    char configurationpath[200];
    char* cfgpath = configurationpath;

    //app.get_configuration_file_path(&cfgpath);
    sprintf(pdx_json, "%s", "C:\\Users\\tosun2\\Desktop\\A_DSA.json");

    // 打开pdx_JSON文件进行读取
    FILE* pdx = fopen(pdx_json, "r");
    if (pdx == NULL) {
        log("Error opening the original file.");
        return -1;
    }

    // 读取JSON数据
    fseek(pdx, 0, SEEK_END);
    long pdx_fileSize = ftell(pdx);
    fseek(pdx, 0, SEEK_SET);

    // 检查文件大小
    if (pdx_fileSize <= 0) {
        log("Error: File is empty or cannot determine file size.");
        fclose(pdx); // 关闭文件
        return  -1; // 退出函数或程序
    }

    // 动态分配内存并检查分配是否成功
    char* pdx_jsonData = (char*)malloc(pdx_fileSize + 1);
    if (!pdx_jsonData) {
        log("Error: Memory allocation failed.");
        fclose(pdx); // 关闭文件
        return -1; // 退出函数或程序
    }

    // 读取文件内容并检查读取是否成功
    size_t pdx_bytesRead = fread(pdx_jsonData, 1, pdx_fileSize, pdx);
    if (pdx_bytesRead > pdx_fileSize) {
        log("Error: Failed to read file contents.");
        fclose(pdx); // 关闭文件
        free(pdx_jsonData); // 释放分配的内存
        return -1; // 退出函数或程序
    }

    // 解析pdx_JSON数据
    cJSON* pdx_root = cJSON_Parse(pdx_jsonData);
    if (pdx_root == NULL) {
        log("Error parsing JSON data.");
        return -1;
    }

    fclose(pdx);


    app.get_configuration_file_path(&cfgpath);
    sprintf(test_json, "%s%s",cfgpath, "Toolbox\\PythonTestSystem\\test.json");

    // 打开test_JSON文件进行读取
    FILE* test = fopen(test_json, "r+");
    if (test == NULL) {
        log("Error opening the original file.");
        return -1;
    }

    // 读取test_JSON数据
    fseek(test, 0, SEEK_END);
    long test_fileSize = ftell(test);
    fseek(test, 0, SEEK_SET);

    // 检查文件大小
    if (test_fileSize <= 0) {
        log("Error: File is empty or cannot determine file size.");
        fclose(test); // 关闭文件
        return -1; // 退出函数或程序
    }

    // 动态分配内存并检查分配是否成功
    char* test_jsonData = (char*)malloc(test_fileSize + 1);
    if (!test_jsonData) {
        log("Error: Memory allocation failed.");
        fclose(test); // 关闭文件
        return -1; // 退出函数或程序
    }

    // 读取文件内容并检查读取是否成功
    size_t test_bytesRead = fread(test_jsonData, 1, test_fileSize, test);
    if (test_bytesRead > test_fileSize) {
        log("Error: Failed to read file contents.");
        fclose(test); // 关闭文件
        free(test_jsonData); // 释放分配的内存
        return -1; // 退出函数或程序
    }

    // 解析test_JSON数据
    cJSON* test_root = cJSON_Parse(test_jsonData);
    if (test_root == NULL) {
        log("Error parsing JSON data.");
        return -1;
    }

    // 根据pdx_json修改test_json数据
    cJSON* Project = cJSON_GetObjectItem(pdx_root, "Project");
    cJSON* System = cJSON_GetObjectItem(Project, "System");
    cJSON* ECUs = cJSON_GetObjectItem(System, "ECUs");
    cJSON* ECU = cJSON_GetObjectItem(ECUs, "ECU");
    cJSON* SWs = cJSON_GetObjectItem(ECU, "SWs");
    cJSON* SW = cJSON_GetObjectItem(SWs, "SW");
    int SW_size = cJSON_GetArraySize(SW);
    for (int ii = 0; ii < SW_size; ii++)
    {
        cJSON* SW_Type = cJSON_GetArrayItem(SW, ii);//解析数组中的对象
        if (SW_Type == NULL || SW_Type->type != cJSON_Object) continue; // 确保 Item 不为空并且是一个对象

        if (strcmp(cJSON_GetObjectItem(SW_Type, "@Type")->valuestring, "APP") != 0)continue;

        cJSON* Services = cJSON_GetObjectItem(SW_Type, "Services");//解析数组中的对象
        cJSON* Service = cJSON_GetObjectItem(Services, "Service");
        if (Service == NULL || Service->type != cJSON_Array) continue;

        int Service_size = cJSON_GetArraySize(Service);

        for (int jj = 0; jj < Service_size; jj++)
        {
            
            cJSON* Sub_Services = cJSON_GetArrayItem(Service, jj);
            if (Sub_Services == NULL || Sub_Services->type != cJSON_Object)continue;

            cJSON* SID = cJSON_GetObjectItem(Sub_Services, "@ID");
            log("SID = %s",SID->valuestring);

            cJSON* NegativeResponseCodes = cJSON_GetObjectItem(Sub_Services,"NegativeResponseCodes");
            if (NegativeResponseCodes == NULL || NegativeResponseCodes->type != cJSON_Object) continue; // 确保 NegativeResponseCodes 不为空并且是一个对象


            cJSON* NegativeResponseCode = cJSON_GetObjectItem(NegativeResponseCodes,"NegativeResponseCode");
            if (NegativeResponseCode == NULL || NegativeResponseCode->type != cJSON_Array)continue;


            int NRC_size = cJSON_GetArraySize(NegativeResponseCode);
            log("NRC_SIZE %d", NRC_size);


            //判断是否存在NRC12
            exist_flag = 0;

            for (int m = 0; m < NRC_size; m++)
            {
                cJSON* NRC = cJSON_GetArrayItem(NegativeResponseCode, m);
                if (NRC == NULL || NRC->type != cJSON_Object)continue;

                cJSON* NRC_Code = cJSON_GetObjectItem(NRC, "@Code");
                log("NRC Code is %s", NRC_Code->valuestring);

                if (strstr(NRC_Code->valuestring, "12") == 0)continue;
                log("NRC12 Exist");
                exist_flag = 1;
            }

            if (exist_flag != 1)//删除NRC12相关的测试用例
            {
                cJSON* Items = cJSON_GetObjectItem(test_root, "Items");	//解析数组
                int Items_size = cJSON_GetArraySize(Items);	//获取数组成员个数 

                for (int i = 0; i < Items_size; i++)
                {
                    cJSON* Item = cJSON_GetArrayItem(Items, i);//解析数组中的对象
                    if (Item == NULL || Item->type != cJSON_Object) continue; // 确保 Item 不为空并且是一个对象

                    cJSON* Sub_Items = cJSON_GetObjectItem(Item, "Items");//解析数组中的对象

                    if (Sub_Items == NULL || Sub_Items->type != cJSON_Array) continue;
                    int Sub_Items_size = cJSON_GetArraySize(Sub_Items);	//获取数组成员个数 

                    for (int j = 0; j < Sub_Items_size; j++)
                    {
                        cJSON* Sub_Item = cJSON_GetArrayItem(Sub_Items, j);//解析数组中的对象
                        if (Sub_Item == NULL || Sub_Item->type != cJSON_Object) continue;

                        if (strstr(cJSON_GetObjectItem(Sub_Item, "Name")->valuestring, SID->valuestring) != 0)
                        {
                            cJSON* Sub_Sub_Items = cJSON_GetObjectItem(Sub_Item, "Items");
                            if (Sub_Sub_Items == NULL || Sub_Sub_Items->type != cJSON_Array) continue;

                            int Sub_Sub_Items_size = cJSON_GetArraySize(Sub_Sub_Items);

                            for (int k = Sub_Sub_Items_size - 1; k >= 0; k--)
                            {
                                cJSON* Sub_Sub_Item = cJSON_GetArrayItem(Sub_Sub_Items, k); // 解析子数组中的对象
                                if (Sub_Sub_Item == NULL || Sub_Sub_Item->type != cJSON_Object) continue; // 确保 Sub_Item 不为空并且是一个对象

                                cJSON* name = cJSON_GetObjectItem(Sub_Sub_Item, "Name"); // 获取对象中的属性 "Name"

                                if (name != NULL && name->type == cJSON_String && strstr(name->valuestring, "NRC12") != NULL)
                                {
                                    printf("%s %s", name->valuestring, strstr(name->valuestring, "NRC12"));
                                    cJSON_DeleteItemFromArray(Sub_Sub_Items, k); // 删除满足条件的元素
                                }
                            }
                        }

                    }

                }
            }


            //判断是否存在NRC7E
            exist_flag = 0;

            for (int m = 0; m < NRC_size; m++)
            {
                cJSON* NRC = cJSON_GetArrayItem(NegativeResponseCode, m);
                if (NRC == NULL || NRC->type != cJSON_Object)continue;

                cJSON* NRC_Code = cJSON_GetObjectItem(NRC, "@Code");
                log("NRC Code is %s", NRC_Code->valuestring);

                if (strstr(NRC_Code->valuestring, "7E") == 0)continue;
                log("NRC7E Exist");
                exist_flag = 1;
            }

            if (exist_flag != 1)//删除NRC12相关的测试用例
            {
                cJSON* Items = cJSON_GetObjectItem(test_root, "Items");	//解析数组
                int Items_size = cJSON_GetArraySize(Items);	//获取数组成员个数 

                for (int i = 0; i < Items_size; i++)
                {
                    cJSON* Item = cJSON_GetArrayItem(Items, i);//解析数组中的对象
                    if (Item == NULL || Item->type != cJSON_Object) continue; // 确保 Item 不为空并且是一个对象

                    cJSON* Sub_Items = cJSON_GetObjectItem(Item, "Items");//解析数组中的对象

                    if (Sub_Items == NULL || Sub_Items->type != cJSON_Array) continue;
                    int Sub_Items_size = cJSON_GetArraySize(Sub_Items);	//获取数组成员个数 

                    for (int j = 0; j < Sub_Items_size; j++)
                    {
                        cJSON* Sub_Item = cJSON_GetArrayItem(Sub_Items, j);//解析数组中的对象
                        if (Sub_Item == NULL || Sub_Item->type != cJSON_Object) continue;

                        if (strstr(cJSON_GetObjectItem(Sub_Item, "Name")->valuestring, SID->valuestring) != 0)
                        {
                            cJSON* Sub_Sub_Items = cJSON_GetObjectItem(Sub_Item, "Items");
                            if (Sub_Sub_Items == NULL || Sub_Sub_Items->type != cJSON_Array) continue;

                            int Sub_Sub_Items_size = cJSON_GetArraySize(Sub_Sub_Items);

                            for (int k = Sub_Sub_Items_size - 1; k >= 0; k--)
                            {
                                cJSON* Sub_Sub_Item = cJSON_GetArrayItem(Sub_Sub_Items, k); // 解析子数组中的对象
                                if (Sub_Sub_Item == NULL || Sub_Sub_Item->type != cJSON_Object) continue; // 确保 Sub_Item 不为空并且是一个对象

                                cJSON* name = cJSON_GetObjectItem(Sub_Sub_Item, "Name"); // 获取对象中的属性 "Name"

                                if (name != NULL && name->type == cJSON_String && strstr(name->valuestring, "NRC7E") != NULL)
                                {
                                    printf("%s %s", name->valuestring, strstr(name->valuestring, "NRC7E"));
                                    cJSON_DeleteItemFromArray(Sub_Sub_Items, k); // 删除满足条件的元素
                                }
                            }
                        }

                    }

                }
            }  
        }

    }

    test = fopen(test_json, "w+");
    if (test == NULL) {
        log("Error opening the original file.");
        return -1;
    }

    // 将修改后的JSON数据写入文件
    fseek(test, 0, SEEK_SET);
    fputs(cJSON_Print(test_root), test);

    // 释放缓存
    fclose(test);
    cJSON_Delete(pdx_root);
    cJSON_Delete(test_root);
    free(pdx_jsonData);
    free(test_jsonData);


    log("JSON file copied and modified successfully.");

    return 0;
}


//读取sddb_json
int sddb_json_reader(char* jsonpath) {
    char pdx_json[200] = "";
    unsigned int exist_flag = 0;

    // 读取pdx_JSON文件名
    char configurationpath[200];
    char* cfgpath = configurationpath;

    //app.get_configuration_file_path(&cfgpath);
    sprintf(pdx_json, "%s", jsonpath);

    // 打开pdx_JSON文件进行读取
    FILE* pdx = fopen(pdx_json, "r");
    if (pdx == NULL) {
        log("Error opening the original file.");
        return -1;
    }

    // 读取JSON数据
    fseek(pdx, 0, SEEK_END);
    long pdx_fileSize = ftell(pdx);
    fseek(pdx, 0, SEEK_SET);

    // 检查文件大小
    if (pdx_fileSize <= 0) {
        log("Error: File is empty or cannot determine file size.");
        fclose(pdx); // 关闭文件
        return  -1; // 退出函数或程序
    }

    // 动态分配内存并检查分配是否成功
    char* pdx_jsonData = (char*)malloc(pdx_fileSize + 1);
    if (!pdx_jsonData) {
        log("Error: Memory allocation failed.");
        fclose(pdx); // 关闭文件
        return -1; // 退出函数或程序
    }

    // 读取文件内容并检查读取是否成功
    size_t pdx_bytesRead = fread(pdx_jsonData, 1, pdx_fileSize, pdx);
    if (pdx_bytesRead > pdx_fileSize) {
        log("Error: Failed to read file contents.");
        fclose(pdx); // 关闭文件
        free(pdx_jsonData); // 释放分配的内存
        return -1; // 退出函数或程序
    }

    // 解析pdx_JSON数据
    cJSON* pdx_root = cJSON_Parse(pdx_jsonData);
    if (pdx_root == NULL) {
        log("Error parsing JSON data.");
        return -1;
    }

    fclose(pdx);

    cJSON* Project = cJSON_GetObjectItem(pdx_root, "Project");
    cJSON* System = cJSON_GetObjectItem(Project, "System");
    cJSON* ECUs = cJSON_GetObjectItem(System, "ECUs");
    cJSON* ECU = cJSON_GetObjectItem(ECUs, "ECU");

    char* st;
    cJSON* Address = cJSON_GetObjectItem(ECU, "@address");
    ECU_Address = strtol(Address->valuestring, &st, 16);

    cJSON* SWs = cJSON_GetObjectItem(ECU, "SWs");
    cJSON* SW = cJSON_GetObjectItem(SWs, "SW");
    int SW_size = cJSON_GetArraySize(SW);
    for (int i = 0; i < SW_size; i++)
    {
        cJSON* SW_Type = cJSON_GetArrayItem(SW, i);//解析数组中的对象
        if (SW_Type == NULL || SW_Type->type != cJSON_Object) continue; // 确保 Item 不为空并且是一个对象

        if (strcmp(cJSON_GetObjectItem(SW_Type, "@Type")->valuestring, "APP") != 0)continue;

        char* stt;
        cJSON* StartupTime = cJSON_GetObjectItem(SW_Type, "StartupTime");
        ECUResetTime = strtol(StartupTime->valuestring, &stt, 10);
        cJSON* amsk = cJSON_GetObjectItem(SW_Type, "DTCStatusAvailabilityMask");
        AvaliableMask = strtol(amsk->valuestring, &st, 16);


        ////TP参数
        cJSON* TransportLayers = cJSON_GetObjectItem(SW_Type, "TransportLayers");
        cJSON* TransportLayer = cJSON_GetObjectItem(TransportLayers, "TransportLayer");

        cJSON* Type = cJSON_GetObjectItem(TransportLayer, "@Type");
        if (strcmp(Type->valuestring, "CAN") == 0)
        {
            BusTypeNow = 1;
        }
        else
        {
            BusTypeNow = 2;
        }

        cJSON* TSSessions = cJSON_GetObjectItem(TransportLayer, "Sessions");
        cJSON* TSSession = cJSON_GetObjectItem(TSSessions, "Session");
        if (TSSession != NULL && TSSession->type == cJSON_Array)
        {
            int sessionsize = cJSON_GetArraySize(TSSession);
            for (int jjj = 0; jjj < sessionsize; jjj++)
            {
                cJSON* TS = cJSON_GetArrayItem(TSSession, jjj);
                char* ssttrr;
                Diag_TP_N_As = strtol(cJSON_GetObjectItem(TS, "N_As_Timeout")->valuestring, &ssttrr, 10);
                Diag_TP_N_Ar = strtol(cJSON_GetObjectItem(TS, "N_Ar_Timeout")->valuestring, &ssttrr, 10);
                Diag_TP_N_Bs = strtol(cJSON_GetObjectItem(TS, "N_Bs_Timeout")->valuestring, &ssttrr, 10);
                Diag_TP_N_Br = strtol(cJSON_GetObjectItem(TS, "N_Br_Performance_us")->valuestring, &ssttrr, 10);
                Diag_TP_N_Cs = strtol(cJSON_GetObjectItem(TS, "N_Cs_Performance_us")->valuestring, &ssttrr, 10);
                Diag_TP_N_Cr = strtol(cJSON_GetObjectItem(TS, "N_Cr_Timeout")->valuestring, &ssttrr, 10);
            }
        }


        cJSON* Services = cJSON_GetObjectItem(SW_Type, "Services");//解析数组中的对象
        cJSON* Service = cJSON_GetObjectItem(Services, "Service");
        if (Service == NULL || Service->type != cJSON_Array) continue;

        int Service_size = cJSON_GetArraySize(Service);

        for (int j = 0; j < Service_size; j++)
        {

            cJSON* Sub_Services = cJSON_GetArrayItem(Service, j);
            if (Sub_Services == NULL || Sub_Services->type != cJSON_Object)continue;

            cJSON* SID = cJSON_GetObjectItem(Sub_Services, "@ID");
            log("SID = %s", SID->valuestring);

            char* str;
            SupportServices[j].service = strtol(SID->valuestring, &str, 16);
            log("Services[%d].Service = %x", j, SupportServices[j].service);
            servicesize = Service_size;


            //读取当前服务支持的NRC
            cJSON* NegativeResponseCodes = cJSON_GetObjectItem(Sub_Services, "NegativeResponseCodes");
            if (NegativeResponseCodes == NULL || NegativeResponseCodes->type != cJSON_Object) continue; // 确保 NegativeResponseCodes 不为空并且是一个对象

            cJSON* NegativeResponseCode = cJSON_GetObjectItem(NegativeResponseCodes, "NegativeResponseCode");
            if (NegativeResponseCode == NULL || NegativeResponseCode->type != cJSON_Array)continue;


            int NRC_size = cJSON_GetArraySize(NegativeResponseCode);
            log("NRC_SIZE %d", NRC_size);

            for (int m = 0; m < NRC_size; m++)
            {
                cJSON* NRC = cJSON_GetArrayItem(NegativeResponseCode, m);
                if (NRC == NULL || NRC->type != cJSON_Object)continue;

                cJSON* NRC_Code = cJSON_GetObjectItem(NRC, "@Code");
                log("NRC Code is %s", NRC_Code->valuestring);
            }

            //读取当前服务支持的子功能
            cJSON* Subfunctions = cJSON_GetObjectItem(Sub_Services, "Subfunctions");
            if (Subfunctions == NULL || Subfunctions->type != cJSON_Object) // 确保子功能不为空并且是一个对象
            {
                if (strcmp(SID->valuestring, "14") == 0)//如果是14服务
                {
                    cJSON* DataParameters = cJSON_GetObjectItem(Sub_Services,"DataParameters");
                    
                    cJSON* DataParameter = cJSON_GetObjectItem(DataParameters, "DataParameter");

                    if (DataParameter != NULL && DataParameter->type == cJSON_Object)
                    {
                        cJSON* Data_ID = cJSON_GetObjectItem(DataParameter,"@ID");

                        if (Data_ID != NULL && cJSON_IsString(Data_ID)) {
                            log("Group of DTC is %s", Data_ID->valuestring);
                        }
                        else {
                            log("Error: Unable to retrieve Group of DTC");
                        }
                    }
                }
                
                if (strcmp(SID->valuestring, "22") == 0)//如果是22服务
                {
                    cJSON* DataIdentifiers = cJSON_GetObjectItem(Sub_Services, "DataIdentifiers");

                    cJSON* DataIdentifier_Array = cJSON_GetObjectItem(DataIdentifiers, "DataIdentifier");

                    if (DataIdentifier_Array != NULL && DataIdentifier_Array->type == cJSON_Array)
                    {

                        int DataIdentifier_size = cJSON_GetArraySize(DataIdentifier_Array);

                        for (int iii = 0; iii < DataIdentifier_size; iii++)
                        {
                            cJSON* DataIdentifier = cJSON_GetArrayItem(DataIdentifier_Array, iii);

                            cJSON* DID = cJSON_GetObjectItem(DataIdentifier, "@ID");
                            log("supported DID is %s", DID->valuestring);
                            readdidsize = DataIdentifier_size;
                            log("did num is % d", readdidsize);
                            char* strstr;
                            ReadDIDList[iii].DID = strtol(DID->valuestring, &strstr, 16);
                            log("Services0x22 DID = %x %x", ReadDIDList[iii].DID>>8, ReadDIDList[iii].DID);
                            

                            cJSON* Sessions = cJSON_GetObjectItem(DataIdentifier, "Sessions");
                            if (Sessions == NULL || Sessions->type != cJSON_Object)continue;

                            cJSON* Session = cJSON_GetObjectItem(Sessions, "Session");
                            if (Session == NULL || Session->type != cJSON_Array)continue;

                            int Session_size = cJSON_GetArraySize(Session);
                            log("session size is %d", Session_size);

                            for (int n = 0; n < Session_size; n++)
                            {
                                cJSON* Supported_Session = cJSON_GetArrayItem(Session, n);
                                if (Supported_Session == NULL || Supported_Session->type != cJSON_Object)continue;

                                cJSON* SP_Session = cJSON_GetObjectItem(Supported_Session, "@ID");

                                log("%s DID Supported Session is %s", SID->valuestring, SP_Session->valuestring);

                                char* strstrstr;
                                int supportsession = strtol(SP_Session->valuestring, &strstrstr, 16);
                                if (supportsession == 0x01)
                                {
                                    ReadDIDList[iii].Sup_Def = 1;
                                }
                                else if (supportsession == 0x03)
                                {
                                    ReadDIDList[iii].Sup_Ext = 1;
                                }
                                else
                                {
                                    ReadDIDList[iii].Sup_PBL = 1;
                                    ReadDIDList[iii].Sup_SBL = 1;
                                }

                            }
                        }

                    }
                }

                if (strcmp(SID->valuestring, "2E") == 0)//如果是2E服务
                {
                    cJSON* DataIdentifiers = cJSON_GetObjectItem(Sub_Services, "DataIdentifiers");

                    cJSON* DataIdentifier_Array = cJSON_GetObjectItem(DataIdentifiers, "DataIdentifier");

                    if (DataIdentifier_Array != NULL && DataIdentifier_Array->type == cJSON_Array)
                    {
                        int DataIdentifier_size = cJSON_GetArraySize(DataIdentifier_Array);

                        for (int iii = 0; iii < DataIdentifier_size; iii++)
                        {
                            cJSON* DataIdentifier = cJSON_GetArrayItem(DataIdentifier_Array, iii);

                            cJSON* DID = cJSON_GetObjectItem(DataIdentifier, "@ID");
                            log("supported DID is %s", DID->valuestring);
                            writedidsize = DataIdentifier_size;
                            log("writedid num is % d", writedidsize);
                            char* strstr;
                            WriteDIDList[iii].DID = strtol(DID->valuestring, &strstr, 16);
                            log("Services0x2e DID = %x", WriteDIDList[iii].DID);

                            cJSON* Sessions = cJSON_GetObjectItem(DataIdentifier, "Sessions");
                            if (Sessions == NULL || Sessions->type != cJSON_Object)continue;

                            cJSON* Session = cJSON_GetObjectItem(Sessions, "Session");
                            if (Session == NULL || Session->type != cJSON_Array)continue;

                            int Session_size = cJSON_GetArraySize(Session);
                            log("session size is %d", Session_size);

                            for (int n = 0; n < Session_size; n++)
                            {
                                cJSON* Supported_Session = cJSON_GetArrayItem(Session, n);
                                if (Supported_Session == NULL || Supported_Session->type != cJSON_Object)continue;

                                cJSON* SP_Session = cJSON_GetObjectItem(Supported_Session, "@ID");

                                log("%s DID Supported Session is %s", SID->valuestring, SP_Session->valuestring);

                                char* strstrstr;
                                int supportsession = strtol(SP_Session->valuestring, &strstrstr, 16);
                                if (supportsession == 0x01)
                                {
                                    WriteDIDList[iii].Sup_Def = 1;
                                }
                                else if (supportsession == 0x03)
                                {
                                    WriteDIDList[iii].Sup_Ext = 1;
                                }
                                else
                                {
                                    WriteDIDList[iii].Sup_PBL = 1;
                                    WriteDIDList[iii].Sup_SBL = 1;
                                }
                            }
                        }

                    }
                    else if(DataIdentifier_Array != NULL && DataIdentifier_Array->type == cJSON_Object)
                    {

                        cJSON* DID = cJSON_GetObjectItem(DataIdentifier_Array, "@ID");
                        log("supported DID is %s", DID->valuestring);
                        char* strstr;
                        WriteDIDList[0].DID = strtol(DID->valuestring, &strstr, 16);
                        log("Services0x2e DID = %x", WriteDIDList[0].DID);
                        writedidsize = 1;

                        cJSON* Sessions = cJSON_GetObjectItem(DataIdentifier_Array, "Sessions");
                        if (Sessions == NULL || Sessions->type != cJSON_Object)continue;

                        cJSON* Session = cJSON_GetObjectItem(Sessions, "Session");
                        if (Session == NULL || Session->type != cJSON_Array)continue;

                        int Session_size = cJSON_GetArraySize(Session);
                        log("session size is %d", Session_size);

                        for (int n = 0; n < Session_size; n++)
                        {
                            cJSON* Supported_Session = cJSON_GetArrayItem(Session, n);
                            if (Supported_Session == NULL || Supported_Session->type != cJSON_Object)continue;

                            cJSON* SP_Session = cJSON_GetObjectItem(Supported_Session, "@ID");

                            log("%s DID Supported Session is %s", SID->valuestring, SP_Session->valuestring);

                            char* strstrstr;
                            int supportsession = strtol(SP_Session->valuestring, &strstrstr, 16);
                            if (supportsession == 0x01)
                            {
                                WriteDIDList[0].Sup_Def = 1;
                            }
                            else if (supportsession == 0x03)
                            {
                                WriteDIDList[0].Sup_Ext = 1;
                            }
                            else
                            {
                                WriteDIDList[0].Sup_PBL = 1;
                                WriteDIDList[0].Sup_SBL = 1;
                            }
                        }
                    }
                    
                }


                if (strcmp(SID->valuestring, "2F") == 0)//如果是2F服务
                {
                    cJSON* DataIdentifiers = cJSON_GetObjectItem(Sub_Services, "DataIdentifiers");

                    cJSON* DataIdentifier_Array = cJSON_GetObjectItem(DataIdentifiers, "DataIdentifier");

                    if (DataIdentifier_Array != NULL && DataIdentifier_Array->type == cJSON_Array)
                    {

                        int DataIdentifier_size = cJSON_GetArraySize(DataIdentifier_Array);

                        for (int iii = 0; iii < DataIdentifier_size; iii++)
                        {
                            cJSON* DataIdentifier = cJSON_GetArrayItem(DataIdentifier_Array, iii);

                            cJSON* DID = cJSON_GetObjectItem(DataIdentifier, "@ID");
                            log("supported DID is %s", DID->valuestring);
                            char* strstr;
                            IODIDList[iii].DID = strtol(DID->valuestring, &strstr, 16);
                            log("Services0x2e DID = %x", IODIDList[iii].DID);
                            iodidsize = DataIdentifier_size;
                            log("writedid num is %d", iodidsize);

                            cJSON* Sessions = cJSON_GetObjectItem(DataIdentifier, "Sessions");
                            if (Sessions == NULL || Sessions->type != cJSON_Object)continue;

                            cJSON* Session = cJSON_GetObjectItem(Sessions, "Session");
                            if (Session == NULL || Session->type != cJSON_Array)continue;

                            int Session_size = cJSON_GetArraySize(Session);
                            log("session size is %d", Session_size);

                            for (int n = 0; n < Session_size; n++)
                            {
                                cJSON* Supported_Session = cJSON_GetArrayItem(Session, n);
                                if (Supported_Session == NULL || Supported_Session->type != cJSON_Object)continue;

                                cJSON* SP_Session = cJSON_GetObjectItem(Supported_Session, "@ID");

                                log("%s DID Supported Session is %s", SID->valuestring, SP_Session->valuestring);

                            }
                        }

                    }
                    else if (DataIdentifier_Array != NULL && DataIdentifier_Array->type == cJSON_Object)
                    {

                        cJSON* DID = cJSON_GetObjectItem(DataIdentifier_Array, "@ID");
                        log("supported DID is %s", DID->valuestring);
                        char* strstr;
                        IODIDList[0].DID = strtol(DID->valuestring, &strstr, 16);
                        log("Services0x2e DID = %x", IODIDList[0].DID);
                        iodidsize = 1;

                        cJSON* Sessions = cJSON_GetObjectItem(DataIdentifier_Array, "Sessions");
                        if (Sessions == NULL || Sessions->type != cJSON_Object)continue;

                        cJSON* Session = cJSON_GetObjectItem(Sessions, "Session");
                        if (Session == NULL || Session->type != cJSON_Array)continue;

                        int Session_size = cJSON_GetArraySize(Session);
                        log("session size is %d", Session_size);

                        for (int n = 0; n < Session_size; n++)
                        {
                            cJSON* Supported_Session = cJSON_GetArrayItem(Session, n);
                            if (Supported_Session == NULL || Supported_Session->type != cJSON_Object)continue;

                            cJSON* SP_Session = cJSON_GetObjectItem(Supported_Session, "@ID");

                            log("%s DID Supported Session is %s", SID->valuestring, SP_Session->valuestring);
                        }
                    }
                }

                continue;
            }

            cJSON* Subfunction = cJSON_GetObjectItem(Subfunctions, "Subfunction");
            if (Subfunction == NULL || Subfunction->type != cJSON_Array)//如果只有一个子功能
            {
                cJSON* SFID = cJSON_GetObjectItem(Subfunction, "@ID");
                log("SF ID is %s", SFID->valuestring);
                char* str;
                SupportServices[j].subfunc[0].Subfunc = strtol(SFID->valuestring, &str, 16);
                SupportServices[j].subfuncsize = 1;

                if (strcmp(SID->valuestring, "31") == 0 && strcmp(SFID->valuestring, "01") == 0)//如果是31服务
                {
                    cJSON* RoutineIdentifiers = cJSON_GetObjectItem(SFID, "RoutineIdentifiers");
                    if (SFID == NULL || SFID->type != cJSON_Object)continue;

                    cJSON* RoutineIdentifier = cJSON_GetObjectItem(RoutineIdentifiers, "RoutineIdentifier");

                    if (RoutineIdentifier != NULL && RoutineIdentifier->type == cJSON_Object)
                    {
                        cJSON* RID = cJSON_GetObjectItem(RoutineIdentifier, "@ID");

                        char* endptr;
                        RIDList[0].RID = strtol(RID->valuestring, &endptr, 16);
                        log("Services0x31 RID = %x", RIDList[0].RID);

                        cJSON* RoutineType = cJSON_GetObjectItem(RoutineIdentifier, "@RoutineType");
                        RIDList[0].RoutineType = atoi(RID->valuestring);
                        log("RoutineType = %d", RIDList[0].RoutineType);

                        ridsize = 1;
                        log("rid num is %d", ridsize);
                    }
                    else if (RoutineIdentifier != NULL && RoutineIdentifier->type == cJSON_Array)
                    {
                        int RoutineIdentifier_size = cJSON_GetArraySize(RoutineIdentifier);
                        log("RoutineIdentifier_size %d", RoutineIdentifier_size);
                        for (int mm = 0; mm < RoutineIdentifier_size; mm++)
                        {
                            cJSON* Routine = cJSON_GetArrayItem(RoutineIdentifier, mm);
                            cJSON* RID = cJSON_GetObjectItem(Routine, "@ID");
                            if (RID == NULL || RID->type != cJSON_String)continue;

                            char* endptr;
                            RIDList[mm].RID = strtol(RID->valuestring, &endptr, 16);
                            log("Services0x31 RID = %x", RIDList[mm].RID);

                            cJSON* RoutineType = cJSON_GetObjectItem(Routine, "@RoutineType");
                            RIDList[mm].RoutineType = atoi(RoutineType->valuestring);
                            log("RoutineType = %d", RIDList[mm].RoutineType);

                            ridsize = RoutineIdentifier_size;
                            log("rid num is %d", ridsize);
                        }
                    }
                    continue;
                }

                cJSON* Sessions = cJSON_GetObjectItem(Subfunction, "Sessions");
                if (Sessions == NULL || Sessions->type != cJSON_Object)continue;

                cJSON* Session = cJSON_GetObjectItem(Sessions, "Session");
                if (Session == NULL || Session->type != cJSON_Array)continue;

                int Session_size = cJSON_GetArraySize(Session);
                log("session size is %d", Session_size);

                for (int n = 0; n < Session_size; n++)
                {
                    cJSON* Supported_Session = cJSON_GetArrayItem(Session, n);
                    if (Supported_Session == NULL || Supported_Session->type != cJSON_Object)continue;

                    cJSON* SP_Session = cJSON_GetObjectItem(Supported_Session, "@ID");

                    log("Supported Session is %s", SP_Session->valuestring);
                    
                    char* strstr;
                    u8 supsession = strtol(SP_Session->valuestring, &strstr, 16);
                    if (supsession == 0x01)
                        SupportServices[j].subfunc[0].Sup_Def = 1;
                    else if(supsession==0x03)
                        SupportServices[j].subfunc[0].Sup_Ext = 1;
                    else {
                        SupportServices[j].subfunc[0].Sup_PBL = 1;
                        SupportServices[j].subfunc[0].Sup_SBL = 1;
                    }
                }

                continue;
            }

            int Subfunction_size = cJSON_GetArraySize(Subfunction);//如果是多个子功能
            log("Subfunction_SIZE %d", Subfunction_size);

            for (int m = 0; m < Subfunction_size; m++)
            {
                cJSON* SF = cJSON_GetArrayItem(Subfunction, m);
                if (SF == NULL || SF->type != cJSON_Object)continue;

                cJSON* SFID = cJSON_GetObjectItem(SF, "@ID");
                log("SF ID is %s", SFID->valuestring);
                char* str;
                SupportServices[j].subfunc[m].Subfunc = strtol(SFID->valuestring, &str, 16);
                SupportServices[j].subfuncsize = Subfunction_size;

                if (strcmp(SID->valuestring, "31") == 0 && strcmp(SFID->valuestring, "01") == 0)//如果是31服务
                {
                    cJSON* RoutineIdentifiers = cJSON_GetObjectItem(SF, "RoutineIdentifiers");
                    if (SF == NULL || SF->type != cJSON_Object)continue;

                    cJSON* RoutineIdentifier = cJSON_GetObjectItem(RoutineIdentifiers, "RoutineIdentifier");

                    if (RoutineIdentifier != NULL && RoutineIdentifier->type == cJSON_Object)
                    {
                        cJSON* RID = cJSON_GetObjectItem(RoutineIdentifier, "@ID");

                        char* endptr;
                        RIDList[0].RID = strtol(RID->valuestring, &endptr, 16);
                        log("Services0x31 RID = %x", RIDList[0].RID);

                        cJSON* RoutineType = cJSON_GetObjectItem(RoutineIdentifier, "@RoutineType");
                        RIDList[0].RoutineType = atoi(RoutineType->valuestring);
                        log("RoutineType = %d", RIDList[0].RoutineType);
                        
                        ridsize = 1;
                        log("rid num is %d", ridsize);
                    }
                    else if (RoutineIdentifier != NULL && RoutineIdentifier->type == cJSON_Array)
                    {
                        int RoutineIdentifier_size = cJSON_GetArraySize(RoutineIdentifier);
                        log("RoutineIdentifier_size %d", RoutineIdentifier_size);
                        for (int mm = 0; mm < RoutineIdentifier_size; mm++)
                        {
                            cJSON* Routine = cJSON_GetArrayItem(RoutineIdentifier, mm);
                            cJSON* RID = cJSON_GetObjectItem(Routine, "@ID");
                            if (RID == NULL || RID->type != cJSON_String)continue;

                            char* endptr;
                            RIDList[mm].RID = strtol(RID->valuestring, &endptr, 16);
                            log("Services0x31 RID = %x", RIDList[mm].RID);

                            cJSON* RoutineType = cJSON_GetObjectItem(Routine, "@RoutineType");
                            RIDList[mm].RoutineType = atoi(RoutineType->valuestring);
                            log("RoutineType = %d", RIDList[mm].RoutineType);

                            ridsize = RoutineIdentifier_size;
                            log("rid num is %d", ridsize);
                        }
                    }
                    continue;
                }


                cJSON* Sessions = cJSON_GetObjectItem(SF,"Sessions");
                if (Sessions == NULL || Sessions->type != cJSON_Object)continue;

                cJSON* Session = cJSON_GetObjectItem(Sessions, "Session");
                if (Session == NULL || Session->type != cJSON_Array)continue;

                int Session_size = cJSON_GetArraySize(Session);
                log("session size is %d", Session_size);

                for (int n = 0; n < Session_size; n++)
                {
                    cJSON* Supported_Session = cJSON_GetArrayItem(Session, n);
                    if (Supported_Session == NULL || Supported_Session->type != cJSON_Object)continue;
                    
                    cJSON* SP_Session = cJSON_GetObjectItem(Supported_Session, "@ID");

                    log("Supported Session is %s", SP_Session->valuestring);
                    char* strstr;
                    u8 supsession = strtol(SP_Session->valuestring, &strstr, 16);
                    if (supsession == 0x01)
                        SupportServices[j].subfunc[m].Sup_Def = 1;
                    else if (supsession == 0x03)
                        SupportServices[j].subfunc[m].Sup_Ext = 1;
                    else {
                        SupportServices[j].subfunc[m].Sup_PBL = 1;
                        SupportServices[j].subfunc[m].Sup_SBL = 1;
                    }
                }

            }

            //如果是SID19
            if(strcmp(SID->valuestring,"19")==0)
            {
                cJSON* DTCS = cJSON_GetObjectItem(Sub_Services, "DTCS");
                
                cJSON* DTC_Array = cJSON_GetObjectItem(DTCS,"DTC");
                if (DTC_Array == NULL || DTC_Array->type != cJSON_Array)continue;

                int DTC_size = cJSON_GetArraySize(DTC_Array);
                for (int p = 0; p < DTC_size; p++)
                {
                    cJSON* DTC = cJSON_GetArrayItem(DTC_Array, p);
                    if (DTC == NULL || DTC->type != cJSON_Object)continue;

                    cJSON* DTC_ID = cJSON_GetObjectItem(DTC,"@ID");

                    log("Supported DTC is %s", DTC_ID->valuestring);

                    char* str;
                    DTCList[p].DTC = strtol(DTC_ID->valuestring, &str, 16);
                    log("Supported DTC is %x",DTCList[p].DTC);

                    dtcsize = DTC_size;
                    log("dtc num is %d", dtcsize);
                }
            }

        }

    }

    // 释放缓存
    cJSON_Delete(pdx_root);
    free(pdx_jsonData);

    log("JSON file read successfully.");

    return 0;
}


void SDDB_Parser()
{
    char INI_FILE_PATH[200] = "";
    char SDDB_FILE_PATH[200] = "";
    char SDDB_JSON_PATH[200] = "";
    
    char title[200] = "DiagUserDefine";
    char key[20] = "SDDB_Path";
    char Buffer[200] = "";

    char* iniptr = INI_FILE_PATH;
    char* jsonptr = SDDB_FILE_PATH;

    GetIniFilePath(&iniptr);
    
    GetINIKeyString(title, key, iniptr, Buffer);
    strcpy(SDDB_FILE_PATH, Buffer);
    printf("SDDB PATH IS %s", SDDB_FILE_PATH);

    GetSDDBJSONFilePath(&jsonptr);

    //Test_Report.sddb_to_json(SDDB_FILE_PATH, jsonptr);
    //解析sddb到json,获取json路径

    sddb_json_reader(jsonptr);

    app.wait(1000, "");

    //写入到INI
    GetIniFilePath(&iniptr);

    int ret = 0;

    strcpy(title, "DiagParameters");
    char val[50] = "";

    strcpy(key, "Bus_Type");
    strcpy(val, "");
    sprintf(val, "%d", BusTypeNow);
    PutINIKeyString(title, key, val, iniptr);

    strcpy(key, "ECU_Address");
    strcpy(val, "0x");
    sprintf(val, "%x", ECU_Address);
    ret = PutINIKeyString(title, key, val, iniptr);
    //printf("%d %d", ret,errno);

    strcpy(key, "StartupTime");
    strcpy(val, "");
    sprintf(val, "%2d", ECUResetTime);
    PutINIKeyString(title, key, val, iniptr);

    strcpy(key, "AMask");
    strcpy(val, "");
    sprintf(val, "0x%x", AvaliableMask);
    PutINIKeyString(title, key, val, iniptr);

    strcpy(key, "N_As");
    strcpy(val, "");
    sprintf(val, "%d", Diag_TP_N_As);
    PutINIKeyString(title, key, val, iniptr);

    strcpy(key, "N_Ar");
    strcpy(val, "");
    sprintf(val, "%d", Diag_TP_N_Ar);
    PutINIKeyString(title, key, val, iniptr);

    strcpy(key, "N_Bs");
    strcpy(val, "");
    sprintf(val, "%d", Diag_TP_N_Bs);
    PutINIKeyString(title, key, val, iniptr);

    strcpy(key, "N_Br");
    strcpy(val, "");
    sprintf(val, "%d", Diag_TP_N_Br);
    PutINIKeyString(title, key, val, iniptr);

    strcpy(key, "N_Cs");
    strcpy(val, "");
    sprintf(val, "%d", Diag_TP_N_Cs);
    PutINIKeyString(title, key, val, iniptr);

    strcpy(key, "N_Cr");
    strcpy(val, "");
    sprintf(val, "%d", Diag_TP_N_Cr);
    PutINIKeyString(title, key, val, iniptr);

    app.wait(1000, "");
}





#endif // !PDXParser
