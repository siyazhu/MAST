CXX       ?= g++
INCDIRS    = -I$(HOME)/atat/src      #path of ATAT/src
CXXFLAGS  ?= -O3 -std=c++17 -Wall -Wextra -Wno-unused-parameter -MMD -MP $(INCDIRS) -Wno-sign-compare -Wno-unused-variable
LDFLAGS   ?=
LDLIBS    ?=

SRCDIR    ?= src
OBJDIR    ?= build
BINDIR    ?= bin
PREFIX    ?= $(HOME)
DESTDIR   ?=

PROGS     := str2lmp str2vasp lmp2str lmp2vasp vasp2lmp vasp2str randstr rmc shuffle

COMMON_OBJS_NAMES := constants element structure
RMC_EXTRA_NAMES   := pdfs angles smooth

objs = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(1)))

EXTERNAL_OBJS := $(HOME)/atat/src/getvalue.o

ifeq ($(wildcard $(EXTERNAL_OBJS)),)
$(error Missing external object: $(EXTERNAL_OBJS))
endif

STR2LMP_OBJS   := $(EXTERNAL_OBJS) $(call objs,$(COMMON_OBJS_NAMES) str2lmp)
STR2VASP_OBJS  := $(EXTERNAL_OBJS) $(call objs,$(COMMON_OBJS_NAMES) str2vasp)
LMP2STR_OBJS   := $(EXTERNAL_OBJS) $(call objs,$(COMMON_OBJS_NAMES) lmp2str)
LMP2VASP_OBJS  := $(EXTERNAL_OBJS) $(call objs,$(COMMON_OBJS_NAMES) lmp2vasp)
VASP2LMP_OBJS  := $(EXTERNAL_OBJS) $(call objs,$(COMMON_OBJS_NAMES) vasp2lmp)
VASP2STR_OBJS  := $(EXTERNAL_OBJS) $(call objs,$(COMMON_OBJS_NAMES) vasp2str)
RANDSTR_OBJS   := $(EXTERNAL_OBJS) $(call objs,$(COMMON_OBJS_NAMES) randstr)
RMC_OBJS       := $(EXTERNAL_OBJS) $(call objs,$(COMMON_OBJS_NAMES) $(RMC_EXTRA_NAMES) rmc)
SHUFFLE_OBJS   := $(EXTERNAL_OBJS) $(call objs,$(COMMON_OBJS_NAMES) shuffle)

ALL_OBJS := \
  $(filter-out $(EXTERNAL_OBJS), \
    $(STR2LMP_OBJS) $(STR2VASP_OBJS) $(LMP2STR_OBJS) $(LMP2VASP_OBJS) \
    $(VASP2LMP_OBJS) $(VASP2STR_OBJS) $(RANDSTR_OBJS) $(RMC_OBJS) $(SHUFFLE_OBJS))
ALL_DEPS := $(ALL_OBJS:.o=.d)

.PHONY: all
all: dirs $(addprefix $(BINDIR)/,$(PROGS))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR)/str2lmp:   $(STR2LMP_OBJS)   | dirs
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR)/str2vasp:  $(STR2VASP_OBJS)  | dirs
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR)/lmp2str:   $(LMP2STR_OBJS)   | dirs
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR)/lmp2vasp:  $(LMP2VASP_OBJS)  | dirs
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR)/vasp2lmp:  $(VASP2LMP_OBJS)  | dirs
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR)/vasp2str:  $(VASP2STR_OBJS)  | dirs
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR)/randstr:   $(RANDSTR_OBJS)   | dirs
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR)/rmc:       $(RMC_OBJS)       | dirs
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR)/shuffle:   $(SHUFFLE_OBJS)   | dirs
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

.PHONY: dirs clean distclean install uninstall debug release
dirs:
	@mkdir -p $(OBJDIR) $(BINDIR)

clean:
	$(RM) -r $(OBJDIR)

distclean: clean
	$(RM) -r $(BINDIR)

debug:
	$(MAKE) CXXFLAGS='-O0 -g -std=c++17 -Wall -Wextra -MMD -MP $(INCDIRS)' all

release:
	$(MAKE) CXXFLAGS='-O3 -march=native -std=c++17 -Wall -Wextra -MMD -MP $(INCDIRS)' all

install: all
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 0755 $(addprefix $(BINDIR)/,$(PROGS)) $(DESTDIR)$(PREFIX)/bin

uninstall:
	$(RM) $(addprefix $(DESTDIR)$(PREFIX)/bin/,$(PROGS))

-include $(ALL_DEPS)
