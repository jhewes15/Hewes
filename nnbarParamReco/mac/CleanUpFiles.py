import ROOT

for x in xrange(100):
  filename = '/Users/jhewes15/neutrino/larlite/data/{}/larlite_mcinfo_{}.root'.format(sys.argv[1], x)
  file = ROOT.TFile(filename)
  if file.IsZombie():
    
