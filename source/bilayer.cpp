
#include <bilayer.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TAxis.h>

#define ARATIO 9.6 //fattore geometrico + parallelo
#define RATIO 1.5 //rapporto Ir/Au
#define RN 3e-2 //resistenza normale

using namespace std;

TApplication app("app",NULL,NULL);


int main(int argn, char **argv){
	char* resRT;
	char* resLT;

	bool sf = false;

	switch (argn){
  	default:
     	resLT = argv[2];
      	sf = true;
  	case 2:
  		resRT = argv[1];
  		break;
    case 1: ;
    case 0:
      cout<<"<nomefile>"<<endl;
      exit(1);
  };

  bilayer dd(resRT);

  double rrr = dd.RRR();
  double rrre = dd.RRRe();


  TF2* RT = dd.fitRT();
  TF2* LT = dd.fitLT();

  double Ir = RT->GetParameter("Ir")*1e-12; 
  double Au = RT->GetParameter("Au")*1e-12;
  double eIr = RT->GetParError(0)*1e-12;
  double eAu = RT->GetParError(1)*1e-12;
  double rIr = eIr/Ir;
  double rAu = eAu/Au;
  double tAu = Au*Ir/(RN*ARATIO*rrr*(Au*RATIO+Ir));
  double tIr = tAu*RATIO;

  double err = rIr+rAu+(eAu+eIr)/(Au*RATIO+Ir);

  double Ir2 = LT->GetParameter("Ir")*1e-12; 
  double Au2 = LT->GetParameter("Au")*1e-12;
  double eIr2 = LT->GetParError(0)*1e-12;
  double eAu2 = LT->GetParError(1)*1e-12;
  double rIr2 = eIr2/Ir2;
  double rAu2 = eAu2/Au2;
  double tAu2 = Au2*Ir2/(RN*ARATIO*(Au2*RATIO+Ir2));
  double tIr2 = tAu2*RATIO;


  double err2 = rIr2+rAu2+(eAu2+eIr2)/(Au2*RATIO+Ir2);

  double rrrIr = Ir/Ir2;
  double rrrAu = Au/Au2;
  double errrIr = sqrt(pow((eIr/Ir),2)+pow((eIr2/Ir2),2))*Ir/Ir2;
  double errrAu = sqrt(pow((eAu/Au),2)+pow((eAu2/Au2),2))*Au/Au2;

  TGraph2D* plot1 = dd.plotRT(); 
  TGraph2D* plot2 = dd.plotLT(); 
  TGraph2D* plot3 = dd.plotTc2D();
  TGraph* plot4 = dd.plotTc();


  cout
  <<endl
  <<"spessori attesi:"<<endl
  <<"RT\t"<<tIr*1e9<<" nm di Ir\t"<<tAu*1e9<<" nm di Au\t "<<err*100<<"%"<<endl
  <<"LT\t"<<tIr2*1e9<<" nm di Ir\t"<<tAu2*1e9<<" nm di Au\t "<<err2*100<<"%"<<endl
  <<endl
  <<"RRR\t"      <<rrr   <<"\t+/-\t"<<rrre                        <<"\t("<<rrre/rrr*100<<"%)"<<endl
  <<"RRR-Ir\t"   <<rrrIr<<"\t+/-\t"<<errrIr<<"\t("<<errrIr/rrrIr*100<<"%)"<<endl
  <<"RRR-Au\t"   <<rrrAu<<"\t+/-\t"<<errrAu<<"\t("<<errrAu/rrrAu*100<<"%)"<<endl;

  TCanvas c1;
  plot1->Draw("P0 tri1");
  RT->Draw("same");

  TCanvas c2;
  plot2->Draw("P0 tri1");
  LT->Draw("same");

  TCanvas c3;
  plot3->Draw("P0 tri1");

  TCanvas c4;
  plot4->Draw("AP");


  app.Run("true");

	return 0;
};