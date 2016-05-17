import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)


# Import the needed modules.  Need larlite and several of it's namespaces
from ROOT import gSystem,TMath
from larlite import larlite as fmwk
from larlite import larutil
from recotool import cmtool, showerreco
from ROOT import protoshower, calo

# from recotool.showerDef import DefaultShowerReco3D

import ROOT
from ROOT.larutil import PxHit, PxPoint
from ROOT import TFile, TTree
from matplotlib import pyplot as plt
from matplotlib.colors import colorConverter as cc


def getShowerRecoAlgModular():
  # This function returns the default shower reco module
  # If you want to extend, customize, or otherwise alter the default
  # reco (which you should!) it's recommended that you do one of two
  # things:
  #   1) Copy this function, and change the lines you need
  #   2) Use this function as is and replace the modules with the built-in functions
  #       to ShowerRecoAlgModular
  # Then, use the return value (alg) in place of ShowerRecoAlg in your python script

  # It should be noted, however, that this "default" set of modules is not yet developed
  # and if you are developing you ought to be updating it here!

  alg = showerreco.ShowerRecoAlgModular()
  alg.SetDebug(False)

  startPoint = showerreco.StartPointPandora()

  axis = showerreco.PCA3DAxis()

  energy = showerreco.LinearEnergy()
  energy.SetGainU(1./0.69)
  energy.SetGainV(1./0.70)
  energy.SetGainY(1./.64)
  energy.SetUseModBox(True)
  energy.setVerbosity(False)

  dqdx = showerreco.dQdxModule()

  dedx = showerreco.dEdxFromdQdx()
  dedx.SetUsePitch(False)
  dedx.setVerbosity(False)

  # alg.AddShowerRecoModule(startPoint)
  alg.AddShowerRecoModule(showerreco.ToyGeoModule())
  alg.AddShowerRecoModule(axis)
  alg.AddShowerRecoModule(energy)
  alg.AddShowerRecoModule(dqdx)
  alg.AddShowerRecoModule(dedx)
  # alg.AddShowerRecoModule(showerreco.StartPoint2DModule()  )
  #alg.AddShowerRecoModule(showerreco.OtherStartPoint3D()  )
  # alg.AddShowerRecoModule(showerreco.ShowerChargeModule()  )

  # alg.AddShowerRecoModule(showerreco.GeoModule())
  alg.PrintModuleList()

  return alg

def DefaultShowerReco3D():

  # Create analysis unit
  ana_unit = fmwk.ShowerReco3D()
    
  # Attach shower reco alg
  sralg = getShowerRecoAlgModular()
  # sralg.SetDebug()
  # sralg.Verbose(False)
  # sralg.Verbose(True)
  # sralg.SetUseArea(True)
  # Attach calo alg
  # calg = calo.CalorimetryAlg()
  # sralg.CaloAlgo(calg)
  #sralg.SetUseModBox(True)
  ana_unit.AddShowerAlgo(sralg)

  return ana_unit


# Create ana_processor instance
my_proc = fmwk.ana_processor()


# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify analysis output root file name
my_proc.set_ana_output_file("data/ShowerHitRestorer_ana.root")
# Specify data output root file name
my_proc.set_output_file("data/ShowerHitRestorer.root")

pfm = fmwk.PFPartMerger()
pfm.SetInputProducer("pandoraNu")
pfm.SetOutputProducer("pandoraNuMerged")
pfm.SetMergeByPCAndVertex( True )
pfm.SetPrimaryConeFactor( 1. )
pfm.SetSecondaryConeFactor( 1. )
my_proc.add_process(pfm)

sr3d=DefaultShowerReco3D()
# set ProtoShower Algo to go from data-products to a ProtoShower object
protoshoweralg = protoshower.ProtoShowerAlgClusterParams()
sr3d.GetProtoShowerHelper().setProtoShowerAlg( protoshoweralg )

sr3d.SetInputProducer("pandoraNu")
sr3d.SetOutputProducer("showerreco")
sr3d.SetRequirePDG11(False)
my_proc.add_process(sr3d)

sr3dm=DefaultShowerReco3D()
# set ProtoShower Algo to go from data-products to a ProtoShower object
sr3dm.GetProtoShowerHelper().setProtoShowerAlg( protoshoweralg )

