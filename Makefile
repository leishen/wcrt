##
## WCRT  -  Win32API CRT
##
## Visual C/C++ makefile (GNU Make)
##
## Copyright (c) 2003-2004 by Joergen Ibsen / Jibz
## All Rights Reserved
##
## http://www.ibsensoftware.com/
##


##
## Makefile options:
##
##   release=1  -  build a release version (optimized, no debug info)
##


## --- options ----------------------------------------------------

ifndef release
  release = 0
endif


## --- paths ------------------------------------------------------

ifeq ($(release), 1)
  dstdir = lib/Release
else
  dstdir = lib/Debug
endif

srcdir = src


## --- files ------------------------------------------------------

target   = $(dstdir)/wcrt.lib

cobjs   := $(patsubst $(srcdir)/%.c,$(dstdir)/%.obj,$(wildcard $(srcdir)/*.c))
cobjs   += $(patsubst $(srcdir)/%.asm,$(dstdir)/%.obj,$(wildcard $(srcdir)/*.asm))
cxxobjs := $(patsubst $(srcdir)/%.cpp,$(dstdir)/%.obj,$(wildcard $(srcdir)/*.cpp))

objects  = $(cobjs) $(cxxobjs)

temps    =

## --- flags ------------------------------------------------------

cflags  = /nologo /W3 /WX /Zl /I$(srcdir) /Iinclude
mlflags = /nologo /W3 /WX /coff /I$(srcdir)

ifeq ($(release), 1)
  cflags += /O1 /DNDEBUG
else
  cflags += /Od /Zi
  mlflags += /Zi
endif

cxxflags = $(cflags) /GX

# --- rules -------------------------------------------------------

.PHONY: all clean

all: $(target)

$(target): $(objects)
	$(RM) $@
	lib /out:$@ $^

$(dstdir)/%.obj : $(srcdir)/%.asm
	ml $(mlflags) /Fo$@ /c $<

$(dstdir)/%.obj : $(srcdir)/%.c
	cl $(cflags) /Fo$@ -c $<

$(dstdir)/%.obj : $(srcdir)/%.cpp
	cl $(cxxflags) /Fo$@ -c $<

clean:
	$(RM) $(objects) $(target) $(temps)


## --- dependencies -----------------------------------------------

