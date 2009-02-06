//=============================================================================
// Box2dMotorsAndLimitsTest.cs
//=============================================================================
//
// TGB-Box2D-Integration (http://code.google.com/p/tgb-box2d-integration/)
// Copyright (c) 2009 Michael Woerister
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.


//=----------------------------------------------------------------------------
// Box2dMotorsAndLimitsTest::getTestName()
//=----------------------------------------------------------------------------
function Box2dMotorsAndLimitsTest::getTestName( %this )
{
    return "Motors and Limits";
}


//=----------------------------------------------------------------------------
// Box2dMotorsAndLimitsTest::setupScene()
//=----------------------------------------------------------------------------
function Box2dMotorsAndLimitsTest::setupScene( %this, %sceneGraph )
{
    %worldRef = %sceneGraph.worldRef;
   
    %groundSco = Box2dCreateRectanglePoly( "100 20" );
    %groundSco.Position = "0 10";
    %groundSco.addBehavior( Box2dBodyBehavior.createInstance() );
    %groundSco.addBehavior( Box2dShapeBehavior.createInstance() );
    
    %sceneGraph.addToScene( %groundSco );
    %worldRef.createBody( %groundSco );
    %groundSco.bodyRef.createShape( %groundSco );
    
    %prevBody = %groundSco.bodyRef;
    %y = -8; 
    
    %box1Sco = %this.createBox();
    %box1Sco.Position = 3 SPC %y;
    %sceneGraph.addToScene( %box1Sco );
    
    %bodyRef = %worldRef.createBody( %box1Sco );
    %bodyRef.createShape( %box1Sco );

    %rjd = new SimObject();
    
    Box2dInitializeRevoluteJointDef( %rjd, %prevBody, %bodyRef, 0 SPC %y );   
    %rjd.motorSpeed = -1.0 * 3.1415;
    %rjd.maxMotorTorque = 10000;
    %rjd.enableMotor = true;
    
    %m_joint1 = %worldRef.createJoint( %rjd );

    %prevBody = %bodyRef;

    %box2Sco = %this.createBox();
    %box2Sco.Position = 9 SPC %y;
    %sceneGraph.addToScene( %box2Sco );
    
    %bodyRef = %worldRef.createBody( %box2Sco );
    %bodyRef.createShape( %box2Sco );

    Box2dInitializeRevoluteJointDef( %rjd, %bodyRef, %prevBody, 6 SPC %y );
    %rjd.motorSpeed = 0.5 * 3.1415;
    %rjd.maxMotorTorque = 2000.0;
    %rjd.enableMotor = true;
    %rjd.lowerAngle = -0.5 * 3.1415;
    %rjd.upperAngle = 0.5 * 3.1415;
    %rjd.enableLimit = true;

    %m_joint2 = %worldRef.createJoint( %rjd );
 
    %box3Sco = %this.createBox();
    %box3Sco.Position = "-10 -10";    
    %box3Sco.Rotation = 90;
    %sceneGraph.addToScene( %box3Sco );    
    
    %bodyRef = %worldRef.createBody( %box3Sco );
    %bodyRef.createShape( %box3Sco );    
 
    %pjd = new SimObject();
    Box2dInitializePrismaticJointDef( %pjd, %groundSco.bodyRef, %bodyRef, "-10 -10",
        "1 0" );
    %pjd.motorSpeed = 10;
    %pjd.maxMotorForce = 1000;
    %pjd.enableMotor = true;
    %pjd.lowerTranslation = 0;
    %pjd.upperTranslation = 20;
    %pjd.enableLimit = true;
 
    %m_joint3 = %worldRef.createJoint( %pjd );
}


//=----------------------------------------------------------------------------
// Box2dMotorsAndLimitsTest::createBox()
//=----------------------------------------------------------------------------
function Box2dMotorsAndLimitsTest::createBox( %this )
{
    %sceneobject = Box2dCreateRectanglePoly( "4 1" );
    %sceneobject.addBehavior( Box2dBodyBehavior.createInstance() );
    %shapeBehavior = Box2dShapeBehavior.createInstance();
    %shapeBehavior.Density = 5;
    %shapeBehavior.Friction = 0.05;    
    %sceneobject.addBehavior( %shapeBehavior );
    
    return %sceneobject;
}

$Box2dMotorsAndLimitsTest = new t2dSceneObject()
{
    class = "Box2dMotorsAndLimitsTest";
};

RegisterTest( $Box2dMotorsAndLimitsTest );
