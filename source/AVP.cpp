#include <base.h>
#include <TApplication.h>
#include <TTree.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TAxis.h>

#define LRANGE -5 // valore minimo asse y

using namespace std;

TApplication app("app",NULL,NULL);


int main(int argn, char **argv){
  char* dir;
  string rootfile = "AVP.root";
  

  bool r = false, l = false;
  bool t = false, f = false;
  
  int opt;

  string path;
  string arg0(argv[0]);


switch (argn){
    default:
      for (int i=1; i<argn-1; i++){
        if (argv[i][0]=='-'){
          opt = strlen(argv[i]);
          for (int j=1; j<opt; j++){
            switch (argv[i][j]) {
              case 'r': r = true; break; //
              case 'l': l = true; break; //
              case 't': t = true; break; //
              default: ;
            };
          };
        };
      }; 
      dir = argv[3];
      rootfile = argv[2];
      break;
    case 3:
      dir = argv[1];
      rootfile = argv[2];
    case 2: 
      dir = argv[1];
      break;
    case 1: ;
    case 0:
      cout<<arg0<<" [opt] <filename>"<<endl;
      exit(1);
  };

  filelist datafile(dir);  //il path è mediato da filelist?
  cout << "trovati "<<datafile.nfile()<<" file dati";
  if (datafile.nofile()) exit(1);
  int n = datafile.nfile();

  TTree t1 ("header","Dati di intestazione");
  TFile file(rootfile.c_str(),"RECREATE");
        cout << "Aperto file "<< rootfile  << endl; //controllo   

  for (int j=0; j<n; j++){
        
    string nome0(rootfile);
         
    fdat primo(datafile.c(j));
    if (primo.notfile())
      {
        cout << "File non trovato" << endl;
        continue; //ricomincia questo ciclo for da capo!
      };
    
    cout << "#" << j << endl
         <<"numero di punti: "<< primo.np()<<endl
         <<"guadagno amplificatore: " <<primo.amp<<endl
         <<"guadagno SQUID: "<<primo.squid<<endl
         <<"Ib =  "<<primo.bias<<" A"<<endl
         <<"T = "<<primo.TmK<<" mK "<< endl
         <<"T -> "<<primo.Tohm<<" ohm blocco"<<endl
         <<"rate -> "<<primo.s()<<" s"<<endl;
  
    ostringstream numero;
    numero<<j;
   
    string titolo("avg"+numero.str()); //nome wfm
    const int n=primo.np();
               
    TH1D h1(titolo.c_str(), "Average Pulse", n, 0,n*primo.s()); // creo istogramma
    for(int k=0; k<n; k++) h1.Fill(primo.dat(k,0), primo.dat(k,1)); // Riempio istogramma
    h1.SetLineColor(0);
    h1.SetMarkerStyle(20);
    h1.SetMinimum(h1.GetMaximumStored());
    h1.Write(); // scrivo istogramma wfm nel file root
                  
    t1.Branch("T",&primo.TmK,"TmK/D");
    t1.Branch("Ib", &primo.bias,"bias/D");
    t1.Branch("blocco",&primo.Tohm,"blocco/D");
    t1.Branch("amp",&primo.amp,"amp/D");
    t1.Branch("squid",&primo.squid,"squid/D");
    t1.Branch("Rl",&primo.Rc,"Rl/D");
    t1.Branch("shunt",&primo.shunt,"shunt/D");            
    t1.Fill(); // aggiungo entry al TTree 
  };

  t1.Write("header"); 
  file.Close(); // chiusura file .root      
  cout << endl  << "*** Convertiti tutti i file ***"<<endl << endl;

  return 0;
};