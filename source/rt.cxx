#include <rt.h>

RT::RT(string nomefile){

  fdat dati(nomefile.c_str());
  
  n = dati.np();
 // cout << "punti: " <<n<<endl;
  
  Tohm = new double[n];
  dTohm = new double[n];
  TmK = new double[n];
  R = new double[n];
  dR = new double[n]; 
  dTmK = new double[n];
  
  for (int i=0; i<n; i++) {
    Tohm[i] = dati.dat(i,0);
    dTohm[i] = dati.dat(i,1);
    R[i] = dati.dat(i,2);
    dR[i] = dati.dat(i,3);
    TmK[i]= dati.dat(i,4);
    dTmK[i] = dTohm[i]/Tohm[i]*TmK[i];
  };
  
  return;
};

RT::RT(double* T, double* Ra, double* dT, double* dRa){
  if (sizeof(*T)<=sizeof(*Ra))
    n = sizeof(*T)/sizeof(double);
  else
    n = sizeof(*Ra)/sizeof(double);

  Tohm = NULL;
  dTohm = NULL;
  TmK = new double[n];
  dTmK = new double[n];
  R = new double[n];
  dR = new double[n];

  for (int i=0; i<n; i++) { //trasferisco ivalori
    TmK[i]  =  T[i];
    dTmK[i] = dT[i];
      R[i]  = Ra[i];
     dR[i]  = dRa[i]; 
  };
};

RT::RT(double *T, double *Ra){
  if (sizeof(*T)<=sizeof(*Ra))
    n = sizeof(*T)/sizeof(double);
  else
    n = sizeof(*Ra)/sizeof(double);

  Tohm = NULL;
  dTohm = NULL;
  TmK = new double[n];
  dTmK = NULL;
  R = new double[n];
  dR = NULL;  

  for (int i=0; i<n; i++) { //trasferisco ivalori
    TmK[i] =  T[i];
      R[i] = Ra[i];
  };
};

RT::RT(){
  n = 0;
  
  Tohm = NULL;
  dTohm = NULL;
  TmK = NULL;
  dTmK = NULL;
  R = NULL;
  dR = NULL;

 return;
};

void RT::subtract(double val){
  for (int i=0; i<n; i++)
    R[i] = R[i] - val;
};

void RT::shift(double val){
  for (int i=0; i<n; i++)
    TmK[i] = TmK[i] + val;
};

TGraphErrors* RT::plot_err() {
  TGraphErrors* ready = new TGraphErrors(n, TmK, R, dTmK, dR);
  ready -> SetMarkerStyle(20);
  ready -> SetTitle("R(T);  T (mK); R (#Omega)");
  return ready;
};

TGraph* RT::rawplot(){
  TGraph* raw   = new TGraph(n, Tohm, R);
  raw   -> SetMarkerStyle(20);
  raw -> SetTitle("R(T) raw data;  T (#Omega); R (#Omega)");
  return raw;
};

TGraph* RT::calplot(){
  TGraph* cal   = new TGraph(n, Tohm, TmK);
  cal -> SetMarkerStyle(20);
  cal -> SetTitle("calibration data;  R (#Omega); T (mK)");
  return cal;
};

TGraph* RT::plot(){
  TGraph* ready = new TGraph(n, TmK, R);
  ready -> SetMarkerStyle(20);
  ready -> SetTitle("R(T);  T (mK); R (#Omega)");
  return ready; 
};

TGraph* RT::Kplot(){    //Plot in Kelvin
  double T[n];
  for (int i=0; i<n; i++)
    T[i] = TmK[i]/1000;
  
  TGraph* ready = new TGraph(n, T, R);
  ready -> SetMarkerStyle(20);
  ready -> SetTitle("R(T);  T (K); R (#Omega)");
  return ready;
};

TGraphErrors* RT::Kplot_err(){    //Plot in Kelvin
  double T[n];
  double dT[n];
  for (int i=0; i<n; i++) {
     T[i] = TmK[i]/1000;
    dT[i] = dTmK[i]/1000;
  };
  
  TGraphErrors* ready = new TGraphErrors(n, T, R, dT, dR);
  ready -> SetMarkerStyle(20);
  ready -> SetTitle("R(T);  T (K); R (#Omega)");
  return ready;
};


double RT::alpha(int j){ //alpha valua at j array position
  double a[n];
  a[0] = (R[1]-R[0])/(TmK[1]-TmK[0])*TmK[0]/R[0];
  for (int i=1; i<n-1; i++)
    a[i]=(R[i+1]-R[i-1])/(TmK[i+1]-TmK[i-1])*TmK[i]/R[i];
  a[n] = (R[n]-R[n-1])/(TmK[n]-TmK[n-1])*TmK[n]/R[n];
  return a[j]; 
};

TGraph* RT::alpha(){ //alpha value plot
  double a[n];
  
  for (int i=1; i<n-1; i++)
    a[i]=alpha(i);

  TGraph* ready = new TGraph(n, TmK, a);
  //ready -> SetDrawOption("AC");
  //ready -> SetMarkerStyle(20);
  ready -> SetLineWidth(5);
  ready -> SetLineColor(4);
  ready -> SetTitle("#alpha(T);  T (mK);  #alpha");
  
  return ready; 
};

double RT::TalphaMax(){
  int      k = 0;
  double val = 0;
  for (int i=0; i<n; i++) {
    if (alpha(i)>=val) {
      val = alpha(i);
      k = i;
    };
  };
  return TmK[k];
};

