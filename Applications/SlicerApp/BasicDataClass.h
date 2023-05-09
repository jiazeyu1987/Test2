#pragma once
#include <qobject.h>
#include <qapplication.h>
#include <QString>
//#include "org_gl_comm_Export.h"
#include "qSlicerAppExport.h"
#include "GLCommonXML.h"

#define REG_FAIL "register_fail"
#define REG_FOR_CHECK "register_for_check"
#define REG_KEY "register_result"
#define REG_SUCCESS "register_ok"

#define PATIENT_MANAGER_PERSPECTIVE "org.gl.zz_patientMange_perspective"
#define PICTURE_CHOOSE_PERSPECTIVE "org.gl.zz_pictureChoose_perspective"
#define PICTURE_CONFIG_PERSPECTIVE "org.gl.zz_pictureConfig_perspective"
#define PICTURE_SEGMENT_PERSPECTIVE "org.gl.naviperspective"
#define OUTPUT_PERSPECTIVE "org.gl.zz_outpureport_perspective"

enum AccountType {
	Administrator,
	User
};

enum AblationType
{
	Low,
	Middle,
	High
};

enum MessageResult
{
	CANCEL = 0,
	OK = 1,
	ACCEPT = 2
};

enum MessageType
{
	INFORMATION = 0,        // 提示信息;
	WARNNING,               // 提示警告;
	QUESTION,               // 提示询问;
	WITHOUTICON,
	//INPUT                   // 提示输入框;
};

enum MessageButtonType
{
	BUTTON_OK = 0,                  // 只有确定按钮;
	BUTTON_OK_AND_CANCEL,           // 确定、取消按钮;
	BUTTON_OK_AND_ACCEPT,		//确定, 接受和取消按钮
	BUTTON_CLOSE                    // 关闭按钮;
};

enum LaserType
{
	NoType = 0,
	Type980,
	Type1064
};

enum LaserMode
{
	NoneMode = 0,
	StandBy,
	Ready,
	Null,
	TestMode
};

//光纤型号
enum OpticalFiberType
{
	FD4C01A = 0, //4mm
	FD4C01B,
	FD10C01A,    //10mm
	FD10C01B,
	FD15C01A,    //15mm
	FD15C01B
};

enum MapperType
{
	AutoMap = 0,
	CustomMap
};

enum BeforeOperateStep
{
	DataImport = 0,
	DcmConfig,
	Register,
	BuildModel
};

class Q_SLICER_APP_EXPORT BasicConfigData
{

public:
	BasicConfigData();
	~BasicConfigData();

	static QString VarFileName;
	static bool AutoTest;              //是否开启测试模式
	static int FirstStep;
	static QMap<QString, QString> AlarmInformationMap;
	static QString TestDataFold;
	static QStringList FiberTypeList; //光纤型号

	static char* patientNameAgeAdvice; //保存到DataStorage中患者的信息的DataNode的名称，节点包括属性名patientName，age，advice
	static char* patientName; //姓名
	static char* age; //年龄
	static char* advice; //医嘱
	
	/*static char * REG_FOR_CHECK;
	static char * REG_KEY;
	static char * REG_SUCCESS;
	static char * REG_FAIL;*/

	static double AblationColor[3][3];
  static GLCommonXML *TestCaseXML;

	static QMap<QString, QString> AddAlarmInformation();

private:
	GLCommonXML *xml;

	void LoadXML();
};