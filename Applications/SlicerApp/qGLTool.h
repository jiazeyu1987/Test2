#pragma once

#include <qobject.h>
#include "qGLCommonType.h"
#include <qtoolbutton.h>
#include <qprocess.h>
#include "qGLMessageBox.h"
#include <qfileinfo.h>
#include "qSlicerProgressBar.h"

enum PathType
{
  File, Dir
};

typedef struct Ablation_ //消融信息
{
    QString time; //时长
    QString power; //功率
    QString waveLen; //波长
    QString energy; //总能量
} Ablation;

typedef struct Backway_
{
    QString name; //名称
    QString distance; //距离
} Backway;

typedef struct Optical_//光纤信息
{
    QString name; //名称
    QString model; //型号
    QString targetOffset; //靶点偏移量（预计移动距离）-
    QString ablationCount; //消融次数或者通过消融信息的数组计算（出光次数）
    QString headstockType; //头架类型
    QVector<Ablation> ablationVec; //消融信息-
    QString X;
    QString Y;
    QString Z;
    QString ARC;
    QString RAN;
    QString remark;
    QVector<Backway> backwayVec;  //后退距离
} Optical;

typedef struct FocalZone_//病灶区
{
    QString focusName; //病灶名称
    QString diagnose; //诊断
    QString ablationVolume; //消融体积
    QString opticalNumber; //光纤使用数量
    QString remark; //备注
} FocalZone;

typedef struct SoftwareInfo_
{
  QString versionRelase; //发行版本号
  QString versionComplete; //完整版本号
  QString installationDateTime; //安装时间
  QString registerName = QStringLiteral("杭州佳量医疗科技有限公司"); //注册人名称
  QString address = QStringLiteral("浙江省杭州市余杭区仓前街道仓兴街1390号9幢B座801室"); //地址
  QString contact = "(86)0571-88730192"; //联系方式
  QString copyright = QStringLiteral("Copyright &#169; 2021杭州佳量医疗科技有限公司 All Rights Reserved");
  QString licenseKey; //许可证号
} SoftwareInfo;

class Q_SLICER_APP_EXPORT GLTool : public QObject
{
  Q_OBJECT
public:
  GLTool() {};
  ~GLTool() {};

  bool createPath(QString path, ::PathType type);
  void setPatientName(QString patientName);
  void setSoftwareoperationrecord(SoftwareOperationRecord& record);
  //顶部工具条-问号-齿轮-箭头
  QWidget *topToolBar(int x, int y, int width, int height, bool autotest, QWidget *parent = nullptr);
  //读取注册表，获取版本，安装时间等
  SoftwareInfo getSoftwareInfo();
  //退出应用程序
  void Exit(QString cause, bool autotest);
  void ShowChangePassword();

  /// <summary>
  /// 
  /// </summary>
  static QString curDir; //当前目录
  static QString imagesDir; //图片位置
  static QString patientDataDir; //患者数据
  static QString fiberDir; //光纤

  static bool odbc; //数据库连接方式

  static int calAge(QString date); //计算年龄,Date格式必须是2021/05/21，即yyyy/MM/dd

  static QToolButton *exitBtn; //退出按钮
  static QToolButton *saveBtn; //设置-齿轮
  static QToolButton *exitBtn1; //退出分析按钮

  static QFileInfoList fileInfoList;
  static QFileInfoList getFileInfoFromDir(QString dir);

    //---------------------pdf内容-----------------------------
   //术中文字记录，保存手术记录文件.pdf（至少包括，日期、操作人、患者（姓名 + 生日）、单根光纤对应的名称 + 型号 + 消融次数 + 每次的时长 + 功率 + 总能量、术中截图）

  //报告详情  
  static QString datetime; //日期（出具时间）
  static QString worker; //操作人（出具人）
  static QString receiveTime; //接收时间
  static QString verifier; //审核人
  static QString doctor; //医生
  static QString hospital; //医院
  static QString address; //所在地
  static QString auditTime; //审核时间

  //患者信息
  static QString patientName; //患者姓名
  static QString patientBirthday; //患者生日
  static QString patientAge; //患者年龄
  static QString height; //身高
  static QString weight; //体重
  static QString patientSex; //性别

  static QString anamnesis; //既往病史
  static QString patientRemark; //患者备注

  //病灶区
  static QVector<FocalZone> focalZoneVec;

  static QString patientAdvice; //医嘱，即患者备注
  static QString patientAnalyName; //分析名称

  //光纤信息
  static QVector<Optical> opticalVec; //光纤信息

  static QStringList capture; //截图路径

  static QString operationRecord; //来自消融后处理-手术记录

  static int patientAnalyseId; //报告所属的分析id

signals:
	void sig_log(QString txt);
/// <summary>
/// 
/// </summary>
private:
  //数据库备份
  void databackup();

  QToolButton* createBtn(QString icon, QString label, QSize size);
  void createHelpMenu();
  void createSetMenu();

private:
  static QWidget *toolBar;
  static QToolButton *setUpBtn; //设置-齿轮
  static QToolButton *helpBtn; //帮助-问号

  QProcess myProcess; // 数据库备份进程
  GLMessageBox msg; 
};