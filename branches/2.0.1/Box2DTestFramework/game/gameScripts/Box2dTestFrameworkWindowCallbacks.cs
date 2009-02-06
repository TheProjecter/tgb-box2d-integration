//=============================================================================
// Box2dTestFrameworkWindowCallbacks.cs
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
// Box2dTestFrameworkWindow::onMouseDown()
//=----------------------------------------------------------------------------
function Box2dTestFrameworkWindow::onMouseDown( %this, %mod, %worldPos, 
    %clicks )
{
    if ( isObject( $currentMoustJoint ) )
    {
        return;
    }
  
    %objects = $Box2dTestFrameworkScene.pickPoint( %worldPos );
    
    if ( %objects $= "" )
    {
        return;
    }
    
    %objectCount = getWordCount( %objects );
    
    %sceneObject = "";
    
    for ( %i = 0; %i < %objectCount; %i++ )
    {
        %object = getWord( %objects, %i );
        %bodyBehavior = %object.getBehavior( Box2dBodyBehavior );
        
        if ( isObject( %bodyBehavior ) )
        {
            %sceneObject = %bodyBehavior.Owner;
            break;
        }
        
        %shapeBehavior = %object.getBehavior( Box2dShapeBehavior );
        
        if ( isObject( %shapeBehavior ) )
        {
            %sceneObject = %object.getMountedParent();
            break;
        }
    }

    if ( %sceneObject $= "" )
    {
        return;    
    }

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
// Box2dTestFrameworkWindow::onMouseUp()
//=----------------------------------------------------------------------------
function Box2dTestFrameworkWindow::onMouseUp( %this, %mod, %worldPos, %clicks )
{
    if ( isObject( $currentMoustJoint ) )
    {
        $Box2dTestFrameworkScene.worldRef.destroyJoint( $currentMoustJoint );
        $currentMoustJoint = "";
    }	
}


//=----------------------------------------------------------------------------
// Box2dTestFrameworkWindow::onMouseDragged()
//=----------------------------------------------------------------------------
function Box2dTestFrameworkWindow::onMouseDragged( %this, %mod, %worldPos, 
    %clicks )
{
    if ( isObject( $currentMoustJoint ) )
    {
        $currentMoustJoint.setTarget( %worldPos );
    }
}


//=----------------------------------------------------------------------------
// Box2dTestFrameworkWindow::onRightMouseDragged()
//=----------------------------------------------------------------------------
function Box2dTestFrameworkWindow::onRightMouseDragged( %this, %modifier, 
    %worldPosition, %clicks )
{
    %windowPoint = %this.getWindowPoint( %worldPosition );
    
    if ( %this.lastRightMousePosition $= "" )
    {
        %this.lastRightMousePosition = %windowPoint;
    }
    else
    {
        %diff = t2dVectorSub( %this.lastRightMousePosition, %windowPoint );
        %newCamPos = t2dVectorAdd( %this.getCurrentCameraPosition(), %diff );
        
        %this.setCurrentCameraPosition( %newCamPos );
        
        %this.lastRightMousePosition = %this.getWindowPoint( %worldPosition );
    }
}


//=----------------------------------------------------------------------------
// Box2dTestFrameworkWindow::onRightMouseUp()
//=----------------------------------------------------------------------------
function Box2dTestFrameworkWindow::onRightMouseUp( %this, %modifier, 
    %worldPosition, %clicks )
{
    %this.lastRightMousePosition = "";
}


