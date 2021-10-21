#! /usr/bin/env python

import sys
import os
import optparse

parser = optparse.OptionParser()
parser.add_option('--config', action='store', type='string', dest='launch', help='Python config file')
parser.add_option('-s', action='store_true', default=False, dest='sim', help='Simulation: jobs are not launched')
(options, args) = parser.parse_args()

curdir = os.getcwd();
sys.path.append(curdir)



if options.launch:
    parameterFile = options.launch
    b = __import__(os.path.splitext(parameterFile)[0]).batch
    b.prepareArea()
    b.launch(simulate=options.sim)
else:
    parser.print_help()
