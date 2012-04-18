#ifndef DAG
#define DAG
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
using namespace std;

class app_dag {
private:
     int _ntasks; //Number of tasks.
     int _nvm;    //Number of VMs.
     int *_S;     //Software demand of each tasks, denoted by the VM's id.
     int **_B;    //Number of bytes transmitted between tasks.
     vector<vector<int> > _paths;

     void find_path(int cur_vertex, vector<int>& curr);

public:
     int ntasks() const;
     int nvm() const;
     int S(const int ) const;
     int B(const int, const int) const;
     app_dag(const char *);
     ~app_dag();
     void gen_paths();
};

#endif
