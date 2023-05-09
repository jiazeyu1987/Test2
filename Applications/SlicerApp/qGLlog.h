#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "qSlicerAppExport.h"

class Q_SLICER_APP_EXPORT Log : public QObject
{
    Q_OBJECT
private:
    explicit Log(QObject *parent = nullptr);

public:
    static Log* instance();

signals:
   void appendAlarmWindow(QString message);

public slots:
    void appendErrorLog(QString mes);
    void appendInfoLog(QString mes);
private:
    void openFile();

private:
    QFile* infoLog;
    QTextStream* infoLogWrite;
    QFile *errorLog;
    QTextStream *errorLogWrite;
    QFile *detailLog;
    QTextStream *detailLogWrite;

    static Log *log;
    static QThread *thread;
};

#endif // LOG_H
