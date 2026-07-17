#ifndef TestReport
#define TestReport


#include <TSMaster.h>
#include <string>

#define MAX_BUFFER_SIZE 1023
u8 ReportDescriptionFlag = 0;//TP层报告开关

native_int Handle=0;
native_int HTMLHandle;
s32 StepNumber=1;
s32 stepnumber = 1;
u8 resultFlag = 0;
s32 language_id = 0;
s64 timestamp = 0;

enum TestResult { TestOK = 0, TestNOK = 1, TestWarning = 2, TestNA = 3, Pass, Fail, Info, Warning };
TestResult Test_Result;

char requestInfo[5000] = { '\0' };
char expectedResponse[5000] = { '\0' };
char actualResponse[5000] = { '\0' };
char FinalStepInfo[5000] = { '\0' };
char openissue[50000] = "";
char issuereason[5000] = "";
char printChar[5000] = { '\0' };
char StepDespriction[256];
//openissue
char issuetype[200] = "";
char testPlatform[200] = "";
char ECUName[200] = "";
char SupplierName[200] = "";
char TestEnvironment[200] = "";
char TestApartment[200] = "";
char TestStage[200] = "";
char TestEngineer[200] = "";
char AppVersion[500] = "";
u8 issueflag = 0;
char Manufacturer[200] = "";
char ProjectName[200] = "";
char DUTName[200] = "";

char TestCaseName[200] = { '\0' };
char TestScopeName[200] = { '\0' };
char tableNum[100] = { '\0' };
char logPath[300] = { '\0' };

//enum TestResult { Pass, Fail, Info, Warning};

char testPurpose_CH[200] = { '\0' };
char testPurpose_EN[200] = { '\0' };

u8 reportOutputFlag = 0;//应用层报告打印开关

/// <summary>
/// html
/// </summary>
void TestReport_HTML_Init()
{
	//test_report.test_init(&HTMLHandle);
	test_report.test_init(&HTMLHandle);
	char HTMLReportPath[1024], HTMLLogoPath[1024];
	test_report.test_init(&HTMLHandle);//自动获取.cpp的名 
	app.get_system_var_string("LatestHTMLPath", 1024, &logPath[0]);
	sprintf(HTMLReportPath, "%s\\DTC_Report.html", logPath);
	test_report.test_set_report_name(HTMLHandle, HTMLReportPath);
	sprintf(HTMLLogoPath, "%s\\TSMaster.svg", logPath);
	test_report.test_set_html_name_and_logo(HTMLHandle, "DTC_Report", HTMLLogoPath);
}
void TestReport_HTML_Title(char* Group, char* CaseName, char* purpose = NULL, char* image = NULL)
{
	test_report.test_title(HTMLHandle, Group, CaseName, image, purpose);
	printf("init title sucessful");
}

