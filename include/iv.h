#include <iv0.h>
#include <TGraph.h> //root
#include <TF1.h>
#include <TAxis.h>

#define IV

using namespace std;


class ivs : public iv {
     
 public:
                       
  ivs(const char *nomefile);  // costruttore
  ivs(const string strfile)  {ivs:ivs(strfile.c_str());};  // costruttore da stringa
  ivs(const ivs &old);        // costruttore di copia
  ivs();                      // costruttore vuoto NULL
  
  ivs operator = (const ivs &old); // assegnazione

  
  inline double p(int a)  {return P[a];};
  inline double* p()      {return P;};
  inline double r(int a)  {return R[a];};
  inline double* iTES()   {return Ites;};
  inline double* Vsh()    {return Vshunt;};

  TGraph* plot();

  double Rp(int a); //fit di R sull'intervallo centrale di 2a punti
  
  ~ivs();

  protected: // Puntatori array dati lunghezza pt()
  
   double *Ites;
   double *Vshunt;
   double *P; // potenza
   double *R; // resistenza

};

