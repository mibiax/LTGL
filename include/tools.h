/********************** 
 * tools di calcolo   *
 **********************/

#include <cmath>
#include <algorithm>
#include <TF1.h>
#include <TGraph.h>

#define PAR  4
#define PAR2 7
#define DEF 2e-4
#define SAT 5

using namespace std;

double Pfnc(double* x, double *par);  //impulso
double Pfnc2(double* x, double *par); //doppio impulso inizio diverso

class pulse {
	public:
		pulse(TGraph* a, double lim);

		inline TF1*        out() {return p;};
		inline double    range() {return l;};
		inline double integral() {return p->Integral(0,l);};
		inline double 	  rise() {return p->GetParameter("Rise");};
		inline double 	 decay() {return p->GetParameter("Decay");};

		friend class dpulse;

	protected:
		double l;
		TF1* p;

};

class pulse2 {
public:
	
	pulse2(TGraph* a, pulse* b);
	
	inline TF1*   out() {return p;};
	inline TF1*  out1() {return p1;};
	inline TF1*  out2() {return p2;};

	friend class dpulse;


protected:
	TF1* p;
	TF1* p1;
	TF1* p2;

};