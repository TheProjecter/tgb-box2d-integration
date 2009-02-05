//=============================================================================
// Box2dJointEdgeRef.h
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

#ifndef _BOX2D_JOINTEDGE_REF_H_
#define _BOX2D_JOINTEDGE_REF_H_

#include "console/simBase.h"
#include "Box2D.h"

class Box2dBodyRef;
class Box2dJointRef;

//=============================================================================
// class Box2dJointEdgeRef
//=============================================================================
class Box2dJointEdgeRef : public SimObject {
public:
    Box2dJointEdgeRef( b2JointEdge *jointEdge = NULL );
    virtual ~Box2dJointEdgeRef();

    Box2dJointRef      *getJoint() const;
    Box2dBodyRef       *getOther() const;
    Box2dJointEdgeRef  *getNext() const;
    Box2dJointEdgeRef  *getPrev() const;

    DECLARE_CONOBJECT(Box2dJointEdgeRef);
private:
    typedef SimObject Parent;

    const b2JointEdge *mJointEdge;
};

#endif
