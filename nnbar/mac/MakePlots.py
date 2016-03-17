from ROOT import TTree, TFile
from matplotlib import pyplot as plt

filename = "TrackQuality_ana.root"

file = TFile(filename,"READ")
tree = file.Get("TrackQualityTree")

track_length_pion   = []
track_length_proton = []
track_length_muon   = []

track_energy_pion   = []

for entry in tree:
  for i in xrange(len(entry.pdg_code)):
    if entry.pdg_code[i] == 211 or entry.pdg_code[i] == -211:
      track_length_pion.append(entry.track_length[i])
      track_energy_pion.append(entry.track_energy[i])
    elif entry.pdg_code[i] == 2212:
      track_length_proton.append(entry.track_length[i])
    elif entry.pdg_code[i] == 13 or entry.pdg_code[i] == -13:
      track_length_muon.append(entry.track_length[i])

n_bins = 20

colors = [ 'red', 'blue', 'green' ]
labels = [ 'pion', 'proton', 'muon' ]

data = [ track_length_pion, track_length_proton, track_length_muon ]

plt.hist(data, n_bins, histtype='bar', range=[ 0, 20 ], color = colors, label = labels )
plt.title('Particle track length')
plt.xlabel('Number of trajectory points')
plt.ylabel('Number of particles')
plt.legend()

plt.savefig("plots/tracklength.png")

plt.clf()

plt.hist2d( track_length_pion, track_energy_pion, bins=n_bins, range=[ [ 0, 20 ], [ 0, 1000 ] ] )
plt.title('Pion track length vs energy')
plt.xlabel('Number of trajectory points')
plt.ylabel('Pion energy [MeV]')
plt.colorbar()

plt.savefig("plots/pion_tracklength_vs_energy.png")
