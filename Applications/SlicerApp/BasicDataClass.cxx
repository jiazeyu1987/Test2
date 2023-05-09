#include "BasicDataClass.h"
#define LOGIN_PERSPECTIVE ""


char* BasicConfigData::patientNameAgeAdvice = "patientNameAgeAdvice"; //保存到GetDataStorage()中患者的信息的DataNode的名称，节点包括属性名patientName，age，advice
char* BasicConfigData::patientName = "patientName"; //姓名
char* BasicConfigData::age = "age"; //年龄
char* BasicConfigData::advice = "advice"; //医嘱QString BasicConfigData::VarFileName = "/Config/variable.xml";

//char* BasicConfigData::REG_FAIL = "register_fail";
//char* BasicConfigData::REG_FOR_CHECK = "register_for_check";
//char* BasicConfigData::REG_KEY = "register_result";
//char* BasicConfigData::REG_SUCCESS = "register_ok";

double BasicConfigData::AblationColor[3][3] = { {0, 0.81, 1}, {0.96, 0.83, 0},  {1, 0.33, 0.05} };

bool BasicConfigData::AutoTest = false;
int BasicConfigData::FirstStep = 0;
QString BasicConfigData::TestDataFold = "E:/aa/cangHuiDong";

QString BasicConfigData::VarFileName = "/Config/variable.xml";

QStringList BasicConfigData::FiberTypeList = { "FD04C01A", "FD04C01B", "FD10C01A", "FD10C01B", "FD15C01A", "FD15C01B"};
QMap<QString, QString> BasicConfigData::AlarmInformationMap = BasicConfigData::AddAlarmInformation();

GLCommonXML *BasicConfigData::TestCaseXML = NULL;

BasicConfigData::BasicConfigData()
{
	LoadXML();
}

BasicConfigData::~BasicConfigData()
{
}

QMap<QString, QString> BasicConfigData::AddAlarmInformation()
{
	QMap<QString, QString> map;
	map.insert("alarm01", QStringLiteral("指示灯按钮未接入！"));
	map.insert("alarm02", QStringLiteral("水路按钮未接入！"));
	return map;
}

void BasicConfigData::LoadXML()
{
}