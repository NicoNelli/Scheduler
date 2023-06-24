#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <QDebug>

/*!
 * \brief The Task class is a generic class with a virtual method that can be overrided during inheritance.
 */
class Task {

public:

    /*!
     * \brief Task class constructor
     * \param interval task time interval
     */
    Task(int interval) : _interval(interval) {
        qDebug() << "[DEBUG] New task instance with time interval: " << QString::fromStdString(std::to_string(_interval));
    }

    /*!
     * \brief taskRoutine is a method that can be overrided in derived classes.
     */
    virtual void taskRoutine(){}

    /*!
     * \brief getInterval return interval variable
     * \return
     */
    int getInterval() const {return _interval;}

    /*!
     * \brief setInterval set interval variable
     * \param interval task interval time
     */
    void setInterval(int interval)
    {
        _interval = interval;
    }

    /*!
     * \brief _readyToBeExecuted flag that states if the task can be executed.
     */
    int _readyToBeExecuted = false;

protected:
    /*!
     * \brief _interval task time interval [s].
     */
    int _interval;

};
#endif // TASK_H
