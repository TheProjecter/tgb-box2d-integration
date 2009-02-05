//=============================================================================
// Box2dUtil.cc
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

#include "./Box2dUtil.h"
#include "T2D/t2dSceneObject.h"
#include <cstdlib>


#ifdef TORQUE_DEBUG

unsigned int Box2dBodyRefsCreated = 0;
unsigned int Box2dBodyRefsDestroyed = 0;
unsigned int Box2dShapeRefsCreated = 0;
unsigned int Box2dShapeRefsDestroyed = 0;
unsigned int Box2dJointRefsCreated = 0;
unsigned int Box2dJointRefsDestroyed = 0;
unsigned int Box2dWorldRefsCreated = 0;
unsigned int Box2dWorldRefsDestroyed = 0;
unsigned int Box2dJointEdgeRefsCreated = 0;
unsigned int Box2dJointEdgeRefsDestroyed = 0;

ConsoleFunction( printBox2dDebugInfo, void, 1, 1, "" )
{
    Con::printf( "Box2dBodyRefsCreated:    %u", Box2dBodyRefsCreated );
    Con::printf( "Box2dBodyRefsDestroyed:  %u", Box2dBodyRefsDestroyed );
    Con::printf( "Box2dShapeRefsCreated:   %u", Box2dShapeRefsCreated );
    Con::printf( "Box2dShapeRefsDestroyed: %u", Box2dShapeRefsDestroyed );
    Con::printf( "Box2dJointRefsCreated:   %u", Box2dJointRefsCreated );
    Con::printf( "Box2dJointRefsDestroyed: %u", Box2dJointRefsDestroyed );
    Con::printf( "Box2dWorldRefsCreated:   %u", Box2dWorldRefsCreated );
    Con::printf( "Box2dWorldRefsDestroyed: %u", Box2dWorldRefsDestroyed );
    Con::printf( "Box2dJointEdgeRefsCreated: %u", Box2dJointEdgeRefsCreated );
    Con::printf( "Box2dJointEdgeRefsDestroyed: %u", Box2dJointEdgeRefsDestroyed );
}

#endif

namespace Box2dUtil
{

//=----------------------------------------------------------------------------
// strToFloat()
//=----------------------------------------------------------------------------
float strToFloat( const char * str )
{
    AssertFatal( isValidFloat( str ), "Invalid float encountered" );
    return dAtof( str );
}


//=----------------------------------------------------------------------------
// strTob2Vec2()
//=----------------------------------------------------------------------------
b2Vec2 strTob2Vec2( const char * str ) 
{
    AssertFatal( isValid2dVector( str ), "Invalid 2d Vector encountered" );

    const t2dVector vec = t2dSceneObject::getStringElementVector( str );

    return b2Vec2( vec.mX, vec.mY );
}

//=----------------------------------------------------------------------------
// b2Vec2ToReturnBuffer()
//=----------------------------------------------------------------------------
const char * b2Vec2ToReturnBuffer( const b2Vec2 & v )
{
    const int BUFFER_SIZE = 32;
    char * returnBuffer = Con::getReturnBuffer( BUFFER_SIZE );

    dSprintf( returnBuffer, BUFFER_SIZE, "%f %f", v.x, v.y );

    return returnBuffer;
}


//=----------------------------------------------------------------------------
// isValidFloat()
//=----------------------------------------------------------------------------
bool isValidFloat( const char* str )
{
    const char * ptr = t2dSceneObject::getFirstNonWhitespace( str );

    // Search for first whitespace after number
    while( *ptr != ' ' && *ptr != '\n' && *ptr != '\t' && *ptr != '\0' )
        ptr++;

    char * end = NULL;

    // Now ptr points to the first character after the number
    const double num = strtod( str, &end );

    // end points to the character after the last valid character,
    // so it should point to the first whitespace after the number.
    // If not conversion had to end early.
    return (end == ptr);
}


//=----------------------------------------------------------------------------
// isValidFloat() - ConsoleFunction
//=----------------------------------------------------------------------------
ConsoleFunction( isValidFloat, bool, 2, 2, "" )
{
    return isValidFloat( argv[1] );
}


//=----------------------------------------------------------------------------
// isValid2dVector()
//=----------------------------------------------------------------------------
bool isValid2dVector( const char* str )
{
    const U32 count = t2dSceneObject::getStringElementCount( str );

    if ( count == 2 ) 
    {
        const char * x = t2dSceneObject::getStringElement( str, 0 ); 
        const char * y = t2dSceneObject::getStringElement( str, 1 );

        return isValidFloat( x ) && isValidFloat( y );
    }
    else
    {
        return false;
    }
}


//=----------------------------------------------------------------------------
// isValid2dVector() - ConsoleFunction
//=----------------------------------------------------------------------------
ConsoleFunction( isValid2dVector, bool, 2, 2, "" )
{
    return isValid2dVector( argv[1] );
}


//=----------------------------------------------------------------------------
// strTo16BitMask()
//=----------------------------------------------------------------------------
uint16 strTo16BitMask( const char* str )
{
    const U32 count = t2dSceneObject::getStringElementCount( str );

    uint16 mask = 0;

    for ( U32 i = 0; i < count; ++i )
    {
        const int bit = dAtoi( t2dSceneObject::getStringElement( str, i ) );
        AssertFatal( bit < 16, "Invalid bit mask" );

        mask |= ( 1 << bit );
    }

    return mask;
}

} // end of namespace Box2dUtil
