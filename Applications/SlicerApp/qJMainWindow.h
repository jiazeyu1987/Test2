/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qJMainWindow_h
#define __qJMainWindow_h

// Slicer includes
#include "qSlicerAppExport.h"
#include "qSlicerMainWindow.h"
#include "qJMainWindow.h"
#include "qSlicerMainWindow_p.h"
#include "qGLMessageDialog.h"
#include "qSlicerProgressBar.h"
#include "itksys/SystemTools.hxx"
#include "QDir.h"
#include "qsettings.h"
#include "qSlicerApplication.h"
#include "qSlicerModuleFactoryManager.h"
#include "qSlicerModuleManager.h"
#include "QTextCodec.h"
#include "qscreen.h"
#include "vtkPolyData.h"
#include "qMouseTracker.h"
#include "GlobalPopup.h"
#include "vtkMRMLROINode.h"
struct ThreadInfo {
    QStringList info;
    double seconds;
};

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(const QString& filename, QObject* parent = nullptr)
        : QObject(parent), m_filename(filename) {}

signals:
    void thread_info();
    void startPlayback(); // 修改信号，去掉参数

public:
    void emit_thread_info(QStringList info1, double seconds1) {
        m_Info.info = info1;
        m_Info.seconds = seconds1;
        auto threadid = QThread::currentThreadId();
        emit thread_info();
    }

public slots:
    void playback();

public:
    QString m_filename;
    ThreadInfo m_Info;
};


class qJMainWindowPrivate;
class Q_SLICER_APP_EXPORT qJMainWindow : public qSlicerMainWindow
{
	Q_OBJECT
        
private:
    int m_Page = -1;
    int m_OldPage = 1;
    int m_MaxPage = 4;
    void updateTopMenuDocker(QString name1);
    void updateMeasureDocker();
    void updateRightTopDocker(QString name1);
public:
	typedef qSlicerMainWindow Superclass;
    
	qJMainWindow(QWidget* parent = nullptr);
	~qJMainWindow() override;
    static QString ProjectName;
    static bool IsPackage;
    static QSettings* ProjectSettings;

    Q_INVOKABLE static QString GetProjectName() { return ProjectName; }
    Q_INVOKABLE static QSettings* GetProjectSettings() { return ProjectSettings; }
    static void CheckPackage() {
        QString app_root = QApplication::applicationDirPath();
        std::string parent_path = itksys::SystemTools::GetParentDirectory(app_root.toStdString().c_str());
        bool package = false;
        QString tmp_lib = QString::fromStdString(parent_path) + "/lib";
        QDir dir(tmp_lib);
        if (dir.exists())
        {
            qJMainWindow::IsPackage = true;
        }
        else {
            qJMainWindow::IsPackage = false;
        }
        qDebug() << "qJMainWindow::IsPackage :" << qJMainWindow::IsPackage;
    }
     
    static void SetProjectName(QString in_name) {
        qJMainWindow::ProjectName = in_name;
       
        
        QString config_path = GetProjectIniPath();
        qDebug() << "qJMainWindow::config_path :" << config_path;
        
        //添加忽略列表
        qSlicerModuleManager* moduleManager = qSlicerApplication::application()->moduleManager();
        qSlicerModuleFactoryManager* moduleFactoryManager = moduleManager->factoryManager();
        ProjectSettings = new QSettings(config_path,QSettings::IniFormat);
        ProjectSettings->setIniCodec(QTextCodec::codecForName("utf-8"));
        QStringList ignoreList = ProjectSettings->value("Modules/IgnoreModules").toStringList();
        qInfo() << "Add ignoreList:" << ignoreList;
        moduleFactoryManager->setModulesToIgnore(ignoreList);
        
        qInfo() << "Add Project Search Path:" << GetProjectBasePath();
        moduleFactoryManager->addSearchPath(GetProjectBasePath());
    };

    Q_INVOKABLE bool isRelease() {
#ifdef _DEBUG
        return false;
#endif // DEBUG
        return true;
    }

