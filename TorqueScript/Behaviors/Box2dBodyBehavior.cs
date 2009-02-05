//=============================================================================
// Box2dBodyBehavior.cs
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
// Box2dBodyBehavior Template
//=----------------------------------------------------------------------------
if ( !isObject(Box2dBodyBehavior) )
{
	new BehaviorTemplate(Box2dBodyBehavior);
	
	Box2dBodyBehavior.behaviorType = "Box2D integration";
	Box2dBodyBehavior.friendlyName = "Box2D Body Behavior";
	Box2dBodyBehavior.description = "Makes the owning t2dSceneObject a Box2D body";
	
	Box2dBodyBehavior.addBehaviorField( "Mass", "The mass of the object", float, 0.0 );
	Box2dBodyBehavior.addBehaviorField( "Inertia", "The inertia of the object", float, 0.0 );
	Box2dBodyBehavior.addBehaviorField( "CenterOfMass", "The center of mass of the object", Point2F, "0.0 0.0" );
	Box2dBodyBehavior.addBehaviorField( "AutoMass", "If true the object's mass/inertia/center is calculated automatically", bool, true );
	Box2dBodyBehavior.addBehaviorField( "LinearDamping", "The linear damping coefficient of the object", float, 0.0 );
	Box2dBodyBehavior.addBehaviorField( "AngularDamping", "The angular damping coefficient of the object", float, 0.0 );
	Box2dBodyBehavior.addBehaviorField( "AllowSleeping", "Is this object allowed to sleep?", bool, true );
	Box2dBodyBehavior.addBehaviorField( "IsSleeping", "Is the object sleeping initially?", bool, false );
	Box2dBodyBehavior.addBehaviorField( "FixedRotation", "If true the object will not rotate", bool, false );
	Box2dBodyBehavior.addBehaviorField( "IsBullet", "Better tunneling prevents", bool, false );
}


//=----------------------------------------------------------------------------
// Box2dBodyBehavior::getBodyRef()
//=----------------------------------------------------------------------------
function Box2dBodyBehavior::getBodyRef( %this )
{
	return %this.bodyRef;	
}


//=----------------------------------------------------------------------------
// Box2dBodyBehavior::onRemoveFromScene()
//=----------------------------------------------------------------------------
function Box2dBodyBehavior::onRemoveFromScene( %this )
{
	t2dAssert( %this.owner.box2d_can_be_removed, 
		"Warning - Box2dBody sceneObject not removed through Box2dWorldRef::destroyBody()" );
}
