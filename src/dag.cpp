#include "dag.hpp"

//The constructor for an app_dag: Receives two filenames as
//parameters: the first is the file that contains the DAG info and the
//second constains the VMs info.
app_dag::app_dag(const char *filename, const char *vmfilename) {
     vinfo = new vm_info(vmfilename);
     ifstream f(filename);
     if (f.is_open() == true) {
          string s;
          getline(f, s, ':');
          f >> _ntasks;
          getline(f, s);
          getline(f, s, ')');
          _I = new int[_ntasks];
          for (int i = 0; i < _ntasks; ++i) {
               f >> _I[i];
          }
          getline(f, s, ')');
          set<int> aux;
          _nvm = 0;
          _S = new int[_ntasks];
          for (int i = 0; i < _ntasks; ++i) {
               f >> _S[i];
               if (aux.insert(_S[i]).second == true) {
                    ++_nvm;
               }
          }
          getline(f,s);
          _B = new int *[_ntasks];
          for (int i = 0; i < _ntasks; ++i) {
               _B[i] = new int[_ntasks];
          }
          for (int i = 0; i < _ntasks; ++i) {
               getline(f, s, ')');
               for (int j = 0; j < _ntasks; ++j) {
                    f >> _B[i][j];
               }
               getline(f,s);
          }
          f.close();
          return;
     } else {
          cout << "[APP] Error reading the file " << filename << endl;
          exit(-1);
     }
}

app_dag::~app_dag() {
     for (int i = 0; i < _ntasks; ++i) {
          delete[] _B[i];
     }
     delete[] _B;
     delete[] _S;
     delete[] _I;
     delete vinfo;
}

//This function prints the modified DAG to the file "filename".
void app_dag::printDAG(const char *filename, int ntasks, vector<vector<int> > B, vector<int> I, vector<int> S) {
     ofstream f(filename);
     if (f.is_open() == true) {
          f << "n: " << ntasks << endl;
          f << "I: [ (1) ";
          for (unsigned i = 0; i < I.size(); ++i) {
               f << I[i] << ' ';
          }
          f << "]" << endl;
          f << "S: [ (1) ";
          for (unsigned i = 0; i < S.size(); ++i) {
               f << S[i] << ' ';
          }
          f << "]" << endl;
          f << "B: [ ";
          for (unsigned i = 0; i < B.size(); ++i) {
               if (i != 0) f << "     ";
               f << "(" << i+1 << " 1) ";
               for (unsigned j = 0; j < B[i].size(); ++j) {
                    f << B[i][j] << ' ';
               }
               f << endl;
          }
          f << "   ]" << endl;
          f << "D: [ ";
          for (unsigned i = 0; i < B.size(); ++i) {
               if (i != 0) f << "     ";
               f << "(" << i+1 << " 1) ";
               for (unsigned j = 0; j < B[i].size(); ++j) {
                    if (B[i][j] != 0) {
                         f << 1 << ' ';
                    } else {
                         f << 0 << ' ';
                    }
               }
               f << endl;
          }
          f << "   ]" << endl;
          f.close();
     } else {
          cout << "Error opening the file " << filename << " to write the MDAG.\n";
          exit(-1);
     }
}

//The next two functions find the paths in the DAG from the vertex 0
//to the n-1. They do that by using a depth-first search.  Everytime
//they reach the n-1, they add the path to the vector "_paths".
void app_dag::find_path(int cur_vertex, vector<int>& curr) {
     curr.push_back(cur_vertex);
     if (cur_vertex == _ntasks-1 && curr[0] == 0) {
          _paths.push_back(curr);
          curr.clear();
          return;
     }
     for (int i = cur_vertex+1; i < _ntasks; ++i) {
          if (_B[cur_vertex][i] > 0) {
               find_path(i, curr);
          }
     }
}

