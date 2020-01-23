#include <base.h> 

#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

#define CUT 10000  // Numero di wfm per filedi root

int main(int argn, char** argv){
  
  char *  nome1;   //path
  char *  nome2;   //rootfile
  
  int elenco=0;

  bool s[4] = {false, false, false, false};
  int opt;

  switch (argn){
  	default:
      for (int i=1; i<argn-1; i++){
        if (argv[i][0]=='-'){
          opt = strlen(argv[i]);
          //cout <<opt<<endl;
          for (int j=1; j<opt; j++){
            switch (argv[i][j]) {
              case 's': s[0] = true; break; //singolo impulso per file to do
              case 'p': s[1] = true; break; //scrivi istogramma
              case 'd': s[2] = true; break; //scrivi solo dati
              case 'f': s[3] = true; break; //to do
              default: ;
            };
          };
        };
      };
    case 3: 
    	nome1=argv[argn-2];
      nome2=argv[argn-1];
      break;
  	case 2:
      nome1=argv[argn-1];
      nome2=argv[argn-1];
      break;
    case 1: ;
    case 0:
      cout<<"pulse [opt] <path> [<rootfile>]"<<endl;
      exit(1);
  };

  cout<< nome1 <<" -> " <<nome2 <<endl;

  filelist datafile(nome1);  //il path è mediato da filelist?
  cout << "trovati "<<datafile.nfile()<<" file dati";
  if (datafile.nofile()) abort();
  
  int ntfile = datafile.nfile()/CUT;         //divisione tra interi!!
  if ((datafile.nfile()%CUT)!=0) ntfile++;   //se la divisione non è esatta aggiungo uno
  cout << " verranno trasformati in " <<ntfile<<" file di root"<<endl;
  
 
  TTree t1 ("header","Dati di intestazione");
  
  
  for (int j=0; j<ntfile; j++){
        
    ostringstream nm;
    nm<<j;
    string nome0(nome2+nm.str()+".root");    //definisco il nome del file root composto con mumero sequnziale
    TFile f(nome0.c_str(),"RECREATE");       //creo il file
    cout << "Aperto file "<< nome0  << endl; //controllo   
        
    int stop = CUT;
    if (j==ntfile-1) stop = datafile.nfile()-j*CUT;
  
    for (int i=0; i<stop; i++) {           //fino a quando non ho esaurito i file della lista 
      
             cout<<"ok"<<endl; 
      fdat primo(datafile.c(i+j*CUT));
      

        if (primo.notfile()){
          cout << "File non trovato" << endl;
          continue; //ricomincia questo ciclo for da capo!
        };
        

        cout << "#" << i+j*CUT << endl;
        cout <<"numero di punti: "<< primo.np()<<endl;
        cout <<"guadagno amplificatore: " <<primo.amp<<endl;
        cout <<"guadagno SQUID: "<<primo.squid<<endl;
        cout <<"Ib =  "<<primo.bias<<" A"<<endl;
        cout <<"T = "<<primo.TmK<<" mK "<< endl;
        cout <<"T -> "<<primo.Tohm<<" ohm blocco"<<endl;
  
        ostringstream numero;
        numero<<i+j*CUT;
   
        int n;
        if (s[0]) n=primo.np(); else n=primo.nc()-1;    
        double wfm[n];
        double T; 
        string titolo;

        t1.Branch("n",&n,"n/I");
        t1.Branch("wfm",&wfm,"wfm[n]/D",sizeof(double)*n);
        t1.Branch("T",&T,"T/D");
        //t1.Branch("Ib", &primo.bias,"bias/D");
        //t1.Branch("blocco",&primo.Tohm,"blocco/D");            
        


        if (s[0]) {
          titolo ="wfm"+numero.str(); //nome wfm //da rivedere
          for(int k=0; k<n; k++) wfm[k]=primo.dat(k); // Riempio istogramma
          T = primo.T();
          t1.Fill(); // aggiungo entry al TTree
        } else {
          for(int k=0; k<n; k++){
            for(int l=1; l<primo.nc(); l++) wfm[l]=primo.dat(k,l);
            T = primo.dat(k,0);
            t1.Fill();
           };
        };

          
          /*
          TH1D h1(titolo.c_str(), "waveform", n, 0,n);    // creo istogramma.
          for(int k=0; k<n; k++) h1.Fill(k,primo.dat(k));
          h1.SetLineColor(0);
          h1.SetMarkerStyle(20);
          h1.SetMinimum(LRANGE);
          h1.Write(); // scrivo istogramma wfm nel file root
          */
                            
    
        }; //fine del loop di file
        
        /*if (j==ntfile-1) */ t1.Write("pulse");         // scrivo tree 
        f.Close(); // chiusura file .root       
        
   };  
    
  cout << endl  << "*** Convertiti tutti i file ***"<<endl << endl;

  return 0;   
};
