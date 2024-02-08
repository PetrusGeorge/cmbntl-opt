N_BITS = $(shell getconf LONG_BIT)
ifeq ($(N_BITS),32)
   BITS_OPTION = -m32
else
   BITS_OPTION = -m64
endif

SRCDIR := src
OBJDIR := obj

CPPC := g++
CPPCFLAGS := $(BITS_OPTION) -fPIC -fexceptions -DIL_STD -std=c++0x -MMD
CPPCLNFLAGS := -lm

ifeq ($(DEBUG), 1)
	CPPCFLAGS += -DDEBUG -g
else
	CPPCFLAGS += -O3 -DNDEBUG
endif

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
TARGET := mlp

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
	@mkdir -p $(OBJDIR) $(SRCDIR)

#this recreates the src folder structure into the obj folder
#can be replaced with a rule that also creates the src structure and repeat on obj
	@mkdir -p $(patsubst $(SRCDIR)%,$(OBJDIR)%,$(shell find $(SRCDIR) -type d))