void app_dag::gen_paths() {
     for (int i = 1; i < _ntasks-1; ++i) {
          vector<int> curr;
          curr.push_back(0);
          if (_B[0][i] > 0) {
               find_path(i, curr);
          }
     }
}

//This function generates the "P_{h,s}" sets, associating one path to
//its software requirement. These are kept in the "_P" map.
void app_dag::gen_Phs() {
     gen_paths();
     for (unsigned i = 0; i < _paths.size(); ++i) {
          for (unsigned j = 0; j < _paths[i].size(); ++j) {
               int vm = _S[_paths[i][j]];
               if (_P.count(make_pair(i,vm)) == false) {
                    vector<int> aux;
                    _P.insert(make_pair(make_pair(i,vm),aux));
               }
               _P[make_pair(i,vm)].push_back(_paths[i][j]);
          }
     }
}

//Calculates the weight of the P_{h,s} pointed by the iterator it. If
//thery is only one vertex in the set, it returns 1.
int app_dag::find_weight(map<pair<int,int>,vector<int> >::iterator it) {
     int weight = 0;
     vector<int> path(_paths[it->first.first]);
     for (unsigned i = 0; i < path.size()-1; ++i) {
          weight += _B[path[i]][path[i+1]];
     }
     return weight;
}

//Finds the heavier P_{h,s} set.
map<pair<int,int>, vector<int> >::iterator app_dag::find_heavy() {
     map<pair<int,int>, vector<int> >::iterator it, max_it;
     int max = 0;
     for (it = _P.begin(); it != _P.end(); ++it) {
          int cur_weight = find_weight(it);
          if (cur_weight > max) {
               max_it = it;
               max = cur_weight;
          }
     }
     return max_it;
}

//Removes the tasks in the set pointed by the iterator cur_it from the
//other sets in P. If those sets become empty, deletes them from P.
void app_dag::remove_tasks(map<pair<int,int>, vector<int> >::iterator cur_it) {
     map<pair<int,int>, vector<int> >::iterator it;
     vector<int> tasks(cur_it->second); //Tasks to be removed from the sets
     for (it = _P.begin(); it != _P.end(); ++it) {
          if (it == cur_it) {
               continue;
          }
          for (unsigned i = 0; i < tasks.size(); ++i) {
               if ((int)count(it->second.begin(), it->second.end(), tasks[i]) != 0) {
                    it->second.erase(find(it->second.begin(), it->second.end(), tasks[i]));
               }
          }
     }
     _P.erase(cur_it);
     for (it = _P.begin(); it != _P.end(); ++it) {
          if (it->second.empty() == true) {
               _P.erase(it);
          }
     }
}

