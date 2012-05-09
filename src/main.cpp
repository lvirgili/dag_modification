#include <iostream>
#include "dag.hpp"

int main(int argc, char **argv) {
     if (argc != 3 && argc != 4) {
          cout << "Usage: ./dagmdf /path/to/appdag /path/to/vminfo [/path/to/exit/file]\n";
          return 0;
     }
     app_dag G(argv[1], argv[2]);
     if (argc == 4) {
          G.dagmdf(argv[3]);
     } else {
          G.dagmdf();
     }
     return 0;
}
