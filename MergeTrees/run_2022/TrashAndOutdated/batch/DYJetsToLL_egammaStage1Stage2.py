from BatchLauncher import BatchLauncher,Tree

outputDirectory = "/home/llr/cms/sauvan/DATA/sauvan/L1/Ntuples/EGammaStage1Stage2/DYJetsToLL_M-50_13TeV-pythia6/Fall13dr-tsg_PU40bx25_POSTLS162_V2-v1/"
outputFile = "tree_electron_stage1_stage2.root"

mainDirectory = "root://polgrid4.in2p3.fr//dpm/in2p3.fr/home/cms/trivcat/store/user/jsauvan/L1_trees/electronTrees/DYJetsToLL_M-50_13TeV-pythia6/Fall13dr-tsg_PU40bx25_POSTLS162_V2-v1/AODSIM/2014_07_01/"

mainFiles = []
mainFiles.append(mainDirectory+"tree_electron_10_1_kaa.root")
mainFiles.append(mainDirectory+"tree_electron_11_1_Cvw.root")
mainFiles.append(mainDirectory+"tree_electron_12_1_nqX.root")
mainFiles.append(mainDirectory+"tree_electron_13_1_EgS.root")
mainFiles.append(mainDirectory+"tree_electron_14_1_SUB.root")
mainFiles.append(mainDirectory+"tree_electron_15_1_lTp.root")
mainFiles.append(mainDirectory+"tree_electron_16_1_dzJ.root")
mainFiles.append(mainDirectory+"tree_electron_17_1_Uaa.root")
mainFiles.append(mainDirectory+"tree_electron_18_1_1QN.root")
mainFiles.append(mainDirectory+"tree_electron_19_1_UWo.root")
mainFiles.append(mainDirectory+"tree_electron_1_1_KxA.root")
mainFiles.append(mainDirectory+"tree_electron_20_1_FEJ.root")
mainFiles.append(mainDirectory+"tree_electron_21_1_1fk.root")
mainFiles.append(mainDirectory+"tree_electron_22_1_O7a.root")
mainFiles.append(mainDirectory+"tree_electron_23_1_Zsd.root")
mainFiles.append(mainDirectory+"tree_electron_2_1_HuT.root")
mainFiles.append(mainDirectory+"tree_electron_3_1_wAb.root")
mainFiles.append(mainDirectory+"tree_electron_4_1_aAz.root")
mainFiles.append(mainDirectory+"tree_electron_5_1_6bo.root")
mainFiles.append(mainDirectory+"tree_electron_6_1_P6d.root")
mainFiles.append(mainDirectory+"tree_electron_7_1_Yjk.root")
mainFiles.append(mainDirectory+"tree_electron_8_1_F6O.root")
mainFiles.append(mainDirectory+"tree_electron_9_1_t0Y.root")


treeClass = "EgammaStage1Stage2Trees"

mainTree = Tree()
mainTree.name = "electronNtuplizer/electronTree"
mainTree.files = mainFiles

secondaryTrees = []
secondaryTrees.append(Tree())
secondaryTrees[-1].name = "l1TriggerNtuplizer/l1TriggerTree"
secondaryTrees[-1].files = ["root://polgrid4.in2p3.fr//dpm/in2p3.fr/home/cms/trivcat/store/user/jsauvan/L1_trees/Stage2Trees/DYJetsToLL_M-50_13TeV-pythia6/Fall13dr-tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW/2014_09_19/tree_stage2_*.root"]
#
secondaryTrees.append(Tree())
secondaryTrees[-1].name = "l1TriggerNtuplizer/l1TriggerTree"
secondaryTrees[-1].files = ["root://polgrid4.in2p3.fr//dpm/in2p3.fr/home/cms/trivcat/store/user/jsauvan/L1_trees/Stage1Trees/DYJetsToLL_M-50_13TeV-pythia6/Fall13dr-tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW/2014_09_19/tree_stage1_*.root"]

batch = BatchLauncher()
batch.name = "merge_eg_stage1_stage2"
batch.outputDirectory = outputDirectory
batch.outputFile = outputFile
batch.treeClass = treeClass
batch.mainTree = mainTree
batch.secondaryTrees = secondaryTrees
batch.nFilesPerJob = 1
batch.scram_arch = "slc5_amd64_gcc472"
batch.cmsswDir = "/home/llr/cms/sauvan/CMSSW/L1_Emulator/CMSSW_6_2_11/"
