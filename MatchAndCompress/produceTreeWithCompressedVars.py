import ROOT
import array
import math
import operator

compressedIetaFile = "./CompressionLUTs/tauCompressEtaLUT_2bit_v8.txt"
compressedEFile = "./CompressionLUTs/tauCompressELUT_5bit_v8.txt"
compressedShapeFile = "./CompressionLUTs/egCompressShapesLUT_calibr_4bit_v4.txt"
compressednTTFile = "./CompressionLUTs/tauCompressnTTLUT_5bit_v8.txt"

inputFileName  = "/data_CMS/cms/mchiusi/Run3preparation/Run3preparation_2023/2023_07_27_olivier/current_calo_params/RAW_124X_MATCHED_current.root";
outputFileName = "/data_CMS/cms/mchiusi/Run3preparation/Run3preparation_2023/2023_07_27_olivier/current_calo_params/RAW_124X_COMPRESSED_current.root";


treeName = "outTreeForCalibration"


def readLUT(lutFileName):
    lut = {}
    i=0
    print lutFileName
    with open(lutFileName, 'r') as f:
        lines = f.readlines()
        for line in lines:
            print "line #",i," content = ",line
            if line[0]=='#': continue
            if line[0]=="" : continue
            tokens = line.split()
            print "tokens = ",tokens
            if len(tokens)<2: continue
            print tokens[0]
            lut[int(tokens[0])] = int(tokens[1])
            print "lut content",lut[int(tokens[0])]
            ++i
    return lut

def sortShapes(shapeHisto):
    numbers = {}
    nbins = shapeHisto.GetNbinsX()
    for b in range(1,nbins+1):
        numbers[b-1] = shapeHisto.GetBinContent(b)
    #
    sortedShapes = sorted(numbers.iteritems(), key=operator.itemgetter(1))
    #
    zeros = []
    nonzeros = []
    for shape,n in sortedShapes:
        if n==0:
            zeros.append((shape,n))
        else:
            nonzeros.append((shape,n))
    #
    nonzeros.reverse()
    sortedShapes = nonzeros
    sortedShapes.extend(zeros)
    #
    lut = {}
    sortedShape = 0
    for shape,n in sortedShapes:
        lut[shape] = sortedShape
        sortedShape += 1
    #
    with open("compressedSortedShapes.txt", 'w') as f:
        for shape in range(0,128):
            sortedShape = lut[shape]
            print >>f, shape, sortedShape
    return lut


## Read Ieta,E,shape compression mapping
compressedIeta  = readLUT(compressedIetaFile)
compressedE     = readLUT(compressedEFile)
compressedShape = readLUT(compressedShapeFile)
compressednTT = readLUT(compressednTTFile)

print len(compressedIeta)

## Reading input trees 
## + filling compressed shape histo
inputFile = ROOT.TFile.Open(inputFileName)
inputTree = inputFile.Get(treeName)
inputTree.__class__ = ROOT.TTree
shapeHisto = ROOT.TH1F("compressedShapeHisto", "compressedShapeHisto", 128, -0.5, 127.5)


data = {"RunNumber"     :array.array('i',[0]),
        "EventNumber"   :array.array('i',[0]),
        #"Weight"  :array.array('f',[0.]),
        #"group"   :array.array('i',[0]),
        #"Cluster_iEta"    :array.array('i',[0]),
        #"Cluster_iPhi"    :array.array('i',[0]),
        "L1Tau_IEt"       :array.array('i',[0]),
        "L1Tau_IEta"       :array.array('i',[0]),
        "L1Tau_eta"       :array.array('f',[0]),
        #"Cluster_shape"   :array.array('i',[0]),
        #"side"    :array.array('i',[0]),
        "Target"  :array.array('f',[0.]),
        "OfflineTau_pt"   :array.array('f',[0.]),
        "OfflineTau_eta"   :array.array('f',[0.]),
        "OfflineTau_isMatched"   :array.array('i',[0]),
        #"OfflineTau_CALOEnergy"   :array.array('f',[0.]),
        "L1Tau_pt"    :array.array('f',[0.]),
        "L1Tau_isMerged"    :array.array('i',[0]),
        "L1Tau_hasEM"    :array.array('i',[0]),
        "L1Tau_Qual"     :array.array('i',[0]),
        "L1Tau_nTT"      :array.array('i',[0]),
        "L1Tau_Iso"      :array.array('i',[0]),
        "L1Tau_IsoFlag"      :array.array('i',[0]),
        "compressedieta" :array.array('i',[0]),
        "compressedE" :array.array('i',[0]),
        "compressedshape" :array.array('i',[0]),
        "compressedsortedshape" :array.array('i',[0]),
        "compressednTT" :array.array('i',[0]),
       }