void TestReport_HTML_Step(s32 stepnumber, char* StepInfo, enum TestResult Result = Info)
{
	char StepNum[10];
	sprintf(StepNum, "%2d", stepnumber);
	strcpy(openissue, "");
	app.log_text(StepInfo, lvlInfo);
	switch (Result)
	{
	case Pass:
		test_report.test_pass(HTMLHandle, StepNum, StepInfo);
		break;
	case Fail:
	{
		s32 year;
		s32 month;
		s32 day;
		s32 hour;
		s32 minute;
		s32 second;
		s32 milliseconds;
		if (0 == app.get_date_time(&year, &month, &day, &hour, &minute, &second, &milliseconds)) {
			// 成功
		}
		char timenow[200] = "";
		sprintf(timenow, "%d年%d月%d日 %d:%d:%d", year, month, day, hour, minute, second);

		char TestID[100] = "";
		strncpy(TestID, TestCaseName, 10);
		
		//snprintf(openissue, sizeof(openissue), "%s,%s,%s,%s,%s,Step：%s Expected result：%s Actual result：%s,,%s,%s,%s,%s,%s,%s,,,,,,%s,,%s", issuetype, TestID, issuereason, timenow, "open", requestInfo, expectedResponse, actualResponse, testPlatform, ECUName,AppVersion, SupplierName, TestEnvironment, TestApartment, TestStage, TestEngineer);
		strcpy(openissue, issuetype);
		strcat(openissue, ",");
		strcat(openissue, TestID);
		strcat(openissue, ",");
		strcat(openissue, issuereason);
		strcat(openissue, ",");
		strcat(openissue, timenow);
		strcat(openissue, ",");
		strcat(openissue, "OPEN");
		strcat(openissue, ",");
		strcat(openissue, "Step:");
		strcat(openissue, requestInfo);
		strcat(openissue, "Expected Result:");
		strcat(openissue, expectedResponse);
		strcat(openissue, "Actual Result:");
		strcat(openissue, actualResponse);
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, testPlatform);
		strcat(openissue, ",");
		strcat(openissue, ECUName);
		strcat(openissue, ",");
		strcat(openissue, AppVersion);
		strcat(openissue, ",");
		strcat(openissue, SupplierName);
		strcat(openissue, ",");
		strcat(openissue, TestEnvironment);
		strcat(openissue, ",");
		strcat(openissue, TestApartment);
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, TestStage);
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, TestEngineer);

		test_report.test_openissue(HTMLHandle, openissue);
		test_report.test_fail(HTMLHandle, StepNum, StepInfo);
		resultFlag++;
		Test_Result = TestNOK;
		break;
	}
	case Info:
		test_report.test_step(HTMLHandle, StepNum, StepInfo);
		break;
	case Warning:
		test_report.test_warning(HTMLHandle, StepNum, StepInfo);
		break;
	default:
		break;
	}
}

void TestStep(const char* StepInfo)
{
	stepnumber++;
	TestReport_HTML_Step(stepnumber, const_cast<char*>(StepInfo));
}

void TestReport_HTML_StepDescription(const char* StepInfo, enum TestResult Result = Info)
{
	strcpy(openissue, "");

	app.log_text(StepInfo, lvlInfo);

	switch (Result)
	{
	case Pass:
		test_report.test_pass(HTMLHandle, "", StepInfo);
		break;
	case Fail:
	{
		s32 year;
		s32 month;
		s32 day;
		s32 hour;
		s32 minute;
		s32 second;
		s32 milliseconds;
		if (0 == app.get_date_time(&year, &month, &day, &hour, &minute, &second, &milliseconds)) {
			// 成功
		}
		char timenow[200] = "";
		sprintf(timenow, "%d年%d月%d日 %d:%d:%d", year, month, day, hour, minute, second);

		char TestID[100] = "";
		strncpy(TestID, TestCaseName, 10);

		//snprintf(openissue, sizeof(openissue), "%s,%s,%s,%s,%s,Step：%s Expected result：%s Actual result：%s,,%s,%s,%s,%s,%s,%s,,,,,,%s,,%s", issuetype, TestID, issuereason, timenow, "open", requestInfo, expectedResponse, actualResponse, testPlatform, AppVersion, SupplierName, TestEnvironment, TestApartment, TestStage, TestEngineer);
		strcpy(openissue, issuetype);
		strcat(openissue, ",");
		strcat(openissue, TestID);
		strcat(openissue, ",");
		strcat(openissue, issuereason);
		strcat(openissue, ",");
		strcat(openissue, timenow);
		strcat(openissue, ",");
		strcat(openissue, "OPEN");
		strcat(openissue, ",");
		strcat(openissue, "Step:");
		strcat(openissue, requestInfo);
		strcat(openissue, " Expected Result:");
		strcat(openissue, expectedResponse);
		strcat(openissue, " Actual Result:");
		strcat(openissue, actualResponse);
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, testPlatform);
		strcat(openissue, ",");
		strcat(openissue, ECUName);
		strcat(openissue, ",");
		strcat(openissue, AppVersion);
		strcat(openissue, ",");
		strcat(openissue, SupplierName);
		strcat(openissue, ",");
		strcat(openissue, TestEnvironment);
		strcat(openissue, ",");
		strcat(openissue, TestApartment);
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, TestStage);
		strcat(openissue, ",");
		strcat(openissue, ",");
		strcat(openissue, TestEngineer);

		test_report.test_openissue(HTMLHandle, openissue);
		test_report.test_fail(HTMLHandle, "", StepInfo);
		resultFlag++;
		Test_Result = TestNOK;
		break;
	}
	case Info:
		test_report.test_step(HTMLHandle, "", StepInfo);
		break;
	case Warning:
		test_report.test_warning(HTMLHandle, "", StepInfo);
		break;
	default:
		break;
	}
}

