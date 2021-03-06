##################################################################
# Environnement PRO*C
# ( inclut les librairies oracle, les suffixes par default )
# Affecte LIBHOME, PROLDLIBS, PLSPCCFLAGS (redefinition possible)
##################################################################
include $(ADMORA)/makeproc_$(ORTORAVERSION)_$(ORTMACHINE)
include $(ORTOSTYPE).makefile

all: lib

##################################################################
# versions
# MAJOR.MINOR : � modifier � la main au fil des versions
# les interfaces des versions MINOR sont compatibles entre elles dans une m�me version MAJOR (corrections, �volutions mineures)
# les interfaces des versions MAJOR peuvent �tre incompatibles entre elles (�volutions)
LIB_COMMONS_MAJOR_VER=2
LIB_COMMONS_MINOR_VER=2
LIB_COMMONS_VER=$(LIB_COMMONS_MAJOR_VER).$(LIB_COMMONS_MINOR_VER)
# on ne gere plus qu'un nb pour la version
# LIB_COMMONS_MINOR_VER=5
# LIB_COMMONS_VER=$(LIB_COMMONS_MAJOR_VER).$(LIB_COMMONS_MINOR_VER)
##################################################################

##################################################################
# Options de compilation
#
# OPTPROF=-pg
# OPTWARN=-Wall
# OPTIONCC = -DDEBUG
# OPTIONCC = -O4
# OPTIONCC = -O
OPTIONDEBUG = -g
OPTIONCC = $(OPTIONDEBUG) $(OPTWARN)
# OPTIONCC = $(OPTPROF)

# indications d'include pour le pre-compilateur C
#CPPOPT=$(CCOOPT)
CPPOPT= \
 -I. \
 -I $(HOME)/projets/bfrance/c/test_unit \
 -I $(ORACLE_HOME)/rdbms/public \
 -I $(ORTINC)

#  -I $(PROJET)/include/EBCommons \

# options de compilation et indications d'include pour le compilateur C
#CCOOPT=$(OPTIONCC) $(OPTIONCC_LOCAL) -I.
CCOOPT=$(OPTIONCC_LOCAL) $(OPTIONCC) $(CPPOPT)



# toujours utile ?
#  -I $(ORACLE_HOME)/rdbms/public

PLSPCCFLAGS = \
  ireclen=1200 oreclen=1200 sqlcheck=semantics \
  userid=$(USERNAMEORA)/$(PASSWORDORA) \
  common_parser=yes hold_cursor=yes MODE=ORACLE unsafe_null=yes DBMS=V8

# common_parser=yes	: permet au precompilateur pro*c de comprendre la fonction row_number()
# � ajouter � PLSPCCFLAGS pour modifier la conversion des VARCHAR2 (base) en char[n] (langage hote)
#char_map=string
#
##################################################################


##################################################################
# debut LIB_COMMONS
# nom de la librairie des objets communs
LIB_COMMONS_NAME=EBCommons.$(LIB_COMMONS_MAJOR_VER)

# repertoires
LIB_COMMONS_DIR=$(PROJET)/lib
LIB_COMMONS_INCDIR=$(PROJET)/include/$(LIB_COMMONS_NAME)
LIB_COMMONS_INCDIR_NEW=$(PROJET)/include/$(LIB_COMMONS_NAME).$(LIB_COMMONS_MINOR_VER)

# librairie archive
LIB_COMMONS_AR_NEW=lib$(LIB_COMMONS_NAME).$(LIB_COMMONS_MINOR_VER).a

# librairie so applicative
LIB_COMMONS_SO=lib$(LIB_COMMONS_NAME).so
LIB_COMMONS_SO_NEW=lib$(LIB_COMMONS_NAME).$(LIB_COMMONS_MINOR_VER).so


# fichier communs
LIB_COMMONS_SRC = \
  AIStrings_basics.c \
  BatchHelper.c \
  Counts_basics.c \
  Erreur.c \
  File32.c \
  Hmap.c \
  inout.c \
  LRUBuf.c \
  ABS.c \
  ABS_None.c \
  Mem.c \
  MallocCtl.c \
  Message.c \
  Pmap_basics.c \
  Timer_basics.c \
  Utile.c

