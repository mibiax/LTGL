#include <iv0.h>


iv::iv(const char *nomefile) {
  
  fdat primo(nomefile); //leggo il file

  if (primo.notfile()) 
    {
      cout << "manca il file: "<< nomefile<<endl;
      fileerror = true;
      return;
    };

  fileerror = false;

  ////// caricamento variabili associate alla curva.
  n=primo.np();
  amp=primo.amp;
  squid=primo.squid;
  TmK=primo.TmK;
  Tohm=primo.Tohm;
  Rshunt=primo.shunt;
  Rc=primo.Rc;
  
  // Allocamento memoria

  I = new double[n];
  V = new double[n];
  for (int i=0; i<n; i++){
    I[i] = primo.dat(i,1);
    V[i] = primo.dat(i,2);
  };
  
  if ((I[10]*V[10])<0) for(int i=0; i<n; i++) I[i]=-I[i]; // correggo inversioni
   
  return;
};

iv::iv(int a, double* bias, double* out){
  n = a;

  I = new double[n];
  V = new double[n];

  TmK=0;
  Tohm=0;
  Rshunt=0;
  Rc=0;
  squid=0;
  amp=0;

  for (int i=0; i<n; i++) { //trasferisco ivalori
    I[i] = bias[i];
    V[i] =  out[i];
  };
};

iv::iv() {
  n=0;

  TmK=0;
  Tohm=0;
  Rshunt=0;
  Rc=0;
  squid=0;
  amp=0;

  I   = NULL; 
  V   = NULL; 

  return;
};


iv::iv(const iv &old) {
  n=old.n;

  TmK=old.TmK;
  Tohm=old.Tohm;
  Rshunt=old.Rshunt;
  Rc=old.Rc;
  squid=old.squid;
  amp=old.amp;

  I = new double[n];
  V = new double[n];
  
  
  for (int i=0; i<n; i++) I[i]=old.I[i];
  for (int i=0; i<n; i++) V[i]=old.V[i];
 

  return;
};


iv iv::operator = (const iv &old) {

  n=old.n;

  TmK=old.TmK;
  Tohm=old.Tohm;
  Rshunt=old.Rshunt;
  Rc=old.Rc;
  squid=old.squid;
  amp=old.amp;

  for (int i=0; i<n; i++) I[i] = old.I[i];
  for (int i=0; i<n; i++) V[i] = old.V[i];

  return *this;
};

string iv::label(){
  stringstream num;
  num << TmK;
  return num.str()+"mK";
};


iv::~iv() {
  delete[] I;
  delete[] V;

  return;
};    
