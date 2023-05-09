#include "qGLMessageDialog.h"

//#include "qGLCommonTool.h"
#include "qGLDb.h"
#include "qGLChangePassword.h"

#include "qpainter.h"

GLChangePassword::GLChangePassword(QWidget *parent) :QDialog(parent)
{
	m_Control.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);          //���ش��ڱ�����
	this->setWindowModality(Qt::ApplicationModal);          //����������
	this->setAttribute(Qt::WA_TranslucentBackground, true); //����͸��
	connect(m_Control.btnCancel, &QPushButton::clicked, this, &GLChangePassword::OnCancelClick);
	connect(m_Control.btnOK, &QPushButton::clicked, this, &GLChangePassword::OnConfirmClick);
}

GLChangePassword::~GLChangePassword()
{
}

void GLChangePassword::SetAccountType(AccountType type)
{
	accountType = type;
	QString lblText = QStringLiteral("�û���");
	if(type == AccountType::User) lblText = QStringLiteral("ԭ����");
	m_Control.lblType->setText(lblText);
}

void GLChangePassword::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);
	painter.fillRect(this->rect(), QColor(0, 0, 0, 180));  //QColor���һ��������������͸����
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
			//GLMessageDialog::ShowWarnning(QStringLiteral("�޸�����"), QStringLiteral("û�и��˺�"));
			GLMessageBox msg;
			msg.queRenWarn(QStringLiteral("�޸�����"), QStringLiteral("û�и��˺�"));
			return;
		}
		if (QString::fromStdString(dbPassword) != oldPassword) {
			//GLMessageDialog::ShowWarnning(QStringLiteral("�޸�����"), QStringLiteral("ԭ�������"));
			GLMessageBox msg;
			msg.queRenWarn(QStringLiteral("�޸�����"), QStringLiteral("ԭ�������"));
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
			//GLMessageDialog::ShowWarnning(QStringLiteral("�޸�����"), QStringLiteral("���ݿ�����ʧ�ܣ�"));
			GLMessageBox msg;
			msg.queRenWarn(QStringLiteral("�޸�����"), QStringLiteral("���ݿ�����ʧ��!"));
			return;
		}
		Account accountExist;
		accountExist.userName = userName;
		if (dbExist.existUsername(accountExist) == 0)
		{
			//GLMessageDialog::ShowWarnning(QStringLiteral("�޸�����"), QStringLiteral("�û��������ڣ�"));
			GLMessageBox msg;
			msg.queRenWarn(QStringLiteral("�޸�����"), QStringLiteral("�û���������!"));
			return;
		}
		if (accountExist._password == password1)
		{
			//GLMessageDialog::ShowWarnning(QStringLiteral("�޸�����"), QStringLiteral("ԭ�������������ͬ��"));
			GLMessageBox msg;
			msg.queRenWarn(QStringLiteral("�޸�����"), QStringLiteral("ԭ�������������ͬ!"));
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
		//GLMessageDialog::ShowInformation(QStringLiteral("�޸�����"), QStringLiteral("�����ʽΪ6��12λ������ĸ��������϶��ɣ����ܵ���ֻ�����ֻ�����ĸ�����ִ�Сд"));
		GLMessageBox msg;
		msg.queRenWarn(QStringLiteral("�޸�����"), QStringLiteral("�����ʽΪ6��12λ������ĸ��������϶��ɣ����ܵ���ֻ�����ֻ�����ĸ�����ִ�Сд!"));
		return false;
	}

	//�ж���������������Ƿ�һ��
	if (password1 != password2)
	{
		//GLMessageDialog::ShowInformation(QStringLiteral("�޸�����"), QStringLiteral("ȷ������������벻һ��"));
		GLMessageBox msg;
		msg.queRenWarn(QStringLiteral("�޸�����"), QStringLiteral("ȷ������������벻һ��!"));
		return false;
	}
	return true;
}

bool GLChangePassword::ChangePassword(QString userName, QString password, QString password2)
{
	//�������ݿ�
	GLDb db;
	if (!db.createDbConnect())
	{
		//GLMessageDialog::ShowWarnning(QStringLiteral("�޸�����"), QStringLiteral("���ݿ�����ʧ�ܣ�"));
		GLMessageBox msg;
		msg.queRenWarn(QStringLiteral("�޸�����"), QStringLiteral("���ݿ�����ʧ��!"));
		return false;
	}
	Account account;
	account.userName = userName;
	account._password = password;
	db.setPassword(account);
	return true;
}
