//=============================================================================
// Box2dJointRef.cc
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

#include "./Box2dJointRef.h"
#include "platform/platform.h"
#include "../Box2dBodyRef.h"
#include "../Box2dUtil.h"
#include "./Box2dJointEdgeRef.h"

IMPLEMENT_CONOBJECT(Box2dJointRef);

//=----------------------------------------------------------------------------
// Box2dJointRef::Box2dJointRef()
//=----------------------------------------------------------------------------
Box2dJointRef::Box2dJointRef():
    mJoint( NULL ),
    mUserData( NULL )
{
    DEBUG_INC( Box2dJointRefsCreated );

    mJointEdgeRefs[0] = NULL;
    mJointEdgeRefs[1] = NULL;
}


//=----------------------------------------------------------------------------
// Box2dJointRef::~Box2dJointRef()
//=----------------------------------------------------------------------------
Box2dJointRef::~Box2dJointRef()
{
    DEBUG_INC( Box2dJointRefsDestroyed );    
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getType()
//=----------------------------------------------------------------------------
b2JointType Box2dJointRef::getType() const
{
    AssertFatal( this->mJoint != NULL, "Box2dJointRef::getType() - No b2Joint referenced." );
    return this->mJoint->GetType();
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getType() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dJointRef, getType, const char*, 2, 2, "" )
{
    char *returnBuffer = Con::getReturnBuffer( 16 );
    dStrcpy( returnBuffer, Box2dJointRef::getJointTypeName( object->getType() ) );

    return returnBuffer;
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getBody1()
//=----------------------------------------------------------------------------
Box2dBodyRef* Box2dJointRef::getBody1()
{
    AssertFatal( this->mJoint != NULL, "Box2dJointRef::getBody1() - No b2Joint referenced." );
    b2Body *body = this->mJoint->GetBody1();
    return body ? static_cast<Box2dBodyRef*>( body->GetUserData() ) : NULL;
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getBody1() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dJointRef, getBody1, S32, 2, 2, "" )
{
    AssertFatal( object->getBody1() != NULL, "" );
    return object->getBody1()->getId();
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getBody2()
//=----------------------------------------------------------------------------
Box2dBodyRef* Box2dJointRef::getBody2()
{
    AssertFatal( this->mJoint != NULL, "Box2dJointRef::getBody2() - No b2Joint referenced." );
    b2Body *body = this->mJoint->GetBody2();
    return body ? static_cast<Box2dBodyRef*>( body->GetUserData() ) : NULL;
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getBody2() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dJointRef, getBody2, S32, 2, 2, "" )
{
    AssertFatal( object->getBody2() != NULL, "" );
    return object->getBody2()->getId();
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getAnchor1()
//=----------------------------------------------------------------------------
b2Vec2 Box2dJointRef::getAnchor1() const
{
    AssertFatal( this->mJoint != NULL, "Box2dJointRef::getAnchor1() - No b2Joint referenced." );
    return this->mJoint->GetAnchor1();
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getAnchor1() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dJointRef, getAnchor1, const char*, 2, 2, "" )
{
    return Box2dUtil::b2Vec2ToReturnBuffer( object->getAnchor1() );
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getAnchor2()
//=----------------------------------------------------------------------------
b2Vec2 Box2dJointRef::getAnchor2() const
{
    AssertFatal( this->mJoint != NULL, "Box2dJointRef::getAnchor2() - No b2Joint referenced." );
    return this->mJoint->GetAnchor2();
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getAnchor2() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dJointRef, getAnchor2, const char*, 2, 2, "" )
{
    return Box2dUtil::b2Vec2ToReturnBuffer( object->getAnchor2() );
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getReactionForce()
//=----------------------------------------------------------------------------
b2Vec2 Box2dJointRef::getReactionForce() const
{
    AssertFatal( this->mJoint != NULL, 
        "Box2dJointRef::getReactionForce() - No b2Joint referenced." );
    return this->mJoint->GetReactionForce();
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getReactionForce() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dJointRef, getReactionForce, const char*, 2, 2, "" )
{
    return Box2dUtil::b2Vec2ToReturnBuffer( object->getReactionForce() );
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getReactionTorque()
//=----------------------------------------------------------------------------
float32 Box2dJointRef::getReactionTorque() const
{
    AssertFatal( this->mJoint != NULL, 
        "Box2dJointRef::getReactionTorque() - No b2Joint referenced." );

    return this->mJoint->GetReactionTorque();
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getReactionTorque() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dJointRef, getReactionTorque, F32, 2, 2, "" )
{
    return object->getReactionTorque();
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getNext()
//=----------------------------------------------------------------------------
Box2dJointRef* Box2dJointRef::getNext()
{
    AssertFatal( this->mJoint != NULL, "Box2dJointRef::getNext() - No b2Joint referenced." );
    b2Joint * next = this->mJoint->GetNext();
    return next ? static_cast<Box2dJointRef*>( next->GetUserData() ) : NULL;
}


//=----------------------------------------------------------------------------
// Box2dJointRef::onJointDestroyedCallback()
//=----------------------------------------------------------------------------
void Box2dJointRef::onJointDestroyedCallback()
{
    this->destroy();
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getNext() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dJointRef, getNext, S32, 2, 2, "" )
{
    return object->getNext() ? object->getNext()->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getJointTypeName()
//=----------------------------------------------------------------------------
const char* Box2dJointRef::getJointTypeName( const b2JointType type )
{
    switch ( type )
    {
    case e_revoluteJoint: return "e_revoluteJoint";
    case e_prismaticJoint: return "e_prismaticJoint";
    case e_distanceJoint: return "e_distanceJoint";
    case e_pulleyJoint: return "e_pulleyJoint";
    case e_mouseJoint: return "e_mouseJoint";
    case e_gearJoint: return "e_gearJoint";
    case e_unknownJoint:
    default: return "e_unknownJoint";
    }
}


//=----------------------------------------------------------------------------
// Box2dJointRef::getJointTypeFromName()
//=----------------------------------------------------------------------------
b2JointType Box2dJointRef::getJointTypeFromName( const char * name )
{
    if ( dStricmp( name, "e_revoluteJoint" ) == 0 )
        return e_revoluteJoint;

    if ( dStricmp( name, "e_prismaticJoint" ) == 0 ) 
        return e_prismaticJoint;

    if ( dStricmp( name, "e_distanceJoint" ) == 0 )
        return e_distanceJoint;

    if ( dStricmp( name, "e_pulleyJoint" ) == 0 )
        return e_pulleyJoint;

    if ( dStricmp( name, "e_mouseJoint" ) == 0 )
        return e_mouseJoint;

    if ( dStricmp( name, "e_gearJoint" ) == 0 )
        return e_gearJoint;

    return e_revoluteJoint;
}


//=----------------------------------------------------------------------------
// Box2dJointRef::destroy()
//=----------------------------------------------------------------------------
void Box2dJointRef::destroy()
{
    AssertFatal( this->mJoint != NULL, "" );

    this->mJoint = NULL;

    if ( this->mJointEdgeRefs[0] != NULL )
    {
        this->mJointEdgeRefs[0]->deleteObject();
        this->mJointEdgeRefs[0] = NULL;
    }

    if ( this->mJointEdgeRefs[1] != NULL )
    {
        this->mJointEdgeRefs[1]->deleteObject();
        this->mJointEdgeRefs[1] = NULL;
    }

    this->deleteObject();
}

//=----------------------------------------------------------------------------
// Box2dJointRef::setJoint()
//=----------------------------------------------------------------------------
void Box2dJointRef::setJoint(b2Joint *joint)
{
    AssertFatal( this->mJoint == NULL, "" );

    this->mJoint = joint;

    for ( b2JointEdge * edge = joint->GetBody1()->GetJointList(); edge != NULL;
          edge = edge->next )
    {
        if ( edge->joint == joint )
        {
            this->mJointEdgeRefs[0] = new Box2dJointEdgeRef( edge );

            if ( !this->mJointEdgeRefs[0]->registerObject() )
            {
                delete this->mJointEdgeRefs[0];
                this->mJointEdgeRefs[0] = NULL;
            }

            break;
        }
    }

    for ( b2JointEdge * edge = joint->GetBody2()->GetJointList(); edge != NULL;
          edge = edge->next )
    {
        if ( edge->joint == joint )
        {
            this->mJointEdgeRefs[1] = new Box2dJointEdgeRef( edge );

            if ( !this->mJointEdgeRefs[1]->registerObject() )
            {
                delete this->mJointEdgeRefs[1];
                this->mJointEdgeRefs[1] = NULL;
            }

            break;
        }
    }

    AssertFatal( this->mJointEdgeRefs[0] != NULL, 
        "Box2dJointRef::setJoint() - mJointEdgeRef[0] not assigned." );
    AssertFatal( this->mJointEdgeRefs[1] != NULL, 
        "Box2dJointRef::setJoint() - mJointEdgeRef[1] not assigned." );
}
