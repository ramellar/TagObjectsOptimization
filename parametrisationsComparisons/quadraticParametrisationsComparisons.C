void quadraticParametrisationsComparisons() {
    // const int nminpt=6, nmineff=6, nmaxeffpt=6, nc=4; 
    // // const int nminpt=2, nmineff=2, nmaxeffpt=2, nc=2; 

    // for(Double_t iminpt = 0 ; iminpt < nminpt ; iminpt++)
    // {
    //   for(Double_t imineff = 0 ; imineff < nmineff ; imineff++)
    //     {
    //       for(Double_t imaxeffpt = 0 ; imaxeffpt < nmaxeffpt ; imaxeffpt ++)
    //         {
    //           for(Double_t c = 0 ; c < nc ; c ++)
    //             {
                  double a,b,q,x1,y1,x2,y2, c;

                  // x1 = 26.+1.*iminpt;
                  // y1 = 0.8+0.02*imineff;
                  // x2 = 38. + 2.*imaxeffpt;
                  // y2 = 1;

                  x1 = 31.;
                  y1 = 0.9;
                  x2 = 48.;
                  y2 = 1.;
                  c = 3;

                  a = (x1*y2 - x2*y1)/(x1*x2*(x2 - x1)) + q/(x1*x2);
                  b = y2/x2 - a*x2 -q/x2;
                  q = -0.01+0.01*c;

                  // if (x1!=31. and x2!=48. and y1!=0.9 and q!=0.02) continue;

                  Double_t Slope = (1.-y1)/(x2-x1);
                  Double_t Kmax  = Slope/(x1-x2);

                  TF1 *current = new TF1("current","[0]*x+[1]",x1,x2);
                  current->SetParameters(Slope,1-Slope*x2);

                  TF1 *theirs = new TF1("theirs","[0]*x*x+[1]*x+[2]",x1,x2);
                  theirs->SetParameters(a,b,q);

                  TF1 *mine   = new TF1("mine", "[0]*x + (1. - [0]*[1]) + [2] * (x - [3]) * (x - [1])",x1,x2);
                  mine->SetParameters(Slope,x2,Kmax,x1);

                  TF1 *mine2   = new TF1("mine2", "[0]*x + (1. - [0]*[1]) + [2] * 0.75 * (x - [3]) * (x - [1])",x1,x2);
                  mine2->SetParameters(Slope,x2,Kmax,x1);

                  TF1 *mine3   = new TF1("mine2", "[0]*x + (1. - [0]*[1]) + [2] * 0.5 * (x - [3]) * (x - [1])",x1,x2);
                  mine3->SetParameters(Slope,x2,Kmax,x1);

                  TF1 *left  = new TF1("left","[0]",x1*0.9,x1);
                  left->SetParameters(&y1);
                  TF1 *right = new TF1("right","1.",x2,x2*1.1);
                  TLine *vertL = new TLine(x1, y1*0.97, x1, 1.02);
                  TLine *vertR = new TLine(x2, y1*0.97, x2, 1.02);

                  TCanvas canvas("canvas","canvas",650,600);
                  canvas.SetLeftMargin(0.15);
                  canvas.SetGrid();
                  // canvas.SetLogy();

                  TLegend* leg = new TLegend(0.45,0.15,0.89,0.35);
                  leg->SetBorderSize(0);
                  leg->SetTextSize(0.025);

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
                  theirs->SetLineColor(2);
                  theirs->SetLineWidth(2);
                  mine->SetLineColor(3);
                  mine->SetLineWidth(2);
                  mine2->SetLineColor(4);
                  mine2->SetLineWidth(2);
                  mine3->SetLineColor(5);
                  mine3->SetLineWidth(2);

                  left->Draw("same");
                  right->Draw("same");
                  vertL->Draw("same");
                  vertR->Draw("same");
                  current->Draw("same");
                  theirs->Draw("same");
                  mine->Draw("same");
                  mine2->Draw("same");
                  mine3->Draw("same");

                  leg->AddEntry(current, "Current parametrization", "L");
                  leg->AddEntry(theirs, "Ritik's parametrization", "L");
                  leg->AddEntry(mine, "My parametrization (K = 1.0)", "L");
                  leg->AddEntry(mine2, "My parametrization (K = 0.75)", "L");
                  leg->AddEntry(mine3, "My parametrization (K = 0.5)", "L");
                  leg->AddEntry(left, "Min/Max efficiencies", "L");
                  leg->Draw("same");

                  TString pdfName = "PDFs/quadraticComparisons/comparison_"+to_string(int(x1))+"_"+to_string(y1)+"_"+to_string(int(x2))+"_"+to_string(q)+".pdf";
                  canvas.SaveAs(pdfName);

                }
//             }
//         }
//     }
// }

