#ifndef DAG
#define DAG
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
using namespace std;

class app_dag {
private:
     int _ntasks;               //Number of tasks.
     int _nvm;                  //Number of VMs.
     int *_S;  //Software demand of each tasks, denoted by the VM's id.
     int **_B; //Number of bytes transmitted between tasks.

public:
     int ntasks();
     int nvm();
     int S(const int ) const;
     int B(const int, const int) const;
     app_dag(const char *);
     ~app_dag();
};

#endif
