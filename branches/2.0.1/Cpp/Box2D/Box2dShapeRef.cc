//=============================================================================
// Box2dShapeRef.cc
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

#include "./Box2dShapeRef.h"
#include "T2D/t2dSceneObject.h"
#include "./Box2dBodyRef.h"
#include "./Box2dUtil.h"

using namespace Box2dUtil;

IMPLEMENT_CONOBJECT(Box2dShapeRef);

//=----------------------------------------------------------------------------
// Box2dShapeRef::Box2dShapeRef()
//=----------------------------------------------------------------------------
Box2dShapeRef::Box2dShapeRef():
    mShape( NULL ),
    mBody( NULL ),
    mSceneObject( NULL ),
    mUserData( NULL )
{
    DEBUG_INC(Box2dShapeRefsCreated);
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::~Box2dShapeRef()
//=----------------------------------------------------------------------------
Box2dShapeRef::~Box2dShapeRef()
{
    DEBUG_INC(Box2dShapeRefsDestroyed);
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getType()
//=----------------------------------------------------------------------------
b2ShapeType Box2dShapeRef::getType() const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::getType() - No b2Shape referenced." );

    return this->mShape->GetType();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getType() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, getType, const char*, 2, 2, "" )
{
    char * returnBuffer = Con::getReturnBuffer( 16 );
    
    switch ( object->getType() )
    {
    case e_circleShape: dStrcpy( returnBuffer, "CIRCLE" ); break;
    case e_polygonShape: dStrcpy( returnBuffer, "POLYGON" ); break;
    case e_unknownShape: dStrcpy( returnBuffer, "UNKNOWN" ); break;
    }

    return returnBuffer;
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::isSensor()
//=----------------------------------------------------------------------------
bool Box2dShapeRef::isSensor() const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::isSensor() - No b2Shape referenced." );

    return this->mShape->IsSensor();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::isSensor() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, isSensor, bool, 2, 2, "" )
{
    return object->isSensor();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::setFilterData()
//=----------------------------------------------------------------------------
void Box2dShapeRef::setFilterData( const b2FilterData& filter )
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::setFilterData() - No b2Shape referenced." );

    return this->mShape->SetFilterData( filter );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::setFilterData() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, setFilterData, void, 5, 5, 
              "(%categoryBits,%maskBits,%groupIndex)" )
{
    b2FilterData filterData;

    filterData.categoryBits = strTo16BitMask( argv[2] );
    filterData.maskBits = strTo16BitMask( argv[3] );
    filterData.groupIndex = dAtoi( argv[4] );
    
    object->setFilterData( filterData );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getFilterData()
//=----------------------------------------------------------------------------
const b2FilterData& Box2dShapeRef::getFilterData() const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::getFilterData() - No b2Shape referenced." );

    return this->mShape->GetFilterData();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getFilterData() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, getFilterData, const char*, 2, 2, 
              "returns \"categoryBits maskBits groupIndex\"" )
{
    const b2FilterData& filterData = object->getFilterData();
    char * returnBuffer = Con::getReturnBuffer( 32 );
    dSprintf( returnBuffer, 32, "%i %i %i", 
        (int)( filterData.categoryBits ),
        (int)( filterData.maskBits ),
        (int)( filterData.groupIndex )
        );

    return returnBuffer;
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::testPoint()
//=----------------------------------------------------------------------------
bool Box2dShapeRef::testPoint(const b2XForm& xf, const b2Vec2& p) const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::testPoint() - No b2Shape referenced." );

    return this->mShape->TestPoint( xf, p );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::testPoint() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, testPoint, bool, 2, 2, "not yet implemented" )
{
    AssertFatal( false, "Box2dShapeRef::testPoint() - Not yet implemented." );
    return false;
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::testSegment()
//=----------------------------------------------------------------------------
bool Box2dShapeRef::testSegment( const b2XForm& xf, float32* lambda, 
    b2Vec2* normal, const b2Segment& segment, float32 maxLambda) const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::testSegment() - No b2Shape referenced." );

    return this->mShape->TestSegment( xf, lambda, normal, segment, maxLambda );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::testSegment() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, testSegment, bool, 2, 2, "not yet implemented" )
{
    AssertFatal( false, 
        "Box2dShapeRef::testSegment() - Not yet implemented." );
    return false;
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::computeAABB()
//=----------------------------------------------------------------------------
void Box2dShapeRef::computeAABB( b2AABB* aabb, const b2XForm& xf ) const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::computeAABB() - No b2Shape referenced." );

    this->mShape->ComputeAABB( aabb, xf );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::computeAABB() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, computeAABB, void, 2, 2, "not yet implemented" )
{
    AssertFatal( false, 
        "Box2dShapeRef::computeAABB() - Not yet implemented." );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::computeSweptAABB()
//=----------------------------------------------------------------------------
void Box2dShapeRef::computeSweptAABB( b2AABB* aabb, 
    const b2XForm& xf1, const b2XForm& xf2 ) const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::computeSweptAABB() - No b2Shape referenced." );

    this->mShape->ComputeSweptAABB( aabb, xf1, xf2 );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::computeSweptAABB() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, computeSweptAABB, void, 2, 2, 
              "not yet implemented" )
{
    AssertFatal( false, 
        "Box2dShapeRef::computeSweptAABB() - Not yet implemented." );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::computeMass()
//=----------------------------------------------------------------------------
void Box2dShapeRef::computeMass( b2MassData* massData ) const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::computeMass() - No b2Shape referenced." );

    this->mShape->ComputeMass( massData );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::computeMass() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, computeMass, void, 2, 2, "not yet implemented" )
{
    AssertFatal( false, 
        "Box2dShapeRef::computeMass() - Not yet implemented." );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getSweepRadius()
//=----------------------------------------------------------------------------
float32 Box2dShapeRef::getSweepRadius() const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::getSweepRadius() - No b2Shape referenced." );

    return this->mShape->GetSweepRadius();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getSweepRadius() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, getSweepRadius, F32, 2, 2, "" )
{
    return object->getSweepRadius();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getFriction()
//=----------------------------------------------------------------------------
float32 Box2dShapeRef::getFriction() const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::getFriction() - No b2Shape referenced." );

    return this->mShape->GetFriction();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getFriction() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, getFriction, F32, 2, 2, "" )
{
    return object->getFriction();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getRestitution()
//=----------------------------------------------------------------------------
float32 Box2dShapeRef::getRestitution() const
{
    AssertFatal( this->mShape != NULL,
        "Box2dShapeRef::getRestitution() - No b2Shape referenced." );

    return this->mShape->GetRestitution();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getRestitution() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, getRestitution, F32, 2, 2, "" )
{
    return object->getRestitution();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getBody()
//=----------------------------------------------------------------------------
Box2dBodyRef * Box2dShapeRef::getBody() const
{ 
    AssertFatal( this->mBody == this->mShape->GetBody()->GetUserData(), 
        "Box2dShapeRef::getBody() - Owner mismatch." );

    return this->mBody;
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getBody() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, getBody, S32, 2, 2, "" )
{ 
    return object->getBody() ? object->getBody()->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getNext()
//=----------------------------------------------------------------------------
Box2dShapeRef *Box2dShapeRef::getNext() const
{
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::getNext() - No b2Shape referenced." );

    b2Shape * next = this->mShape->GetNext();

    return ( next ? 
        static_cast<Box2dShapeRef*>( next->GetUserData() ) : NULL );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getNext() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dShapeRef, getNext, S32, 2, 2, "" )
{ 
    return object->getNext() ? object->getNext()->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::getSceneObject()
//=----------------------------------------------------------------------------
t2dSceneObject * Box2dShapeRef::getSceneObject() const
{
    return this->mSceneObject;
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::onShapeDestroyedCallback()
//=----------------------------------------------------------------------------
void Box2dShapeRef::onShapeDestroyedCallback()
{
    this->destroy();
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::isSupported()
//=----------------------------------------------------------------------------
bool Box2dShapeRef::isSupported( const t2dSceneObject* sceneObject )
{
    return Box2dBodyRef::isSupported( sceneObject );
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::setBody()
//=----------------------------------------------------------------------------
void Box2dShapeRef::setBody( Box2dBodyRef * body )
{
    this->mBody = body;
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::setSceneObject()
//=----------------------------------------------------------------------------
void Box2dShapeRef::setSceneObject( t2dSceneObject * sceneObject )
{
    AssertFatal( this->mSceneObject == NULL, "" );

    this->mSceneObject = sceneObject;
}

//=----------------------------------------------------------------------------
// Box2dShapeRef::setShape()
//=----------------------------------------------------------------------------
void Box2dShapeRef::setShape( b2Shape * shape )
{
    AssertFatal( shape->GetUserData() == this, 
        "Box2dShapeRef::setShape() - Shape/Proxy mismatch." );
    AssertFatal( this->mShape == NULL, 
        "Box2dShapeRef::setShape() - Shape assigned twice." );

    this->mShape = shape;
}


//=----------------------------------------------------------------------------
// Box2dShapeRef::destroy()
//=----------------------------------------------------------------------------
void Box2dShapeRef::destroy()
{
    AssertFatal( this->mSceneObject != NULL, 
        "Box2dShapeRef::destroy() - No SceneObject" );
    AssertFatal( this->mShape != NULL, 
        "Box2dShapeRef::destroy() - No b2Shape" );

    if ( this->mBody->getSceneObject() != this->mSceneObject )
    {
        this->mSceneObject->setDataField( 
            StringTable->insert( "box2d_can_be_removed" ), NULL, "1" 
            );

        this->mSceneObject->safeDelete();
        this->mSceneObject = NULL;
    }

    this->mShape = NULL;
    
    this->deleteObject();
}
