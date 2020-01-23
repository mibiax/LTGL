#include <base.h>
#include <TGraphErrors.h>

class FP {
    
  public:
    FP(string nomefile);
    FP();
        
    inline double x(int i)   {return X[i];};
    inline double y(int i)   {return Y[i];};
    inline double* x()       {return X;};
    inline double* y()       {return Y;};

    TGraph* plot();
     
    ~FP();

 protected:
    int n;
    double* X; 
    double* Y; 
};
