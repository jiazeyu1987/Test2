#pragma once
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtimer.h>
#include "qSlicerAppExport.h"

//�����Ϣ��ʾ��
class Q_SLICER_APP_EXPORT GLMessageBoxStyle : public QLabel
{
        Q_OBJECT
public:
        static GLMessageBoxStyle& instance();
        void setMessage(const QString message, const QPoint* pPoint = nullptr);
        void setEnableEvent(bool enable);

private:
        GLMessageBoxStyle();
        ~GLMessageBoxStyle();

        void onTimer();
        void enterEvent(QEvent*);
        void leaveEvent(QEvent*);

private:
        QWidget* parent;
        QTimer* timer;
        bool enter;
        int transparent;
        bool enableEvent = false; //�Ƿ����Զ����¼�
};

class Q_SLICER_APP_EXPORT GLMessageBox : public QMessageBox
{
        Q_OBJECT
public:
        GLMessageBox(QString title, QString txt, QVector<QPair<QAbstractButton*, QMessageBox::ButtonRole>> btnVec, QMessageBox::Icon icon, QWidget* parent = nullptr);
        GLMessageBox(QWidget* parent = nullptr);
        ~GLMessageBox();

        QMessageBox::ButtonRole queRenInfo(QString title, QString txt, QString btnText = QStringLiteral("ȷ��"));
        QMessageBox::ButtonRole queRenWarn(QString title, QString txt, QString btnText = QStringLiteral("ȷ��"));
        QMessageBox::ButtonRole queRenError(QString title, QString txt, QString btnText = QStringLiteral("ȷ��"));

        QMessageBox::ButtonRole YesOrNoInfo(QString title, QString txt, QString txtYes = QStringLiteral("��"), QString txtNo = QStringLiteral("��"));
        QMessageBox::ButtonRole YesOrNoWarn(QString title, QString txt, QString txtYes = QStringLiteral("��"), QString txtNo = QStringLiteral("��"));
        QMessageBox::ButtonRole YesOrNoError(QString title, QString txt, QString txtYes = QStringLiteral("��"), QString txtNo = QStringLiteral("��"));

        QAbstractButton* result();

        void SetSize(int width, int height);  //�޸Ĵ�С

private:
        int m_width = 256;
        int m_height = 110;

        void resizeEvent(QResizeEvent* event);
};