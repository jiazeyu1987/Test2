#include <QObject>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QThread>
#include <QCursor>
#include <QWidget>
#include <qSlicerApplication.h>
#include <GlobalPopup.h>
class qMouseTracker : public QObject
{
    Q_OBJECT
public:
    QTextStream* ts = nullptr;
    QFile* file = nullptr;
    int count = 0;
    int lastX = 0, lastY = 0;
    Qt::MouseButton lastButton = Qt::NoButton;
    QEvent::Type lastType = QEvent::MouseMove;
    int lastKey = 0;
    QEvent::Type lastkeytype;
    bool needWrite = false;

    int speed = 1;
    qint64 startTime = 0;
    int linenumber = 0;
    QTextStream* logStream;
    explicit qMouseTracker(QFile* file1,QTextStream* out,QObject* parent = nullptr)
        : QObject(parent)
    {
        ts = out;
        file = file1;
       
    }

    void close() {
        if (ts) {
            file->close();
        }
    }


    bool eventFilter(QObject* watched, QEvent* event) override
    {
        needWrite = false;
        qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        if (event->type() == QEvent::MouseMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (round(QCursor::pos().x()) != lastX || round(QCursor::pos().y()) != lastY)
            {
                *ts << timestamp << ",M," << round(QCursor::pos().x()) << "," << round(QCursor::pos().y())<<"," << mouseEvent->modifiers() << "\n";
                lastX = round(QCursor::pos().x());
                lastY = round(QCursor::pos().y());
                needWrite = true;
            }
        }
        else if (event->type() == QEvent::MouseButtonPress ||
            event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::MouseButtonDblClick ||
            event->type() == QEvent::Wheel) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
           if (mouseEvent->button() != lastButton || lastType != mouseEvent->type())
            {
                *ts << timestamp << ",B," << event->type() << "," << mouseEvent->button() << "," << mouseEvent->modifiers() << "\n";
                lastButton = mouseEvent->button();
                lastType = mouseEvent->type();
                needWrite = true;
            }
        }
        else if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (true) {
                *ts << timestamp << ",K," << event->type() << "," << keyEvent->key() << "," << keyEvent->modifiers() << "\n";
                lastKey = keyEvent->key();
                lastkeytype = keyEvent->type();
                needWrite = true;
               
            }
        }
        else {
            if (event->type() == QEvent::Close) {
                auto on = watched->objectName();
                if("qSlicerMainWindowWindow" == on) {
                // 处理关闭事件
                *ts << timestamp << ",Q," << event->type() << "\n";
                }
            }
            else if (watched == reinterpret_cast<QObject*>(qSlicerApplication::application()->mainWindow()) && event->type() == QEvent::WindowStateChange) {
               
            }
        }
        if (needWrite && ++count >= 10) {
            ts->flush();
            count = 0;
        }
        return QObject::eventFilter(watched, event);
    }
};