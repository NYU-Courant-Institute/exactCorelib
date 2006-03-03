# python script for generating Makefile

import os.path, sys

empty_makefile="""# Makefile

# set the core library directory and include Make.config
CORE_DIR=%s
include ${CORE_DIR}/Make.config

# define targets list
TARGETS=

# default target
all: ${TARGETS}

#@begin targets
#@end targets

#include standard rules from Make.rules
include ${CORE_DIR}/Make.rules
"""
def gen_empty_makefile(core_dir, make_fname="Makefile"):
  if os.path.isfile(make_fname):
    ans=raw_input("%s exists! Do you want to overwrite it? (y/n)" % make_fname)
    if ans != 'y' and ans != 'Y': return
  f = open(make_fname, 'w')
  f.write(empty_makefile % core_dir) 

def add_target(target_name, make_fname="Makefile"):
  if target_name == "": return
  tmp_fn = make_fname+'.bak'
  tmp_f = open(tmp_fn, "w")
  f = open(make_fname, "r")
  cont_line = False
  for line in f.readlines:
    if line.startswith("@begin targets"):
      tmp_f.write(line)
      tmp_f.write("%s:%s.o\n\n" % (target_name, targetname))
    else if line.startswith("TARGETS="):
      if lines.endswith("\\"): 
         cont_line = True
      else:
         tmp_f.write("\\\t%s")
    else if cont_line and not line.endswith("\\"):
         cont_line = False
    tmp_f.write(line)
   tmp_f.close()
   f.close()
   os.system("mv %s %s" % (tmp_fn, make_fname))

if __name__ == '__main__':
  gen_empty_makefile("..")
