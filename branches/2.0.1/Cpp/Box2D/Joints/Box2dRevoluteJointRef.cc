//=============================================================================
// Box2dRevoluteJointRef.cc
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

#include "./Box2dRevoluteJointRef.h"

IMPLEMENT_CONOBJECT(Box2dRevoluteJointRef);

//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::getJointAngle()
//=----------------------------------------------------------------------------
float32 Box2dRevoluteJointRef::getJointAngle() const
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::getJointAngle() - No b2Joint referenced." );

    return this->joint()->GetJointAngle();
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::getJointSpeed()
//=----------------------------------------------------------------------------
float32 Box2dRevoluteJointRef::getJointSpeed() const
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::getJointSpeed() - No b2Joint referenced." );

    return this->joint()->GetJointSpeed();
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::isLimitEnabled()
//=----------------------------------------------------------------------------
bool Box2dRevoluteJointRef::isLimitEnabled() const
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::isLimitEnabled() - No b2Joint referenced." );

    return this->joint()->IsLimitEnabled();
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::enableLimit()
//=----------------------------------------------------------------------------
void Box2dRevoluteJointRef::enableLimit( const bool flag )
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::enableLimit() - No b2Joint referenced." );

    this->joint()->EnableLimit( flag );
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::getLowerLimit()
//=----------------------------------------------------------------------------
float32 Box2dRevoluteJointRef::getLowerLimit() const
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::getLowerLimit() - No b2Joint referenced." );

    return this->joint()->GetLowerLimit();
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::getUpperLimit()
//=----------------------------------------------------------------------------
float32 Box2dRevoluteJointRef::getUpperLimit() const
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::getUpperLimit() - No b2Joint referenced." );

    return this->joint()->GetUpperLimit();
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::setLimits()
//=----------------------------------------------------------------------------
void Box2dRevoluteJointRef::setLimits( const float32 lower, const float32 upper )
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::setLimits() - No b2Joint referenced." );

    this->joint()->SetLimits( lower, upper );
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::isMotorEnabled()
//=----------------------------------------------------------------------------
bool Box2dRevoluteJointRef::isMotorEnabled() const
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::isMotorEnabled() - No b2Joint referenced." );

    return this->joint()->IsMotorEnabled();
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::enableMotor()
//=----------------------------------------------------------------------------
void Box2dRevoluteJointRef::enableMotor( const bool flag )
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::enableMotor() - No b2Joint referenced." );

    this->joint()->EnableMotor( flag );
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::setMotorSpeed()
//=----------------------------------------------------------------------------
void Box2dRevoluteJointRef::setMotorSpeed( const float32 speed )
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::setMotorSpeed() - No b2Joint referenced." );

    this->joint()->SetMotorSpeed( speed );
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::getMotorSpeed()
//=----------------------------------------------------------------------------
float32 Box2dRevoluteJointRef::getMotorSpeed() const
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::getMotorSpeed() - No b2Joint referenced." );

    return this->joint()->GetMotorSpeed();
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::setMaxMotorTorque()
//=----------------------------------------------------------------------------
void Box2dRevoluteJointRef::setMaxMotorTorque( const float32 torque )
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::setMaxMotorTorque() - No b2Joint referenced." );

    this->joint()->SetMaxMotorTorque( torque );
}


//=----------------------------------------------------------------------------
// Box2dRevoluteJointRef::getMotorTorque()
//=----------------------------------------------------------------------------
float32 Box2dRevoluteJointRef::getMotorTorque() const
{
    AssertFatal( this->getJoint() != NULL, 
        "Box2dRevoluteJointRef::setMotorTorque() - No b2Joint referenced." );

    return this->joint()->GetMotorTorque();
}
