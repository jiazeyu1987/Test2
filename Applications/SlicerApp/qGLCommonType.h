#pragma once
#include <qvector.h>
#define ADMINISTRATOR_USERNAME "administrator"
#define ADMINISTRATOR_PASSWORD "1234567890"

//ҽ����
typedef struct Doctors
{
	int doctorId = -1; //ҽ��id
	QString doctorName; //ҽ������
    QString sex; //�Ա�
    QString mobile; //�ֻ�
	QString hostpital; //ҽԺ��ַ
	QString department; //ҽ�����ڲ���
	bool enableColumn = true; //���Ƿ���Ч
	QString createTime; //���ݴ���ʱ��
	QString changedTime; //�������¸ı�ʱ��
	QString remark = "";
	QString md5 = QString::null; //MD5������Ϣ
} Doctors, *p_struct_doctors;

//�˺ű�
typedef struct Account
{
	int accountId = -1; //�˺�id
	QString userName; //�û���
	QString _password; //����
	int isFirstUse = -1; //1��ʾ����ʹ�ã�2��ʾ���α��޸ģ�3��ʾ����δ�޸�
	int doctorId; //ҽ��Id
	QString doctorName; //ҽ������
	bool enableColumn = true; //���Ƿ���Ч
	QString createTime; //���ݴ���ʱ��
	QString changedTime; //�������¸ı�ʱ��
	QString remark = "";
	QString md5 = QString::null; //MD5������Ϣ
} Account, *p_struct_account;

//�˺ű�
typedef struct Account1
{
    int accountId = -1; //�˺�id
    QString userName; //�û���
    QString _password; //����
    int isFirstUse = -1; //1��ʾ����ʹ�ã�2��ʾ���α��޸ģ�3��ʾ����δ�޸�
    QString doctorName; //ҽ������
    QString sex; //�Ա�
    QString mobile; //�ֻ�
    QString hostpital; //ҽԺ��ַ
    QString department; //ҽ�����ڲ���
    bool enableColumn = true; //���Ƿ���Ч
    QString createTime; //���ݴ���ʱ��
    QString changedTime; //�������¸ı�ʱ��
    QString remark = "";
    QString md5 = QString::null; //MD5������Ϣ
} Account1, *p_struct_account1;

//���߻�����Ϣ��
typedef struct Patient
{
	int patientId = -1; //����id
    QString dicomPatientId; //dicom�ļ��д洢�Ļ���id����Ϊ�ж�ȷ������Ψһ�Եı�־
    int accountId = -1; //�����������˻�����ͬ���˻������߲��ܹ���
	QString patientName; //��������
	QString birthday; //��������
    QString age; //ע�⣺���������ݿ���У����Ͻ���Ϊ�˷����ʾ
	QString sex; //�����Ա�
    QString height; //���
    QString weight; //����
    QString surgerSummary; //������Ҫ
    QString recordModificationTime; //��¼�޸�ʱ��
    QStringList reportList; //��������·�������ݿ���û�д��У����ϴ�����Ϊ��Ӧ�ó��������ʹ��
    QString projectPath; //��Ŀ·��
    QString logPath; //��־·��
    QString mergeSign; //�ϲ���־���ɻ���id��ɣ��м��ö��Ÿ���
	bool enableColumn = true; //���Ƿ���Ч
	QString createTime; //���ݴ���ʱ��
	QString changedTime; //�������¸ı�ʱ��
	QString remark; //��ע
	QString md5 = QString::null; //MD5������Ϣ
} Patient, *p_struct_patient;

//�������б�--���Ǹû���һ���ܵı�
typedef struct PatientsSeries
{
	int patientsSeriesId = -1; //��������id
	int patientId = -1; //����id
    QString dicomPatientId; //dicom�洢�Ļ���id
    QString accessionNumber; //���ţ�study��أ�
    QString dicomPatientName; //dicom�洢�Ļ�������
    QString dicomBirthday; //dicom�洢�Ļ�������
    QString dicomSex; //dicom�洢�Ļ����Ա�
    QString dicomHeight; //dicom�洢�Ļ������
    QString dicomWeight; //dicom�洢�Ļ�������
    QString seriesInstanceUID = QString(); //����ʵ��Ψһʶ��ţ�QString()�൱��QString::Null���ࣩ����QString("")��ͬ�����ǵ�seriesInstanceUID������Ϊ�գ�����ֵ������ɲ�������ʧ��
	int seriesNumber = -1; //���к�
	QString scanTime; //ɨ��ʱ��
	QString seriesDescribe; //��������
	QString modality; //CT PT MR��
    QString pixelSpacing; //���ؼ��
    QString thickness; //���
    QString spaceBetween; //�������
	int sliceCount = -1; //��Ƭ��
    QString seriesDir; //�����������ļ��е�·��
    QString screenshot; //����ͼ·��
    bool enableSeries = true; //�Ƿ��������
	bool enableColumn = true; //���Ƿ���Ч
	QString createTime; //���ݴ���ʱ��
	QString changedTime; //�������¸ı�ʱ��
	QString remark = ""; //��ע
	QString md5 = QString::null; //MD5������Ϣ
} PatientsSeries, *pPatientsSeries;

