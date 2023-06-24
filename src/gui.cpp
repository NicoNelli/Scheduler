
#include "include/gui.h"
#include "include/printTextTask.h"
#include "include/checkIfFileExist.h"
#include "include/scheduler.h"

#include <QAction>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <iostream>

/*!
     * \brief GUI class constractor
     * \param mapOfTask unordered map containing all tasks to be scheduled
     * \param obj scheduler object
*/
GUI::GUI(std::unordered_map<std::string, Task*>& mapOfTask, Scheduler* obj)
{
    qDebug("[DEBUG] loading the application..");

    _scheduler_obj = obj;
    _mapOfTask = mapOfTask;
    createMessageGroupBox();
    createActions();
    createTrayIcon();

    trayIcon->setIcon(QIcon(":/images/bad.png"));

    connect(task1Button, &QAbstractButton::clicked, this, &GUI::task1Routine);
    connect(task2Button, &QAbstractButton::clicked, this, &GUI::task2Routine);
    /*
     * here it is possible to add other routine..
     */

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(task1Box);
    mainLayout->addWidget(task2Box);
    /*
     * future boxes can be added here..
     */

    setLayout(mainLayout);

    trayIcon->show();

    setWindowTitle(tr("Scheduler"));
    resize(400, 300);
}

/*!
 * \brief GUI::closeEvent
 * \param event Event to send in background the GUI.
 */
void GUI::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Scheduler"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

/*!
 * \brief GUI::task1Routine is the routine of task1 (additional task can be implemented accordingly).
 */
void GUI::task1Routine()
{
    static_cast<PrintTextTask*>(_mapOfTask.at("Task1"))->setTextToBePrinted(qUtf8Printable(task1Edit->toPlainText()));

    std::unique_lock<std::mutex> lk(_scheduler_obj->m);

    static_cast<PrintTextTask*>(_mapOfTask.at("Task1"))->_readyToBeExecuted = true;
    _scheduler_obj->schedulingReady = true;

    _scheduler_obj->cv.notify_one();
    lk.unlock();
}

/*!
 * \brief GUI::task1Routine is the routine of task2(additional task can be implemented accordingly).
*/
void GUI::task2Routine()
{
    static_cast<CheckIfFileExist*>(_mapOfTask.at("Task2"))->setFileName(qUtf8Printable(task2Edit->text()));

    std::unique_lock<std::mutex> lk(_scheduler_obj->m);

    _scheduler_obj->schedulingReady = true;
    static_cast<CheckIfFileExist*>(_mapOfTask.at("Task2"))->_readyToBeExecuted = true;

    _scheduler_obj->cv.notify_one();
    lk.unlock();
}

/*!
 * \brief GUI::createMessageGroupBox designs the interface of the GUI.
 */
void GUI::createMessageGroupBox()
{
    task1Box = new QGroupBox(tr("Task 1:"));
    task1Label = new QLabel(tr("Text to be printed:"));
    task1Edit = new QTextEdit;
    task1Edit->setPlainText(tr(""));

    task1Button = new QPushButton(tr("Set"));
    task1Button->setDefault(true);

    QGridLayout *Layout1 = new QGridLayout;
    Layout1->addWidget(task1Label, 1, 0);
    Layout1->addWidget(task1Edit, 2, 0, 1, 4);
    Layout1->addWidget(task1Button, 5, 2, 1, 1);
    Layout1->setColumnStretch(3, 1);
    Layout1->setRowStretch(4, 1);
    task1Box->setLayout(Layout1);


    task2Box = new QGroupBox(tr("Task 2:"));

    task2Label = new QLabel(tr("File to be checked:"));
    task2Edit = new QLineEdit(tr(""));

    task2Button = new QPushButton(tr("Set"));
    task2Button->setDefault(true);

    QGridLayout *Layout2 = new QGridLayout;

    Layout2->addWidget(task2Label, 1, 0);
    Layout2->addWidget(task2Edit, 2, 0, 2, 4);
    Layout2->addWidget(task2Button, 5, 2, 1, 1);
    Layout2->setColumnStretch(3, 1);
    Layout2->setRowStretch(4, 1);
    task2Box->setLayout(Layout2);
}

/*!
 * \brief GUI::createActions designs actions to interact with the system tray icon.
 */
void GUI::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);

    //connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(quitAction, &QAction::triggered, this, &GUI::handleQuit);

}

/*!
 * \brief GUI::handleQuit is connect with the Quit action and send a signal to the scheduler to interrupt itself.
 */
void GUI::handleQuit()
{

    // Before closing the GUI, send a signal to the scheduling thread.
    std::unique_lock<std::mutex> lk(_scheduler_obj->m);
    _scheduler_obj->interruptScheduling = true;
    _scheduler_obj->cv.notify_one();
    lk.unlock();

    qDebug("[DEBUG] closing the application..");
    QCoreApplication::quit();
}

/*!
 * \brief GUI::createTrayIcon creates the system tray icon.
 */
void GUI::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}
