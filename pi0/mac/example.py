import sys
from ROOT import gSystem
gSystem.Load("libHewes_pi0")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing pi0..."

sys.exit(0)

