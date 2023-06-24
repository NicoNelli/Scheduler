# Scheduler
Scheduler implementation with qt libraries
  
## How it works:

Currently the application uses two indipendent tasks. The first one, prints every 10 seconds in debug mode a text chosen by GUI. The second task checks every 30 seconds if a specified file (chosen from GUI) exist.
It is important to note that the loop period is chosen using greatest common divisor to allow scalability changing and adding other task.
In addition to this, it is possible to interact with the GUI even if the scheduler is still running.

### Further development:

It is possible to add additional tasks; the code is scalable and designed for that purpose. In order to add a new task, the preferrable way is to create a new derived class from Task base class overriding the taskRoutine method according to new task specifications. In addition to this, the main file and the GUI interface needs to be updated accordingly.
A doxygen documentation is provided to help the reader during further development.
