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
