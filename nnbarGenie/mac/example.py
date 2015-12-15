import sys
from ROOT import gSystem
gSystem.Load("libHewes_GENIEnnbar")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing GENIEnnbar..."

sys.exit(0)

