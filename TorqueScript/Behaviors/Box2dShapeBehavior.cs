//=============================================================================
// Box2dShapeBehavior.cs
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
// BehaviorFieldStack::createBitMask16Gui()
//=----------------------------------------------------------------------------
function BehaviorFieldStack::createBitMask16Gui(%this, %behavior, %fieldIndex)
{
	%fieldInfo = %behavior.template.getBehaviorField(%fieldIndex);
	%name = getField(%fieldInfo, 0);
	%description = %behavior.template.getBehaviorFieldDescription(%fieldIndex);

	%control = %this.createMask(%name, %name, 0, 15, %description );

	for ( %i = 0; %i < 16; %i++ )
	{		
		%button = %control.findObjectByInternalName("MaskButton" @ %i);
		%button.object = %behavior;
	}
	
	// "All" and "None" buttons are hidden :(
}


//=----------------------------------------------------------------------------
// Box2dShapeBehavior Template
//=----------------------------------------------------------------------------
if ( !isObject(Box2dShapeBehavior) )
{
	if ( isPackage(Tools) )
	{
		// Register needed field types
		BehaviorEditor::registerFieldType("BitMask16", "createBitMask16Gui");	
	}
	
	new BehaviorTemplate(Box2dShapeBehavior);
	
	Box2dShapeBehavior.behaviorType = "Box2D integration";
	Box2dShapeBehavior.friendlyName = "Box2D Shape Behavior";
	Box2dShapeBehavior.description = "Makes the owning t2dSceneObject a Box2D shape";
	
	Box2dShapeBehavior.addBehaviorField( "Type", "The type of this shape", enum, "POLYGON", "POLYGON\tCIRCLE" );
	Box2dShapeBehavior.addBehaviorField( "Friction", "The shape's coefficient of friction", float, 0.2 );
	Box2dShapeBehavior.addBehaviorField( "Restitution", "The shape's coefficient of restitution", float, 0.0 );
	Box2dShapeBehavior.addBehaviorField( "Density", "The shape's density", float, 0.0 );
	Box2dShapeBehavior.addBehaviorField( "IsSensor", "The shape's coefficient of friction", bool, false );
	Box2dShapeBehavior.addBehaviorField( "FilterCategoryBits", "See Box2D documentation", "BitMask16" , "1" );
	Box2dShapeBehavior.addBehaviorField( "FilterMaskBits", "See Box2D documentation", "BitMask16" , "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15" );
	Box2dShapeBehavior.addBehaviorField( "FilterGroupIndex", "See Box2D documentation", int , 0 );
}


//=----------------------------------------------------------------------------
// Box2dShapeBehavior::onRemoveFromScene()
//=----------------------------------------------------------------------------
function Box2dShapeBehavior::onRemoveFromScene( %this )
{
	t2dAssert( %this.owner.box2d_can_be_removed, 
		"Warning - Box2dBody sceneObject not removed through Box2dWorldRef::destroyBody()" );
}




// -- needed for editor integration


//=----------------------------------------------------------------------------
// Box2dShapeBehavior::setFilterCategoryBits()
//=----------------------------------------------------------------------------
function Box2dShapeBehavior::setFilterCategoryBits( %this, %value )
{
	%this.FilterCategoryBits = %value;
}


//=----------------------------------------------------------------------------
// Box2dShapeBehavior::getFilterCategoryBits()
//=----------------------------------------------------------------------------
function Box2dShapeBehavior::getFilterCategoryBits( %this )
{
	return %this.FilterCategoryBits;
}


//=----------------------------------------------------------------------------
// Box2dShapeBehavior::setFilterMaskBits()
//=----------------------------------------------------------------------------
function Box2dShapeBehavior::setFilterMaskBits( %this, %value )
{
	%this.FilterMaskBits = %value;	
}


//=----------------------------------------------------------------------------
// Box2dShapeBehavior::getFilterMaskBits()
//=----------------------------------------------------------------------------
function Box2dShapeBehavior::getFilterMaskBits( %this )
{
	return %this.FilterMaskBits;	
}
