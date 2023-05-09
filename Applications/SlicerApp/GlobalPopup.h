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
        //设置背景透明
        setAttribute(Qt::WA_TranslucentBackground, true);
        //设置无边框
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        QLabel* label = new QLabel(QStringLiteral("已截屏"), this);
        label->setStyleSheet("font-size: 48px; color: white; background-color: rgba(0, 0, 0, 0.1)");
        //设置标签居中
        label->setAlignment(Qt::AlignCenter);
        //自动适应标签大小
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFixedSize(1920, 1080);
        label->setFixedSize(1920, 1080);

        {
            //异步定时器关闭弹窗
            QTimer::singleShot(200, this, [this]() {
                this->close();
                });
        }
    }

    GlobalPopup(QString message, float alpha) {
        //设置背景透明
        setAttribute(Qt::WA_TranslucentBackground, true);
        //设置无边框
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        QLabel* label = new QLabel(message, this);
        label->setStyleSheet(QStringLiteral("font-size: 48px; color: white; background-color: rgba(0, 0, 0, %1)").arg(alpha));
        //设置标签居中
        label->setAlignment(Qt::AlignCenter);
        //自动适应标签大小
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFixedSize(1920, 1080);
        label->setFixedSize(1920, 1080);
    }

    GlobalPopup(QString message,int delay,bool async=false)
    {
        //设置背景透明
        setAttribute(Qt::WA_TranslucentBackground, true);
        //设置无边框
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        //创建标签并设置样式
        QString meg = message + QString("(%1)").arg(delay / 1000);
        QLabel* label = new QLabel(meg, this);
        label->setStyleSheet("font-size: 48px; color: white; background-color: rgba(0, 0, 0, 0.8)");
        //设置标签居中
        label->setAlignment(Qt::AlignCenter);
        //自动适应标签大小
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFixedSize(1920, 1080);
        label->setFixedSize(1920, 1080);

        {
            //异步定时器关闭弹窗
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