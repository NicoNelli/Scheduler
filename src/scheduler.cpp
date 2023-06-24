#include "include/scheduler.h"

/*!
 * \brief Scheduler::greatestCommonDivisor compute the greatest common divisor between two numbers.
 * \param a
 * \param b
 * \return
 */
int Scheduler::greatestCommonDivisor(int a, int b)
{
    if (a == 0)
        return b;
    return greatestCommonDivisor(b % a, a);
}

/*!
 * \brief Scheduler::findGreatestCommmonDivisor find the greatest commmon divisor in a vector of int.
 * \param vec
 * \return
 */
int Scheduler::findGreatestCommmonDivisor(std::vector<int>& vec)
{
    int result = vec.at(0);
    for (size_t i = 1; i < vec.size(); ++i)
    {
        result = greatestCommonDivisor(vec.at(i), result);

        if(result == 1)
        {
            return 1;
        }
    }
    return result;
}

/*!
 * \brief Scheduler::executeScheduling executeScheduling is the core method of the class. It schedule the task received as input
 * \param mapOfT mapOfT contains the task name as well as the task to be executed.
 */
void Scheduler::executeScheduling(std::unordered_map<std::string, Task*> *mapOfT)
{

    // let's copy the data from the map to the priority queue and let's collect the intervals time.
    for (auto& it: *mapOfT) {
        taskQueue.emplace(it.first, std::chrono::steady_clock::now() + std::chrono::seconds(it.second->getInterval()));
        timeIntervals.emplace_back(it.second->getInterval());
    }

    // compute the greatest common divisor used later with the sleep.
    _amountOfSleep = findGreatestCommmonDivisor(timeIntervals);

    qDebug() << "[DEBUG] loop rate chosen according to the greatest common divisor between task time intervals: " << QString::fromStdString(std::to_string(_amountOfSleep));

    // here the thread is waiting green flag from the GUI
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [this]{return (schedulingReady || interruptScheduling);});

    cv.notify_all();
    lk.unlock();

    // as soon as the green light is received, the scheduler will start.
    while(schedulingReady && !interruptScheduling){

        // Get the current time
        time_point_st currentTime = std::chrono::steady_clock::now();

        // execute all the possible task.
        while (this->taskQueue.top().second <= currentTime) {

            std::pair<std::string, time_point_st> currentTask = this->taskQueue.top();

            // Execute the task if it's ready
            if(mapOfT->at(currentTask.first)->_readyToBeExecuted){
                //qDebug() << "[DEBUG] Executing task with name " << QString::fromStdString(currentTask.first);
                mapOfT->at(currentTask.first)->taskRoutine();
            }

            // Update the task's next execution timestamp
            currentTask.second = currentTime + std::chrono::seconds(mapOfT->at(currentTask.first)->getInterval());

            // Re-insert the task back into the priority queue
            taskQueue.pop();
            taskQueue.push(currentTask);
        }
        sleep(_amountOfSleep);
    }
}

