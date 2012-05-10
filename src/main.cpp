#include <iostream>
#include "dag.hpp"

int main(int argc, char **argv) {
     if (argc != 3 && argc != 4) {
          cout << "Usage: ./dagmdf /path/to/appdag /path/to/vminfo [/path/to/exit/file]\n";
          return 0;
     }

     //Reads the application DAG with information in the file argv[1]
     //and VM info in the file argv[2].
     app_dag G(argv[1], argv[2]);

     //If an exit file was supplied in argv[3], pass it as an
     //argument. Else, use the default file, "/tmp/mdf_dag.dat".
     if (argc == 4) {
          G.dagmdf(argv[3]);
     } else {
          G.dagmdf();
     }
     return 0;
}
