#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include "taskComparator.h"

#include <iostream>
#include <unistd.h>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <unordered_map>
#include <QDebug>

typedef std::chrono::steady_clock::time_point time_point_st;

/*!
 * \brief   The Scheduler class schedules different tasks independently using a priority queue.
 *          This approach ensures that the tasks are executed based on their respective intervals,
 *          and the priority queue helps maintain the correct execution order based on the timestamps.
 */
class Scheduler{

public:
    /*!
     * \brief Scheduler class constructor.
     */
    Scheduler() {
        schedulingReady = false;
        interruptScheduling = false;
    }

    /*!
     * \brief operator = is mandatory due to the mutable mutex attribute.
     * \param o
     * \return
     */
    Scheduler& operator=(const Scheduler& o)
    {
        if (this != &o)
        {
            std::lock(m, o.m);
            std::lock_guard<std::mutex> lhs_lk(m, std::adopt_lock);
            std::lock_guard<std::mutex> rhs_lk(o.m, std::adopt_lock);
            schedulingReady = o.schedulingReady;

        }
        return *this;
    }

    /*!
     * \brief greatestCommonDivisor computes between two numbers the greatest common divisor
     * \param a
     * \param b
     * \return
     */
    int greatestCommonDivisor(int a, int b);

    /*!
     * \brief findGreatestCommmonDivisor computes the greatest common divisor in a vector
     * \param vec
     * \return
     */
    int findGreatestCommmonDivisor(std::vector<int>& vec);

    /*!
     * \brief executeScheduling executes the scheduling based on a set of task.
     * \param mapOfT
     */
    void executeScheduling(std::unordered_map<std::string, Task*> *mapOfT);

    /*!
     * \brief schedulingReady is the flag responsible to start the scheduling. The ok is got from the GUI.
     */
    bool schedulingReady;

    /*!
     * \brief interruptScheduling is the flag responsible to stop the scheduling. The ok is got from the GUI.
     */
    bool interruptScheduling;

    /*!
     * \brief cv condition variable between scheduler and GUI thread.
     */
    std::condition_variable cv;

    /*!
     * \brief m mutex that is used between GUI and scheduler for the starting phase.
     */
    mutable std::mutex m;

private:

    /*!
     * \brief   _amountOfSleep is the greatest commmon divisor between all the task time intervals.
     *          This allows to have a reasonable sleep function to not take always busy the CPU.
     */
    int _amountOfSleep;

    /*!
     * \brief   timeIntervals vector containing all the task time intervals.
     *          It is important to compute then the greatest common divisor.
     */
    std::vector<int> timeIntervals;

    /*!
     * \brief taskQueue priority queue to implement the scheduler.
     */
    std::priority_queue<std::pair<std::string, time_point_st>, std::vector<std::pair<std::string, time_point_st>>, TaskComparator> taskQueue;


};

#endif // SCHEDULER_H
