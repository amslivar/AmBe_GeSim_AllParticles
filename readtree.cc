/****************************************************************** 
Code created by Ana Maria Slivar in June 2024 

read TrueEnergy branch from AmBe GeSim root files
create histogram of TrueEnergy
calculate efficiency of 4438 keV peak

usage root 'readtree.cc' -q
******************************************************************/


#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>

void readtree() {
    // Open the ROOT file
    TFile* file = TFile::Open("/home/amslivar/work/AmBe_v2/GeIII/AmBe_GeSim_722keV.root");

    // Get the TTree
    TTree* tree = (TTree*)file->Get("tT1");

    // Set up the variable to read the branch
    float TrueEnergy;
    tree->SetBranchAddress("TrueEnergy", &TrueEnergy);

    // Create a histogram for TrueEnergy
    TH1F* hTrueEnergy = new TH1F("hTrueEnergy", "True Energy Distribution;True Energy (MeV);Counts / 1.25 keV", 800, 0, 1); // Adjust binning and range as needed

    // Loop over the tree entries and fill the histogram
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t iEntry = 0; iEntry < nEntries; ++iEntry) {
        tree->GetEntry(iEntry);
        hTrueEnergy->Fill(TrueEnergy);
    }


    // Draw the histogram
    TCanvas* canvas = new TCanvas("canvas", "True Energy Histogram", 800, 600);
    canvas->SetLogy(); 
    hTrueEnergy->Draw();
    canvas->SaveAs("TrueEnergyHistogram_722keV.root");

    int binNumber = 578; // this should not have been hard coded, but it is easier like this for now
        // so i can actually see the root files and adjust the bin number accordingly


    // Get the content and error of the specified bin
    double binContent = hTrueEnergy->GetBinContent(binNumber);
    double binError = hTrueEnergy->GetBinError(binNumber);

    double binLowEdge = hTrueEnergy->GetBinLowEdge(binNumber);
    double binWidth = hTrueEnergy->GetBinWidth(binNumber);
    double binHighEdge = binLowEdge + binWidth;

    std::cout << "********************************************" << std::endl;
    std::cout << "Number of entries: " << nEntries << std::endl;
    std::cout << "********************************************" << std::endl;

    // Print the standard deviation of the bin and the bin limits
    std::cout << "Bin number: " << binNumber << std::endl;
    std::cout << "No of entries in bin: " << binContent << std::endl;
    std::cout << "Standard deviation (bin error): " << binError << std::endl;
    std::cout << "Bin limits: " << binLowEdge << " - " << binHighEdge << std::endl;

    std::cout << "********************************************" << std::endl;

    double efficiency = binContent / nEntries;
    double efficiencyError = binError / nEntries;
    std::cout << "Efficiency: " << efficiency << " +/- " << efficiencyError << std::endl;

    std::cout << "Upper limit efficiency: " << efficiency + efficiencyError << std::endl;
    std::cout << "Lower limit efficiency: " << efficiency - efficiencyError << std::endl;

    // Clean up
    delete file; 
}