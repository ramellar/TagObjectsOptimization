import ROOT
#from GBR2LUT import GBR2LUT
from GBR2LUTEmulator import GBR2LUTEmulator

version = "V0p1"

inputFile = "forests_2023/GBRFullLikelihood_Trigger_Stage2_Run3_MC_compressedieta_compressediet_hasEM_isMerged_optimization{}_results.root".format(version)

sortedShapesFile = "CompressionCalibIdent/compressedSortedShapes.txt"

header = """
# Calibration vs |ieta|,ET,hasEM,isMerged Derived from 13 TeV MC (VBF->tautau) using loose tau ID selection, with semi-parametric regression and NO Layer-1 calibration
# The LUT output is (ET_off/ET_L1 - 0.5) between 0 and 2, encoded on 9 bits: corr = LUT_output/256. + 0.5 <-> pt_calib = (pt_raw>>1)+ ((LUT_output*pt_raw)>>8)
# Index is (BinEt<<4)+(BinEta<<2)+(BinHasEM<<1)+BinIsMerged
# compressedE encoded on 5 bits
# abs(compressedieta) encoded on 2 bits
# L1Tau_hasEM and L1Tau_isMerged each encoded on 1 bit
# anything after # is ignored with the exception of the header
# the header is first valid line starting with #<header> versionStr(unused but may be in future) </header>
#<header> V13 9 9</header>
"""

ietapoints = []
isMergedPoints = []
epoints = []
hasEMPoints = []

for i in range(0,4):
    ietapoints.append(i)

for i in range(0,32):
    epoints.append(i)

for i in range(0,2):
    isMergedPoints.append(i)

for i in range(0,2):
    hasEMPoints.append(i)    

gbr2luts = []
gbr2luts.append(GBR2LUTEmulator())
gbr2luts[-1].name = "GBRFullLikelihood_Trigger_Stage2_Run3_MC_compressedieta_compressediet_hasEM_isMerged_optimization{}".format(version)
gbr2luts[-1].inputFileName = inputFile
gbr2luts[-1].outputFileName = "corrections_2023/corrections_Trigger_Stage2_Run3_MC_compressedieta_compressediet_hasEM_isMerged_optimization{}.root".format(version)
gbr2luts[-1].sortedShapes = sortedShapesFile
gbr2luts[-1].variablePoints.append(("abs(compressedieta)",ietapoints))
gbr2luts[-1].variablePoints.append(("compressedE",epoints))
gbr2luts[-1].variablePoints.append(("L1Tau_hasEM",hasEMPoints))
gbr2luts[-1].variablePoints.append(("L1Tau_isMerged",isMergedPoints))
#gbr2luts[-1].variablePoints.append(("abs(compressedieta)",ietapoints))
gbr2luts[-1].retrieveForest()
gbr2luts[-1].createTH4()

# DON'T CARE FOR NOW
# gbr2luts2 = []
# gbr2luts2.append(GBR2LUTEmulator())
# gbr2luts2[-1].name = "GBRFullLikelihood_Trigger_Stage2_2017_compressedieta_compressediet_hasEM_isMerged_MC_SandeepCristina_MC_VBF"
# gbr2luts2[-1].inputFileName = inputFile
# gbr2luts2[-1].outputFileName = "corrections/corrections_Trigger_2017_compressedieta_compressedE_hasEM_isMerged_SandeepCristina_MC_VBF_{}.txt".format(version)
# gbr2luts2[-1].sortedShapes = sortedShapesFile
# gbr2luts2[-1].variablePoints.append(("abs(compressedieta)",ietapoints))
# gbr2luts2[-1].variablePoints.append(("compressedE",epoints))
# gbr2luts2[-1].variablePoints.append(("L1Tau_hasEM",hasEMPoints))
# gbr2luts2[-1].variablePoints.append(("L1Tau_isMerged",isMergedPoints))
# #gbr2luts2[-1].variablePoints.append(("abs(compressedieta)",ietapoints))
# gbr2luts2[-1].retrieveForest()
# gbr2luts2[-1].createLUT(header)
