#ifndef GL_Change_Password
#define GL_Change_Password
#include "qdialog.h"

#include "BasicDataClass.h"

#include "ui_qGLChangePassword.h"

class Q_SLICER_APP_EXPORT GLChangePassword : public QDialog {
	Q_OBJECT
public:
	GLChangePassword(QWidget *parent = 0);
	~GLChangePassword();

	void SetAccountType(AccountType type);
protected:
	void paintEvent(QPaintEvent *e) override;
/// <summary>
/// 
/// </summary>
private slots:
	void OnCancelClick();
	void OnConfirmClick();

private:
	//Change password
	bool ChangePassword(QString userName, QString password, QString password2);
	//Detects whether the input is legitimate
	bool DetectInputLegit(QString password, QString password2);

private:
	AccountType accountType;

	Ui::GLChangePassword m_Control;
};



#endif // !GL_Change_Password
