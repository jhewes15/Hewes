import sys
from ROOT import gSystem
gSystem.Load("libHewes_Pi0Benchmark")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Pi0Benchmark..."

sys.exit(0)

