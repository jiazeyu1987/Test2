#include "qSlicerProgressBar.h"
#include <qpainter.h>
#include "qSlicerApplication.h"
#include "qlogging.h"
#include "qtimer.h"
#include "qSlicerIOManager.h"
qSlicerProgressBar::qSlicerProgressBar(QWidget* parent)
    :QDialog(parent)
{
    ui.setupUi(this); 
    ui.label->setAlignment(Qt::AlignCenter);

    //不明白为什么这三者必须同时用，否则要么不透明，要么不能显示（或者最上面显示）
    setWindowFlags(Qt::FramelessWindowHint);          //隐藏窗口标题栏
    setWindowModality(Qt::ApplicationModal);          //阻滞主窗口
    setAttribute(Qt::WA_TranslucentBackground, false); //设置透明
    setWindowState(Qt::WindowFullScreen);
    
    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
}

qSlicerProgressBar::~qSlicerProgressBar() 
{
}

void qSlicerProgressBar::InitProgressBar(QString txt, int min, int max)
{
    this->setText(txt);
    this->setProgressMinimun(min);
    this->setProgressTotalValue(max);
}
void qSlicerProgressBar::setText(QString txt)
{
    ui.label->setText(txt);
}

void qSlicerProgressBar::hide() {
    std::cout << "hide progress bar" << std::endl;
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QDialog::hide();
}

void qSlicerProgressBar::enterEvent(QEvent* event) {
    QDialog::enterEvent(event);
}

void qSlicerProgressBar::show() {
   
    /*bool HideCursor = qSlicerApplication::application()->settings->value("boot_config/HideCursor").toString() == "2";
    if(HideCursor){
    QTimer::singleShot(50, [=]() {
        QApplication::setOverrideCursor(Qt::BlankCursor);
        qSlicerApplication::application()->processEvents();
        });
    }*/
    std::cout << "show progress bar"<<std::endl;
    if (isHidden()) { 
    QDialog::show();
    move(0, 0);
    qSlicerApplication::application()->processEvents();
    }
    else {
        setProgressTotalValue(0);
        qSlicerApplication::application()->processEvents();
    }
    
}

void qSlicerProgressBar::onFinished(int) {
    std::cout << "finish progress bar" << std::endl;
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    /*bool HideCursor = qSlicerApplication::application()->settings->value("boot_config/HideCursor").toString() == "2";
    if (HideCursor) {
        QTimer::singleShot(100, [=]() {
            QApplication::setOverrideCursor(Qt::ArrowCursor);
            qSlicerApplication::application()->processEvents();
            });
    }*/
}

void qSlicerProgressBar::setProgressValue(int v)
{
    if (v > 100) {
        v = 100;
    }
    if (v == 100) {
        hide();
    }
    ui.label_3->setText(QString::number(v, 10) + "%");
    ui.progressBar->setValue(v);
    qSlicerApplication::application()->processEvents();
}



int qSlicerProgressBar::getProgressValue()
{
    return ui.progressBar->value();
}

void qSlicerProgressBar::setProgressTotalValue(int t)
{
    ui.progressBar->setMaximum(t);
}

void qSlicerProgressBar::addProgressValue(int t)
{
    int value = getProgressValue();
    int last_value = t + value;
    if (last_value > 99) {
        last_value = 99;
    }
    ui.progressBar->setValue(last_value);
}

void qSlicerProgressBar::setProgressMinimun(int min)
{
    ui.progressBar->setMinimum(min);
}

void qSlicerProgressBar::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, 180));
}
