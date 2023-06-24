
#include "include/task.h"
#include "include/scheduler.h"
#include "include/gui.h"
#include "include/printTextTask.h"
#include "include/checkIfFileExist.h"

#include <QApplication>
#include <QMessageBox>
#include <iostream>
#include <thread>
#include <unordered_map>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(scheduler);

    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Scheduler"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    /*!
     * \brief   mapOfTask is the map containing the task set.
     *          You just need to provide name and time interval [s].
     */
    std::unordered_map<std::string, Task*> mapOfTask;
    mapOfTask.emplace(std::make_pair("Task1", new PrintTextTask(2)));
    mapOfTask.emplace(std::make_pair("Task2", new CheckIfFileExist(6)));

    /*!
     * \brief scheduler_obj schedules all the task provided few lines above.
     */
    Scheduler scheduler_obj;

    /*!
     *  thread_obj executes the scheduling inside a dedicated thread separated from the GUI
     */
    std::thread thread_obj([a = &scheduler_obj, &mapOfTask]()
                    {
                        a->executeScheduling(&mapOfTask);
                    });

    /*!
     * \brief GUI instance of the GUI taking as input set of task and schedules object.
     */
    GUI GUI(mapOfTask, &scheduler_obj);
    GUI.show();

    int result = app.exec();

    thread_obj.join();

    return result;
}