void TestReport_HTML_Final()
{
	//test_report.test_final(HTMLHandle);
	test_report.test_final(HTMLHandle);
}

/// <summary>
/// word
/// </summary>
void TestReport_Init()
{
	//test_report.test_report_init(&HTMLHandle);
}

void TestReport_Cover()
{
	//test_report.test_report_cover(Handle, "GEELY CAN DIAGNOSTIC TEST", "1.0", "", "TOSUN");
}

void TestReport_AddHeader(char* HeaderInfo, u8 HeaderLevel)
{
	//test_report.test_report_add_head(Handle, HeaderInfo, HeaderLevel);
}

void TestReport_AddImage(char* addrInfo, char* Describe, float Asize)
{
	//test_report.test_report_add_image(Handle, addrInfo, Describe,Asize);
}

void TestReport_AddInfo(char* infoDescribe)
{
	//test_report.test_report_add_info(Handle, infoDescribe);
}

void TestReport_CreateTable( char* tableName)
{
	//if (language_id == 2052)
	//	test_report.test_report_start_add_table(Handle, "时间戳,步骤,操作,期望结果,实际结果,测试结果", tableName, 6);
	//else
	//	test_report.test_report_start_add_table(Handle, "TimeStamp,Step,Action,Expected Result,Actual Result,Test Result", tableName, 6);
}

void TestReport_EndTable()
{
	//test_report.test_report_end_add_table(Handle);
}

void TestReport_Step(char* stepInfo)
{
	//test_report.test_report_add_table_info(Handle, stepInfo);
}

void TestReport_Result(char* caseName , u8 reportResult)
{
	//test_report.test_report_test_result(&test, Handle, caseName, reportResult);
}

void TestReport_End()
{
	//test_report.test_report_finalize(Handle);
	test_report.test_final(HTMLHandle);
}

void TestReport_StartLogging()
{
	strcpy(logPath,"");
	app.get_system_var_string("LatestHTMLPath", 300, logPath);
	strcat(logPath, "ReportData\\");
	logPath[strlen(logPath)] = '\0';

	//log
	//app.start_system_message_log(logPath);

	strcat(logPath, TestCaseName);
	strcat(logPath, ".blf\0");
	app.start_log_w_filename(logPath);
}

void TestReport_StopLogging()
{
	app.end_log();
	//log
	//char* s;
	//app.end_system_message_log(&s);
}

void TestReport_TableInit()
{
	//TestReport_AddHeader(TestScopeName, 1);
	//TestReport_AddHeader(TestCaseName, 2);

	if (language_id == 2052)
	{
		//TestReport_AddInfo(testPurpose_CH);
		TestReport_HTML_Title(TestScopeName, TestCaseName, testPurpose_CH);
	}
	else
	{
		//TestReport_AddInfo(testPurpose_EN);
		TestReport_HTML_Title(TestScopeName, TestCaseName, testPurpose_EN);
	}

	//strncpy(tableNum, TestCaseName, 10);
	//TestReport_CreateTable(tableNum);
	StepNumber = 0;
	stepnumber = 0;
}

void TestReport_ExportReport()
{
	//test_report.test_export_report();
}

