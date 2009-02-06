//=============================================================================
// Box2dSimpleTest.cs
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
// Box2dSimpleTest::getTestName()
//=----------------------------------------------------------------------------
function Box2dSimpleTest::getTestName( %this )
{
    return "SimpleTest";
}


//=----------------------------------------------------------------------------
// Box2dSimpleTest::setupScene()
//=----------------------------------------------------------------------------
function Box2dSimpleTest::setupScene( %this, %sceneGraph )
{
    %object = Box2dCreateRectanglePoly( "2 2" );
    
    %bodyBehavior = Box2dBodyBehavior.createInstance();
    %object.addBehavior( %bodyBehavior );
    %shapeBehavior = Box2dShapeBehavior.createInstance();
    %shapeBehavior.Density = 1;
    %object.addBehavior( %shapeBehavior );
    
    %sceneGraph.addToScene( %object );
    
    %bodyRef = %sceneGraph.worldRef.createBody( %object );
    %bodyRef.createShape( %object );
}

$Box2dSimpleTest = new t2dSceneObject()
{
    class = "Box2dSimpleTest";
};

RegisterTest( $Box2dSimpleTest );