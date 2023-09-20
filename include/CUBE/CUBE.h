// CUBE - Header only C/C++/C# build system.
// 
// License at end of file.

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
#include "CUBE/CUBEIO.h"
#include "CUBE/CUBEPath.h"
#include "CUBE/CUBEStackString.h"
#include "CUBE/CUBEString.h"

#endif

// MIT License
// 
// Copyright (c) 2023 River Govers
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
