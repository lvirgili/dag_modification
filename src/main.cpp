#include <iostream>
#include "dag.hpp"

int main(int argc, char **argv) {
     if (argc != 2) {
          cout << "Usage: ./dagmdf /path/to/appdag\n";
          return 0;
     }
     app_dag G(argv[1]);
     cout << G.ntasks() << endl;
     cout << G.nvm() << endl;
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

     return 0;
}
