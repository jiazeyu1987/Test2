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

#include "qJMainWindow.h"

// Qt includes
#include <QDesktopServices>
#include <QLabel>
#include <QPixmap>
#include <QStyle>
#include <QUrl>

// Slicer includes
#include "qSlicerAboutDialog.h"
#include "qSlicerAbstractModule.h"
#include "qSlicerActionsDialog.h"
#include "qSlicerApplication.h"
#include "qSlicerErrorReportDialog.h"
#include "qSlicerModuleManager.h"
#include "vtkSlicerVersionConfigure.h" // For Slicer_VERSION_MAJOR,Slicer_VERSION_MINOR
#include "qSlicerModuleSelectorToolBar.h"
#include "qtimer.h"
#include "QtPlatformHeaders/QWindowsWindowFunctions.h"
#include "QFile.h"
#include "QTranslator.h"
#include "QScreen.h"
#include "QMessageBox.h"
#include "qSlicerModuleFactoryManager.h"
#include <QRandomGenerator>
// Slicer includes
#include "qSlicerAbstractModuleWidget.h"
#include "vtkMatrix4x4.h"
#include "vtkTransform.h"
#include "qSlicerApplication.h"
#include "qMRMLViewControllerBar.h"
#include "qSlicerLayoutManager.h"
#include "vtkMRMLSliceNode.h"
#include "qMRMLSliceWidget.h"
#include "qMRMLSliceControllerWidget.h"
#include "ctkPythonConsole.h"
#include "QSettings.h"
#include <PythonQt.h>
#include <chrono>
#include "vtkSlicerApplicationLogic.h"
#include "QFontDatabase.h"
#include "QNetworkInterface.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "qtest.h"
#include <QAbstractEventDispatcher>
class qSlicerModuleSelectorToolBar;
class qJMainWindowPrivate;
class qSlicerLayoutManager;
QString qJMainWindow::ProjectName = "";
bool qJMainWindow::IsPackage = false;
QSettings* qJMainWindow::ProjectSettings = nullptr;
#ifndef __qJMainWindow_p_h
#define __qJMainWindow_p_h



//-----------------------------------------------------------------------------
class Q_SLICER_APP_EXPORT qJMainWindowPrivate
    : public qSlicerMainWindowPrivate
{
    Q_DECLARE_PUBLIC(qJMainWindow);
public:
    typedef qSlicerMainWindowPrivate Superclass;
    qJMainWindowPrivate(qJMainWindow& object);
    ~qJMainWindowPrivate() override;
    void AddRightDocker();
    void AddBottomDocker();
    void AddTopRight();
    void init() override;
    /// Reimplemented for custom behavior
    void setupUi(QMainWindow* mainWindow) override;
    void resetLayout();
    QDockWidget* PanelDockWidgetRight;
    qSlicerModulePanel* ModulePanelRight;
    QDockWidget* PanelDockWidgetBottom;
    qSlicerModulePanel* ModulePanelBottom;
    QDockWidget* PanelDockWidgetTop;
    qSlicerModulePanel* ModulePanelTop;
};

#endif

namespace
{

    //-----------------------------------------------------------------------------
    void setThemeIcon(QAction* action, const QString& name)
    {
        action->setIcon(QIcon::fromTheme(name, action->icon()));
    }

} // end of anonymous namespace

//-----------------------------------------------------------------------------
// qJMainWindowPrivate methods

qJMainWindowPrivate::qJMainWindowPrivate(qJMainWindow& object)
    : Superclass(object)
{
    
}

//-----------------------------------------------------------------------------
qJMainWindowPrivate::~qJMainWindowPrivate() = default;


void qJMainWindowPrivate::resetLayout() {
    this->PanelDockWidgetRight->close();
    this->PanelDockWidgetBottom->close();
    this->PanelDockWidgetTop->close();
    this->PanelDockWidget->close();

    PanelDockWidgetRight->setMinimumSize(0, 0);
    PanelDockWidgetRight->setMaximumSize(1000000, 1000000);
    PanelDockWidgetBottom->setMinimumSize(0, 0);
    PanelDockWidgetBottom->setMaximumSize(1000000, 1000000);
    PanelDockWidgetTop->setMinimumSize(0, 0);
    PanelDockWidgetTop->setMaximumSize(1000000, 1000000);
    PanelDockWidget->setMinimumSize(0, 0);
    PanelDockWidget->setMaximumSize(1000000, 1000000);
   
    this->CentralWidget->hide();

    this->ModulePanelRight->setModule("");
    this->ModulePanelTop->setModule("");
    this->ModulePanelBottom->setModule("");
    this->ModulePanel->setModule("");
    ModulePanelRight->setStyleSheet("background - color:rgb(0, 110, 0)");
    //PanelDockWidget->setStyleSheet("background - color:rgb(0, 0, 0)");
    PanelDockWidgetRight->setStyleSheet("background - color:rgb(110, 0, 0)");
    ModulePanel->setMinimumSize(0, 0);
    ModulePanel->setMaximumSize(1000000, 1000000);
    ModulePanelRight->setMinimumSize(0, 0);
    ModulePanelRight->setMaximumSize(1000000, 1000000);
    ModulePanelBottom->setMinimumSize(0, 0);
    ModulePanelBottom->setMaximumSize(1000000, 1000000);
    ModulePanelTop->setMinimumSize(0, 0);
    ModulePanelTop->setMaximumSize(1000000, 1000000);

    this->OutputBottom->setModule("");
    this->left_dicom->setModule("");
    this->TopFilterWidget->setModule("");

    this->CentralWidget->hide();
    this->CentralWidget1->hide();
    this->OutputBottom->hide();
    this->left_dicom->hide();
    this->TopFilterWidget->hide();

    
}

