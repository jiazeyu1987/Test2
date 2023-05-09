#pragma once
#ifndef GL_Register_Account
#define GL_Register_Account
#include "qdialog.h"
#include "BasicDataClass.h"
#include "ui_qGLRegisterAccount.h"
#include "qSlicerAppExport.h"
   
class Q_SLICER_APP_EXPORT GLRegisterAccount : public QDialog {
	Q_OBJECT
public:
	GLRegisterAccount(QWidget* parent = nullptr);
	~GLRegisterAccount();
protected:
	void paintEvent(QPaintEvent* e) override;

private slots:
	void OnCancelClick();
	void OnConfirmClick();

private:
	//Detects whether the input is legitimate
	bool DetectInputLegit(QString organization, QString department, QString name, QChar sex, QString mobile, QString userName, QString password, QString password2);
	//register an account
	bool RegisterAnAccount(QString organization, QString department, QString name, QChar sex, QString mobile, QString userName, QString password, QString password2);

private:
	AccountType accountType;
	Ui::qGLRegisterAccount m_Control;
};



#endif // !GL_Register_Account
