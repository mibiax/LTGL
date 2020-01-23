/**********************************************
* Sorgente classe "fdat" lettura file dati ASCII 
* formato *.dat salvati con LabView 
* Ver 1.2 autore Michele Biasotti 
************************************************/
/////////////////////////////////////////////////
/****** include solo libreire standard *********/


#include <base.h>


using namespace std;

 //il costruttore legge un file dati e carica i valori in un vettore dinamico "dati"
fdat::fdat(const char* nomef) {
  string riga;		// variabile di transito per la singola riga 	 
  
  k=0;    // Numero dati per riga
  l=0;	// numero di righe
  			
  bias=0;
  Tohm=0;
  TmK=0;
  amp=0;
  squid=0;
  shunt=0;
  Rc=0;
  sampling=0;
		 
  fstream filedati(nomef); // apriamo il file
  error=!filedati.is_open(); //guardo se è aperto
  if (error) return;
	
	
  while (!filedati.eof()){  // fino a quando il file non è finito	
    
  	getline(filedati,riga); //leggo un a riga dal file, tollero il primo carattere diverso da numero seguito da spazio
  
    if (riga.find("[G]")==0) {// leggo guadagni
	
	  getline(filedati,riga); 
	  istringstream dariga; //creo buffer stringa
	  if (!isdigit(riga[0])) riga.erase(0,2);
	  dariga.str(riga);     // riempo buffer 
	  dariga >> squid;
				    		  
	  getline(filedati,riga);
	  if (!isdigit(riga[0])) riga.erase(0,2);
	  dariga.str(riga);     // riempo buffer 
	  dariga>> amp;
	  continue;  
	};
  
    if (riga.find("[T]")==0){     //leggo Temperatura
 
      getline(filedati,riga);
	  istringstream dariga; //creo buffer stringa
      if (riga.find("NaN")!=0) {
	    if (!isdigit(riga[0])) riga.erase(0,2);
	    dariga.str(riga);     // riempo buffer 
	    dariga >> TmK;
	  } else TmK=0;
				      
	  getline(filedati,riga);
	  if (!isdigit(riga[0])) riga.erase(0,2);
	  dariga.str(riga);     // riempo buffer 
	  dariga >> Tohm;
	  continue;
	};

    if (riga.find("[R]")==0){ //leggo resistenze
   		{
	  		getline(filedati,riga);
	  		istringstream dariga; //creo buffer stringa
	  		if (!isdigit(riga[0])) riga.erase(0,2);
	  		dariga.str(riga);     // riempo buffer 
	  		dariga >> shunt;
	 	};
   		{
   			istringstream dariga;	    				   
	  		getline(filedati,riga);
	  		if (!isdigit(riga[0])) riga.erase(0,2);
	  		dariga.str(riga);     // riempo buffer 
	  		dariga >> Rc;
   		};
	  continue;
	};
				    
				
    if (riga.find("[working point]")==0){ //leggo dati punto di lavoro Ib, T, Tohm, // non funziona!!!
    //cout << "trovato working point"<<endl;
    	{
	   		istringstream wp; //creo buffer stringa
	   		getline(filedati,riga);
	   		if (!isdigit(riga[0])) riga.erase(0,2);
	   		wp.str(riga); 
	   		wp >> bias;
    	};
				 		
    	{
     		istringstream wp;         		
	   		getline(filedati,riga);
	   		if (!isdigit(riga[0])) riga.erase(0,2);
	   		wp.str(riga);     // riempo buffer 
     		wp >> TmK;
    	};

	  	{
     		istringstream wp;
     		getline(filedati,riga);
	   		if (!isdigit(riga[0])) riga.erase(0,2);
     		wp.str(riga);     // riempo buffer 
	   		wp >> Tohm;
    	};
    
	  	continue;
	};

	if (riga.find("[sample time]")==0) {
		getline(filedati,riga);
		istringstream dariga; //creo buffer stringa
	  	if (!isdigit(riga[0])) riga.erase(0,2);
	  	dariga.str(riga);     // riempo buffer 
	  	dariga >> sampling;
	  	//cout<<"trovato salmpling time "<<sampling<<endl; //controllo
	  	continue;
	};
				  	 	
    if (riga.find("[dati]")==0||riga.find("[dat]")==0){ // individuo i dati
    		
		while (!filedati.eof()) {
	    	getline(filedati,riga); // leggo una riga
						
	      	istringstream dariga; //creo buffer stringa
	      	dariga.str(riga); // riempo buffer 
	      	//cout<<riga<<'\n'; // controllo
	      	int i=0;
        	string parola;
	      	while(dariga>>parola) {	
      			double val=atof(parola.c_str());			
		  		//if (!dariga.good()) val=NAN;
		  		//cout<<val<< " cicli->"<< i <<'\n';	 // controllo
		  		dati.push_back(val); // lo carico nel vettore
		  		i++; 
			};											
	    	if (k<i) k=i;
		};
		break;
	};
				
 };
	     
		
			
  filedati.close();  	// chiudo il file dati
  n = dati.size();   	// controllo quanti dati ho ltto 
	     				
  l=n/k;  
};

    
fdat::fdat(const fdat &old) {
  dati = old.dati;
  n = old.n;
  l = old.l;
  k = old.k;
  amp = old.amp;
  squid = old.squid;
  bias=old.bias;
  TmK=old.TmK;
  Tohm=old.Tohm;
  sampling=old.sampling;
};
		
fdat fdat::operator = (const fdat &old) {
  dati = old.dati;
  n = old.n;
  l = old.l;
  k = old.k;
  amp = old.amp;
  squid = old.squid;
  bias=old.bias;
  TmK=old.TmK;
  Tohm=old.Tohm;
  sampling=old.sampling;
};
		
double fdat::sum(int a) {
  double b=0;
  for (int i=0;i<l;i++) b=b+dati[i*k+a];
  return b;
};
	    
double fdat::media(int a) {return (sum(a)/l);};

