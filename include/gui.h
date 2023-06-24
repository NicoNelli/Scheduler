#ifndef GUI_H
#define GUI_H

#include "task.h"
#include "scheduler.h"

#include <QSystemTrayIcon>
#include <QDialog>
#include <unordered_map>

QT_BEGIN_NAMESPACE
class QAction;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

/*!
 * \brief The GUI class adopt qt library to provide an easy interface for scheduling different tasks.
 */
class GUI : public QDialog
{
    Q_OBJECT

public:
    GUI(std::unordered_map<std::string, Task*>& mapOfTask, Scheduler* obj);

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void handleQuit();

private slots:

    void task1Routine();
    void task2Routine();

private:
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();

    QGroupBox *task1Box, *task2Box;

    /*!
     * \brief task1Label object to provide text in the GUI
     */
    QLabel *task1Label;

    /*!
     * \brief task2Label object to provide text in the GUI
     */
    QLabel *task2Label;

    /*!
     * \brief task1Edit object used to store text got from GUI
     */
    QTextEdit *task1Edit;

    /*!
     * \brief task2Edit object used to store text got from GUI
     */
    QLineEdit *task2Edit;

    /*!
     * \brief task1Button set button for the GUI
     */
    QPushButton *task1Button;

    /*!
     * \brief task2Button set button for the GUI
     */
    QPushButton *task2Button;

    /*!
     * \brief minimizeAction action for the system tray icon.
     */
    QAction *minimizeAction;

    /*!
     * \brief minimizeAction action for the system tray icon.
     */
    QAction *maximizeAction;

    /*!
     * \brief minimizeAction action for the system tray icon.
     */
    QAction *restoreAction;

    /*!
     * \brief minimizeAction action for the system tray icon.
     */
    QAction *quitAction;

    /*!
     * \brief trayIcon object for the system tray icon.
     */
    QSystemTrayIcon *trayIcon;

    /*!
     * \brief trayIconMenu menu widget object
     */
    QMenu *trayIconMenu;

    /*!
     * \brief _mapOfTask contains all the task that will be executed later from the scheduler
     */
    std::unordered_map<std::string, Task*> _mapOfTask;

    /*!
     * \brief   _scheduler_obj allows syncronization with the GUI during init phase as well as to terminate the scheduler
     *          (in principle can be extended to stop also the schedule of a task during runtime).
     */
    Scheduler *_scheduler_obj;

};
#endif
