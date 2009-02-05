//=============================================================================
// Box2dWorldRef.h
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

#ifndef _BOX_2D_WORLD_REF_H_
#define _BOX_2D_WORLD_REF_H_

#include "console/simBase.h"
#include "T2D/t2dSceneGraph.h"
#include "core/tVector.h"
#include "component/behaviors/behaviorInstance.h"
#include "Box2D.h"
#include "./Box2dWorldCallbacks.h"

class Box2dBodyRef;
class Box2dShapeRef;
class Box2dJointRef;

//=============================================================================
// class Box2dWorldRef
//=============================================================================
class Box2dWorldRef : public SimObject {
public:

    Box2dWorldRef();
    virtual ~Box2dWorldRef();

    virtual bool onAdd();
    virtual void onRemove();
    
    //=========================================================================
    // Box2d interface
    //=========================================================================
    void setDestructionListener( Box2dDestructionListener* listener );
	void setBoundaryListener( Box2dBoundaryListener* listener );

    void setContactFilter( Box2dContactFilter* filter );
	void setContactListener( Box2dContactListener* listener );

    Box2dBodyRef * createBody( t2dSceneObject * def );
    void destroyBody( Box2dBodyRef * bodyRef );

    Box2dJointRef* createJoint( SimObject* def );
    void destroyJoint( Box2dJointRef* joint );

	Box2dBodyRef* getGroundBody();

	void step( float32 timeStep, int32 iterations );
    int32 query( const b2AABB& aabb, Box2dShapeRef** shapes, int32 maxCount );

    Box2dBodyRef* getBodyList();
    Box2dJointRef* getJointList();

    void refilter( Box2dShapeRef* shape );

	void setWarmStarting( bool flag );
	void setPositionCorrection( bool flag );
	void setContinuousPhysics( bool flag );

    void validate();

	int32 getProxyCount() const;
	int32 getPairCount() const;
	int32 getBodyCount() const;
	int32 getJointCount() const;
	int32 getContactCount() const;

	void setGravity( const b2Vec2& gravity );
    b2Vec2 getGravity() const;

    F32  getTickTimeStep() const { return ITickable::smTickSec; }

    DECLARE_CONOBJECT(Box2dWorldRef);
private:
    typedef SimObject Parent;
    typedef Vector<Box2dBodyRef*> BodyVector;

    class InternalDestructionListener : public b2DestructionListener {
    public:
        InternalDestructionListener(): mForwardListener( NULL ) {}
        
        virtual void SayGoodbye(b2Joint* joint);
	    virtual void SayGoodbye(b2Shape* shape);

        void setForwardListener( Box2dDestructionListener *forwardListener );
    private:
        Box2dDestructionListener  *mForwardListener;
    };

    class InternalBoundaryListener : public b2BoundaryListener {
    public:
        InternalBoundaryListener() : mForwardListener( NULL ) {}

        virtual void Violation( b2Body * body );

        void setForwardListener( Box2dBoundaryListener *listener );
    private:
        Box2dBoundaryListener *mForwardListener;
    };

    class InternalContactFilter : public b2ContactFilter {
    public:
        InternalContactFilter(): mForwardContactFilter( NULL ) {}

        virtual bool ShouldCollide( b2Shape *shape1, b2Shape *shape2 );
        
        void setForwardFilter( Box2dContactFilter *filter );
        Box2dContactFilter *getForwardFilter() const;
    private:
        Box2dContactFilter *mForwardContactFilter;
    };

    class InternalContactListener : public b2ContactListener {
    public:
        InternalContactListener(): mForwardListener( NULL ) {}

        virtual void Add(const b2ContactPoint* point);

        virtual void Persist(const b2ContactPoint* point);

	    virtual void Remove(const b2ContactPoint* point);

	    virtual void Result(const b2ContactResult* point);

        void setForwardListener( Box2dContactListener * listener );
    private:
        Box2dContactListener *mForwardListener;
        Box2dContactPoint mContactPoint;
        Box2dContactResult mContactResult;
    };

    void createGroundBody();
    void destroyGroundBody();

    void readJointBaseData( SimObject *def, b2JointDef * jointDef, 
        void * userData );
    Box2dJointRef* createRevoluteJoint( SimObject *def );
    Box2dJointRef* createMouseJoint( SimObject *def );
    Box2dJointRef* createDistanceJoint( SimObject *def );
    Box2dJointRef* createPrismaticJoint( SimObject *def );
    Box2dJointRef* createGearJoint( SimObject *def );

    b2World                        *mWorld;
    InternalDestructionListener     mInternalDestructionListener;
    InternalBoundaryListener        mInternalBoundaryListener;
    InternalContactFilter           mInternalContactFilter;
    InternalContactListener         mInternalContactListener;
    BodyVector                      mBodyRefs;
    Box2dBodyRef                   *mGroundBody;
    b2Vec2                          mGravity;
};

#endif
