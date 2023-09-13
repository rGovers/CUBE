#ifndef INCLUDED_HEADER_CUBE
#define INCLUDED_HEADER_CUBE

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
typedef unsigned char CBUINT8;
typedef unsigned short CBUINT16;
typedef unsigned int CBUINT32;
typedef unsigned long long CBUINT64;
#else
typedef unsigned char CBUINT8;
typedef unsigned short CBUINT16;
typedef unsigned int CBUINT32;
typedef unsigned long long CBUINT64;
#endif

#ifndef CBNULL
#define CBNULL 0
#endif

#ifndef CBTRUE
#define CBTRUE 1
#endif

#ifndef CBFALSE
#define CBFALSE 0
#endif

#ifndef CBBOOL
#define CBBOOL CBUINT8
#endif

#ifdef __cplusplus
}
#endif

#include "CUBE/CUBECommandLine.h"
#include "CUBE/CUBECProject.h"
#include "CUBE/CUBECSProject.h"
#include "CUBE/CUBEStackString.h"
#include "CUBE/CUBEString.h"

#endif