tags="A0 B0 C0 D0 E0 F0 G0 H0 I0 J0 K0 L0 M0 N0 O0 P0 Q0 R0 S0 T0 U0 V0 W0 X0 Y0 Z0 A1 B1 C1 D1 E1 F1 G1 H1 I1 J1 K1 L1 M1 N1 O1 P1 Q1 R1 S1 T1 U1 V1 W1 X1 Y1 Z1 A2 B2 C2 D2 E2 F2 G2 H2 I2 J2 K2 L2 M2 N2 O2 P2 Q2 R2 S2 T2 U2 V2 W2 X2 Y2 Z2 A3 B3 C3 D3 E3 F3 G3 H3 I3 J3 K3 L3 M3 N3 O3 P3 Q3 R3 S3 T3 U3 V3 W3 X3 Y3 Z3 A4 B4 C4 D4 E4 F4 G4 H4 I4 J4 K4 L4 M4 N4 O4 P4 Q4 R4 S4 T4 U4 V4 W4 X4 Y4 Z4 A5 B5 C5 D5 E5 F5 G5 H5 I5 J5 K5 L5 M5 N5 O5 P5 Q5 R5 S5 T5 U5 V5 W5 X5 Y5 Z5 A6 B6 C6 D6 E6 F6 G6 H6 I6 J6 K6 L6 M6 N6 O6 P6 Q6 R6 S6 T6 U6 V6 W6 X6 Y6 Z6 A7 B7 C7 D7 E7 F7 G7 H7 I7 J7 K7 L7 M7 N7 O7 P7 Q7 R7 S7 T7 U7 V7 W7 X7 Y7 Z7 A8 B8 C8 D8 E8 F8 G8 H8 I8 J8 K8 L8 M8 N8 O8 P8 Q8 R8 S8 T8 U8 V8 W8 X8 Y8 Z8"
#tags="A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"