    static QString GetPythonBasePath() {
        QString app_root = QApplication::applicationDirPath();
        if (app_root == "") {
            throw;
            return "";
        }
        qDebug() << "app_root" << app_root;
        //D:\S521\S4RO\Slicer-build\bin
        std::string parent_path = itksys::SystemTools::GetParentDirectory(app_root.toStdString().c_str());
        
        if (!qJMainWindow::IsPackage) {
            //D:\S521\S4RO\Slicer-build
            parent_path = itksys::SystemTools::GetParentDirectory(parent_path);
            //D:\S521\S4RO
            parent_path = itksys::SystemTools::GetParentDirectory(parent_path);
            //D:\S521
            parent_path = itksys::SystemTools::GetParentDirectory(parent_path);
            //D:\S521/GLPyModule
            auto python_root = QString::fromStdString(parent_path) + "/GLPyModule";
            return python_root;
        }
        else {
            auto python_root = QString::fromStdString(parent_path) + "/GLPyModule";
            return python_root;
        }
    }
    Q_INVOKABLE static QString GetProjectBasePath() {
        return GetPythonBasePath() + "/Project/"+ qJMainWindow::ProjectName;
    }
    static QString GetProjectDBPath() { 
        qDebug() << "GetProjectBasePath:" << GetProjectBasePath();
        return GetProjectBasePath()+"/Resources/db_jl.db";
    };
    static QString GetProjectQssPath() {
        return GetProjectBasePath() + "/Resources/main.qss";
    };
    static QString GetProjectQMPath() { 
        return GetProjectBasePath() + "/Resources/app_qt_zh_CN.qm";
    };
    static QString GetProjectIniPath() {
        return GetProjectBasePath() + "/Resources/config.ini";
    }; 
    static QString GetProjectTestPath() {
        return GetProjectBasePath() + "/Test";
    };
    Q_INVOKABLE static QRect GetScreenSize() {
         QRect mRect1 = QGuiApplication::primaryScreen()->geometry();
         return mRect1;
    }

    QByteArray KEY = QByteArray::fromHex("1b5e302777346a503f633f2b443a6c52");
    void encryptFile(const QString& fileName);
    void encryptFolder(const QString& folderPath);
    void decryptFile(const QString& inputFilePath, const QString& outputFilePath);
    void decryptFolder(const QString& folderPath);
    void deleteFiles(const QDir& directory, QString sub);
	void show();
    Q_INVOKABLE QString GetMac();
    Q_INVOKABLE QString GetVolumeID();
    Q_INVOKABLE QWidget* GetCenterWidget();
    Q_INVOKABLE QWidget* GetCenterWidgetBorder();
    Q_INVOKABLE QDockWidget* GetDockWidget(QString direction);
    Q_INVOKABLE qSlicerModulePanel* GetModulePanel(QString direction);
    Q_INVOKABLE void ResetLayout();
    Q_INVOKABLE void rotate_to_vector(double dx, double dy, double dz, double dx1, double dy1, double dz1);
    QString m_GLFullPathFileName = "";
    bool close_flag = false;
    Q_INVOKABLE void SetPage(int i);
    Q_INVOKABLE void SetNextPage();
    Q_INVOKABLE void SetPreviewPage();
    Q_INVOKABLE int getOldPage() {
        return m_OldPage;
    };

    Q_INVOKABLE qSlicerProgressBar* instance = nullptr;
    Q_INVOKABLE qSlicerProgressBar* ShowProgressBar(QString title) {
        auto old_instance = instance;
        QWidget* parent = dynamic_cast<QWidget*>(this);
        instance = new qSlicerProgressBar(nullptr);
        instance->InitProgressBar(title, 0, 100);
        instance->show();
        instance->setProgressValue(0);
        if (old_instance) {
            old_instance->hide();
            delete old_instance;
        }
        return instance;
    }

    Q_INVOKABLE void SetProgressCounts(int val) {
        if (!instance) {
            return;
        }
        int steps = (96 - instance->getProgressValue()) / val;
        SetProgressStep(steps);
    }

    Q_INVOKABLE void SetProgressStep(int val) {
        if (!instance) {
            return;
        }
        if (val < 1) {
            val = 1;
        }
        instance->steps = val;
    }

    Q_INVOKABLE void SetProgress(int val) {
        if (!instance) {
            return;
        }
        instance->setProgressValue(val);
        if (val == 100) {
            HideProgressBar();
        }
    }

    Q_INVOKABLE void UpdateProgress() {
        if (!instance) {
            return;
        }
        if (instance->getProgressValue() + (int)(instance->steps) > 99) {
            instance->setProgressValue(99);
        }
        else {
            instance->setProgressValue(instance->getProgressValue() + (int)(instance->steps));
        }
        
    }
    
    Q_INVOKABLE void HideProgressBar() {
        if (!instance) {
            return;
        }
        instance->hide();
        delete instance;
        instance = nullptr;
    }

    Q_INVOKABLE int ShowInformation(QString title, QString message) {
        QStringList list;
        return qGLMessageDialog::ShowQuestion(title, message, MessageButtonType::BUTTON_OK_AND_CANCEL, list, true);
    }

