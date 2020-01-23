/**********************************************
* Header classe "fdat" lettura file dati ASCII 
* formato *.dat salvati con LabView 
* Ver 1.2 autore Michele Biasotti
************************************************/
/////////////////////////////////////////////////
/****** include solo libreire standard *********/

#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <string>
#ifndef FILELIST
#include <filelist.h>
#endif

#define BASE

using namespace std;

class fdat 
{
 private:
  
  vector<double> dati; 	// Vettore dove vengono caricati i dati 
						
  int k;      // Numero dati per riga
  int l;      // numero di righe
  int n;      // numero totale di dati
   
  bool error; //segnala errore

  double sampling;		
		
 public:	
		
  fdat(const char * nomef); //costruttore legge un file dati ASCII
  fdat(const fdat &old);              // costruttore di copia	
  fdat operator = (const fdat &old);  //assegnazione		


  //****************** Funzioni inline lettura variapili pravate
	
  inline double dat(int a, int b) {return dati.at(a*k+b);}; // restitusce l'elelemto riga a colonna b	    
  inline double dat(int a) {return dati.at(a);}; // restitusce l'elemento a-esimo attacando tutte le righe   
  inline double operator[] (int a) {return dati.at(a);}; // restitusce l'elemento a-esimo attacando tutte le righe
	inline double T() {return TmK;};
  inline double s() {return sampling;};

  inline int np() {return l;};    // restituisce il numero di "punti" ovvero il numero di linee dei dati.
  inline int nc() {return k;};    //  restituisce il numeri di numeri per linea.
  inline int nn() {return l*k;};  // numero totale vaori caricati
  
  inline bool notfile() {return error;}; // resituisce vero se manca il file 


  //******************* Dichiarazione Metodi
  
  double sum(int a);   // restituisce la somma della a-esima colonna    
  double media(int a); // restituisce la media della a-esima colonna

  
  //***************** altre variabili membro

  double bias,  Tohm;
  double TmK;
  double amp,squid;
  double shunt;
  double Rc;
	    
	    
};
