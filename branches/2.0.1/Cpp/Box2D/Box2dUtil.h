//=============================================================================
// Box2dUtil.h
//=============================================================================
/*
TGB-Box2D-Integration (http://code.google.com/p/tgb-box2d-integration/)
Copyright (c) 2009 Michael Woerister

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _BOX_2D_UTIL_H_
#define _BOX_2D_UTIL_H_

#include "Box2D.h"
#include "T2D/t2dVector.h"

#ifdef TORQUE_DEBUG

inline void DEBUG_INC( unsigned int & variable ) 
{
    ++variable;
}

extern unsigned int Box2dBodyRefsCreated;
extern unsigned int Box2dBodyRefsDestroyed;
extern unsigned int Box2dShapeRefsCreated;
extern unsigned int Box2dShapeRefsDestroyed;
extern unsigned int Box2dJointRefsCreated;
extern unsigned int Box2dJointRefsDestroyed;
extern unsigned int Box2dWorldRefsCreated;
extern unsigned int Box2dWorldRefsDestroyed;
extern unsigned int Box2dJointEdgeRefsCreated;
extern unsigned int Box2dJointEdgeRefsDestroyed;

#else

#define DEBUG_INC(x)    { }

#endif

namespace Box2dUtil
{

//=----------------------------------------------------------------------------
// strToFloat()
//=----------------------------------------------------------------------------
float strToFloat( const char * str );


//=----------------------------------------------------------------------------
// strTob2Vec2()
//=----------------------------------------------------------------------------
b2Vec2 strTob2Vec2( const char * str );

//=----------------------------------------------------------------------------
// b2Vec2ToReturnBuffer()
//=----------------------------------------------------------------------------
const char * b2Vec2ToReturnBuffer( const b2Vec2 & v );

//=----------------------------------------------------------------------------
// isValidFloat()
//=----------------------------------------------------------------------------
bool isValidFloat( const char* str );

//=----------------------------------------------------------------------------
// isValid2dVector()
//=----------------------------------------------------------------------------
bool isValid2dVector( const char* str );

//=----------------------------------------------------------------------------
// strTo16BitMask()
//=----------------------------------------------------------------------------
uint16 strTo16BitMask( const char* str );


//=----------------------------------------------------------------------------
// sign()
//=----------------------------------------------------------------------------
inline int sign( const float value ) 
{ 
    return value < 0.0f ? -1 : ( value > 0.0f ? 1 : 0 ); 
}

inline int sign( const int value ) 
{ 
    return value < 0 ? -1 : ( value > 0 ? 1 : 0 ); 
}


//=----------------------------------------------------------------------------
// projectVector()
//=----------------------------------------------------------------------------
inline t2dVector projectVector( const t2dVector &source, 
                                const t2dVector &dest )
{
    const F32 factor = ( source * dest ) / dest.len();
    return ( dest.getUnitDirection() * factor );
}

} // end of namespace Box2dUtil

#endif
