#include "qGLMessageDialog.h"

//#include "qGLCommonTool.h"
#include "qGLDb.h"
#include "qGLChangePassword.h"

#include "qpainter.h"

GLChangePassword::GLChangePassword(QWidget *parent) :QDialog(parent)
{
	m_Control.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);          //隐藏窗口标题栏
	this->setWindowModality(Qt::ApplicationModal);          //阻滞主窗口
	this->setAttribute(Qt::WA_TranslucentBackground, true); //设置透明
	connect(m_Control.btnCancel, &QPushButton::clicked, this, &GLChangePassword::OnCancelClick);
	connect(m_Control.btnOK, &QPushButton::clicked, this, &GLChangePassword::OnConfirmClick);
}

GLChangePassword::~GLChangePassword()
{
}

void GLChangePassword::SetAccountType(AccountType type)
{
	accountType = type;
	QString lblText = QStringLiteral("用户名");
	if(type == AccountType::User) lblText = QStringLiteral("原密码");
	m_Control.lblType->setText(lblText);
}

void GLChangePassword::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);
	painter.fillRect(this->rect(), QColor(0, 0, 0, 180));  //QColor最后一个参数代表背景的透明度
}

void GLChangePassword::OnCancelClick()
{
	QDialog::reject();
	this->close();
}

void GLChangePassword::OnConfirmClick()
{
	//When the account type is Administrator, info represents the user account number, otherwise info represent the old password
	QString userName = m_Control.editUser->text();
	QString oldPassword = userName;
	QString password1 = m_Control.editPassword->text();
	QString password2 = m_Control.editPassword2->text();
	if (accountType == AccountType::User) {
		std::string dbUserName(GLDb::glAccount.userName.toStdString());
		std::string dbPassword(GLDb::glAccount._password.toStdString());
		if (dbUserName.empty() || dbPassword.empty()) {
			//GLMessageDialog::ShowWarnning(QStringLiteral("修改密码"), QStringLiteral("没有该账号"));
			GLMessageBox msg;
			msg.queRenWarn(QStringLiteral("修改密码"), QStringLiteral("没有该账号"));
			return;
		}
		if (QString::fromStdString(dbPassword) != oldPassword) {
			//GLMessageDialog::ShowWarnning(QStringLiteral("修改密码"), QStringLiteral("原密码错误"));
			GLMessageBox msg;
			msg.queRenWarn(QStringLiteral("修改密码"), QStringLiteral("原密码错误"));
			return;
		}
		userName = QString::fromStdString(dbUserName);
	}
	if (!DetectInputLegit(password1, password2))
		return;

	//Database detection is placed after local detection(DetectInputLegit)
	if (accountType == AccountType::Administrator) 
	{
		GLDb dbExist(this);
		if (!dbExist.createDbConnect())
		{
			//GLMessageDialog::ShowWarnning(QStringLiteral("修改密码"), QStringLiteral("数据库连接失败！"));
			GLMessageBox msg;
			msg.queRenWarn(QStringLiteral("修改密码"), QStringLiteral("数据库连接失败!"));
			return;
		}
		Account accountExist;
		accountExist.userName = userName;
		if (dbExist.existUsername(accountExist) == 0)
		{
			//GLMessageDialog::ShowWarnning(QStringLiteral("修改密码"), QStringLiteral("用户名不存在！"));
			GLMessageBox msg;
			msg.queRenWarn(QStringLiteral("修改密码"), QStringLiteral("用户名不存在!"));
			return;
		}
		if (accountExist._password == password1)
		{
			//GLMessageDialog::ShowWarnning(QStringLiteral("修改密码"), QStringLiteral("原密码和新密码相同！"));
			GLMessageBox msg;
			msg.queRenWarn(QStringLiteral("修改密码"), QStringLiteral("原密码和新密码相同!"));
			return;
		}
	}

	if (!ChangePassword(userName, password1, password2))
		return;
	QDialog::accept();
	this->close();
}
bool DetectPasswordLegit(QString password)
{
	//Determine if the password length is legitimate
	if (password.length() < 6 || password.length() > 12)
	{
		return false;
	}

	//Determine if the password contains number + letter
	int letter = 0;
	int number = 0;
	int other = 0;
	for (int i = 0; i < password.size(); i++)
	{
		QChar c = password.at(i);
		if (!c.isLetterOrNumber())
			other++;
		else if (c.isNumber())
			number++;
		else if (c.isLetter())
			letter++;
	}

	if (other != 0 || number == 0 || letter == 0)
	{
		return false;
	}
	return true;
}

bool GLChangePassword::DetectInputLegit(QString password1, QString password2)
{
	if (!DetectPasswordLegit(password1)) {
		//GLMessageDialog::ShowInformation(QStringLiteral("修改密码"), QStringLiteral("密码格式为6到12位，由字母和数字组合而成，不能单独只有数字或者字母，区分大小写"));
		GLMessageBox msg;
		msg.queRenWarn(QStringLiteral("修改密码"), QStringLiteral("密码格式为6到12位，由字母和数字组合而成，不能单独只有数字或者字母，区分大小写!"));
		return false;
	}

	//判断输入的两遍密码是否一样
	if (password1 != password2)
	{
		//GLMessageDialog::ShowInformation(QStringLiteral("修改密码"), QStringLiteral("确认密码和新密码不一致"));
		GLMessageBox msg;
		msg.queRenWarn(QStringLiteral("修改密码"), QStringLiteral("确认密码和新密码不一致!"));
		return false;
	}
	return true;
}

bool GLChangePassword::ChangePassword(QString userName, QString password, QString password2)
{
	//存入数据库
	GLDb db;
	if (!db.createDbConnect())
	{
		//GLMessageDialog::ShowWarnning(QStringLiteral("修改密码"), QStringLiteral("数据库连接失败！"));
		GLMessageBox msg;
		msg.queRenWarn(QStringLiteral("修改密码"), QStringLiteral("数据库连接失败!"));
		return false;
	}
	Account account;
	account.userName = userName;
	account._password = password;
	db.setPassword(account);
	return true;
}
