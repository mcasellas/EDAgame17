# To play with the Dummy, uncomment the line for your platform:
#DUMMY_OBJ = AIDummy.o.Linux64
#DUMMY_OBJ = AIDummy.o.Linux32
#DUMMY_OBJ = AIDummy.o.Win64.Cygwin
#DUMMY_OBJ = AIDummy.o.Win64.MinGW
#DUMMY_OBJ = AIDummy.o.Win32.MinGW
DUMMY_OBJ = AIDummy.o.macOS

# Add here any extra .o player files you want to link to the executable
EXTRA_OBJ =

# Configuration
OPTIMIZE = 2 # Optimization level    (0 to 3)
DEBUG    = 0 # Compile for debugging (0 or 1)
PROFILE  = 0 # Compile for profile   (0 or 1)


# Do not edit past this line
############################

# The following two lines will detect all your players (files matching "AI*.cc")

PLAYERS_SRC = $(wildcard AI*.cc)
PLAYERS_OBJ = $(patsubst %.cc, %.o, $(PLAYERS_SRC)) $(EXTRA_OBJ) $(DUMMY_OBJ)


# Flags

ifeq ($(strip $(PROFILE)),1)
	PROFILEFLAGS=-pg
endif

ifeq ($(strip $(DEBUG)),1)
	DEBUGFLAGS=-g
endif

CXXFLAGS = -std=c++11 -Wall -Wno-unused-variable $(PROFILEFLAGS) $(DEBUGFLAGS) -O$(strip $(OPTIMIZE))
LDFLAGS  = -std=c++11                            $(PROFILEFLAGS) $(DEBUGFLAGS) -O$(strip $(OPTIMIZE))


# Rules

OBJ = Structs.o Settings.o State.o Info.o Random.o Board.o Action.o Player.o Registry.o Utils.o

all: Game

ex: Game
	./Game mcasellas_v5 Dummy Dummy Dummy -s 5 -i default.cnf -o default.out

jo: Game
	./Game mcasellas_v5 BackInBlack_ BackInBlack_ BackInBlack_ -s 1 -i default.cnf -o default.out

rob: Game
	./Game mcasellas_v2 MrRobb MrRobb MrRobb -s 30 -i default.cnf -o default.out


clean:
	rm -rf Game  *.o *.exe *.out Makefile.deps

Game:  $(OBJ) Game.o Main.o $(PLAYERS_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

SecGame: $(OBJ) SecGame.o SecMain.o
	$(CXX) $^ -o $@ $(LDFLAGS) -lrt

%.exe: %.o $(OBJ) SecGame.o SecMain.o
	$(CXX) $^ -o $@ $(LDFLAGS) -lrt

Makefile.deps: *.cc
	$(CXX) $(CXXFLAGS) -MM *.cc > Makefile.deps

include Makefile.deps
