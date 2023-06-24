#ifndef CHECKIFFILEEXIST_H
#define CHECKIFFILEEXIST_H

#include "task.h"

#include <fstream>
#include <QDebug>
#include <QString>

/*!
 * \brief   The CheckIfFileExist class is a derived class of Task class.
 *          It overrides the taskRoutine method in order to implement its own routine task.
 */
class CheckIfFileExist : public Task {
public:

    /*!
     * \brief CheckIfFileExist class constructor
     * \param interval is the task time interval
     */
    CheckIfFileExist(int interval): Task(interval) {}

    /*!
     * \brief taskRoutine prints, in debug mode, if a given file (provided by the GUI) exist.
     */
    void taskRoutine() override {
        std::ifstream f(this->_fileName.c_str());
        if(!f.good())
        {
            qDebug() << QString("file with path: ") + QString::fromStdString(this->getFileName()) + QString(" not found...");
        }
    }

    /*!
     * \brief getFileName return the file name
     * \return
     */
    std::string getFileName() const { return _fileName;}

    /*!
     * \brief setFileName set _fileName
     * \param str string containing file name
     */
    void setFileName(const std::string& str) {_fileName = str;}

private:
    /*!
     * \brief _fileName file name to check if exist.
     */
    std::string _fileName;
};

#endif // CHECKIFFILEEXIST_H
