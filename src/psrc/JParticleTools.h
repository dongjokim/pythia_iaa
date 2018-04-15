//===========================================================
// JParticleTools.h
//	 Created  Sun Jun 19 12:14:10 CEST 2011  by classmaker
//	 Jan Rak
//===========================================================

#ifndef JPARTICLETOOLS_H
#define JPARTICLETOOLS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <Pythia8/Pythia.h>

#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TRandom3.h"

#include "JHistos.h"
#include "set"
#include "map"

using namespace std;
using namespace Pythia8; 

class JParticleTools {

    public:

        JParticleTools (Event &inevent, JHistos *inhistos): 
            event(inevent),
            histos(inhistos){

                unif = new TRandom3();
                TrackEtaRange = 0.8;
            }

        void InitializeEvent(){
            UniqueID=0;
        }

        void GetParticles(int = 0);

        TRandom3 *unif;

        int UniqueID;
        TLorentzVector lvParticle;

        Event &event;
        JHistos *histos;

        double TrackEtaRange ;

};

#endif

