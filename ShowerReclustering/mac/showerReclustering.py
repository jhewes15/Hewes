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
from ROOT import protoshower
from ROOT import calo

# from recotool.showerDef import DefaultShowerReco3D

from ROOT.larutil import PxHit, PxPoint
from ROOT.larlite import spacepoint, shower
from ROOT import TFile, TTree
from matplotlib import pyplot as plt



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
my_proc.set_ana_output_file("data/showerReclustering_ana.root")
# Specify data output root file name
my_proc.set_output_file("data/showerReclustering.root")

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

sr3d.SetInputProducer("pandoraNuMerged")
sr3d.SetOutputProducer("showerreco")
sr3d.SetRequirePDG11(False)
my_proc.add_process(sr3d)

cq = fmwk.ShowerContainment()
cq.SetVerbose(True)
my_proc.add_process(cq)

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run(0)
# my_proc.process_event(2)

outfile = TFile("data/showerReclustering_ana.root","read")
out_tree = outfile.Get("clusterquality")

n_event = 1

for event in out_tree:
  
  for shower in xrange(len(event._pxhits)):
  
    hit_x = []
    hit_y = []
    hit_in_cone_x = []
    hit_in_cone_y = []
    hit_in_new_cone_x = []
    hit_in_new_cone_y = []
    sp_in_cone_x = []
    sp_in_cone_y = []
    sp_not_in_cone_x = []
    sp_not_in_cone_y = []
    cone_x = []
    cone_y = []
    new_cone_x = []
    new_cone_y = []
  
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

    for plane in event._pxhits_in_new_cone[shower]:
      temp_x = []
      temp_y = []
      for hit in plane:
        temp_x.append(hit.w)
        temp_y.append(hit.t)
      hit_in_new_cone_x.append(temp_x)
      hit_in_new_cone_y.append(temp_y)

    for plane in event._sp_in_cone_2d[shower]:
      temp_x = []
      temp_y = []
      for point in plane:
        temp_x.append(point.w)
        temp_y.append(point.t)
      sp_in_cone_x.append(temp_x)
      sp_in_cone_y.append(temp_y)
    
    for plane in event._sp_not_in_cone_2d[shower]:
      temp_x = []
      temp_y = []
      for point in plane:
        temp_x.append(point.w)
        temp_y.append(point.t)
      sp_not_in_cone_x.append(temp_x)
      sp_not_in_cone_y.append(temp_y)

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

    for plane in event._new_cone_projection[shower]:
      temp_x = []
      temp_y = []
      for point in plane:
        temp_x.append(point.w)
        temp_y.append(point.t)
      if len(temp_x):
        temp_x.append(temp_x[0])
        temp_y.append(temp_y[0])
      new_cone_x.append(temp_x)
      new_cone_y.append(temp_y)

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

    plt.savefig("./plots/hit_{:03}_{:02}.png".format(n_event,shower + 1))
    plt.clf()
    
    if event._new_cone[shower]:
      
      newpurity = [ 0., 0., 0., 0., 0. ]
      
      for i in xrange(3):
        if len(hit_x[i]):
          newpurity[i] = 100. * float(len(hit_in_new_cone_x[i]))/float(len(hit_x[i]))
        if len(hit_x[0]) or len(hit_x[1]) or len(hit_x[2]):
          newpurity[3] = 100. * float(len(hit_in_new_cone_x[0])+len(hit_in_new_cone_x[1])+len(hit_in_new_cone_x[2]))/float(len(hit_x[0])+len(hit_x[1])+len(hit_x[2]))
        newpurity[4] = purity[4]
      
      for i in xrange(len(hit_x)):
        plt.subplot("31{}".format(i+1))
        plt.plot(new_cone_x[i], new_cone_y[i], "b")
        plt.plot(hit_x[i], hit_y[i], "r.")
        plt.text(0.7, 0.05, "Containment: {:.1f}%".format(newpurity[i]), transform=plt.gca().transAxes)
        if i == 0:
          plt.text(0.03, 0.8, "3D containment: {:.1f}%".format(newpurity[4]), transform=plt.gca().transAxes)

      plt.savefig("./plots/newcone_{:03}_{:02}.png".format(n_event,shower + 1))
      plt.clf()
    
    
    for i in xrange(len(cone_x)):
      plt.subplot("31{}".format(i+1))
      plt.plot(cone_x[i], cone_y[i], "b")
      plt.plot(sp_in_cone_x[i], sp_in_cone_y[i], "g.")
      plt.plot(sp_not_in_cone_x[i], sp_not_in_cone_y[i], "r.")
      plt.text(0.7, 0.05, "Containment: {:.1f}%".format(purity[i]), transform=plt.gca().transAxes)
      if i == 0:
        plt.text(0.03, 0.8, "3D containment: {:.1f}%".format(purity[4]), transform=plt.gca().transAxes)

    plt.savefig("./plots/spacepoint_{:03}_{:02}.png".format(n_event,shower + 1))
    plt.clf()

  n_event = n_event + 1

sys.exit()

