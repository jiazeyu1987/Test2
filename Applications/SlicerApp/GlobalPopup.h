#ifndef __GlobalPopup_h
#define __GlobalPopup_h
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPalette>
#include <QTimer>
#include <vtkMRMLScene.h>
class GlobalPopup : public QWidget
{
public:
    GlobalPopup() {
        //���ñ���͸��
        setAttribute(Qt::WA_TranslucentBackground, true);
        //�����ޱ߿�
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        QLabel* label = new QLabel(QStringLiteral("�ѽ���"), this);
        label->setStyleSheet("font-size: 48px; color: white; background-color: rgba(0, 0, 0, 0.1)");
        //���ñ�ǩ����
        label->setAlignment(Qt::AlignCenter);
        //�Զ���Ӧ��ǩ��С
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFixedSize(1920, 1080);
        label->setFixedSize(1920, 1080);

        {
            //�첽��ʱ���رյ���
            QTimer::singleShot(200, this, [this]() {
                this->close();
                });
        }
    }

    GlobalPopup(QString message, float alpha) {
        //���ñ���͸��
        setAttribute(Qt::WA_TranslucentBackground, true);
        //�����ޱ߿�
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        QLabel* label = new QLabel(message, this);
        label->setStyleSheet(QStringLiteral("font-size: 48px; color: white; background-color: rgba(0, 0, 0, %1)").arg(alpha));
        //���ñ�ǩ����
        label->setAlignment(Qt::AlignCenter);
        //�Զ���Ӧ��ǩ��С
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFixedSize(1920, 1080);
        label->setFixedSize(1920, 1080);
    }

    GlobalPopup(QString message,int delay,bool async=false)
    {
        //���ñ���͸��
        setAttribute(Qt::WA_TranslucentBackground, true);
        //�����ޱ߿�
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        //������ǩ��������ʽ
        QString meg = message + QString("(%1)").arg(delay / 1000);
        QLabel* label = new QLabel(meg, this);
        label->setStyleSheet("font-size: 48px; color: white; background-color: rgba(0, 0, 0, 0.8)");
        //���ñ�ǩ����
        label->setAlignment(Qt::AlignCenter);
        //�Զ���Ӧ��ǩ��С
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFixedSize(1920, 1080);
        label->setFixedSize(1920, 1080);

        {
            //�첽��ʱ���رյ���
            QTimer::singleShot(0, this, [this, label,delay, message]() {
                this->onClose(label,delay+1000, message);
                });
        }
    }

    void onClose(QLabel* label,int delay,QString message) {
        int newdelay = delay - 1000;
        if (newdelay < 1) {
            close();
            qSlicerApplication::application()->mrmlScene()->InvokeEvent(1331225, nullptr);
        }
        else {
            label->setText(message + QString("(%1)").arg(newdelay/1000));
            QTimer::singleShot(1000, this, [this, label, newdelay, message]() {
                this->onClose(label, newdelay, message);
                });
        }
    }
};
#endif