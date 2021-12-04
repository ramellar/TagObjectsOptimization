from itertools import product

tags=["A0","B0","C0","D0","E0","F0","G0","H0","I0","J0","K0","L0","M0","N0","O0","P0","Q0","R0","S0","T0","U0","V0","W0","X0","Y0","Z0","A1","B1","C1","D1","E1","F1","G1","H1","I1","J1","K1","L1","M1","N1","O1","P1","Q1","R1","S1","T1","U1","V1","W1","X1","Y1","Z1","A2","B2","C2","D2","E2","F2","G2","H2","I2","J2","K2","L2","M2","N2","O2","P2","Q2","R2","S2","T2","U2","V2","W2","X2","Y2","Z2","A3","B3","C3","D3","E3","F3","G3","H3","I3","J3","K3","L3","M3","N3","O3","P3","Q3","R3","S3","T3","U3","V3","W3","X3","Y3","Z3","A4","B4","C4","D4","E4","F4","G4","H4","I4","J4","K4","L4","M4","N4","O4","P4","Q4","R4","S4","T4","U4","V4","W4","X4","Y4","Z4","A5","B5","C5","D5","E5","F5","G5","H5","I5","J5","K5","L5","M5","N5","O5","P5","Q5","R5","S5","T5","U5","V5","W5","X5","Y5","Z5","A6","B6","C6","D6","E6","F6","G6","H6","I6","J6","K6","L6","M6","N6","O6","P6","Q6","R6","S6","T6","U6","V6","W6","X6","Y6","Z6","A7","B7","C7","D7","E7","F7","G7","H7","I7","J7","K7","L7","M7","N7","O7","P7","Q7","R7","S7","T7","U7","V7","W7","X7","Y7","Z7","A8","B8","C8","D8","E8","F8","G8","H8","I8","J8","K8","L8","M8","N8","O8","P8","Q8","R8","S8","T8","U8","V8","W8","X8","Y8","Z8"]

mins=[10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46]
maxs_sum=[15, 18, 21, 24, 27, 30, 33, 36, 39, 41, 44, 47, 50, 53, 56, 59, 61, 64]

combinations = product(mins, maxs_sum)

effMin = 0.8

for (tag,th) in zip(tags,combinations):
    print( '                //Progression_'+tag )
    print( '                Double_t Efficiency_Progression_'+tag+' = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., '+str(th[0])+', effMin, '+str(th[0]+th[1])+');' )
    print( '                if(Efficiency_Progression_'+tag+'>=0.9999) Efficiency_Progression_'+tag+' = 1.0001;' )
    print( '                Int_t Int_Efficiency_Progression_'+tag+' = int(Efficiency_Progression_'+tag+'*100);' )
    print( '                ostringstream convert_Progression_'+tag+';' )
    print( '                convert_Progression_'+tag+' << Int_Efficiency_Progression_'+tag+' ;' )
    print( '                TString Result_Progression_'+tag+' = TString(convert_Progression_'+tag+'.str());' )
    print( '                Int_t IsoCut_Progression_'+tag+' = histosIsolation[Result_Progression_'+tag+']->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);' )
    print( '                if(Int_Efficiency_Progression_'+tag+'==100) IsoCut_Progression_'+tag+' = 1000;' )
    print( '                LUT_Progression_'+tag+'->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_'+tag+');' )
    print( "" )
