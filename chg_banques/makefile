
# include $(ADMORA)/makeproc.$(MACHINE)

MAIN = chg_banques

INC_H = \
 Fichier.h\
 Astring.h\
 utile.h\
 List.h\
 Cerr.h\
 temps.h\
 ConnexionOracle.h\
 mallocCtl.h\
 Banques.h\
 chg_banques.h

OTHER_H =

SRC_PC=\
 ConnexionOracle.pc\
 Banques.pc
SRC_C=\
 Fichier.c\
 Astring.c\
 utile.c\
 List.c\
 Cerr.c\
 temps.c\
 mallocCtl.c\
 chg_banques.c


MAIN_OBJ = $(MAIN).o $(INC_H:.h=.o)
MAIN_INC = $(INC_H) $(OTHER_H)


INCLUDE = -I. -I$(ORTINC) -I$(TUXDIR)/include -I $(PROJET)/include
CMNOBJ = $(ORTOBJ)/exploit.o $(ORTOBJ)/orstring.o $(ORTOBJ)/traceTP.o \
         $(ORTLIB)/ortproc.a
LOCALLIBS=

OPTIONILD = -z i_quiet
OPTIONLD = -g -L$(HOME)/lib -L$(ORTLIB) -L.
LIBRARIES = -ltrace -lnsl -lclntsh

# OPTIONCC_DEBUG = -g -xs -KPIC
OPTIONCC_DEBUG = -g -xs
OPTIONCC_OPTIM = -O
OPTIONCC =  $(INCLUDE) $(OPTIONCC_DEBUG) $(TUXOPT)
CCOOPT = $(OPTIONCC)	# utile pour CCO
CPPOPT = $(INCLUDE)	# utile pour CPP

CC = cc -xCC

.pc.o:
	@echo "Pré-processing en cours ..."
	$(CPP) $(OPTION) $*.pc > $*.pcc
	@echo "Pré-compilation en cours ..."
	$(PROC) iname=$*.pcc oname=$*.c $(PLSPCCFLAGS)
	@echo "Compilation en cours ..."
	$(CCO) $*.c -o $*.o


# .SUFFIXES: .o .c .pc
# .c.o : $*.h $(SRVINC)
#	$(CC) $(OPTIONCC) $(INCLUDE) -c $*.c -o $*.o

all: main

main:		$(MAIN)

install: $(MAIN)
	cp $(MAIN) $(EXEC)

preprod:
	cp $(EXEC)/$(MAIN) $(PREPROD)/exec
	cp $(MAIN).c $(MAIN_INC) $(SRC_C) $(SRC_PC) makefile $(PREPROD)/c/$(MAIN)

# Nettoyage pour recompilation
clean:
	rm -f *.pcc $(SRC_PC:.pc=.c) *.o $(MAIN)


# Compilation du batch
$(MAIN): $(MAIN).c $(MAIN_INC) $(MAIN_OBJ) $(LOCALLIBS) makefile
	$(CC) -o $@ $(CCOOPT) $(OPTIONLD) $(MAIN).c $(MAIN_OBJ) \
                    $(CMNOBJ) $(LIBRARIES)

# dépendances obtenues avec
# makedepend -I $ORTINC -I$(TUXDIR)/include -I $(HOME)/include *.c *.h
