//=============================================================================
// Box2dBridgeTest.cs
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
// Box2dBridgeTest::getTestName()
//=----------------------------------------------------------------------------
function Box2dBridgeTest::getTestName( %this )
{
    return "BridgeTest";
}


//=----------------------------------------------------------------------------
// Box2dBridgeTest::setupScene()
//=----------------------------------------------------------------------------
function Box2dBridgeTest::setupScene( %this, %sceneGraph )
{
    %groundSceneObject = Box2dCreateRectanglePoly( "100 20" );
    %groundSceneObject.setPosition( "0 10" );
    %groundSceneObject.addBehavior( Box2dBodyBehavior.createInstance() );
    %groundSceneObject.addBehavior( Box2dShapeBehavior.createInstance() );

    %sceneGraph.addToScene( %groundSceneObject );    
    
    %sceneGraph.worldRef.createBody( %groundSceneObject );
    %groundSceneObject.bodyRef.createShape( %groundSceneObject );
    
    %numPlanks = 30;

    %prevBody = %groundSceneObject.bodyRef;
    
    %jd = new SimObject();
    %jd.jointType = e_revoluteJoint;
    
    for ( %i = 0; %i < %numPlanks; %i++ )
    {
        %plankSceneObject = %this.createPlank();
        %plankSceneObject.SetPosition( ( -14.5 + %i ) SPC -5 );
        
        %sceneGraph.addToScene( %plankSceneObject ); 
        
        %sceneGraph.worldRef.createBody( %plankSceneObject );        
        %plankSceneObject.bodyRef.createShape( %plankSceneObject );
        %plankSceneObject.bodyRef.setMassFromShapes();        
        
        %anchor = ( -15.0 + %i ) SPC -5;

        %this.initJointDef( %jd, %prevBody, %plankSceneObject.bodyRef, %anchor );
        
        %sceneGraph.worldRef.createJoint( %jd );
        
        %prevBody = %plankSceneObject.bodyRef;
    }

    %anchor = ( -15 + %numPlanks ) SPC -5;
    %this.initJointDef( %jd, %prevBody, %groundSceneObject.bodyRef, %anchor );
    %sceneGraph.worldRef.createJoint( %jd );
    
    %jd.delete();
}


//=----------------------------------------------------------------------------
// Box2dBridgeTest::createPlank()
//=----------------------------------------------------------------------------
function Box2dBridgeTest::createPlank( %this )
{
    %sceneObject = Box2dCreateRectanglePoly( "1 0.25" );
    
    %shapeBehavior = Box2dShapeBehavior.createInstance();
    %shapeBehavior.Density = 20;
    %shapeBehavior.Friction = 0.2;
    
    %sceneObject.addBehavior( Box2dBodyBehavior.createInstance() );
    %sceneObject.addBehavior( %shapeBehavior );
    
    return %sceneObject;
}


//=----------------------------------------------------------------------------
// Box2dBridgeTest::initJointDef()
//=----------------------------------------------------------------------------
function Box2dBridgeTest::initJointDef( %this, %def, %prevBody, %body, %anchor )
{
    %def.body1 = %prevBody;
	%def.body2 = %body;
	%def.localAnchor1 = %prevBody.getLocalPoint( %anchor );
	%def.localAnchor2 = %body.getLocalPoint( %anchor );
	%def.referenceAngle = %body.getAngle() - %prevBody.getAngle();
}

$Box2dBridgeTest = new t2dSceneObject()
{
    class = "Box2dBridgeTest";
};

RegisterTest( $Box2dBridgeTest );