void TestReport_StepInfo(TestResult ResultInfo)
{
	char printF[5000] = { "\0" };
	char html_info[5000] = "";

	strcpy(FinalStepInfo, "");

	if(reportOutputFlag==1)
	{
		StepNumber++;

		app.get_timestamp(&timestamp);

		double ttiimmee = double(timestamp) / 1000000;

		strcat(FinalStepInfo,std::to_string(ttiimmee).c_str());
		strcat(FinalStepInfo, ",");

		if (language_id == 2052)
		{
			strcat(FinalStepInfo,std::to_string(StepNumber).c_str());
			strcat(FinalStepInfo,",发送请求：");
			strcat(FinalStepInfo, requestInfo);

			strcpy(printF, "发送请求：");
			strcat(printF, requestInfo);
			app.log_text(requestInfo, lvlInfo);

			strcat(FinalStepInfo, ",期望响应：");
			strcat(FinalStepInfo, expectedResponse);
			strcat(FinalStepInfo, ",实际响应：");
			strcat(FinalStepInfo, actualResponse);

			if (ResultInfo == Pass)
			{
				strcpy(printF, "ECU响应：");
				strcat(printF, actualResponse);
				strcat(printF, ";3");
				app.log_text(printF, lvlOK);
			}
			else
			{
				strcpy(printF, "ECU响应：");
				strcat(printF, actualResponse);
				strcat(printF, ";1");
				app.log_text(printF, lvlError);
			}

			strcat(FinalStepInfo, ",");
		}
		else
		{
			strcat(FinalStepInfo, std::to_string(StepNumber).c_str());
			strcat(FinalStepInfo, ",Send Request：");
			strcat(FinalStepInfo, requestInfo);
			strcat(FinalStepInfo, ",Expected Resposne：");
			strcat(FinalStepInfo, expectedResponse);
			strcat(FinalStepInfo, ",DUT Response");
			strcat(FinalStepInfo, actualResponse);
			strcat(FinalStepInfo, ",");
		}


		if (ResultInfo != Pass)
			resultFlag += 1;

		sprintf(html_info, "%s%s%s%s%s%s", "发送请求：", requestInfo, ",期望响应：", expectedResponse, ",实际响应：", actualResponse);

		//TestReport_Step(FinalStepInfo);
		TestReport_HTML_StepDescription(html_info, ResultInfo);
	}
}



void TestReport_PrintInfo(char* InfoToShow)
{
	app.log_text(InfoToShow,lvlInfo);
}

void PrintCaseNameAndPurpose()
{
	if(language_id == 2052)
	{
		strcpy(printChar, "测试用例名称:");
		strcat(printChar, TestCaseName);
		TestReport_PrintInfo(printChar);
		strcpy(printChar, testPurpose_CH);
		TestReport_PrintInfo(printChar);
	}
	else
	{
		strcpy(printChar, "TestCaseName:");
		strcat(printChar, TestCaseName);
		TestReport_PrintInfo(printChar);
		strcpy(printChar, testPurpose_EN);
		TestReport_PrintInfo(printChar);
	}

}


void SetReportDescription(u8 flag)
{
	//char word[200] = ",,关闭报告生成";
	char html[200] = "关闭报告生成";
	//char word1[200] = ",,开启报告生成";
	char html1[200] = "开启报告生成";
	if (flag == 0)
	{
		reportOutputFlag = 0;
		//TestReport_Step(word);
		TestReport_HTML_Step(StepNumber, html);
	}
	else
	{
		reportOutputFlag = 1;
		//TestReport_Step(word1);
		TestReport_HTML_Step(StepNumber, html1);
	}
}

void TestReport_TestInfo(const native_int Handle, const char* ProductName, const char* HWVersion, 
	const char* SWVersion, const char* DUTVersion, const char* TestEngineer, const char* ManufacturerName) {
	test_report.test_testinfo(Handle, ProductName, HWVersion, SWVersion, DUTVersion, TestEngineer, ManufacturerName);
}



