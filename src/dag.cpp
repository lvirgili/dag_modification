#include "dag.hpp"

int app_dag::ntasks() const {
     return _ntasks;
}

int app_dag::nvm() const {
     return _nvm;
}

int app_dag::I(const int i) const {
     return _I[i];
}

int app_dag::S(const int i) const {
     return _S[i];
}

int app_dag::B(const int i, const int j) const {
     return _B[i][j];
}

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
          cout << "Erro na leitura do arquivo " << filename << endl;
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

int app_dag::find_weight(map<pair<int,int>,vector<int> >::iterator it) {
     int weight = 0;
     vector<int> path(it->second);
     if (path.size() > 1) {
          for (unsigned i = 0; i < path.size()-1; ++i) {
               weight += _B[path[i]][path[i+1]];
          }
     } else {
          weight = 1; //Just one task, return a small number as there is no "path".
     }
     return weight;
}

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

void app_dag::remove_tasks(map<pair<int,int>, vector<int> >::iterator cur_it) {
     map<pair<int,int>, vector<int> >::iterator it;
     vector<int> tasks(cur_it->second); //Tasks to be removed from the sets
     for (it = _P.begin(); it != _P.end(); ++it) {
          for (unsigned i = 0; i < tasks.size(); ++i) {
               if (count(it->second.begin(), it->second.end(), tasks[i]) != 0) {
                    it->second.erase(find(it->second.begin(), it->second.end(), tasks[i]));
               }
          }
          if (it->second.empty() == true) { //Set became empty.
               _P.erase(it);
          }
     }
}

void app_dag::dagmdf(const char *outfile) {
     gen_Phs(); // Creates the set P_{h,s}
     map<pair<int,int>, vector<int> >::iterator it;
     vector<int> newS, newI;
     vector<vector<int> > newB;
     int vms_added = 0;
     newS.push_back(0); newI.push_back(0); // The repository.
     vector<int> repo(1,0); newB.push_back(repo);
     while (_P.empty() == false) {
          map<pair<int,int>, vector<int> >::iterator heavy = find_heavy();
          ++vms_added;
          newS.push_back(0);
          newI.push_back(vinfo->TV(heavy->first.second - 1));
          newB[0].push_back(vinfo->BV(heavy->first.second - 1));
          vector<int> vm(_ntasks, 0); newB.push_back(vm);
          for (unsigned i = 0; i < heavy->second.size(); ++i) {
               newB[vms_added][heavy->second[i]] = 0x7FFFFFFF; // Adds an arc from the vm to the task i with weight infinite.
          }
          remove_tasks(heavy);
     }
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
     int new_ntasks = _ntasks + vms_added + 1;
     printDAG(outfile, new_ntasks, newB, newI, newS);
}

void app_dag::dagmdf() {
     dagmdf("/tmp/mdf_dag.txt");
}
