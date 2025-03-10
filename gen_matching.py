import ROOT
import os
import glob

import os
import ROOT

# Define a function to check the matching between reco tau and gen tau in each event with deltaR < 0.1
ROOT.gInterpreter.Declare("""
    #include <TLorentzVector.h>
    #include <cmath>
    
    bool check_genMatching(float tau_pt, float tau_eta, float tau_phi, float tau_mass,
                            const ROOT::VecOps::RVec<float>& gen_tau_pts,
                            const ROOT::VecOps::RVec<float>& gen_tau_etas,
                            const ROOT::VecOps::RVec<float>& gen_tau_phis,
                            const ROOT::VecOps::RVec<float>& gen_tau_masses,
                            int nGenParticle) {
        // Create TLorentzVector for the reco tau
        TLorentzVector dau;
        dau.SetPtEtaPhiE(tau_pt, tau_eta, tau_phi, tau_mass);
        //cout<<nGenParticle<<endl;
        //cout << "tau_pt: " << tau_pt << endl;
        // Loop over gen taus
        for (size_t i = 0; i < nGenParticle; ++i) {
            //cout << "gen_tau_pts: " << gen_tau_pts << endl;
            //cout << "gen_tau_pts[i]: " << gen_tau_pts[i] << endl;
            TLorentzVector gen_tau;
            gen_tau.SetPtEtaPhiM(gen_tau_pts[i], gen_tau_etas[i], gen_tau_phis[i], gen_tau_masses[i]);

            // Check DeltaR between reco tau and gen tau
            if (dau.DeltaR(gen_tau) < 0.1) {
                return true;  // Match found
            }
        }
        return false;  // No match found
    }
""")

# Define a function to get the best matched gen tau for each reco tau

ROOT.gInterpreter.Declare("""
    #include <TLorentzVector.h>
    #include <cmath>
    #include <limits>  // For maximum float value

    std::tuple<float, float, float, float> get_best_matched_gen(
        float tau_pt, float tau_eta, float tau_phi, float tau_mass,
        const ROOT::VecOps::RVec<float>& gen_tau_pts,
        const ROOT::VecOps::RVec<float>& gen_tau_etas,
        const ROOT::VecOps::RVec<float>& gen_tau_phis,
        const ROOT::VecOps::RVec<float>& gen_tau_masses,
        int nGenParticle) {
        
        TLorentzVector dau;
        dau.SetPtEtaPhiE(tau_pt, tau_eta, tau_phi, tau_mass);
                          
        float matched_pt = -1, matched_eta = -999, matched_phi = -999, matched_mass = -1;
                          
        for (size_t i = 0; i < nGenParticle; ++i) {
            TLorentzVector gen_tau;
            gen_tau.SetPtEtaPhiM(gen_tau_pts[i], gen_tau_etas[i], gen_tau_phis[i], gen_tau_masses[i]);

            if (dau.DeltaR(gen_tau) < 0.1) {
                matched_pt = gen_tau_pts[i];
                matched_eta = gen_tau_etas[i];
                matched_phi = gen_tau_phis[i];
                matched_mass = gen_tau_masses[i];
            }
        }
                                  
        return std::make_tuple(matched_pt, matched_eta, matched_phi, matched_mass);
    }
""")


if __name__ == "__main__":
    indir = '/data_CMS/cms/amella/Run3_2025/MC25_Winter_optmization'
    file = "GluGlu_miniAOD_E"

    outdir = f'/data_CMS/cms/amella/Run3_2025/MC25_Winter_optmization'
    print(" ### INFO: Saving output in ", outdir)
    os.system('mkdir -p ' + outdir)

    file = ROOT.TFile.Open(f'{indir}/{file}.root')
    tree = file.Get("Ntuplizer_noTagAndProbe/TagAndProbe")
    df1 = ROOT.RDataFrame("Ntuplizer_noTagAndProbe/TagAndProbe", file)


    # Define the genMatching function for each event using Define()
    df = df1.Define("genMatched", 
                   "check_genMatching(l1tPt, l1tEta, l1tPhi, l1tE, "
                   "genParticlePt, genParticleEta, genParticlePhi, genParticleMass, nGenParticle)")

    # Apply a filter to keep events where there is a match
    df = df.Filter("genMatched")

    print("Total events with genMatched:", df.Count().GetValue())
    print("Total events in the dataframe:", df1.Count().GetValue())

    

    # df1.Snapshot("Ntuplizer_noTagAndProbe/TagAndProbe", f"{outdir}/MiniAOD_MC_2025_E.root")

    print("Columns in df:", df.GetColumnNames())
    # df.Display(["genParticlePt"]).Print()

    df = df.Define("MatchedGenPt",    "std::get<0>(get_best_matched_gen(l1tPt, l1tEta, l1tPhi, l1tE, genParticlePt, genParticleEta, genParticlePhi, genParticleMass, nGenParticle))")
    df = df.Define("MatchedGenEta",   "std::get<1>(get_best_matched_gen(l1tPt, l1tEta, l1tPhi, l1tE, genParticlePt, genParticleEta, genParticlePhi, genParticleMass, nGenParticle))")
    df = df.Define("MatchedGenPhi",   "std::get<2>(get_best_matched_gen(l1tPt, l1tEta, l1tPhi, l1tE, genParticlePt, genParticleEta, genParticlePhi, genParticleMass, nGenParticle))")
    df = df.Define("MatchedGenMass",  "std::get<3>(get_best_matched_gen(l1tPt, l1tEta, l1tPhi, l1tE, genParticlePt, genParticleEta, genParticlePhi, genParticleMass, nGenParticle))")

    # df.Display(["matched_gen_pt"]).Print()

    df.Snapshot("Ntuplizer_noTagAndProbe/TagAndProbe", f"{outdir}/gen_matched_GluGlu_miniAOD_E.root")