void qJMainWindowPrivate::AddRightDocker() {
    Q_Q(qSlicerMainWindow);
    QWidget* dockWidgetContents_4;
    QVBoxLayout* verticalLayout_4;
    dockWidgetContents_4 = new QWidget();
    dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(dockWidgetContents_4->sizePolicy().hasHeightForWidth());
    dockWidgetContents_4->setSizePolicy(sizePolicy1);

    verticalLayout_4 = new QVBoxLayout(dockWidgetContents_4);
    verticalLayout_4->setSpacing(0);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    verticalLayout_4->setSizeConstraint(QLayout::SetNoConstraint);
    verticalLayout_4->setContentsMargins(0, 0, 0, 0);
    PanelDockWidgetRight = new QDockWidget(q);
    PanelDockWidgetRight->setObjectName(QString::fromUtf8("PanelDockWidgetRight"));
    PanelDockWidgetRight->setMinimumSize(QSize(69, 91));
    PanelDockWidgetRight->setFeatures(QDockWidget::NoDockWidgetFeatures);
    PanelDockWidgetRight->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    PanelDockWidgetRight->setWidget(dockWidgetContents_4);
    q->addDockWidget(Qt::RightDockWidgetArea, PanelDockWidgetRight);
    ModulePanelRight = new qSlicerModulePanel(dockWidgetContents_4);
    ModulePanelRight->setObjectName(QString::fromUtf8("ModulePanelRight"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(ModulePanelRight->sizePolicy().hasHeightForWidth());
    ModulePanelRight->setSizePolicy(sizePolicy);
    ModulePanelRight->setStyleSheet(QString::fromUtf8(""));
    verticalLayout_4->addWidget(ModulePanelRight);
    this->ModulePanelRight->setModuleManager(qSlicerApplication::application()->moduleManager());

    QWidget* oldTitalbar = PanelDockWidgetRight->titleBarWidget();
    QWidget* newTitalbar = new QWidget();
    PanelDockWidgetRight->setTitleBarWidget(newTitalbar);
    delete oldTitalbar;
}


void qJMainWindowPrivate::AddTopRight() {
    Q_Q(qSlicerMainWindow);
    PanelDockWidgetTop = new QDockWidget(q);
    PanelDockWidgetTop->setObjectName(QString::fromUtf8("PanelDockWidgetTop"));
    PanelDockWidgetTop->setFeatures(QDockWidget::NoDockWidgetFeatures);
    PanelDockWidgetTop->setAllowedAreas(Qt::AllDockWidgetAreas);
    QWidget* dockWidgetContents_10;
    dockWidgetContents_10 = new QWidget();
    dockWidgetContents_10->setObjectName(QString::fromUtf8("dockWidgetContents_10"));
    QVBoxLayout* verticalLayout_9;
    verticalLayout_9 = new QVBoxLayout(dockWidgetContents_10);
    verticalLayout_9->setSpacing(0);
    verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
    verticalLayout_9->setContentsMargins(0, 0, 0, 0);

    ModulePanelTop = new qSlicerModulePanel(dockWidgetContents_10);
    ModulePanelTop->setObjectName(QString::fromUtf8("ModulePanelTop"));

    verticalLayout_9->addWidget(ModulePanelTop);

    PanelDockWidgetTop->setWidget(dockWidgetContents_10);
    q->addDockWidget(Qt::TopDockWidgetArea, PanelDockWidgetTop);
    this->ModulePanelTop->setModuleManager(qSlicerApplication::application()->moduleManager());
    PanelDockWidgetTop->setStyleSheet("background-color:rgb(54,61,74)");

    QWidget* oldTitalbar = PanelDockWidgetTop->titleBarWidget();
    QWidget* newTitalbar = new QWidget();
    PanelDockWidgetTop->setTitleBarWidget(newTitalbar);
    delete oldTitalbar;
}

void qJMainWindowPrivate::AddBottomDocker() {
    Q_Q(qSlicerMainWindow);
    PanelDockWidgetBottom = new QDockWidget(q);
    PanelDockWidgetBottom->setObjectName(QString::fromUtf8("PanelDockWidgetBottom"));
    PanelDockWidgetBottom->setMinimumSize(QSize(69, 93));
    PanelDockWidgetBottom->setMaximumSize(QSize(524287, 322));
    PanelDockWidgetBottom->setFeatures(QDockWidget::NoDockWidgetFeatures);
    PanelDockWidgetBottom->setAllowedAreas(Qt::AllDockWidgetAreas);
    QWidget* dockWidgetContents_10;
    dockWidgetContents_10 = new QWidget();
    dockWidgetContents_10->setObjectName(QString::fromUtf8("dockWidgetContents_10"));
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(dockWidgetContents_10->sizePolicy().hasHeightForWidth());
    dockWidgetContents_10->setSizePolicy(sizePolicy1);
    dockWidgetContents_10->setMaximumSize(QSize(16777215, 300));
    QVBoxLayout* verticalLayout_9;
    verticalLayout_9 = new QVBoxLayout(dockWidgetContents_10);
    verticalLayout_9->setSpacing(2);
    verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
    verticalLayout_9->setContentsMargins(0, 2, 0, 0);
    
    ModulePanelBottom = new qSlicerModulePanel(dockWidgetContents_10);
    ModulePanelBottom->setObjectName(QString::fromUtf8("ModulePanelBottom"));
    ModulePanelBottom->setMinimumSize(QSize(269, 269));
    ModulePanelBottom->setMaximumSize(QSize(16777215, 16777215));
    ModulePanelBottom->setStyleSheet(QString::fromUtf8(""));

    verticalLayout_9->addWidget(ModulePanelBottom);

    PanelDockWidgetBottom->setWidget(dockWidgetContents_10);
    q->addDockWidget(Qt::BottomDockWidgetArea, PanelDockWidgetBottom);
    this->ModulePanelBottom->setModuleManager(qSlicerApplication::application()->moduleManager());

    QWidget* oldTitalbar = PanelDockWidgetBottom->titleBarWidget();
    QWidget* newTitalbar = new QWidget();
    PanelDockWidgetBottom->setTitleBarWidget(newTitalbar);
    delete oldTitalbar;
}


//-----------------------------------------------------------------------------
void qJMainWindowPrivate::init()
{
    Q_Q(qSlicerMainWindow);
    this->Superclass::init();
    this->menubar->setVisible(false);
    q->hide();
    StatusBar->hide();
}

//-----------------------------------------------------------------------------
void qJMainWindowPrivate::setupUi(QMainWindow * mainWindow)
{
    Q_Q(qJMainWindow);
    //----------------------------------------------------------------------------
    // Add actions
    //----------------------------------------------------------------------------
    QAction* helpKeyboardShortcutsAction = new QAction(mainWindow);
    helpKeyboardShortcutsAction->setObjectName("HelpKeyboardShortcutsAction");
    helpKeyboardShortcutsAction->setText(qJMainWindow::tr("&Keyboard Shortcuts Reference"));
    helpKeyboardShortcutsAction->setToolTip(qJMainWindow::tr("Raise a window that lists commonly-used keyboard shortcuts."));

    QAction* helpDocumentationAction = new QAction(mainWindow);
    helpDocumentationAction->setObjectName("HelpDocumentationAction");
    helpDocumentationAction->setText(qJMainWindow::tr("Documentation"));
    helpDocumentationAction->setShortcut(QKeySequence(qJMainWindow::tr("Ctrl+1", "Documentation")));

    QAction* helpQuickStartAction = new QAction(mainWindow);
    helpQuickStartAction->setObjectName("HelpQuickStartAction");
    helpQuickStartAction->setText(qJMainWindow::tr("Quick Start"));

    QAction* helpGetHelpAction = new QAction(mainWindow);
    helpGetHelpAction->setObjectName("HelpGetHelpAction");
    helpGetHelpAction->setText(qJMainWindow::tr("Get Help"));

    QAction* helpUserInterfaceAction = new QAction(mainWindow);
    helpUserInterfaceAction->setObjectName("HelpUserInterfaceAction");
    helpUserInterfaceAction->setText(qJMainWindow::tr("User Interface"));

    QAction* helpVisitSlicerForumAction = new QAction(mainWindow);
    helpVisitSlicerForumAction->setObjectName("HelpVisitSlicerForumAction");
    helpVisitSlicerForumAction->setText(qJMainWindow::tr("Visit the Slicer Forum"));

    QAction* helpBrowseTutorialsAction = new QAction(mainWindow);
    helpBrowseTutorialsAction->setObjectName("HelpBrowseTutorialsAction");
    helpBrowseTutorialsAction->setText(qJMainWindow::tr("Browse Tutorials"));
    helpBrowseTutorialsAction->setToolTip(qJMainWindow::tr("Raise the training pages in your favorite web browser"));

    QAction* helpJoinUsOnTwitterAction = new QAction(mainWindow);
    helpJoinUsOnTwitterAction->setObjectName("HelpJoinUsOnTwitterAction");
    helpJoinUsOnTwitterAction->setText(qJMainWindow::tr("Join Us on Twitter"));

    QAction* helpSearchFeatureRequestsAction = new QAction(mainWindow);
    helpSearchFeatureRequestsAction->setObjectName("HelpSearchFeatureRequestsAction");
    helpSearchFeatureRequestsAction->setText(qJMainWindow::tr("Search Feature Requests"));

    QAction* helpViewLicenseAction = new QAction(mainWindow);
    helpViewLicenseAction->setObjectName("HelpViewLicenseAction");
    helpViewLicenseAction->setText(qJMainWindow::tr("View License"));

    QAction* helpHowToCiteAction = new QAction(mainWindow);
    helpHowToCiteAction->setObjectName("HelpHowToCiteAction");
    helpHowToCiteAction->setText(qJMainWindow::tr("How to Cite"));

    QAction* helpSlicerPublicationsAction = new QAction(mainWindow);
    helpSlicerPublicationsAction->setObjectName("HelpSlicerPublicationsAction");
    helpSlicerPublicationsAction->setText(qJMainWindow::tr("Slicer Publications"));

    QAction* helpAcknowledgmentsAction = new QAction(mainWindow);
    helpAcknowledgmentsAction->setObjectName("HelpAcknowledgmentsAction");
    helpAcknowledgmentsAction->setText(qJMainWindow::tr("Acknowledgments"));

    QAction* helpReportBugOrFeatureRequestAction = new QAction(mainWindow);
    helpReportBugOrFeatureRequestAction->setObjectName("HelpReportBugOrFeatureRequestAction");
    helpReportBugOrFeatureRequestAction->setText(qJMainWindow::tr("Report a Bug"));
    helpReportBugOrFeatureRequestAction->setToolTip(qJMainWindow::tr("Report error or request enhancement or new feature."));

    QAction* helpAboutSlicerAppAction = new QAction(mainWindow);
    helpAboutSlicerAppAction->setObjectName("HelpAboutSlicerAppAction");
    helpAboutSlicerAppAction->setText(qJMainWindow::tr("About 3D Slicer"));
    helpAboutSlicerAppAction->setToolTip(qJMainWindow::tr("Provides a description of the Slicer effort and its support."));

    //----------------------------------------------------------------------------
    // Calling "setupUi()" after adding the actions above allows the call
    // to "QMetaObject::connectSlotsByName()" done in "setupUi()" to
    // successfully connect each slot with its corresponding action.
    this->Superclass::setupUi(mainWindow);

    //----------------------------------------------------------------------------
    // Configure
    //----------------------------------------------------------------------------
   
    
    mainWindow->setWindowIcon(QIcon(qJMainWindow::GetProjectBasePath()+"/Resources/Icons/logo.png"));


    this->HelpMenu->addAction(helpDocumentationAction);
    this->HelpMenu->addAction(helpQuickStartAction);
    this->HelpMenu->addAction(helpGetHelpAction);
    this->HelpMenu->addAction(helpUserInterfaceAction);
    this->HelpMenu->addSeparator();
    this->HelpMenu->addAction(helpKeyboardShortcutsAction);
    this->HelpMenu->addAction(helpBrowseTutorialsAction);
    this->HelpMenu->addSeparator();
    this->HelpMenu->addAction(helpVisitSlicerForumAction);
    this->HelpMenu->addAction(helpJoinUsOnTwitterAction);
    this->HelpMenu->addAction(helpSearchFeatureRequestsAction);
    this->HelpMenu->addAction(helpReportBugOrFeatureRequestAction);
    this->HelpMenu->addSeparator();
    this->HelpMenu->addAction(helpViewLicenseAction);
    this->HelpMenu->addAction(helpHowToCiteAction);
    this->HelpMenu->addAction(helpSlicerPublicationsAction);
    this->HelpMenu->addAction(helpAcknowledgmentsAction);
    this->HelpMenu->addSeparator();
    this->HelpMenu->addAction(helpAboutSlicerAppAction);

    //----------------------------------------------------------------------------
    // Icons in the menu
    //----------------------------------------------------------------------------
    // Customize QAction icons with standard pixmaps
    QIcon networkIcon = mainWindow->style()->standardIcon(QStyle::SP_DriveNetIcon);
    QIcon informationIcon = mainWindow->style()->standardIcon(QStyle::SP_MessageBoxInformation);
    QIcon questionIcon = mainWindow->style()->standardIcon(QStyle::SP_MessageBoxQuestion);

    helpAboutSlicerAppAction->setIcon(informationIcon);
    helpReportBugOrFeatureRequestAction->setIcon(questionIcon);

    setThemeIcon(helpAboutSlicerAppAction, "help-about");
    setThemeIcon(helpReportBugOrFeatureRequestAction, "tools-report-bug");

    q->removeToolBar(MainToolBar);
    MainToolBar->setParent(nullptr);
    q->removeToolBar(ModuleToolBar);
    ModuleToolBar->setParent(nullptr);
    q->removeToolBar(ViewToolBar);
    ViewToolBar->setParent(nullptr);
    q->removeToolBar(MouseModeToolBar);
    MouseModeToolBar->setParent(nullptr);
    q->removeToolBar(CaptureToolBar);
    CaptureToolBar->setParent(nullptr);
    q->removeToolBar(ViewersToolBar);
    ViewersToolBar->setParent(nullptr);
    q->removeToolBar(DialogToolBar);
    DialogToolBar->setParent(nullptr);
    //q->removeToolBar(LayoutToolBar);
    //LayoutToolBar->setParent(nullptr);
    q->removeToolBar(ModuleSelectorToolBar);
    ModuleSelectorToolBar->setParent(nullptr);

   // q->removeToolBar(UndoRedoToolBar);
    //UndoRedoToolBar->setParent(nullptr);
    AddRightDocker();
    AddTopRight();
    AddBottomDocker();

    PanelDockWidgetRight->setFeatures(PanelDockWidgetRight->features() & ~QDockWidget::DockWidgetClosable & ~QDockWidget::DockWidgetMovable & ~QDockWidget::DockWidgetFloatable);
    PanelDockWidgetBottom->setFeatures(PanelDockWidgetRight->features() & ~QDockWidget::DockWidgetClosable & ~QDockWidget::DockWidgetMovable & ~QDockWidget::DockWidgetFloatable);
    PanelDockWidgetTop->setFeatures(PanelDockWidgetRight->features() & ~QDockWidget::DockWidgetClosable & ~QDockWidget::DockWidgetMovable & ~QDockWidget::DockWidgetFloatable);
    PanelDockWidget->setFeatures(PanelDockWidgetRight->features() & ~QDockWidget::DockWidgetClosable & ~QDockWidget::DockWidgetMovable & ~QDockWidget::DockWidgetFloatable);


    OutputBottom->setModuleManager(qSlicerApplication::application()->moduleManager());
    TopFilterWidget->setModuleManager(qSlicerApplication::application()->moduleManager());
    left_dicom->setModuleManager(qSlicerApplication::application()->moduleManager());

    this->ModulePanel->setHelpAndAcknowledgmentVisible(false);
    this->ModulePanelBottom->setHelpAndAcknowledgmentVisible(false);
    this->ModulePanelTop->setHelpAndAcknowledgmentVisible(false);
    this->ModulePanelRight->setHelpAndAcknowledgmentVisible(false);
    this->OutputBottom->setHelpAndAcknowledgmentVisible(false);
    this->TopFilterWidget->setHelpAndAcknowledgmentVisible(false);
    this->left_dicom->setHelpAndAcknowledgmentVisible(false);
    this->DataProbeCollapsibleWidget->setVisible(false);
    QWidget* oldTitalbar = PanelDockWidget->titleBarWidget();
    QWidget* newTitalbar = new QWidget();
    PanelDockWidget->setTitleBarWidget(newTitalbar);
    delete oldTitalbar;
    q->pythonConsole()->hide();
}



//-----------------------------------------------------------------------------
// qJMainWindow methods

//-----------------------------------------------------------------------------
qJMainWindow::qJMainWindow(QWidget * _parent)
    : Superclass(new qJMainWindowPrivate(*this), _parent)
{
    
    Q_D(qJMainWindow);
    d->init();
    
    

    hide();
    auto app = qSlicerApplication::application();
    //加载程序QSS样式
    QString fileName = GetProjectQssPath();
    QFile styleFile(fileName);
    if (styleFile.open(QIODevice::ReadOnly)) {
        QString styleSheet(styleFile.readAll());
        app->setStyleSheet(styleSheet); 
        styleFile.close();
    }
    GetHardDriveSerialNumber();
    //@by sxm
    //汉化程序样式 
    QTranslator translator;
    QString strCN = GetProjectQMPath();
    translator.load(strCN);
    app->installTranslator(&translator);
   
    QString ttfpath = GetProjectBasePath() + "/Resources/SourceHanSansCN.ttf";
    if(itksys::SystemTools::FileExists(ttfpath.toStdString().c_str()))
    {
        int fontId = QFontDatabase::addApplicationFont(ttfpath);
        qDebug() << "fontId = " << fontId;
        QString msyh = QFontDatabase::applicationFontFamilies(fontId).at(0);
        qDebug() << "msyh = " << msyh;
        QFont font(msyh);
        QApplication::setFont(font);
    }
    
} 

void qJMainWindow::StartPlayback(QString playback_script_path) {
    auto userSettings = GetProjectSettings();
    int playback_mode = userSettings->value("Test/playback_mode", 0).toInt();
    if (playback_mode == 2) {
        auto threadid = QThread::currentThreadId();
        auto filename = playback_script_path;
        QThread* thread = new QThread();
        worker = new Worker(filename); // 传递 filename 参数
        worker->moveToThread(thread);

        QObject::connect(thread, &QThread::started, worker, &Worker::startPlayback); // 简化连接
        QObject::connect(worker, &Worker::startPlayback, worker, &Worker::playback);

        bool res = QObject::connect(worker, &Worker::thread_info, this, &qJMainWindow::process_animate_test_event, Qt::QueuedConnection);
        QObject::connect(qApp, &QCoreApplication::aboutToQuit, thread, &QThread::quit);
        QObject::connect(thread, &QThread::finished, worker, &QObject::deleteLater);
        QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
    }
}
 
 
void qJMainWindow::StartRecord(QString path) {
    auto userSettings = GetProjectSettings();
    int record_mode = userSettings->value("Test/record_mode", 0).toInt();

    QString playback_script_path = path;
    std::string playback_script_folder_str = itksys::SystemTools::GetParentDirectory(playback_script_path.toStdString().c_str());
    QString playback_script_folder = QString::fromStdString(playback_script_folder_str);
    itksys::SystemTools::MakeDirectory(playback_script_folder.toStdString().c_str());
    if(path==""){
        QString playback_script = userSettings->value("Test/playback_script", "").toString();
        itksys::SystemTools::MakeDirectory(GetProjectTestPath().toStdString().c_str());
        playback_script_folder = GetProjectTestPath() + "/scripts/";
        itksys::SystemTools::MakeDirectory(playback_script_folder.toStdString().c_str());
        playback_script_path = GetProjectTestPath() + "/scripts/" + playback_script;
    }
    if (record_mode == 2)
    {
        qDebug() << "on record mode:" << playback_script_path;
        QFile* file = new QFile(playback_script_path);
        if (file->exists()) {
            if (file->remove()) {
                qDebug() << "Log file deleted successfully.";
            }
            else {
                qDebug() << "Failed to delete log file.";
            }
        }
        else {
            qDebug() << "Log file not found.";
        }
        if (file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QTextStream* out = new QTextStream(file);
            tracker = new qMouseTracker(file, out);
            tracker->setParent(qSlicerApplication::application());
            qSlicerApplication::application()->installEventFilter(tracker);
        }
        else {
            qDebug() << "file open error";
        }
    }
}

//-----------------------------------------------------------------------------
qJMainWindow::qJMainWindow(qJMainWindowPrivate * pimpl,
    QWidget * windowParent)
    : Superclass(pimpl, windowParent)
{
    // init() is called by derived class.
    
}



//-----------------------------------------------------------------------------
qJMainWindow::~qJMainWindow() = default;

QString qJMainWindow::GetVolumeID() {
    return m_HardDriveSerialNumber;
}

// 获取MAC地址
QString qJMainWindow::GetMac()
{
    // 获取所有网络接口列表
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();

    // 遍历接口
    int nCnt = nets.count();
    QString strMacAddr = "";
    for (int i = 0; i < nCnt; i++)
    {
        // 网络接口需被激活且正在运行且不是回环地址，才可能是要找的地址
        if (nets[i].flags().testFlag(QNetworkInterface::IsUp) &&
            nets[i].flags().testFlag(QNetworkInterface::IsRunning) &&
            !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            // 该mac的ip地址不能是回环地址并且是ipv4地址
            for (int j = 0; j < nets[i].addressEntries().size(); j++)
            {
                if (nets[i].addressEntries().at(j).ip() != QHostAddress::LocalHost &&
                    nets[i].addressEntries().at(j).ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    strMacAddr = nets[i].hardwareAddress();
                }
            }
        }
    }

    return strMacAddr;
}

void qJMainWindow::show() {
    Q_D(qJMainWindow);
    
    SetPage(1);

    qDebug() << "setting window state";
    if (ProjectSettings&&ProjectSettings->value("General/full_screen").toString() == "2") {
        qWarning() <<"show full screen"<< (ProjectSettings->value("General/full_screen").toString());
        //this->setWindowFlags(Qt::FramelessWindowHint);
        //this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        this->showFullScreen();
        QWindowsWindowFunctions::setHasBorderInFullScreen(windowHandle(), true);
    }
    else {
        qDebug() << "showMaximized" <<(ProjectSettings->value("General/full_screen").toString());
        QTimer::singleShot(0, [=] {showNormal();
        showMaximized(); });
         
    }
    Superclass::show();

    if (ProjectSettings && ProjectSettings->value("General/preserved").toString() == "2") {}
    else {
        QTimer::singleShot(0, [=] {
            if (IsPackage)
            {
                qDebug() << "clear cache";
                QDir folder(qJMainWindow::GetPythonBasePath());
                deleteFiles(folder, "py");
                deleteFiles(folder, "pyc");
                deleteFiles(folder, "pyd");
            }
            });
    }
    
    
    

#ifdef TestResearch
    int rate = 0;
    QDate currentDate = QDate::currentDate();
    QDate targetDate(StartYear, StartMonth, StartDay);

    if (currentDate <= targetDate) {

    }
    else {
        int daysAfterTarget = currentDate.daysTo(targetDate);
        rate = abs(daysAfterTarget * DifferDayRate);
        if (rate > MaxResearchRate) {
            rate = MaxResearchRate;
        }
    }
    static QDateTime currentDateTime = QDateTime::currentDateTime();
    static quint32 seed;
    seed = currentDateTime.toTime_t();
    qsrand(seed);
    qsrand(QDateTime::currentDateTime().toTime_t());
    auto randomNumber = qrand() % 100;
    if(PrintTestResearch){
        qDebug() << "on start:"<<randomNumber << "," << rate;
    }
    if (randomNumber < rate){
    QTimer::singleShot(5000, [rate] {
        {
            char* test;
            test = "s";
            delete test;
        }
        });
    }
#endif
}   

void Worker::playback()
{
    auto threadid = QThread::currentThreadId();
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file.errorString();
        return;
    }
    qint64 start_time = 0;
    QTextStream in(&file);
    auto static st = std::chrono::high_resolution_clock::now();
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        qint64 timestamp = fields.at(0).toLongLong();
        if (start_time == 0) {
            start_time = timestamp;
            continue;
        }
        auto elapse = timestamp - start_time;
        QThread::usleep(elapse*1000); // Delay for 1 millisecond
        start_time = timestamp;
        auto et = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(et - st);
        double seconds = duration.count() / 1000000.0;
        st = et;
        emit_thread_info(fields, seconds);
    }

    file.close();
}


