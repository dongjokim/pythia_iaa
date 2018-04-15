// $Id: JConst.h,v 1.5 2008/05/08 15:19:52 djkim Exp $

////////////////////////////////////////////////////
/*!
  \file JConst.h
  \brief
  \author J. Rak, D.J.Kim, R.Diaz (University of Jyvaskyla)
  \email: djkim@jyu.fi
  \version $Revision: 1.5 $
  \date $Date: 2008/05/08 15:19:52 $
*/
////////////////////////////////////////////////////

#ifndef JCONST_H
#define JCONST_H

//
//  Constants
//
// for JHisto and PhxJHisto
#define MAXNOCENTRBIN 10         // Maximum no of centrality bins defined in JCard.h
#define PtDim 20                 // 
#define MAXNORUNS 100            // Maximum no of runs in a nanoDST file

const double kJPi           = 3.14159265358979; //TMath::Pi();
const double kJTwoPi        = 2*kJPi;
const double JToRadian     = kJPi/180.;
const double JToDegree     = 180./kJPi;
const double ElectronMass = .51099906e-3;
const double PionMass     = .1395675;

enum expName {kPHENIX, kALICE};
const int numberOfExperiments = 2;

//====================== particle types ============================================
const int numberOfParticleTypes = 8;
enum particleType   {hadron,    pion,   kaon,   proton,      photon,   decayphoton,   pizero, eta};

char const * const particleTypeStrName[numberOfParticleTypes] =
                    {"hadron", "pion", "kaon", "proton",     "photon", "decayphoton",  "pizero", "eta"};
char const * const particleProtoType[numberOfExperiments][numberOfParticleTypes] =
                    {{"PhJCgl",       "PhJCgl",       "PhJCgl",       "PhJCgl",       "PhJPhoton",  "PhJPhoton",  "JPiZero","JPiZero"},
                     {"AliJTrack",    "AliJTrack",    "AliJTrack",    "AliJTrack",    "AliJPhoton", "AliJPhoton", "JPiZero","JPiZero"}};
//==================================================================================

enum fillType { real, mixed, rotated };
enum corrType { kTriggType, kAssocType, kLongType, kCentrType, kMassType, kEtaGapType, kZVertType, kNoType }; 

// PHENIX  constants
enum TEMC {PbSc, PbGl}; 

#endif
