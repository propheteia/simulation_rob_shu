/*
 *  Copyright (C) 2011 Justin Stoecker
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef RVDRAW_H
#define RVDRAW_H

#include <string>
#include <cstdio>
#include <cstring>
using namespace std;
class Rvdraw
{
  public:
      Rvdraw(){};
      ~Rvdraw(){};
      inline int writeCharToBuf(unsigned char* buf, unsigned char value) ;
      inline int writeFloatToBuf(unsigned char* buf, float value);
      inline int writeColorToBuf(unsigned char* buf, const float* color, int channels);
      inline int writeStringToBuf(unsigned char* buf, const string* text);
      unsigned char* newBufferSwap(const string* name, int* bufSize);
      unsigned char* newCircle(const float* center, float radius, float thickness,
	  const float* color, const string* setName, int* bufSize);
      unsigned char* newLine(const float* a, const float* b, float thickness,
	  const float* color, const string* setName, int* bufSize);
      unsigned char* newPoint(const float* p, float size, const float* color, 
	  const string* setName, int* bufSize);
      unsigned char* newSphere(const float* p, float radius, const float* color, 
	  const string* setName, int* bufSize);
      unsigned char* newPolygon(const float* v, int numVerts, const float* color,
	  const string* setName, int* bufSize);
      unsigned char* newAnnotation(const string* text, const float* p,
	  const float* color, const string* setName, int* bufSize);
};

//unsigned char* newAgentAnnotation(const string* text, bool leftTeam,
//    int agentNum, const float* color, int* bufSize) {
//
//  *bufSize = (text == NULL) ? 3 : 7 + text->length;
//  unsigned char* buf = new unsigned char[*bufSize];
//
//  long i = 0;
//  i += writeCharToBuf(buf+i, 2);
//  
//  if (text == NULL) {
//    i += writeCharToBuf(buf+i, 2);
//    i += writeCharToBuf(buf+i, (leftTeam ? agentNum - 1 : agentNum + 127));
//  } else {
//    i += writeCharToBuf(buf+i, 1);
//    i += writeCharToBuf(buf+i, (leftTeam ? agentNum - 1 : agentNum + 127));
//    i += writeColorToBuf(buf+i, color, 3);
//    i += writeStringToBuf(buf+i, text);
//  }
//
//  return buf;
//}

#endif
