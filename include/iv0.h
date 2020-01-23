#ifndef BASE
#include <base.h> //lettura file sviluppata da Michele Biasotti
#endif
#define IV0

using namespace std;


class iv {

 public:
  iv(const char *nomefile);   // costruttore
  iv(const string strfile) {iv:iv(strfile.c_str());};   // costruttore da stringa
  iv(const iv &old);          // costruttore di copia
  iv(int n, double* bias, double* out);   //costruttore da dati
  iv();                       // costruttore vuoto NULL e 0

  iv operator = (const iv &old); // assegnazione
  
  inline bool operator < (const iv &dx){return (Tohm>dx.Tohm);}; // ordinamento
  inline bool operator > (const iv &dx){return (Tohm<dx.Tohm);};
              

  //*****************  Uscite dirette di membri privati

  inline int pt()           {return n/2;};       // numero di punti sulla curva.

  inline double out(int a)  {return V[a];};
  inline double* out()      {return V;};
  inline double bias(int a) {return I[a];};
  inline double* bias()     {return I;};
  inline double T()         {return TmK;};       //temperatura in mK
  inline double block()     {return Tohm;};      //resistenza termometro blocco
  inline double shunt()     {return Rshunt;};    //resistenza di shunt
  inline double gain()      {return squid;};     // Guadagno squid
  inline double ampl()      {if (amp<=0) return 1; else return amp; }; // guadagno preamp.
  inline double RL()        {return Rc;};        // resistenza di lettura corrente
  
  inline bool nofile()      {return fileerror;};
  
  inline void SetRL(double a)   {Rc = a;};
  inline void SetAmp(double a)  {amp = a;};
  inline void SetSquid(double a){squid = a;};
  inline void SetRsh(double a)  {Rshunt = a;};
  inline void SetT(double T)    {TmK = T;};

  string label();

  ~iv();

 protected:
  int n;

  double TmK;
  double Tohm;
  double Rshunt;
  double Rc;
  double squid;
  double amp;
  
  double *I; // I bias 
  double *V; // V out squid

  bool fileerror;
};

