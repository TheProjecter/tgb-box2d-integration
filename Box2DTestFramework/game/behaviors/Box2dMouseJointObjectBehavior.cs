//=============================================================================
// Box2dMouseJointObjectBehavior.cs
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
// Box2dAddMouseJointBehavior Template
//=----------------------------------------------------------------------------
if ( !isObject(Box2dAddMouseJointBehavior) )
{
    new BehaviorTemplate(Box2dAddMouseJointBehavior);

    Box2dAddMouseJointBehavior.behaviorType = "Box2D integration";
    Box2dAddMouseJointBehavior.friendlyName = "Box2DAddMouseJointBehavior";
    Box2dAddMouseJointBehavior.description = 
        "Makes the owning t2dSceneObject a Box2D body";
}


//=----------------------------------------------------------------------------
// Box2dAddMouseJointBehavior::onAddToScene()
//=----------------------------------------------------------------------------
function Box2dAddMouseJointBehavior::onAddToScene( %this, %scene )
{
    %this.owner.useMouseEvents = true;	
    sceneWindow2D.useWindowMouseEvents = true;
}


//=----------------------------------------------------------------------------
// Box2dAddMouseJointBehavior::onMouseDown()
//=----------------------------------------------------------------------------
function Box2dAddMouseJointBehavior::onMouseDown( %this, %mod, %worldPos, 
    %clicks )
{
    if ( isObject( $currentMoustJoint ) )
    {
        return;
    }

    %sceneObject = %this.owner;
    %bodyRef = %sceneObject.bodyRef;	
	
    if ( !isObject( %sceneObject.bodyRef ) )
    {
        if ( isObject( %sceneObject.shapeRef ) )
        {
            %bodyRef = %sceneObject.shapeRef.getBody();	
        }
        else
        {
            return;	
        }
    }

    %worldRef = %sceneObject.getSceneGraph().worldRef;	
    
    %md = new SimObject();
    %md.jointType = e_mouseJoint;
    %md.body1 = %worldRef.getGroundBody();
    %md.body2 = %bodyRef;
    %md.target = %worldPos;
    %md.maxForce = 500.0 * %bodyRef.getMass();
 
    $currentMoustJoint = %worldRef.createJoint( %md );
    %bodyRef.wakeUp();
    
    %this.currentMouseJoint = $currentMoustJoint;
}


//=----------------------------------------------------------------------------
// sceneWindow2D::onMouseUp()
//=----------------------------------------------------------------------------
function sceneWindow2D::onMouseUp( %this, %mod, %worldPos, %clicks )
{
    if ( isObject( $currentMoustJoint ) )
    {
        sceneWindow2D.getSceneGraph().worldRef.destroyJoint( $currentMoustJoint );
        $currentMoustJoint = "";
    }	
}


//=----------------------------------------------------------------------------
// sceneWindow2D::onMouseDragged()
//=----------------------------------------------------------------------------
function sceneWindow2D::onMouseDragged( %this, %mod, %worldPos, %clicks )
{
    if ( isObject( $currentMoustJoint ) )
    {
        $currentMoustJoint.setTarget( %worldPos );
    }
}