print "First pass: reading tree to build compressed shape histo"
nentries = inputTree.GetEntriesFast()
for e in xrange(nentries):
    if inputTree.OfflineTau_isMatched!=1: continue
    inputTree.GetEntry(e)
    data["RunNumber"][0]    = int(inputTree.RunNumber)
    data["EventNumber"][0]  =  int(inputTree.EventNumber)
    data["L1Tau_IEt"][0]      =  int(inputTree.L1Tau_RawIEt)
    data["L1Tau_IEta"][0]      =  int(inputTree.L1Tau_IEta)
    data["L1Tau_eta"][0]      =  inputTree.L1Tau_eta
    #data["Cluster_shape"][0]  =  int(inputTree.Cluster_shape)
    #data["side"][0]   =  int(inputTree.side)
    data["Target"][0] =  inputTree.Target
    data["OfflineTau_pt"][0]  =  inputTree.OfflineTau_pt
    data["OfflineTau_eta"][0]  =  inputTree.OfflineTau_eta
    data["OfflineTau_isMatched"][0]  =  inputTree.OfflineTau_isMatched
    #data["OfflineTau_CALOEnergy"][0] = inputTree.OfflineTau_CALOEnergy
    data["L1Tau_pt"][0]   =  inputTree.L1Tau_pt
    data["L1Tau_isMerged"][0] =  int(inputTree.L1Tau_isMerged)
    data["L1Tau_hasEM"][0] =   int(inputTree.L1Tau_hasEM)
    data["L1Tau_Qual"][0] =   int(inputTree.L1Tau_Qual)
    data["L1Tau_nTT"][0] =   int(inputTree.L1Tau_nTT)
    data["L1Tau_Iso"][0] =   int(inputTree.L1Tau_Iso)
    data["L1Tau_IsoFlag"][0] =   int(inputTree.L1Tau_IsoFlag)
    data["compressedieta"][0]  = int(math.copysign(compressedIeta[abs(data["L1Tau_IEta"][0])], data["L1Tau_IEta"][0]))
    #data["compressedieta"][0]  = int(math.copysign(compressedIeta[abs(data["Cluster_iEta"][0])], data["Cluster_iEta"][0]))
    data["compressedE"][0]     = compressedE[min(data["L1Tau_IEt"][0],255)]
    #data["compressedshape"][0] = compressedShape[data["Cluster_shape"][0]]
    data["compressednTT"][0] = compressednTT[data["L1Tau_nTT"][0]]
    shapeHisto.Fill(data["compressedshape"][0])
    #outputTree.Fill()

#print "First pass: reading tree to build compressed shape histo"
#nentries = inputTree.GetEntriesFast()
#for e in xrange(nentries):
#    inputTree.GetEntry(e)
#    data["Run"][0]    = int(inputTree.Run)
#    data["Event"][0]  =  int(inputTree.Event)
#    data["Weight"][0] =  inputTree.Weight
#    data["group"][0]  =  int(inputTree.group)
#    data["ieta"][0]   =  int(inputTree.ieta)
#    data["iphi"][0]   =  int(inputTree.iphi)
#    data["E"][0]      =  int(inputTree.E)
#    data["shape"][0]  =  int(inputTree.shape)
#    data["side"][0]   =  int(inputTree.side)
#    data["target"][0] =  inputTree.target
#    data["ptoff"][0]  =  inputTree.ptoff
#    data["etl1"][0]   =  inputTree.etl1
#    data["compressedieta"][0]  = int(math.copysign(compressedIeta[abs(data["ieta"][0])], data["ieta"][0]))
#    data["compressedE"][0]     = compressedE[min(data["E"][0],255)]
#    data["compressedshape"][0] = compressedShape[data["shape"][0]]
#    shapeHisto.Fill(data["compressedshape"][0])
#    #outputTree.Fill()

