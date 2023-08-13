//===========================================================
// JParticleTools.h
// DongJo Kim (dong.jo.kim@cern.ch)
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

#include "../JCORRAN/Base/AliJBaseTrack.h"
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
				fInputList = new TClonesArray("AliJBaseTrack", 1500 );
				fInputListWideEta = new TClonesArray("AliJBaseTrack", 1500 );
				TrackEtaRange = 1.0;
			}

		void InitializeEvent(){
			fInputList->Clear();
			fInputListWideEta->Clear();
			UniqueID=0;
		}

		void GetParticles();
		TClonesArray * GetInputList() const{return fInputList;}
		TClonesArray * GetInputListWideEta() const{return fInputListWideEta;}

		TRandom3 *unif;

		int UniqueID;
		TLorentzVector lvParticle;
		TClonesArray *fInputList;  // tracklist
		TClonesArray *fInputListWideEta;  // tracklist

		Event &event;
		JHistos *histos;

		double TrackEtaRange ;

};

#endif

