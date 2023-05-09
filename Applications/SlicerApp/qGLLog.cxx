#include "qGLLog.h"

#include <QDateTime>
#include <QThread>
#include <QCoreApplication>

Log *Log::log = nullptr;
QThread *Log::thread = nullptr;

Log::Log(QObject *parent) : QObject(parent)
{
    openFile();
}

Log *Log::instance()
{

    if (!log) {
        log = new Log;
    }

    if (!thread) {
        thread = new QThread;
        thread->start();
    }

    if (thread != log->thread) {
        log->moveToThread(thread);
        thread->start();
    }

    return log;
}

void Log::appendErrorLog(QString mes)
{
    *errorLogWrite << mes << "\r\n";
    errorLogWrite->flush();
}

void Log::appendInfoLog(QString mes)
{
    *infoLogWrite << mes << "\r\n";
    infoLogWrite->flush();
}

void Log::openFile()
{
  QString curPath = qApp->applicationDirPath();

  QString logName = curPath + QString("/logs/error-%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
  errorLog = new QFile(logName);
  if (!errorLog->open(QIODevice::WriteOnly | QIODevice::Append)) {
    //emit appendAlarmWindow(QString("错误日志打开失败：%1").arg(errorLog->errorString()));
  }
  errorLogWrite = new QTextStream(errorLog);

  QString fileName1 = QString("%1/logs/info%2.log").arg(curPath).arg(QDateTime::currentDateTime().toString("yyyyMMdd"));
  infoLog = new QFile(fileName1);
  if (!infoLog->open(QIODevice::WriteOnly | QIODevice::Append)) {
    //emit appendAlarmWindow(QString("info日志打开失败：%1").arg(infoLog->errorString()));
  }
  infoLogWrite = new QTextStream(infoLog);
}
