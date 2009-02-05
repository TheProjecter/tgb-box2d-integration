//=============================================================================
// Box2dJointEdgeRef.cc
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

#include "./Box2dJointEdgeRef.h"
#include "./Box2dJointRef.h"
#include "../Box2dBodyRef.h"
#include "../Box2dUtil.h"

IMPLEMENT_CONOBJECT(Box2dJointEdgeRef);

//=----------------------------------------------------------------------------
// Box2dJointEdgeRef::Box2dJointEdgeRef()
//=----------------------------------------------------------------------------
Box2dJointEdgeRef::Box2dJointEdgeRef( b2JointEdge *jointEdge /* = NULL */ ) :
    mJointEdge( jointEdge )
{
    DEBUG_INC( Box2dJointEdgeRefsCreated );
}


//=----------------------------------------------------------------------------
// Box2dJointEdgeRef::~Box2dJointEdgeRef()
//=----------------------------------------------------------------------------
Box2dJointEdgeRef::~Box2dJointEdgeRef()
{
    DEBUG_INC( Box2dJointEdgeRefsDestroyed );
}


//=----------------------------------------------------------------------------
// Box2dJointEdgeRef::getNext()
//=----------------------------------------------------------------------------
Box2dJointEdgeRef *Box2dJointEdgeRef::getNext() const
{
    AssertFatal( this->mJointEdge != NULL,
        "Box2dJointEdgeRef::getNext() - No b2JointEdge referenced." );

    b2JointEdge *nextEdge = this->mJointEdge->next;

    if ( nextEdge != NULL )
    {
        b2Body *thisBody = NULL;

        if ( this->mJointEdge->joint->GetBody1() == this->mJointEdge->other )
        {
            thisBody = this->mJointEdge->joint->GetBody2();
        }
        else
        {
            thisBody = this->mJointEdge->joint->GetBody1();
        }

        Box2dJointRef *nextJoint = 
            static_cast<Box2dJointRef*>( nextEdge->joint->GetUserData() );

        if ( nextJoint->getJointEdgeRef1()->mJointEdge->other == thisBody )
        {
            return nextJoint->getJointEdgeRef2();
        }
        else
        {
            return nextJoint->getJointEdgeRef1();
        }
    }
    else
    {
        return NULL;
    }
}


//=----------------------------------------------------------------------------
// Box2dJointEdgeRef::getNext() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod(Box2dJointEdgeRef, getNext, S32, 2, 2, "" )
{
    Box2dJointEdgeRef *next = object->getNext();
    return next != NULL ? next->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dJointEdgeRef::getPrev()
//=----------------------------------------------------------------------------
Box2dJointEdgeRef *Box2dJointEdgeRef::getPrev() const
{
    AssertFatal( mJointEdge != NULL,
        "Box2dJointEdgeRef::getPrev() - No b2JointEdge referenced." );

    b2JointEdge *prevEdge = this->mJointEdge->prev;

    if ( prevEdge != NULL )
    {
        b2Body *thisBody = NULL;

        if ( this->mJointEdge->joint->GetBody1() == this->mJointEdge->other )
        {
            thisBody = this->mJointEdge->joint->GetBody2();
        }
        else
        {
            thisBody = this->mJointEdge->joint->GetBody1();
        }

        Box2dJointRef *prevJoint = 
            static_cast<Box2dJointRef*>( prevEdge->joint->GetUserData() );

        if ( prevJoint->getJointEdgeRef1()->mJointEdge->other == thisBody )
        {
            return prevJoint->getJointEdgeRef2();
        }
        else
        {
            return prevJoint->getJointEdgeRef1();
        }
    }
    else
    {
        return NULL;
    }
}


//=----------------------------------------------------------------------------
// Box2dJointEdgeRef::getPrev() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod(Box2dJointEdgeRef, getPrev, S32, 2, 2, "" )
{
    Box2dJointEdgeRef *prev = object->getPrev();
    return prev != NULL ? prev->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dJointEdgeRef::getJoint()
//=----------------------------------------------------------------------------
Box2dJointRef *Box2dJointEdgeRef::getJoint() const
{
    AssertFatal( mJointEdge != NULL,
        "Box2dJointEdgeRef::getJoint() - No b2JointEdge referenced." );

    return static_cast<Box2dJointRef*>( 
        this->mJointEdge->joint->GetUserData() 
        );
}


//=----------------------------------------------------------------------------
// Box2dJointEdgeRef::getJoint() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dJointEdgeRef, getJoint, S32, 2, 2, "" )
{
    return object->getJoint()->getId();
}


//=----------------------------------------------------------------------------
// Box2dJointEdgeRef::getOther()
//=----------------------------------------------------------------------------
Box2dBodyRef *Box2dJointEdgeRef::getOther() const
{
    AssertFatal( mJointEdge != NULL,
        "Box2dJointEdgeRef::getOther() - No b2JointEdge referenced." );

    return static_cast<Box2dBodyRef*>( 
        this->mJointEdge->other->GetUserData() 
        );
}

//=----------------------------------------------------------------------------
// Box2dJointEdgeRef::getOther() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dJointEdgeRef, getOther, S32, 2, 2, "" )
{
    return object->getOther()->getId();
}
