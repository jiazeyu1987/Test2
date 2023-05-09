#pragma once
#include <qvector.h>
#define ADMINISTRATOR_USERNAME "administrator"
#define ADMINISTRATOR_PASSWORD "1234567890"

//医生表
typedef struct Doctors
{
	int doctorId = -1; //医生id
	QString doctorName; //医生姓名
    QString sex; //性别
    QString mobile; //手机
	QString hostpital; //医院地址
	QString department; //医生所在部门
	bool enableColumn = true; //列是否有效
	QString createTime; //数据创建时间
	QString changedTime; //数据最新改变时间
	QString remark = "";
	QString md5 = QString::null; //MD5编码信息
} Doctors, *p_struct_doctors;

//账号表
typedef struct Account
{
	int accountId = -1; //账号id
	QString userName; //用户名
	QString _password; //密码
	int isFirstUse = -1; //1表示初次使用，2表示初次被修改，3表示初次未修改
	int doctorId; //医生Id
	QString doctorName; //医生姓名
	bool enableColumn = true; //列是否有效
	QString createTime; //数据创建时间
	QString changedTime; //数据最新改变时间
	QString remark = "";
	QString md5 = QString::null; //MD5编码信息
} Account, *p_struct_account;

//账号表
typedef struct Account1
{
    int accountId = -1; //账号id
    QString userName; //用户名
    QString _password; //密码
    int isFirstUse = -1; //1表示初次使用，2表示初次被修改，3表示初次未修改
    QString doctorName; //医生姓名
    QString sex; //性别
    QString mobile; //手机
    QString hostpital; //医院地址
    QString department; //医生所在部门
    bool enableColumn = true; //列是否有效
    QString createTime; //数据创建时间
    QString changedTime; //数据最新改变时间
    QString remark = "";
    QString md5 = QString::null; //MD5编码信息
} Account1, *p_struct_account1;

//患者基本信息表
typedef struct Patient
{
	int patientId = -1; //病人id
    QString dicomPatientId; //dicom文件中存储的患者id，作为判断确定患者唯一性的标志
    int accountId = -1; //病人所属的账户，不同的账户，患者不能共用
	QString patientName; //病人姓名
	QString birthday; //病人生日
    QString age; //注意：不属于数据库的列，加上仅是为了方便表示
	QString sex; //病人性别
    QString height; //身高
    QString weight; //体重
    QString surgerSummary; //手术纪要
    QString recordModificationTime; //记录修改时间
    QStringList reportList; //分析报告路径，数据库中没有此列，加上此列是为了应用程序更方便使用
    QString projectPath; //项目路径
    QString logPath; //日志路径
    QString mergeSign; //合并标志，由患者id组成，中间用逗号隔开
	bool enableColumn = true; //列是否有效
	QString createTime; //数据创建时间
	QString changedTime; //数据最新改变时间
	QString remark; //备注
	QString md5 = QString::null; //MD5编码信息
} Patient, *p_struct_patient;

//患者序列表--这是该患者一个总的表
typedef struct PatientsSeries
{
	int patientsSeriesId = -1; //患者序列id
	int patientId = -1; //病人id
    QString dicomPatientId; //dicom存储的患者id
    QString accessionNumber; //检查号（study相关）
    QString dicomPatientName; //dicom存储的患者姓名
    QString dicomBirthday; //dicom存储的患者生日
    QString dicomSex; //dicom存储的患者性别
    QString dicomHeight; //dicom存储的患者身高
    QString dicomWeight; //dicom存储的患者体重
    QString seriesInstanceUID = QString(); //序列实体唯一识别号，QString()相当于QString::Null（类），与QString("")不同，考虑到seriesInstanceUID不允许为空，赋此值，会造成插入或更新失败
	int seriesNumber = -1; //序列号
	QString scanTime; //扫描时间
	QString seriesDescribe; //序列描述
	QString modality; //CT PT MR等
    QString pixelSpacing; //像素间距
    QString thickness; //层厚
    QString spaceBetween; //层与层间距
	int sliceCount = -1; //切片数
    QString seriesDir; //该序列所在文件夹的路径
    QString screenshot; //缩略图路径
    bool enableSeries = true; //是否加载序列
	bool enableColumn = true; //列是否有效
	QString createTime; //数据创建时间
	QString changedTime; //数据最新改变时间
	QString remark = ""; //备注
	QString md5 = QString::null; //MD5编码信息
} PatientsSeries, *pPatientsSeries;

