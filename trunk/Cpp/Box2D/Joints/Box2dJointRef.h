//=============================================================================
// Box2dJointRef.h
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

#ifndef _BOX2D_JOINT_REF_H_
#define _BOX2D_JOINT_REF_H_

#include "Box2D.h"
#include "console/simBase.h"

class Box2dBodyRef;
class Box2dJointEdgeRef;

//=============================================================================
// class Box2dJointRef
//=============================================================================
class Box2dJointRef : public SimObject {
public:
    Box2dJointRef();
    virtual ~Box2dJointRef();

    b2JointType getType() const;

	Box2dBodyRef* getBody1();
	Box2dBodyRef* getBody2();

	b2Vec2 getAnchor1() const;
	b2Vec2 getAnchor2() const;

	b2Vec2 getReactionForce() const;
	float32 getReactionTorque() const;

	Box2dJointRef* getNext();

    void setUserData( void * userData ) { this->mUserData = userData; }
    void *getUserData() const { return this->mUserData; }

    void onJointDestroyedCallback();

    Box2dJointEdgeRef *getJointEdgeRef1() const { return mJointEdgeRefs[0]; }
    Box2dJointEdgeRef *getJointEdgeRef2() const { return mJointEdgeRefs[1]; }

    static const char* getJointTypeName( const b2JointType type );
    static b2JointType getJointTypeFromName( const char * name );

    DECLARE_CONOBJECT(Box2dJointRef);
protected:
    friend class Box2dWorldRef;

    void setJoint( b2Joint * joint );
    b2Joint * getJoint() const { return this->mJoint; }

    void destroy();

private:
    typedef SimObject Parent;

    b2Joint *mJoint;
    void *   mUserData;
    Box2dJointEdgeRef *mJointEdgeRefs[2];
};

#endif
