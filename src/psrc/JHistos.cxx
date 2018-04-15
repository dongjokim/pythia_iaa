#include  "JHistos.h"

//______________________________________________________________________________
JHistos::JHistos(JCard* cardP){   // constructor
    card=cardP;
    maxEtaRange = card->Get("EtaRange");
}

void JHistos::CreateToyHistos(){
    const int NBINS=150;
    double LogBinsX[201], LimL=0.1, LimH=100;
    double logBW = (log(LimH)-log(LimL))/NBINS;
    for(int ij=0;ij<=NBINS;ij++) LogBinsX[ij]=LimL*exp(ij*logBW);

    int  bins = 240; 
    double lr = -9./20., ur = lr+2;
 
    hChargedPtpp= new TH1D("hChargedPtpp"," ", NBINS, LogBinsX ); hChargedPtpp->Sumw2();
    hChargedPtAA= new TH1D("hChargedPtAA"," ", NBINS, LogBinsX ); hChargedPtAA->Sumw2();

    hChargedPion[0]= new TH1D("hChargedPion00"," ", NBINS, LogBinsX ); hChargedPion[0]->Sumw2();
    hChargedPion[1]= new TH1D("hChargedPion01"," ", NBINS, LogBinsX ); hChargedPion[1]->Sumw2();
    hProton[0]= new TH1D("hProton00"," ", NBINS, LogBinsX ); hProton[0]->Sumw2();
    hProton[1]= new TH1D("hProton01"," ", NBINS, LogBinsX ); hProton[1]->Sumw2();

    hZppInc= new TH1D("hZppInc", "proton FF", 100, 0.0, 1.1);
    hZAAInc= new TH1D("hZAAInc", "AA FF", 100, 0.0, 1.1);

    LimL=1.0; LimH=3000;
    const int NXTBIN=3000;
    hGluonJet = new TH1D("hGluonJet", "gluon jets", NXTBIN, LimL, LimH ); hGluonJet->Sumw2();
    hQuarkJet = new TH1D("hQuarkJet", "gluon jets", NXTBIN, LimL, LimH ); hQuarkJet->Sumw2();
    hGG       = new TH1D("hGG", "from GG ", NXTBIN, LimL, LimH ); hGG->Sumw2();
    hQG       = new TH1D("hQG", "from QG ", NXTBIN, LimL, LimH ); hQG->Sumw2();
    hQQ       = new TH1D("hQQ", "from QQ ", NXTBIN, LimL, LimH ); hQQ->Sumw2();

    for (int hit = 0; hit < card->GetNoOfBins(kTriggType);hit++)
    {
        float pTt1 = card->GetBinBorder(kTriggType, hit);
        float pTt2 = card->GetBinBorder(kTriggType, hit + 1);
        sprintf(htit, "parton pT: %3.1f-%3.1f", pTt1, pTt2);
        sprintf(hname, "h71from23%02d", hit);
        h71from23[hit] = new TH1D(hname, htit, 100, 0, 2);
        h71from23[hit]->Sumw2();
        sprintf(hname, "hdPhi71from23%02d", hit);
        hdPhi71from23[hit] = new TH1D(hname, htit, bins, lr, ur );
        hdPhi71from23[hit]->Sumw2();
    }

    hGluonJet23 = new TH1D("hGluonJet23", "gluon jets", NXTBIN, LimL, LimH ); hGluonJet->Sumw2();
    hQuarkJet23 = new TH1D("hQuarkJet23", "gluon jets", NXTBIN, LimL, LimH ); hQuarkJet->Sumw2();
    hGG23       = new TH1D("hGG23", "from GG ", NXTBIN, LimL, LimH ); hGG->Sumw2();
    hQG23       = new TH1D("hQG23", "from QG ", NXTBIN, LimL, LimH ); hQG->Sumw2();
    hQQ23       = new TH1D("hQQ23", "from QQ ", NXTBIN, LimL, LimH ); hQQ->Sumw2();

    hDphiAssoc2D = new TH2D("hDphiAssoc2D", "#Delta#eta x #Delta#phi", 100, -2, 2, 100, -9./20., 9./20.+2);


    cout<<"Histos done"<<endl;
}

