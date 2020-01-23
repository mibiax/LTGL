/***************************************
 * Classe filelist 
 * restituisce l'elenco dei file .dat 
 * presenti in una cartella come segue: 
 * ./<path>*.dat
 ***************************************/

#include "filelist.h"


using namespace std;


filelist::filelist(string path)
{
  const string comm1("ls -1 ./");
  const string comm2("*.dat > lista.temp");
  string comm(comm1+path+comm2);
  const char* comm0 = comm.c_str();
  cout << comm0 << endl;

  system(comm0); // chiamata al sistema  eseguzione comando di shell

  fstream lista;

  try
    {
      lista.open("lista.temp"); 
    }
  catch(...)
    {
      n=0;
      return;
    };

  n=0;

  while (!(lista.eof())) // fino a quando non ho esaurito i file della lista 
      {
	
	string nome;
	getline(lista, nome);
	fstream test(nome.c_str());

	if (test.is_open())
	  {
	    lsfile.push_back(nome);
	    n++;
	  };
	test.close();
	//cout << "leggo il file #"<<n<<" ->" <<nome <<endl;
      };
  //cout << "caricati "<< n<<" file"<<endl;
  lista.close();
  system("rm lista.temp");
  
};

//template multi *******************************************************





