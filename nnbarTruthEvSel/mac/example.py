import sys
from ROOT import gSystem
gSystem.Load("libHewes_nnbarTruthEvSel")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing nnbarTruthEvSel..."

sys.exit(0)

