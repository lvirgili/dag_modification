Embedding Software Requirements in Grid Scheduling
==================================================

Compiling
---------
A simple
```bash
make
```

will generate an executable called <pre>dagmdf<\pre>.

Usage
-----
The executable has to be run with at least 2 parameters:

1.  The path to a file containing an application DAG information;
2.  The path to a file containing the information of the virtual machines.

Also, a third argument, which is optional, is the path for the file
that the modified graph will be printed. If not provided, the file
used is "/tmp/mdf_dag.dat".

Examples
--------
To modify the DAG represented by the file "test.dat", using the VMs in
the file "vm_test.dat", do:

```bash
./dagmdf test.dat vm_test.dat
```

This will print the modified DAG in the default exit file,
"/tmp/mdf_dag.txt".

Providing an exit file path:

```bash
./dagmdf test.dat vm_test.dat exit_file.dat
```

Will print the modified DAG in "exit_file.dat".