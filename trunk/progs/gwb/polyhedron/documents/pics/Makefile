.PHONE:
	all
BASESK=base.sk
BASETEX=base.tex

CUBESK=cube.sk
CUBETEX=cube.tex

CUBEDERSK=tricube.sk equitricube.sk trapecube.sk\
parallelcube.sk rectcube.sk squarecube.sk pentacube.sk hexacube.sk\
f2ebox.sk f2vbox.sk f2nbox.sk\
f3vbox.sk f3ebox.sk
CUBEDERTEX=tricube.tex equitricube.tex trapecube.tex\
parallelcube.tex rectcube.tex squarecube.tex pentacube.tex hexacube.tex\
f2ebox.tex f2vbox.tex f2nbox.tex\
f3vbox.tex f3ebox.tex

SOLIDSK=solidcube.sk
SOLIDSUBSK=firstsubcube.sk secondsubcube.sk
SOLIDTEX=solidcube.tex
SOLIDSUBTEX=firstsubcube.tex secondsubcube.tex
SKFILES=${BASESK} ${CUBESK} ${CUBEDERSK} ${SOLIDSK} ${SOLIDSUBSK}
TEXFILES=${BASETEX} ${CUBETEX} ${CUBEDERTEX} ${SOLIDTEX} ${SOLIDSUBTEX}
#${CUBEDERTEX} ${CUBEDERSK}:${CUBESK}
#${SOLIDSUBTEX} ${SOLIDSUBSK}:${SOLIDSK}

default:all
all: ${TEXFILES}
%.tex: %.sk 
	sketch  $^ -o $@
clean:
	rm ${TEXFILES}
tex:
	@echo ${TEXFILES}
sk:
	@echo ${SKFILES}
