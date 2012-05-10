#include "vm_info.hpp"

vm_info::vm_info(const char *filename) {
     ifstream f(filename);
     if (f.is_open() == true) {
          string s;
          getline(f, s, ':');
          f >> _numvms;
          getline(f, s);
          getline(f, s, ')');
          _SV = new int[_numvms];
          for (int i = 0; i < _numvms; ++i) {
               f >> _SV[i];
          }
          getline(f, s, ')');
          _BV = new double[_numvms];
          for (int i = 0; i < _numvms; ++i) {
               f >> _BV[i];
          }
          getline(f, s, ')');
          _TV = new double[_numvms];
          for (int i = 0; i < _numvms; ++i) {
               f >> _TV[i];
          }
     } else {
          cout << "[VM] Error reading the file " << filename << endl;
          exit(-1);
     }
}

vm_info::~vm_info() {
     delete[] _SV;
     delete[] _BV;
     delete[] _TV;
}

int vm_info::numvms() const {
     return _numvms;
}

int vm_info::SV(const int i) const {
     return _SV[i];
}

double vm_info::BV(const int i) const {
     return _BV[i];
}

double vm_info::TV(const int i) const {
     return _TV[i];
}