void qJMainWindow::process_animate_test_event() {
    auto threadid = QThread::currentThreadId();
    QStringList fields = worker->m_Info.info;
    QString eventType = fields.at(1);
    QPoint cursorPos = QCursor::pos(); // 获取当前鼠标位置
    QWidget* widget = qApp->widgetAt(cursorPos); // 获取鼠标当前位置下的窗口
    auto localPos = widget->mapFromGlobal(cursorPos);

    try{
        if (eventType == "M") {
            int x = fields.at(2).toInt();
            int y = fields.at(3).toInt();
            int modifier1 = fields.at(4).toInt();
            Qt::KeyboardModifier modifier = static_cast<Qt::KeyboardModifier>(modifier1);
            QCursor::setPos(x, y);
            QMouseEvent* evt = new QMouseEvent(QEvent::MouseMove,
                localPos,
                Qt::LeftButton, Qt::LeftButton, modifier);
            QCoreApplication::postEvent(widget, evt);
        }
        else if (eventType == "B") {
            int buttonType1 = fields.at(2).toInt();
            int button = fields.at(3).toInt();
            int modifier1 = fields.at(4).toInt();
            Qt::KeyboardModifier modifier = static_cast<Qt::KeyboardModifier>(modifier1);
            Qt::MouseButton mosuebutton = static_cast<Qt::MouseButton>(button);
            QEvent::Type buttonType = static_cast<QEvent::Type>(buttonType1);
            if (buttonType == QEvent::MouseButtonPress ||
                buttonType == QEvent::MouseButtonRelease ||
                buttonType == QEvent::MouseButtonDblClick
                )
            {
                QMouseEvent* evt = new QMouseEvent(buttonType,
                    localPos,
                    mosuebutton, mosuebutton, modifier);
                QCoreApplication::postEvent(widget, evt);
            }
            if (buttonType == QEvent::Wheel)
            {
                QWheelEvent* evt = new QWheelEvent(localPos,
                    button,
                    Qt::NoButton, modifier);
                QCoreApplication::postEvent(widget, evt);
            }
        }
        else if (eventType == "K") {
            int buttonType1 = fields.at(2).toInt();
            int button = fields.at(3).toInt();
            int modifier1 = fields.at(4).toInt();
            Qt::KeyboardModifier modifier = static_cast<Qt::KeyboardModifier>(modifier1);
            QEvent::Type buttonType = static_cast<QEvent::Type>(buttonType1);
            QKeyEvent* eve1 = new QKeyEvent(buttonType, button, modifier);
            QCoreApplication::postEvent(widget, (QEvent*)eve1);
        }
        else if (eventType == "Q") {
            //closeEvent(new QCloseEvent());
            need_confirm = false;
            //QTimer::singleShot(0, qApp, SLOT(closeAllWindows()));
           qSlicerApplication::application()->mrmlScene()->InvokeEvent(1331227, nullptr);
        }
    }
    catch (...) {
    
    }
}