//The function that does the "work".
void app_dag::dagmdf_path(const char *outfile) {
     gen_Phs(); // Creates the set P_{h,s}
     map<pair<int,int>, vector<int> >::iterator it;
     vector<int> newS, newI; //These will be the new I and S arrays.
     vector<vector<int> > newB; //This will be the new B matrix.
     int vms_added = 0; //The number of VMs that were added.

     //Creates the vertex for the repository.
     newS.push_back(0); newI.push_back(0);
     vector<int> repo(1,0); newB.push_back(repo);

     while (_P.empty() == false) {
          //Finds the heavier P.
          map<pair<int,int>, vector<int> >::iterator heavy = find_heavy();
          ++vms_added;
          newS.push_back(0); //The software requirement of a VM is null.
          newI.push_back(vinfo->TV(heavy->first.second - 1)); //Its weight is its size.
          //The arc from the repository to the VM has the value of the time to boot the VM.
          newB[0].push_back(vinfo->BV(heavy->first.second - 1));
          vector<int> vm(_ntasks, 0); newB.push_back(vm);
          //For each task in the heavier P, adds an arc from the VM to the the task with value infinite.
          for (unsigned i = 0; i < heavy->second.size(); ++i) {
               newB[vms_added][heavy->second[i]] = 0x7FFFFFFF; // Adds an arc from the vm to the task i with weight infinite.
          }
          //Finally, deletes the tasks in the the heavier P from the other sets.
          remove_tasks(heavy);
     }

     //Appends the tasks info to the added data for printing.
     for (int i = 0; i < _ntasks; ++i) {
          newB[0].push_back(0);
          newI.push_back(_I[i]);
          newS.push_back(_S[i]);
     }
     for (unsigned i = 1; i < newB.size(); i++) {
          vector<int> aux(vms_added+1, 0);
          newB[i].insert(newB[i].begin(), aux.begin(), aux.end());
     }
     for (int i = 0; i < _ntasks; ++i) {
          vector<int> task(vms_added+1, 0);
          for (int j = 0; j < _ntasks; ++j) {
               task.push_back(_B[i][j]);
          }
          newB.push_back(task);
     }
     int new_ntasks = _ntasks + vms_added + 1; //This is needed, or the destructor for the app_dag seg faults.

     //Some tests to see if the generated DAG is within reason.
     if (new_ntasks < (_ntasks + _paths.size())) {
          //This one checks if there is at least the repository was added.
          cout << "[ASSERTIONS] DAG has too few added vertices." << endl;
     }
     if (new_ntasks > (1 + 2*_ntasks)) {
          //This one checks if more than _ntasks VMs were added, which
          //is more than the maximum (one for each VM).
          cout << "[ASSERTIONS] DAG has too many added vertices." << endl;
     }
     //These next lines check to see if each task has its own VM.
     // set<int> reached;
     // pair<set<int>::iterator, bool> p;
     // for (int i = 1; i <= vms_added; ++i) {
     //      for (unsigned task = 0; task < newB[i].size(); ++task) {
     //           if (newB[i][task] != 0) {
     //                p = reached.insert(task);
     //                cout << task << endl;
     //                if (p.second == false) {
     //                     cout << "[ASSERTIONS] Task " << task << " has more than one VM." << endl;
     //                }
     //           }
     //      }
     // }
     // if (reached.size() != (unsigned)_ntasks) {
     //      cout << "[ASSERTIONS] There is a task that has no adjacent VM." << endl;
     //      cout << reached.size() << ' ' << _ntasks << endl;
     // }

     //Prints the modfied DAG to "outfile."
     printDAG(outfile, new_ntasks, newB, newI, newS);
}

void app_dag::dagmdf_path() {
     dagmdf_path("/tmp/mdf_dag.dat");
}

void app_dag::dagmdf_oneeach(const char *outfile) {
     vector<int> newS, newI; //These will be the new I and S arrays.
     vector<vector<int> > newB; //This will be the new B matrix.

     //Creates the vertex for the repository.
     newS.push_back(0); newI.push_back(0);
     vector<int> repo(1,0); newB.push_back(repo);

     for (int i = 0; i < _ntasks; ++i) {
          newS.push_back(0);
          newI.push_back(vinfo->TV(_S[i] - 1));
          newB[0].push_back(vinfo->BV(_S[i] - 1));
          vector<int> vm(2 * _ntasks + 1, 0);
          vm[i + _ntasks + 1] = 0x7FFFFFFF;
          newB.push_back(vm);
     }

     //Appends the tasks info to the added data for printing.
     for (int i = 0; i < _ntasks; ++i) {
          newB[0].push_back(0);
          newI.push_back(_I[i]);
          newS.push_back(_S[i]);
     }

     for (int i = 0; i < _ntasks; ++i) {
          vector<int> task(2 * _ntasks + 1, 0);
          for (int j = 0; j < _ntasks; ++j) {
               task[j + _ntasks + 1] = (_B[i][j]);
          }
          newB.push_back(task);
     }

     int new_ntasks = 2 * _ntasks + 1;

     printDAG(outfile, new_ntasks, newB, newI, newS);

}

void app_dag::dagmdf_oneeach() {
     dagmdf_oneeach("/tmp/mdf_dag.dat");
}
