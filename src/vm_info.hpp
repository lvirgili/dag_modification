#ifndef VM
#define VM

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class vm_info {
private:
     int _numvms;
     int *_SV;                  //Software offer of the VM.
     double *_BV;               //Time taken for booting the VM.
     double *_TV;               //Size of the VM.

public:
     vm_info(const char *);     //Constructor, receives the file that has the VM's info.
     ~vm_info();
     //Getters.
     int numvms() const;
     int SV(const int) const;
     double BV(const int) const;
     double TV(const int) const;
};

#endif
