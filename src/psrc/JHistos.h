//===========================================================
// JHistos.h
//	 J
//===========================================================

#ifndef JHISTOS_H
#define JHISTOS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <TMath.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>

using namespace std;


class JHistos {

    public:
        JHistos(); //constructor
        virtual ~JHistos(){;}		//destructor

        void ScaleInvar(){
            scaleNotEquidistantHisto( hChargedPtpp);
            scaleNotEquidistantHisto( hChargedPtAA);
            for(int ii=0; ii<2; ii++){
                scaleNotEquidistantHisto( hChargedPion[ii]);
                scaleNotEquidistantHisto( hProton[ii]);
            }
        }

        void scaleNotEquidistantHisto(TH1D *hid, double sc=1){
            for(int i=1;i<= hid->GetNbinsX();i++){
                hid->SetBinContent(i,hid->GetBinContent(i)*sc/hid->GetBinWidth(i));
                hid->SetBinError(i,hid->GetBinError(i)*sc/hid->GetBinWidth(i));
            }   
        }


        // create histograms 
        void CreateToyHistos();

    public:
        char  hname[40], htit[40];

        // == Inclusive in constructor ==========================================
        TH1D *hChargedPtpp, *hChargedPtAA;
        TH1D *hChargedPion[2], *hProton[2];
        TH1D *hChargedMidMult, *hChargedV0MMult;
        TH1D *hChargedMidEta, *hChargedV0MEta;
        TH1D *hGluonJet, *hQuarkJet, *hGG, *hQG, *hQQ;
        TH1D *hGluonJet23, *hQuarkJet23, *hGG23, *hQG23, *hQQ23;
        TH1D *hZppInc, *hZAAInc;
        TH1D *hNParton71, *hNLaParticle, *hNSelectedParton71;
        TH2D *hPartonDR;

        void CreatePythia();

    protected:
        double maxEtaRange;

};

#endif