sr3dm.SetInputProducer("pandoraNuMerged")
sr3dm.SetOutputProducer("showerrecoMerged")
sr3dm.SetRequirePDG11(False)
my_proc.add_process(sr3dm)

shr1 = fmwk.ShowerHitRestorer()
shr1.SetInputProducer("showerreco")
shr1.SetOutputProducer("showerhit")
shr1.SetTreeName("SHR_unmerged")
my_proc.add_process(shr1)

shr2 = fmwk.ShowerHitRestorer()
shr2.SetInputProducer("showerrecoMerged")
shr2.SetOutputProducer("showerhitMerged")
shr2.SetTreeName("SHR_merged")
my_proc.add_process(shr2)


print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run(0)

outfile = TFile("data/ShowerHitRestorer_ana.root","read")
merged_tree = outfile.Get("SHR_merged")

n_event = 1

missing_charge = [ [], [], [] ]
shower_count_merged = []

for event in merged_tree:
  
  shower_count_merged.append(event._n_showers)
  
  n_showers = len(event._cone_projection)
  
  for shower in xrange(n_showers):
  
    hit_candidate_x = [ [], [], [] ]
    hit_candidate_y = [ [], [], [] ]
    hit_x = [ [], [], [] ]
    hit_y = [ [], [], [] ]
    cone_x = [ [], [], [] ]
    cone_y = [ [], [], [] ]
    
    for hit in event._hit_candidates[shower]:
      print int(hit.plane)
      hit_candidate_x[int(hit.plane)].append(hit.w)
      hit_candidate_y[int(hit.plane)].append(hit.t)
  
    for hit in event._associated_hits[shower]:
      hit_x[int(hit.plane)].append(hit.w)
      hit_y[int(hit.plane)].append(hit.t)
 
    for plane in event._cone_projection[shower]:
      cone_x[int(point.plane)].append(point.w)
      cone_y[int(point.plane)].append(point.t)
      for plane in cone_x:
        if len(plane):
          plane.append(plane[0])
      for plane in cone_y:
        if len(plane):
          plane.append(plane[0])


    for plane in xrange(len(hit_x)):
      plt.subplot("31{}".format(plane+1))
      plt.plot(cone_x[plane], cone_y[plane], "b")
      plt.plot(hit_candidate_x[plane], hit_candidate_y[plane], "g.")
      plt.plot(hit_x[plane], hit_y[plane], "r.")

    plt.savefig("./plots/newhits_{:03}_{:02}.png".format(n_event,shower + 1))
    plt.clf()
      
  for plane in xrange(len(event._missing_charge)):
    missing_charge[plane].append(event._missing_charge[plane])

  n_event = n_event + 1

max_charge = max( [ max(missing_charge[0]), max(missing_charge[1]), max(missing_charge[2]) ] )
min_charge = min( [ min(missing_charge[0]), min(missing_charge[1]), min(missing_charge[2]) ] )

for plane in xrange(3):
  if plane == 0:
    plt.title('Charge per plane ignored due to degenerate hits')
  elif plane == 2:
    plt.xlabel('Charge')
  plt.ylabel('No. events')
  plt.subplot("31{}".format(plane+1))
  plt.hist(missing_charge[plane], range=[min_charge,max_charge])
    
plt.savefig("./plots/degenerate_hit_charge_{:03}_{:02}.png".format(n_event,shower + 1))
plt.clf()

unmerged_tree = outfile.Get("SHR_unmerged")

shower_count_unmerged = []

for event in unmerged_tree:
  shower_count_unmerged.append(event._n_showers)

plt.hist(shower_count_merged, 10, range=[0,10], histtype='stepfilled', edgecolor=cc.to_rgba('b',1), facecolor=cc.to_rgba('b',0.4), label='merged pfpart')
plt.hist(shower_count_unmerged, 10, range=[0,10], histtype='stepfilled', edgecolor=cc.to_rgba('r',1), facecolor=cc.to_rgba('r',0.4), label='unmerged pfpart')
plt.title('Number of showers reconstructed in event')
plt.xlabel('Number of showers')
plt.ylabel('Events')
plt.legend()
plt.savefig("./plots/n_showers.png")
plt.clf()

sys.exit()