for tag in $tags; do
    echo '"'$tag'",'

    #echo 'TH3F* LUT_Progression_'$tag' = new TH3F("LUT_Progression_'$tag'","LUT_Progression_'$tag'",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);'

    if [[ $tag == *"0"* ]]; then
        effMin="0."
    elif [[ $tag == *"1"* ]]; then
        effMin="0.1"
    elif [[ $tag == *"2"* ]]; then
        effMin="0.2"
    elif [[ $tag == *"3"* ]]; then
        effMin="0.3"
    elif [[ $tag == *"4"* ]]; then
        effMin="0.4"
    elif [[ $tag == *"5"* ]]; then
        effMin="0.5"
    elif [[ $tag == *"6"* ]]; then
        effMin="0.6"
    elif [[ $tag == *"7"* ]]; then
        effMin="0.7"
    elif [[ $tag == *"8"* ]]; then
        effMin="0.8"
    fi

    if [[ $tag == *"A"* ]]; then
        MIN="10"
        MAX="30"
        COL="kRed"
    elif [[ $tag == *"B"* ]]; then
        MIN="10"
        MAX="35"
        COL="kRed-4"
    elif [[ $tag == *"C"* ]]; then
        MIN="10"
        MAX="40"
        COL="kRed-7"
    elif [[ $tag == *"D"* ]]; then
        MIN="10"
        MAX="45"
        COL="kRed-9"
    elif [[ $tag == *"E"* ]]; then
        MIN="10"
        MAX="50"
        COL="kRed-10"
    elif [[ $tag == *"F"* ]]; then
        MIN="15"
        MAX="35"
        COL="kMagenta"
    elif [[ $tag == *"G"* ]]; then
        MIN="15"
        MAX="40"
        COL="kMagenta-4"
    elif [[ $tag == *"H"* ]]; then
        MIN="15"
        MAX="45"
        COL="kMagenta-7"
    elif [[ $tag == *"I"* ]]; then
        MIN="15"
        MAX="50"
        COL="kMagenta-9"
    elif [[ $tag == *"J"* ]]; then
        MIN="15"
        MAX="55"
        COL="kMagenta-10"
    elif [[ $tag == *"K"* ]]; then
        MIN="20"
        MAX="40"
        COL="kBlue"
    elif [[ $tag == *"L "* ]]; then
        MIN="20"
        MAX="45"
        COL="kBlue-4"
    elif [[ $tag == *"M"* ]]; then
        MIN="20"
        MAX="50"
        COL="kBlue-7"
    elif [[ $tag == *"N"* ]]; then
        MIN="20"
        MAX="55"
        COL="kBlue-9"
    elif [[ $tag == *"O"* ]]; then
        MIN="20"
        MAX="60"
        COL="kBlue-10"
    elif [[ $tag == *"P"* ]]; then
        MIN="25"
        MAX="45"
        COL="kCyan"
    elif [[ $tag == *"Q"* ]]; then
        MIN="25"
        MAX="50"
        COL="kCyan-4"
    elif [[ $tag == *"R"* ]]; then
        MIN="25"
        MAX="55"
        COL="kCyan-7"
    elif [[ $tag == *"S"* ]]; then
        MIN="25"
        MAX="60"
        COL="kCyan-9"
    elif [[ $tag == *"T"* ]]; then
        MIN="25"
        MAX="65"
        COL="kCyan-10"
    elif [[ $tag == *"U"* ]]; then
        MIN="30"
        MAX="50"
        COL="kGreen"
    elif [[ $tag == *"V"* ]]; then
        MIN="30"
        MAX="55"
        COL="kGreen-4"
    elif [[ $tag == *"W"* ]]; then
        MIN="30"
        MAX="60"
        COL="kGreen-7"
    elif [[ $tag == *"X"* ]]; then
        MIN="30"
        MAX="65"
        COL="kGreen-9"
    elif [[ $tag == *"Y"* ]]; then
        MIN="30"
        MAX="70"
        COL="kGreen-10"
    elif [[ $tag == *"Z"* ]]; then
        MIN="35"
        MAX="70"
        COL="kYellow"
    fi

    echo '//Progression_'$tag
    echo 'Double_t Efficiency_Progression_'$tag' = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., '$MIN', '$effMin', '$MAX');'
    echo 'if(Efficiency_Progression_'$tag'>=0.9999) Efficiency_Progression_'$tag' = 1.0001;'
    echo 'Int_t Int_Efficiency_Progression_'$tag' = int(Efficiency_Progression_'$tag'*100);'
    echo 'ostringstream convert_Progression_'$tag';'
    echo 'convert_Progression_'$tag' << Int_Efficiency_Progression_'$tag' ;'
    echo 'TString Result_Progression_'$tag' = TString(convert_Progression_'$tag'.str());'
    echo 'Int_t IsoCut_Progression_'$tag' = histosIsolation[Result_Progression_'$tag']->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);'
    echo 'if(Int_Efficiency_Progression_'$tag'==100) IsoCut_Progression_'$tag' = 1000;'
    echo 'LUT_Progression_'$tag'->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_'$tag');'
    echo ""

    # echo 'LUT_Progression_'$tag'->Write();'

    # echo 'TH1F* pt_Progression_'$tag' = new TH1F("pt_Progression_'$tag'","pt_Progression_'$tag'",240,0.,240.);'

    # echo 'TH2F* pt_DiTau_Progression_'$tag' = new TH2F("pt_DiTau_Progression_'$tag'","pt_DiTau_Progression_'$tag'",240,0.,240.,240,0.,240.);//'

    # echo 'bool Filled_Progression_'$tag' = kFALSE;'

    # echo 'std::vector<Int_t> Index_Taus_Progression_'$tag';'
    # echo 'Index_Taus_Progression_'$tag'.push_back(-1); Index_Taus_Progression_'$tag'.push_back(-1);'
    # echo 'std::vector<Float_t> pt_Taus_Progression_'$tag';'
    # echo 'pt_Taus_Progression_'$tag'.push_back(-99.); pt_Taus_Progression_'$tag'.push_back(-99.);'
    # echo 'std::vector<Float_t> eta_Taus_Progression_'$tag';'
    # echo 'eta_Taus_Progression_'$tag'.push_back(-99.); eta_Taus_Progression_'$tag'.push_back(-99.);'
    # echo ""

    # echo 'TString Result_Progression_'$tag' = "LUT_Progression_'$tag'";'
    # echo 'Int_t IsoCut_Progression_'$tag' = histosIsolation[Result_Progression_'$tag']->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);'
    # echo 'if(!Filled_Progression_'$tag' && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_'$tag')'
    # echo '    {'
    # echo '        pt_Progression_'$tag'->Fill(in_CalibPt->at(iL1Tau));'
    # echo '        Filled_Progression_'$tag' = kTRUE;'
    # echo '    }'
    # echo ""

    # echo 'if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_'$tag'.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_'$tag')'
    # echo '    {'
    # echo '        Index_Taus_Progression_'$tag'.at(1)=Index_Taus_Progression_'$tag'.at(0);'
    # echo '        pt_Taus_Progression_'$tag'.at(1)=pt_Taus_Progression_'$tag'.at(0);'
    # echo '        Index_Taus_Progression_'$tag'.at(0)=iL1Tau;'
    # echo '        pt_Taus_Progression_'$tag'.at(0)=in_CalibPt->at(iL1Tau);'
    # echo '    }'
    # echo 'else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_'$tag'.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_'$tag')'
    # echo '    {'
    # echo '        Index_Taus_Progression_'$tag'.at(1)=iL1Tau;'
    # echo '        pt_Taus_Progression_'$tag'.at(1)=in_CalibPt->at(iL1Tau);'
    # echo '    }'

    # echo 'if(Index_Taus_Progression_'$tag'.at(0)>=0 && Index_Taus_Progression_'$tag'.at(1)>=0)'
    # echo '            {'
    # echo '                pt_DiTau_Progression_'$tag'->Fill(pt_Taus_Progression_'$tag'.at(0),pt_Taus_Progression_'$tag'.at(1),weight);'
    # echo '            }'

    # echo 'TH1F* rate_Progression_'$tag' = new TH1F("rate_Progression_'$tag'","rate_Progression_'$tag'",240,0.,240.);'

    # echo 'TH1F* rate_DiTau_Progression_'$tag' = new TH1F("rate_DiTau_Progression_'$tag'","rate_DiTau_Progression_'$tag'",240,0.,240.);'

    # echo 'rate_Progression_'$tag'->SetBinContent(i+1,pt_Progression_'$tag'->Integral(i+1,241)/Denominator*scale);'

    # echo 'rate_DiTau_Progression_'$tag'->SetBinContent(i+1,pt_DiTau_Progression_'$tag'->Integral(i+1,241,i+1,241)/Denominator*scale);'

    # echo 'pt_Progression_'$tag'->Write();'

    # echo 'pt_DiTau_Progression_'$tag'->Write();'

    # echo 'rate_Progression_'$tag'->Write();'

    # echo 'rate_DiTau_Progression_'$tag'->Write();'

    # echo 'TH1F* rate_NewLayer1_Option'$tag'_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_'$tag'");'

    # echo 'rate_NewLayer1_Option'$tag'_mean->SetLineColor('$COL');'
    # echo 'rate_NewLayer1_Option'$tag'_mean->SetLineWidth(2);'
    # echo 'rate_NewLayer1_Option'$tag'_mean->Draw("same");'
    # echo ""

    # echo 'leg->AddEntry(rate_NewLayer1_Option'$tag'_mean,"Di-#tau iso (Option '$tag')","L");'

    # echo 'TH1F* ratioPlot'$tag'_mean = (TH1F*)rate_NewLayer1_Option'$tag'_mean->Clone("ratioPlot");'
    # echo 'ratioPlot'$tag'_mean->Divide(rate_NewLayer1_noIso_mean);'
    # echo 'ratioPlot'$tag'_mean->SetLineColor('$COL');'
    # echo 'ratioPlot'$tag'_mean->Draw("same");'
    # echo ""

    # echo 'Double_t Threshold_NewLayer1_Option'$tag' = 0.;'

    # echo 'for(UInt_t i = 1 ; i <= rate_NewLayer1_Option'$tag'_mean->GetNbinsX() ; ++i)'
    # echo '{'
    # echo '    if(rate_NewLayer1_Option'$tag'_mean->GetBinContent(i)<=Target)'
    # echo '    {'
    # echo '        Threshold_NewLayer1_Option'$tag' = rate_NewLayer1_Option'$tag'_mean->GetBinLowEdge(i);'
    # echo '        break;'
    # echo '    }'
    # echo '}'
    # echo ""

    # echo 'cout<<"Double_t Threshold_NewLayer1_Option'$tag' = "<<Threshold_NewLayer1_Option'$tag'-0.49<<";"<<endl;'
    
    # echo 'cout<<"TString Threshold_NewLayer1_Option'$tag'_newnTT = \""<<Threshold_NewLayer1_Option'$tag'<<"\";"<<endl;'

    # echo 'TH1F* pt_pass_Option'$tag'  = new TH1F("pt_pass_Option'$tag'" ,"pt_pass_Option'$tag'" ,21,binning);'

    # echo 'Int_t Cut_L1Tau_Iso_Option'$tag'  = histosIsolation["LUT_Progression_'$tag'"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);'

    # echo 'if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option'$tag' && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option'$tag') pt_pass_Option'$tag'->Fill(in_OfflineTau_pt);'

    # echo 'TGraphAsymmErrors* turnOn_Option'$tag' = new TGraphAsymmErrors(pt_pass_Option'$tag',pt,"cp");'
    # echo 'turnOn_Option'$tag'->Write();'
    # echo ""

done


