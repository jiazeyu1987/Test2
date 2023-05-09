

#include <qapplication.h>
#include <qGLMessageDialog.h>

//日志
//#include <mitkLog.h>
//#include <mbilog.h>
//#include <mitkLogMacros.h>

qGLMessageDialog::qGLMessageDialog(QWidget* parent) :QDialog(parent), ui(new Ui::qGLMessageDialog())
{
	ui->setupUi(this);
	ui->lblMsgWithoutIcon->hide();

	//设置半透明背景 by zbs
	this->setWindowFlags(Qt::FramelessWindowHint);          //隐藏窗口标题栏
	//this->setWindowModality(Qt::ApplicationModal);          //阻滞主窗口
	this->setAttribute(Qt::WA_TranslucentBackground, true); //设置透明
	this->setAttribute(Qt::WA_DeleteOnClose);               //关闭即销毁
	connect(ui->pBtnOK, SIGNAL(clicked()), this, SLOT(onOKClicked()));
	connect(ui->pBtnOK2, SIGNAL(clicked()), this, SLOT(onOk2Clicked()));
	connect(ui->pBtnCancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
	connect(ui->pBtnClose, SIGNAL(clicked()), this, SLOT(onCloseClicked()));
	setModal(true);
}


void qGLMessageDialog::dosomething()
{

}

void qGLMessageDialog::onOKClicked()
{
	QDialog::accept();
	MessageResult::OK;
	close();
}

void qGLMessageDialog::onOk2Clicked()
{
	QDialog::done(2);
	MessageResult::ACCEPT;
	close();
}

void qGLMessageDialog::onCancelClicked()
{
	QDialog::reject();
	MessageResult::CANCEL;
	close();
}

void qGLMessageDialog::onCloseClicked()
{
	 close();
}
//背景重绘
//void qGLMessageDialog::paintEvent(QPaintEvent* event)
//{
//	QPainter painter(this);
//	painter.fillRect(this->rect(), QColor(0, 0, 0, 180));  //QColor最后一个参数代表背景的透明度
//}

qGLMessageDialog::~qGLMessageDialog()
{
	delete ui;
}

void qGLMessageDialog::SetTitle(QString title)
{
	ui->lblTitle->setText(title);
}

void qGLMessageDialog::SetContentText(QString mainMsg, QString addMsg)
{
	if (messageType != MessageType::WITHOUTICON) {
		ui->lblMainMsg->setText(mainMsg);
		ui->lblAdditionalMsg->setText(addMsg);
		ui->lblMainMsg->setAlignment(Qt::AlignCenter);
	}
	else {
		ui->lblMsgWithoutIcon->setText(mainMsg);
	}
}

void qGLMessageDialog::SetMsgType(MessageType type)
{
	QString qss = "border-image: url(:/public/SlicerResources/public/finish.png)";
	messageType = type;
	switch (type)
	{
	case INFORMATION:
		qss = "border-image: url(:/public/SlicerResources/public/finish.png)";
		break;
	case WARNNING:
		qss = "border-image: url(:/public/SlicerResources/public/warning.png)";
		break;
	case QUESTION:
		qss = "border-image: url(:/public/SlicerResources/public/tip.png)";
		break;
	case WITHOUTICON:
		ui->lblIcon->hide();
		ui->lblAdditionalMsg->hide();
		ui->lblMsgWithoutIcon->show();
		break;
	default:
		break;
	}
	ui->lblIcon->setStyleSheet(qss);
}

void qGLMessageDialog::SetMsgButtonLabel(QStringList lblList)
{
	int size = lblList.count();
	if (size <= 0) return;
	ui->pBtnOK->setText(lblList[0]);
	if (size >= 2) ui->pBtnCancel->setText(lblList[1]);
	if (size >= 3) ui->pBtnOK2->setText(lblList[2]);
}

void qGLMessageDialog::SetMsgButtonType(MessageButtonType type)
{
	switch (type)
	{
	case BUTTON_OK:
		ui->pBtnCancel->hide();
		ui->pBtnOK2->hide();
		break;
	case BUTTON_OK_AND_CANCEL:
		ui->pBtnOK2->hide();
		break;
	case BUTTON_OK_AND_ACCEPT:
	case BUTTON_CLOSE:
		break;
	default:
		break;
	}
}

int qGLMessageDialog::ShowMessageBox(const QString& title, const QString& contentText, MessageType messageType, MessageButtonType messageButtonType, bool isModelWindow, QStringList lblList)
{
	qGLMessageDialog* msgBox = new qGLMessageDialog;
	msgBox->SetTitle(title);
	msgBox->SetMsgType(messageType);
	msgBox->SetContentText(contentText);
	msgBox->SetMsgButtonLabel(lblList);
	msgBox->SetMsgButtonType(messageButtonType);
	if (isModelWindow)
		return msgBox->exec();
	else
		msgBox->show();
	return -1;
}

void qGLMessageDialog::ShowInformation(const QString& contentText, bool isModelWindow)
{
	int result = ShowMessageBox(QStringLiteral(" 提示信息"), contentText, MessageType::INFORMATION, MessageButtonType::BUTTON_OK, isModelWindow);
}

void qGLMessageDialog::ShowInformation(const QString& title, const QString& contentText, bool isModelWindow)
{
	int result = ShowMessageBox(title, contentText, MessageType::INFORMATION, MessageButtonType::BUTTON_OK, isModelWindow);
}

void qGLMessageDialog::ShowInformationWithoutIcon(const QString& title, const QString& contentText)
{
	int result = ShowMessageBox(title, contentText, MessageType::WITHOUTICON, MessageButtonType::BUTTON_OK, true);
}

void qGLMessageDialog::ShowWarnning(const QString& contentText, bool isModelWindow)
{
	int result = ShowMessageBox(QStringLiteral(" 警告信息"), contentText, MessageType::WARNNING, MessageButtonType::BUTTON_OK, isModelWindow);
}

void qGLMessageDialog::ShowWarnning(const QString& title, const QString& contentText, bool isModelWindow)
{
	int result = ShowMessageBox(title, contentText, MessageType::WARNNING, MessageButtonType::BUTTON_OK, isModelWindow);
}

MessageResult qGLMessageDialog::ShowQuestion(const QString& title, const QString& contentText, MessageButtonType type, QStringList lblList, bool isModelWindow)
{
	int result = ShowMessageBox(title, contentText, MessageType::QUESTION, type, isModelWindow, lblList);
	if (result == OK)
	{
		return MessageResult::OK;
	}
	else if (result == CANCEL)
	{
		return MessageResult::CANCEL;
	}
	else if (result == ACCEPT)
	{
		return MessageResult::ACCEPT;
	}

	return MessageResult::CANCEL;
}
