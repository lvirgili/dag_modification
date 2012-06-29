Embedding Software Requirements in Grid Scheduling
==================================================

There are three ways to embed the software requirements in the
application digraph (DAG) so far:
1. Adding one virtual machine (VM) to each task of the DAG;
2. Instantiating each needed VM once and all tasks are dependent on
this instance; and
3. Using the algorithm proposed [here](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?reload=true&arnumber=5962664&contentType=Conference+Publications).

Compiling
---------
* One VM to each task
This one is compiled by issuing
```bash
make one_each
```

that generates an executable called "one_each".

* One VM instance
This one is compiled by issuing
```bash
make only_one
```

that generates an executable called "only_one".

* Path based modification
This one is compiled by issuing
```bash
make path_mdf
```

that generates an executable called "path_mdf".

Usage
-----
All the executables are executed with at least the same 2 parameters:

1.  The path to a file containing an application DAG information;
2.  The path to a file containing the information of the virtual machines.

Also, a third argument, which is optional, is the path for the file in
which the modified graph will be printed. If not provided, the file
used is "/tmp/mdf_dag.dat".

Examples
--------
In these examples, ![this sample graph][sample] will be modified.

* Adding one VM to each task
The command to modify the graph with this algorithm is:

```bash
./one_each sample.dat vm_sample.dat
```

This will print the modified DAG in the default exit file,
"/tmp/mdf_dag.txt", represented by ![this graph][oneeach_mdf]

* One instance of each VM
The command for this modification has the same parameters, except that
it receives the third one, the output file.

```bash
./only_one sample.dat vm_sample.dat output.dat
```

This generates ![this graph][onlyone_mdf].

* Path modification
To obtain the modification presented in the paper:
```bash
./path_mdf sample.dat vm_sample.dat output.dat
```

That has ![this output][path_mdf].

[sample]: examples/sample.jpeg "Sample DAG"
[oneeach_mdf]: examples/one_each_mdf.jpeg "DAG modified with one VM to
each task"
[onlyone_mdf]: examples/onlyone_mdf.jpeg "DAG modified with only one
instance of each VM"
[path_mdf]: examples/path_mdf.jpeg "DAG with the path dependent modification"
