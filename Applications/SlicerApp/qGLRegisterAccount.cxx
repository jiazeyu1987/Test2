#include "qGLMessageDialog.h"

//#include "GLCommonTool.h"
#include "qGLDb.h"
#include "qGLRegisterAccount.h"

#include "qpainter.h"

GLRegisterAccount::GLRegisterAccount(QWidget* parent) :QDialog(parent)
{
	m_Control.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);          //���ش��ڱ�����
	this->setWindowModality(Qt::ApplicationModal);          //����������
	this->setAttribute(Qt::WA_TranslucentBackground, true); //����͸��
	connect(m_Control.btnCancel, &QPushButton::clicked, this, &GLRegisterAccount::OnCancelClick);
	connect(m_Control.btnOK, &QPushButton::clicked, this, &GLRegisterAccount::OnConfirmClick);
	m_Control.editPhone->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));

	//��ǿ������� 2022��6��14��
	//��ӿؼ��Ľ���˳��
	setTabOrder(m_Control.editOrganizations, m_Control.editDepartment);
	setTabOrder(m_Control.editDepartment, m_Control.editName);
	setTabOrder(m_Control.editName, m_Control.cmbSex);
	setTabOrder(m_Control.cmbSex, m_Control.editPhone);
	setTabOrder(m_Control.editPhone, m_Control.editUser);
	setTabOrder(m_Control.editUser, m_Control.editPassword);
	setTabOrder(m_Control.editPassword, m_Control.editPassword2);
}

GLRegisterAccount::~GLRegisterAccount()
{
}

void GLRegisterAccount::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.fillRect(this->rect(), QColor(0, 0, 0, 180));  //QColor���һ��������������͸����
}

void GLRegisterAccount::OnCancelClick()
{
	QDialog::reject();
	this->close();
}

void GLRegisterAccount::OnConfirmClick()
{
	QString organization = m_Control.editOrganizations->text();;
	QString department = m_Control.editDepartment->text();
	QString name = m_Control.editName->text();
	QChar sex = QChar::Null;
	if (m_Control.cmbSex->currentText() == QStringLiteral("��"))
		sex = QChar('M');
	else
		sex = QChar('F');
	QString mobile = m_Control.editPhone->text();
	QString userName = m_Control.editUser->text();
	QString password1 = m_Control.editPassword->text();
	QString password2 = m_Control.editPassword2->text();
	if (!DetectInputLegit(organization, department, name, sex, mobile, userName, password1, password2))
		return;
	if (!RegisterAnAccount(organization, department, name, sex, mobile, userName, password1, password2))
		return;
	QDialog::accept();
	this->close();
}

bool GLRegisterAccount::DetectInputLegit(QString organization, QString department, QString name, QChar sex, QString mobile, QString userName, QString password1, QString password2)
{
	if (organization.isEmpty() || department.isEmpty() || name.isEmpty() || sex.isNull() || mobile.isEmpty() || userName.isEmpty())
	{
		qGLMessageDialog::ShowInformation(QStringLiteral("�û�ע��"), QStringLiteral("<font color='red'>����д������</font>"));
		return false;
	}

	/*if (!GLCommonTool::DetectPasswordLegit(password1)) {
		qGLMessageDialog::ShowInformation(QStringLiteral("�޸�����"), QStringLiteral("�����ʽΪ6��12λ������ĸ��������϶��ɣ����ܵ���ֻ�����ֻ�����ĸ�����ִ�Сд"));
		return false;
	}*/

	//�ж���������������Ƿ�һ��
	if (password1 != password2)
	{
		qGLMessageDialog::ShowInformation(QStringLiteral("�޸�����"), QStringLiteral("ȷ������������벻һ��"));
		return false;
	}
	return true;
}

bool GLRegisterAccount::RegisterAnAccount(QString organization, QString department, QString name, QChar sex, QString mobile, QString userName, QString password, QString password2)
{
	//�������ݿ�
	GLDb db;
	if (!db.createDbConnect())
	{
		QString title = QStringLiteral("�޸�����");
		QString txt = QStringLiteral("���ݿ�����ʧ�ܣ�");
		qGLMessageDialog::ShowWarnning(title, txt);
		return false;
	}
	Doctors doctor;
	doctor.doctorName = name;
	doctor.hostpital = organization;
	doctor.department = department;
	doctor.sex = sex;
	doctor.mobile = mobile;
	Account account;
	account.userName = userName;
	account._password = password;
	db.reg(doctor, account);
	return true;
}
