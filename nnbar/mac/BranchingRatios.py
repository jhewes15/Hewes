import sys

if len(sys.argv) < 2:
  msg  = '\n'
  msg += "Usage 1: {} $INPUT_ROOT_FILE(s)\n".format(sys.argv[0])
  msg += '\n'
  sys.stderr.write(msg)
  sys.exit(1)

from larlite import larlite as fmwk

filename = "./BranchingRatios.root"

my_proc = fmwk.ana_processor()

for x in xrange(len(sys.argv)-1):
  my_proc.add_input_file(sys.argv[x+1])

my_proc.set_io_mode(fmwk.storage_manager.kREAD)

my_proc.set_ana_output_file(filename)
my_proc.add_process(fmwk.BranchingRatios())

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run()

print
print  "Finished running ana_processor event loop!"
print

sys.exit()