void qJMainWindow::keyReleaseEvent(QKeyEvent* keyEvent)
{
    if (keyEvent->key() == Qt::Key_F && keyEvent->modifiers() == Qt::ControlModifier)
    {
        qDebug() <<  QEvent::KeyRelease << QEvent::KeyPress;
        GlobalPopup* popup = new GlobalPopup();
        //将弹窗放置在屏幕中央
        popup->move(0, 0);
        //显示弹窗
        popup->show();
        qSlicerApplication::application()->mrmlScene()->InvokeEvent(1331226, nullptr);
    }
    this->Superclass::keyReleaseEvent(keyEvent);
}

void qJMainWindow::keyPressEvent(QKeyEvent* event)
{
    //qDebug() << "keyPressEvent" << event->modifiers() << "," << event->key() << endl;
    //if (int(event->modifiers()) == int(Qt::AltModifier) && int(event->key()) == int(Qt::Key_F4)) {
    //    qDebug() << "keyPressEvent" << endl;
    //    event->ignore(); // 忽略掉该按键事件
    //    return;
    //}
    //else {
    //    qDebug() << "keyPressEvent" << event->modifiers() << "," << event->key() << endl;
    //}
    this->Superclass::keyPressEvent(event);
}

bool qJMainWindow::check_verification_from_file(QString str1) {
    PythonQt::init();
    PythonQtObjectPtr context = PythonQt::self()->getMainModule();
    auto res = context.evalScript(QString(
        "import slicer.util as util;widget = util.getModuleWidget('%1'); widget.deshacoc(%2);").arg("JMeasure").arg(str1));
    return res.toBool();
}

