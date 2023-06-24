#ifndef PRINTTEXTTASK_H
#define PRINTTEXTTASK_H

#include "task.h"

#include <QDebug>
#include <QString>

/*!
 * \brief   The PrintTextTask class is a derived class of Task class.
 *          It overrides the taskRoutine method in order to implement its own routine task.
 */
class PrintTextTask : public Task {
public:

    /*!
     * \brief PrintTextTask class constructor
     * \param interval task time interval
     */
    PrintTextTask(int interval): Task(interval) {}

    /*!
     * \brief taskRoutine prints in debug mode a provided text.
     */
    void taskRoutine() override {qDebug() << QString::fromStdString(this->getTextToBePrinted());}

    /*!
     * \brief getTextToBePrinted return the text string
     * \return
     */
    std::string getTextToBePrinted() const { return _textToBePrinted;}

    /*!
     * \brief setTextToBePrinted set text to be printed
     * \param str string containing text
     */
    void setTextToBePrinted(const std::string& str) {_textToBePrinted = str;}

private:
    /*!
     * \brief _textToBePrinted stores text to be printed during scheduling.
     */
    std::string _textToBePrinted;

};

#endif // PRINTTEXTTASK_H
