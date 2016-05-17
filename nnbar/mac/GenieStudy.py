import sys

if len(sys.argv) < 2:
  msg  = '\n'
  msg += "Usage 1: {} $INPUT_ROOT_FILE(s)\n".format(sys.argv[0])
  msg += '\n'
  sys.stderr.write(msg)
  sys.exit(1)

from larlite import larlite as fmwk
from ROOT import TFile, TTree
from matplotlib import pyplot as plt
from matplotlib.colors import colorConverter as cc

#my_proc = fmwk.ana_processor()
#
#for x in xrange(len(sys.argv)-1):
#  my_proc.add_input_file(sys.argv[x+1])
#
#my_proc.set_io_mode(fmwk.storage_manager.kREAD)
#
filename = "./genie_study/files/GenieTree.root"
#my_proc.set_ana_output_file(filename)
#my_proc.add_process(fmwk.GeneratorInfo())
#
#print
#print  "Finished configuring ana_processor. Start event loop!"
#print
#
#my_proc.run()
#
#print
#print  "Finished running ana_processor event loop!"
#print

#print  "Would you like to generate plots? [y/n]"
#while True:
#  choice = raw_input().lower()
#  if choice == "y":
#    break
#  elif choice == "n":
#    sys.exit(0)
#  else:
#    print  "Didn't understand that. Please type \"y\" or \"n\"."

file = TFile(filename,"read")
tree = file.Get("genie_tree")

pion_multiplicity = []
nucleon_multiplicity = []

pion_momentum = []
nucleon_momentum = []

pion_tracklen = []
nucleon_tracklen = []

for event in tree:
  
  pion_multiplicity.append(event.mult_pip + event.mult_pim + event.mult_pi0)
  nucleon_multiplicity.append(event.mult_n + event.mult_p)

  for a in event.mom_pip:
    pion_momentum.append(a)
  for a in event.mom_pim:
    pion_momentum.append(a)
  for a in event.mom_pi0:
    pion_momentum.append(a)

  for a in event.mom_n:
    nucleon_momentum.append(a)
  for a in event.mom_p:
    nucleon_momentum.append(a)

  for a in event.trklen_pip:
    pion_tracklen.append(a)
  for a in event.trklen_pim:
    pion_tracklen.append(a)

  for a in event.trklen_n:
    nucleon_tracklen.append(a)
  for a in event.trklen_p:
    nucleon_tracklen.append(a)

plt.hist( pion_multiplicity, 20, range=[0,20], histtype='stepfilled', edgecolor=cc.to_rgba('b',1), facecolor=cc.to_rgba('b',0.4) , label='pion' )
plt.hist( nucleon_multiplicity, 20, range=[0,20], histtype='stepfilled', edgecolor=cc.to_rgba('r',1), facecolor=cc.to_rgba('r',0.4), label='nucleon' )
plt.title('Primary particle multiplicity')
plt.xlabel('No. particles')
plt.ylabel('No. events')
plt.legend()
plt.savefig("./genie_study/plots/01_multiplicity.pdf")
plt.clf()

plt.hist( pion_momentum, 50, range=[0,1], histtype='stepfilled', edgecolor=cc.to_rgba('b',1), facecolor=cc.to_rgba('b',0.4), normed=True, label='pion' )
plt.hist( nucleon_momentum, 50, range=[0,1], histtype='stepfilled', edgecolor=cc.to_rgba('r',1), facecolor=cc.to_rgba('r',0.4), normed=True, label='nucleon' )
plt.title('Primary particle momentum')
plt.xlabel('Momentum [GeV]')
plt.ylabel('No. particles [normed]')
plt.legend()
plt.savefig("./genie_study/plots/02_momentum.pdf")
plt.clf()

plt.hist( pion_tracklen, 50, range=[0,200], histtype='stepfilled', edgecolor=cc.to_rgba('b',1), facecolor=cc.to_rgba('b',0.4), normed=True, label='pi+/-' )
plt.hist( nucleon_tracklen, 50, range=[0,200], histtype='stepfilled', edgecolor=cc.to_rgba('r',1), facecolor=cc.to_rgba('r',0.4), normed=True, label='nucleon' )
plt.title('Primary particle track length')
plt.xlabel('Track length [cm]')
plt.ylabel('No. particles [normed]')
plt.legend()
plt.savefig("./genie_study/plots/03_tracklen.pdf")

 



