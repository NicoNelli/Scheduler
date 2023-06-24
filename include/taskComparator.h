#ifndef TASKCOMPARATOR_H
#define TASKCOMPARATOR_H

#include "task.h"

#include <chrono>

typedef std::chrono::steady_clock::time_point time_point_st;

/*!
 * \brief The TaskComparator class compares two custom pair object based on their value.
 */
class TaskComparator {

public:
    bool operator()(const std::pair<std::string, time_point_st>& t1, const std::pair<std::string, time_point_st>& t2) const {
        return t1.second > t2.second;
    }
};

#endif // TASKCOMPARATOR_H
