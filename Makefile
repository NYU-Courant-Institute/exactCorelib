# Makefile for level above Corelib2
#########################################
# 
#  The main (default) target there is to lock certain files:
#			Makefiles, Make.config 
#			t.cpp, tt.cpp 
#  so that super-users  may freely
#  modifly these files without accidentally
#  committing these programs.
#	
#  (1) This file is to be run from the Exact Account
#  				inside a Linux machine.
#  (2) You can see what locks are held by whom by typing
#					>> svn status -u
#
#  --Chee Yap
#    Jan 2017
#########################################

default: lock

lock:
	cd trunk; svn lock Makefile Make.config -m "make lock"
	cd trunk/progs; svn lock Makefile Make.options Make.rules \
		-m "make lock"

unlock:
	cd trunk; svn unlock Makefile Make.config 
	cd trunk/progs; svn unlock Makefile Make.options Make.rules 

flock forcedlock:
	cd trunk; svn lock --force Makefile Make.config -m "make lock"
	cd trunk/progs; svn lock --force Makefile Make.options Make.rules \
		-m "make lock"

funlock forcedunlock:
	cd trunk; svn unlock --force Makefile Make.config 
	cd trunk/progs; svn unlock --force Makefile Make.options Make.rules 

########## COMMIT:	commitexact should be called from unix/exact acct:
########## COMMIT:	commit 		should be called from surface pro or bk:
commitexact:
	make unlock; \
		svn -m"Makefile commit from corelib2" commit; \
		make lock;

commit:
		svn -m"Makefile commit from corelib2" commit; \

#########################################
# END
#########################################
