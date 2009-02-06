//=============================================================================
// Box2dJointUtility.cs
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
// Box2dInitializeRevoluteJoint()
//=----------------------------------------------------------------------------
function Box2dInitializeRevoluteJoint( %jointDef, %body1, %body2, %anchor )
{
    if ( %jointDef.jointType $= "" )
    {
        %jointDef.jointType = e_revoluteJoint;    
    }
    else if ( %jointDef.jointType !$= e_revoluteJoint )
    {
        warn( "Box2dInitializeRevoluteJoint() - %jointDef must be of type" SPC
              "e_revoluteJoint." );
        return;
    }    
    
    %jointDef.body1 = %body1;
	%jointDef.body2 = %body2;
	%jointDef.localAnchor1 = %body1.getLocalPoint( %anchor );
	%jointDef.localAnchor2 = %body2.getLocalPoint( %anchor );
	%jointDef.referenceAngle = %body2.getAngle() - %body1.getAngle();
}


//=----------------------------------------------------------------------------
// Box2dInitializePrismaticJoint()
//=----------------------------------------------------------------------------
function Box2dInitializePrismaticJoint( %jointDef, %body1, %body2, %anchor, 
    %axis )
{
    if ( %jointDef.jointType $= "" )
    {
        %jointDef.jointType = e_prismaticJoint;    
    }
    else if ( %jointDef.jointType !$= e_prismaticJoint )
    {
        warn( "Box2dInitializePrismaticJoint() - %jointDef must be of type" SPC
              "e_prismaticJoint." );
        return;
    }    
    
    %jointDef.body1 = %body1;
	%jointDef.body2 = %body2;
	%jointDef.localAnchor1 = %body1.getLocalPoint( %anchor );
	%jointDef.localAnchor2 = %body2.getLocalPoint( %anchor );
	%jointDef.localAxis1 = %body1.getLocalVector( %axis );
	%jointDef.referenceAngle = %body2.getAngle() - %body1.getAngle();
}