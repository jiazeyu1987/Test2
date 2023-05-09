#ifndef GLDB_H
#define GLDB_H

#include <qsqldatabase.h>
#include <qobject.h>
#include "qGLCommonType.h"
#include <qtimer.h>
#include <qhash.h>
#include "qGLMessageBox.h"
#include "qSlicerAppExport.h"

class Q_SLICER_APP_EXPORT GLDb : public QObject
{
    Q_OBJECT
public:
    explicit GLDb(QObject *parent = nullptr);
    ~GLDb();

    /*
     * Ŀ�꣺�������ݿ�����
     */
    int createDbConnect();
    /*
     * Ŀ�꣺����ҽ����
     */
    int setDoctors(Doctors& doctor);
    //�����˻���
    int setAccount(Account& account);
    /*
     * Ŀ�꣺��������ע����û����������
     * ����ֵ�����ظ��»�������ݿ������
     */
    int reg(Doctors& doctors, Account& account);
    //��ʱ����
    int reg(Account1& account);
	/*
	 * Ŀ�꣺�������滼�߻�����Ϣ
	 * ����ֵ�����ظ��»�������ݿ������
	 */
	int setPatients(Patient& patient);
    //���úϲ���־
    int setPatientMergeSplitSign(QStringList patientIdList, QString mergeSign);
	/*
	 * Ŀ�꣺�����DICOM�е����һЩ����
	 * 
	 */
	int setPatientsSeries(PatientsSeries series); //��������-�洢
	/*
	 * Ŀ�꣺��������ҽ���Ի��ߵĲ�������ſ�
	 * ����ֵ�����ظ��»�������ݿ������
	 */
	int setPatientsAnalyse(PatientsAnalyse& patientsAnalyse);
    //���»��߷�����ļ�¼�ĸ���ʱ��
    int setPatientAnalyseChangedTime(PatientsAnalyse& patientsAnalyse);
    // ���û��߷�������
    int setPatientAnalyseReport(PatientsAnalyse& patientsAnalyse);
	/*
	 * Ŀ�꣺�������滼�ߵ������嵥
	 * ����ֵ�����ظ��»�������ݿ������
	 */
	int setPatientsAnalyseSeries(PatientsAnalyseSeries patientsAnalyseSeries);
	//�����˻���isFirstUse��־λ
	int setAccountIsFirstUse(Account& account);
	//��������
	int setPassword(Account account);

    int setSurgerySummary(Patient & patient);
    //���û��߷���Id����Id��
    int setPatientAnalyseIdPatientIdId(PatientAnalyseIdPatientIdId patientAnalyseIdPatientIdId);
    //���ݻ��߷���Id��ȡ����Id
    QVector<Patient> getPatientIdFromPatientAnalyseIdPatientIdId(PatientsAnalyse patientsAnalyse);
    //���ݻ���id��ȡ���߷���id
    QVector<int> getPatientAnalyseIdFromPatientAnalyseIdPatientIdId(Patient patient);
//public slots: //�Զ������Ͳ�����Ϊ��������Ҫע��qRegisterMetaType/Q_DECLARE_METATYPE
	//�������������¼
	int setSoftwareOperationRecord(SoftwareOperationRecord& record);
   //���ù���ˮ·���Ӽ�¼
   int setFiberWaterConnectRecord(FiberWaterConnectRecord& record);
  /*
   * Ŀ�꣺������ȡ���߻�����Ϣ
   */
   QVector<Patient> getPatients(Account account);
   //��ȡָ��id�Ļ�����Ϣ
   Patient getPatients(int patientId);
	/*
	 * Ŀ�꣺��ȡ����id
	 */
    int getPatientId(Patient patient);
    /*
     * Ŀ�꣺��ȡ�½�n���½���������sort����
     * ������n��ʾ��ȡ��������sort��ʾ������ʱ������2�Ӵ�С��1��С����0������, patientId(-1��ʾ��һ)
     */
    QVector<PatientsAnalyse> getPatientsAnalyse(int n = 5, int sort = 2);
    //���ݻ��߷���id��ȡ���߷���
    PatientsAnalyse getPatientsAnalyse1(int patientAnalyseId);
    //��ȡ��������
    QVector<PatientsSeries> GLDb::getPatientsSeries(Patient patient);
    //��ȡ�������У����ݻ�������id
    QVector<PatientsSeries> GLDb::getPatientsSeries(QVector<PatientsAnalyseSeries> patientsAnalyseSeriesVec);
    void slot_init();

    /*
     * Ŀ�꣺��֤�û���������
     */
    enum LoginStatus login(Account& account);
    //��֤�Ƿ�����û���
    int existUsername(Account& account);

    //-----------------------����enable-----------------------
    bool setPatientAnalyseEnable(PatientsAnalyse analyse);
    //��ȡ���߷�������
    QVector<PatientsAnalyseSeries> getPatientAnalyseSeries(PatientsAnalyse analyse);

private:
	//--------------------------------------------------------
	QString calMd5softwareoperationrecord(int rowId, bool updateToDb = true);
	QString calMd5accounts(int rowId, bool updateToDb = true);
    QString calMd5accounts1(int rowId, bool updateToDb = true); //�û���2�棨�û����ҽ����ϲ���
	QString calMd5patients(int rowId, bool updateToDb = true);
	QString calMd5patientsoperation(int rowId, bool updateToDb = true);
	QString calMd5PatientsSeries(int rowId, bool updateToDb = true);
	QString calMd5patientsanalyse(int rowId, bool updateToDb = true);
    QString calMd5patientsAnalyseIdPatientId(int rowId, bool updateToDb = true);
	QString calMd5patientsanalysesequencelist(int rowId, bool updateToDb = true);
	QString calMd5patientsimagesequence(int rowId, bool updateToDb = true);
	QString calMd5doctors(int rowId, bool updateToDb = true);
    QString calMd5fiberWaterConnectRecord(int rowId, bool updateToDb = true);
    /*
     * Ŀ�꣺����md5��ֵ
     * �������������е�ֵ�����ж����˳����βƴ�ӣ��м���pkay������boolֵtrueת��Ϊoka��false ת koa��ǰ�����ǰ׺jui�ͺ�׺lqp��Ȼ��Դ������м���md5��ֵ
     * ��Ҫ���ֶΰ��մ�С��������
     */
    QString md5(QStringList strList);
    //-------------------------------------------------------
    //����Md5
    int setMd5(QString tableName, QString md5, int id);
    /*
     * Ŀ�꣺�����������ڵ�����
     * ������������·����������·ݲ�������������1����ֻ�����Ĳ���Ϊ��1��
     */
    quint8 calAge(QString birthday);
    void messageBoxSet(bool exist, int num, QString errorString);

private slots:
	/*
	 * Ŀ�꣺�������ã�mysqlĬ����8Сʱ(wait_timeout��interactive_timeout��ȡ����client_interactive��־���Ŀͻ��ˣ�MySQL�������Ͽ����ӡ�)
	 * ������1���ӷ�һ��SELECT 1;
	 */
	void slot_heartbeat();
signals:
    void sig_log(QString txt);
public:
  static Account glAccount;

private:
	QSqlDatabase db;
	QString host = "127.0.0.1";
	QString username = "testuser";
	QString password = "test";
	QString dbname = "db_jl"; //

	QTimer timer;
	int heartbeatTime = 1000 * 60;
	QHash<QString, QString> hashTableToPrimaryId;
};

#endif // !GLDB_H