//=============================================================================
// Box2dPulleyJointRef.cc
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

#include "./Box2dPulleyJointRef.h"
#include "console/console.h"
#include "../Box2dUtil.h"

IMPLEMENT_CONOBJECT(Box2dPulleyJointRef);

//=----------------------------------------------------------------------------
// Box2dPulleyJointRef::getGroundAnchor1()
//=----------------------------------------------------------------------------
b2Vec2 Box2dPulleyJointRef::getGroundAnchor1() const
{
    return this->joint()->GetGroundAnchor1();
}


//=----------------------------------------------------------------------------
// Box2dPulleyJointRef::getGroundAnchor1() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPulleyJointRef, getGroundAnchor1, const char*, 2, 2, "" )
{
    return Box2dUtil::b2Vec2ToReturnBuffer( object->getGroundAnchor1() );
}


//=----------------------------------------------------------------------------
// Box2dPulleyJointRef::getGroundAnchor2()
//=----------------------------------------------------------------------------
b2Vec2 Box2dPulleyJointRef::getGroundAnchor2() const
{
    return this->joint()->GetGroundAnchor2();
}


//=----------------------------------------------------------------------------
// Box2dPulleyJointRef::getGroundAnchor2() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPulleyJointRef, getGroundAnchor2, const char*, 2, 2, "" )
{
    return Box2dUtil::b2Vec2ToReturnBuffer( object->getGroundAnchor2() );
}


//=----------------------------------------------------------------------------
// Box2dPulleyJointRef::getLength1()
//=----------------------------------------------------------------------------
float32 Box2dPulleyJointRef::getLength1() const
{
    return this->joint()->GetLength1();
}


//=----------------------------------------------------------------------------
// Box2dPulleyJointRef::getLength1() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPulleyJointRef, getLength1, F32, 2, 2, "" )
{
    return object->getLength1();
}


//=----------------------------------------------------------------------------
// Box2dPulleyJointRef::getLength2()
//=----------------------------------------------------------------------------
float32 Box2dPulleyJointRef::getLength2() const
{
    return this->joint()->GetLength2();
}


//=----------------------------------------------------------------------------
// Box2dPulleyJointRef::getLength2() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPulleyJointRef, getLength2, F32, 2, 2, "" )
{
    return object->getLength2();
}


//=----------------------------------------------------------------------------
// Box2dPulleyJointRef::getRatio()
//=----------------------------------------------------------------------------
float32 Box2dPulleyJointRef::getRatio() const
{
    return this->joint()->GetRatio();
}


//=----------------------------------------------------------------------------
// Box2dPulleyJointRef::getRatio() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPulleyJointRef, getRatio, F32, 2, 2, "" )
{
    return object->getRatio();
}
