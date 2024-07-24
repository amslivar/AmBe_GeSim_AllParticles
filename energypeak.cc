/****************************************************************** 
Code created by Ana Maria Slivar in June 2024 

code to read the AmBe root file from GeSim and fit a Gaussian to the 4438 keV peak

usage:        root 'energypeak.cc' -q 
******************************************************************/


#include <TMath.h>
#include <TF1.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <TString.h>
#include <TObjArray.h>
#include <TGraph.h>
#include <TVectorD.h>
#include <TCanvas.h>


Double_t gaus_linear(Double_t *x, Double_t *par){
    Double_t functionvalue;
    functionvalue = 0.399*(par[0]/par[2])*exp(-pow(x[0]-par[1],2.0)/(2.*pow(par[2],2.0)))+par[3]+par[4]*x[0];
    return functionvalue; 
}

Double_t gaus_offset(Double_t *x, Double_t *par){
    Double_t functionvalue;
    functionvalue = 0.399*(par[0]/par[2])*exp(-pow(x[0]-par[1],2.0)/(2.*pow(par[2],2.0)))+par[3];
    return functionvalue; 
}


void energypeak() {


    // Open the ROOT file
    TFile* file = TFile::Open("AmBe_5mi_corrected.root");

    // Get the TTree
    TTree* tree = (TTree*)file->Get("tT1");

    // Declare a variable to hold the data from the Energy branch
    float Energy;

    // Set the branch address
    tree->SetBranchAddress("Energy", &Energy);

    // Create a histogram for Energy
    TH1F* hEnergy = new TH1F("hEnergy", "Energy Distribution;Energy (MeV);Counts / 1 keV", 5000, 0, 5); // Adjust binning and range as needed

    // Debug: Print the tree structure to verify the branch
    //tree->Print();

    // Loop over the tree entries and fill the histogram
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t iEntry = 0; iEntry < nEntries; ++iEntry) {
        tree->GetEntry(iEntry);
        hEnergy->Fill(Energy); 
    }

    // Draw the histogram
    TCanvas* canvas = new TCanvas("canvas", "Energy Histogram", 800, 600);
    double peak_c, peak_s, xlow, xhigh;
    peak_c =  4.438;
    peak_s = .05;
    xlow = 4.425;
    xhigh = 4.46;
    hEnergy->Sumw2();
    TF1* fit4438 = new TF1("fit4438",gaus_offset,xlow,xhigh,4);
    fit4438->SetParNames("A","#mu","#sigma","offset","m");
    fit4438->SetParameters(480.,peak_c,.05,.5);
    //fit4438->FixParameter(1, peak_c);
    //fit4438->FixParameter(3, 0.);
    //hEnergy->GetXaxis()->SetRange(0,0);
    hEnergy->GetXaxis()->SetRangeUser(xlow,xhigh);
    hEnergy->Fit("fit4438","E","",xlow,xhigh);
    hEnergy->SetTitle("the 4438 peak");
    hEnergy->SetStats(kFALSE);

    canvas->SetLogy(); 
    hEnergy->Draw();    
    // Get chi-square and ndf from the fit result
    double chi2 = fit4438->GetChisquare();
    double ndf = fit4438->GetNDF();
    double chi2ndf = chi2 / ndf;

    // Adjusting the legend entries to show parameter names, values, their errors, the number of entries, and chi2/ndf
    TLegend* legend = new TLegend(0.6, 0.68, 0.9, 0.9);
    legend->AddEntry((TObject*)0, Form("Entries = %.0f", hEnergy->GetEntries()), "");
    legend->AddEntry((TObject*)0, Form("A = %.4f #pm %.4f", fit4438->GetParameter(0), fit4438->GetParError(0)), "");
    legend->AddEntry((TObject*)0, Form("#mu = %.4f #pm %.4f", fit4438->GetParameter(1), fit4438->GetParError(1)), "");
    legend->AddEntry((TObject*)0, Form("#sigma = %.4f #pm %.4f", fit4438->GetParameter(2), fit4438->GetParError(2)), "");
    legend->AddEntry((TObject*)0, Form("offset = %.4f #pm %.4f", fit4438->GetParameter(3), fit4438->GetParError(3)), "");
    legend->AddEntry((TObject*)0, Form("#chi^{2}/ndf = %.4f", chi2ndf), "");
    legend->Draw();
    canvas->Update();

    canvas->SaveAs("EnergyHistogram_5mi_fit.root");
    canvas->SaveAs("EnergyHistogram_5mi_fit.png");
    
    // Clean up
    delete file; 


}