void qJMainWindow::set_verification(bool b) {
    if (b) {
        writeRegedit(VER_FORBID_FOREVER, "0");
        writeRegedit(VER_FOREVER, "1");
    }
    else {
        writeRegedit(VER_FORBID_FOREVER, "1");
        writeRegedit(VER_FOREVER, "0");
    }
}

QString qJMainWindow::GetHardDriveSerialNumber()
{
    char volumeName[MAX_PATH + 1] = { 0 };
    char fileSystemName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    DWORD maxComponentLen = 0;
    DWORD fileSystemFlags = 0;

    GetVolumeInformationA("C:\\", volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName));

    char szSerialNumber[100];
    sprintf_s(szSerialNumber, "%X", serialNumber);
    
    m_HardDriveSerialNumber = QString::fromLatin1(szSerialNumber);
    qDebug() << "m_HardDriveSerialNumber" << m_HardDriveSerialNumber;
    return QString::fromLatin1(szSerialNumber);
}

bool qJMainWindow::check_verification() {
    
    //检查是否有永久权限
    QString ver_forever = getRegedit(VER_FOREVER);
    if (ver_forever == "1") {
        return true;
    }
    else {
        return false;
    }
    QString ver_forbid_forever = getRegedit(VER_FORBID_FOREVER);
    if (ver_forbid_forever == "1") {
        /*auto res = qGLMessageDialog::ShowMessageBox(QStringLiteral(" 警告信息"), QStringLiteral("软件试用已结束，请联系供应商注册！"), MessageType::WARNNING, MessageButtonType::BUTTON_OK, true);
        QTimer::singleShot(100, [=]() {
            qSlicerApplication::quit();
            });*/
        return false;
    }

    //没有永久权限,检查是否是第一次启动
    QDateTime currentime = QDateTime::currentDateTime();
    QString timeNet = currentime.toString("yyyy-MM-dd hh:mm:ss");
    int iYear, iMonth, iDay;
    QStringList temp = timeNet.split(" ").at(0).split("-");
    iYear = temp.at(0).toInt();
    iMonth = temp.at(1).toInt();
    iDay = temp.at(2).toInt();

    QString ver_first = getRegedit(VER_FIRST);
    if (ver_first == "") {
        QStringList dateList;
        dateList << QString::number(iYear) << QString::number(iMonth) << QString::number(iDay);
        QString date = dateList.join(".");
        writeRegedit(VER_FIRST, date);
    }
    else {
        QStringList temp = ver_first.split(".");
        int iyearBefor = temp.at(0).toInt();
        int imonthBefor = temp.at(1).toInt();
        int idayBefor = temp.at(2).toInt();
        int iDateBefore = cal(iyearBefor, imonthBefor, idayBefor);
        int iDateAfter = cal(iYear, iMonth, iDay);
        //检测时间是否被修改
        if (iDateBefore > iDateAfter)
        {
            QMessageBox::critical(0, QStringLiteral("提示"), QStringLiteral("检测到系统时间被改变！"));
            writeRegedit(VER_FORBID_FOREVER, "1");
            //check_verification();
            return false;
        }

        //次数更新
        int iGap = iDateAfter - iDateBefore;
        iViableDays = iViableDays - iGap;
        if (iViableDays <= 0)
        {
            writeRegedit(VER_FORBID_FOREVER, "1");
            return false;
        }
    }
}

