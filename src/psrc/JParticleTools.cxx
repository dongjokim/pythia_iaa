#include "JParticleTools.h"
#include <set>

using namespace std;

//extern double DeltaPhiX(double dphi);

//--------------------------------------------------------
void JParticleTools::GetParticles( int filtering ){ 

    //double random = unif->Rndm();

    for(int partIdx = 0; partIdx < event.size(); partIdx++){ 

        if( !event[partIdx].isFinal() || !event[partIdx].isCharged() || !event[partIdx].isHadron() ) continue;
        if(event[partIdx].vProd().pAbs()>1.0) continue; //production vertex >1.0mm to reject secondaries. 

        double trEta = event[partIdx].eta();
        //cout<< "pTt=" << pTtBin <<" pTa="<<pTaBin <<endl;  
        if( fabs(trEta)>TrackEtaRange ) continue;

        lvParticle.SetPxPyPzE(event[partIdx].px(), event[partIdx].py(), event[partIdx].pz(), event[partIdx].e() );
        lvParticle.SetUniqueID(UniqueID++);

        if (filtering>0) {

            int indexParton71 = -1;
            // find the mother string. It could be resonance (123442).
            int daughter = partIdx;
            while(event[daughter].motherList().size()<=1) daughter = event[daughter].motherList()[0];
            int mother1 = event[daughter].motherList()[0];
            int mother2 = event[daughter].motherList()[0] + event[daughter].motherList().size();
            //cout <<" m1="<<  mother1 <<" m2="<< mother2 <<endl; 

            //find a closest parton from the string.
            TLorentzVector parton71;
            double DeltaR = 0.3;
            for(int im=mother1; im<mother2; im++){
                //if( event[im].pT()<lvParticle.Pt() ) continue;
                parton71.SetPxPyPzE(event[im].px(), event[im].py(), event[im].pz(), event[im].e());
                double dr = parton71.DeltaR(lvParticle);
                //if(dr<DeltaR && parton71.Pt()>lvParticle.Pt() && parton71.E() > lvParticle.E()) 

                if(dr<DeltaR) {
                    DeltaR = dr;
                    indexParton71 = im;
                }
            } 
            if(indexParton71 < 0) continue;

            int indexParton23 = indexParton71;
            while(event[indexParton23].motherList()[0]>3) indexParton23 = event[indexParton23].motherList()[0];
            
            if( event[indexParton23].statusAbs() == 23 ){
                double pTPart = lvParticle.Pt();
                TLorentzVector parton23(event[indexParton23].px(), event[indexParton23].py(), event[indexParton23].pz(), event[indexParton23].e());
                double pT23   = parton23.Pt();
		double pT71   = parton71.Pt();
		double dr = parton71.DeltaR(parton23);

		if(event[indexParton23].id() == 21){
			histos->hGluonJet->Fill(pTPart); 
			histos->hGluonJet23->Fill(pT23); 
		} else {
			histos->hQuarkJet->Fill(pTPart); 
			histos->hQuarkJet23->Fill(pT23); 
		}
		bool bGG = (event[5].id() == 21 && event[6].id() == 21);
		bool bQQ = (event[5].id() != 21 && event[6].id() != 21);
		if(bGG){
			histos->hGG->Fill(pTPart);
			histos->hGG23->Fill(pT23);
		} else if(bQQ){
			histos->hQQ->Fill(pTPart);
			histos->hQQ23->Fill(pT23);
		} else if(!bGG && !bQQ){
			histos->hQG->Fill(pTPart);
			histos->hQG23->Fill(pT23);
		}
	    }

	    // kill all gluons (gluon=21)
	    if( event[indexParton23].statusAbs() != 23 || event[indexParton23].id() == 21) continue;

	    // kill all quarks
	    //if( event[indexParton23].statusAbs() != 23 || event[indexParton23].id() != 21) continue;
	    //cout << "AA indexParton23 = "<<indexParton23 << " id=" << event[indexParton23].id() <<endl;

	    //RAA - try to filter out the low energy partons to get RAA
	    /*
	       if( event[indexParton23].statusAbs() == 23){
	       double pT23   = event[indexParton23].pT();
	    //double RAA = 0.0123902 +  0.0143046*pT23 - 0.000109241*pT23*pT23;
	    double RAA = 0.02 +  0.005*pT23;  /// 0.15 0.005
	    //cout.precision(5); cout<<1.0001 <<" "<< unif->Rndm() <<endl; 
	    if(random > RAA) return;
	    }
	     */
	    AliJBaseTrack track( lvParticle );
	    track.SetID(event[partIdx].id());
	    track.SetParticleType(kJHadron);
	    track.SetTrackEff(1.);
	    new((*fInputList)[fInputList->GetEntriesFast()]) AliJBaseTrack(track);
	}

	// charge pion +-211
	if(fabs(event[partIdx].id())==211) histos->hChargedPion[filtering]->Fill(event[partIdx].pT());
	// proton = 2212 
	if(event[partIdx].id()==2212) {
		histos->hProton[filtering]->Fill(event[partIdx].pT());
		//cout << event[partIdx].pT() <<endl; 
	}
    }
}

