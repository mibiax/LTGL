#include <rt.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TMultiGraph.h>
#include <cstring>

#define RTDIR0 "./"
#define RTDIR "./"

TApplication app("app",NULL,NULL);

int main(int argn, char** argv){

  bool r = false, e = false;
  bool d = false, f = false;
  bool K = false, a = false;
  bool x = false, y = false; 
  bool c = false, C = false;
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
              case 'r': r = true; break; //Raw file
              case 'e': e = true; break; //Error bars
              case 'd': d = true; break; //all Directory files
              case 'f': f = true; break; //fit transition (only mK version)
              case 'y': y = true; break; //resistance offset subtraction
              case 'x': x = true; break; //temperature shift
              case 'K': K = true; break; //plot in Kelvin (not mK)
              case 'a': a = true; break; //plot alpha
              case 'c': c = true; break; //use different calibration
              case 'C': C = true; break; //show calibration used
              default: ;
            };
          };
        };
      };	
  	if (d) goto serie;
  	case 2: goto singolo;
    case 1: ;
    case 0:
      cout<<arg0<<" [opt] <filename>"<<endl
      <<endl<<" Optinos:"<<endl
      <<"\t-r Plot raw data R vs. R termometer\n"
      <<"\t-e Plot with error bars\n"
      <<"\t-d Plot all *.dat file in a directory\n"
      <<"\t-f Fit transition (only mK version)\n"
      <<"\t-y Resistance offset subtraction\n"
      <<"\t-x Temperature shift\n"
      <<"\t-K Plot in Kelvin (not mK)\n"
      <<"\t-a Plot alpha\n"
      <<"\t-c Use a different termometer calibration\n"
      <<"\t-C Show calibration used\n"
      <<endl;
      exit(1);
  };

    
 serie: {
    cout << "serie" <<endl;
    path = RTDIR + string(argv[2]);
    filelist lista(path);
    if (lista.nofile()) return 1;
    cout<<lista.nfile()<<" file trovati"<<endl;
    
    
    for (int i=0; i<lista.nfile(); i++) {
      cout << lista.s(i)<< endl;
    };
    
    RT* misure[lista.nfile()];
    
    for (int i=0; i<lista.nfile(); i++) misure[i] = new RT(lista.s(i));  
    
    TMultiGraph mplot("rt","R(T)");

    if (e) {  
      if (K) {
        for (int i=0; i<lista.nfile(); i++) {
          TGraph* plot = misure[i]->Kplot_err();
          plot->SetMarkerColor(i+1);
          mplot.Add(plot);
        };
      } else {
          for (int i=0; i<lista.nfile(); i++) {
            TGraph* plot = misure[i]->plot_err();
            plot->SetMarkerColor(i+1);
            mplot.Add(plot);
          };
        };
    } else {
      if (K) {
        for (int i=0; i<lista.nfile(); i++) {
          TGraph* plot = misure[i]->Kplot();
          plot->SetMarkerColor(i+1);
          mplot.Add(plot);
        };
      } else {
        for (int i=0; i<lista.nfile(); i++) {
          TGraph* plot = misure[i]->plot();
          plot->SetMarkerColor(i+1);
          mplot.Add(plot);
        };
      };
    };

    if (K) mplot.SetTitle("R(T); T (K); R (#Omega)");
      else mplot.SetTitle("R(T); T (mK); R (#Omega)");

    TCanvas cc0("multiplot",0,0);
    mplot.Draw("APL");

    
    if (r) {
      TCanvas* cc1 = new TCanvas("raw",0,0);

      misure[0]->rawplot()->Draw("AP");
      for (int i=1; i<lista.nfile(); i++) {
        TGraph* plot = misure[i]->rawplot();
        plot->SetMarkerColor(i+2);
        plot->Draw("sameP");
      };
    };

    app.Run(true);

    return 0; 
  };
  //***********************************************/

  singolo: //etichetta

  path = RTDIR0 + string(argv[argn-1]);
  cout << path <<endl;  
    
  RT prova(path);

  if (C) {
    TGraph* gr = prova.calplot();
    TCanvas p;
    gr->Draw("A*L");
    app.Run(true);
    delete gr;
  };

  if (c) {
    system("ls *.cal*");
    string calfile;
    cin>>calfile;
    cal plot = prova.recal(calfile);
    if (plot.plot != NULL) {
      TCanvas p;
      plot.plot->Draw("A*L");
      plot.ht->Draw("same");
      plot.lt->Draw("same");
      app.Run(true);
      delete plot.plot;
      delete plot.ht;
      delete plot.lt;
    };
  };

  if (y) {
    double val;
    cout<<"Value (offset) to subtract (ohm): ";
    cin>>val;
    cout<<endl;
    prova.subtract(val);
  };

  if (x) {
    double val;
    cout<<"Value to shift (mK): ";
    cin>>val;
    cout<<endl;
    prova.shift(val);
  };

  if (r) {
    TCanvas* cr = new TCanvas("raw",0,0);
    prova.rawplot()->Draw("APL");
  };


  if (a) {
    TCanvas* ca = new TCanvas("alpha",0,0);
    prova.alpha()->Draw("APL");
  };


  TCanvas* c0 = new TCanvas("ready",0, 0);
  if (e) {  
    if (K) prova.Kplot_err()->Draw("APL");
      else prova.plot_err()->Draw("APL");
  } else {
    if (K) prova.Kplot()->Draw("APL");
      else prova.plot()->Draw("APL");
  };

  if (f) {
    TrPar p = prova.fitpar();

    cout <<"Rp = "<<p.Rp<<"\t+/- "<<p.dRp<<" ohm"<<endl;
    cout <<"Rn = "<<p.Rn<<"\t+/- "<<p.dRn<<" ohm"<<endl;
    cout <<" l = "<<p.l<<"\t+/- "<<p.dl<<endl;
    cout <<"Tc = "<<p.Tc<<"\t+/- "<<p.dTc<<" mK"<<endl<<endl;
    cout<< "Tc (90%) = "<<prova.Tc90()<<" mK"<<endl;
    cout<< "Tc (10%) = "<<prova.Tc10()<<" mK"<<endl;
    prova.fit()->Draw("same");
  };

  app.Run(true);

  return 0;
};