int qJMainWindow::cal(int y, int m, int d)
{
    int mon[12] = { 0,31,59,90,120,151,181,212,243,273,304,334 };
    return y * 365 + y / 4 - y / 100 + y / 400 +
        mon[m - 1] + d - 1 +
        (((y % 100 != 0 && y % 4 == 0) || y % 400 == 0) && m > 2);
}


//---------------------------------------------------------------------------
void qJMainWindow::on_HelpKeyboardShortcutsAction_triggered()
{
    qSlicerActionsDialog actionsDialog(this);
    actionsDialog.setActionsWithNoShortcutVisible(false);
    actionsDialog.setMenuActionsVisible(false);
    actionsDialog.addActions(this->findChildren<QAction*>(), "Slicer Application");

    // scan the modules for their actions
    QList<QAction*> moduleActions;
    qSlicerModuleManager* moduleManager = qSlicerApplication::application()->moduleManager();
    foreach(const QString & moduleName, moduleManager->modulesNames())
    {
        qSlicerAbstractModule* module =
            qobject_cast<qSlicerAbstractModule*>(moduleManager->module(moduleName));
        if (module)
        {
            moduleActions << module->action();
        }
    }
    if (moduleActions.size())
    {
        actionsDialog.addActions(moduleActions, "Modules");
    }
    // TODO add more actions
    actionsDialog.exec();
}

void qJMainWindow::SetNextPage() {
    if (m_Page + 1 > m_MaxPage) {
        return;
    }
    SetPage(m_Page + 1);
}

void qJMainWindow::SetPreviewPage() {
    if (m_Page - 1 < 0 ) {
        return;
    }
    SetPage(m_Page - 1);
}

void qJMainWindow::ResetLayout() {
    Q_D(qJMainWindow);
    d->resetLayout();
}

QWidget* qJMainWindow::GetCenterWidget() {
    Q_D(qJMainWindow);
    return d->CentralWidget;
}

QWidget* qJMainWindow::GetCenterWidgetBorder() {
    Q_D(qJMainWindow);
    return d->CentralWidget1;
}

QDockWidget* qJMainWindow::GetDockWidget(QString direction) {
    Q_D(qJMainWindow);
    if (direction == "left") {
        return d->PanelDockWidget;
    }
    else if (direction == "right") {
        return d->PanelDockWidgetRight;
    }
    else if (direction == "top") {
        return d->PanelDockWidgetTop;
    }
    else if (direction == "bottom") {
        return d->PanelDockWidgetBottom;
    }
    
    return nullptr;
}

void qJMainWindow::closeEvent(QCloseEvent* event)
{
    Superclass::closeEvent(event);
    //tracker->close();
}
qSlicerModulePanel* qJMainWindow::GetModulePanel(QString direction) {
    Q_D(qJMainWindow);
    if (direction == "left") {
        return d->ModulePanel;
    }
    else if (direction == "right") {
        return d->ModulePanelRight;
    }
    else if (direction == "top") {
        return d->ModulePanelTop;
    }
    else if (direction == "bottom") {
        return d->ModulePanelBottom;
    }
    else if (direction == "middle_right") {
        return d->left_dicom;
    }
    else if (direction == "middle_top") {
        return d->TopFilterWidget;
    }
    else if (direction == "middle_bottom") {
        return d->OutputBottom;
    }
    return nullptr;
}

void qJMainWindow::SetPage(int i) {
    if (m_Page == i) {
        return;
    }
    m_OldPage = m_Page;
    m_Page = i;
    PythonQt::init();
    PythonQtObjectPtr context = PythonQt::self()->getMainModule();
    context.evalScript(QString(
        "import slicer.util as util;widget = util.getModuleWidget('%1'); widget.SetPage(%2);").arg(ProjectName).arg(i));
    return;
}

void qJMainWindow::updateRightTopDocker(QString name1) 
{
    Q_D(qJMainWindow);
    if (m_Page == 2) {
        int height = FilterRightSize[1];
        int width = FilterRightSize[0];
        d->ModulePanelRight->setModule("");
        d->ModulePanelRight->setModule(name1);
        d->ModulePanelRight->setMaximumSize(FilterRightSize[0], FilterRightSize[1]);
        d->ModulePanelRight->setMinimumSize(FilterRightSize[0], FilterRightSize[1]);
        d->PanelDockWidgetRight->show();
        d->ModulePanelRight->show();
        return;
    }
    d->ModulePanelRight->setModule("");
    d->ModulePanelRight->setModule(name1);
    //d->PanelDockWidgetRight->setGeometry(1432, NavigationBarSize[1], FilterRightSize[0], FilterRightSize[1]);
    d->ModulePanelRight->setFixedSize(FilterRightSize[0], FilterRightSize[1]);
    d->PanelDockWidgetRight->show();
    d->ModulePanelRight->show();
}


void qJMainWindow::updateMeasureDocker() 
{
    Q_D(qJMainWindow);
    d->CentralWidget->show();
    d->CentralWidget1->show();
    d->TopFilterWidget->setGeometry(0,
        0,
        MeasureWidgetGeometry[2],
        MeasureWidgetGeometry[3]);
    d->TopFilterWidget->setModule("Measure");
    d->TopFilterWidget->show();
    d->TopFilterWidget->setStyleSheet("background-color:rgb(0,0,0)");

}

