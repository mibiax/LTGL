#include <bilayer.h>

bilayer::bilayer(char* nomefile){
	fdat datiRT(nomefile);

	n = datiRT.np();
	m1 = 0;
	m2 = 0;
  mtc = 0;

	Ir = new double[n];
	Au = new double[n];
	R1 = new double[n];
	R2 = new double[n];
	geof = new double[n];
  Tc = new double[n];

	for (int i=0; i<n; i++){
  	Ir[i] = datiRT.dat(i, 1);//*1e-9;
  	Au[i] = datiRT.dat(i, 2);//*1e-9;
  	R1[i] = datiRT.dat(i, 3);//*1e-3;
  	R2[i] = datiRT.dat(i, 4);//*1e-3;
    geof[i]= datiRT.dat(i, 5);
    Tc[i] = datiRT.dat(i, 6);
  	if (datiRT.dat(i, 3)!=0) m1++;
  	if (datiRT.dat(i, 4)!=0) m2++;
    if (datiRT.dat(i, 6)!=0) mtc++;
  	//cout << Ir[i]<<"\t"<<Au[i]<<"\t"<<R1[i]<<"\t"<<R2[i]<<"\t"<<Tc[i]<<endl;
  };

	return;
};

double bilayer::RRR(){
  
  	int m3=0;
  	double out=0;

  	for (int i=0; i<n; i++){	
  		if (R1[i]==0 || R2[i]==0) continue; 
      if (isnan(R1[i]) || isnan(R2[i])) continue;
  		out=out+R1[i]/R2[i];
  		m3++;
  	};

  	out = out/m3;

	return out;
};

TH1D* bilayer::hRRR() {
  TH1D* out = new TH1D("RRR","RRR",12,1,4);
  //double rrr
  for (int i=0; i<n; i++){  
    if (R1[i]==0 || R2[i]==0) continue; 
    if (isnan(R1[i]) || isnan(R2[i])) continue;
    double r = R1[i]/R2[i]; 
    cout<<"test RRR -> "<<r<<endl;
    out->Fill(r);
  };
  return out;
};

double bilayer::RRRe(){ 
	int m3=0;
	double rrr = RRR();
	double out = 0;

	for (int i=0; i<n; i++){
  	if (R1[i]==0 || R2[i]==0) continue;
    if (isnan(R1[i]) || isnan(R2[i])) continue;
  	double scarto = rrr-R1[i]/R2[i];
  	out = out + scarto*scarto;
    m3++;
  };

  out = sqrt(out/m3);

  return out;
};

TF2* bilayer::fitRT(){

  double x[m1], y[m1], z[m1];
  int j=0;
  for (int i=0; i<n; i++){
  	if (R1[i]==0 || isnan(R1[i])) continue;
  	x[j] = Ir[i];
  	y[j] = Au[i];
  	z[j] = R1[i]*geof[i];
  	j++; 
  };

  TF2* out = new TF2("res1","[0]*[1]/([1]*x+[0]*y)",0,300,0,100);

  out->SetParLimits(0,rhoIr*mnm,100*rhoIr*mnm);
  out->SetParLimits(1,rhoAu*mnm,100*rhoAu*mnm);
  out->SetParameter(0,rhoIr*mnm);
  out->SetParameter(1,rhoAu*mnm);
  out->SetParName(0, "Ir");
  out->SetParName(1,"Au");

  TGraph2D  dati(m1, x, y, z);
  dati.Fit(out);

  return out;

};

TF2* bilayer::fitLT(){

  double x[m2], y[m2], z[m2];
  int j=0;
  for (int i=0; i<n; i++){
  	if (R2[i]==0 || isnan(R2[i])) continue;
  	x[j] = Ir[i];
  	y[j] = Au[i];
  	z[j] = R2[i]*geof[i];
  	j++; 
  };

  TF2* out = new TF2("res2","[0]*[1]/([1]*x+[0]*y)",0,300,0,100);

  out->SetParLimits(0,rhoIr*mnm/10,10*rhoIr*mnm);
  out->SetParLimits(1,rhoAu*mnm/10,10*rhoAu*mnm);
  out->SetParameter(0,rhoIr*mnm);
  out->SetParameter(1,rhoAu*mnm);
  out->SetParName(0, "Ir");
  out->SetParName(1,"Au");

  TGraph2D  dati(m2, x, y, z);
  dati.Fit(out);

  return out;

};

TF1* bilayer::fitTc(){
  TF1* out = new TF1("tc","pol1",0,180);
  //out->SetParLimits(0,0,160);
  //out->SetParLimits(1,0,160);
  //out->SetParLimits(2,0,160);
  //out->SetParameter(0, 80);
  //out->SetParameter(1,0);
  //out->SetParameter(2, 10);
  TGraph* plot = plotTc();
  plot->Fit(out);
  delete plot;
  return out;
};

TGraph2D* bilayer::plotRT(){

  double x[m1], y[m1], z[m1];
  int j=0;
  for (int i=0; i<n; i++){
  	if (R1[i]==0 || isnan(R1[i])) continue;
  	x[j] = Ir[i];
  	y[j] = Au[i];
  	z[j] = R1[i]*geof[i];
  	j++; 
  };

  TGraph2D* out = new TGraph2D(m1, x, y, z);
  out->SetTitle("RT; Ir (nm); Au (nm); R (m#Omega)");

  return out;
};

TGraph2D* bilayer::plotLT(){

  double x[m2], y[m2], z[m2];
  int j=0;
  for (int i=0; i<n; i++){
    if (R2[i]==0 || isnan(R2[i])) continue;
    x[j] = Ir[i];
    y[j] = Au[i];
    z[j] = R1[i]*geof[i];
    j++; 
  };

  TGraph2D* out = new TGraph2D(m1, x, y, z);
  out->SetTitle("LT; Ir (nm); Au (nm); R (m#Omega)");

  return out;
};

TGraph2D* bilayer::plotTc2D(){

  double x[mtc], y[mtc], z[mtc];
  int j=0;
  for (int i=0; i<n; i++){
    if (Tc[i]==0) continue;
    x[j] = Ir[i]/Au[i];
    y[j] = Ir[i];
    z[j] = Tc[i];
    j++; 
  };

  TGraph2D* out = new TGraph2D(j, x, y, z);
  out->SetTitle("Tc; Ir/Au; Ir (nm); (mK)");

  return out;
}


TGraph* bilayer::plotTc(){
  double x[mtc], y[mtc];
  int j=0;
  for (int i=0; i<n; i++){
    if (Tc[i]==0||Au[i]==0) continue;
    y[j] = Ir[i]/Au[i];
    x[j] = Tc[i];
    j++; 
  };

  TGraph* out = new TGraph(j, x, y);
  out->SetMarkerStyle(20);
  out->SetMarkerSize(1);
  out->SetTitle("Tc; Tc (mK); Ir/Au ratio");

  return out;

};


bilayer::~bilayer(){
	delete[] Ir;
	delete[] Au, R1, R2, geof, Tc;
};