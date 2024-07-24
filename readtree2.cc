/****************************************************************** 
Code created by Ana Maria Slivar in June 2024 

... I think I can delete this? no use so far, but I will keep it for now
same as the other one, but with a different root file, getting energy instead of TrueEnergy
basically I need to fit a peak here, so see energypeak.cc for that
... I am afraid of deleting it now and then needing it later
******************************************************************/


#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>

void readtree2() {
    // Open the ROOT file
   TFile* file = TFile::Open("AmBe_v8.root");

    // Get the TTree
   TTree* tree = (TTree*)file->Get("tT1");

    // Set up the variable to read the branch
   float Energy;
   tree->SetBranchAddress("Energy", &Energy);

    // Create a histogram for Energy
    TH1F* hEnergy = new TH1F("hEnergy", "Energy Distribution;Energy;Counts", 400, 0, 6); // Adjust binning and range as needed

    // Loop over the tree entries and fill the histogram
   Long64_t nEntries = tree->GetEntries();
   for (Long64_t iEntry = 0; iEntry < nEntries; ++iEntry) {
      tree->GetEntry(iEntry);
      hEnergy->Fill(Energy);
   }

    // Draw the histogram
   TCanvas* canvas = new TCanvas("canvas", "Energy Histogram", 800, 600);
   canvas->SetLogy(); 
   hEnergy->Draw();
   canvas->SaveAs("EnergyHistogram100k.root");

    // Clean up
    delete file; 
}