SOURCES = main
LIBS = 
SUBDIRS = 
MKSUBDIRS = $(addprefix $(OBJPATH)/, $(SUBDIRS))

SRCPATH = ./src
BINPATH = ./bin
OBJPATH = ./bin/obj
DBGSUFF = _DBG.o
OPTSUFF = _OPT.o
DBGFLGS = -Wall -g
OPTFLGS = -Ofast
DBGBINP = $(BINPATH)/debug
OPTBINP = $(BINPATH)/optimized

DEPENDS = $(addprefix $(SRCPATH)/, $(addsuffix .c, $(SOURCES)) $(addsuffix .h, $(SOURCES)))
DBGOBJ = $(addprefix $(OBJPATH)/, $(addsuffix $(DBGSUFF), $(SOURCES)))
OPTOBJ = $(addprefix $(OBJPATH)/, $(addsuffix $(OPTSUFF), $(SOURCES)))

.DEFAULT_GOAL := debug
.PHONY: debug, run, clean, debug-compile, optimized-compile

$(OBJPATH)/%$(DBGSUFF): $(SRCPATH)/%.c $(SRCPATH)/%.h
	gcc $(DBGFLGS) $(LIBS) -c -o $@ $<

$(OBJPATH)/%$(OPTSUFF): $(SRCPATH)/%.c $(SRCPATH)/%.h
	gcc $(OPTFLGS) $(LIBS) -c -o $@ $<

debug-compile: $(DBGOBJ)
	gcc $(DBGFLGS) $(LIBS) -o $(DBGBINP) $^

optimized-compile: $(OPTOBJ)
	gcc $(OPTFLGS) $(LIBS) -o $(OPTBINP) $^

debug: debug-compile
	$(DBGBINP)

run: optimized-compile
	$(OPTBINP)

clean:
	rm -r ./bin
	mkdir $(BINPATH) $(OBJPATH) $(MKSUBDIRS)