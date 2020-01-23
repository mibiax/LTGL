#include <base.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TMath.h>

#define HCUT 20
#define LCUT 1e3

struct TrPar {
	double	Rp;
	double	Rn;
	double 	l;
	double  Tc;
	double dRp;
	double dRn;
	double dl;
	double dTc;
};

struct cal {
    TGraph* plot;
    TF1* ht;
    TF1* lt;
};


class RT {
    
  public:
    RT(string nomefile);
    RT(double* T, double* R, double* dT, double* dR);
    RT(double* T, double* R);
    RT();
        
    inline double T(int i)         {return TmK[i];}; 
    inline double* T()             {return TmK;};
    inline double res(int i)       {return R[i];};
    inline double* res()           {return R;};
    inline double blk(int i)       {return Tohm[i];};
    inline double* blk()           {return Tohm;};

    cal recal(string nome, double hcut=HCUT, double lcut=LCUT);   //provvisorio da sistemare
    void subtract(double val); //sottrae un valore di resistenze (offset)
    void shift(double val); 

    TGraphErrors* plot_err();
    TGraphErrors* Kplot_err();

    TGraph* plot();
    TGraph* Kplot();
    TGraph* rawplot(); 
    TGraph* calplot(); //utilizzo dei dati per genrare calibrazione da implementare
    TGraph* alpha();
    
    TF1* fit();
    TF1* inv();        //da verificare fit inverso inversione transizione.
    TF1* inv(TrPar p);

    TrPar fitpar();

    double alpha(int i);
    double TalphaMax();
    double inv(double r);
    double inv(TrPar p, double r);
    double Tc();   
    double Tc10();
    double Tc90();
    
    ~RT();

 protected:

    int n;
    double* Tohm; 
    double* dTohm; 
    double* TmK;
    double* dTmK;
    double* R; 
    double* dR;
};