//______________________________________________________________________________
void JHistos::CreateAzimuthCorrHistos()
{
    int  bins = 240; 
    //double lr = -1./3, ur= 5./3;
    double lr = -9./20., ur = lr+2;
    double pt_bw=10/100.0;  //see hPt histo below, let's make 10 bins per 1GeV/c

    const int NUE=20;
    double UEBinsx[101], UEa = card->GetBinBorder(kAssocType, 0), UEb = card->GetBinBorder(kAssocType, card->GetNoOfBins(kAssocType));
    double logUEbw = (log(UEb)-log(UEa))/NUE;
    for(int ij=0;ij<=NUE;ij++) UEBinsx[ij]=UEa*exp(ij*logUEbw);

    const int NUEfar=10;
    double UEBinsxFar[101];
    logUEbw = (log(UEb)-log(UEa))/NUEfar;
    for(int ij=0;ij<=NUE;ij++) UEBinsxFar[ij]=UEa*exp(ij*logUEbw);

    if(card->GetNoOfBins(kCentrType) > kMaxNoCentrBin ){
        cout<<"ERROR: No of Centrality bins exceed max dim in JHistos.cxx "<<endl;
        exit(0);
    }

    for (int hic = 0;hic < card->GetNoOfBins(kCentrType);hic++) {
        float b1 = card->GetBinBorder(kCentrType, hic);
        float b2 = card->GetBinBorder(kCentrType, hic + 1);

        //==================================
        //  trigger pt histos 
        //==================================
        for (int hit = 0; hit < card->GetNoOfBins(kTriggType);hit++)
        {
            float pTt1 = card->GetBinBorder(kTriggType, hit);
            float pTt2 = card->GetBinBorder(kTriggType, hit + 1);
            sprintf(htit, "cent: %3.0f -%3.0f pTt: %3.1f-%3.1f", b1, b2, pTt1, pTt2);

            for(int ityp=0; ityp<2; ityp++){
                sprintf(hname, "hTriggPtBin%02d%02d%02d", ityp, hic, hit);
                hTriggPtBin[ityp][hic][hit] = new TH1D(hname, htit,(int)TMath::Ceil((pTt2-pTt1)/pt_bw),pTt1, pTt2);
                hTriggPtBin[ityp][hic][hit]->Sumw2();

            }
        }

        //==================================
        //  associated dphi histos 
        //==================================
        for(int hit=0; hit < card->GetNoOfBins(kTriggType); hit++){
            float pTt1 = card->GetBinBorder(kTriggType, hit);
            float pTt2 = card->GetBinBorder(kTriggType, hit + 1);
            //let's not fill mixed at all
            for(int iEtaGap=0; iEtaGap < card->GetNoOfBins(kEtaGapType); iEtaGap++){ 
                float etaGapThr = card->GetBinBorder(kEtaGapType, iEtaGap );
                for (int hia = 0; hia < card->GetNoOfBins(kAssocType); hia++){
                    float pTa1 = card->GetBinBorder(kAssocType, hia);
                    float pTa2 = card->GetBinBorder(kAssocType, hia + 1);

                    sprintf(htit, "cent: %2.1f-%2.1f #Delta#eta>%1.1f  pTt: %2.1f-%2.1f pta: %2.1f-%2.1f",
                            b1, b2, etaGapThr, pTt1, pTt2, pTa1,pTa2);
                    for(int ityp=0; ityp<2; ityp++){
                        sprintf(hname, "hDphiAssoc%02dC%02dE%02dT%02dA%02d", ityp, hic, iEtaGap, hit, hia);
                        hDphiAssoc[ityp][hic][iEtaGap][hit][hia] = new TH1D(hname, htit, bins, lr, ur);
                        hDphiAssoc[ityp][hic][iEtaGap][hit][hia]->Sumw2();

                        sprintf(hname, "hDEtaNear%02d%02d%02d%02d%02d", ityp, hic, iEtaGap, hit, hia);
                        hDEtaNear[ityp][hic][iEtaGap][hit][hia] = new TH1D(hname, htit, 320, -2*maxEtaRange, 2*maxEtaRange);
                        hDEtaNear[ityp][hic][iEtaGap][hit][hia]->Sumw2();
                    }//ityp

                }//assoc
                sprintf(hname, "hDEtaNearPtAssocDist%02d%02d%02d", hic, iEtaGap, hit);
                //hDEtaNearPtAssocDist[hic][iEtaGap][hit] = new TH1D(hname, htit, 100, 0, 30);
                hDEtaNearPtAssocDist[hic][iEtaGap][hit] = new TH1D(hname, htit, NUE, UEBinsx);
                hDEtaNearPtAssocDist[hic][iEtaGap][hit]->Sumw2();

                sprintf(hname, "hDEtaFarBckPtAssocDist%02d%02d%02d", hic, iEtaGap, hit);
                hDEtaFarBckPtAssocDist[hic][iEtaGap][hit] = new TH1D(hname, htit, NUEfar, UEBinsxFar);
                hDEtaFarBckPtAssocDist[hic][iEtaGap][hit]->Sumw2();
            }//eta gap

            sprintf(hname, "hDEtaNearPtAssoc%02d%02d", hic, hit);
            hDEtaNearPtAssoc[hic][hit] = new TProfile(hname, htit, 320, -2*maxEtaRange, 2*maxEtaRange);
            hDEtaNearPtAssoc[hic][hit]->Sumw2();

            sprintf(hname, "hDEtaFarPtAssocDist%02d%02d", hic, hit);
            hDEtaFarPtAssocDist[hic][hit] = new TH1D(hname, htit, NUEfar, UEBinsxFar);
            hDEtaFarPtAssocDist[hic][hit]->Sumw2();

        }//trigger


        //==================================
        //  associated kperp histos 
        //==================================
        for (int hit = 0; hit < card->GetNoOfBins(kTriggType);hit++){
            float pTt1 = card->GetBinBorder(kTriggType, hit);
            float pTt2 = card->GetBinBorder(kTriggType, hit + 1);
            for(int ityp=0; ityp<2; ityp++){
                for (int hiklong = 0; hiklong < card->GetNoOfBins(kLongType); hiklong++){
                    float klong1 = card->GetBinBorder(kLongType, hiklong);
                    float klong2 = card->GetBinBorder(kLongType, hiklong + 1);
                    for(int iEtaGap=0; iEtaGap < card->GetNoOfBins(kEtaGapType); iEtaGap++){ 
                        //let's not compute mixed random background - takes a lot of time.
                        float etaGapThr = card->GetBinBorder(kEtaGapType, iEtaGap );
                        sprintf(htit, "cent: %2.1f-%2.1f #Delta#eta>%1.1f  pTt: %2.1f-%2.1f k_{#parallel}: %2.1f-%2.1f",
                                b1, b2, etaGapThr, pTt1, pTt2, klong1, klong2);

                        sprintf(hname, "hPerpNBg%02dC%02dE%02dT%02dA%02d", ityp, hic, iEtaGap, hit, hiklong);
                        hPerpNBg[ityp][hic][iEtaGap][hit][hiklong] = new TH1D(hname, htit, 600, 0, 10); //FK// Kperp background
                        hPerpNBg[ityp][hic][iEtaGap][hit][hiklong]->Sumw2();
                    } //eta

                    sprintf(htit, "cent: %2.1f-%2.1f  pTt: %2.1f-%2.1f k_{#parallel}: %2.1f-%2.1f",
                            b1, b2, pTt1, pTt2, klong1, klong2);

                    sprintf(hname, "hPerpN%02dC%02dT%02dA%02d", ityp, hic, hit, hiklong);
                    hPerpN[ityp][hic][hit][hiklong] = new TH1D(hname, htit, 600, 0, 10); //FK// Measured kperp
                    hPerpN[ityp][hic][hit][hiklong]->Sumw2();
                }//klong 
            }//ityp
        }

    }//centrality


    //==================================
    // others 
    //==================================
    for(int hit=0; hit < card->GetNoOfBins(kTriggType); hit++){
        float pTt1 = card->GetBinBorder(kTriggType, hit);
        float pTt2 = card->GetBinBorder(kTriggType, hit + 1);
        for (int hia = 0; hia < card->GetNoOfBins(kAssocType); hia++){
            float pTa1 = card->GetBinBorder(kAssocType, hia);
            float pTa2 = card->GetBinBorder(kAssocType, hia + 1);
            sprintf(htit, "pTt: %3.1f-%3.1f pTta: %3.1f-%3.1f", pTt1, pTt2, pTa1,pTa2);

            sprintf(hname, "hZpp%02d%02d", hit, hia);
            hZpp[hit][hia] = new TH1D(hname, htit, bins, lr, ur);
            hZpp[hit][hia]->Sumw2();

            sprintf(hname, "hZAA%02d%02d", hit, hia);
            hZAA[hit][hia] = new TH1D(hname, htit, bins, lr, ur);
            hZAA[hit][hia]->Sumw2();
        }
    }
    //--- IAA signal ---
    for(int ityp=0; ityp<2; ityp++){
        for (int hic = 0;hic < card->GetNoOfBins(kCentrType);hic++) {
            float b1 = card->GetBinBorder(kCentrType, hic);
            float b2 = card->GetBinBorder(kCentrType, hic + 1);
            for (int hiz = 0; hiz < card->GetNoOfBins(kZVertType); hiz++) {
                float z1 = card->GetBinBorder(kZVertType, hiz);
                float z2 = card->GetBinBorder(kZVertType, hiz + 1);
                for(int hit=0; hit < card->GetNoOfBins(kTriggType); hit++){
                    float pTt1 = card->GetBinBorder(kTriggType, hit);
                    float pTt2 = card->GetBinBorder(kTriggType, hit + 1);
                    for(int iRGap=0; iRGap <= card->Get("EtaGapSignalBin"); iRGap++){ 
                        float RGap1 = card->GetBinBorder(kEtaGapType, iRGap );
                        float RGap2 = card->GetBinBorder(kEtaGapType, iRGap + 1 );
                        sprintf(htit, "C: %2.0f-%2.0f%% Z: %1.1f-%1.1f pTt: %2.1f-%2.1f pta: %2.1f-%2.1f %2.1f<R<%2.1f",
                                b1, b2, z1, z2, pTt1, pTt2, card->GetBinBorder(kAssocType,0), card->GetBinBorder(kAssocType, card->GetNoOfBins(kAssocType)),
                                RGap1, RGap2);

                        sprintf(hname, "hDRNearPt%02d%02d%02d%02d%02d", ityp, hic, hiz, iRGap, hit);
                        hDRNearPt[ityp][hic][hiz][iRGap][hit] = new TH1D(hname, htit, NUE, UEBinsx);
                        hDRNearPt[ityp][hic][hiz][iRGap][hit]->Sumw2();

                        sprintf(hname, "hDRFarPt%02d%02d%02d%02d%02d", ityp, hic, hiz, iRGap, hit);
                        hDRFarPt[ityp][hic][hiz][iRGap][hit] = new TH1D(hname, htit, NUEfar, UEBinsxFar);
                        hDRFarPt[ityp][hic][hiz][iRGap][hit]->Sumw2();
                    }
                }
            }
        }
    }

    // --- Moons ---
    for(int ityp=0; ityp<2; ityp++){
        for (int hic = 0;hic < card->GetNoOfBins(kCentrType);hic++) {
            float b1 = card->GetBinBorder(kCentrType, hic);
            float b2 = card->GetBinBorder(kCentrType, hic + 1);
            for (int hiz = 0; hiz < card->GetNoOfBins(kZVertType); hiz++) {
                float z1 = card->GetBinBorder(kZVertType, hiz);
                float z2 = card->GetBinBorder(kZVertType, hiz + 1);
                for(int iPhiGap=0; iPhiGap < card->GetNoOfBins(kEtaGapType); iPhiGap++){ 
                    float phiGap1 = card->GetBinBorder(kEtaGapType, iPhiGap );
                    float phiGap2 = card->GetBinBorder(kEtaGapType, iPhiGap + 1 );
                    for(int iRGap=iPhiGap; iRGap < card->GetNoOfBins(kEtaGapType); iRGap++){ //not from 0
                        float RGap1 = card->GetBinBorder(kEtaGapType, iRGap );
                        float RGap2 = card->GetBinBorder(kEtaGapType, iRGap + 1 );
                        for(int hit=0; hit < card->GetNoOfBins(kTriggType); hit++){
                            float pTt1 = card->GetBinBorder(kTriggType, hit);
                            float pTt2 = card->GetBinBorder(kTriggType, hit + 1);
                            sprintf(htit, "C: %2.0f-%2.0f%% Z: %1.1f-%1.1f pTt: %2.1f-%2.1f pta: %2.1f-%2.1f %2.1f<R<%2.1f %2.1f<#Delta#phi<%2.1f",
                                    b1, b2, z1, z2, pTt1, pTt2, card->GetBinBorder(kAssocType,0), card->GetBinBorder(kAssocType, card->GetNoOfBins(kAssocType)),
                                    RGap1, RGap2, phiGap1, phiGap2);

                            sprintf(hname, "hDRNearPtMoon%02d%02d%02d%02d%02d%02d", ityp, hic, hiz, iRGap, iPhiGap, hit);
                            hDRNearPtMoon[ityp][hic][hiz][iRGap][iPhiGap][hit] = new TH1D(hname, htit, NUE, UEBinsx);
                            hDRNearPtMoon[ityp][hic][hiz][iRGap][iPhiGap][hit]->Sumw2();

                            sprintf(hname, "hDRFarPtMoon%02d%02d%02d%02d%02d%02d", ityp, hic, hiz, iRGap, iPhiGap, hit);
                            hDRFarPtMoon[ityp][hic][hiz][iRGap][iPhiGap][hit] = new TH1D(hname, htit, NUE, UEBinsx);
                            hDRFarPtMoon[ityp][hic][hiz][iRGap][iPhiGap][hit]->Sumw2();
                        }
                    }
                }
            }
        }
    }
    for(int iPhiGap=0; iPhiGap < card->GetNoOfBins(kEtaGapType); iPhiGap++){
        float phiGap1 = card->GetBinBorder(kEtaGapType, iPhiGap );
        float phiGap2 = card->GetBinBorder(kEtaGapType, iPhiGap + 1 );
        for(int iRGap=0; iRGap < card->GetNoOfBins(kEtaGapType); iRGap++){
            float RGap1 = card->GetBinBorder(kEtaGapType, iRGap );            
            float RGap2 = card->GetBinBorder(kEtaGapType, iRGap + 1 );

            sprintf(htit, "%2.1f<R<%2.1f %2.1f<#phi<%2.1f", RGap1, RGap2, phiGap1, phiGap2);
            sprintf(hname, "hDphiAssoc2DTest%02d%02d", iRGap, iPhiGap);
            hDphiAssoc2DTest[iRGap][iPhiGap] = new TH2D(hname, htit, 100, -2*maxEtaRange, 2*maxEtaRange, 100, lr, ur);
        }
    }

    //==========================
    // 2D histos 
    //==========================
    for (int hic = 0;hic < card->GetNoOfBins(kCentrType);hic++) {
        float b1 = card->GetBinBorder(kCentrType, hic);
        float b2 = card->GetBinBorder(kCentrType, hic + 1);
        for (int hiz = 0; hiz < card->GetNoOfBins(kZVertType); hiz++) {
            float z1 = card->GetBinBorder(kZVertType, hiz);
            float z2 = card->GetBinBorder(kZVertType, hiz + 1);
            for (int hia = 0; hia < card->GetNoOfBins(kAssocType); hia++){
                float pTa1 = card->GetBinBorder(kAssocType, hia);
                float pTa2 = card->GetBinBorder(kAssocType, hia + 1);
                for(int hit=0; hit < card->GetNoOfBins(kTriggType); hit++){
                    float pTt1 = card->GetBinBorder(kTriggType, hit);
                    float pTt2 = card->GetBinBorder(kTriggType, hit + 1);
                    for(int ityp=0; ityp<2; ityp++){
                        sprintf(htit, "C: %2.0f-%2.0f%% Z: %1.1f-%1.1f pTt: %3.1f-%3.1f pTta: %3.1f-%3.1f", b1, b2, z1, z2, pTt1, pTt2, pTa1,pTa2);
                        sprintf(hname, "hDphiAssoc2DIAA%02d%02d%02d%02d%02d", ityp, hic, hiz, hit, hia);
                        hDphiAssoc2DIAA[ityp][hic][hiz][hit][hia] = new TH2D(hname, htit, 100, -2*maxEtaRange, 2*maxEtaRange, 100, lr, ur);
                        hDphiAssoc2DIAA[ityp][hic][hiz][hit][hia]->Sumw2();
                    }
                }
            }
        }
    }


}

