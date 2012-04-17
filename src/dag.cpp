#include "dag.hpp"

int app_dag::ntasks() {
     return _ntasks;
}

int app_dag::nvm() {
     return _nvm;
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
          getline(f, s); // Throw away the I info.
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
          _B = new int *[_ntasks];
          for (int i = 0; i < _ntasks; ++i) {
               _B[i] = new int[_ntasks];
          }
          for (int i = 0; i < _ntasks; ++i) {
               getline(f, s, ')');
               for (int j = 0; j < _ntasks; ++j) {
                    f >> _B[i][j];
               }
          }
          return;
     } else {
          cout << "Erro na leitura do arquivo " << f << endl;
          exit(-1);
     }
}

app_dag::~app_dag() {
     delete[] _S;
     for (int i = 0; i < _ntasks; ++i) {
          delete[] _B[i];
     }
     delete[] _B;
}
