//=================================================
// JCard.h
// last modified FK 6.NOV 2009
//=================================================

#ifndef JCARD_H
#define JCARD_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <math.h>

#include <string.h>
#include <TString.h>
#include <TVector.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TFile.h>
#include <vector>
#include <TVector3.h>
#include <map>
//#include <TRandom.h>


#include "JConst.h"
using namespace std;


const int kMaxDimBuffer = 200;//max length of a line read to a buffer


class JCard {



  //====   M e m b e r    F u n c t i o n s   ========

public:
  JCard(char *filename); // constructor
  
  virtual ~JCard(){;}    // destructor

  float  Get(TString keyword, int=0); //get TVector component 
  float  Get(corrType ctype, int=0);  //get TVector component
  int    GetN(TString keyword);       //get TVector dimension
  int    GetN(corrType ctype);        //get TVector dimension

  void   Print(); 
  void   WriteCard(TFile *file);  


  int    GetEventPoolDepth(){ return (int) Get("EventPoolDepth");}

  bool IsLessThanUpperPairPtCut(double inPairPt){ return inPairPt<Get("UpperPairPtCut"); }

  //---- Collision Species --
  int    GetCentralityType() { return (int) Get("CollisionType"); }
  float  GetCollisionType()  { return Get("CollisionType");}
  bool   IsHeavyIon()  { return GetCentralityType() == 0 ; }
  bool   IsppCentNch() { return GetCentralityType() == 1 ; }
  bool   IsppCentBBC() { return GetCentralityType() == 2 ; }



  //--- c o r r e l a t i o n  bins & borders --
  int    GetNoOfBins (corrType ctype){ return GetN(ctype)-1; }
  float  GetBinBorder(corrType ctype, int ii){ return Get(ctype,ii); }
  int    GetBin(corrType ctype, float val);
  double GetBinCenter(corrType ctype, int ii){ return (Get(ctype,ii)+Get(ctype,ii+1))/2.;}

  double GetIsolationR()      {return (double) Get("IsolationR");}
  double GetCutOnBkgActivity(){return (double) Get("CutOnBkgActivity");}
  double GetIsolationMinPt()  {return (double) Get("IsolationMinPt");}

  //--- PYTHIA --
  bool   IsInEtaRange(float eta){return fabs(eta)<Get("EtaRange"); }
  int    SearchParentMethod(){ 
      int pyMethod = int(Get("SearchParentMethod")); 
      if(pyMethod ==0 || pyMethod == 1 || pyMethod==2 ) return pyMethod; 
      else {
          cout<< "SearchParentMethod can be just 0, 1 or 2"<< endl;
          exit(-1);
      }
  }


protected:
  void    ReadInputCard();
  unsigned int GetTVectorIndex(TString keyword);
  TString GetKeyWord(corrType ctype);


  //====   D a t a    M e m b e r s  ========

protected:
  ifstream incard;
  char cardname[255];
  int  nentry;                   //Number of lines in cfg file
  vector< TString >   KeyWordVector;      //array of key words
  vector< TVector >   fValuesVector;     //array of float number confg parameter vectors 
  map< TString, unsigned int > MapKeyWordToTVector;//mapping keywords to TVector

  //  ClassDef(JCard,1)
};

#endif

