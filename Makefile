
## Makefile for the vfs project as part of course db-101 data management
## Author : arunk - MT2009054

## Following are the Global Variables

# Uncomment this in case if your target ic strict 'c' compiler
# OPTIONS = -ansi -pedantic-errors
CC 	:= gcc -c
CC_LINK := gcc
SRC	:= src/
OBJ := obj/
GUI := gui/
TESTS := test-cases/
GLOBALINCLUDES 	:= $(SRC)globals.h $(SRC)vfs_error_codes.h
GLOBALCOMMANDS := mkdir -p $(OBJ)


# Sample compile comand: $(CC) $(CFLAGS) $(INCLUDES) -c $(input) -o $(output)

#The default target is all
# Other targets users can use are ll ht etc
all: ds vfs-integ

##A separate target to only build the data structures
ds : ll ht bst nary
##Just an useful target which will list all the available targets
list: 
	@echo ""
	@echo "exe : build all the following executables."
	@echo "exe-p1 : build the executable of phase-1"
	@echo "exe-bst : build the executable for bst"
	@echo "exe-nary : build the executable for nary"
	@echo ""
	@echo "all : compile all the following APIs."
	@echo "ll : linked list"
	@echo "ht : Hash table"
	@echo "bst : Binary Search Tree"
	@echo "nary : Nary tree"
	@echo ""
	@echo "gui : All with GTK GUI"


#Target 'exe' will create the executable, we can make this default if needed.
PHASE_1_EXE := vfs-phase1-exe
BST_EXE := vfs-bst-exe
NARY_EXE := vfs-nary-exe
VFS_INTEG_EXE := vfs-integ-exe

exe: all $(PHASE_1_EXE) $(BST_EXE) $(NARY_EXE) $(VFS_INTEG_EXE)
exe-p1 : ll ht $(PHASE_1_EXE)
exe-bst : bst $(BST_EXE)
exe-nary : nary $(NARY_EXE)
exe-vfs-integ : vfs-integ $(VFS_INTEG_EXE)



########## Following are rules for linked list ###############
LL_FILE := llist
LL_SRC := $(SRC)$(LL_FILE).c
LL_HEAD := $(SRC)$(LL_FILE).h

LL_OBJ:=$(OBJ)$(LL_FILE).o

#create a var same as target just for readibility and to avoid retyping, in case multiple objs
#just for future use, in case.
LL := $(LL_OBJ)

# LL is the target that should be invoked by other targets or by user
ll: $(LL)


$(LL_OBJ): $(LL_SRC) $(LL_HEAD)
	$(GLOBALCOMMANDS)
	$(CC) $(LL_SRC) -o $(LL_OBJ)
####### End of Linked list ##############


########## Following are rules for Hash Tables ###############
HT_FILE := hash
HT_SRC := $(SRC)$(HT_FILE).c
HT_HEAD := $(SRC)$(HT_FILE).h

HT_OBJ:=$(OBJ)$(HT_FILE).o

#create a var same as target just for readibility and to avoid retyping, in case multiple objs
#just for future use, in case.
HT := $(HT_OBJ)

# ht is the target that should be invoked by other targets or by user
ht: $(HT)

#It has dependency on linked list as well, but need to rebuild only if the signature changes
$(HT_OBJ): $(HT_SRC) $(HT_HEAD) $(LL_HEAD)
	$(GLOBALCOMMANDS)
	$(CC) $(HT_SRC) -o $(HT_OBJ)

####### End of Hash Table ##############


########## Following are rules for BST ###############
BST_FILE := VFS_BST
BST_SRC := $(SRC)$(BST_FILE).c
BST_HEAD := $(SRC)$(BST_FILE).h

BST_OBJ:=$(OBJ)$(BST_FILE).o

BST := $(BST_OBJ)

# BST is the target that should be invoked by other targets or by user
bst: $(BST)

$(BST_OBJ): $(BST_SRC) $(BST_HEAD)
	$(GLOBALCOMMANDS)
	$(CC) $(BST_SRC) -o $(BST_OBJ)

####### End of BST ##############

########## Following are rules for NARY ###############
NARY_FILE := narytree
NARY_SRC := $(SRC)$(NARY_FILE).c
NARY_HEAD := $(SRC)$(NARY_FILE).h

NARY_OBJ:=$(OBJ)$(NARY_FILE).o