void qJMainWindow::updateTopMenuDocker(QString name1)
{
    Q_D(qJMainWindow);
    d->ModulePanelTop->setModule(name1);
    d->PanelDockWidgetTop->setGeometry(TitleWidgetGeometry[0], TitleWidgetGeometry[1], TitleWidgetGeometry[2], TitleWidgetGeometry[3]);
    d->PanelDockWidgetTop->setMaximumHeight(TitleWidgetGeometry[3]);
    d->ModulePanelTop->show();
    d->PanelDockWidgetTop->show();
    
    if (m_Page == 2) {
        d->CentralWidget->setGeometry(0, MeasureWidgetGeometry[3],
            FilterCenterWidgetSize[0], FilterCenterWidgetSize[1]);
        d->CentralWidget1->show();
        d->CentralWidget1->setGeometry(0, TitleWidgetGeometry[3],
            MeasureWidgetGeometry[2],
            1080 - TitleWidgetGeometry[3]);
        d->CentralWidget1->setMaximumWidth(MeasureWidgetGeometry[2]);
    }
    else {
        d->CentralWidget->show();
        d->CentralWidget->setGeometry(0, MeasureWidgetGeometry[3],
            FilterCenterWidgetSize[0] + FilterMiddleRightSize[0], FilterCenterWidgetSize[1] + FilterBottomSize[1]);
        d->CentralWidget1->show();
        d->CentralWidget1->setGeometry(0, TitleWidgetGeometry[3],
            MeasureWidgetGeometry[2],
            1080 - TitleWidgetGeometry[3]);
        d->CentralWidget1->setMaximumWidth(MeasureWidgetGeometry[2]);
    }
    return;
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpBrowseTutorialsAction_triggered()
{
    QDesktopServices::openUrl(QUrl(qSlicerApplication::application()->documentationBaseUrl() + "/user_guide/getting_started.html#tutorials"));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpDocumentationAction_triggered()
{
    QDesktopServices::openUrl(QUrl(qSlicerApplication::application()->documentationBaseUrl()));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpQuickStartAction_triggered()
{
    QDesktopServices::openUrl(QUrl(qSlicerApplication::application()->documentationBaseUrl() + "/user_guide/getting_started.html#quick-start"));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpGetHelpAction_triggered()
{
    QDesktopServices::openUrl(QUrl(qSlicerApplication::application()->documentationBaseUrl() + "/user_guide/get_help.html"));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpUserInterfaceAction_triggered()
{
    QDesktopServices::openUrl(QUrl(qSlicerApplication::application()->documentationBaseUrl() + "/user_guide/user_interface.html"));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpVisitSlicerForumAction_triggered()
{
    QDesktopServices::openUrl(QUrl("https://discourse.slicer.org/"));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpJoinUsOnTwitterAction_triggered()
{
    QDesktopServices::openUrl(QUrl("https://twitter.com/3dslicerapp"));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpSearchFeatureRequestsAction_triggered()
{
    QDesktopServices::openUrl(QUrl("https://discourse.slicer.org/c/support/feature-requests/9"));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpViewLicenseAction_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Slicer/Slicer/blob/main/License.txt"));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpHowToCiteAction_triggered()
{
    QDesktopServices::openUrl(QUrl(qSlicerApplication::application()->documentationBaseUrl() + "/user_guide/about.html#how-to-cite"));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpSlicerPublicationsAction_triggered()
{
    QDesktopServices::openUrl(QUrl("https://scholar.google.com/scholar?&as_sdt=1%2C22&as_vis=1&q=%28%223D+Slicer%22+OR+%22slicer+org%22+OR+Slicer3D%29+-Slic3r+&btnG="));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpAcknowledgmentsAction_triggered()
{
    QDesktopServices::openUrl(QUrl(qSlicerApplication::application()->documentationBaseUrl() + "/user_guide/about.html#acknowledgments"));
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpAboutSlicerAppAction_triggered()
{
    qSlicerAboutDialog about(this);
    about.setLogo(QPixmap(":/Logo.png"));
    about.exec();
}

//---------------------------------------------------------------------------
void qJMainWindow::on_HelpReportBugOrFeatureRequestAction_triggered()
{
    qSlicerErrorReportDialog errorReport(this);
    errorReport.exec();
}

void qJMainWindow::rotate_to_vector(double dx0, double dy0, double dz0, double dx1, double dy1, double dz1) {
    double m_PointInput[3];
    m_PointInput[0] = dx0;
    m_PointInput[1] = dy0;
    m_PointInput[2] = dz0;

    double m_PointTarget[3];
    m_PointTarget[0] = dx1;
    m_PointTarget[1] = dy1;
    m_PointTarget[2] = dz1;

    double dx = m_PointTarget[0];
    double dy = m_PointTarget[1];
    double dz = m_PointTarget[2];



    double vector1[3];
    vector1[0] = m_PointInput[0] - m_PointTarget[0];
    vector1[1] = m_PointInput[1] - m_PointTarget[1];
    vector1[2] = m_PointInput[2] - m_PointTarget[2];
    double vector0[3];
    vector0[0] = 0;
    vector0[1] = 0;
    vector0[2] = 1;
    double rotationVector_Local[3] = { 0.0 };
    double angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(vector0, vector1));
    vtkMath::Cross(vector0, vector1, rotationVector_Local);
    vtkNew<vtkMatrix4x4> modelToParentTransform;
    modelToParentTransform->Identity();
    modelToParentTransform->SetElement(0, 3, m_PointTarget[0]);
    modelToParentTransform->SetElement(1, 3, m_PointTarget[1]);
    modelToParentTransform->SetElement(2, 3, m_PointTarget[2]);
    vtkNew<vtkTransform> handleToWorldMatrix;
    handleToWorldMatrix->PostMultiply();
    handleToWorldMatrix->Concatenate(modelToParentTransform);
    handleToWorldMatrix->Translate(-m_PointTarget[0], -m_PointTarget[1], -m_PointTarget[2]);
    handleToWorldMatrix->RotateWXYZ(angle, rotationVector_Local);
    handleToWorldMatrix->Translate(m_PointTarget);
    modelToParentTransform->DeepCopy(handleToWorldMatrix->GetMatrix());


    typedef std::map<vtkSmartPointer<vtkMRMLNode>, qMRMLViewControllerBar* > ControllerMapType;
    ControllerMapType::iterator cit;
    auto lm = qSlicerApplication::application()->layoutManager();
    auto sliceNames = lm->sliceViewNames();
    for (int i = 0; i < sliceNames.size(); i++) {
        vtkMRMLSliceNode* sliceNode = lm->sliceWidget(sliceNames.at(i))->mrmlSliceNode();
        vtkNew<vtkMatrix4x4> ijkToRAS;
        sliceNode->SetOrientationToDefault();
        vtkMatrix4x4* sliceToRAS = sliceNode->GetSliceToRAS();
        sliceToRAS->Multiply4x4(modelToParentTransform, sliceToRAS, sliceToRAS);
        sliceNode->GetSliceToRAS()->DeepCopy(sliceToRAS);
        sliceNode->UpdateMatrices();
    }

    for (int i = 0; i < sliceNames.size(); i++) {
        auto controller = lm->sliceWidget(sliceNames.at(i))->sliceController();
        auto node = lm->sliceWidget(sliceNames.at(i))->mrmlSliceNode();
        controller->fitSliceToBackground();
    }
    lm->resetThreeDViews();
    vtkMRMLSliceNode::JumpAllSlices(qSlicerApplication::application()->mrmlScene(), dx, dy, dz, 1);
}

void qJMainWindow::writeRegedit(QString key, QString val)
{
    QSettings Setting(QSettings::NativeFormat, QSettings::SystemScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    Setting.setValue(key, val);
}

QString qJMainWindow::getRegedit(QString key)
{
    QSettings Setting(QSettings::NativeFormat, QSettings::SystemScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    QString val = Setting.value(key).toString();
    return val;
}

void qJMainWindow::removeRegedit(QString key)
{
    QSettings Setting(QSettings::NativeFormat, QSettings::SystemScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    Setting.remove(key);
}

bool qJMainWindow::RemoveDuplicateCells(vtkPolyData* input)
{
    //vtkNew<vtkPolyData> output;
    //if (input->GetNumberOfPolys() == 0)
    //{
    //    // set up a polyData with same data arrays as input, but
    //    // no points, polys or data.
    //    output->ShallowCopy(input);
    //    return true;
    //}

    //// Copy over the original points. Assume there are no degenerate points.
    //output->SetPoints(input->GetPoints());

    //// Remove duplicate polys.
    //std::map<std::set<int>, vtkIdType> polySet;
    //std::map<std::set<int>, vtkIdType>::iterator polyIter;

    //// Now copy the polys.
    //vtkNew<vtkIdList> polyPoints;
    //const vtkIdType numberOfPolys = input->GetNumberOfPolys();
    //vtkIdType progressStep = numberOfPolys / 100;
    //if (progressStep == 0)
    //{
    //    progressStep = 1;
    //}

    //output->Allocate(input->GetNumberOfCells());
    //int ndup = 0;

    //output->GetPointData()->PassData(input->GetPointData());
    //output->GetCellData()->CopyAllocate(input->GetCellData(), numberOfPolys);

    //for (int id = 0; id < numberOfPolys; ++id)
    //{
    //    // duplicate points do not make poly vertices or triangles
    //    // strips degenerate so don't remove them
    //    int polyType = input->GetCellType(id);
    //    if (polyType == VTK_POLY_VERTEX || polyType == VTK_TRIANGLE_STRIP)
    //    {
    //        input->GetCellPoints(id, polyPoints);
    //        vtkIdType newId = output->InsertNextCell(polyType, polyPoints);
    //        output->GetCellData()->CopyData(input->GetCellData(), id, newId);
    //        continue;
    //    }

    //    input->GetCellPoints(id, polyPoints);
    //    std::set<int> nn;
    //    std::vector<int> ptIds;
    //    for (int i = 0; i < polyPoints->GetNumberOfIds(); ++i)
    //    {
    //        int polyPtId = polyPoints->GetId(i);
    //        nn.insert(polyPtId);
    //        ptIds.push_back(polyPtId);
    //    }

    //    // this conditional may generate non-referenced nodes
    //    polyIter = polySet.find(nn);

    //    // only copy a cell to the output if it is neither degenerate nor duplicate
    //    if (nn.size() == static_cast<unsigned int>(polyPoints->GetNumberOfIds()) &&
    //        polyIter == polySet.end())
    //    {
    //        vtkIdType newId = output->InsertNextCell(input->GetCellType(id), polyPoints);
    //        output->GetCellData()->CopyData(input->GetCellData(), id, newId);
    //        polySet[nn] = newId;
    //    }
    //    else if (polyIter != polySet.end())
    //    {
    //        ++ndup; // cell has duplicate(s)
    //    }
    //}

    //if (ndup)
    //{
    //    vtkDebugMacro(<< "vtkRemoveDuplicatePolys : " << ndup
    //        << " duplicate polys (multiple instances of a polygon) have been"
    //        << " removed." << endl);

    //    output->Squeeze();
    //}
    //input->DeepCopy(output);
    return true;
}

// 加密函数
void qJMainWindow::encryptFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open file for reading:" << fileName;
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    for (int i = 0; i < data.size(); i++)
    {
        data[i] = data[i] ^ KEY.at(i % KEY.size());
    }

    file.setFileName(fileName + ".yp");
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Failed to open file for writing:" << file.fileName();
        return;
    }

    file.write(data);
    file.close();

    if (!QFile::remove(fileName))
    {
        qWarning() << "Failed to remove original file:" << fileName;
    }
}
void qJMainWindow::encryptFolder(const QString& folderPath)
{
    QDir dir(folderPath);
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    QFileInfoList files = dir.entryInfoList();
    for (const QFileInfo& fileInfo : files)
    {
        if (fileInfo.isFile())
        {
            encryptFile(fileInfo.absoluteFilePath());
        }
        else if (fileInfo.isDir())
        {
            encryptFolder(fileInfo.absoluteFilePath());
        }
    }
}

void qJMainWindow::decryptFile(const QString& inputFilePath, const QString& outputFilePath) {
    QFile inputFile(inputFilePath);
    if (inputFile.open(QIODevice::ReadOnly)) {
        QByteArray inputData = inputFile.readAll();
        inputFile.close();

        QByteArray outputData(inputData.size(), 0);
        for (int i = 0; i < inputData.size(); i++) {
            outputData[i] = inputData[i] ^ KEY[i % KEY.size()];
        }

        QFile output(outputFilePath);
        if (output.open(QIODevice::WriteOnly)) {
            output.write(outputData);
            output.close();
        }
    }
}

void qJMainWindow::decryptFolder(const QString& folderPath)
{
    QDir folder(folderPath);
    QFileInfoList fileList = folder.entryInfoList(QDir::Files);
    for (int i = 0; i < fileList.size(); ++i) {
        const QFileInfo& fileInfo = fileList.at(i);
        if (fileInfo.suffix() == "yp") {
            QString inputFilePath = fileInfo.absoluteFilePath();
            QString outputFilePath = fileInfo.absoluteFilePath().left(fileInfo.absoluteFilePath().length() - 3) + ".py";
            decryptFile(inputFilePath, outputFilePath);
        }
    }

    QFileInfoList folderList = folder.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folderList.size(); ++i) {
        const QFileInfo& folderInfo = folderList.at(i);
        decryptFolder(folderInfo.absoluteFilePath());
    }
}

void qJMainWindow::deleteFiles(const QDir& directory,QString sub)
{

    // 获取目录下所有文件和子目录
    QFileInfoList files = directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    // 遍历文件和目录
    for (const QFileInfo& file : files)
    {
        if (file.isDir()) // 如果是目录，递归删除目录下的.py文件
        {
            QString abspath = file.absoluteFilePath();
            if (abspath.contains("DExtension")) {
                return;
            }
            else {
                QDir subDir(abspath);
                deleteFiles(subDir, sub);
            }
            
        }
       
        else if (file.suffix() == sub) // 如果是.py文件，删除之
        {
            qDebug() << "remove file:" << file.absoluteFilePath();
            QFile::remove(file.absoluteFilePath());
        }
    }
}