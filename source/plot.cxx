#include <plot.h>

FP::FP(string nomefile){

  fdat dati(nomefile.c_str());
  
  n = dati.np();
 // cout << "punti: " <<n<<endl;
  
  X = new double[n];
  Y = new double[n];
  
  
  for (int i=0; i<n; i++) {
    X[i] = dati.dat(i,0);
    Y[i] = dati.dat(i,1);
  };
  
  return;
};

FP::FP(){
  n = 0;
  
  X = NULL;
  Y = NULL;

 return;
};


TGraph* FP::plot(){
  TGraph* ready = new TGraphErrors(n, X, Y);
  ready -> SetMarkerStyle(20);
  //ready -> SetTitle("R(T);  T (mK); R (#Omega)");
  return ready;
};


FP::~FP(){
  delete[] X, Y;
};




