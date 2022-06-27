void sigmoidParametrisationsComparisons() {
    // const int nminpt=6, nmineff=6, nmaxeffpt=6, nc=4; 
    // const int nminpt=2, nmineff=2, nmaxeffpt=2, nc=2; 

    // for(Double_t iminpt = 0 ; iminpt < nminpt ; iminpt++)
    // {
    //   for(Double_t imineff = 0 ; imineff < nmineff ; imineff++)
    //     {
    //       for(Double_t imaxeffpt = 0 ; imaxeffpt < nmaxeffpt ; imaxeffpt ++)
    //         {
    //           for(Double_t c = 0 ; c < nc ; c ++)
    //             {
                  double a,b,q,x1,y1,x2,y2;

                  // x1 = 26.+1.*iminpt;
                  // y1 = 0.8+0.02*imineff;
                  // x2 = 38. + 2.*imaxeffpt;
                  // y2 = 1;
                  // a = (x1*y2 - x2*y1)/(x1*x2*(x2 - x1)) + q/(x1*x2);
                  // b = y2/x2 - a*x2 -q/x2;
                  // q = -0.01+0.01*c;

                  x1 = 13;
                  y1 = 0.9;
                  x2 = 57;
                  y2 = 1;

                  Double_t Slope = (1.-y1)/(x2-x1);
                  Double_t Kmax  = Slope/(x1-x2);

                  TF1 *current = new TF1("current","[0]*x+[1]",x1,x2);
                  current->SetParameters(Slope,1-Slope*x2);

                  TF1 *left  = new TF1("left","[0]",x1*0.9,x1);
                  left->SetParameters(&y1);
                  TF1 *right = new TF1("right","1.",x2,x2*1.1);
                  TLine *vertL = new TLine(x1, y1*0.975, x1, 1.02);
                  TLine *vertR = new TLine(x2, y1*0.975, x2, 1.02);

                  TF1 *sigmoid01 = new TF1("sigmoid01","(1-[0]) / (1+exp(-(x-([1]+[2])/2)*0.1)) + [0]",x1*0.9,x2*1.1);
                  sigmoid01->SetParameters(y1, x2, x1);
                  TF1 *sigmoid02 = new TF1("sigmoid02","(1-[0]) / (1+exp(-(x-([1]+[2])/2)*0.2)) + [0]",x1*0.9,x2*1.1);
                  sigmoid02->SetParameters(y1, x2, x1);
                  TF1 *sigmoid05 = new TF1("sigmoid05","(1-[0]) / (1+exp(-(x-([1]+[2])/2)*0.5)) + [0]",x1*0.9,x2*1.1);
                  sigmoid05->SetParameters(y1, x2, x1);
                  TF1 *sigmoid1 = new TF1("sigmoid1","(1-[0]) / (1+exp(-(x-([1]+[2])/2)*1)) + [0]",x1*0.9,x2*1.1);
                  sigmoid1->SetParameters(y1, x2, x1);
                  TF1 *sigmoid2 = new TF1("sigmoid2","(1-[0]) / (1+exp(-(x-([1]+[2])/2)*2)) + [0]",x1*0.9,x2*1.1);
                  sigmoid2->SetParameters(y1, x2, x1);
                  TF1 *sigmoid10 = new TF1("sigmoid10","(1-[0]) / (1+exp(-(x-([1]+[2])/2)*10)) + [0]",x1*0.9,x2*1.1);
                  sigmoid10->SetParameters(y1, x2, x1);

                  TCanvas canvas("canvas","canvas",650,600);
                  canvas.SetLeftMargin(0.15);
                  canvas.SetGrid();
                  // canvas.SetLogy();

                  TLegend* leg = new TLegend(0.65,0.15,0.89,0.35);
                  leg->SetBorderSize(0);
                  leg->SetTextSize(0.02);

                  TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
                  //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
                  pad1->SetLeftMargin(0.15);
                  pad1->SetGridx();         // Vertical grid
                  pad1->SetGridy();         // Vertical grid
                  pad1->Draw();             // Draw the upper pad: pad1
                  pad1->cd();               // pad1 becomes the current pad

                  TF1 *dummy  = new TF1("dummy","-1",x1*0.9,x2*1.1);
                  dummy->SetTitle("");
                  dummy->GetXaxis()->SetLimits(x1*0.9,x2*1.1);
                  dummy->GetYaxis()->SetRangeUser(y1*0.98,1.02);
                  dummy->GetYaxis()->SetTitle("Efficiency");
                  dummy->GetXaxis()->SetTitle("p_{T}(#tau) [GeV]");
                  dummy->Draw();

                  left->SetLineColor(1);
                  left->SetLineWidth(2);
                  right->SetLineColor(1);
                  right->SetLineWidth(2);

                  vertL->SetLineColor(1);
                  vertL->SetLineWidth(2);
                  vertL->SetLineStyle(2);
                  vertR->SetLineColor(1);
                  vertR->SetLineWidth(2);
                  vertR->SetLineStyle(2);

                  current->SetLineColor(16);
                  current->SetLineWidth(2);

                  sigmoid01->SetLineColor(5);
                  sigmoid01->SetLineWidth(2);
                  sigmoid02->SetLineColor(6);
                  sigmoid02->SetLineWidth(2);
                  sigmoid05->SetLineColor(7);
                  sigmoid05->SetLineWidth(2);
                  sigmoid1->SetLineColor(2);
                  sigmoid1->SetLineWidth(2);
                  sigmoid2->SetLineColor(3);
                  sigmoid2->SetLineWidth(2);
                  sigmoid10->SetLineColor(4);
                  sigmoid10->SetLineWidth(2);

                  vertL->Draw("same");
                  vertR->Draw("same");
                  current->Draw("same");
                  sigmoid01->Draw("same");
                  sigmoid02->Draw("same");
                  sigmoid05->Draw("same");
                  sigmoid1->Draw("same");
                  sigmoid2->Draw("same");
                  // sigmoid10->Draw("same");
                  left->Draw("same");
                  right->Draw("same");

                  leg->AddEntry(current, "Current parametrization", "L");
                  leg->AddEntry(sigmoid01, "K = 0.1", "L");
                  leg->AddEntry(sigmoid02, "K = 0.2", "L");
                  leg->AddEntry(sigmoid05, "K = 0.5", "L");
                  leg->AddEntry(sigmoid1, "K = 1.0", "L");
                  leg->AddEntry(sigmoid2, "K = 2.0", "L");
                  // leg->AddEntry(sigmoid10, "10.0 * signmoid", "L");
                  leg->AddEntry(left, "Min/Max efficiencies", "L");
                  leg->Draw("same");

                  TString pdfName = "PDFs/signmoidsComparisons/comparison_"+to_string(int(x1))+"_"+to_string(y1)+"_"+to_string(int(x2))+".pdf";
                  canvas.SaveAs(pdfName);

                }
//             }
//         }
//     }
// }

