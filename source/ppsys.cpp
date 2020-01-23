#include <base.h> // classe per la lettura dei fiel .dat

#include <TH1.h>
#include <TGraph.h>
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

  switch (argc){
    case 1:
      cout <<"ppsys <rootfile>"<<endl;
      return 1;
    default:
      ;
  };
	
  
  char *nome1=argv[1]; // nome file root iniziale
  string nm(nome1);
 
  TFile f(nome1,"READ");

  TTree* data = (TTree*)f.Get("pulse");


  TBranch* nbr = data->GetBranch("n");
  TBranch* Tbr = data->GetBranch("T");
  TBranch* wbr = data->GetBranch("wfm");



  int m = Tbr->GetEntries();
  cout<<" numero impulsi: "<<m<<endl;
  int m1 = 0;
  int n = nbr->GetEntry(10);
  cout<<" numero punti per impulso: "<<n<<endl;
  double wfm[n];
  for (int j=0; j<n; j++) wfm[j] = 0;

  for (int i=0; i<m; i++) {
    //int n = nbr->GetEntry(i);
    for (int j=0; j<n; j++) {
      wfm[j]=wfm[j]+wbr->GetEntry(i*m+j);
      m1++;
    };
  };

  f.Close();

  TH1D h1("ave", "waveform", n, 0,n); 
  for(int i=0; i<n; i++) h1.Fill(i,wfm[i]);
  h1.SetLineColor(0);
  h1.SetMarkerStyle(20);
  //h1.SetMinimum(LRANGE);

  TCanvas c;

  h1.Draw();

  app.Run(true);

  return 0;  
};
 
  
  
  
  
 