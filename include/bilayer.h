#include <base.h>
#include <cmath>
#include <TGraph2D.h>
#include <TGraph.h>
#include <TF2.h>
#include <TF1.h>
#include <TH1D.h>

#define BILAYER

const double rhoIr = 4.7e-8; //ohm*m
const double rhoAu = 2.4e-8; //ohm*m
const double mnm = 1e12; //ohm*m->mohm*nm

class bilayer{

 public:
 	bilayer(char* nomefile);

 	double RRR();  //average RRR
 	double RRRe(); //average RRR error

 	inline int num() {return n;};

 	TF2* fitRT();
 	TF2* fitLT();
 	TF1* fitTc();

 	TH1D* hRRR();

 	TGraph2D* plotRT();	
 	TGraph2D* plotLT();
 	TGraph2D* plotTc2D();
 	TGraph*   plotTc();
 	
 	~bilayer();

 protected:

 	int n;
 	int m1, m2;
 	int mtc;

 	double* Ir;
 	double* Au;
  	double* R1; 
  	double* R2;
  	double* geof;
  	double* Tc;
  	
};