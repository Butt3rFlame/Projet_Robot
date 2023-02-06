#
# Projet Robot (MRPiZ C) - Makefile principal.
# 
# @author Matthias Brun (matthias.brun@eseo.fr)
#
# @modified by Matthieu Bonhomme (matthieu.bonhomme@reseau.eseo.fr)
#

# Répertoire d'installation de la librairie MRPiZ
# pour la compilation à destination de la cible MRPiZ (à adapter) :
LIB_MRPIZ = $(realpath ../../lib_mrpiz-v0.3/)

#
# Organisation des sources.
#

export SRCDIR = src
export BINDIR = bin

export SUBDIRS_TELCO = $(SRCDIR)/telco
export SUBDIRS_COMMANDO = $(SRCDIR)/commando

#
# Définitions des binaires à générer.
#
export PROG_TELCO = $(BINDIR)/telco
export PROG_COMMANDO = $(BINDIR)/commando

#
# Définitions des outils.
#

# Compilateur.

export CC = gcc
export CCFLAGS  = -DINTOX -DINTOX_ADDRESS=127.0.0.1 -DINTOX_PORT=12345
export CCFLAGS += -I$(LIB_MRPIZ)/include/mrpiz/
export LDFLAGS  = -L$(LIB_MRPIZ)/lib/ -lintoxmrpiz -lintox

# dans tous les cas.

#sans debuggage : 
#export CCFLAGS += -O3 -DNDEBUG 
# avec debuggage : 
export CCFLAGS += -Og -g -DDEBUG 

export CCFLAGS += -MMD -MP # gestion automatique des dépendances
export CCFLAGS += -D_BSD_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_XOPEN_SOURCE -D_DEFAULT_SOURCE -D_GNU_SOURCE
export CCFLAGS += -std=c99 -Wall -pedantic
export LDFLAGS += -lm -lrt -lpthread

#
# Règles du Makefile.
#

.PHONY: all clean $(SUBDIRS)

# Compilation.
all: $(SUBDIRS_COMMANDO) $(SUBDIRS_TELCO)

# Nettoyage.
clean: $(SUBDIRS_COMMANDO) $(SUBDIRS_TELCO)
	@rm -f $(PROG_COMMANDO) core* $(BINDIR)/core*
	@rm -f $(PROG_TELCO) core* $(BINDIR)/core*

# Sous-répertoires.
$(SUBDIRS_COMMANDO):
	$(MAKE) $(MAKECMDGOALS) -C $@
	
$(SUBDIRS_TELCO):
	$(MAKE) $(MAKECMDGOALS) -C $@

#
# Arrêt du programme par ctrl+c
# (utile pour utilisation dans certaines IDE)
#

.PHONY: kill

kill:
	killall -s INT "$(PROG_TELCO)"
	killall -s INT "$(PROG_COMMANDO)"
