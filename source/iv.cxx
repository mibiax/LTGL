#include <iv.h>


ivs::ivs(const char *nomefile): iv(nomefile) { // costruttore

  P = new double[n];
  R = new double[n];
  Ites  = new double[n];
  Vshunt = new double[n];
    
 
  for (int i=0; i<n; i++) {
       Ites[i]=V[i]/gain()/ampl()/1000;       //guadagno squid in mV/uA
       Vshunt[i]=(I[i]-Ites[i])*Rshunt/1000; //Rshunt in mohm
       P[i]=Ites[i]*Vshunt[i];
       R[i]=Vshunt[i]/Ites[i];
  };

   return;
};


ivs::ivs(): iv() {
 
  P   = NULL; 
  R   = NULL;
  Ites  = NULL;
  Vshunt = NULL;

  return;
};


ivs::ivs(const ivs &old): iv(old) { // costruttore di copia


  P = new double[n];
  R = new double[n];
  Ites  = new double[n];
  Vshunt = new double[n];
  
  
  for (int i=0; i<n; i++) P[i]=old.P[i];
  for (int i=0; i<n; i++) R[i]=old.R[i];
  for (int i=0; i<n; i++) Ites[i]  = old.Ites[i];
  for (int i=0; i<n; i++) Vshunt[i] = Vshunt[i];

  return;
};


ivs ivs::operator = (const ivs &old) {

  n=old.n;

  TmK=old.TmK;
  Tohm=old.Tohm;
  Rshunt=old.Rshunt;
  Rc=old.Rc;
  squid=old.squid;
  amp=old.amp;

  for (int i=0; i<n; i++) I[i] = old.I[i];
  for (int i=0; i<n; i++) V[i] = old.V[i];
  for (int i=0; i<n; i++) P[i] = old.P[i];
  for (int i=0; i<n; i++) R[i] = old.R[i];
  for (int i=0; i<n; i++) Ites[i]  = old.Ites[i];
  for (int i=0; i<n; i++) Vshunt[i] = Vshunt[i];

  return *this;
};

TGraph* ivs::plot(){

  TGraph* p1 = new TGraph(n,Vshunt, Ites); //Genero il grafico della curva IV
  string titolo = "I(V) ";
  titolo = titolo + label();
  p1->SetName(label().c_str());
  p1->SetTitle(titolo.c_str());
  p1->GetXaxis()->SetTitle("V (V)"); 
  p1->GetXaxis()->CenterTitle();
  p1->GetYaxis()->SetTitle("I (A)");
  p1->GetYaxis()->CenterTitle();
  
  return p1;
};


//**************** metodo di FIT

double ivs::Rp(int a)
{
  TGraph gt(n,Ites,Vshunt);
  TF1 fRp("fRp","[0]*x",Ites[n/4-a],Vshunt[n/4+a]);
  fRp.SetParName(0, "Rp");
  fRp.SetParLimits(0,0,100);
  fRp.SetParameter(0,0.001);
  gt.Fit("fRp","W0","A",V[n/4-a],V[n/4+a]);
  double out = fRp.GetParameter(0);
  double err = fRp.GetParError(0);
  cout << V[n/4-a] << " : "<< V[n/4+a]<<endl;
  return out;
};


ivs::~ivs() {
  
  delete[] P;
  delete[] R;
  delete[] Ites;
  delete[] Vshunt;

  return;
};    