#Nary requires string processing file
STR_FILE := string_tok
STR_SRC := $(SRC)$(STR_FILE).c
STR_HEAD := $(SRC)$(STR_FILE).h

STR_OBJ:=$(OBJ)$(STR_FILE).o

NARY := $(NARY_OBJ) $(STR_OBJ)

# BST is the target that should be invoked by other targets or by user
nary: $(NARY)

#It has dependency on str process as well, but need to rebuild only if the signature changes
$(NARY_OBJ): $(NARY_SRC) $(NARY_HEAD) $(STR_HEAD)
	$(GLOBALCOMMANDS)
	$(CC) $(NARY_SRC) -o $(NARY_OBJ)


$(STR_OBJ):  $(STR_SRC) $(STR_HEAD)
	$(CC) $(STR_SRC) -o $(STR_OBJ)

####### End of NARY ##############

########## Following are rules for vfs-integ ###############
VFS_INTEG_FILE := vfs
VFS_INTEG_SRC := $(SRC)$(VFS_INTEG_FILE).c
VFS_INTEG_HEAD := $(SRC)$(VFS_INTEG_FILE).h

VFS_INTEG_OBJ:=$(OBJ)$(VFS_INTEG_FILE).o

VFS-INTEG := $(VFS_INTEG_OBJ)

# vfs-integ is the target that should be invoked by other targets or by user
vfs-integ: $(VFS-INTEG)

$(VFS_INTEG_OBJ): $(VFS_INTEG_SRC) $(VFS_INTEG_HEAD)
	$(GLOBALCOMMANDS)
	$(CC) $(VFS_INTEG_SRC) -o $(VFS_INTEG_OBJ)

####### End of vfs-integ ##############

###### Following are rules for executables ##########

#Declare all the depedencies of executable for phase-1
DEPS_P1 := $(TESTS)vfs_test_phase1.c $(HT) $(LL)
$(PHASE_1_EXE): $(DEPS_P1)
	$(CC_LINK) $(DEPS_P1) -o $(PHASE_1_EXE)


#Declare all the depedencies of executable for bst
DEPS_BST := $(TESTS)vfs_test_bst.c $(BST)
$(BST_EXE): $(DEPS_BST)
	$(CC_LINK) $(DEPS_BST) -o $(BST_EXE)

#Declare all the depedencies of executable for nary, it also depends on str_process
DEPS_NARY := $(TESTS)vfs_test_nary.c $(NARY)
$(NARY_EXE): $(DEPS_NARY)
	$(CC_LINK) $(DEPS_NARY) -o $(NARY_EXE)

#Declare all the depedencies of executable for vfs-integ
DEPS_VFS_INTEG := $(TESTS)vfs_test_integ.c $(VFS-INTEG) $(LL) $(HT) $(BST) $(NARY)
$(VFS_INTEG_EXE): $(DEPS_VFS_INTEG)
	$(CC_LINK) $(DEPS_VFS_INTEG) -o $(VFS_INTEG_EXE)


############ End of executables ###############


######### Following are for GUI using gtk #############

GUI_EXE := launch_vfs_gui
GTK_FLAGS := `pkg-config --cflags --libs gtk+-2.0`

GUI_SRC := $(GUI)main.c $(GUI)menu.c
GUI_HEAD := $(GUI)menu.h

GUI_INCL := -I$(SRC)

gui : all $(GUI_EXE)

DEPS_ALL := $(VFS-INTEG) $(LL) $(HT) $(BST) $(NARY)

$(GUI_EXE): $(GUI_SRC) $(GUI_HEAD) $(DEPS_ALL)
	$(GLOBALCOMMANDS)
	$(CC_LINK) $(GUI_SRC) $(DEPS_ALL) $(GTK_FLAGS) $(GUI_INCL) -o $(GUI_EXE)
	chmod a+x $(GUI_EXE)


###### Define the global dependencies for multiple files ie many is to many deps mapping
#### This means that if you modify eg: globals.h then you need to compile all in LHS
$(LL) $(HT) $(BST) $(NARY_OBJ) $(VFS-INTEG): $(GLOBALINCLUDES)

######## Define the clean rule ################
clean:
	rm -rf $(OBJ)
	rm -rf $(PHASE_1_EXE)
	rm -rf $(BST_EXE)
	rm -rf $(NARY_EXE)
	rm -rf $(VFS_INTEG_EXE)

########### End of clean ##########
