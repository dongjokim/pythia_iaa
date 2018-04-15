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

#include  "JCard.h"
#include  "JConst.h"
using namespace std;

#define kMaxNoCentrBin 10   // Maximum no of centrality bins defined in JCard.h
#define kPtDim 20           // 
#define kMaxNoRuns 100      // Maximum no of runs in a nanoDST file

class JHistos {

    public:
        JHistos(JCard* cardP); //constructor
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
        void CreateAzimuthCorrHistos();
        void CreateToyHistos();

    public:
        JCard  *card;
        char  hname[40], htit[40];

        // == Inclusive in constructor ==========================================
        TH1D *hChargedPtpp, *hChargedPtAA;
        TH1D *hChargedPion[2], *hProton[2];
        TH1D *hGluonJet, *hQuarkJet, *hGG, *hQG, *hQQ;
        TH1D *hGluonJet23, *hQuarkJet23, *hGG23, *hQG23, *hQQ23;
        TH1D *h71from23[kPtDim], *hdPhi71from23[kPtDim];

        TH1D *hZppInc, *hZAAInc;
        TH1D *hZpp[kPtDim][kPtDim], *hZAA[kPtDim][kPtDim];
        TH2D *hDphiAssoc2D, *hDphiAssoc2DNoCorr;

        //==Pi0 Invariant Mass Spectra===========================================
        TH1D *hPi0PtSgn[kMaxNoCentrBin],      *hPi0PtBgn[kMaxNoCentrBin];
        TH1D *hKMass[2];   //K0 short
        TH2D *hPi0Mass[2]; //pi0 
        TH1D *hPi0TriggMass[2][kMaxNoCentrBin][kPtDim];

        //==Pt stat. correlations ===============================================
        TH1D *hPtNear[2][2][kMaxNoCentrBin];
        TH1D *hPtFar [2][2][kMaxNoCentrBin];

        //assorted
        TH1D *hPhi           [kMaxNoCentrBin];
        TH1D *hDphiAssoc  [2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim]; //FK//
        TH1D *hDphiAssocIsolTrigg [4][kMaxNoCentrBin][kPtDim][kPtDim]; //FK// trigger isolated hadron
        TProfile *hMeanPtAssoc [kMaxNoCentrBin][kPtDim][kPtDim];
        TProfile *hMeanZtAssoc [kMaxNoCentrBin][kPtDim][kPtDim];
        TH1D *hPtAssocUE[kPtDim], *hPtAssocN[kPtDim], *hPtAssocF[kPtDim];
        TH1D *hPtAssocUEIsolTrigg[kPtDim]; //FK// trigger isolated hadron

        //MOON
        TH1D *hDRNearPt[2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim];
        TH1D *hDRFarPt [2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim];
        TH1D *hDRNearPtMoon[2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim];
        TH1D *hDRFarPtMoon [2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim][kPtDim];

        TH2D *hDphiAssoc2DTest[kPtDim][kPtDim];
        TH2D *hDphiAssoc2DIAA[2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim];

        //cosThetaStar
        TH1D *hCosThetaStar[2][kMaxNoCentrBin][kPtDim];
        TH2D *hCMSrap[kMaxNoCentrBin][kPtDim];          
        TProfile *pCMSrap;
        TH1D *hInvMass[kPtDim], *hPairPtMass[kPtDim], *hPairDPhi[kPtDim], *hPairDpT[kPtDim];
        TH1D *hPairPtDphi[3][kPtDim][kPtDim], *hPairPt[3][kPtDim][kPtDim];

        TH1D *hDEtaNear    [2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim];
        TProfile *hDEtaNearPtAssoc[kMaxNoCentrBin][kPtDim];
        TH1D     *hDEtaNearPtAssocDist[kMaxNoCentrBin][kPtDim][kPtDim];
        TH1D     *hDEtaFarBckPtAssocDist[kMaxNoCentrBin][kPtDim][kPtDim];
        TH1D     *hDEtaFarPtAssocDist[kMaxNoCentrBin][kPtDim];

        TH1D *hIphiTrigg   [2][kMaxNoCentrBin][kPtDim];
        TH1D *hIphiAssoc   [2][kMaxNoCentrBin][kPtDim];
        TH1D *hFixPtBin       [kMaxNoCentrBin][kPtDim];
        TH1D *hTriggPtBin  [2][kMaxNoCentrBin][kPtDim];
        TH1D *hTriggMult      [kMaxNoCentrBin][kPtDim];
        TH1D *hAssocPtBin     [kMaxNoCentrBin][kPtDim][kPtDim];
        TH1D *hTriggPtBinIsolTrigg  [2][kMaxNoCentrBin][kPtDim]; //FK// trigger isolated hadron
        //TH1D *hInvMassLike[2][kMaxNoCentrBin][kPtDim], *hInvMassUnLike[2][kMaxNoCentrBin][kPtDim];

        //==================================================
        // xe slopes - done manually for the pp purpose only
        // you have to determine 7 trigger bins
        //==================================================
        TH1D     *hxEN[2][kPtDim], *hxEF[2][kPtDim], *hxEFIsolTrigg[2][kPtDim];
        TH1D     *hPerpN[2][kMaxNoCentrBin][kPtDim][kPtDim]; 
        TH1D     *hPerpNBg[2][kMaxNoCentrBin][kPtDim][kPtDim][kPtDim]; 


        //===================================================
        // PHENIX histograms
        //===================================================
        //==Run-by-Run calib ================================
        TH1D *TofPbSc[kMaxNoRuns]; TH1D *TofPbGl[kMaxNoRuns];
        //==Pt and FlipSlide Spectra=============================================
        TH1D *hCglPt3PC[kMaxNoCentrBin],    *hCglPtFlip3PC[kMaxNoCentrBin];
        TH1D *hCglPt32PC[kMaxNoCentrBin],   *hCglPtFlip32PC[kMaxNoCentrBin];


        //===================================================
        // PYTHIA 
        //===================================================
        TH1D *hNParton71, *hNLaParticle, *hNSelectedParton71;
        TH2D *hPartonDR;

        TH1D *hKT[kPtDim][kPtDim];
        TH1D *hTriggD[kPtDim], *hTriggParton71Pt[kPtDim], *hTriggParton71m23[kPtDim];
        TH1D *hAssocD[kPtDim][kPtDim], *hAssocParton71Pt[kPtDim][kPtDim], *hAssocParton71m23[kPtDim][kPtDim];

        void CreatePythia();

    protected:
        double maxEtaRange;

};

#endif

