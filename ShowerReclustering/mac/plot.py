import sys
import ROOT
from ROOT.larutil import PxHit, PxPoint
from ROOT.larlite import spacepoint, shower
from ROOT import TFile, TTree
from matplotlib import pyplot as plt

outfile = TFile("data/showerReclustering_ana.root","read")
out_tree = outfile.Get("clusterquality")

n_event = 1

for event in out_tree:
  
  for shower in xrange(len(event._pxhits)):
  
    hit_x = []
    hit_y = []
    hit_in_cone_x = []
    hit_in_cone_y = []
    cone_x = []
    cone_y = []
  
    for plane in event._pxhits[shower]:
      temp_x = []
      temp_y = []
      for hit in plane:
        temp_x.append(hit.w)
        temp_y.append(hit.t)
      hit_x.append(temp_x)
      hit_y.append(temp_y)
 
    for plane in event._pxhits_in_cone[shower]:
      temp_x = []
      temp_y = []
      for hit in plane:
        temp_x.append(hit.w)
        temp_y.append(hit.t)
      hit_in_cone_x.append(temp_x)
      hit_in_cone_y.append(temp_y)

    for plane in event._cone_projection[shower]:
      temp_x = []
      temp_y = []
      for point in plane:
        temp_x.append(point.w)
        temp_y.append(point.t)
      if len(temp_x):
        temp_x.append(temp_x[0])
        temp_y.append(temp_y[0])
      cone_x.append(temp_x)
      cone_y.append(temp_y)

    purity = [ 0., 0., 0., 0., 0. ]

    for i in xrange(3):
      if len(hit_x[i]):
        purity[i] = 100. * float(len(hit_in_cone_x[i]))/float(len(hit_x[i]))
      if len(hit_x[0]) or len(hit_x[1]) or len(hit_x[2]):
        purity[3] = 100. * float(len(hit_in_cone_x[0])+len(hit_in_cone_x[1])+len(hit_in_cone_x[2]))/float(len(hit_x[0])+len(hit_x[1])+len(hit_x[2]))

    purity[4] = 100. * float(len(event._sp_in_cone[shower])) / float(len(event._sp_in_cone[shower]) + len(event._sp_not_in_cone[shower]))
  
    for i in xrange(len(hit_x)):
      plt.subplot("31{}".format(i+1))
      plt.plot(cone_x[i], cone_y[i], "b")
      plt.plot(hit_x[i], hit_y[i], "r.")
      plt.text(0.7, 0.05, "Containment: {:.1f}%".format(purity[i]), transform=plt.gca().transAxes)
      if i == 0:
        plt.text(0.03, 0.8, "3D containment: {:.1f}%".format(purity[4]), transform=plt.gca().transAxes)

    plt.savefig("./plots/display{:03}_{:02}.png".format(n_event,shower + 1))
    plt.clf()

  n_event = n_event + 1

sys.exit()


