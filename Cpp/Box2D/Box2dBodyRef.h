//=============================================================================
// Box2dBodyRef.h
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

#ifndef _BOX2D_REF_H_
#define _BOX2D_REF_H_

#include "T2D/t2dSceneObject.h"
#include "console/simBase.h"
#include "Box2D.h"

class Box2dShapeRef;
class Box2dWorldRef;
class Box2dJointRef;
class Box2dJointEdgeRef;

//=============================================================================
// class Box2dBodyRef
//=============================================================================
class Box2dBodyRef : public SimObject {
public:

    Box2dBodyRef();
    virtual ~Box2dBodyRef();

    //=========================================================================
    // Box2d interface
    //=========================================================================

    // This method creates a new shape from the t2dSceneObject given as 
    // definition.
    // PRE: - def must not be NULL.
    //      - def must own a valid Box2dShapeBehavior instance
    //      - def can be in the same scenegraph as the body sceneobject but 
    //        does not have to
    // POST: - a new Box2dShapeRef object is returned representing the new 
    //         Box2D shape
    //       - def is in the same scenegraph as the body sceneobject
    //       - def is mounted to the body sceneobject
    //       - def must not manually be deleted or removed from the scene
    Box2dShapeRef * createShape( t2dSceneObject * def );

    // This method destroys a given shape
    // PRE: - shape must be a valid Box2dShapeRef wrapping a still existing 
    //        b2Shape
    // POST: - shape is deleted
    //       - the wrapped b2Shape is destroyed
    //       - the t2dSceneObject representing the shape is deleted
    void destroyShape( Box2dShapeRef * shape );

	void setMass(const b2MassData* massData);
    void setMassFromShapes();

	bool setXForm(const b2Vec2& position, float32 angle);
	const b2XForm& getXForm() const;

	const b2Vec2& getPosition() const;
	float32 getAngle() const;

	const b2Vec2& getWorldCenter() const;
	const b2Vec2& getLocalCenter() const;

	void setLinearVelocity(const b2Vec2& v);
    b2Vec2 getLinearVelocity() const;
    void setAngularVelocity(float32 omega);
    float32 getAngularVelocity() const;

	void applyForce(const b2Vec2& force, const b2Vec2& point);
    void applyTorque(float32 torque);
    void applyImpulse(const b2Vec2& impulse, const b2Vec2& point);

	float32 getMass() const;
    float32 getInertia() const;

	b2Vec2 getWorldPoint(const b2Vec2& localPoint) const;
    b2Vec2 getWorldVector(const b2Vec2& localVector) const;
    b2Vec2 getLocalPoint(const b2Vec2& worldPoint) const;
    b2Vec2 getLocalVector(const b2Vec2& worldVector) const;
    b2Vec2 getLinearVelocityFromWorldPoint(const b2Vec2& worldPoint) const;
	b2Vec2 getLinearVelocityFromLocalPoint(const b2Vec2& localPoint) const;

	bool isBullet() const;
	void setBullet(bool flag);

	bool isStatic() const;
	bool isDynamic() const;
	bool isFrozen() const;

	bool isSleeping() const;
	void allowSleeping(bool flag);
	void wakeUp();
	void putToSleep();

    Box2dShapeRef      *getShapeList();
    Box2dJointEdgeRef  *getJointList();
    Box2dBodyRef       *getNext() const;
    
    void *getUserData() const { return this->mUserData; }
    void  setUserData( void * userData ) { this->mUserData = userData; }

    Box2dWorldRef  *getWorld() const;

    void setAutoSetMassFromShapes( const bool flag );
    bool isAutoSetMassFromShapes() const;

    t2dSceneObject* getSceneObject() const { return mSceneObject; }

    void onShapeDestroyedCallback( Box2dShapeRef * shapeRef );

    // Returns true if the given t2dSceneObject can be represented
    // as a Box2D body.
    static bool isSupported( const t2dSceneObject* sceneObject );

    DECLARE_CONOBJECT(Box2dBodyRef);
private:
    typedef SimObject Parent;

    friend class Box2dWorldRef;

    void updateAutoMass();

    void setBody( b2Body * body );
    void setSceneObject( t2dSceneObject * sceneObject );
    void setWorld( Box2dWorldRef * world );
    void setNext( Box2dBodyRef * bodyRef );

    void destroy();

    b2Body* getBody() const { return mBody; }
    
    b2Body         *mBody;
    t2dSceneObject *mSceneObject;
    Box2dWorldRef  *mWorld; // b2World doesn't have userdata, so store it here
    bool            mAutoSetMassFromShapes;
    bool            mAutoMassDirty;
    Box2dBodyRef   *mNext;
    void*           mUserData;
};

#endif