//患者分析表
typedef struct PatientsAnalyse
{
	int patientAnalyseId = -1; //患者分析id
    int accountId = -1; //病人所属的账户，不同的账户，患者不能共用
	QString analyseName; //分析名称
	QString illnessKind; //疾病种类
	QString doctorRemark; //患者备注
    QString patientName; //病人姓名
    QString birthday; //病人生日
    QString age; //注意：不属于数据库的列，加上仅是为了方便表示
    QString sex; //病人性别
    QString height; //身高
    QString weight; //体重
    QString reportPath; //报告路径
	bool enableColumn = true; //列是否有效
	QString createTime; //数据创建时间
	QString changedTime; //数据最新改变时间
	QString remark = ""; //备注
	QString md5 = QString::null; //MD5编码信息
    bool is_package = false;
} PatientsAnalyse, *p_struct_patientsAnalyse;

//患者分析表Id患者Id表
typedef struct PatientAnalyseIdPatientIdId
{
    int patientAnalyseIdPatientIdId = -1; //患者分析表Id患者Id
    int patientAnalyseId = -1; //患者分析id
    int patientId = -1; //病人id
    bool enableColumn = true; //列是否有效
    QString createTime; //数据创建时间
    QString changedTime; //数据最新改变时间
    QString remark = ""; //备注
    QString md5 = QString::null; //MD5编码信息
} PatientAnalyseIdPatientIdId;

//患者分析序列表
typedef struct PatientsAnalyseSeries
{
	int patientAnalyseSeriesId = -1; //患者序列id
	int patientAnalyseId = -1; //患者分析id
    int patientsSeriesId = -1; //患者序列id
	bool enableSequence = true; //是否默认加载序列
	bool enableColumn = true; //列是否有效
	QString createTime; //数据创建时间
	QString changedTime; //数据最新改变时间
	QString remark = ""; //备注
	QString md5 = QString::null; //MD5编码信息
} PatientsAnalyseSeries, *pPatientsAnalyseSeries;

//手术操作记录表
typedef struct SoftwareOperationRecord
{
	int softwareOperationRecordId = -1;
	QString content;
	QString	classes; // enum('_User', '_Data', '_General')
	QString	module; // enum('Login')
	int	accountId = -1;
	QString	userName = "";
	bool enableColumn = true;
	QString	createTime;
	QString	changedTime;
	QString	remark;
	QString	md5 = QString::null;
} SoftwareOperationRecord, *p_struct_OperationRecord;

//光纤水路连接记录表
typedef struct FiberWaterConnectRecord
{
  int fiberWaterConnectRecordId = -1;
  QString content; //连接记录: 正常连接，正常断开，异常断开等
  QString	recordTime; //记录时间: yyyy/mm/dd hh:mm:ss.zzz
  QString	device; // 设备：enum('Fiber', 'Water')
  int	accountId = -1;
  QString	userName = "";
  bool enableColumn = true;
  QString	createTime;
  QString	changedTime;
  QString	remark;
  QString	md5 = QString::null;
} FiberWaterConnectRecord, *p_struct_FiberWaterConnectRecord;

enum LoginStatus //登录状态
{
	VerificationSuccess, //验证成功
	USBKeySuccess, //USB插入成功
	USBKeyNone, //没有检测到USB密钥
	USBKeyVerificationFailed, //USB密钥验证不通过
	SoftwareUserAgreementUnCheck, //软件用户协议未选中
	UsernameEmpty, //用户名为空
	UsernameFormatError, //用户名格式不合理
	UsernameError, //用户名错误
	DataTampered, //数据被篡改
	PasswordEmpty, //密码为空
    PasswordFormatError, //密码格式不合理
	PasswordError, //密码错误
	Undefined, //未定义
    AdministratorVerificationSuccess, //管理员登录成功
};

//用于当dicom的patientId相同时，但是其他字段冲突时，医生手动选择，以消除冲突
typedef struct ComoboxData
{
    QString name;
    QString age;
    QString birthday;
    QString dicomPatientId;
    QString accessionNumber;
    QString sex;
} ComoboxData;

enum PatientEnum
{
    Report_ = 0, PatientName, DicomPatientId, PatientBirthday, PatientAge, PatientSex,
    RecordModificationTime, AccessionNumber, Modalities, SurgerSummary
};

//代表原始数据，这里的原始数据是与患者相关的一些数据，并不仅仅指表格中的列
typedef struct RowData
{
    QString patientId;
    QStringList report;
    QString patientName;
    QString dicomPatientId;
    QString birthday;
    QString age;
    QString sex;
    QString height;
    QString weight;
    QString modifyDateTime;
    QString accessionNumber;
    QString modalities;
    QString surgrySummary;
    QString mergeSign; //合并标志
    QVector<PatientsAnalyse> patientAnalyseVec;
} RowData;