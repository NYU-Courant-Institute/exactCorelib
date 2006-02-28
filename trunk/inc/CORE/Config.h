#ifndef __CORE_CONFIG_H__
#define __CORE_CONFIG_H__

// define version number
#define CORE_VERSION            2
#define CORE_VERSION_MINOR      0
#define CORE_VERSION_PATCHLEVEL 0

// macros for defining namespace
#define CORE_BEGIN_NAMESPACE    namespace CORE {
#define CORE_END_NAMESPACE      };
#define CORE_NS                 CORE

// diable old names
//#define CORE_DISABLE_OLDNAMES

// disable reference counting
//#define CORE_DISABLE_REFCOUNTING

// disable memory pool
//#define CORE_DISABLE_MEMPOOL

// disable debug
//#define CORE_DISABLE_DEBUG

// debug filter
//#define CORE_DEBUG_FILTER 1

// debug root bound
//#define CORE_DEBUG_ROOTBOUND 1

// disable automatic namespace
//#define CORE_NO_AUTOMATIC_NAMESPACE

#endif /*__CORE_CONFIG_H__*/
