#include "qGLTool.h"
#include <qfile.h>
#include <qdir.h>
#include <qcoreapplication.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qmenu.h>
#include <QDesktopWidget>
#include <qtextedit.h>
#include <qapplication.h>
#include <qtoolbutton.h>
#include <qsettings.h>
#include <QHBoxLayout>
#include "qGLDb.h"
#include "qGLChangePassword.h"
#include "qGLRegisterAccount.h"
#include <qdatetime.h>
#include <qtextcodec.h>
#include "qGLOperateRecord.h"
#include <qdesktopservices.h>
#include "qSlicerApplication.h"
#include "QMainWindow.h"
#include "qSlicerIOManager.h"
QString GLTool::curDir = QCoreApplication::applicationDirPath();
QString GLTool::patientName = QStringLiteral("lin");
QString GLTool::imagesDir = GLTool::curDir + "/images";
QString GLTool::patientDataDir = GLTool::curDir + "/patientData/" + GLTool::patientName;
QString GLTool::fiberDir = GLTool::curDir + "/fiber";

QWidget *GLTool::toolBar = nullptr;
QToolButton *GLTool::saveBtn = nullptr; //����
QToolButton *GLTool::setUpBtn = nullptr; //����-����
QToolButton *GLTool::helpBtn = nullptr; //����-�ʺ�
QToolButton *GLTool::exitBtn1 = nullptr; //�˳�����
QToolButton *GLTool::exitBtn = nullptr; //�˳���ť

//QProcess GLTool::myProcess;
//GLMessageBox GLTool::msg;
QFileInfoList GLTool::fileInfoList;
bool GLTool::odbc = false; //Ĭ�ϲ�ʹ��odbc��ʽ����

  //��������  
QString GLTool::datetime; //���ڣ�����ʱ�䣩
QString GLTool::worker; //�����ˣ������ˣ�
QString GLTool::receiveTime; //����ʱ��
QString GLTool::verifier; //�����
QString GLTool::doctor; //ҽ��
QString GLTool::hospital; //ҽԺ
QString GLTool::address; //���ڵ�
QString GLTool::auditTime; //���ʱ��
  //������Ϣ
//QString GLTool::patientName; //��������
QString GLTool::patientBirthday; //��������
QString GLTool::patientAge; //��������
QString GLTool::height; //���
QString GLTool::weight; //����
QString GLTool::patientSex; //�Ա�

QString GLTool::anamnesis; //������ʷ
QString GLTool::patientRemark; //���߱�ע
  //������
QVector<FocalZone> GLTool::focalZoneVec;

QString GLTool::patientAdvice; //ҽ���������߱�ע
QString GLTool::patientAnalyName; //��������

//������Ϣ
QVector<Optical> GLTool::opticalVec; //������Ϣ

QStringList GLTool::capture; //��ͼ·��

QString GLTool::operationRecord; //�������ں���-������¼

int GLTool::patientAnalyseId; //���������ķ���id



bool GLTool::createPath(QString path, ::PathType type)
{
	if (type == ::PathType::File)
	{
		//�ж�·���Ƿ����
		QFile file(path);
		if (file.exists())
			return true;

		int first = path.lastIndexOf("/");

		//����Ŀ¼
		QString dirpath = path.mid(0, first);
		QDir dir(dirpath);
		if (!dir.exists())
			dir.mkdir(dirpath);

		//�����ļ�
		if (dir.exists())
		{
			QString filename = path.mid(first + 1);
			if (file.open(QIODevice::WriteOnly))
			{
				file.close();
				return true;
			}
		}
		return false;
	}

	if (type == ::PathType::Dir)
	{
		return QDir().mkpath(path);
	}

	return false;
}

void GLTool::setPatientName(QString patientName)
{
	GLTool::patientName = patientName;
	GLTool::patientDataDir = GLTool::curDir + "/patientData/" + GLTool::patientName;
}

void GLTool::setSoftwareoperationrecord(SoftwareOperationRecord & record)
{

}


QToolButton * GLTool::createBtn(QString iconStr, QString label, QSize size)
{
	QString qss("QToolButton::menu-indicator{image: none;}; ");
	QToolButton* btn = new QToolButton;
	btn->setStyleSheet(qss);
	btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	QIcon icon;
	QString iconFmt = ":/abc/blue/%1";
	icon.addFile(iconFmt.arg(iconStr));
	btn->setIcon(icon);
	btn->setIconSize(size);
	btn->setText(label);
	return btn;
}

