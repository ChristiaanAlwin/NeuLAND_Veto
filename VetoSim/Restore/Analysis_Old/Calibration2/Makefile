CPPFLAGS:=
CFLAGS:=$(shell root-config --cflags)
LDFLAGS:=
LIBS:=$(shell root-config --libs) $(shell pkg-config freetype2 --libs)

######################################################################

MACRO:=SetSim

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)

MACRO:=CalibrateCuts

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o *_C.*