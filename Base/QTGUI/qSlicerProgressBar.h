#pragma once
#include <QDialog>
#include "qSlicerBaseQTGUIExport.h"
#include "ui_qSlicerProgressBar.h"

class Q_SLICER_BASE_QTGUI_EXPORT qSlicerProgressBar : public QDialog
{
    Q_OBJECT
public:
    qSlicerProgressBar(QWidget* parent = nullptr);
    ~qSlicerProgressBar();
    int steps = 1;
    Q_INVOKABLE void setText(QString txt);
    Q_INVOKABLE void setProgressValue(int v);
    Q_INVOKABLE void setValue(int v) { setProgressValue(v); }
    int getProgressValue( );
    Q_INVOKABLE void setProgressTotalValue(int t);
    Q_INVOKABLE void addProgressValue(int t);
    void setProgressMinimun(int min);
    void InitProgressBar(QString txt,int min, int max);
    Q_INVOKABLE void show();
    Q_INVOKABLE void hide();
    virtual void enterEvent(QEvent* event);
public slots:
    void onFinished(int);
private:
    void paintEvent(QPaintEvent *event);
private:
    Ui::qSlicerProgressBar ui;
};


