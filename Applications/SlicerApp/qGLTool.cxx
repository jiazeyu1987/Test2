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
QToolButton *GLTool::saveBtn = nullptr; //保存
QToolButton *GLTool::setUpBtn = nullptr; //设置-齿轮
QToolButton *GLTool::helpBtn = nullptr; //帮助-问号
QToolButton *GLTool::exitBtn1 = nullptr; //退出分析
QToolButton *GLTool::exitBtn = nullptr; //退出按钮

//QProcess GLTool::myProcess;
//GLMessageBox GLTool::msg;
QFileInfoList GLTool::fileInfoList;
bool GLTool::odbc = false; //默认不使用odbc方式连接

  //报告详情  
QString GLTool::datetime; //日期（出具时间）
QString GLTool::worker; //操作人（出具人）
QString GLTool::receiveTime; //接收时间
QString GLTool::verifier; //审核人
QString GLTool::doctor; //医生
QString GLTool::hospital; //医院
QString GLTool::address; //所在地
QString GLTool::auditTime; //审核时间
  //患者信息
//QString GLTool::patientName; //患者姓名
QString GLTool::patientBirthday; //患者生日
QString GLTool::patientAge; //患者年龄
QString GLTool::height; //身高
QString GLTool::weight; //体重
QString GLTool::patientSex; //性别

QString GLTool::anamnesis; //既往病史
QString GLTool::patientRemark; //患者备注
  //病灶区
QVector<FocalZone> GLTool::focalZoneVec;

QString GLTool::patientAdvice; //医嘱，即患者备注
QString GLTool::patientAnalyName; //分析名称

//光纤信息
QVector<Optical> GLTool::opticalVec; //光纤信息

QStringList GLTool::capture; //截图路径

QString GLTool::operationRecord; //来自消融后处理-手术记录

int GLTool::patientAnalyseId; //报告所属的分析id



bool GLTool::createPath(QString path, ::PathType type)
{
	if (type == ::PathType::File)
	{
		//判断路径是否存在
		QFile file(path);
		if (file.exists())
			return true;

		int first = path.lastIndexOf("/");

		//创建目录
		QString dirpath = path.mid(0, first);
		QDir dir(dirpath);
		if (!dir.exists())
			dir.mkdir(dirpath);

		//创建文件
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
	//-----------详细信息--------------
	menu->addAction(QStringLiteral("详细信息"), [=]()
		{
			//emit this.sig_log("工具栏-详细信息");
			//数据放到配置文件，数据库或者注册表
			SoftwareInfo info = getSoftwareInfo();
            QString c = "";
			QString txt = QStringLiteral
			(
				"软件名称：立体定向手术计划软件<br/>"
				"型号规格：PS101<br/>"
				"发布版本：%1<br/>" //1
				"完整版本：%2<br/>" //1.0.0
				"安装时间：%3<br/>"
				"注册人名称：%4<br/>"
				"地址：%5<br/>"
				"联系方式：%6<br/>"
				"版权声明：%7"
				//"许可证号：%5"
			).arg(info.versionRelase).arg(info.versionComplete).arg(info.installationDateTime)
				.arg(info.registerName).arg(info.address).arg(info.contact).arg(info.copyright);
			GLMessageBox msg;
			msg.queRenInfo(QStringLiteral("详细信息"), txt);
			//GLMessageDialog::ShowInformationWithoutIcon(QStringLiteral("详细信息"), txt);
		});
	//-----------操作手册----------------------
	menu->addAction(QStringLiteral("操作手册"), []()
		{
			//MITK_CUSTOM << "工具栏-操作手册";
			QString manual = curDir + "/configure/manual.pdf";
			QDesktopServices::openUrl(QUrl::fromLocalFile(manual));
		});
}

void GLTool::createSetMenu()
{
	QMenu *menu = new QMenu(toolBar);
	setUpBtn->setMenu(menu);
	setUpBtn->setPopupMode(QToolButton::InstantPopup);
	menu->addAction(QStringLiteral("修改密码"), []()
	{
		//MITK_CUSTOM << "修改密码";
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
		menu->addAction(QStringLiteral("注册用户"), []()
		{
			//MITK_CUSTOM << "注册用户";
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
		saveBtn = createBtn("topmenu_save.png", QStringLiteral("保存"), QSize(26, 30));
	}

	if (!setUpBtn)
	{
		setUpBtn = createBtn("topmenu_setting.png", QStringLiteral("设置"), QSize(26, 30));
		createSetMenu();
	}


	if (!helpBtn)
	{
		helpBtn = createBtn("topmenu_help.png", QStringLiteral("帮助"), QSize(26, 26));
		createHelpMenu();
	}

	if (!exitBtn1) {
		exitBtn1 = createBtn("topmenu_exit.png", QStringLiteral("退出分析"), QSize(24, 24));
	}

	if (!exitBtn)
	{
		exitBtn = createBtn("topmenu_quit.png", QStringLiteral("退出"), QSize(24, 24));
	}

	//布局
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
	QSettings reg("HKEY_LOCAL_MACHINE\\Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MainApp.exe", QSettings::NativeFormat); //WOW6432Node是因为32为安装程序写注册表时被重定向了                                                                                                                                          //QString version = reg.value("DisplayVersion").toString();
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
		QAbstractButton *btn = new QPushButton(QStringLiteral("确认"));
		QPair<QAbstractButton *, QMessageBox::ButtonRole> btnPair(btn, QMessageBox::ButtonRole::AcceptRole);
		QVector<QPair<QAbstractButton *, QMessageBox::ButtonRole>> btnVec(1, btnPair);
		QString title = QStringLiteral("日志审核");
		QString txt = QStringLiteral("数据库连接失败！");
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

	//备份数据库
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

//数据库备份
void GLTool::databackup()
{
	//GLMessageBox msg1;
	//QMessageBox::ButtonRole role = msg1.YesOrNoError(QStringLiteral("退出程序"), QStringLiteral("是否备份数据库数据"));
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
	myProcess.setStandardOutputFile(filename); //可以自动生成文件，但是路径需要已经存在
	myProcess.start(program, arguments);

	//同步读写
	//if (!myProcess.waitForStarted())
	//  QString error = myProcess.errorString();

	//if (!myProcess.waitForFinished(60 * 1000))
	//  QString error = myProcess.errorString();

	//QByteArray result = myProcess.readAll();
	//myProcess.close();

	//异步读写 
	connect(&myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus)
		{
			QByteArray result = myProcess.readAll();
			myProcess.close();
			myProcess.kill();
			msg.accept();
			//createPath("C:/ProgramData/JiaLiang/SurgeryPlan/", Dir);
			//QFile::copy(filename, "C:/ProgramData/JiaLiang/SurgeryPlan/" + name);
			GLMessageBox msg;
			msg.queRenInfo(QStringLiteral("退出程序"), QStringLiteral("数据库数据备份完成！"));		});

	QMessageBox::ButtonRole role1 = msg.queRenInfo(QStringLiteral("退出程序"), QStringLiteral("正在备份……\n如果需要中止备份，请点击中止备份"), QStringLiteral("中止备份"));
	if (role1 == QMessageBox::AcceptRole)
	{
		myProcess.close();
		myProcess.kill();
	}
}

int GLTool::calAge(QString date)
{
	//emit sig_log("计算患者年龄，患者生日："+date);
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
