# Makefile for preparing softwares under Mac OSX

GMP=gmp-4.1.4
MPFR=mpfr-2.2.0
SIP=sip-4.3.2

PLATFORM=macx-g++
PREFIX=$(shell dirname `pwd`)
GMP_PREFIX=${PREFIX}/gmp
MPFR_PREFIX=${PREFIX}/mpfr

#in solaris, you need set ABI=32
ABI="ABI=32"
ABI=

all:

gmp: ${GMP}.tar.gz
	cd ${GMP}; ./configure CC=gcc ${ABI} --prefix=${GMP_PREFIX}

mpfr: ${MPFR}.tar.gz
	cd ${MPFR}; ./configure CC=gcc --prefix=${MPFR_PREFIX} \
	    --with-gmp-include=${GMP_PREFIX}/include \
	    --with-gmp-lib=${GMP_PREFIX}/lib

sip: ${SIP}.tar.gz
	cd ${SIP}; python configure.py -x -p ${PLATFORM} \
	    -b ${PREFIX}/bin -d ${PREFIX}/lib \
	    -e ${PREFIX}/inc -v ${PREFIX}/sip

${GMP}.tar.gz: 
	wget ftp://ftp.gnu.org/gnu/gmp/${GMP}.tar.gz
	gunzip -cd ${GMP}.tar.gz | tar xvf -

${MPFR}.tar.gz: 
	wget http://www.mpfr.org/mpfr-current/${MPFR}.tar.gz
	gunzip -cd ${MPFR}.tar.gz | tar xvf -

${SIP}.tar.gz: 
	wget http://www.riverbankcomputing.com/Downloads/sip4/${MPFR}.tar.gz
	gunzip -cd ${SIP}.tar.gz | tar xvf -

