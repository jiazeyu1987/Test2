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

typedef struct Ablation_ //������Ϣ
{
    QString time; //ʱ��
    QString power; //����
    QString waveLen; //����
    QString energy; //������
} Ablation;

typedef struct Backway_
{
    QString name; //����
    QString distance; //����
} Backway;

typedef struct Optical_//������Ϣ
{
    QString name; //����
    QString model; //�ͺ�
    QString targetOffset; //�е�ƫ������Ԥ���ƶ����룩-
    QString ablationCount; //���ڴ�������ͨ��������Ϣ��������㣨���������
    QString headstockType; //ͷ������
    QVector<Ablation> ablationVec; //������Ϣ-
    QString X;
    QString Y;
    QString Z;
    QString ARC;
    QString RAN;
    QString remark;
    QVector<Backway> backwayVec;  //���˾���
} Optical;

typedef struct FocalZone_//������
{
    QString focusName; //��������
    QString diagnose; //���
    QString ablationVolume; //�������
    QString opticalNumber; //����ʹ������
    QString remark; //��ע
} FocalZone;

typedef struct SoftwareInfo_
{
  QString versionRelase; //���а汾��
  QString versionComplete; //�����汾��
  QString installationDateTime; //��װʱ��
  QString registerName = QStringLiteral("���ݼ���ҽ�ƿƼ����޹�˾"); //ע��������
  QString address = QStringLiteral("�㽭ʡ�������ຼ����ǰ�ֵ����˽�1390��9��B��801��"); //��ַ
  QString contact = "(86)0571-88730192"; //��ϵ��ʽ
  QString copyright = QStringLiteral("Copyright &#169; 2021���ݼ���ҽ�ƿƼ����޹�˾ All Rights Reserved");
  QString licenseKey; //���֤��
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
  //����������-�ʺ�-����-��ͷ
  QWidget *topToolBar(int x, int y, int width, int height, bool autotest, QWidget *parent = nullptr);
  //��ȡע�����ȡ�汾����װʱ���
  SoftwareInfo getSoftwareInfo();
  //�˳�Ӧ�ó���
  void Exit(QString cause, bool autotest);
  void ShowChangePassword();

  /// <summary>
  /// 
  /// </summary>
  static QString curDir; //��ǰĿ¼
  static QString imagesDir; //ͼƬλ��
  static QString patientDataDir; //��������
  static QString fiberDir; //����

  static bool odbc; //���ݿ����ӷ�ʽ

  static int calAge(QString date); //��������,Date��ʽ������2021/05/21����yyyy/MM/dd

  static QToolButton *exitBtn; //�˳���ť
  static QToolButton *saveBtn; //����-����
  static QToolButton *exitBtn1; //�˳�������ť

  static QFileInfoList fileInfoList;
  static QFileInfoList getFileInfoFromDir(QString dir);

    //---------------------pdf����-----------------------------
   //�������ּ�¼������������¼�ļ�.pdf�����ٰ��������ڡ������ˡ����ߣ����� + ���գ����������˶�Ӧ������ + �ͺ� + ���ڴ��� + ÿ�ε�ʱ�� + ���� + �����������н�ͼ��

  //��������  
  static QString datetime; //���ڣ�����ʱ�䣩
  static QString worker; //�����ˣ������ˣ�
  static QString receiveTime; //����ʱ��
  static QString verifier; //�����
  static QString doctor; //ҽ��
  static QString hospital; //ҽԺ
  static QString address; //���ڵ�
  static QString auditTime; //���ʱ��

  //������Ϣ
  static QString patientName; //��������
  static QString patientBirthday; //��������
  static QString patientAge; //��������
  static QString height; //���
  static QString weight; //����
  static QString patientSex; //�Ա�

  static QString anamnesis; //������ʷ
  static QString patientRemark; //���߱�ע

  //������
  static QVector<FocalZone> focalZoneVec;

  static QString patientAdvice; //ҽ���������߱�ע
  static QString patientAnalyName; //��������

  //������Ϣ
  static QVector<Optical> opticalVec; //������Ϣ

  static QStringList capture; //��ͼ·��

  static QString operationRecord; //�������ں���-������¼

  static int patientAnalyseId; //���������ķ���id

signals:
	void sig_log(QString txt);
/// <summary>
/// 
/// </summary>
private:
  //���ݿⱸ��
  void databackup();

  QToolButton* createBtn(QString icon, QString label, QSize size);
  void createHelpMenu();
  void createSetMenu();

private:
  static QWidget *toolBar;
  static QToolButton *setUpBtn; //����-����
  static QToolButton *helpBtn; //����-�ʺ�

  QProcess myProcess; // ���ݿⱸ�ݽ���
  GLMessageBox msg; 
};