void GLTool::createHelpMenu()
{
	QMenu *menu = new QMenu(toolBar);
	helpBtn->setMenu(menu);
	helpBtn->setPopupMode(QToolButton::InstantPopup);
	//-----------��ϸ��Ϣ--------------
	menu->addAction(QStringLiteral("��ϸ��Ϣ"), [=]()
		{
			//emit this.sig_log("������-��ϸ��Ϣ");
			//���ݷŵ������ļ������ݿ����ע���
			SoftwareInfo info = getSoftwareInfo();
            QString c = "";
			QString txt = QStringLiteral
			(
				"������ƣ����嶨�������ƻ����<br/>"
				"�ͺŹ��PS101<br/>"
				"�����汾��%1<br/>" //1
				"�����汾��%2<br/>" //1.0.0
				"��װʱ�䣺%3<br/>"
				"ע�������ƣ�%4<br/>"
				"��ַ��%5<br/>"
				"��ϵ��ʽ��%6<br/>"
				"��Ȩ������%7"
				//"���֤�ţ�%5"
			).arg(info.versionRelase).arg(info.versionComplete).arg(info.installationDateTime)
				.arg(info.registerName).arg(info.address).arg(info.contact).arg(info.copyright);
			GLMessageBox msg;
			msg.queRenInfo(QStringLiteral("��ϸ��Ϣ"), txt);
			//GLMessageDialog::ShowInformationWithoutIcon(QStringLiteral("��ϸ��Ϣ"), txt);
		});
	//-----------�����ֲ�----------------------
	menu->addAction(QStringLiteral("�����ֲ�"), []()
		{
			//MITK_CUSTOM << "������-�����ֲ�";
			QString manual = curDir + "/configure/manual.pdf";
			QDesktopServices::openUrl(QUrl::fromLocalFile(manual));
		});
}

void GLTool::createSetMenu()
{
	QMenu *menu = new QMenu(toolBar);
	setUpBtn->setMenu(menu);
	setUpBtn->setPopupMode(QToolButton::InstantPopup);
	menu->addAction(QStringLiteral("�޸�����"), []()
	{
		//MITK_CUSTOM << "�޸�����";
		GLChangePassword *changPassword = new GLChangePassword;
		AccountType type = GLDb::glAccount.userName == ADMINISTRATOR_USERNAME ? AccountType::Administrator : AccountType::User;
		changPassword->SetAccountType(type);
		changPassword->show();
		changPassword->exec();
		if (changPassword) {
			delete changPassword;
			changPassword = 0;
		}
	});

	if (GLDb::glAccount.userName == ADMINISTRATOR_USERNAME)
	{
		menu->addAction(QStringLiteral("ע���û�"), []()
		{
			//MITK_CUSTOM << "ע���û�";
			GLRegisterAccount *uiRegister = new GLRegisterAccount();
			uiRegister->show();
			uiRegister->exec();
			if (uiRegister) {
				delete uiRegister;
				uiRegister = 0;
			}
		});
	}
}

QWidget * GLTool::topToolBar(int x, int y, int width, int height, bool autotest, QWidget *parent)
{
	if (!toolBar)
	{
		toolBar = new QWidget();
	}

	toolBar->setStyleSheet(QString::fromUtf8(""));
	toolBar->setFixedSize(QSize(680, 66));
	QString qss
	(
		"QToolButton::menu-indicator{image: none;}; "
	);
	toolBar->setStyleSheet("background-color: transparent;");
	if (!saveBtn) {
		saveBtn = createBtn("topmenu_save.png", QStringLiteral("����"), QSize(26, 30));
	}

	if (!setUpBtn)
	{
		setUpBtn = createBtn("topmenu_setting.png", QStringLiteral("����"), QSize(26, 30));
		createSetMenu();
	}


	if (!helpBtn)
	{
		helpBtn = createBtn("topmenu_help.png", QStringLiteral("����"), QSize(26, 26));
		createHelpMenu();
	}

	if (!exitBtn1) {
		exitBtn1 = createBtn("topmenu_exit.png", QStringLiteral("�˳�����"), QSize(24, 24));
	}

	if (!exitBtn)
	{
		exitBtn = createBtn("topmenu_quit.png", QStringLiteral("�˳�"), QSize(24, 24));
	}

	//����
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addStretch(6); //6/12
	layout->addWidget(saveBtn, 1);
	layout->addWidget(setUpBtn, 1);
	layout->addWidget(helpBtn, 1);
	layout->addWidget(exitBtn1, 1);
	layout->addWidget(exitBtn, 1);

	toolBar->setParent(parent);
	toolBar->setGeometry(x, y, width, height);
	toolBar->setLayout(layout);

	return toolBar;
}

SoftwareInfo GLTool::getSoftwareInfo()
{
	SoftwareInfo info;
	QSettings reg("HKEY_LOCAL_MACHINE\\Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MainApp.exe", QSettings::NativeFormat); //WOW6432Node����Ϊ32Ϊ��װ����дע���ʱ���ض�����                                                                                                                                          //QString version = reg.value("DisplayVersion").toString();
	QStringList keyList = reg.childKeys();
	info.versionRelase = reg.value("version_relase").toString();
	info.versionComplete = reg.value("version").toString();
	info.installationDateTime = reg.value("InstallationDate").toString();

	return info;
}