//���߷�����
typedef struct PatientsAnalyse
{
	int patientAnalyseId = -1; //���߷���id
    int accountId = -1; //�����������˻�����ͬ���˻������߲��ܹ���
	QString analyseName; //��������
	QString illnessKind; //��������
	QString doctorRemark; //���߱�ע
    QString patientName; //��������
    QString birthday; //��������
    QString age; //ע�⣺���������ݿ���У����Ͻ���Ϊ�˷����ʾ
    QString sex; //�����Ա�
    QString height; //���
    QString weight; //����
    QString reportPath; //����·��
	bool enableColumn = true; //���Ƿ���Ч
	QString createTime; //���ݴ���ʱ��
	QString changedTime; //�������¸ı�ʱ��
	QString remark = ""; //��ע
	QString md5 = QString::null; //MD5������Ϣ
    bool is_package = false;
} PatientsAnalyse, *p_struct_patientsAnalyse;

//���߷�����Id����Id��
typedef struct PatientAnalyseIdPatientIdId
{
    int patientAnalyseIdPatientIdId = -1; //���߷�����Id����Id
    int patientAnalyseId = -1; //���߷���id
    int patientId = -1; //����id
    bool enableColumn = true; //���Ƿ���Ч
    QString createTime; //���ݴ���ʱ��
    QString changedTime; //�������¸ı�ʱ��
    QString remark = ""; //��ע
    QString md5 = QString::null; //MD5������Ϣ
} PatientAnalyseIdPatientIdId;

//���߷������б�
typedef struct PatientsAnalyseSeries
{
	int patientAnalyseSeriesId = -1; //��������id
	int patientAnalyseId = -1; //���߷���id
    int patientsSeriesId = -1; //��������id
	bool enableSequence = true; //�Ƿ�Ĭ�ϼ�������
	bool enableColumn = true; //���Ƿ���Ч
	QString createTime; //���ݴ���ʱ��
	QString changedTime; //�������¸ı�ʱ��
	QString remark = ""; //��ע
	QString md5 = QString::null; //MD5������Ϣ
} PatientsAnalyseSeries, *pPatientsAnalyseSeries;

//����������¼��
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

//����ˮ·���Ӽ�¼��
typedef struct FiberWaterConnectRecord
{
  int fiberWaterConnectRecordId = -1;
  QString content; //���Ӽ�¼: �������ӣ������Ͽ����쳣�Ͽ���
  QString	recordTime; //��¼ʱ��: yyyy/mm/dd hh:mm:ss.zzz
  QString	device; // �豸��enum('Fiber', 'Water')
  int	accountId = -1;
  QString	userName = "";
  bool enableColumn = true;
  QString	createTime;
  QString	changedTime;
  QString	remark;
  QString	md5 = QString::null;
} FiberWaterConnectRecord, *p_struct_FiberWaterConnectRecord;

enum LoginStatus //��¼״̬
{
	VerificationSuccess, //��֤�ɹ�
	USBKeySuccess, //USB����ɹ�
	USBKeyNone, //û�м�⵽USB��Կ
	USBKeyVerificationFailed, //USB��Կ��֤��ͨ��
	SoftwareUserAgreementUnCheck, //����û�Э��δѡ��
	UsernameEmpty, //�û���Ϊ��
	UsernameFormatError, //�û�����ʽ������
	UsernameError, //�û�������
	DataTampered, //���ݱ��۸�
	PasswordEmpty, //����Ϊ��
    PasswordFormatError, //�����ʽ������
	PasswordError, //�������
	Undefined, //δ����
    AdministratorVerificationSuccess, //����Ա��¼�ɹ�
};

//���ڵ�dicom��patientId��ͬʱ�����������ֶγ�ͻʱ��ҽ���ֶ�ѡ����������ͻ
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

//����ԭʼ���ݣ������ԭʼ�������뻼����ص�һЩ���ݣ���������ָ����е���
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
    QString mergeSign; //�ϲ���־
    QVector<PatientsAnalyse> patientAnalyseVec;
} RowData;