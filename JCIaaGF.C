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


// Configuration for Toy MC track generation //
using namespace std;
using namespace Pythia8;

int main(int argc, char **argv) {

	//==== Read arguments =====
	if ( argc<5 ) {
		cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout<<"+  "<<argv[0]<<" [outputFile] [Seed] [Nevt] [pythiaconfig] [card]"<<endl;
		cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout << endl << endl;
		exit(1);
	}

	char *outFile;
	outFile   = argv[1];
	int randomseed = atoi(argv[2]);
	int Nevt = atoi(argv[3]);

	//PYTHIA 8-------------------------------------------
	char* pythiaconfig  = argv[4];
	char* cardName   = argv[5];

	cout <<"Setting ....." << endl;
	//---------------------
	//Pythia initialization
	//---------------------
	Pythia pythia;   // Generator.
	Event& event      = pythia.event;
	ParticleData& pdt = pythia.particleData;

	// Read in commands from external file.
	pythia.readFile(pythiaconfig);

	// Extract settings to be used in the main program.
	int    nEvent  = pythia.mode("Main:numberOfEvents");
	bool   showCS  = pythia.flag("Main:showChangedSettings");
	bool   showCPD = pythia.flag("Main:showChangedParticleData");
	//double energy  = pythia.mode("Beams:eCM");

	cout<<"Events="<<nEvent <<" RNDM seed "<< randomseed << endl;

	pythia.readString("Random:setSeed = on");
	pythia.readString(Form("Random:seed=%02d",randomseed));
	// Initialize. Beam parameters set in .cmnd file.
	pythia.init();

	// List changed data.
	if (showCS)  pythia.settings.listChanged();
	if (showCPD) pdt.listChanged();
	//PYTHIA 8-------------------------------------------

	TStopwatch timer;
	timer.Start();

	// Booking need histos & variables
	// same as UserCreateOutputObjects()
	TFile *fout = new TFile(outFile ,"recreate" );
	fout->mkdir("JCIaa");
	fout->cd("JCIaa");

	// Tree generation
	TClonesArray *tracks = new TClonesArray("AliJBaseTrack",10000);
    TTree *jTree = new TTree("jTree","Tree from ToyMC");
    jTree->Branch("JTrackList",&tracks);

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
	JParticleTools *ptool  = new JParticleTools(event, histos);
	
	AliJIaaAna *fIaaAna;
	fIaaAna = new AliJIaaAna( kFALSE );

	fIaaAna->SetCard( card );
	fIaaAna->SetTrigger((char*)"hadron");
	fIaaAna->SetAssoc((char*)"hadron");
	fIaaAna->UserCreateOutputObjects();
	fIaaAna->GetCard()->WriteCard(fout->GetDirectory("JCIaa"));
	// ========================================================

	int ieout = Nevt/20;
	if (ieout<1) ieout=1;
	int EventCounter = 0;
	int MultMid = -999;
	int MultV0M = -999; 
	for(int ievt=0; ievt<Nevt; ievt++){
		ptool->InitializeEvent();
		if (!pythia.next()) continue;
		//Start Analysis ==========================================
		// Add Particles from pythia
		ptool->GetParticles();
		tracks->Clear();
		MultMid = 0; 
		MultV0M = 0;
		TClonesArray *inputListWideEta = ptool->GetInputListWideEta();
		TClonesArray *inputList = ptool->GetInputList();
		for(int t=0; t<inputListWideEta->GetEntriesFast(); t++) {
			AliJBaseTrack *trk = (AliJBaseTrack*)inputListWideEta->At(t);
			new((*tracks)[tracks->GetEntriesFast()]) AliJBaseTrack(*trk);
			if(TMath::Abs(trk->Eta())<1.0 && trk->Pt()>0.2) {
				MultMid++; histos->hChargedMidEta->Fill(trk->Eta());
			}
			if((2.8<trk->Eta() && trk->Eta()<5.1) || (-3.7< trk->Eta() && trk->Eta()<-1.7)) {
				MultV0M++; histos->hChargedV0MEta->Fill(trk->Eta()); //  $2.8 < \eta < 5.1$ and $-3.7 < \eta < -1.7$, 	
			}
		}
		if(ievt % ieout == 0)  cout << ievt << "\t" << int(float(ievt)/Nevt*100) << "%\t Ntrk="<< inputList->GetEntriesFast() <<":"<< tracks->GetEntriesFast() << endl ;
		histos->hChargedMidMult->Fill(MultMid);
		histos->hChargedV0MMult->Fill(MultV0M);
		fIaaAna->Init();
		//fIaaAna->SetTrackList(inputList);
		//fIaaAna->SetRunNumber(0);
		//fIaaAna->SetCentrality(1.);
		//fIaaAna->SetZVertex(0.);
		//fIaaAna->UserExec();
		//fout->cd("JCIaa");
		jTree->Fill();	
	
		//
		EventCounter++;

	} // event loop done.
	
	fout->Write();
	fout->Close();
	cout << EventCounter << " events are analyzed successfully."<< endl;
	timer.Print();
}
