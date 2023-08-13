#include  "JHistos.h"

//______________________________________________________________________________
JHistos::JHistos(){   // constructor
}

void JHistos::CreateToyHistos(){
    const int NBINS=150;
    double LogBinsX[201], LimL=0.1, LimH=100;
    double logBW = (log(LimH)-log(LimL))/NBINS;
    for(int ij=0;ij<=NBINS;ij++) LogBinsX[ij]=LimL*exp(ij*logBW);

    int  bins = 240; 
    double lr = -9./20., ur = lr+2;
 
    hChargedMidMult= new TH1D("hChargedMidMult"," ", 900, 0, 300); hChargedMidMult->Sumw2();
    hChargedV0MMult= new TH1D("hChargedV0MMult"," ", 900, 0, 300); hChargedV0MMult->Sumw2();
    hChargedMidEta= new TH1D("hChargedMidEta"," ", 100, -5., 5.); hChargedMidEta->Sumw2();
    hChargedV0MEta= new TH1D("hChargedV0MEta"," ", 100, -5., 5.); hChargedV0MEta->Sumw2();
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

    hGluonJet23 = new TH1D("hGluonJet23", "gluon jets", NXTBIN, LimL, LimH ); hGluonJet->Sumw2();
    hQuarkJet23 = new TH1D("hQuarkJet23", "gluon jets", NXTBIN, LimL, LimH ); hQuarkJet->Sumw2();
    hGG23       = new TH1D("hGG23", "from GG ", NXTBIN, LimL, LimH ); hGG->Sumw2();
    hQG23       = new TH1D("hQG23", "from QG ", NXTBIN, LimL, LimH ); hQG->Sumw2();
    hQQ23       = new TH1D("hQQ23", "from QQ ", NXTBIN, LimL, LimH ); hQQ->Sumw2();
    hPartonDR   = new TH2D("hPartonDR","distance",500,0,100,200,0,1.5);

    cout<<"Histos done"<<endl;
}