//添加测试信息pass
void TestInfoPrintPass(const char* descriptionsBuffer, ...)
{
	char Buffer[1024] = "";
	char Buffer1[1024] = "";
	char Buffer2[1024] = "";

	va_list ap;
	va_start(ap, descriptionsBuffer);
	vsnprintf(Buffer, MAX_BUFFER_SIZE, descriptionsBuffer, ap);
	//vsnprintf(Buffer, strlen(descriptionsBuffer) + len, descriptionsBuffer, ap);
	va_end(ap);

	//InfoStep++;//html和word都用不到
	sprintf(Buffer1, "%s", Buffer);
	// app.log_text(Buffer, lvlOK);
	app.log_text(Buffer1, lvlOK);

	//std::string tmp = std::to_string(InfoStep);
	//char const* InfoStep_char = tmp.c_str();//可以正常打出1234来


	test_report.test_pass(HTMLHandle, "", Buffer1);//,之后的在报告打不出，但是可以在系统消息打出来


	//Test_Report.test_report_add_info(Handle, Buffer2);

}
//添加测试信息fail
void TestInfoPrintFail(const char* descriptionsBuffer, ...)
{
	char Buffer[1024] = "";
	char Buffer1[1024] = "";
	char Buffer2[1024] = "";

	va_list ap;
	va_start(ap, descriptionsBuffer);
	vsnprintf(Buffer, MAX_BUFFER_SIZE, descriptionsBuffer, ap);
	//vsnprintf(Buffer, strlen(descriptionsBuffer) + len, descriptionsBuffer, ap);
	va_end(ap);

	//system_log
	//InfoStep++;
	sprintf(Buffer1, "%s", Buffer);
	app.log_text(Buffer1, lvlError);

	//std::string tmp = std::to_string(InfoStep);
	//char const* InfoStep_char = tmp.c_str();//可以正常打出1234来

	test_report.test_fail(HTMLHandle, "", Buffer1);//,之后的在报告打不出，但是可以在系统消息打出来
	resultFlag++;
	Test_Result = TestNOK;

	//Test_Report.test_report_add_info(Handle, Buffer2);

}

//添加测试信息pass
void TestInfoPrintWarning(const char* descriptionsBuffer, ...)
{
	char Buffer[1024] = "";
	char Buffer1[1024] = "";
	char Buffer2[1024] = "";

	va_list ap;
	va_start(ap, descriptionsBuffer);
	vsnprintf(Buffer, MAX_BUFFER_SIZE, descriptionsBuffer, ap);
	// vsnprintf(Buffer, strlen(descriptionsBuffer) + len, descriptionsBuffer, ap);
	va_end(ap);

	//system_lo
	//InfoStep++;//html和word都用不到
	sprintf(Buffer1, "%s", Buffer);
	app.log_text(Buffer1, lvlWarning);


	test_report.test_warning(HTMLHandle, "", Buffer1);//,之后的在报告打不出，但是可以在系统消息打出来

	//Test_Report.test_report_add_info(Handle, Buffer2);

}


//添加测试信息pass
void TestInfoPrint(const char* descriptionsBuffer, ...)
{
	char Buffer[1024] = "";
	char Buffer1[1024] = "";
	char Buffer2[1024] = "";

	va_list ap;
	va_start(ap, descriptionsBuffer);
	vsnprintf(Buffer, MAX_BUFFER_SIZE, descriptionsBuffer, ap);
	// vsnprintf(Buffer, strlen(descriptionsBuffer) + len, descriptionsBuffer, ap);
	va_end(ap);

	//system_lo
	//InfoStep++;//html和word都用不到
	sprintf(Buffer1, "%s", Buffer);
	app.log_text(Buffer1, lvlHint);


	test_report.test_step(HTMLHandle, "", Buffer1);//,之后的在报告打不出，但是可以在系统消息打出来

	//Test_Report.test_report_add_info(Handle, Buffer2);

}







class testException : public _exception
{
public:
	testException() {}
	~testException() throw()
	{
	}

	virtual const char* what() const throw()
	{
		return message.c_str();
	}

private:
	std::string message = "用户退出测试用例";
};
static inline void CHK_EXIT()
{
	do
	{
		if (0 == test.check_test_terminate())
		{
			throw testException();
		}
	} while (0);
}

#endif // !TestReport