void GLTool::Exit(QString cause, bool autotest)
{
	GLDb db;
	if (!db.createDbConnect())
	{
		QAbstractButton *btn = new QPushButton(QStringLiteral("ȷ��"));
		QPair<QAbstractButton *, QMessageBox::ButtonRole> btnPair(btn, QMessageBox::ButtonRole::AcceptRole);
		QVector<QPair<QAbstractButton *, QMessageBox::ButtonRole>> btnVec(1, btnPair);
		QString title = QStringLiteral("��־���");
		QString txt = QStringLiteral("���ݿ�����ʧ�ܣ�");
		//GLMessageBox msg(title, txt, btnVec, QMessageBox::Warning);
	}
	else
	{
		SoftwareOperationRecord record;
		record.content = cause;
		record.classes = "_User";
		record.module = "Login";
		db.setSoftwareOperationRecord(record);
	}

	//�������ݿ�
	if (!autotest)
		databackup();

	QTimer::singleShot(0, qApp, QApplication::closeAllWindows);
}

void GLTool::ShowChangePassword()
{
	GLChangePassword *changPassword = new GLChangePassword;
	AccountType type = GLDb::glAccount.userName == ADMINISTRATOR_USERNAME ? AccountType::Administrator : AccountType::User;
	changPassword->SetAccountType(type);
	changPassword->show();
	changPassword->exec();
	if (changPassword) {
		delete changPassword;
		changPassword = 0;
	}
}


QFileInfoList GLTool::getFileInfoFromDir(QString dir)
{
    QDir _dir(dir);
    QFileInfoList _fileInfoList = _dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
    fileInfoList << _fileInfoList;
    for (int i = 0; i < _fileInfoList.size(); i++)
    {
        getFileInfoFromDir(_fileInfoList.at(i).filePath());
    }

    return fileInfoList;
}

//���ݿⱸ��
void GLTool::databackup()
{
	//GLMessageBox msg1;
	//QMessageBox::ButtonRole role = msg1.YesOrNoError(QStringLiteral("�˳�����"), QStringLiteral("�Ƿ񱸷����ݿ�����"));
	//if (role == QMessageBox::NoRole)
	//  return;
	QString path = "C:/ProgramData/dbbackup/"; //c:/dbbackup/
	QString name = QString("db_jl_bak%1.sql").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
	createPath(path, Dir);
	QString filename = path + name;

	//QString filename = curDir + "/" + QString("db_jl_bak.sql");
	QSettings set(curDir + "/configure/cfg.ini", QSettings::IniFormat);
	set.setIniCodec(QTextCodec::codecForName("GBK"));
	QString mysqldump = set.value("MySQL/mysqldump").toString();
	QString program = mysqldump + "mysqldump.exe";
	QStringList arguments;
	arguments << "-uroot" << "-p123456" << "db_jl";
	//QProcess myProcess;
	//myProcess.setWorkingDirectory("C:/Program Files/MySQL/MySQL Server 8.0/bin");
	//myProcess.setProgram("cmd");
	//myProcess.setArguments(arguments);
	myProcess.setStandardOutputFile(filename); //�����Զ������ļ�������·����Ҫ�Ѿ�����
	myProcess.start(program, arguments);

	//ͬ����д
	//if (!myProcess.waitForStarted())
	//  QString error = myProcess.errorString();

	//if (!myProcess.waitForFinished(60 * 1000))
	//  QString error = myProcess.errorString();

	//QByteArray result = myProcess.readAll();
	//myProcess.close();

	//�첽��д 
	connect(&myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus)
		{
			QByteArray result = myProcess.readAll();
			myProcess.close();
			myProcess.kill();
			msg.accept();
			//createPath("C:/ProgramData/JiaLiang/SurgeryPlan/", Dir);
			//QFile::copy(filename, "C:/ProgramData/JiaLiang/SurgeryPlan/" + name);
			GLMessageBox msg;
			msg.queRenInfo(QStringLiteral("�˳�����"), QStringLiteral("���ݿ����ݱ�����ɣ�"));		});

	QMessageBox::ButtonRole role1 = msg.queRenInfo(QStringLiteral("�˳�����"), QStringLiteral("���ڱ��ݡ���\n�����Ҫ��ֹ���ݣ�������ֹ����"), QStringLiteral("��ֹ����"));
	if (role1 == QMessageBox::AcceptRole)
	{
		myProcess.close();
		myProcess.kill();
	}
}

int GLTool::calAge(QString date)
{
	//emit sig_log("���㻼�����䣬�������գ�"+date);
	int year = date.section('/', 0, 0).toInt();
	int month = date.section('/', 1, 1).toInt();
	int day = date.section('/', 2, 2).toInt();

	int currentYear = QDate::currentDate().year();
	int currentMonth = QDate::currentDate().month();
	int currentDay = QDate::currentDate().day();

	int yearDiff = currentYear - year;
	if (month > currentMonth)
		yearDiff--;
	if (month == currentDay && day > currentDay)
		yearDiff--;

	return yearDiff;
}
