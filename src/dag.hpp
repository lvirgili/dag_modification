#ifndef DAG
#define DAG
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include "vm_info.hpp"
using namespace std;

class app_dag {
private:
     int _ntasks;               //Number of tasks.
     int _nvm;                  //Number of VMs.
     int *_I;                   //Weight of each task.
     int *_S;                   //Software demand of each tasks, denoted by the VM's id.
     int **_B;                  //Number of bytes transmitted between tasks.
     vector<vector<int> > _paths;
     map<pair<int,int>, vector<int> > _P;
     vm_info *vinfo;
     int _max_vm;               //The max id of the VMs.

     map<pair<int,int>, vector<int> >::iterator find_heavy();
     void gen_paths();
     void find_path(int cur_vertex, vector<int>& curr);
     void gen_Phs();
     int find_weight(map<pair<int,int>, vector<int> >::iterator it);
     void remove_tasks(map<pair<int,int>, vector<int> >::iterator cur_it);
     void printDAG(const char *, int, vector<vector<int> >, vector<int>, vector<int>);

public:
     app_dag(const char *, const char *);
     ~app_dag();
     void dagmdf_path(const char *);
     void dagmdf_path();
     void dagmdf_oneeach(const char *);
     void dagmdf_oneeach();
     void dagmdf_onlyone(const char *);
     void dagmdf_onlyone();
};

#endif
