.PHONE:
	all
ROOTSK=cube.sk
ROOTTEX=cube.tex
DERSKFILES=hexacube.sk tricube.sk equitricube.sk trapecube.sk\
parallelcube.sk rectcube.sk squarecube.sk pentacube.sk hexacube.sk
DERTEXFILES=hexacube.tex tricube.tex equitricube.tex trapecube.tex\
parallelcube.tex rectcube.tex squarecube.tex pentacube.tex hexacube.tex
SKFILES=${ROOTSK} ${DERSKFILES}
TEXFILES=${ROOTTEX} ${DERTEXFILES}
${DERSKFILES}:${ROOTSK}

all: ${TEXFILES}
%.tex: %.sk 
	sketch  $^ -o $@
clean:
	rm ${TEXFILES}

