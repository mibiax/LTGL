#include <bilayer.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TAxis.h>

const double TC = 100; //temperatura critica richiesta (mK)

#define ARATIO 9.6 //fattore geometrico + parallelo 
#define RN 3e-2 //resistenza normale (ohm)

using namespace std;

TApplication app("app",NULL,NULL);



int main(int argn, char **argv){
	char* resRT;
	char* resLT;

	bool r = false, l = false;
  bool t = false, f = false;
  
  int opt;

  string path;
  string arg0(argv[0]);

  switch (argn){
    default:
      for (int i=1; i<argn-1; i++){
        if (argv[i][0]=='-'){
          opt = strlen(argv[i]);
          for (int j=1; j<opt; j++){
            switch (argv[i][j]) {
              case 'r': r = true; break; //
              case 'l': l = true; break; //
              case 't': t = true; break; //
              default: ;
            };
          };
        };
      }; 
      resRT = argv[2];
      break;
    case 2: 
      resRT = argv[1];
      break;
    case 1: ;
    case 0:
      cout<<arg0<<" [opt] <filename>"<<endl;
      exit(1);
  };


  bilayer dd(resRT);

  double rrr = dd.RRR();
  double rrre = dd.RRRe();



  TF2* RT = dd.fitRT();
  TF2* LT = dd.fitLT();
  TF1* Tc = dd.fitTc();

  double ratio = Tc->Eval(TC);

  double Ir = RT->GetParameter("Ir")*1e-12; 
  double Au = RT->GetParameter("Au")*1e-12;
  double eIr = RT->GetParError(0)*1e-12;
  double eAu = RT->GetParError(1)*1e-12;
  double rIr = eIr/Ir;
  double rAu = eAu/Au;
  double tAu = Au*Ir/(RN*ARATIO*rrr*(Au*ratio+Ir));
  double tIr = tAu*ratio;

  double err = rIr+rAu+(eAu+eIr)/(Au*ratio+Ir);

  double Ir2 = LT->GetParameter("Ir")*1e-12; 
  double Au2 = LT->GetParameter("Au")*1e-12;
  double eIr2 = LT->GetParError(0)*1e-12;
  double eAu2 = LT->GetParError(1)*1e-12;
  double rIr2 = eIr2/Ir2;
  double rAu2 = eAu2/Au2;
  double tAu2 = Au2*Ir2/(RN*ARATIO*(Au2*ratio+Ir2));
  double tIr2 = tAu2*ratio;


  double err2 = rIr2+rAu2+(eAu2+eIr2)/(Au2*ratio+Ir2);

  double rrrIr = Ir/Ir2;
  double rrrAu = Au/Au2;
  double errrIr = sqrt(pow((eIr/Ir),2)+pow((eIr2/Ir2),2))*Ir/Ir2;
  double errrAu = sqrt(pow((eAu/Au),2)+pow((eAu2/Au2),2))*Au/Au2;

  TGraph2D* plot1 = dd.plotRT(); 
  TGraph2D* plot2 = dd.plotLT(); 
  TGraph2D* plot3 = dd.plotTc2D();
  TGraph* plot4 = dd.plotTc();

  cout<<"ratio Tc = "<<TC<<" -> "<<ratio<<endl;

  cout<<endl
  <<"res.\tRT\tLT\tRRR"<<endl
  <<setprecision(3)
  <<"Ir\t"<<Ir<<"+/-"<<eIr<<"\t"<<Ir2<<"+/-"<<eIr2<<"\t"<<rrrIr<<endl
  <<"Au\t"<<Au<<"+/-"<<eAu<<"\t"<<Au2<<"+/-"<<eAu2<<"\t"<<rrrAu<<endl;


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
  
  plot1->Draw("P0 tri");
  RT->Draw("same");
  
  TCanvas c2;
  plot2->Draw("P0 tri1");
  LT->Draw("same");


  if (r) {
    TCanvas* cr = new TCanvas;
    dd.hRRR()->Draw();
  };

  if (t) {
    TCanvas* c3 = new TCanvas;
    plot3->Draw("P0 tri1");

    TCanvas* c4 = new TCanvas;
    plot4->Draw("AP");
    Tc->Draw("same");
  };


  app.Run("true");

	return 0;
};