# exclu car present dans dpacommun
#	exploit.c \

.PRECIOUS : Cnxora.c

LIB_COMMONS_SRC_PC = \
	Cnxora.pc

LIB_COMMONS_INC = \
  AIStrings_basics.h \
  AIStrings_struct.h \
  BatchHelper.h \
  Cnxora.h \
  Counts.h \
  Counts_defines.h \
  Counts_struct.h \
  Counts_basics.h \
  Date_formats.h \
  Erreur.h \
  File32.h \
  Hmap.h \
  inout.h \
  LRUBuf.h \
  ABS.h \
  Mem.h \
  MallocCtl.h \
  Message.h \
  Pmap_const.h \
  Pmap_struct.h \
  Pmap_basics.h \
  Timer.h \
  Timer_basics.h \
  Timer_struct.h \
  Utile.h



LIB_COMMONS_OBJ = $(LIB_COMMONS_SRC:.c=.o) $(LIB_COMMONS_SRC_PC:.pc=.o)

# construction de la librairie dynamique
# le -g n'est pas utile
# le -m32 est indispensable pour faire une lib d'objets compil�e en 32 bits
# il est conseill� de recompiler avec -fPIC
lib: $(LIB_COMMONS_SO)

$(LIB_COMMONS_SO): makefile $(LIB_COMMONS_INC) $(LIB_COMMONS_OBJ)
	@echo "***************************************"
	@echo "  => Creation version " $(LIB_COMMONS_VER)
	@echo "***************************************"
	$(MAKE) clean; $(MAKE) OPTIONCC="-fPIC $(OPTIONDEBUG) $(OPTWARN)" $(LIB_COMMONS_OBJ)
	cc -m32 -shared -Wall $(LIB_COMMONS_OBJ) -o $(LIB_COMMONS_SO)

#	cc -m32 -shared -Wl,-soname,$(LIB_COMMONS_SO) -Wall $(LIB_COMMONS_OBJ) -o $(LIB_COMMONS_SO)
#	cc -m32 -shared -Wl,-soname,$(LIB_COMMONS_SO_NEW) -Wall $(LIB_COMMONS_OBJ) -o $(LIB_COMMONS_SO)

