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
     * 目标：创建数据库连接
     */
    int createDbConnect();
    /*
     * 目标：保存医生表
     */
    int setDoctors(Doctors& doctor);
    //设置账户表
    int setAccount(Account& account);
    /*
     * 目标：用来保存注册的用户名和密码等
     * 返回值：返回更新或插入数据库的行数
     */
    int reg(Doctors& doctors, Account& account);
    //暂时不用
    int reg(Account1& account);
	/*
	 * 目标：用来保存患者基本信息
	 * 返回值：返回更新或插入数据库的行数
	 */
	int setPatients(Patient& patient);
    //设置合并标志
    int setPatientMergeSplitSign(QStringList patientIdList, QString mergeSign);
	/*
	 * 目标：保存从DICOM中导入的一些数据
	 * 
	 */
	int setPatientsSeries(PatientsSeries series); //患者序列-存储
	/*
	 * 目标：用来保存医生对患者的病情分析概况
	 * 返回值：返回更新或插入数据库的行数
	 */
	int setPatientsAnalyse(PatientsAnalyse& patientsAnalyse);
    //更新患者分析表的记录的更改时间
    int setPatientAnalyseChangedTime(PatientsAnalyse& patientsAnalyse);
    // 设置患者分析报告
    int setPatientAnalyseReport(PatientsAnalyse& patientsAnalyse);
	/*
	 * 目标：用来保存患者的序列清单
	 * 返回值：返回更新或插入数据库的行数
	 */
	int setPatientsAnalyseSeries(PatientsAnalyseSeries patientsAnalyseSeries);
	//设置账户表isFirstUse标志位
	int setAccountIsFirstUse(Account& account);
	//设置密码
	int setPassword(Account account);

    int setSurgerySummary(Patient & patient);
    //设置患者分析Id患者Id表
    int setPatientAnalyseIdPatientIdId(PatientAnalyseIdPatientIdId patientAnalyseIdPatientIdId);
    //根据患者分析Id获取患者Id
    QVector<Patient> getPatientIdFromPatientAnalyseIdPatientIdId(PatientsAnalyse patientsAnalyse);
    //根据患者id获取患者分析id
    QVector<int> getPatientAnalyseIdFromPatientAnalyseIdPatientIdId(Patient patient);
//public slots: //自定义类型不能作为参数，需要注册qRegisterMetaType/Q_DECLARE_METATYPE
	//设置软件操作记录
	int setSoftwareOperationRecord(SoftwareOperationRecord& record);
   //设置光纤水路连接记录
   int setFiberWaterConnectRecord(FiberWaterConnectRecord& record);
  /*
   * 目标：用来获取患者基本信息
   */
   QVector<Patient> getPatients(Account account);
   //获取指定id的患者信息
   Patient getPatients(int patientId);
	/*
	 * 目标：获取患者id
	 */
    int getPatientId(Patient patient);
    /*
     * 目标：获取新建n个新建分析，按sort排序
     * 参数：n表示获取的行数，sort表示按创建时间排序（2从大到小，1从小到大，0不排序）, patientId(-1表示任一)
     */
    QVector<PatientsAnalyse> getPatientsAnalyse(int n = 5, int sort = 2);
    //根据患者分析id获取患者分析
    PatientsAnalyse getPatientsAnalyse1(int patientAnalyseId);
    //获取患者序列
    QVector<PatientsSeries> GLDb::getPatientsSeries(Patient patient);
    //获取患者序列，根据患者序列id
    QVector<PatientsSeries> GLDb::getPatientsSeries(QVector<PatientsAnalyseSeries> patientsAnalyseSeriesVec);
    void slot_init();

    /*
     * 目标：验证用户名和密码
     */
    enum LoginStatus login(Account& account);
    //验证是否存在用户名
    int existUsername(Account& account);

    //-----------------------设置enable-----------------------
    bool setPatientAnalyseEnable(PatientsAnalyse analyse);
    //获取患者分析序列
    QVector<PatientsAnalyseSeries> getPatientAnalyseSeries(PatientsAnalyse analyse);

private:
	//--------------------------------------------------------
	QString calMd5softwareoperationrecord(int rowId, bool updateToDb = true);
	QString calMd5accounts(int rowId, bool updateToDb = true);
    QString calMd5accounts1(int rowId, bool updateToDb = true); //用户第2版（用户表和医生表合并）
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
     * 目标：计算md5的值
     * 方法：将所有列的值按照列定义的顺序首尾拼接，中间用pkay隔开，bool值true转换为oka，false 转 koa，前后加上前缀jui和后缀lqp，然后对此行所有计算md5的值
     * 需要对字段按照从小到大排序
     */
    QString md5(QStringList strList);
    //-------------------------------------------------------
    //设置Md5
    int setMd5(QString tableName, QString md5, int id);
    /*
     * 目标：计算两个日期的年龄
     * 方法：年龄和月份相减，如果月份不够，则年龄少1，即只有足额的才认为是1岁
     */
    quint8 calAge(QString birthday);
    void messageBoxSet(bool exist, int num, QString errorString);

private slots:
	/*
	 * 目标：心跳设置，mysql默认是8小时(wait_timeout或interactive_timeout，取决于client_interactive标志）的客户端，MySQL会主动断开连接。)
	 * 方法：1分钟发一次SELECT 1;
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