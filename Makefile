HEADERS   := vector
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := `root-config --cflags --glibs` 
CC_FLAGS := -fPIC -c -g -Wall `root-config --cflags` -O2
CC_FLAGS += -MMD


all: convert


convert.so: $(OBJ_FILES) obj/convertDict.o
	g++ -fPIC -shared $(LD_FLAGS) -o $@ $^

convert: convert.so bin/convert.cpp
	g++ -Wall $(LD_FLAGS) -L. -o $@ $^  

obj/%.o: src/%.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

#for putting delphes classes in vectors
$(HEADERS):

classdef/LinkDef.h:

convertDict.cxx: $(HEADERS) classdef/LinkDef.h
	rootcint -f $@ -c $(CC_FLAGS) -p $^

obj/convertDict.o: convertDict.cxx
	g++ $(CC_FLAGS) -c -o $@ $<

clean: 
	rm -f obj/*.o obj/*.d
	rm -f convert.so
	rm -f convert
	rm -f convertdict_rdict.pcm
	rm -f convertdict.cxx

-include $(OBJ_FILES:.o=.d)