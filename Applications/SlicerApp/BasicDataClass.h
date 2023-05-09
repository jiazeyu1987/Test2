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
	INFORMATION = 0,        // ��ʾ��Ϣ;
	WARNNING,               // ��ʾ����;
	QUESTION,               // ��ʾѯ��;
	WITHOUTICON,
	//INPUT                   // ��ʾ�����;
};

enum MessageButtonType
{
	BUTTON_OK = 0,                  // ֻ��ȷ����ť;
	BUTTON_OK_AND_CANCEL,           // ȷ����ȡ����ť;
	BUTTON_OK_AND_ACCEPT,		//ȷ��, ���ܺ�ȡ����ť
	BUTTON_CLOSE                    // �رհ�ť;
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

//�����ͺ�
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
	static bool AutoTest;              //�Ƿ�������ģʽ
	static int FirstStep;
	static QMap<QString, QString> AlarmInformationMap;
	static QString TestDataFold;
	static QStringList FiberTypeList; //�����ͺ�

	static char* patientNameAgeAdvice; //���浽DataStorage�л��ߵ���Ϣ��DataNode�����ƣ��ڵ����������patientName��age��advice
	static char* patientName; //����
	static char* age; //����
	static char* advice; //ҽ��
	
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