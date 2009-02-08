//=============================================================================
// Box2dGearsTest.cs
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
// Box2dGearsTest::getTestName()
//=----------------------------------------------------------------------------
function Box2dGearsTest::getTestName( %this )
{
    return "Gears Test";
}


//=----------------------------------------------------------------------------
// Box2dGearsTest::setupScene()
//=----------------------------------------------------------------------------
function Box2dGearsTest::setupScene( %this, %sceneGraph )
{
    %worldRef = %sceneGraph.worldRef;
    
    %groundSco = Box2dCreateRectanglePoly( "100 20" );
    %groundSco.Position = "0 10";
    %groundSco.addBehavior( Box2dBodyBehavior.createInstance() );
    %groundSco.addBehavior( Box2dShapeBehavior.createInstance() );
        
    %worldRef.createBody( %groundSco ).createShape( %groundSco );

    %circle1Sco = Box2dCreateCirclePoly( "2 2", 32 );
    %circle1ShapeBehavior = Box2dShapeBehavior.createInstance();    
    %circle1ShapeBehavior.Density = 5;
    %circle1ShapeBehavior.Type = CIRCLE;
    %circle1Sco.addBehavior( %circle1ShapeBehavior );    
    
    %circle2Sco = Box2dCreateCirclePoly( "4 4", 32 );
    %circle2ShapeBehavior = Box2dShapeBehavior.createInstance();    
    %circle2ShapeBehavior.Density = 5;
    %circle2ShapeBehavior.Type = CIRCLE;
    %circle2Sco.addBehavior( %circle2ShapeBehavior );

    %boxSco = Box2dCreateRectanglePoly( "1 10" );
    %boxShapeBehavior = Box2dShapeBehavior.createInstance();
    %boxShapeBehavior.Density = 5;    
    %boxSco.addBehavior( %boxShapeBehavior );    
    
    %circle1Sco.Position = "-3 -12";
    %circle1Sco.addBehavior( Box2dBodyBehavior.createInstance() );
    %worldRef.createBody( %circle1Sco ).createShape( %circle1Sco );
    
    %jd1 = new SimObject()
    {
        jointType = e_revoluteJoint;
        body1 = %groundSco.bodyRef;
        body2 = %circle1Sco.bodyRef;
        localAnchor1 = %groundSco.bodyRef.getLocalPoint( %circle1Sco.Position );
        localAnchor2 = %circle1Sco.bodyRef.getLocalPoint( %circle1Sco.Position );
        referenceAngle = %circle1Sco.bodyRef.getAngle() - %groundSco.bodyRef.getAngle();     
    };
    
    %joint1 = %worldRef.createJoint( %jd1 );
    %jd1.delete();
    
    %circle2Sco.Position = "0 -12";
    %circle2Sco.addBehavior( Box2dBodyBehavior.createInstance() );
    %worldRef.createBody( %circle2Sco ).createShape( %circle2Sco );
    
    %jd2 = new SimObject();
    Box2dInitializeRevoluteJointDef( %jd2, %groundSco.bodyRef,
        %circle2Sco.bodyRef, %circle2Sco.Position );
    
    %joint2 = %worldRef.createJoint( %jd2 );
    %jd2.delete();    
    
    %boxSco.Position = "2.5 -12";
    %boxSco.addBehavior( Box2dBodyBehavior.createInstance() );
    %worldRef.createBody( %boxSco ).createShape( %boxSco );

    %jd3 = new SimObject();
    Box2dInitializePrismaticJointDef( %jd3, %groundSco.bodyRef, %boxSco.bodyRef,
        %boxSco.Position, "0 -1" );
    %jd3.lowerTranslation = -5;
    %jd3.upperTranslation = 5;
    %jd3.enableLimit = true;

    %joint3 = %worldRef.createJoint( %jd3 );
    %jd3.delete();

    %jd4 = new SimObject()
    {
        jointType = e_gearJoint;
        body1 = %circle1Sco.bodyRef;
        body2 = %circle2Sco.bodyRef;
        joint1 = %joint1;
        joint2 = %joint2;
        ratio = 2;
    };
    
    %worldRef.createJoint( %jd4 );
    %jd4.delete();    
   
    %jd5 = new SimObject()
    {
        jointType = e_gearJoint;
        body1 = %circle2Sco.bodyRef;
        body2 = %boxSco.bodyRef;
        joint1 = %joint2;
        joint2 = %joint3;
        ratio = -1 / 2;
    };
    
    %worldRef.createJoint( %jd5 );
    %jd5.delete();
}

$Box2dGearsTest = new t2dSceneObject()
{
    class = "Box2dGearsTest";
};

RegisterTest( $Box2dGearsTest );
