# Makefile for all examples in ${CORE}/progs
############################################################

############################################################
# variables
############################################################

### The programs in basicprogs will be automatically tested
### 	on ALL platforms when you do
###	"make test" or "make time":

basicprogs=bareiss \
	chull \
	compare \
	demos \
	fileIO \
	fortune \
	generic \
	kahan \
	pi \
	poly \
	sumOfSqrts \
	testIdent \
	tutorial

### The programs in moreprogs will be automatically tested
### 	on ALL unix-type platforms (sun, sgi, cyg, gnu) when you do
###	"make test" or "make time":

moreprogs= \
	chullGraham \
	curves \
	delaunay \
	ieee \
	gaussian \
	geom2d \
	geom3d \
	heron \
	hypergeom \
	nestedSqrt \
	pentagon \
	prover \
	radical \
	testFilter \
	testIO \
	tests

### The programs in extraprogs are not automatically testd on
### 	any platforms.  You will have to run it yourself:

extraprogs=cgal \
	determinant \
	continuedFrac \
	constants \

allprogs= ${basicprogs} ${moreprogs} ${extraprogs}


############################################################
# targets
############################################################
default: makebasic

makebasic:
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p; echo ""; fi; \
	done

test testbasic:
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p test; echo ""; fi; \
	done

############################################################

makemoreprogs: 
	@for p in ${moreprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p; echo ""; fi; \
	done

moretest: makemoreprogs
	@for p in ${moreprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p moretest test; echo ""; fi; \
	done
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p moretest; echo ""; fi; \
	done

############################################################

makeall:  makeprogs
	@for p in ${extraprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p; echo ""; fi; \
	done


############################################################
# Housekeeping
############################################################
clean: cleanbasic
	@for p in ${moreprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p clean; echo ""; fi; \
	done

cleanbasic: 
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p clean; echo ""; fi; \
	done

veryclean: verycleanbasic
	@for p in ${moreprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p veryclean; echo ""; fi\
	done

verycleanbasic: 
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p veryclean; echo ""; fi\
	done
