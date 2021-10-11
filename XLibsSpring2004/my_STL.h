////////////////////////////////////////////////////////////////////////////////
//
// Additional include for making STL more convenient
//
// Author: Alexandre Kotliar, K-D Lab
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __STL_ADDITION_H__
#define __STL_ADDITION_H__

// For disabling some warning 
#pragma warning( disable : 4786 4284 4800)
#pragma warning( disable : 4244 4018)

#ifdef __ICL
#pragma warning( disable : 880 1125)
#endif

#ifndef FOR_EACH
#define FOR_EACH(list, iterator) \
  for(iterator = (list).begin(); iterator != (list).end(); ++iterator)
#endif

//using namespace std;
#if !defined (_STLP_NO_NEW_IOSTREAMS)
#define   _STLP_NO_NEW_IOSTREAMS	1
#endif

#if !defined (_STLP_NO_NAMESPACES)
#define   _STLP_NO_NAMESPACES 1
#endif

#endif // __STL_ADDITION_H__