## Sort compressed shapes and write in file
compressedSortedShape = sortShapes(shapeHisto)

## TODO
## Reading and filling tree with compressed and sorted values
outputFile = ROOT.TFile.Open(outputFileName, "RECREATE")
outputTree = ROOT.TTree(treeName, treeName)
for name, a in data.items():
    outputTree.Branch(name, a, "{0}/{1}".format(name, a.typecode.upper()))
shapeHisto.Write()

print "Second pass: reading tree for filling output tree"
for e in xrange(nentries):
    inputTree.GetEntry(e)
    if inputTree.OfflineTau_isMatched!=1: continue
    data["RunNumber"][0]    = int(inputTree.RunNumber)
    data["EventNumber"][0]  =  int(inputTree.EventNumber)
    #data["Weight"][0] =  inputTree.Weight
    #data["group"][0]  =  int(inputTree.group)
    #data["Cluster_iEta"][0]   =  int(inputTree.Cluster_iEta)
    #data["Cluster_iPhi"][0]   =  int(inputTree.Cluster_iPhi)
    data["L1Tau_IEt"][0]      =  int(inputTree.L1Tau_RawIEt)
    #print inputTree.L1Tau_IEta
    data["L1Tau_IEta"][0]      =  int(inputTree.L1Tau_IEta)
    data["L1Tau_eta"][0]      =  inputTree.L1Tau_eta
    #data["Cluster_shape"][0]  =  int(inputTree.Cluster_shape)
    #data["side"][0]   =  int(inputTree.side)
    data["Target"][0] =  inputTree.Target
    data["OfflineTau_pt"][0]  =  inputTree.OfflineTau_pt
    data["OfflineTau_eta"][0]  =  inputTree.OfflineTau_eta
    data["OfflineTau_isMatched"][0]  =  inputTree.OfflineTau_isMatched
    #data["OfflineTau_CALOEnergy"][0]  =  inputTree.OfflineTau_CALOEnergy
    data["L1Tau_pt"][0]   =  inputTree.L1Tau_pt
    data["L1Tau_isMerged"][0]   =  inputTree.L1Tau_isMerged
    data["L1Tau_hasEM"][0]   =  inputTree.L1Tau_hasEM
    data["L1Tau_Qual"][0]   =  inputTree.L1Tau_Qual
    data["L1Tau_nTT"][0]   =  inputTree.L1Tau_nTT
    data["L1Tau_Iso"][0]   =  inputTree.L1Tau_Iso
    data["L1Tau_IsoFlag"][0]   =  inputTree.L1Tau_IsoFlag
    data["compressedieta"][0]  = int(math.copysign(compressedIeta[abs(data["L1Tau_IEta"][0])], data["L1Tau_IEta"][0]))
    #data["compressedieta"][0]  = int(math.copysign(compressedIeta[abs(data["Cluster_iEta"][0])], data["Cluster_iEta"][0]))
    data["compressedE"][0]     = compressedE[min(data["L1Tau_IEt"][0],255)]
    #data["compressedshape"][0] = compressedShape[data["Cluster_shape"][0]]
    data["compressedsortedshape"][0] = compressedSortedShape[data["compressedshape"][0]]
    data["compressednTT"][0] = compressednTT[data["L1Tau_nTT"][0]]
    outputTree.Fill()

outputFile.cd()
outputTree.Write()
outputFile.Close()
inputFile.Close()



