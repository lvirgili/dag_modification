#ifndef DAG
#define DAG
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <map>
using namespace std;

class app_dag {
private:
     int _ntasks; //Number of tasks.
     int _nvm;    //Number of VMs.
     int *_I;     //Weight of each task.
     int *_S;     //Software demand of each tasks, denoted by the VM's id.
     int **_B;    //Number of bytes transmitted between tasks.
     vector<vector<int> > _paths;
     map<pair<int,int>, vector<int> > _P;

     void find_path(int cur_vertex, vector<int>& curr);
     void gen_Phs();

public:
     int ntasks() const;
     int nvm() const;
     int I(const int) const;
     int S(const int) const;
     int B(const int, const int) const;
     app_dag(const char *);
     ~app_dag();
     void gen_paths();
};

#endif
