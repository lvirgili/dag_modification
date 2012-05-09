#include <iostream>
#include "dag.hpp"

int main(int argc, char **argv) {
     if (argc != 3 && argc != 4) {
          cout << "Usage: ./dagmdf /path/to/appdag /path/to/vminfo\n";
          return 0;
     }
     app_dag G(argv[1], argv[2]);
     cout << "***** DAG *****\n";
     cout << G.ntasks() << endl;
     cout << G.nvm() << endl;
     cout << "I: ";
     for (int i = 0; i < G.ntasks(); ++i) {
          cout << G.I(i) << ' ';
     }
     cout << endl;
     cout << "S: ";
     for (int i = 0; i < G.ntasks(); ++i) {
          cout << G.S(i) << ' ';
     }
     cout << endl;
     for (int i = 0; i < G.ntasks(); ++i) {
          for (int j = 0; j < G.ntasks(); ++j) {
               cout << G.B(i, j) << ' ';
          }
          cout << endl;
     }
     cout << "***** VM *****\n";
     cout << "numvms: " << G.vinfo->numvms() << endl;
     for (int i = 0; i < G.vinfo->numvms(); ++i) {
          cout << G.vinfo->SV(i) << ' ';
     }
     cout << endl;
     for (int i = 0; i < G.vinfo->numvms(); ++i) {
          cout << G.vinfo->BV(i) << ' ';
     }
     cout << endl;
     for (int i = 0; i < G.vinfo->numvms(); ++i) {
          cout << G.vinfo->TV(i) << ' ';
     }
     cout << endl;
     cout << "======\n";
     G.dagmdf();

     return 0;
}
