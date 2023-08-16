#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TRandom.h>
#include <TClonesArray.h>
#include "src/JCORRAN/Base/AliJCard.h"
#include "src/JCORRAN/Base/AliJBaseTrack.h"
#include "src/JCORRAN/Base/iaaAnalysis/AliJIaaAna.h"
#include <Pythia8/Pythia.h>
#include <TStopwatch.h>
#include "src/psrc/JHistos.h"
#include "src/psrc/JParticleTools.h"
#include "src/psrc/JTreeDataManager.h"


// Configuration for Toy MC track generation //
using namespace std;

int main(int argc, char **argv) {

	//==== Read arguments =====
	if ( argc<2 ) {
		cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout<<"+  "<<argv[0]<<" [inputlist][outputFile][card]"<<endl;
		cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout << endl << endl;
		exit(1);
	}

	char *inputfile = argv[1];
	char *outputfile = argv[2];
	char* cardName   = argv[3];


	//PYTHIA 8-------------------------------------------

	TStopwatch timer;
	timer.Start();

	// Booking need histos & variables
	// same as UserCreateOutputObjects()
	TFile *fout = new TFile(outputfile ,"recreate" );
	fout->mkdir("JCIaa");
	fout->cd("JCIaa");

	// Tree generation
	TClonesArray *inputList = new TClonesArray("AliJBaseTrack",10000);
	JTreeDataManager* dmg = new JTreeDataManager();

	// === Set up JCard ====
	AliJCard *card = new AliJCard(cardName);
	card->PrintOut();
	//card->ReadLine( cardSetting.Data() );
	//card->ReCompile();
	card->PrintOut();

	// === Create analysis object ===
	// Gluon Filtering part
	JHistos *histos = new JHistos();
	histos->CreateToyHistos();

	//==== Read the Data files =====
	dmg->ChainInputStream(inputfile);
	//AliJBaseEventHeader *eventHeader;

	int Nevt = dmg->GetNEvents();

/*
	AliJIaaAna *fIaaAna;
	fIaaAna = new AliJIaaAna( kFALSE );

	fIaaAna->SetCard( card );
	fIaaAna->SetTrigger((char*)"hadron");
	fIaaAna->SetAssoc((char*)"hadron");
	fIaaAna->UserCreateOutputObjects();
	fIaaAna->GetCard()->WriteCard(fout->GetDirectory("JCIaa"));
*/
	// ========================================================

	int ieout = Nevt/20;
	if (ieout<1) ieout=1;
	int EventCounter = 0;
	int MultMid = -999;
	int MultV0M = -999; 
	for(int ievt=0; ievt<Nevt; ievt++){
		dmg->LoadEvent(ievt);
		//eventHeader  = dmg->GetEventHeader();
		//cout << eventHeader->GetEventID() << endl;
		//if(!dmg->IsGoodEvent()) continue;  // Vertex cut applied in IsGoodEvent and histo saved there too
		inputList->Clear();
		dmg->RegisterList(inputList, NULL);
		MultMid = 0; 
		MultV0M = 0;

		for(int t=0; t<inputList->GetEntriesFast(); t++) {
			AliJBaseTrack *trk = (AliJBaseTrack*)inputList->At(t);
			if(TMath::Abs(trk->Eta())<1.0 && trk->Pt()>0.2) {
				MultMid++; histos->hChargedMidEta->Fill(trk->Eta());
			}
			if((2.8<trk->Eta() && trk->Eta()<5.1) || (-3.7< trk->Eta() && trk->Eta()<-1.7)) {
				MultV0M++; histos->hChargedV0MEta->Fill(trk->Eta()); //  $2.8 < \eta < 5.1$ and $-3.7 < \eta < -1.7$, 	
			}
		}
		if(ievt % ieout == 0)  cout << ievt << "\t" << int(float(ievt)/Nevt*100) << "%\t Ntrk="<< inputList->GetEntriesFast() << endl ;
		histos->hChargedMidMult->Fill(MultMid);
		histos->hChargedV0MMult->Fill(MultV0M);
		//fIaaAna->Init();
		//fIaaAna->SetTrackList(inputList);
		//fIaaAna->SetRunNumber(0);
		//fIaaAna->SetCentrality(1.);
		//fIaaAna->SetZVertex(0.);
		//fIaaAna->UserExec();
		//fout->cd("JCIaa");


		//
		EventCounter++;

	} // event loop done.

	fout->Write();
	fout->Close();
	cout << EventCounter << " events are analyzed successfully."<< endl;
	timer.Print();
}
