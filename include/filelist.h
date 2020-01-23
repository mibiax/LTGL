/***************************************
 * Classe filelist header file
 * restituisce l'elenco dei file .dat 
 * presenti in una cartella come segue: 
 * ./<path>*.dat (solo UNIX non portabile)
 ***************************************/
////////////////////////////////////////
/**** include solo librerie standard ***/

#ifndef BASE
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#endif

#ifndef FILELIST
#define FILELIST

using namespace std;

class filelist 
{
 private:

  int n;
  vector <string> lsfile;
  
 public:

  filelist(string path); //costruttore
    
  inline string s(int a)           {return lsfile.at(a);};
  inline const char* c(int a)      {return lsfile.at(a).c_str();};
  inline string operator[] (int a) {return lsfile.at(a);};
  inline bool nofile()             {if (n==0) return true; else return false;};
  inline int nfile()               {return n;};

  ~filelist() {}; //non fa ninete
};


#endif