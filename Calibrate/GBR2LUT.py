import ROOT
import array
from itertools import product

class GBR2LUT:
    def __init__(self):
        self.name = ""
        self.inputFileName = "regression.root"
        self.inputFile = None
        self.forestName = "EBCorrection"
        self.forest = None
        self.inputVariables = []
        self.variablePoints = []
        self.outputFileName = "corrections.txt"
        self.outputRootFile = None
        self.lut = None

#    def retrieveForest(self):
#        ROOT.gSystem.AddDynamicPath("./")
#        ROOT.gSystem.Load("libDictionary_C.so")
#        self.inputFile = ROOT.TFile.Open(self.inputFileName)
#        self.forest = ROOT.MakeNullPointer( "GBRForest" )
#        self.inputFile.GetObject(self.forestName, self.forest)
#        #
#        varlist = ROOT.MakeNullPointer( ROOT.vector("string") )
#        self.inputFile.GetObject("varlistEB", varlist)
#        #if len(varlist)!=2:
#        #    raise StandardError("ERROR: Number of input variables != 2. Not implemented for the moment.")
#        for i,name in enumerate(varlist):
#            if self.variablePoints[i][0]!=name:
#                raise StandardError("ERROR: Input variables are not given in the correct order in self.variablePoints")
#            self.inputVariables.append(name)

    def retrieveForest(self):
        cmsswBase = os.environ["CMSSW_BASE"]
        scram_arch = os.environ["SCRAM_ARCH"]
        ROOT.gSystem.AddDynamicPath("{0}/lib/{1}/".format(cmsswBase, scram_arch))
        ROOT.gSystem.Load("libHiggsAnalysisGBRLikelihood.so") 
        self.inputFile = ROOT.TFile.Open(self.inputFileName)
        self.forest = ROOT.MakeNullPointer( "GBRForestD" ) 
        self.inputFile.GetObject(self.forestName, self.forest)
        #
        varlist = ROOT.MakeNullPointer( ROOT.vector("string") )
        self.inputFile.GetObject("varlistEB", varlist)
        #if len(varlist)!=2:
        #    raise StandardError("ERROR: Number of input variables != 2. Not implemented for the moment.")
        for i,name in enumerate(varlist):
            if self.variablePoints[i][0]!=name:
                raise StandardError("ERROR: Input variables are not given in the correct order in self.variablePoints")
            self.inputVariables.append(name)

    def createLUT(self):
        ## Creating binning for TH2F filling
        bins1 = []
        bins2 = []
        npoints1 = len(self.variablePoints[0][1])
        npoints2 = len(self.variablePoints[1][1])
        #
        binlow = self.variablePoints[0][1][0] - (self.variablePoints[0][1][1]-self.variablePoints[0][1][0])/2.
        bins1.append(binlow)
        for v1,v2 in zip(self.variablePoints[0][1], self.variablePoints[0][1][1:]):
            binlow = (v1+v2)/2.
            bins1.append(binlow)
        binhigh = self.variablePoints[0][1][npoints1-1] + (self.variablePoints[0][1][npoints1-1]-self.variablePoints[0][1][npoints1-2])/2.
        bins1.append(binhigh)
        #
        binlow = self.variablePoints[1][1][0] - (self.variablePoints[1][1][1]-self.variablePoints[1][1][0])/2.
        bins2.append(binlow)
        for v1,v2 in zip(self.variablePoints[1][1], self.variablePoints[1][1][1:]):
            binlow = (v1+v2)/2.
            bins2.append(binlow)
        binhigh = self.variablePoints[1][1][npoints2-1] + (self.variablePoints[1][1][npoints2-1]-self.variablePoints[1][1][npoints2-2])/2.
        bins2.append(binhigh)

        self.lut = ROOT.TH2F("LUT_"+self.name, "LUT", len(bins1)-1, array.array('f',bins1), len(bins2)-1, array.array('f',bins2))

        with open(self.outputFileName, 'w') as output:
            inputPoints = [ it[1] for it in self.variablePoints ]
            for inputs in product(*inputPoints):
                value1 = inputs[0]
                if self.variablePoints[0][2]: value1 = self.variablePoints[0][2][value1] ## value mapping
                value2 = inputs[1]
                if self.variablePoints[1][2]: value2 = self.variablePoints[1][2][value2] ## value mapping
                resp = self.forest.GetResponse(array.array('f',inputs))
                bin1 = self.lut.GetXaxis().FindBin(value1)
                bin2 = self.lut.GetYaxis().FindBin(value2)
                self.lut.SetBinContent(bin1, bin2, resp)
                strInputs = ""
                for inp in inputs:
                    strInputs += str(inp)+" "
                print >>output, strInputs, resp

        self.outputRootFile.cd()
        self.lut.Write()

