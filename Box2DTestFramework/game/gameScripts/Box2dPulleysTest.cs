//=============================================================================
// Box2dPulleysTest.cs
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
// Box2dPulleysTest::getTestName()
//=----------------------------------------------------------------------------
function Box2dPulleysTest::getTestName( %this )
{
    return "Pulleys Test";
}


//=----------------------------------------------------------------------------
// Box2dPulleysTest::setupScene()
//=----------------------------------------------------------------------------
function Box2dPulleysTest::setupScene( %this, %sceneGraph )
{
    %worldRef = %sceneGraph.worldRef;
    
    %groundSco = Box2dCreateRectanglePoly( "100 20" );
    %groundSco.Position = "0 10";
    
    %groundSco.addBehavior( Box2dShapeBehavior.createInstance() );
    %groundSco.addBehavior( Box2dBodyBehavior.createInstance() );    
    
    %groundBody = %worldRef.createBody( %groundSco );
	%groundBody.createShape( %groundSco );
	
    %a = 2.0;
	%b = 4.0;
    %y = 16.0;
    %L = 12.0;

    %def = Box2dCreateRectanglePoly( ( 2 * %a ) SPC ( 2 * %b ) );
    %shapeBehavior = Box2dShapeBehavior.createInstance();
    %shapeBehavior.density = 5.0;
    %def.addBehavior( %shapeBehavior );
    %def.Position = -10 SPC -%y;
	%def.addBehavior( Box2dBodyBehavior.createInstance() );
    
    %body1Sco = %def.cloneWithBehaviors( false );
	%worldRef.createBody( %body1Sco ).createShape( %body1Sco );	
	
	%body2Sco = %def.cloneWithBehaviors( false );
    %body2Sco.Position = 10 SPC -%y;    
    %worldRef.createBody( %body2Sco ).createShape( %body2Sco );	
    
    %pulleyDef = new SimObject();
    %anchor1 = -10 SPC -( %y + %b );
    %anchor2 = 10 SPC -( %y + %b );
    %groundAnchor1 = -10 SPC -( %y + %b + %L );
    %groundAnchor2 =  10 SPC -( %y + %b + %L );
    Box2dInitializePulleyJointDef( 
        %pulleyDef, 
        %body1Sco.bodyRef, 
        %body2Sco.bodyRef, 
        %groundAnchor1, 
        %groundAnchor2, 
        %anchor1, 
        %anchor2, 
        2 );

    %pulleyDef.collideConnected = true;    
    
	%worldRef.createJoint( %pulleyDef );
	
	%pulleyDef.delete();
	%def.delete();
}

$Box2dPulleysTest = new t2dSceneObject()
{
    class = "Box2dPulleysTest";
};

RegisterTest( $Box2dPulleysTest );