libinstall:
	@echo "***************************************"
	@echo "  => Installation version " $(LIB_COMMONS_VER)
	@echo "***************************************"
	# le so applicatif et ses liens symboliques
	# si on change le nom de la lib gr�ce � son num�ro de version
	#cp -pf ./$(LIB_COMMONS_SO) ./$(LIB_COMMONS_SO_NEW)
	tar cvfz $(LIB_COMMONS_NAME).tar.gz ./$(LIB_COMMONS_SO)
	tar xvfz $(LIB_COMMONS_NAME).tar.gz -C $(LIB_COMMONS_DIR);
	# si on ne change par le nom de la lib avec son num�ro de version
	#tar cvfz $(LIB_COMMONS_NAME).tar.gz ./$(LIB_COMMONS_SO)
	#tar xvfz $(LIB_COMMONS_NAME).tar.gz -C $(LIB_COMMONS_DIR);
	rm $(LIB_COMMONS_NAME).tar.gz
	# les includes
	if [ ! -d $(LIB_COMMONS_INCDIR_NEW) ] ; then mkdir $(LIB_COMMONS_INCDIR_NEW) ; fi;
	-rm $(LIB_COMMONS_INCDIR_NEW)/*
	tar cvfz $(LIB_COMMONS_SO_NEW:.so=)_inc.tar.gz $(LIB_COMMONS_INC)
	tar xvfz $(LIB_COMMONS_SO_NEW:.so=)_inc.tar.gz -C $(LIB_COMMONS_INCDIR_NEW);
	rm $(LIB_COMMONS_SO_NEW:.so=)_inc.tar.gz

libln:
	@echo "***************************************"
	@echo "  => lien symbolique version " $(LIB_COMMONS_VER)
	@echo "***************************************"
	if [ -h $(LIB_COMMONS_INCDIR) ] ; then rm $(LIB_COMMONS_INCDIR) ; fi;
	ln -fs $(LIB_COMMONS_INCDIR_NEW) $(LIB_COMMONS_INCDIR)

#
# fin LIB_COMMONS
##################################################################

##################################################################
# re-creation du code C effac� systematiquement lors de la compilation !
codec :
	$(CPP) Cnxora.pc          > Cnxora.i
	proc   iname=Cnxora.i       oname=Cnxora.c      $(PLSPCCFLAGS)
##################################################################


clean:
	rm -f *~ *.o *.so *.i *.lis $(LIB_COMMONS_SRC_PC:.pc=.c)


# recompilation avec optimisation
install:
	$(MAKE) clean; $(MAKE) OPTIONCC=-O lib



##################################################################

TEST_UNIT_ABS=test_ABS
TEST_UNIT_LRUBUF=test_LRUBuf
TEST_UNIT_ERREUR=test_Erreur
PRG_TEST_UNIT=$(TEST_UNIT_ABS) $(TEST_UNIT_LRUBUF) $(TEST_UNIT_ERREUR)
LIBRARY = \
 $(ORTLIBORA)/ortproc.a \
 $(ORTLIBORA)/ortutil.a \
 $(ORTOBJORA)/verifcode.o \
 $(ORTOBJ)/orstring.o \
 -lexploit \
 -lrt

OBJS_TEST_UNIT=test_unit.o

# test du module ABS
OBJS_TEST_ABS = \
  $(OBJS_TEST_UNIT) \
  $(LIB_COMMONS_OBJ) \
  $(TEST_UNIT_ABS).o

# test du module LRUBuf
OBJS_TEST_LRUBUF = \
  $(OBJS_TEST_UNIT) \
  $(LIB_COMMONS_OBJ) \
  $(TEST_UNIT_LRUBUF).o


# test du module Erreur
OBJS_TEST_ERREUR = \
  $(OBJS_TEST_UNIT) \
  $(LIB_COMMONS_OBJ) \
  $(TEST_UNIT_ERREUR).o


tu: $(PRG_TEST_UNIT)

# creation du .o lanceur de test
test_unit.o: $(HOME)/projets/bfrance/c/test_unit/test_unit.h $(HOME)/projets/bfrance/c/test_unit/test_unit.c
	@echo "compilation test_unit.c ..."
	$(CC) $(CCOOPT) $(OPTIONDBG) $(OPTPROF) $(CCOOPTELF) -c $(HOME)/projets/bfrance/c/test_unit/test_unit.c


# Compilation des tests unitaires

$(TEST_UNIT_ABS): $(OBJS_TEST_ABS) makefile $(LIB_COMMONS_INC) $(LIB_COMMONS_OBJ)
	@echo "Edition de lien en cours $@..."
	$(CC) $(OPTIONDBG) $(OPTPROF) -L$(ORACLE_HOME)/lib -L$(PROJET)/lib $(CCOOPTELF) -o $@ $(OBJS_TEST_ABS) $(LIBRARY) $(PROLDLIBS)

$(TEST_UNIT_LRUBUF): $(OBJS_TEST_LRUBUF) makefile $(LIB_COMMONS_INC) $(LIB_COMMONS_OBJ)
	@echo "Edition de lien en cours $@..."
	$(CC) $(OPTIONDBG) $(OPTPROF) -L$(ORACLE_HOME)/lib -L$(PROJET)/lib $(CCOOPTELF) -o $@ $(OBJS_TEST_LRUBUF) $(LIBRARY) $(PROLDLIBS)

$(TEST_UNIT_ERREUR): $(OBJS_TEST_ERREUR) makefile $(LIB_COMMONS_INC) $(LIB_COMMONS_OBJ)
	@echo "Edition de lien en cours $@..."
	$(CC) $(OPTIONDBG) $(OPTPROF) -L$(ORACLE_HOME)/lib -L$(PROJET)/lib $(CCOOPTELF) -o $@ $(OBJS_TEST_ERREUR) $(LIBRARY) $(PROLDLIBS)




##################################################################


#-----------------------------------------------------------------------------
# FIN DU MAKEFILE
#-----------------------------------------------------------------------------
