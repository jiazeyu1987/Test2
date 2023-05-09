#include "BasicDataClass.h"
#define LOGIN_PERSPECTIVE ""


char* BasicConfigData::patientNameAgeAdvice = "patientNameAgeAdvice"; //���浽GetDataStorage()�л��ߵ���Ϣ��DataNode�����ƣ��ڵ����������patientName��age��advice
char* BasicConfigData::patientName = "patientName"; //����
char* BasicConfigData::age = "age"; //����
char* BasicConfigData::advice = "advice"; //ҽ��QString BasicConfigData::VarFileName = "/Config/variable.xml";

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
	map.insert("alarm01", QStringLiteral("ָʾ�ư�ťδ���룡"));
	map.insert("alarm02", QStringLiteral("ˮ·��ťδ���룡"));
	return map;
}

void BasicConfigData::LoadXML()
{
}