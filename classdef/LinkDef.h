
#ifndef CLASSDEF_LINKDEF_H_
#define CLASSDEF_LINKDEF_H_



#ifdef __CLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class std::vector<float>+;
#pragma link C++ class std::vector<std::vector<float> >+;
#pragma link C++ class std::vector<std::vector<std::vector<float> > >+;
#pragma link C++ class std::vector<std::vector<std::vector<std::vector<float> > > >+;


#endif

#endif
