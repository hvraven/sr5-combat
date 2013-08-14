# Directories
BUILDDIR       = build
SRCDIR         = src

# Libraries
PKGCONFIG_LIBRARIES = ncursesw menuw
PKGCONFIG_CPPFLAGS  = $(shell pkg-config --cflags $(PKGCONFIG_LIBRARIES))
PKGCONFIG_LDFLAGS   = $(shell pkg-config --libs $(PKGCONFIG_LIBRARIES))

# Compiler Options
CXXFLAGS       = -Wall -Wextra -ggdb
CPPFLAGS       = -std=c++11 -I$(SRCDIR) $(PKGCONFIG_CPPFLAGS)
LDFLAGS        = $(PKGCONFIG_LDFLAGS)

df             = $(BUILDDIR)/$(*F)
MAKEDEPEND     = $(CXX) -MM $(CPPFLAGS) $<

# Source files
SRCS           = $(shell egrep -L '^(int )?main' $(SRCDIR)/*.cpp | sort)
OBJS           = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o, $(SRCS))

# Target source files
TARGETSRCS     = $(shell egrep -l '^(int )?main' $(SRCDIR)/*.cpp | sort)
TARGETS        = $(patsubst $(SRCDIR)/%.cpp, %, $(TARGETSRCS))
BUILD_TARGETS  = $(patsubst %,$(BUILDDIR)/%, $(TARGETS))

.DEFAULT_GOAL: all
all: $(TARGETS)

$(TARGETS): %: $(BUILDDIR)/%
	cp $^ $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(BUILDDIR)/.ghost
	@$(MAKEDEPEND) |  sed -e 's:^$(@F):$@:' > $(@:.o=.P)
	$(COMPILE.cpp) -o $@ $<

$(BUILDDIR)/.ghost:
	mkdir -p $(BUILDDIR)
	touch $(BUILDDIR)/.ghost

$(BUILD_TARGETS): % : %.o $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.P: %.o

.PHONY: clean distclean
clean:
	-find $(BUILDDIR) -maxdepth 1 -iname '*.o' -delete -or -iname '*.a' -delete \
		-or -type f -executable -delete

distclean:
	rm -r $(BUILDDIR)
	rm -f $(TARGETS)

-include $(SRCS:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.P)
-include $(TARGETS:%=$(BUILDDIR)/%.P)
