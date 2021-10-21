from T3Job import T3Job
import copy
import os
import shutil
import stat
import sys
import datetime
import subprocess
import ROOT


class Tree:
    def __init__(self):
        self.name = "l1TriggerNtuplizer/l1TriggerTree"
        self.files = []
        self.index = 0

    def toString(self):
        s = ""
        files = ""
        for f in self.files:
            files += " "+f
        if self.index==-1:
            s += "MainTree: {0}\n".format(self.name)
            s += "MainFiles:{0}\n".format(files)
        else:
            s += "SecondaryTrees.{0}.Tree: {1}\n".format(self.index,self.name)
            s += "SecondaryTrees.{0}.Files:{1}\n".format(self.index,files)
        return s


class BatchLauncher:
    def __init__(self):
        self.name = ""
        self.exe = os.getcwd()+"/../../merge.exe"
        self.outputDirectory = "./"
        self.outputFile = "merge.root"
        self.treeClass = ""
        self.mainTree = None
        self.secondaryTrees = []
        self.nFilesPerJob = 1
        self.nJobs = 0
        self.queue = "cms"
        self.local = False
        self.version = ""
        self.scram_arch = "slc6_amd64_gcc472"
        self.cmsswDir   = "/home/llr/cms/sauvan/CMSSW/EmptyArea/CMSSW_5_3_5/"


    def prepareArea(self):
        self.findVersion()
        ### perform some checks
        if not os.path.isfile(self.exe):
            raise StandardError("ERROR: cannot find executable")
        if not (stat.S_IXUSR & os.stat(self.exe)[stat.ST_MODE]):
            raise StandardError("ERROR: you don't have exe permission for "+self.exe)
        ### Create running area
        if not os.path.isdir(self.outputDirectory):
            os.makedirs(self.outputDirectory)
        if not os.path.isdir(self.outputDirectory+"/"+self.version):
            os.makedirs(self.outputDirectory+"/"+self.version)
        if not os.path.isdir(self.outputDirectory+"/"+self.version+"/jobs"):
            os.makedirs(self.outputDirectory+"/"+self.version+"/jobs")
        if not os.path.isdir(self.outputDirectory+"/"+self.version+"/logs"):
            os.makedirs(self.outputDirectory+"/"+self.version+"/logs")
        os.symlink(os.getcwd()+"/../../obj/", self.outputDirectory+"/"+self.version+"/obj")

    
        ### Create configurations files
        for i,tree in enumerate(self.secondaryTrees):
            tree.index = i
        inputFiles = copy.copy(self.mainTree.files)
        while len(inputFiles)>0:
            self.nJobs += 1
            inputs = []
            for i in range(0,self.nFilesPerJob):
                inputs.append(inputFiles[0])
                inputFiles = inputFiles[1:]
                if len(inputFiles)==0:
                    break
            mainTree = Tree()
            mainTree.name = self.mainTree.name
            mainTree.files = inputs
            mainTree.index = -1
            outputFile = self.outputDirectory+"/"+self.version+"/"+self.outputFile
            outputFile = outputFile.replace(".root", "_{0}.root".format(self.nJobs-1))
            with open(self.outputDirectory+"/"+self.version+"/jobs/"+self.name+"."+str(self.nJobs)+".config", 'w') as fConfig:
                print >> fConfig, "TreeClass: {0}".format(self.treeClass)
                print >> fConfig, mainTree.toString()
                print >>fConfig, "SecondaryTrees.N: {0}".format(len(self.secondaryTrees))
                for secTree in self.secondaryTrees:
                    print >> fConfig, secTree.toString()
                print >>fConfig, "OutputFile: {0}".format(outputFile)
                print >>fConfig, "\n"


    def printInfo(self):
        print "\n>>>>>> You are going to send", self.nJobs, "jobs on batch <<<<<<"
        print "  "+self.name+"  Version "+self.version
        print ""

    def launch(self, simulate=False):
        self.printInfo()
        answer = ""
        go = False
        while answer!='y' and answer!='n':
            sys.stdout.write("Do you want to continue (y/n)? ")
            answer = sys.stdin.readline()
            answer = answer.strip()
        if answer == 'y':
            go = True
            print "-- LAUNCH\n"
        elif answer =='n':
            print "-- STOP\n"
        if go:
            for id in range(1,self.nJobs+1):
                job = T3Job("job_"+self.name+"."+str(id))
                job.scram_arch = self.scram_arch
                job.cmsswDir = self.cmsswDir
                job.exe = self.exe
                job.queue = self.queue
                job.parameters = "jobs/"+self.name+"."+str(id)+".config"
                job.inputFiles.append(self.outputDirectory+"/"+self.version+"/jobs/"+self.name+"."+str(id)+".config")
                job.outputDir = self.outputDirectory+"/"+self.version
                job.checkParams()
                job.prepareArea()
                job.createScript()
                job.prepareCommand()
                job.launch(self.local, simulate)

    def findVersion(self):
        self.version = "v_1_"+str(datetime.date.today())
        if os.path.isdir(self.outputDirectory):
            listdirs= [f for f in os.listdir(self.outputDirectory) if os.path.isdir(os.path.join(self.outputDirectory,f))]
            numberMax = 0
            for d in listdirs:
                if "v_" in d:
                    number = int(d.split("_")[1])
                    if number > numberMax:
                        numberMax = number
            self.version = "v_"+str(numberMax+1)+"_"+str(datetime.date.today())

    def findLastVersion(self):
        lastVersion = ""
        if os.path.isdir(self.outputDirectory):
            listdirs= [f for f in os.listdir(self.outputDirectory) if os.path.isdir(os.path.join(self.outputDirectory,f))]
            numberMax = 0
            for d in listdirs:
                if "v_" in d:
                    number = int(d.split("_")[1])
                    if number > numberMax:
                        numberMax = number
                        lastVersion = d
        return lastVersion

