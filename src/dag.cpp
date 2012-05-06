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

app_dag::app_dag() {
     _ntasks = _nvm = 0;
}

app_dag::app_dag(const char *filename) {
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
          return;
     } else {
          cout << "Erro na leitura do arquivo " << f << endl;
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
     for (unsigned i = 0; i < _paths.size(); ++i) {
          for (unsigned j = 0; j < _paths[i].size(); ++j) {
               cout << _paths[i][j] << ' ';
          }
          cout << endl;
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
     for (unsigned i = 0; i < path.size()-1; ++i) {
          weight += _B[path[i]][path[i+1]];
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

void app_dag::dagmdf() {
     gen_Phs();
     map<pair<int,int>, vector<int> >::iterator it;
     cout << "+++++\n";
     for (it = _P.begin(); it != _P.end(); it++) {
          cout << it->first.first << ' ' << it->first.second << ": ";
          vector<int> aux(it->second);
          for (unsigned i = 0; i < aux.size(); ++i) {
               cout << aux[i] << ' ';
          }
          cout << endl;
     }
     cout << "=======\n";
     cout << "Heavy: ";
     cout << find_heavy()->first.first << ' ' << find_heavy()->first.second << endl;
     cout << endl;
     remove_tasks(find_heavy());
     for (it = _P.begin(); it != _P.end(); it++) {
          cout << it->first.first << ' ' << it->first.second << ": ";
          for (unsigned i = 0; i < it->second.size(); ++i) {
               cout << it->second[i] << ' ';
          }
          cout << endl;
     }
     cout << endl;
}
