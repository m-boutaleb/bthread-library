# SU WINDOWS NON MI FUNZIONA PERCHE' HO SPAZI NEL MIO PERCORSO
# DIRS
ODIR=./
LDIR=./
IDIR=./
SRCDIR=./
LIBS=-lm
DISTDIR=./
CFLAGS=-I$(IDIR) -fno-stack-protector
# COMMANDS
CC=cc
AR=ar rcs
RM=rm -f
SLN=ln -s
TARGZ_CREATE=tar zcf
# VARS
MAJOR=1
MINOR=0
PATCH=0
VERSION=$(MAJOR).$(MINOR).$(PATCH)
BASE_NAME=ex0_$(VERSION)
LIB_NAME=lib$(BASE_NAME)
SHARED_LIB=$(LIB_NAME).so
STATIC_LIB=$(LIB_NAME).a
CLEAN_LS=`ls -I Makefile -I cmake-build-debug -I "*.h" -I "*.c"`
CLIENT=$(BASE_NAME)
DYNAMIC_OBJ=dynamic_$(CLIENT).o
DYNAMIC_CLIENT=dynamic_$(CLIENT)
DYNAMIC_TARGZ=dynamic_$(BASE_NAME).tar.gz
STATIC_OBJ=static_$(CLIENT).o
STATIC_CLIENT=static_$(CLIENT)
STATIC_TARGZ=static_$(BASE_NAME).tar.gz
MAINFILE=./test/scheduler/scheduler_test

_DEPS=tqueue.h bthread.h bthread_private.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ=tqueue.o bthread.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

# ######################################################################################################################
# DYNAMIC
# ######################################################################################################################
run_dynamic: $(DYNAMIC_CLIENT)
	LD_LIBRARY_PATH=. ./$^
dist_dynamic_targz: $(DYNAMIC_CLIENT)
	$(TARGZ_CREATE) $(DYNAMIC_TARGZ) $(DYNAMIC_CLIENT) $(SHARED_LIB)
$(DYNAMIC_CLIENT): $(SHARED_LIB)
	$(CC) -o $(DISTDIR)/$@ $(MAINFILE).c -L$(LDIR) -l:$< $(CFLAGS)
$(SHARED_LIB): $(OBJ)
	$(CC) -shared -o $(LDIR)/$@ $^
# ######################################################################################################################
# STATIC
# ######################################################################################################################
run_static: $(STATIC_CLIENT)
	./$(DISTDIR)/$^
dist_static_targz: $(STATIC_CLIENT)
	$(TARGZ_CREATE) $(STATIC_TARGZ) $(STATIC_CLIENT)
$(STATIC_CLIENT): $(STATIC_LIB)
	$(CC) -w -o $(DISTDIR)/$@ $(MAINFILE).c -L$(LDIR) -l:$< $(CFLAGS)
$(STATIC_LIB): $(OBJ)
	$(AR) -o $(LDIR)/$@ $^
# ######################################################################################################################
# COMMON
# ######################################################################################################################
$(ODIR)/%.o:$(SRCDIR)/%.c $(DEPS)
	$(CC) -w -c -o $@ $< $(CFLAGS)
clean:
	rm -f $(DISTDIR)/*$(VERSION) $(LDIR)/*.a $(DISTDIR)/*.tar.gz $(LDIR)/lib* $(ODIR)/* *~ core $(INCDIR)/*~
	@echo Clean performed
fast_clean:
	rm -f $(CLEAN_LS)
	
