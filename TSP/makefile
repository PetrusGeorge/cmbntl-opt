N_BITS = $(shell getconf LONG_BIT)
ifeq ($(N_BITS),32)
   BITS_OPTION = -m32
else
   BITS_OPTION = -m64
endif

SRCDIR := src
OBJDIR := obj
BINDIR := bin

#Compilation flags
CPPC := g++
CPPCFLAGS := $(BITS_OPTION) -fPIC -fexceptions -DIL_STD -std=c++17 -MMD
CPPCLNFLAGS := -lm -lpthread

SRCS := $(shell find -L $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))


ifeq ($(DEBUG), 1)
	CPPCFLAGS += -DDEBUG -g3
else
	CPPCFLAGS += -O3 -DNDEBUG
endif

ifdef NAME
	TARGET := $(BINDIR)/$(NAME)
else
	TARGET := $(BINDIR)/bin.out
endif

.PHONY: all clean rebuild run mkdirs

all: mkdirs $(TARGET)
	@printf "\nBuild complete: $(TARGET)\n"

$(TARGET): $(OBJS)
	@printf "\nLinking objects to create executable: $@\n"
	$(CPPC) $^ -o $@ $(CPPCLNFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@printf "\nCompiling: $<\n"
	$(CPPC) $(CPPCFLAGS) -c $< -o $@ 

clean:
	@printf "\nCleaning object files...\n"
	rm -f $(TARGET)
	find $(OBJDIR) -type f -name '*.o' -exec rm -f {} +
	find $(OBJDIR) -type f -name '*.d' -exec rm -f {} +
	@printf "\nClean complete.\n"

rebuild: clean all

run: all
	@printf "\nRunning: $(TARGET)\n"
	./$(TARGET)

mkdirs:
	@printf "Making directories\n"
	@mkdir -p $(OBJDIR) $(SRCDIR) $(BINDIR)

#this recreates the src folder structure into the obj folder
#can be replaced with a rule that also creates the src structure and repeat on obj
# @mkdir -p $(patsubst $(SRCDIR)%,$(OBJDIR)%,$(shell find $(SRCDIR) -type d)) #old command dont support symlinks
#new command supports symlinks
	@find -L $(SRCDIR) -type d -not -path "$(SRCDIR)" | sed -e "s|$(SRCDIR)/|$(OBJDIR)/|" | xargs -I {} mkdir -p {}

