#pragma once
/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef qGLMessageDialog_H
#define qGLMessageDialog_H

//#include <org_mitk_gui_common_Export.h>
#include <QString>
#include "ui_qGLMessageDialog.h"
//#include "org_gl_comm_Export.h"
#include "qSlicerAppExport.h"
#include "qpainter.h"
#include "BasicDataClass.h"

class Q_SLICER_APP_EXPORT qGLMessageDialog : public QDialog
{
	Q_OBJECT

public:
	explicit qGLMessageDialog(QWidget* parent = 0);
	~qGLMessageDialog();

	void static ShowInformation(const QString& contentText, bool isModelWindow = true);
	void static ShowInformation(const QString& title, const QString& contentText, bool isModelWindow = true);
	void static ShowInformationWithoutIcon(const QString& title, const QString& contentText);
	void static ShowWarnning(const QString& contentText, bool isModelWindow = true);
	void static ShowWarnning(const QString& title, const QString& contentText, bool isModelWindow = true);
	MessageResult static ShowQuestion(const QString& title, const QString& contentText, MessageButtonType type = MessageButtonType::BUTTON_OK_AND_CANCEL,
		QStringList lblList = { QStringLiteral("确定"), QStringLiteral("取消"), QStringLiteral("接受") }, bool isModelWindow = true);

protected slots:
	void dosomething();
	void onOKClicked();
	void onOk2Clicked();
	void onCancelClicked();
	void onCloseClicked();

public:
	//void paintEvent(QPaintEvent* event);
	void SetTitle(QString title);
	void SetContentText(QString mainMsg, QString addMsg = NULL);
	void SetMsgType(MessageType type);
	void SetMsgButtonLabel(QStringList lblList);
	void SetMsgButtonType(MessageButtonType type);
	int static ShowMessageBox(//QWidget* parent,
		const QString& title,                //标题
		const QString& contentText,          //提示信息内容
		MessageType messageType,             //信息类型
		MessageButtonType messageButtonType, //按钮类型
		bool isModelWindow = false,
		QStringList lblList = { QStringLiteral("确定"), QStringLiteral("取消"), QStringLiteral("接受") });
private:
	Ui::qGLMessageDialog* ui;
	MessageType messageType;

};

#endif /* define qGLMessageDialog_H */
