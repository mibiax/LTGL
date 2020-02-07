#include <iv.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TMultiGraph.h>
#include <cstring>

#define RTDIR "./"

TApplication app("app",NULL,NULL);

int main(int argn, char** argv){

  bool s = false, a = false;
  bool d = false, f = false;
  
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
              case 's': s = true; break; //
              case 'a': a = true; break; //
              case 'd': d = true; break; //
              case 'f': f = true; break; //
              default: ;
            };
          };
        };
      };	
  	if (d) goto serie;
  	case 2: goto singola;
    case 1: ;
    case 0:
      cout<<arg0<<" [opt] <filename>"<<endl;
      exit(1);
  };

  serie: {
    cout << "serie" <<endl;
    path = RTDIR + string(argv[2]);
    filelist lista(path);
    if (lista.nofile()) return 1;
    cout<<lista.nfile()<<" file trovati"<<endl;
    for (int i=0; i<lista.nfile(); i++) {
      cout << lista.s(i)<< endl;
    };

    iv* misure[lista.nfile()];
    
    for (int i=0; i<lista.nfile(); i++) misure[i] = new iv(lista.s(i));

    if (s) {
      double sq;
      cout<<"Squid Gain (mv/uA): ";
      cin>> sq;
      cout<<endl;
      for (int i=0; i<lista.nfile(); i++) misure[i]->SetSquid(sq);
    };

    if (a) {
      double am;
      cout<<"amplifier Gain : ";
      cin>> am;
      cout<<endl;
      for (int i=0; i<lista.nfile(); i++) misure[i]->SetSquid(am);
    };
      

    return 0;
  };

  singola:
  iv curva(RTDIR+arg0);

  return 0;
};