PROGRAM       = JCIaaGF

version       = JTKT
CXX           = g++
CXXFLAGS      = -g -Wall   -Wno-deprecated -D$(version) 
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
#############################################
# -bind_at_load helps to remove linker error
############################################
CXXFLAGS += $(shell root-config --cflags)
LDFLAGS  = $(shell root-config --libs)
LDFLAGS += -L$(PYTHIA8)/lib -lpythia8
INCS    += -I$(PYTHIA8)/include

CXXFLAGS  += $(INCS)

HDRSDICT = src/JCORRAN/Base/AliJBaseTrack.h 
           
HDRS	+= $(HDRSDICT)	src/JCORRAN/Base/AliJRunTable.h \
			src/JCORRAN/Base/AliJCard.h \
			src/JCORRAN/Base/AliJBaseCard.h \
			src/JCORRAN/Base/AliJTrackCut.h \
			src/JCORRAN/Base/AliJEfficiency.h \
			src/JCORRAN/Base/AliJHistManager.h \
			src/JCORRAN/Base/AliJHistogramInterface.h \
			src/JCORRAN/Base/AliJEventPool.h \
			src/JCORRAN/Base/AliJAcceptanceFunctions.h \
			src/JCORRAN/Base/AliJAcceptanceCorrection.h \
			src/JCORRAN/Base/AliJCorrelationInterface.h \
			src/JCORRAN/Base/iaaAnalysis/AliJIaaHistograms.h \
			src/JCORRAN/Base/iaaAnalysis/AliJIaaCorrelations.h \
			src/JCORRAN/Base/iaaAnalysis/AliJIaaAna.h \
			src/psrc/JCard.h \
			src/psrc/JHistos.h \
			src/psrc/JParticleTools.h \
                        nanoDict.h


SRCS = $(HDRS:.h=.cxx)
OBJS = $(HDRS:.h=.o)

all:            $(PROGRAM)

$(PROGRAM):     $(OBJS) src/JCORRAN/Base/AliJConst.h $(PROGRAM).C
		@echo "Linking $(PROGRAM) ..."
		$(CXX) -lEG -lPhysics -L$(PWD) $(PROGRAM).C $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(PROGRAM)
		@echo "done"

%.cxx:

%: %.cxx
#  commands to execute (built-in):
	$(LINK.cc) $^ $(CXXFLAGS) $(LOADLIBES) $(LDLIBS) -o $@

%.o: %.cxx %.h
#  commands to execute (built-in):
	$(COMPILE.cc) $(OUTPUT_OPTION) $<


clean:
		rm -f $(OBJS) core *Dict* $(PROGRAM).o *.d $(PROGRAM) $(PROGRAM).sl

cl:  clean $(PROGRAM)

nanoDict.cc: $(HDRSDICT)
		@echo "Generating dictionary ..."
		@rm -f nanoDict.cc nanoDict.hh nanoDict.h
		@rootcint nanoDict.cc -c -D$(version) $(HDRSDICT)
