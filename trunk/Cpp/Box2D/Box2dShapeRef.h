//=============================================================================
// Box2dShapeRef.h
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

#ifndef _BOX_2D_SHAPE_REF_H_
#define _BOX_2D_SHAPE_REF_H_

#include "Box2d.h"
#include "console/simBase.h"

class Box2dBodyRef;
class t2dSceneObject;

class Box2dShapeRef : public SimObject {
public:

    Box2dShapeRef();
    virtual ~Box2dShapeRef();

    //=========================================================================
    // Box2d interface
    //=========================================================================
	b2ShapeType getType() const;
	bool isSensor() const;
	void setFilterData( const b2FilterData& filter );
	const b2FilterData& getFilterData() const;
	bool testPoint( const b2XForm& xf, const b2Vec2& p ) const;
	bool testSegment( const b2XForm& xf, float32* lambda, b2Vec2* normal, 
                      const b2Segment& segment, float32 maxLambda) const;
    void computeAABB( b2AABB* aabb, const b2XForm& xf ) const;
    void computeSweptAABB( b2AABB* aabb, const b2XForm& xf1, 
        const b2XForm& xf2 ) const;
    void computeMass( b2MassData* massData ) const;
	float32 getSweepRadius() const;
	float32 getFriction() const;
	float32 getRestitution() const;

    Box2dBodyRef * getBody() const;
    Box2dShapeRef *getNext() const;

    void setUserData( void * userData ) { this->mUserData = userData; }
    void *getUserData() const { return this->mUserData; }

    t2dSceneObject * getSceneObject() const;

    void onShapeDestroyedCallback();

    // Returns true if the given t2dSceneObject can be bound to a Box2D shape.
    static bool isSupported( const t2dSceneObject* sceneObject );

    DECLARE_CONOBJECT(Box2dShapeRef);
private:
    typedef SimObject Parent;
    friend class Box2dBodyRef;
    friend class Box2dWorldRef;

    void setBody( Box2dBodyRef * body );
    void setSceneObject( t2dSceneObject * sceneObject );
    void setShape( b2Shape * shape );

    b2Shape *getShape() const { return this->mShape; }
    void destroy();

    Box2dBodyRef   *mBody;
    b2Shape        *mShape;
    t2dSceneObject *mSceneObject;
    void           *mUserData;
};

#endif
