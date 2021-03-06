#include <iostream>

// Include the required root-headers:
#include "TMath.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TString.h"
#include "TSystem.h"
#include "TFile.h"

using namespace std;

TCanvas* DrawHistDouma_2D_flatArray(Int_t const Nhist, Int_t const Nhor, Int_t const Nvert, TH2F* const Raw[], TString const Title[], TString const xlabel, TString const ylabel, TString const zlabel)
{
  // This program paints the histogram TH2D* Raw. 
  // This program controls every aspect of the plot
  // except the histogram data, which is taken from the input.
  // Hence all style aspects are defined here.
  // Title is the histogram and Canvas title you want pianted and
  
  // Output is max, the highest point in the histogram.
  
  // Written by C. A. Douma.
  
  // =====================================================================================
  // Before we actually create the picture, we first need to obtain our scale factor:
  Double_t ScaleFactor = 0.1;
  FileStat_t stom;
  Int_t inputs_test = gSystem->GetPathInfo("../../Data/Inputs/Inputs.root",stom);
   
  if (inputs_test==0)
  {
    // Connect to the Inputs.root file:
    TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  
    // And then Get the histograms from this file:
    TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
    TH1I* Integers = (TH1I*) inputs->Get("Integers");
    
    // obtain the scale factor:
    ScaleFactor = Doubles->GetBinContent(401);
  }
    
  // ======================================================================================
  // We begin with declaring some variables:
  TString st = "";
  TString kstr = "";
  Double_t Xmin = Raw[0]->GetXaxis()->GetXmin();
  Double_t Xmax = Raw[0]->GetXaxis()->GetXmax();
  Double_t XcMax= 0.0;
  Double_t XcMin = 0.0;
  Double_t Ymin = Raw[0]->GetYaxis()->GetXmin();
  Double_t Ymax = Raw[0]->GetYaxis()->GetXmax();
  Double_t YcMax= 0.0;
  Double_t YcMin = 0.0;
  Double_t max = Raw[0]->GetMaximum();
  Double_t min = Raw[0]->GetMinimum();
  Double_t cmax = 0.0;
  Double_t cmin = 0.0;
  
  // Then perform searches for the global min and max:
  for (Int_t k=0; k<Nhist; ++k)
  {
    XcMin = Raw[k]->GetXaxis()->GetXmin();
    XcMax = Raw[k]->GetXaxis()->GetXmax();
    YcMin = Raw[k]->GetYaxis()->GetXmin();
    YcMax = Raw[k]->GetYaxis()->GetXmax();
    cmax = Raw[k]->GetMaximum();
    cmin = Raw[k]->GetMinimum();
    
    if (XcMax>Xmax) {Xmax = XcMax;}
    if (XcMin<Xmin) {Xmin = XcMin;}
    if (YcMax>Ymax) {Ymax = YcMax;}
    if (YcMin<Ymin) {Ymin = YcMin;}
    if (cmax>max) {max = cmax;}
    if (cmin<min) {min = cmin;}
  }
  
  // Then perform histogram settings:
  for (Int_t k=0; k<Nhist; ++k)
  {
    // Define kstring:
    kstr = st.Itoa(k+1,10);
    
    cout << Title[k] << "\n";
    
    Raw[k]->SetTitle(Title[k]);
    Raw[k]->SetXTitle(xlabel);
    Raw[k]->SetYTitle(ylabel);
    Raw[k]->SetZTitle(zlabel);
  
    // Control position of axis labels along the axis:
    Raw[k]->GetXaxis()->CenterTitle(kTRUE);
    Raw[k]->GetYaxis()->CenterTitle(kTRUE);
    Raw[k]->GetZaxis()->CenterTitle(kTRUE);
    // kFALSE will put the labels at the end of the axis.
  
    // Set ranges of the histogram:
    Raw[k]->SetAxisRange(Xmin,Xmax,"x");
    Raw[k]->SetAxisRange(Ymin,Ymax,"y");
  
    Raw[k]->SetMaximum(1.0*max);
    Raw[k]->SetMinimum(1.0*min);
    Raw[k]->SetMinimum(0.3); // Log-scale option.
  
    // Add the histogram painting options:
    Raw[k]->SetOption("COLZfbbb"); // fb = front box & bb = back box. Shuts these down.
    // We have "lego", etc. see THistPainter ROOT.
  }
  
  // =======================================================================================
  // Then we define the current style for histogram plotting:
  
  // --------------------------------------------------------------------
  // General aspects:
  gStyle->SetFrameLineWidth(1.0); // Thickness of frame 
  gStyle->SetFrameLineColor(0); // Color of frame
  gStyle->SetHistLineColor(1); // line color of histogram
  gStyle->SetHistLineWidth(1.0); // line thickness of histogram
  gStyle->SetLineWidth(2.0); // Line thickness of axis!
  gStyle->SetOptStat(0); // No stat box
  gStyle->SetOptLogx(0); // log-scale on x-axis, either 0 or 1.
  gStyle->SetOptLogy(0); // log-scale on y-axis, either 0 or 1.
  gStyle->SetOptLogz(1); // log-scale on z-axis, either 0 or 1.
  gStyle->SetScreenFactor(ScaleFactor); // size of the canvas on the screen.
  gStyle->SetPadBottomMargin(0.14); // margin of the histogram to the boundary of the pad on the bottom.
  gStyle->SetPadTopMargin(0.14); // margin of the histogram to the boundary of the pad on the Top.
  gStyle->SetPadLeftMargin(0.155); // margin of the histogram to the boundary of the pad on the Left.
  gStyle->SetPadRightMargin(0.15); // margin of the histogram to the boundary of the pad on the Right.
  // 0=white 1=black 2=red 3=green 4=blue 5=yellow 6=magenta 7=cyaan 8=darkgreen 9=purple, there are lots of other options...
  
  // --------------------------------------------------------------------
  // Controlling the Title.
  
  gStyle->SetTitleFont(1); // Title font type
  gStyle->SetTitleTextColor(6); // Title font color
  gStyle->SetTitleW(1.0); // Title font size horizontal
  gStyle->SetTitleH(0.1); // Title font size vertical; real font size is an interplay between them!
  gStyle->SetTitleY(0.99); // Title vertical position
  gStyle->SetTitleX(0.5); // Title horizontal position
  
  // --------------------------------------------------------------------
  // Controlling the X-axis is done in a similar manner. Now "x" tells that it is about the x-axis!
  // NB: root calls the numerical scale a label. The text table is called a title in root!
  
  gStyle->SetAxisColor(12,"x"); // x-axis own color
  gStyle->SetTickLength(0.03,"x"); // x-axis length of the numerical scale markers
  gStyle->SetNdivisions(1010,"x"); // amount of scale markers. n = n1+100*n2+10000*n3 primary, secondary & third divisions. default = 510.
  
  gStyle->SetLabelFont(2,"x"); // x-axis numerical scale font type
  gStyle->SetLabelColor(12,"x"); // x-axis numerical scale font color
  gStyle->SetLabelSize(0.04,"x"); // x-axis numerical scale font size
  gStyle->SetLabelOffset(0.01,"x"); // x-axis numerical scale vertical positionthe same as the histogram!
  
  gStyle->SetTitleFont(1,"x"); // x-axis label font type
  gStyle->SetTitleColor(1,"x"); // x-axis label font colorRaw->LabelsOption("v");
  gStyle->SetTitleSize(0.05,"x"); // x-axis label font size
  gStyle->SetTitleOffset(1.0,"x"); // x-axis label vertical position
  
  // --------------------------------------------------------------------
  // Controlling the Y-axis is done in a similar manner. Now "y" tells that it is about the y-axis!
  
  gStyle->SetAxisColor(12,"y"); // y-axis own color
  gStyle->SetTickLength(0.03,"y"); // y-axis length of the numerical scale markers
  gStyle->SetNdivisions(1010,"y"); // amount of scale markers. n = n1+100*n2+10000*n3 primary, secondary & third divisions. default = 510.
  
  gStyle->SetLabelFont(2,"y"); // y-axis numerical scale2.0,0.0 font type
  gStyle->SetLabelColor(12,"y"); // y-axis numerical scale font color
  gStyle->SetLabelSize(0.04,"y"); // y-axis numerical scale font size
  gStyle->SetLabelOffset(0.005,"y"); // y-axis numerical scale vertical position
  
  gStyle->SetTitleFont(1,"y"); // y-axis label font type
  gStyle->SetTitleColor(1,"y"); // y-axis label font color
  gStyle->SetTitleSize(0.05,"y"); // y-axis label font size
  gStyle->SetTitleOffset(1.15,"y"); // y-axis label vertical position
  
  // --------------------------------------------------------------------
  // Controlling the Z-axis is done in a similar manner. Now "z" tells that it is about the z-axis!
  
  gStyle->SetAxisColor(12,"z"); // z-axis own color
  gStyle->SetTickLength(0.03,"z"); // z-axis length of the numerical scale markers
  gStyle->SetNdivisions(410,"z"); // amount of scale markers. n = n1+100*n2+10000*n3 primary, secondary & third divisions. default = 510.
  
  gStyle->SetLabelFont(2,"z"); // z-axis numerical scale2.0,0.0 font type
  gStyle->SetLabelColor(12,"z"); // z-axis numerical scale font color
  gStyle->SetLabelSize(0.04,"z"); // z-axis numerical scale font size
  gStyle->SetLabelOffset(0.0,"z"); // z-axis numerical scale vertical position
  
  gStyle->SetTitleFont(1,"z"); // z-axis label font type
  gStyle->SetTitleColor(1,"z"); // z-axis label font color
  gStyle->SetTitleSize(0.05,"z"); // z-axis label font size
  gStyle->SetTitleOffset(0.85,"z"); // z-axis label horizontal position
  
  // =======================================================================================
  // We add some text inside the picture: 
  
  // First text box.
  TPaveText* text = new TPaveText(Xmin+(Xmax-Xmin)*0.93,Ymin+(Ymax-Ymin)*1.00,Xmin+(Xmax-Xmin)*0.94,Ymin+(Ymax-Ymin)*1.01,"NB"); // NB says that no borders are drawn. 
  // Creates an empty text box between (x1,y1) and (x2,y2). Input arguments are in the order x1,y1,x2,y2.
  // Coordinates are on a scale -1 to 1 on the pad!
  // x1 and y1 are left lower corner and x2 and y2 are right upper corner of the box.
  // Now we have the empty text field. Program the field now:
  text->SetFillColor(0); // Color of the box.
  text->AddText("Written by"); // Add text to the box. Text is printed at the center of the box.
  text->SetTextColor(7); // Define text color
  text->SetTextSize(0.03); // Define text size
  text->SetTextFont(1); // Define text font
  text->SetTextAngle(0.0); // Defines the angle at which the text is displayed.
  
  // Second text box:
  TPaveText* text2 = new TPaveText(Xmin+(Xmax-Xmin)*0.93,Ymin+(Ymax-Ymin)*0.96,Xmin+(Xmax-Xmin)*0.94,Ymin+(Ymax-Ymin)*0.97,"NB"); // NB says that no borders are drawn.  
  text2->SetFillColor(0); 
  text2->AddText("C. A. Douma"); 
  text2->SetTextColor(7); 
  text2->SetTextSize(0.03); 
  text2->SetTextFont(1);
  text2->SetTextAngle(0.0); 
  
  // ======================================================================================
  // Finally, we do the real painting:
  
  // In this case we have to be Careful. At first we need to determine how many
  // canvasses we need:
  Int_t NCanvas = 0;
  Int_t HistCount = Nhist;
  while (HistCount>0)
  {
    HistCount = HistCount - (Nvert*Nhor);
    NCanvas = NCanvas + 1;
  }
  
  // Now the number of canvasses we need is given. Make it constant:
  Int_t const CanvasArray_ii = NCanvas;
  Int_t const CanvasArray_i = CanvasArray_ii;
  Int_t const CanvasArray = CanvasArray_i;
  
  // Declare Canvas Array:
  TCanvas* Pages [CanvasArray];
  Int_t index = 0;
  Int_t padnr = 1;
  
  // Now start painting. Notice that the divided Canvas is 
  // first filled horizontally and then vertically!
  for (Int_t k=0; k<CanvasArray; ++k)
  {
    kstr = st.Itoa(k,10);
    Pages[k] = new TCanvas(Title[0] + "_Canvas_" + kstr);
    Pages[k]->Divide(Nhor,Nvert);
    padnr = 1;
    
    for (Int_t khor = 0; khor<Nhor; ++khor)
    {
      for (Int_t kvert = 0; kvert<Nvert; ++kvert)
      {
	padnr = 1 + khor + Nhor*kvert;
        index = (k*(Nhor*Nvert)) + padnr - 1;
	
	Pages[k]->cd(padnr);
        if (index<Nhist) {Raw[index]->UseCurrentStyle();}
        if (index<Nhist) {Raw[index]->Draw();}
        //if (index<Nhist) {text->Draw("same");}
        //if (index<Nhist) {text2->Draw("csame");}
      }
    }
    Pages[k]->SetPad(0.0,0.0,1.0,1.0);
    // This function selects the margins of the entire canvas
    // to the boundaries. If you want to controle the margins
    // between the individual histograms, use the
    // gStyle->SetPadMargin functions higher up in this macro.
    
  }
  
  // That should do it!
  
  // =========================================================================================
  return Pages[0];
}
