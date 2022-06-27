void allQuadraticsParametrisationsComparisons() {
    std::vector<float> mins = {10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46};
    std::vector<float> maxs = {15, 18, 21, 24, 27, 30, 33, 36, 39, 41, 44, 47, 50, 53, 56, 59, 61, 64};
    std::vector<float> effMins = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    std::vector<float> kappas = {0.5, 0.75, 1.0};

    TCanvas canvas("canvas","canvas",650,600);
    canvas.SetLeftMargin(0.15);
    canvas.SetGrid();
    // canvas.SetLogy();

    TLegend* leg = new TLegend(0.20,0.75,0.50,0.88);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.025);

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad

    TF1 *dummy  = new TF1("dummy","-1",10.,90.);
    dummy->SetTitle("");
    dummy->GetXaxis()->SetLimits(10.,90.);
    dummy->GetYaxis()->SetRangeUser(0.0, 1.3);
    dummy->GetYaxis()->SetTitle("Efficiency");
    dummy->GetXaxis()->SetTitle("p_{T}(#tau) [GeV]");
    dummy->Draw();

    for(Int_t i = 0 ; i < mins.size() ; i++)
    {
      for(Int_t j = 0 ; j < maxs.size() ; j++)
        {
          for(Int_t h = 0 ; h < effMins.size() ; h ++)
            {
              for(Int_t k = 0 ; k < kappas.size() ; k ++)
                {
                  double x1,y1,x2,y2,kappa;

                  x1 = mins[i];
                  y1 = effMins[h];
                  x2 = mins[i]+maxs[j];
                  y2 = 1;
                  kappa = kappas[k];
                  Double_t Slope = (1.-y1)/(x2-x1);
                  Double_t Kmax  = Slope/(x1-x2);

                  TF1 *quadratic = new TF1("quadratic","[0]*x + (1. - [0]*[1]) + [2] * (x - [3]) * (x - [1])",x1,x2);
                  quadratic->SetParameters(Slope,x2,Kmax*kappa,x1);

                  quadratic->SetLineColor(kBlue-h);
                  quadratic->SetLineWidth(1);

                  quadratic->Draw("same");

                  if(i==0 & j==0 & h==0 & k==0) leg->AddEntry(quadratic, "Quadratic parametrisations", "L");
                }
            }
        }
    }

    for(Int_t i = 0 ; i < mins.size() ; i++)
    {
      for(Int_t j = 0 ; j < maxs.size() ; j++)
        {
          for(Int_t h = 0 ; h < effMins.size() ; h ++)
            {
              for(Int_t k = 0 ; k < kappas.size() ; k ++)
                {
                  double x1,y1,x2,y2,kappa;

                  x1 = mins[i];
                  y1 = effMins[h];
                  x2 = mins[i]+maxs[j];
                  y2 = 1;
                  kappa = kappas[k];
                  Double_t Slope = (1.-y1)/(x2-x1);
                  Double_t Kmax  = Slope/(x1-x2);

                  Double_t mid = (x1+x2)/2;

                  TMarker *marker = new TMarker(mid,Slope*mid + 1-Slope*x2 + Kmax*kappa * (mid-x1) * (mid-x2),7);
                  marker->SetMarkerColor(kRed-h);
                  marker->DrawMarker(mid,Slope*mid + 1-Slope*x2 + Kmax*kappa * (mid-x1) * (mid-x2));

                  if(i==0 & j==0 & h==0 & k==0) leg->AddEntry(marker, "Quadratic mid-efficiency point", "P");
                }
            }
        }
    }

    leg->Draw("same");

    TString pdfName = "PDFs/ALLquadratics.pdf";
    canvas.SaveAs(pdfName);
}

