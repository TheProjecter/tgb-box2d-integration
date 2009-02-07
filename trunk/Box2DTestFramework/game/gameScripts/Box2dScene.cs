//=============================================================================
// Box2dScene.cs
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
// Box2dScene::onUpdateSceneTick()
//=----------------------------------------------------------------------------
function Box2dScene::onUpdateSceneTick( %this )
{
    if ( isObject( %this.worldRef ) )
    {
        %this.worldRef.step( %this.worldRef.getTickTimeStep(), 10 );
    }
}


//=----------------------------------------------------------------------------
// Box2dScene::onLevelLoaded()
//=----------------------------------------------------------------------------
function Box2dScene::onLevelLoaded( %this )
{
    %this.worldRef = new Box2dWorldRef()
    {
        sceneGraph = %this;
        gravity = %this.gravity;
    };
    
    %this._setupPreExistingObjects();
}


//=----------------------------------------------------------------------------
// Box2dScene::_setupPreExistingObjects()
//=----------------------------------------------------------------------------
function Box2dScene::_setupPreExistingObjects( %this )
{
    %objectCount = %this.getSceneObjectCount();
    
    // setup pre-existing bodies
    for ( %i = 0; %i < %objectCount; %i++ )
    {
        %sceneObject = %this.getSceneObject( %i );

        if ( isObject( %sceneObject.getBehavior( "Box2dBodyBehavior" ) ) )
        {
            %bodyRef = %this.createBody( %sceneObject );
	
            if ( !t2dVectorCompare( %sceneObject.LinearVelocity, "0 0" ) )
            {
                %bodyRef.setLinearVelocity( %sceneObject.LinearVelocity );	
            }
	
            if ( %sceneObject.AngularVelocity != 0.0 )
            {
                %bodyRef.setAngularVelocity( mDegToRad( %sceneObject.AngularVelocity ) );
            }

            %sceneObject.setAtRest();
        }
    }

    // setup pre-existing shapes
    for ( %i = 0; %i < %objectCount; %i++ )
    {
        %sceneObject = %this.getSceneObject( %i );
        
        if ( isObject( %sceneObject.getBehavior(Box2dShapeBehavior) ) )
        {
            %bodySceneObject = %this._findBodySceneObjectForShape( %sceneObject );
            %bodyRef = %bodySceneObject.bodyRef;
            
            if ( !isObject( %bodyRef ) )
            {
                error( "Invalid bodyRef!" );	
            }
            
            %bodyRef.createShape( %sceneObject );
        }
    }
}


//=----------------------------------------------------------------------------
// Box2dScene::onLevelEnded()
//=----------------------------------------------------------------------------
function Box2dScene::onLevelEnded( %this )
{
    if ( isObject( %this.worldRef ) )
    {
        %this.worldRef.delete();
    }
    
    if ( isFunction(printBox2dDebugInfo) )
    {
        printBox2dDebugInfo();
    }
}


//=----------------------------------------------------------------------------
// Box2dScene::createBody()
//=----------------------------------------------------------------------------
function Box2dScene::createBody( %this, %sceneObject )
{
    %bodyRef = %this.worldRef.createBody( %sceneObject );	
    
    if ( !isObject( %bodyRef ) )
    {
        error( "Box2dScene::createBody() - Error creating Box2dBody" );	
    }
}


//=----------------------------------------------------------------------------
// Box2dScene::_findBodySceneObjectForShape()
//=----------------------------------------------------------------------------
function Box2dScene::_findBodySceneObjectForShape( %this, %shapeSceneObject )
{
    %bodyBehavior = %shapeSceneObject.getBehavior( "Box2dBodyBehavior" );		
    
    if ( isObject( %bodyBehavior ) )
    {
        return %shapeSceneObject;
    }
    else
    {
        %parent = %shapeSceneObject.getMountedParent();
        %bodyBehavior = %parent.getBehavior( "Box2dBodyBehavior" );
        
        if ( isObject( %bodyBehavior ) )
        {
            return %parent;
        }
        else
        {
            error( "Box2dScene::_findBodySceneObjectForShape() -" SPC 
                   "Could not find Box2D-body object." );	
        }
    }
}
