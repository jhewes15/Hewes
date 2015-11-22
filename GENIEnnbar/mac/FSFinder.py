import sys, ROOT

from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(100):
  filename = '/Users/jhewes15/neutrino/larlite/data/nnbar/larlite_mcinfo_{}.root'.format(x)
  file = ROOT.TFile(filename)
  if not file.IsZombie():
    my_proc.add_input_file(filename)

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("FinalStateSummary.root");

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
my_proc.add_process(fmwk.FinalStates())

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run();

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)
