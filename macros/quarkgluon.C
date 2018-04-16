
#include "include/Filipad.h"

void LoadData();
void Compare();
void DrawQ(int ic);

double lowx=0.2;
double highx=20.;
double ly = 1e-4;
double hy = 5.e2;
double lowIAA = -0.2;
double highIAA = 10.2;

TLatex latexRun;
TString strRun = "#sqrt{#it{s}} = 2.76 TeV, Pythia";

const int Nsets = 1;
TString infiles[Nsets] = {
	"../test-1.root"
};
TFile *fin[Nsets];

TString sLeg[Nsets] = {
	"pythia"
};

int gMarkers[] = {20,24,21,25,23,27,29,30};
int gColors[]={kBlack, kRed, kBlue, kDeepSea, kPink, kGray, kRed, kBlack};

TH1D *hGluonJet[Nsets];
TH1D *hGluonJet23[Nsets];

TH1D *hQuarkJet[Nsets];
TH1D *hQuarkJet23[Nsets];

TH1D *hQGratio[Nsets];
TH1D *hQG23ratio[Nsets];

TString dirName = "JCIaa";


int iRef=0;

//------------------------------------------------------------------------------------------------
void LoadData() {

	for(int i=0;i<Nsets;i++){
		fin[i] = TFile::Open(infiles[i]);
	}
	int irefD = 0;
	int Nevts = 100;
	for(int i=0;i<Nsets;i++){
		hGluonJet[i] = (TH1D *)fin[i]->Get(Form("%s/hGluonJet",dirName.Data()));
		hGluonJet23[i] = (TH1D *)fin[i]->Get(Form("%s/hGluonJet23",dirName.Data()));
		hQuarkJet[i] = (TH1D *)fin[i]->Get(Form("%s/hQuarkJet",dirName.Data()));
		hQuarkJet23[i] = (TH1D *)fin[i]->Get(Form("%s/hQuarkJet23",dirName.Data()));
		hGluonJet[i]->Scale(1./Nevts,"width");
		hGluonJet23[i]->Scale(1./Nevts,"width");
		hQuarkJet[i]->Scale(1./Nevts,"width");
		hQuarkJet23[i]->Scale(1./Nevts,"width");
	} // iset
	// Calculation Various Ratios
	// 1. Out/In ratio for each data setfor(int i=0;i<Nsets;i++){
	for(int i=0;i<Nsets;i++){
		hQGratio[i] = (TH1D*)hGluonJet[i]->Clone();
		hQGratio[i]->Divide(hQuarkJet[i]);
		hQG23ratio[i] = (TH1D*)hGluonJet23[i]->Clone();
		hQG23ratio[i]->Divide(hQuarkJet23[i]);
	}
}

//------------------------------------------------------------------------------------------------
void Compare(){
	LoadData();
	DrawQ(1);
}

//------------------------------------------------------------------------------------------------
void DrawQ(int ic) {
	Filipad *fpad;

	fpad = new Filipad(ic+1, 1.1, 0.5, 100, 100, 0.7,5);
	fpad->Draw();
	//==== Upper pad
	TPad *p = fpad->GetPad(1); //upper pad
	p->SetTickx(); p->SetLogx(0); p->SetLogy(1); p->cd();
	//hy = hGluonJet[0]->GetMaximum()*1.2;
	TH2F *hfr = new TH2F("hfr"," ", 100,lowx, highx, 10, ly, hy); // numbers: tics x, low limit x, upper limit x, tics y, low limit y, upper limit y
	hset( *hfr, "p_{T}", "dN/dp_{T}",1.1,1.0, 0.09,0.09, 0.01,0.01, 0.04,0.05, 510,505);//settings of the upper pad: x-axis, y-axis
	hfr->Draw();
	//Legend definition
	TLegend *leg = new TLegend(0.45,0.4,0.85,0.78,"","brNDC");
	leg->SetTextSize(0.037);leg->SetBorderSize(0);leg->SetFillStyle(0);//legend settings;

	latexRun.DrawLatexNDC( 0.25, 0.85 ,strRun);

	//leg->AddEntry((TObject*)NULL,hDeltaEtaSig[0][AA][ic][iPTT][iPTA]->GetTitle(),"");

	for(int iS=0;iS<Nsets;iS++) {
		hGluonJet[iS]->SetMarkerStyle(20);
		hGluonJet[iS]->SetMarkerColor(1);
		hGluonJet[iS]->SetLineColor(1);
		hGluonJet[iS]->Draw("p,same");

		hGluonJet23[iS]->SetMarkerStyle(24);
		hGluonJet23[iS]->Draw("p,same");

		hQuarkJet[iS]->SetMarkerStyle(21);
		hQuarkJet[iS]->SetMarkerColor(2);
		hQuarkJet[iS]->SetLineColor(2);
		hQuarkJet[iS]->Draw("p,same");
		hQuarkJet23[iS]->SetMarkerStyle(25);
		hQuarkJet23[iS]->Draw("p,same");
		leg->AddEntry(hGluonJet[iS],"gluon","pl");
		leg->AddEntry(hQuarkJet[iS],"guark","pl");
		leg->AddEntry(hGluonJet23[iS],"gluon23","pl");
		leg->AddEntry(hQuarkJet23[iS],"guark23","pl");
	}


	leg->Draw();

	//==== Lower pad
	p = fpad->GetPad(2);
	p->SetTickx(); p->SetGridy(1); p->SetLogx(0), p->SetLogy(0); p->cd();
	TH2F *hfr1 = new TH2F("hfr1"," ", 100, lowx, highx, 10, lowIAA, highIAA);
	hset( *hfr1, "p_{T}", Form("Ratio to %s",sLeg[iRef].Data()),1.1,1.0, 0.09,0.09, 0.01,0.01, 0.08,0.08, 510,505);
	hfr1->Draw();
	for(int i=0;i<Nsets;i++) {
		hQGratio[i]->SetMarkerStyle(20);
		hQGratio[i]->SetMarkerColor(gColors[i]);
		hQGratio[i]->SetLineColor(gColors[i]);
		hQGratio[i]->Draw("p,same");
		hQG23ratio[i]->SetMarkerStyle(24);
		hQG23ratio[i]->Draw("p,same");
	}
	//gPad->GetCanvas()->SaveAs(Form("figs/DeltaEta_OUTOIN_C%02dT%02dA%02d.pdf",ic,iPTT,iPTA));
}
