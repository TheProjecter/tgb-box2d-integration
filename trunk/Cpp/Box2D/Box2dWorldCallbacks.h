//=============================================================================
// Box2dWorldCallbacks.h
//=============================================================================
/*
TGB-Box2D-Integration (http://code.google.com/p/tgb-box2d-integration/)
Copyright (c) 2009 Michael Woerister

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _BOX_2D_WORLD_CALLBACKS_H_
#define _BOX_2D_WORLD_CALLBACKS_H_

class Box2dShapeRef;
class Box2dJointRef;
class Box2dBodyRef;

//=============================================================================
// class Box2dDestructionListener
//=============================================================================
// Joints and shapes are destroyed when their associated
// body is destroyed. Implement this listener so that you
// may nullify references to these joints and shapes.
class Box2dDestructionListener {
public:
    virtual ~Box2dDestructionListener() {}

	// Called when any joint is about to be destroyed due
	// to the destruction of one of its attached bodies.
	virtual void sayGoodbye( Box2dJointRef *joint ) = 0;

	// Called when any shape is about to be destroyed due
	// to the destruction of its parent body.
	virtual void sayGoodbye( Box2dShapeRef *shape ) = 0;
};


//=============================================================================
// class Box2dBoundaryListener
//=============================================================================
// This is called when a body's shape passes outside of the world boundary.
class Box2dBoundaryListener
{
public:
	virtual ~Box2dBoundaryListener() {}

	// This is called for each body that leaves the world boundary.
	// Warning: you can't modify the world inside this callback.
	virtual void violation( Box2dBodyRef* body ) = 0;
};


//=============================================================================
// class Box2dContactFilter
//=============================================================================
// Implement this class to provide collision filtering. In other words, you can 
// implement this class if you want finer control over contact creation.
class Box2dContactFilter {
public:
    virtual ~Box2dContactFilter() {}

    virtual bool shouldCollide( Box2dShapeRef *shape1, 
                                Box2dShapeRef *shape2 ) = 0; 
};

/// The default contact filter.
extern const Box2dContactFilter *Box2dDefaultFilter;


//=============================================================================
// class Box2dContactPoint
//=============================================================================
// This structure is used to report contact points.
struct Box2dContactPoint 
{
    // the first shape
    Box2dShapeRef* shape1;  

    // the second shape
	Box2dShapeRef* shape2;	

    // position in world coordinates
	b2Vec2 position;        

    // velocity of point on body2 relative to point on body1 (pre-solver)
	b2Vec2 velocity;

    // points from shape1 to shape2
	b2Vec2 normal;

    // the separation is negative when shapes are touching
	float32 separation;

    // the combined friction coefficient
	float32 friction;

    // the combined restitution coefficient
	float32 restitution;    

    // the contact id identifies the features in contact
	b2ContactID id;         
};


//=============================================================================
// class Box2dContactResult
//=============================================================================
// This structure is used to report contact point results.
struct Box2dContactResult
{
    // the first shape
	Box2dShapeRef* shape1;  

    // the second shape
	Box2dShapeRef* shape2;	

    // position in world coordinates
	b2Vec2 position;        

    // points from shape1 to shape2
	b2Vec2 normal;          

    // the normal impulse applied to body2
	float32 normalImpulse;  

    // the tangent impulse applied to body2
	float32 tangentImpulse; 

    // the contact id identifies the features in contact
	b2ContactID id;         
};


//=============================================================================
// class Box2dContactListener
//=============================================================================
// Implement this class to get collision results. You can use these results for
// things like sounds and game logic. You can also get contact results by
// traversing the contact lists after the time step. However, you might miss
// some contacts because continuous physics leads to sub-stepping.
// Additionally you may receive multiple callbacks for the same contact in a
// single time step.
// You should strive to make your callbacks efficient because there may be
// many callbacks per time step.
// Warning: The contact separation is the last computed value.
// Warning: You cannot create/destroy Box2D entities inside these callbacks.
class Box2dContactListener
{
public:
	virtual ~Box2dContactListener() {}

	// Called when a contact point is added. This includes the geometry
	// and the forces.
	virtual void add( const Box2dContactPoint* point ) { (void) point; }

	// Called when a contact point persists. This includes the geometry
	// and the forces.
	virtual void persist( const Box2dContactPoint* point ) { (void) point; }

	// Called when a contact point is removed. This includes the last
	// computed geometry and forces.
	virtual void remove( const Box2dContactPoint* point ) { (void) point; }

	// Called after a contact point is solved.
	virtual void result( const Box2dContactResult* point ) { (void) point; }
};

#endif
