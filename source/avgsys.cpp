#include <base.h> // classe per la lettura dei fiel .dat

#include <TH1.h>
#include <TF1.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>

#include <TCanvas.h>
#include <TApplication.h>

#define SAT 4.9
#define TRIG 1.2
#define TAU  10

TApplication app("app",NULL,NULL);

int main(int argc, char** argv){

  if(argc<2) // verifica parametri
    {
      cout <<argv[0]<<" <rootfile>"<<endl;
      return 1;
    };
	
  
  char *nome1=argv[1]; // nome file root iniziale
  string nm(nome1);
  string nome2("N"+nm);
  
  cout << nome2.c_str()<< " nome nuovo file"<<endl;
  TFile f(nome1,"READ");
  TFile nuovo(nome2.c_str(),"RECREATE");
  
  TIter next(f.GetListOfKeys()); //leggo lista delle chiavi del file f
  TKey *key;
  
  int  nwfm = 0;  //numero wfm
  int  mwfm = 0;  //numero wfm buone
  int mtrig = 0;  //bin trigger medio
  int   pup = 0;  //numero wfm pile pup
  int   sat = 0;  //numero wfm saturate
  int notrg = 0;  //numero wfm senza trigger
  
  int nb;         //numero bin
  int bmax;       //bin massimo
  int brecov = 0; //bin recovery
  
 
 //leggo il file root e rimuovo le forme d'onda non buone 
  
  while ((key = (TKey*)next())) {
    TH1D *h1;
    
    string title(key->GetTitle());
    if (title!="Average Pulse") continue;
    
  	string nome(key->GetName()); //nome wfm
    
    h1 = (TH1D*) f.Get(nome.c_str()); //carico in memoria un istogramma
    //cout << "Key " << nwfm << endl;
    //cout << " Classname " <<key->GetClassName() << endl;
    //cout << " Title " <<key->GetTitle() << endl;
    nwfm++;
    nb = h1->GetSize();                                    //leggo numero di bean nell'istogramma
    int trig = 0;
    int up = 0;
    bool flag = false;
    
    for (int i=0; i<nb-5; i++)   {                            //cerco il il trigger medio e pileup 
      if ((h1->At(i+5)-h1->At(i))>TRIG) {
        	if (flag) continue;
	        else {
	          flag=true;
	          up++;
            trig=i;         
	        };      
      } else flag=false;   
    };

    if (up>1)     {pup++; cout<<nwfm-1<<"\tpilepup"<<endl; continue;};       //se pileup scarta
    if (trig==0)  {notrg++; cout<<nwfm-1<<"\tnotrigger"<<endl; continue;};   //Se no trigger scarta
    if (trig==nb) {notrg++; cout<<nwfm-1<<"\tendtrigger"<<endl; continue;};  //se trigger scarta
    mwfm++;
    mtrig = mtrig + trig;
    
    bmax = h1->GetMaximumBin();
    
    
    double ll;
    for (int i=bmax+10; i<nb; i++) {
      double tau = h1->GetBinContent(i)/h1->GetBinContent(bmax);
      if (tau<exp(-TAU)) {ll = i; break;}; 
    };
    brecov = brecov + ll;
    //cout << nwfm <<" TAU -> "<<ll<<endl;
    
    h1->Write();
   
    endloop: ;
  };
 
 brecov = brecov / mwfm;
 mtrig = mtrig / mwfm;      //faccio la media del punto di trigger: divisione tra interi

 f.Close();     // chiusura file .root       
 
 cout << "wfm:\t"<<mwfm<<"/"<<nwfm<<endl //stampo a schermo il rapporto delle wfr scartate
      <<endl
      <<"pileup:\t\t"<<pup<<endl
      <<"no trigger:\t"<<notrg<<endl
      <<"saturazioni:\t"<<sat<<endl
      <<endl
      <<"punto a 5 tau:\t"<<brecov<<endl
      <<"punto trigger medio:\t"<<mtrig<<endl
      <<endl;
  
 TIter next2(nuovo.GetListOfKeys());
 next2.Begin();

 TH1D h0;  
 for (int i=0; i<nb; i++) h0.Fill(i,0);

 while ((key = (TKey*)next2())) {  
   //cout<<key->GetName()<<endl;
   string nome(key->GetName());
   TH1D *h1 = (TH1D*) nuovo.Get(nome.c_str()); //carico in memoria un istogramma
   nb = h1->GetSize();                     //leggo numero di bean nell'istogramma
   int trig=0;
   for (int i=0; i<nb-5; i++) if ((h1->At(i+5)-h1->At(i))>TRIG) trig=i;
   int delta=trig-mtrig;
   TH1D h2;
   if (delta>0) for (int j=0; j<nb-delta; j++) h1->SetBinContent(j,h1->GetBinContent(j+delta));
	 if (delta<0) for (int j=-delta; j<nb; j++) h1->SetBinContent(j,h1->GetBinContent(j+delta));  
   
   for (int i=0; i<250+brecov; i++) h2.SetBinContent(i,h1->GetBinContent(mtrig-250+i)); //non funziona
   
   if (delta>0) for (int j=mtrig-250; j<(int)(1.5*brecov)-delta; j++) h2.Fill(j,h1->GetBinContent(j+delta));
	 if (delta<0) for (int j=mtrig-250-delta; j<(int)(1.5*brecov); j++) h2.Fill(j,h1->GetBinContent(j+delta));
   
   
   
   
   //h0.Add(h1,1.0/mwfm);
   h0=h0+*h1;  //......<-****
   
   
   
 };

 nuovo.Close();
  
 h0.Scale(1./(double)mwfm);

 TCanvas c("mean pulse",0,0);
 h0.Draw("P");
 
  
  
 app.Run(true);

  return 0;  
};
 
  
  
  
  
 