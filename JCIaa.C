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

// Configuration for Toy MC track generation //
using namespace std;
using namespace Pythia8;

int main(int argc, char **argv) {

	//==== Read arguments =====
	if ( argc<6 ) {
		cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout<<"+  "<<argv[0]<<" [outputFile] [Seed] [Nevt] [pythiaconfig] [card]"<<endl;
		cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout << endl << endl;
		exit(1);
	}

	char *outFile;
	outFile   = argv[1];
	int randomseed = atoi(argv[2]);
	int Nevt = atoi(argv[3]);;

	//PYTHIA 8-------------------------------------------
	char* pythiaconfig  = argv[4];
	char* cardName   = argv[5];

	cout <<"Setting ....." << endl;
	//---------------------
	//Pythia initialization
	//---------------------
	Pythia pythia;   // Generator.
	//Event& event      = pythia.event;
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

	TClonesArray *inputList;

	inputList = new TClonesArray("AliJBaseTrack", 1500 );

	// Booking need histos & variables
	// same as UserCreateOutputObjects()
	TFile *fout = new TFile(outFile ,"recreate" );
	fout->mkdir("JCIaa");
	fout->cd("JCIaa");

       // === Set up JCard ====
        AliJCard *card = new AliJCard(cardName);
        card->PrintOut();
        //card->ReadLine( cardSetting.Data() );
        //card->ReCompile();
        card->PrintOut();

  // === Create analysis object ===

        AliJIaaAna *fIaaAna;
        fIaaAna = new AliJIaaAna( kFALSE );

        fIaaAna->SetCard( card );
        fIaaAna->SetTrigger((char*)"hadron");
        fIaaAna->SetAssoc((char*)"hadron");
        fIaaAna->UserCreateOutputObjects();
        fIaaAna->GetCard()->WriteCard(fout->GetDirectory("JCIaa"));
	// ========================================================
	double etaMaxCutForPart = 0.8;

	int ieout = Nevt/20;
	if (ieout<1) ieout=1;
	int EventCounter = 0;

	for(int ievt=0; ievt<Nevt; ievt++){
		inputList->Clear();
		if (!pythia.next()) continue;
		if(ievt % ieout == 0) cout << ievt << "\t" << int(float(ievt)/Nevt*100) << "%" << endl ;
		//Start Analysis ==========================================
		// Add Particles from pythia
		int Nch_pthia = 0;
		for (int i = 0; i < pythia.event.size(); ++i) {//loop over all the particles in the event
			if( pythia.event[i].isFinal() && TMath::Abs(pythia.event[i].eta()) < etaMaxCutForPart && pythia.event[i].isCharged() && pythia.event[i].isHadron() )
			{
				TLorentzVector lvparticle(pythia.event[i].px(), pythia.event[i].py(), pythia.event[i].pz(), pythia.event[i].e());
				AliJBaseTrack track( lvparticle );
				track.SetID(pythia.event[i].id());
				track.SetParticleType(kJHadron);
				track.SetTrackEff(1.);
				new ( (*inputList)[inputList->GetEntriesFast()] ) AliJBaseTrack(track);
				Nch_pthia++;
			}
		}

		//cout << "Nch from pythia = "<< Nch_pthia << endl;
		//cout << "Total = "<< inputList->GetEntriesFast() << endl;

		//
		fIaaAna->Init();
		fIaaAna->SetTrackList(inputList);
		//cout <<"fIaaAna::"<< fIaaAna->GetInputList()->GetEntriesFast() << endl;
		fIaaAna->SetRunNumber(0);
		fIaaAna->SetCentrality(1);
		fIaaAna->SetZVertex(0.);
		fIaaAna->UserExec();

		//
		EventCounter++;

	} // event loop done.

	fout->Write();
	fout->Close();
	cout << EventCounter << " events are analyzed successfully."<< endl;
	timer.Print();
}