    Q_INVOKABLE int ShowInformationReject(QString title, QString message) {
        QStringList list;
        list << QStringLiteral("同意") << QStringLiteral("拒绝") << QStringLiteral("同意");
        return qGLMessageDialog::ShowQuestion(title, message, MessageButtonType::BUTTON_OK_AND_CANCEL, list, true);
    }

    Q_INVOKABLE void ShowWarningText(QString message) {
        qGLMessageDialog::ShowWarnning(message, true);
    }

    Q_INVOKABLE void OnGlobalPopupMessage(QString message, float alpha) {
        if (m_GlobalPopup) {
            HideGlobalPopup();
        }
        m_GlobalPopup = new GlobalPopup(message,alpha);
        m_GlobalPopup->move(0, 0);
        m_GlobalPopup->show();
    }

    Q_INVOKABLE void HideGlobalPopup() {
        if (m_GlobalPopup) {
            m_GlobalPopup->hide();
            delete m_GlobalPopup;
            m_GlobalPopup = nullptr;
        }
    }

    GlobalPopup* m_GlobalPopup = nullptr;

    Q_INVOKABLE void ShowGlobalPopup(QString message,int delay=4000) {
        GlobalPopup* popup = new GlobalPopup(message,delay);
        //获取屏幕的中心点坐标
        QPoint center = QDesktopWidget().availableGeometry().center();
        //将弹窗放置在屏幕中央
        popup->move(0, 0);
        //显示弹窗
        popup->show();
    }

    Q_INVOKABLE void StartRecord(QString path="");
    Q_INVOKABLE void StartPlayback(QString path = "");
    Q_INVOKABLE bool check_verification();
    Q_INVOKABLE void set_verification(bool b);
    Q_INVOKABLE QString qJMainWindow::GetHardDriveSerialNumber();
    Q_INVOKABLE QString qJMainWindow::GetHardDriveSerialNumberPython() {
        return m_HardDriveSerialNumber;
    };
    Q_INVOKABLE QString m_HardDriveSerialNumber = "";
    Q_INVOKABLE bool check_verification_from_file(QString str1);
    Q_INVOKABLE bool RemoveDuplicateCells(vtkPolyData* input);
public slots:
	void on_HelpKeyboardShortcutsAction_triggered();
	void on_HelpBrowseTutorialsAction_triggered();
	void on_HelpDocumentationAction_triggered();
	void on_HelpQuickStartAction_triggered();
	void on_HelpGetHelpAction_triggered();
	void on_HelpUserInterfaceAction_triggered();
	void on_HelpVisitSlicerForumAction_triggered();
	void on_HelpJoinUsOnTwitterAction_triggered();
	void on_HelpSearchFeatureRequestsAction_triggered();
	void on_HelpViewLicenseAction_triggered();
	void on_HelpHowToCiteAction_triggered();
	void on_HelpSlicerPublicationsAction_triggered();
	void on_HelpAcknowledgmentsAction_triggered();

	void on_HelpReportBugOrFeatureRequestAction_triggered();
	void on_HelpAboutSlicerAppAction_triggered();
private:
       signals:
           void keyPressed(int key, Qt::KeyboardModifiers modifiers);
           void animate_test_event(QStringList fields);
public slots:
    void process_animate_test_event();
private:
    int iViableDays = 1;
    Worker* worker;
    QString VER_FOREVER = "jmainwindow_verification_forevera_1.0.1";
    QString VER_FORBID_FOREVER = "jmainwindow_verification_forbid_forever";
    QString VER_FIRST = "jmainwindow_verification_first";
    void writeRegedit(QString key, QString val);
    void playback(const QString& filename);
    QString getRegedit(QString key);
    void removeRegedit(QString key);
    
    int cal(int y, int m, int d);
protected:
	qJMainWindow(qJMainWindowPrivate* pimpl, QWidget* parent);
    int TitleWidgetGeometry[4] = { 0,0,1920,42 };
    int MeasureWidgetGeometry[4] = { 0,0,1441 - 11,34+56 };
    int FilterCenterWidgetSize[2] = { 1208,885-56 };
    int FilterBottomSize[2] = { 1208,120 };
    int FilterRightSize[2] = { 479,1038 };
    int FilterMiddleRightSize[2] = { 233,974 };

    void keyReleaseEvent(QKeyEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    int OutputRightSize[2] = { 488,1000 };
    int NavigationBarSize[2] = { 1920,40 };

    qMouseTracker* tracker;
    void closeEvent(QCloseEvent* event) override;
private:
	Q_DECLARE_PRIVATE(qJMainWindow);
	Q_DISABLE_COPY(qJMainWindow);
};

#endif
