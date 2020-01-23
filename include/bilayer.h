#include <base.h>
#include <cmath>
#include <TGraph2D.h>
#include <TGraph.h>
#include <TF2.h>

#define BILAYER

class bilayer{

 public:
 	bilayer(char* nomefile);

 	double RRR();  //average RRR
 	double RRRe(); //average RRR error

 	TF2* fitRT();
 	TF2* fitLT();

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