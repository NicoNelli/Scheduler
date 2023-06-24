HEADERS       = include/gui.h \
                include/checkIfFileExist.h \
                include/printTextTask.h \
                include/scheduler.h \
                include/task.h \
                include/taskComparator.h
SOURCES       = src/main.cpp \
                src/gui.cpp \
                src/scheduler.cpp

RESOURCES     = scheduler.qrc

QT += widgets
requires(qtConfig(combobox))
