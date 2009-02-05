//=============================================================================
// Box2dPrismaticJointRef.cc
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

#include "./Box2dPrismaticJointRef.h"
#include "../Box2dUtil.h"

IMPLEMENT_CONOBJECT(Box2dPrismaticJointRef);

//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getJointTranslation()
//=----------------------------------------------------------------------------
float32 Box2dPrismaticJointRef::getJointTranslation() const
{
    return this->joint()->GetJointTranslation();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getJointTranslation() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, getJointTranslation, F32, 2, 2, "" )
{
    return object->getJointTranslation();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getJointSpeed()
//=----------------------------------------------------------------------------
float32 Box2dPrismaticJointRef::getJointSpeed() const
{
    return this->joint()->GetJointSpeed();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getJointSpeed() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, getJointSpeed, F32, 2, 2, "" )
{
    return object->getJointSpeed();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::isLimitEnabled()
//=----------------------------------------------------------------------------
bool Box2dPrismaticJointRef::isLimitEnabled() const
{
    return this->joint()->IsLimitEnabled();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::isLimitEnabled() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, isLimitEnabled, bool, 2, 2, "" )
{
    return object->isLimitEnabled();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::enableLimit()
//=----------------------------------------------------------------------------
void Box2dPrismaticJointRef::enableLimit( const bool flag )
{
    this->joint()->EnableLimit( flag );
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::enableLimit() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, enableLimit, void, 3, 3, "" )
{
    object->enableLimit( dAtob( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getLowerLimit()
//=----------------------------------------------------------------------------
float32 Box2dPrismaticJointRef::getLowerLimit() const
{
    return this->joint()->GetLowerLimit();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getLowerLimit() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, getLowerLimit, F32, 2, 2, "" )
{
    return object->getLowerLimit();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getUpperLimit()
//=----------------------------------------------------------------------------
float32 Box2dPrismaticJointRef::getUpperLimit() const
{
    return this->joint()->GetUpperLimit();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getUpperLimit() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, getUpperLimit, F32, 2, 2, "" )
{
    return object->getUpperLimit();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::setLimits()
//=----------------------------------------------------------------------------
void Box2dPrismaticJointRef::setLimits( const float32 lower, 
                                        const float32 upper )
{
    this->joint()->SetLimits( lower, upper );
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::setLimits() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, setLimits, void, 4, 4, "" )
{
    const float32 lower = Box2dUtil::strToFloat( argv[2] );
    const float32 upper = Box2dUtil::strToFloat( argv[3] );

    object->setLimits( lower, upper );
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::isMotorEnabled()
//=----------------------------------------------------------------------------
bool Box2dPrismaticJointRef::isMotorEnabled() const
{
    return this->joint()->IsMotorEnabled();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::isMotorEnabled() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, isMotorEnabled, bool, 2, 2, "" )
{
    return object->isMotorEnabled();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::enableMotor()
//=----------------------------------------------------------------------------
void Box2dPrismaticJointRef::enableMotor( const bool flag )
{
    this->joint()->EnableMotor( flag );
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::enableMotor() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, enableMotor, void, 3, 3, "" )
{
    object->enableMotor( dAtob( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::setMotorSpeed()
//=----------------------------------------------------------------------------
void Box2dPrismaticJointRef::setMotorSpeed( const float32 speed )
{
    this->joint()->SetMotorSpeed( speed );
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::setMotorSpeed() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, setMotorSpeed, void, 3, 3, "" )
{
    object->setMotorSpeed( Box2dUtil::strToFloat( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getMotorSpeed()
//=----------------------------------------------------------------------------
float32 Box2dPrismaticJointRef::getMotorSpeed() const
{
    return this->joint()->GetMotorSpeed();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getMotorSpeed() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, getMotorSpeed, F32, 2, 2, "" )
{
    return object->getMotorSpeed();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::setMaxMotorForce()
//=----------------------------------------------------------------------------
void Box2dPrismaticJointRef::setMaxMotorForce( const float32 force )
{
    this->joint()->SetMaxMotorForce( force );
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::setMaxMotorForce() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, setMaxMotorForce, void, 3, 3, "" )
{
    object->setMaxMotorForce( Box2dUtil::strToFloat( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getMotorForce()
//=----------------------------------------------------------------------------
float32 Box2dPrismaticJointRef::getMotorForce() const
{
    return this->joint()->GetMotorForce();
}


//=----------------------------------------------------------------------------
// Box2dPrismaticJointRef::getMotorForce() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dPrismaticJointRef, getMotorForce, F32, 2, 2, "" )
{
    return object->getMotorForce();
}
