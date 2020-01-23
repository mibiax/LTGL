#include <tools.h>

double Pfnc(double* x, double *par) {
  double out = 0; 
  if (x[0]<par[0]) 
    out=0;
  else 
    out = par[1]*(exp((-x[0]+par[0])/par[2])-exp((-x[0]+par[0])/par[3]));
  return out;

};


double Pfnc2(double* x, double *par) {
  double out = 0; 
  if (x[0]<par[0]) 
    out=0;
  else 
    out = par[1]*(exp((-x[0]+par[0])/par[2])-exp((-x[0]+par[0])/par[3]))+par[4]*(exp((-x[0]+par[0])/par[5])-exp((-x[0]+par[0])/par[6]));
  return out;

};


pulse::pulse(TGraph* a, double tmax) {
  
  int n = a->GetN();
  double* X = a->GetX();
  double trange = X[n-1]; // !!!!
  double* Y = a->GetY();
  double max = *max_element(Y,Y+n);
  l = tmax;

  //cout<<"tmax: "<< tmax <<"\tmax: "<<max<<"\t n:"<<n<<"\t time range:"<<trange<<endl;

  p = new TF1("Pulse0",Pfnc, 0, trange, PAR); 
  
  p->SetParName(0,"t0");
  p->SetParName(1,"A");
  p->SetParName(2,"Decay");
  p->SetParName(3,"Rise");

  p->SetParLimits(0,tmax/2,tmax); //limito izio impulso prima del massimo
  p->SetParLimits(1,0,SAT);  //limito il massimo valore
  p->SetParLimits(2,0,DEF);
  p->SetParLimits(3,0,DEF);

  p->SetParameter(0,tmax-3e-6); //arretro di 3 us
  p->SetParameter(1,max);
  p->SetParameter(2,2e-6);
  p->SetParameter(3,3e-5);

  a->Fit(p,"WR0Q",0, trange);

  return;
};


pulse2::pulse2(TGraph* a, pulse* b) {


  int n = a->GetN();
  double* X = a->GetX();
  double tmax = X[n-1];
  double max = a->GetMaximum(); 

  
  p  = new TF1("Pulse3",Pfnc2, 0, tmax, PAR2); 
  p1 = new TF1("Pulse1",Pfnc, 0, tmax, PAR); 
  p2 = new TF1("Pulse2",Pfnc, 0, tmax, PAR); 
  
  p->SetParName(0,"t0");
  p->SetParName(1,"A");
  p->SetParName(2,"Decay");
  p->SetParName(3,"Rise");
  p->SetParName(4,"B");
  p->SetParName(5,"Decay2");
  p->SetParName(6,"Rise2");

  p->SetParLimits(0,0,tmax); //limito izio impulso prima del massimo
  p->SetParLimits(1,0,SAT); //limito il massimo valore
  p->SetParLimits(4,0,SAT); //limito il massimo valore

  p->SetParameter(0,b->out()->GetParameter(0));
  p->SetParameter(1,max);
  p->SetParameter(2,b->out()->GetParameter(2));
  p->SetParameter(3,b->out()->GetParameter(3));
  p->SetParameter(4,max);
  p->SetParameter(5,b->out()->GetParameter(2)*2);
  p->SetParameter(6,b->out()->GetParameter(3)*2);
  p->SetLineColor(3);

  a->Fit(p,"WR",0, b->range());

  p1->SetParameter(0,p->GetParameter(0));
  p1->SetParameter(1,p->GetParameter(1));
  p1->SetParameter(2,p->GetParameter(2));
  p1->SetParameter(3,p->GetParameter(3));
  p1->SetParName(0,"t0");
  p1->SetParName(1,"A");
  p1->SetParName(2,"Decay");
  p1->SetParName(3,"Rise");
  p1->SetLineColor(2);
  
  p2->SetParameter(0,p->GetParameter(0));
  p2->SetParameter(1,p->GetParameter(4));
  p2->SetParameter(2,p->GetParameter(5));
  p2->SetParameter(3,p->GetParameter(6));
  p2->SetParName(0,"t0");
  p2->SetParName(1,"B");
  p2->SetParName(2,"Decay2");
  p2->SetParName(3,"Rise2");
  p1->SetLineColor(4);

  return;
};


