//=============================================================================
// Box2dPrismaticJointRef.h
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

#ifndef _BOX2D_PRISMATIC_JOINT_REF_H_
#define _BOX2D_PRISMATIC_JOINT_REF_H_

#include "./Box2dJointRef.h"

//=============================================================================
// class Box2dPrismaticJointRef
//=============================================================================
class Box2dPrismaticJointRef : public Box2dJointRef {
public:

	float32 getJointTranslation() const;
	float32 getJointSpeed() const;

	bool isLimitEnabled() const;
	void enableLimit( const bool flag );

	float32 getLowerLimit() const;
	float32 getUpperLimit() const;

	void setLimits( const float32 lower, const float32 upper );
	bool isMotorEnabled() const;
	void enableMotor( const bool flag );
	void setMotorSpeed( const float32 speed );
	float32 getMotorSpeed() const;
	void setMaxMotorForce( const float32 force );
	float32 getMotorForce() const;

    DECLARE_CONOBJECT(Box2dPrismaticJointRef);
private:
    typedef Box2dJointRef Parent;

    b2PrismaticJoint* joint() const 
    {
        return static_cast<b2PrismaticJoint*>( this->getJoint() ); 
    }
};

#endif
