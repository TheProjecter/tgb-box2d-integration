//=============================================================================
// Box2dMouseJointRef.h
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

#ifndef _BOX2D_MOUSE_JOINT_REF_H_
#define _BOX2D_MOUSE_JOINT_REF_H_

#include "./Box2dJointRef.h"

//=============================================================================
// class Box2dMouseJointRef
//=============================================================================
class Box2dMouseJointRef : public Box2dJointRef {
public:

    // Use this to update the target point.
	void setTarget( const b2Vec2& target );

    DECLARE_CONOBJECT(Box2dMouseJointRef);
private:
    typedef Box2dJointRef Parent;
    
    b2MouseJoint * joint() const 
    { 
        return static_cast<b2MouseJoint*>( this->getJoint() ); 
    }
};

#endif