double RT::Tc10(){
  TrPar p = fitpar();
  return inv(p, p.Rp+p.Rn*0.1);
};

double RT::Tc90(){
  TrPar p = fitpar();
  return inv(p, p.Rp+p.Rn*0.9);
};

double RT::Tc(){
  TrPar p = fitpar();
  return p.Tc;
};

TF1* RT::fit(){
  int      k = 0;
  int      m = 0;
  double val = 0;
  for (int i=0; i<n; i++) {
    if (TmK[i]>=val) {
      val = TmK[i];
      k = i;
    };
  };
  val = 0;
  for (int i=0; i<n; i++) {
    if (R[i]>=val) {
      val = R[i];
      m = i;
    };
  };
  TF1* RTfit = new TF1("trans","[0]+[1]/(1+exp([2]*(1-x/[3])))",0,TmK[k]);
  RTfit->SetParName(0, "Rp"); 
  RTfit->SetParName(1, "Rn");
  RTfit->SetParName(2, "l");
  RTfit->SetParName(3, "Tc");   
  RTfit->SetParameter(0, 0); 
  RTfit->SetParameter(1, R[m]);
  RTfit->SetParameter(2, 10);
  RTfit->SetParameter(3, TalphaMax());
  RTfit->SetParLimits(0, 0, R[m]);
  RTfit->SetParLimits(1, 0, R[m]);
  //RTfit->SetParLimits(2, 0, 1000);
  RTfit->SetParLimits(3, 0, TmK[k]);
  if (dR==NULL||dTmK==NULL)
    plot()->Fit(RTfit,"W0Q","",0,TmK[k]);
  else
    plot_err()->Fit(RTfit,"0Q","",0,TmK[k]);
  return RTfit;
};

TrPar RT::fitpar(){
  TF1* dummy = fit();
  TrPar out;
  out.Rp = dummy->GetParameter(0);
  out.Rn = dummy->GetParameter(1);
  out.l = dummy->GetParameter(2);
  out.Tc = dummy->GetParameter(3);
  out.dTc = dummy->GetParError(3);
  out.dRp = dummy->GetParError(0);
  out.dRn = dummy->GetParError(1);
  out.dl = dummy->GetParError(2);
  out.dTc = dummy->GetParError(3);
  delete dummy;
  return out;
};

TF1* RT::inv(){
  TrPar param = fitpar();
  TF1* out = new TF1("inv","[0]*(1+[1]*log((x-[2])/([2]+[3]-x)))",param.Rp,param.Rn+param.Rp);
  out->SetParameter(0, param.Tc);
  out->SetParameter(1, 1/param.l);
  out->SetParameter(2, param.Rp);
  out->SetParameter(3,param.Rn);
  return out;
};

TF1* RT::inv(TrPar p){
  TF1* out = new TF1("inv","[0]*(1+[1]*log((x-[2])/([2]+[3]-x)))",p.Rp,p.Rn+p.Rp);
  out->SetParameter(0, p.Tc);
  out->SetParameter(1, 1/p.l);
  out->SetParameter(2, p.Rp);
  out->SetParameter(3,p.Rn);
  return out;
};

double RT::inv(double r){
  TF1* dummy = inv();
  double out = dummy->Eval(r);
  delete dummy;
  return out;
};

double RT::inv(TrPar p, double r){
  TF1 out("inv","[0]*(1+[1]*log((x-[2])/([2]+[3]-x)))",p.Rp,p.Rn+p.Rp);
  out.SetParameter(0, p.Tc);
  out.SetParameter(1, 1/p.l);
  out.SetParameter(2, p.Rp);
  out.SetParameter(3,p.Rn);

  return out.Eval(r);
};

cal RT::recal(string nomefile, double hcut, double lcut){
  fdat calib(nomefile.c_str());
  if (calib.notfile()) {
    cout<<"No calibration file"<<endl;
    cal N = {NULL, NULL, NULL};
    return N;
  };

  const int m = calib.np();
  double res[m];
  double temp[m];
  double etemp[m];


  for (int i=0; i<m; i++){
    res[i]=calib.dat(i,0);
    temp[i]=calib.dat(i,1);
  };

  double maxT = TMath::MaxElement(m,temp);
  double minT = TMath::MinElement(m,temp);

  double maxR = TMath::MaxElement(m,res);
  double minR = TMath::MinElement(m,res);

  TGraph* gr = new TGraph(m,res,temp);
  TF1* ht = new TF1("ht","[0]/(log(x/[1]))^[2]",1,hcut);
  TF1* lt = new TF1("lt","[0]/(log(x/[1]))^[2]",lcut,maxR);
  ht->SetParameter(0,10);
  ht->SetParameter(1,1);
  ht->SetParameter(2,4);

  lt->SetParameter(0,10);
  lt->SetParameter(1,1);
  lt->SetParameter(2,2);

  gr->Fit(ht,"W","",minR,hcut);
  gr->Fit(lt,"w","",lcut,maxR);

  for (int i=0; i<n; i++){
    if (Tohm[i]>minR && Tohm[i]<maxR) TmK[i]=gr->Eval(Tohm[i]);
      else if (Tohm[i]<minR) TmK[i]=ht->Eval(Tohm[i]);
        else TmK[i]=lt->Eval(Tohm[i]);
    if (dTohm != NULL) dTmK[i]=dTohm[i]/Tohm[i]*TmK[i];
  };

  cal out;

  out.plot = gr;
  out.ht = ht;
  out.lt = lt;
  return out;
};

RT::~RT(){
  delete[] Tohm, dTohm, TmK, dTmK, R, dR;
};



