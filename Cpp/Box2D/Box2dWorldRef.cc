//=============================================================================
// Box2dWorldRef.cc
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

#include "./Box2dWorldRef.h"
#include "./Box2dBodyRef.h"
#include "./Box2dUtil.h"
#include "./Box2dShapeRef.h"
#include "./Box2dWorldCallbacks.h"
#include "./Joints/Box2dJointRef.h"
#include "./Joints/Box2dRevoluteJointRef.h"
#include "./Joints/Box2dMouseJointRef.h"
#include "./Joints/Box2dDistanceJointRef.h"
#include "./Joints/Box2dPrismaticJointRef.h"
#include "./Joints/Box2dGearJointRef.h"
#include "./Joints/Box2dPulleyJointRef.h"
#include "core/tVector.h"
#include "core/frameAllocator.h"

IMPLEMENT_CONOBJECT(Box2dWorldRef);

using namespace Box2dUtil;

namespace
{

//=----------------------------------------------------------------------------
// notEmpty()
//=----------------------------------------------------------------------------
inline bool notEmpty( const char * value )
{
    return value && (*value);
}

//=============================================================================
// class DummyDefaultContactFilter
//=============================================================================
class DummyDefaultContactFilter : public Box2dContactFilter {
public:
    virtual ~DummyDefaultContactFilter() {}

    virtual bool shouldCollide( Box2dShapeRef *, Box2dShapeRef* )
    {
        return false;
    }
} staticDummyDefaultContactFilter;

}

const Box2dContactFilter *Box2dDefaultFilter = 
	&staticDummyDefaultContactFilter;


//=----------------------------------------------------------------------------
// Box2dWorldRef::Box2dWorldRef()
//=----------------------------------------------------------------------------
Box2dWorldRef::Box2dWorldRef():
    mWorld( NULL ),
    mGroundBody( NULL )
{
#ifdef TORQUE_DEBUG
    Box2dWorldRefsCreated++;
#endif
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::~Box2dWorldRef()
//=----------------------------------------------------------------------------
Box2dWorldRef::~Box2dWorldRef()
{
    #ifdef TORQUE_DEBUG
    Box2dWorldRefsDestroyed++;
    #endif
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::onAdd()
//=----------------------------------------------------------------------------
bool Box2dWorldRef::onAdd()
{
    if ( !Parent::onAdd() )
    {
        return false;
    }

    StringTableEntry stSceneGraph = StringTable->insert( "SceneGraph" );

    t2dSceneGraph *sceneGraph = dynamic_cast<t2dSceneGraph*>(
        Sim::findObject( this->getDataField( stSceneGraph, NULL ) )
        );

    AssertFatal( sceneGraph, "Box2dWorldRef::onAdd() - Invalid SceneGraph." );

    if ( !sceneGraph )
    {
        Con::errorf( "Box2dWorldRef::onAdd() - Invalid SceneGraph." );
        return false;
    }

    this->mSceneGraph = sceneGraph;

    b2AABB worldBounds;
    worldBounds.lowerBound.Set( -500.0f, -500.0f );
    worldBounds.upperBound.Set(  500.0f,  500.0f );
    this->mGravity = b2Vec2( 0.0f, 0.0f );

    const char * gravityString = 
        this->getDataField( StringTable->insert( "Gravity" ), NULL );

    if ( notEmpty( gravityString ) )
    {
        this->mGravity = Box2dUtil::strTob2Vec2( gravityString );
    }

    this->mWorld = new b2World( worldBounds, this->mGravity, true );
    this->mWorld->SetDestructionListener( 
        &(this->mInternalDestructionListener) 
        );

    this->createGroundBody();

    return true;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::onRemove()
//=----------------------------------------------------------------------------
void Box2dWorldRef::onRemove()
{
    while ( this->mBodyRefs.size() > 1 )
    {
        this->destroyBody( this->mBodyRefs.last() );
    }

    AssertFatal( this->mBodyRefs[0] == this->mGroundBody, "" );

    this->destroyGroundBody();

    delete this->mWorld;
    this->mWorld = NULL;

    Parent::onRemove();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::setDestructionListener()
//=----------------------------------------------------------------------------
void Box2dWorldRef::setDestructionListener( Box2dDestructionListener* listener )
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::setDestructionListener() - No b2World is referenced" );

    this->mInternalDestructionListener.setForwardListener( listener );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::setBoundaryListener()
//=----------------------------------------------------------------------------
void Box2dWorldRef::setBoundaryListener( Box2dBoundaryListener* listener )
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::setBoundaryListener() - No b2World is referenced" );

    if ( listener == NULL )
    {
        this->mWorld->SetBoundaryListener( NULL );
        this->mInternalBoundaryListener.setForwardListener( NULL );
    }
    else
    {
        this->mWorld->SetBoundaryListener( 
            &( this->mInternalBoundaryListener ) 
            );

        this->mInternalBoundaryListener.setForwardListener( listener );
    }
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::setContactFilter()
//=----------------------------------------------------------------------------
void Box2dWorldRef::setContactFilter( Box2dContactFilter* filter )
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::setContactFilter() - No b2World is referenced" );

    if ( filter == NULL )
    {
        this->mInternalContactFilter.setForwardFilter( NULL );
        this->mWorld->SetContactFilter( NULL );
    }
    else if ( filter == Box2dDefaultFilter )
    {
        this->mInternalContactFilter.setForwardFilter( NULL );
        this->mWorld->SetContactFilter( &b2_defaultFilter );
    }
    else
    {
        this->mInternalContactFilter.setForwardFilter( filter );
        this->mWorld->SetContactFilter( &( this->mInternalContactFilter ) );
    }
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::setContactListener()
//=----------------------------------------------------------------------------
void Box2dWorldRef::setContactListener( Box2dContactListener* listener )
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::setContactListener() - No b2World is referenced" );

    if ( listener == NULL )
    {
        this->mWorld->SetContactListener( NULL );
        this->mInternalContactListener.setForwardListener( NULL );
    }
    else
    {
        this->mWorld->SetContactListener( 
            &( this->mInternalContactListener ) 
            );

        this->mInternalContactListener.setForwardListener( listener );
    }
    
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createBody()
//=----------------------------------------------------------------------------
Box2dBodyRef * Box2dWorldRef::createBody( t2dSceneObject * def )
{
    static const StringTableEntry stBodyRef = StringTable->insert( "bodyRef" );
   
    AssertFatal( def != NULL, 
        "Box2dWorldRef::createBody() - No def object." );
    AssertFatal( Box2dBodyRef::isSupported( def ),
        "Box2dWorldRef::createBody() - This t2dSceneObject is not supported" );

    BehaviorInstance * bodyBehavior = 
        def->getBehavior( StringTable->insert( "Box2dBodyBehavior" ) );

    AssertFatal( bodyBehavior != NULL,
        "Box2dWorldRef::createBody() - \
        def own an instance of Box2dBodyBehavior." );

    AssertFatal( !Sim::findObject( def->getDataField( stBodyRef, NULL ) ),
        "Box2dWorldRef::createBody() - def object already has a BoxBodyRef" );

    AssertFatal( ( def->getSceneGraph() == NULL ) || 
        ( def->getSceneGraph() == this->mSceneGraph ), 
        "Box2dWorldRef::createBody() - def object contained in wrong scene" );

    Box2dBodyRef * bodyRef = new Box2dBodyRef();

    if ( ! bodyRef->registerObject() ) {
        AssertFatal( false,
            "Box2dWorldRef::createBody() - Couldn't register Box2dBodyRef" );

        delete bodyRef;
        return NULL;
    }

    if ( def->getSceneGraph() == NULL )
    {
        this->mSceneGraph->addToScene( def );
    }

    b2BodyDef bodyDef;

    const char *massString =  
        bodyBehavior->getDataField( StringTable->insert( "Mass" ), NULL );

    const char *inertiaString = 
        bodyBehavior->getDataField( StringTable->insert( "Inertia" ), NULL );

    const char *centerOfMassString = bodyBehavior->getDataField( 
        StringTable->insert( "CenterOfMass" ), 
        NULL 
        );

    const char *linearDampingString = bodyBehavior->getDataField( 
        StringTable->insert( "LinearDamping" ), 
        NULL 
        );

    const char *angularDampingString = bodyBehavior->getDataField( 
        StringTable->insert( "AngularDamping" ), 
        NULL 
        );

    const char *allowSleepingString = bodyBehavior->getDataField( 
        StringTable->insert( "AllowSleeping" ), 
        NULL 
        );

    const char *isSleepingString = bodyBehavior->getDataField( 
        StringTable->insert( "IsSleeping" ), 
        NULL 
        );

    const char *fixedRotationString = bodyBehavior->getDataField(
        StringTable->insert( "FixedRotation" ), 
        NULL 
        );

    const char *isBulletString = bodyBehavior->getDataField(
        StringTable->insert( "IsBullet" ),
        NULL
        );

    const char *autoSetMassFromShapes = bodyBehavior->getDataField( 
        StringTable->insert( "AutoMass" ), 
        NULL 
        );
    
    bodyDef.massData.mass = strToFloat( massString );
    bodyDef.massData.I = strToFloat( inertiaString );
    bodyDef.massData.center = strTob2Vec2( centerOfMassString );
    bodyDef.linearDamping = strToFloat( linearDampingString );
    bodyDef.angularDamping = strToFloat( angularDampingString );
    bodyDef.allowSleep = dAtob( allowSleepingString );
    bodyDef.isSleeping = dAtob( isSleepingString );
    bodyDef.fixedRotation = dAtob( fixedRotationString );
    bodyDef.isBullet = dAtob( isBulletString );

    t2dSceneObject * sceneObject = def;

    bodyDef.position.x = sceneObject->getPosition().mX;
    bodyDef.position.y = sceneObject->getPosition().mY;
    bodyDef.angle = mDegToRad( sceneObject->getRotation() );

    bodyDef.userData = bodyRef;

    b2Body * body = this->mWorld->CreateBody( &bodyDef );

    bodyRef->setBody( body );
    bodyRef->setSceneObject( sceneObject );
    bodyRef->setWorld( this );
    
    if ( ! this->mBodyRefs.empty() )
    {
        this->mBodyRefs.last()->setNext( bodyRef );
    }

    this->mBodyRefs.push_back( bodyRef );

    bodyRef->setAutoSetMassFromShapes( dAtob( autoSetMassFromShapes ) );

    sceneObject->setDataField( 
        stBodyRef, 
        NULL, 
        bodyRef->getIdString() 
        );

    return bodyRef;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createBody() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dWorldRef, createBody, S32, 3, 3, "" )
{
    t2dSceneObject *def = 
        dynamic_cast<t2dSceneObject*>( Sim::findObject( argv[2] ) );
    AssertFatal( def != NULL, 
        "Box2dWorldRef::createBodyRef() - Invalid Def Object" );

    Box2dBodyRef * bodyRef = object->createBody( def );
    AssertFatal( bodyRef != NULL, 
        "Error creating Box2dBodyRef. Debug me." );

    return bodyRef->getId();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::destroyBody( Box2dBodyRef * bodyRef )
//=----------------------------------------------------------------------------
void Box2dWorldRef::destroyBody( Box2dBodyRef * bodyRef )
{
    AssertFatal( bodyRef != NULL, 
        "Box2dWorldRef::destroyBody() - bodyRef must not be NULL" );

    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::destroyBody() - No b2World is referenced" );

    AssertFatal( bodyRef->getWorld() == this, 
        "Box2dWorldRef::destroyBody() - \
        bodyRef is not contained in this world." 
        );

    b2Body *body = bodyRef->getBody();

    AssertFatal( body != NULL, 
        "Box2dWorldRef::destroyBody() - body must not be NULL" );
    
    AssertFatal( 
        find( mBodyRefs.begin(), mBodyRefs.end(), bodyRef ) != mBodyRefs.end(),
        "Box2dWorldRef::destroyBody() - bodyRef not contained in list" 
        );
    
    for ( int i = 0; i < this->mBodyRefs.size(); ++i )
    {
        if ( this->mBodyRefs[i] == bodyRef )
        {
            if ( i > 0 )
            {
                Box2dBodyRef * prev = this->mBodyRefs[i-1];
                prev->setNext( bodyRef->getNext() );
            }

            this->mBodyRefs.erase( i );
            break;
        }
    }

    this->mWorld->DestroyBody( body );

    bodyRef->destroy();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::destroyBody() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dWorldRef, destroyBody, void, 3, 3, "" )
{
    Box2dBodyRef * bodyRef = 
        dynamic_cast<Box2dBodyRef*>( Sim::findObject( argv[2] ) );
    object->destroyBody( bodyRef );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createJoint()
//=----------------------------------------------------------------------------
Box2dJointRef* Box2dWorldRef::createJoint( SimObject* def )
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::createJoint() - No b2World is referenced" );
    AssertFatal( def != NULL, 
        "Box2dWorldRef::createJoint() - def must not be null" );

    static const StringTableEntry stJointType = 
        StringTable->insert( "jointType" );
    
    const b2JointType jointType = Box2dJointRef::getJointTypeFromName( 
        def->getDataField( stJointType, NULL ) 
        );

    switch ( jointType )
    {
    case e_revoluteJoint: return this->createRevoluteJoint( def );
    case e_mouseJoint: return this->createMouseJoint( def );
    case e_distanceJoint: return this->createDistanceJoint( def );
    case e_prismaticJoint: return this->createPrismaticJoint( def );
    case e_gearJoint: return this->createGearJoint( def );
    case e_pulleyJoint: return this->createPulleyJoint( def );
    
    default: Con::errorf( 
                 "Box2dWorldRef::createJoint() - Unknown JointType %s.", 
                 def->getDataField( stJointType, NULL ) 
                 );
    }

    return NULL;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createJoint() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dWorldRef, createJoint, S32, 3, 3, "" )
{
    SimObject * def = Sim::findObject( argv[2] );
    
    Box2dJointRef *jointRef = object->createJoint( def );

    return jointRef ? jointRef->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::destroyJoint()
//=----------------------------------------------------------------------------
void Box2dWorldRef::destroyJoint( Box2dJointRef* jointRef )
{
    AssertFatal( 
        this->mWorld, 
        "Box2dWorldRef::destroyJoint() - No b2World is referenced" 
        );

    AssertFatal( 
        jointRef, 
        "Box2dWorldRef::destroyJoint() - jointRef must not be NULL" 
        );
    
    b2Joint * joint = jointRef->getJoint();

    this->mWorld->DestroyJoint( joint );
    
    jointRef->destroy();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::destroyJoint() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dWorldRef, destroyJoint, void, 3, 3, "" )
{
    Box2dJointRef *jointRef = dynamic_cast<Box2dJointRef*>( 
        Sim::findObject( argv[2] ) 
        );

    if ( !jointRef )
    {
        Con::errorf( 
            "Box2dWorldRef::destroyJoint() - %s is not a valid Box2dJointRef.",
            argv[2] 
        );
    }
    
    object->destroyJoint( jointRef );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getGroundBody()
//=----------------------------------------------------------------------------
Box2dBodyRef* Box2dWorldRef::getGroundBody()
{
    return this->mGroundBody;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getGroundBody() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dWorldRef, getGroundBody, S32, 2, 2, "" )
{
    return object->getGroundBody()->getId();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::step()
//=----------------------------------------------------------------------------
void Box2dWorldRef::step( float32 timeStep, int32 iterations )
{
    for ( BodyVector::iterator it = this->mBodyRefs.begin(); 
          it != this->mBodyRefs.end(); ++it ) 
    {
        (*it)->updateAutoMass();
    }

    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::step() - No b2World is referenced" );

    this->mWorld->Step( timeStep, iterations );

    for ( BodyVector::iterator it = this->mBodyRefs.begin(); 
          it != this->mBodyRefs.end(); ++it ) 
    {
        const Box2dBodyRef * const body = *it;

        if ( !body->isSleeping() && !body->isStatic() )
        {
            AssertFatal( body->getBody()->GetUserData() == body, 
                "Box2dWorldRef::step() - Invalid Box2dBodyRef in list." );

            t2dSceneObject * sceneObject = body->getSceneObject();
            AssertFatal( sceneObject != NULL || body == this->mGroundBody, 
                "Box2dWorldRef::step() - \
                 Box2dBodyRef without sceneobject in list." 
                 );

            if ( sceneObject ) 
            {
                const b2Vec2 position = body->getPosition();
                const t2dVector pos2( position.x, position.y );

                if ( pos2.notEqual( sceneObject->getPosition() ) )
                {
                    const F32 linearSpeed = 
                        (pos2 - sceneObject->getPosition()).len() / 
                        ITickable::smTickSec;

                    sceneObject->moveTo( 
                        pos2,
                        linearSpeed,
                        true,
                        false,
                        false,
                        0.01f
                        );
                }

                const F32 rotation = mRadToDeg( body->getAngle() );

                if ( mNotEqual( rotation, sceneObject->getRotation() ) )
                {
                    sceneObject->setRotation( rotation );
                }
            }
        }
    }
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::step() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dWorldRef, step, void, 4, 4, "" )
{
    const float32 timeStep = strToFloat( argv[2] );
    const int32 iterations = dAtoi( argv[3] );

    AssertFatal( 
        iterations > 0, 
        "Box2dWorldRef::step() - Invalid iteration count." 
        );

    object->step( timeStep, iterations );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::query()
//=----------------------------------------------------------------------------
int32 Box2dWorldRef::query( const b2AABB& aabb, Box2dShapeRef **shapes, 
                            int32 maxCount )
{
    FrameTemp<b2Shape*> shapeTemp( maxCount );
    
    const int32 count = this->mWorld->Query( aabb, shapeTemp, maxCount );

    for ( int i = 0; i < count; ++i )
    {
        shapes[i] = static_cast<Box2dShapeRef*>( shapeTemp[i]->GetUserData() );
    }

    return count;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getBodyList()
//=----------------------------------------------------------------------------
Box2dBodyRef* Box2dWorldRef::getBodyList()
{
    return this->mBodyRefs.size() != 0 ? this->mBodyRefs[0] : NULL;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getJointList()
//=----------------------------------------------------------------------------
Box2dJointRef* Box2dWorldRef::getJointList()
{
    b2Joint* internalList = this->mWorld->GetJointList();
    return internalList != NULL ? 
        static_cast<Box2dJointRef*>( internalList->GetUserData() ) : NULL;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::refilter()
//=----------------------------------------------------------------------------
void Box2dWorldRef::refilter( Box2dShapeRef* shape )
{
    this->mWorld->Refilter( shape->getShape() );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::refilter() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dWorldRef, refilter, void, 3, 3, "" )
{
    Box2dShapeRef * shapeRef = dynamic_cast<Box2dShapeRef*>( 
        Sim::findObject( argv[2] ) 
        );

    if ( !shapeRef )
    {
        Con::errorf( 
            "Box2dWorldRef::refilter() - %s is not a valid Box2dShapeRef.", 
            argv[2] 
            );
    }

    object->refilter( shapeRef );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::setWarmStarting()
//=----------------------------------------------------------------------------
void Box2dWorldRef::setWarmStarting( bool flag )
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::setWarmStarting() - No b2World is referenced" );

    this->mWorld->SetWarmStarting( flag );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::setPositionCorrection()
//=----------------------------------------------------------------------------
void Box2dWorldRef::setPositionCorrection( bool flag ) 
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::setPositionCorrection() - No b2World is referenced" );

    this->mWorld->SetPositionCorrection( flag );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::setContinuousPhysics()
//=----------------------------------------------------------------------------
void Box2dWorldRef::setContinuousPhysics( bool flag )
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::setContinuousPhysics() - No b2World is referenced" );

    this->mWorld->SetContinuousPhysics( flag );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::validate()
//=----------------------------------------------------------------------------
void Box2dWorldRef::validate()
{
    AssertFatal( 
        this->mWorld != NULL, 
        "Box2dWorldRef::validate() - No b2World is referenced" 
        );

    this->mWorld->Validate();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getProxyCount()
//=----------------------------------------------------------------------------
int32 Box2dWorldRef::getProxyCount() const
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::getProxyCount() - No b2World is referenced" );

    return this->mWorld->GetProxyCount();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getPairCount()
//=----------------------------------------------------------------------------
int32 Box2dWorldRef::getPairCount() const
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::getPairCount() - No b2World is referenced" );

    return this->mWorld->GetPairCount();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getBodyCount()
//=----------------------------------------------------------------------------
int32 Box2dWorldRef::getBodyCount() const
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::getBodyCount() - No b2World is referenced" );

    return this->mWorld->GetBodyCount();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getJointCount()
//=----------------------------------------------------------------------------
int32 Box2dWorldRef::getJointCount() const
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::getJointCount() - No b2World is referenced" );

    return this->mWorld->GetJointCount();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getContactCount()
//=----------------------------------------------------------------------------
int32 Box2dWorldRef::getContactCount() const
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::getContactCount() - No b2World is referenced" );

    return this->mWorld->GetContactCount();
}

	
//=----------------------------------------------------------------------------
// Box2dWorldRef::setGravity()
//=----------------------------------------------------------------------------
void Box2dWorldRef::setGravity(const b2Vec2& gravity)
{
    AssertFatal( this->mWorld != NULL, 
        "Box2dWorldRef::setGravity() - No b2World is referenced" );

    this->mGravity = gravity;
    this->mWorld->SetGravity( gravity );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::setGravity() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dWorldRef, setGravity, void, 3, 3, "" )
{
    object->setGravity( strTob2Vec2( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getGravity()
//=----------------------------------------------------------------------------
b2Vec2 Box2dWorldRef::getGravity() const
{
    return this->mGravity;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getGravity() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dWorldRef, getGravity, const char*, 2, 2, "" )
{
    return b2Vec2ToReturnBuffer( object->getGravity() );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::getTickTimeStep() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dWorldRef, getTickTimeStep, F32, 2, 2, 
              "Returns the time one tick takes in seconds." )
{
    return object->getTickTimeStep();
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::InternalDestructionListener::sayGoodBye( b2Joint )
//=----------------------------------------------------------------------------
void Box2dWorldRef::InternalDestructionListener::SayGoodbye( b2Joint *joint )
{
    Box2dJointRef *jointRef = static_cast<Box2dJointRef*>( 
        joint->GetUserData() 
        );

    if ( this->mForwardListener )
    {
        this->mForwardListener->sayGoodbye( jointRef );
    }
    
    jointRef->onJointDestroyedCallback();

    joint->SetUserData( NULL );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::InternalDestructionListener::sayGoodBye( b2Shape )
//=----------------------------------------------------------------------------
void Box2dWorldRef::InternalDestructionListener::SayGoodbye( b2Shape *shape )
{
    Box2dShapeRef *shapeRef = static_cast<Box2dShapeRef*>( 
        shape->GetUserData() 
        );

    if ( this->mForwardListener )
    {
        this->mForwardListener->sayGoodbye( shapeRef );
    }

    shapeRef->onShapeDestroyedCallback();

    shape->SetUserData( NULL );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::InternalDestructionListener::setForwardListener()
//=----------------------------------------------------------------------------
void Box2dWorldRef::InternalDestructionListener::setForwardListener(
    Box2dDestructionListener *forwardListener )
{
    this->mForwardListener = forwardListener;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::InternalBoundaryListener::Violation()
//=----------------------------------------------------------------------------
void Box2dWorldRef::InternalBoundaryListener::Violation( b2Body * body )
{
    Box2dBodyRef *const bodyRef = 
        static_cast<Box2dBodyRef*>( body->GetUserData() );

    this->mForwardListener->violation( bodyRef );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::InternalBoundaryListener::setForwardListener()
//=----------------------------------------------------------------------------
void Box2dWorldRef::InternalBoundaryListener::setForwardListener( 
    Box2dBoundaryListener *listener )
{
    this->mForwardListener = listener;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::InternalContactFilter::ShouldCollide()
//=----------------------------------------------------------------------------
bool Box2dWorldRef::InternalContactFilter::ShouldCollide( b2Shape *shape1, 
                                                          b2Shape *shape2 )
{
    Box2dShapeRef * const shapeRef1 = 
        static_cast<Box2dShapeRef*>( shape1->GetUserData() );

    Box2dShapeRef * const shapeRef2 = 
        static_cast<Box2dShapeRef*>( shape2->GetUserData() );

    return this->mForwardContactFilter->shouldCollide( shapeRef1, shapeRef2 );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::InternalContactFilter::setForwardFilter()
//=----------------------------------------------------------------------------
void Box2dWorldRef::InternalContactFilter::setForwardFilter( 
    Box2dContactFilter *filter )
{
    this->mForwardContactFilter = filter;
}

        
//=----------------------------------------------------------------------------
// Box2dWorldRef::InternalContactFilter::getForwardFilter()
//=----------------------------------------------------------------------------
Box2dContactFilter *Box2dWorldRef::InternalContactFilter::getForwardFilter()
    const
{
    return this->mForwardContactFilter;
}


void Box2dWorldRef::InternalContactListener::Add( const b2ContactPoint* point )
{
    this->mContactPoint.id = point->id;
    this->mContactPoint.friction = point->friction;
    this->mContactPoint.normal = point->normal;
    this->mContactPoint.position = point->position;
    this->mContactPoint.restitution = point->restitution;
    this->mContactPoint.separation = point->separation;
    this->mContactPoint.velocity = point->velocity;

    // This could be faster but may be less portable?
    //dMemcpy( &( this->mContactPoint ), point, sizeof( b2ContactPoint ) );

    this->mContactPoint.shape1 = 
        static_cast<Box2dShapeRef*>( point->shape1->GetUserData() );

    this->mContactPoint.shape2 = 
        static_cast<Box2dShapeRef*>( point->shape2->GetUserData() );

    this->mForwardListener->add( &( this->mContactPoint ) );
}
 
void Box2dWorldRef::InternalContactListener::Persist(
    const b2ContactPoint* point )
{
    this->mContactPoint.id = point->id;
    this->mContactPoint.friction = point->friction;
    this->mContactPoint.normal = point->normal;
    this->mContactPoint.position = point->position;
    this->mContactPoint.restitution = point->restitution;
    this->mContactPoint.separation = point->separation;
    this->mContactPoint.velocity = point->velocity;

    // This could be faster but may be less portable?
    //dMemcpy( &( this->mContactPoint ), point, sizeof( b2ContactPoint ) );

    this->mContactPoint.shape1 = 
        static_cast<Box2dShapeRef*>( point->shape1->GetUserData() );

    this->mContactPoint.shape2 = 
        static_cast<Box2dShapeRef*>( point->shape2->GetUserData() );

    this->mForwardListener->persist( &( this->mContactPoint ) );
}

void Box2dWorldRef::InternalContactListener::Remove( 
    const b2ContactPoint* point )
{
    this->mContactPoint.id = point->id;
    this->mContactPoint.friction = point->friction;
    this->mContactPoint.normal = point->normal;
    this->mContactPoint.position = point->position;
    this->mContactPoint.restitution = point->restitution;
    this->mContactPoint.separation = point->separation;
    this->mContactPoint.velocity = point->velocity;

    // This could be faster but may be less portable?
    //dMemcpy( &( this->mContactPoint ), point, sizeof( b2ContactPoint ) );

    this->mContactPoint.shape1 = 
        static_cast<Box2dShapeRef*>( point->shape1->GetUserData() );

    this->mContactPoint.shape2 = 
        static_cast<Box2dShapeRef*>( point->shape2->GetUserData() );

    this->mForwardListener->remove( &( this->mContactPoint ) );
}

void Box2dWorldRef::InternalContactListener::Result(
    const b2ContactResult* point )
{
    this->mContactResult.id = point->id;
    this->mContactResult.normal  = point->normal;
    this->mContactResult.normalImpulse = point->normalImpulse;
    this->mContactResult.position = point->position;
    this->mContactResult.tangentImpulse = point->tangentImpulse;
    
    // This could be faster but may be less portable?
    // dMemcpy( &( this->mContactResult ), point, sizeof( b2ContactResult ) );

    this->mContactResult.shape1 = 
        static_cast<Box2dShapeRef*>( point->shape1->GetUserData() );

    this->mContactResult.shape2 = 
        static_cast<Box2dShapeRef*>( point->shape2->GetUserData() );

    this->mForwardListener->result( &( this->mContactResult ) );
}

void Box2dWorldRef::InternalContactListener::setForwardListener( 
    Box2dContactListener * listener )
{
    this->mForwardListener = listener;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createGroundBody()
//=----------------------------------------------------------------------------
void Box2dWorldRef::createGroundBody()
{
    AssertFatal( this->mWorld != NULL, "" );

    Box2dBodyRef * bodyRef = new Box2dBodyRef();

    if ( !bodyRef->registerObject() )
    {
        AssertFatal(
            false, 
            "Box2dWorldRef::createGroundBody() - \
            Couldn't register Box2dBodyRef." 
            );

        return;
    }

    bodyRef->setBody( this->mWorld->GetGroundBody() );
    bodyRef->setSceneObject( NULL );
    bodyRef->setWorld( this );

    this->mWorld->GetGroundBody()->SetUserData( bodyRef );
    this->mGroundBody = bodyRef;

    AssertFatal( this->mBodyRefs.empty(), "" );
    this->mBodyRefs.push_back( this->mGroundBody );
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::destroyGroundBody()
//=----------------------------------------------------------------------------
void Box2dWorldRef::destroyGroundBody()
{
    AssertFatal( this->mWorld != NULL, "" );
    AssertFatal( this->mGroundBody != NULL, "" );
    AssertFatal( this->mGroundBody->getBody() == this->mWorld->GetGroundBody(),
        "" );
    AssertFatal( this->mBodyRefs.size() == 1, "" );
    AssertFatal( this->getJointCount() == 0, "" );
    
    while ( this->mGroundBody->getShapeList() )
    {
        this->mGroundBody->destroyShape( this->mGroundBody->getShapeList() );    
    }

    this->mBodyRefs.clear();

    this->mGroundBody->destroy();
    this->mGroundBody = NULL;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::readJointBaseData()
//=----------------------------------------------------------------------------
void Box2dWorldRef::readJointBaseData( SimObject *def, 
                                       b2JointDef * jointDef, 
                                       void * userData )
{
    static const StringTableEntry stBody1 = StringTable->insert( "body1" );
    static const StringTableEntry stBody2 = StringTable->insert( "body2" );
    static const StringTableEntry stCollideConnected = StringTable->insert( 
        "collideConnected" 
        );

    const char *body1Id = def->getDataField( stBody1, NULL );
    const char *body2Id = def->getDataField( stBody2, NULL );
    const bool collideConnected = dAtob( 
        def->getDataField( stCollideConnected, NULL )
        );

    Box2dBodyRef* bodyRef1 = dynamic_cast<Box2dBodyRef*>( 
        Sim::findObject( body1Id ) 
        );

    Box2dBodyRef* bodyRef2 = dynamic_cast<Box2dBodyRef*>( 
        Sim::findObject( body2Id ) 
        );

    AssertFatal( bodyRef1 != NULL, "" );
    AssertFatal( bodyRef2 != NULL, "" );

    jointDef->body1 = bodyRef1->getBody();
    jointDef->body2 = bodyRef2->getBody();
    jointDef->userData = userData;
    jointDef->collideConnected = collideConnected;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createRevoluteJoint()
//=----------------------------------------------------------------------------
Box2dJointRef* Box2dWorldRef::createRevoluteJoint( SimObject *def )
{
    AssertFatal( this->mWorld != NULL, "" );
    AssertFatal( def != NULL, "" );
    AssertFatal( dStricmp( def->getDataField( 
        StringTable->insert( "jointType" ), NULL ), "e_revoluteJoint" ) == 0, 
        "" 
        );

    Box2dRevoluteJointRef * jointRef = new Box2dRevoluteJointRef();

    if ( !jointRef->registerObject() )
    {
        AssertFatal( 
            false, 
            "Box2dWorldRef::createRevoluteJoint() - \
            Couldn't register jointRef."
            );

        return NULL;
    }
    
    b2RevoluteJointDef jointDef;

    this->readJointBaseData( def, &jointDef, jointRef );
    
    static const StringTableEntry stLocalAnchor1 = 
        StringTable->insert( "localAnchor1" );

    static const StringTableEntry stLocalAnchor2 = 
        StringTable->insert( "localAnchor2" );

    static const StringTableEntry stReferenceAngle = 
        StringTable->insert( "referenceAngle" );

    static const StringTableEntry stLowerAngle = 
        StringTable->insert( "lowerAngle" );

    static const StringTableEntry stUpperAngle = 
        StringTable->insert( "upperAngle" );

    static const StringTableEntry stMaxMotorTorque = 
        StringTable->insert( "maxMotorTorque" );

    static const StringTableEntry stMotorSpeed = 
        StringTable->insert( "motorSpeed" );

    static const StringTableEntry stEnableLimit = 
        StringTable->insert( "enableLimit" );

    static const StringTableEntry stEnableMotor = 
        StringTable->insert( "enableMotor" );

    // localAnchor1
    const char * value = def->getDataField( stLocalAnchor1, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.localAnchor1 = strTob2Vec2( value );
    }

    // localAnchor2
    value = def->getDataField( stLocalAnchor2, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.localAnchor2 = strTob2Vec2( value );
    }

    // referenceAngle
    value = def->getDataField( stReferenceAngle, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.referenceAngle = strToFloat( value );
    }

    // lowerAngle
    value = def->getDataField( stLowerAngle, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.lowerAngle = strToFloat( value );
    }

    // upperAngle
    value = def->getDataField( stUpperAngle, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.upperAngle = strToFloat( value );
    }

    // maxMotorTorque
    value = def->getDataField( stMaxMotorTorque, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.maxMotorTorque = strToFloat( value );
    }

    // motorSpeed
    value = def->getDataField( stMotorSpeed, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.motorSpeed = strToFloat( value );
    }

    // enableLimit
    value = def->getDataField( stEnableLimit, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.enableLimit = strToFloat( value );
    }

    // enableMotor
    value = def->getDataField( stEnableMotor, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.enableMotor = strToFloat( value );
    }

    b2Joint * joint = this->mWorld->CreateJoint( &jointDef );
    jointRef->setJoint( joint );

    return jointRef;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createMouseJoint()
//=----------------------------------------------------------------------------
Box2dJointRef* Box2dWorldRef::createMouseJoint( SimObject *def )
{
    AssertFatal( this->mWorld != NULL, "" );
    AssertFatal( def != NULL, "" );
    AssertFatal( dStricmp( def->getDataField( 
        StringTable->insert( "jointType" ), NULL ), "e_mouseJoint" ) == 0, 
        "" 
        );

    Box2dMouseJointRef * jointRef = new Box2dMouseJointRef();

    if ( !jointRef->registerObject() )
    {
        AssertFatal( 
            false, 
            "Box2dWorldRef::createMouseJoint() - Couldn't register jointRef." 
            );

        return NULL;
    }
    
    b2MouseJointDef jointDef;

    this->readJointBaseData( def, &jointDef, jointRef );
    
    static const StringTableEntry stTarget = StringTable->insert( "target" );

    static const StringTableEntry stMaxForce = 
        StringTable->insert( "maxForce" );

    static const StringTableEntry stFrequencyHz = 
        StringTable->insert( "frequencyHz" );

    static const StringTableEntry stDampingRatio = 
        StringTable->insert( "dampingRatio" );

    static const StringTableEntry stTimeStep = 
        StringTable->insert( "timeStep" );
        
    // target
    const char * value = def->getDataField( stTarget, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.target = strTob2Vec2( value );
    }

    // maxForce
    value = def->getDataField( stMaxForce, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.maxForce = strToFloat( value );
    }

    // frequencyHz
    value = def->getDataField( stFrequencyHz, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.frequencyHz = strToFloat( value );
    }

    // dampingRatio
    value = def->getDataField( stDampingRatio, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.dampingRatio = strToFloat( value );
    }

    // timeStep
    value = def->getDataField( stTimeStep, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.timeStep = strToFloat( value );
    }

    b2Joint * joint = this->mWorld->CreateJoint( &jointDef );
    jointRef->setJoint( joint );

    return jointRef;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createDistanceJoint()
//=----------------------------------------------------------------------------
Box2dJointRef* Box2dWorldRef::createDistanceJoint( SimObject *def )
{
    AssertFatal( this->mWorld != NULL, "" );
    AssertFatal( def != NULL, "" );
    AssertFatal( dStricmp( def->getDataField( 
        StringTable->insert( "jointType" ), NULL ), "e_distanceJoint" ) == 0, 
        "" 
        );

    Box2dDistanceJointRef *jointRef = new Box2dDistanceJointRef();

    if ( !jointRef->registerObject() )
    {
        AssertFatal( 
            false, 
            "Box2dWorldRef::createDistanceJoint() - \
            Couldn't register jointRef." 
            );

        return NULL;
    }
    
    b2DistanceJointDef jointDef;

    this->readJointBaseData( def, &jointDef, jointRef );

    static const StringTableEntry stLength = StringTable->insert( "length" );

    static const StringTableEntry stFrequencyHz = 
        StringTable->insert( "frequencyHz" );

    static const StringTableEntry stDampingRatio = 
        StringTable->insert( "dampingRatio" );
        
    // length
    const char * value = def->getDataField( stLength, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.length = strToFloat( value );
    }

    // frequencyHz
    value = def->getDataField( stFrequencyHz, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.frequencyHz = strToFloat( value );
    }

    // dampingRatio
    value = def->getDataField( stDampingRatio, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.dampingRatio = strToFloat( value );
    }

    b2Joint * joint = this->mWorld->CreateJoint( &jointDef );
    jointRef->setJoint( joint );

    return jointRef;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createPrismaticJoint()
//=----------------------------------------------------------------------------
Box2dJointRef* Box2dWorldRef::createPrismaticJoint( SimObject *def )
{
    AssertFatal( this->mWorld != NULL, "" );
    AssertFatal( def != NULL, "" );
    AssertFatal( dStricmp( def->getDataField( 
        StringTable->insert( "jointType" ), NULL ), "e_prismaticJoint" ) == 0, 
        "" 
        );

    Box2dPrismaticJointRef *jointRef = new Box2dPrismaticJointRef();

    if ( !jointRef->registerObject() )
    {
        AssertFatal( 
            false, 
            "Box2dWorldRef::createPrismaticJoint() - \
            Couldn't register jointRef." 
            );

        return NULL;
    }
    
    b2PrismaticJointDef jointDef;

    this->readJointBaseData( def, &jointDef, jointRef );

    static const StringTableEntry stLocalAnchor1 = 
        StringTable->insert( "localAnchor1" );

    static const StringTableEntry stLocalAnchor2 = 
        StringTable->insert( "localAnchor2" );

    static const StringTableEntry stLocalAxis1 = 
        StringTable->insert( "localAxis1" );

    static const StringTableEntry stReferenceAngle = 
        StringTable->insert( "referenceAngle" );

    static const StringTableEntry stEnableLimit = 
        StringTable->insert( "enableLimit" );

    static const StringTableEntry stLowerTranslation = 
        StringTable->insert( "lowerTranslation" );

    static const StringTableEntry stUpperTranslation = 
        StringTable->insert( "upperTranslation" );

    static const StringTableEntry stEnableMotor = 
        StringTable->insert( "enableMotor" );

    static const StringTableEntry stMaxMotorForce = 
        StringTable->insert( "maxMotorForce" );

    static const StringTableEntry stMotorSpeed = 
        StringTable->insert( "motorSpeed" );
     
    // localAnchor1
    const char *value = def->getDataField( stLocalAnchor1, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.localAnchor1 = strTob2Vec2( value );
    }

    // localAnchor2
    value = def->getDataField( stLocalAnchor2, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.localAnchor2 = strTob2Vec2( value );
    }

    // localAxis1
    value = def->getDataField( stLocalAxis1, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.localAxis1 = strTob2Vec2( value );
    }

    // referenceAngle
    value = def->getDataField( stReferenceAngle, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.referenceAngle = strToFloat( value );
    }

    // enableLimit
    value = def->getDataField( stEnableLimit, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.enableLimit = dAtob( value );
    }

    // lowerTranslation
    value = def->getDataField( stLowerTranslation, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.lowerTranslation = strToFloat( value );
    }

    // upperTranslation
    value = def->getDataField( stUpperTranslation, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.upperTranslation = strToFloat( value );
    }

    // enableMotor
    value = def->getDataField( stEnableMotor, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.enableMotor = dAtob( value );
    }

    // maxMotorForce
    value = def->getDataField( stMaxMotorForce, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.maxMotorForce = strToFloat( value );
    }

    // motorSpeed
    value = def->getDataField( stMotorSpeed, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.motorSpeed = strToFloat( value );
    }

    b2Joint * joint = this->mWorld->CreateJoint( &jointDef );
    jointRef->setJoint( joint );

    return jointRef;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createGearJoint()
//=----------------------------------------------------------------------------
Box2dJointRef* Box2dWorldRef::createGearJoint( SimObject *def )
{
    AssertFatal( this->mWorld != NULL, "" );
    AssertFatal( def != NULL, "" );
    AssertFatal( dStricmp( def->getDataField( 
        StringTable->insert( "jointType" ), NULL ), "e_gearJoint" ) == 0, 
        "" 
        );

    Box2dGearJointRef *jointRef = new Box2dGearJointRef();

    if ( !jointRef->registerObject() )
    {
        AssertFatal( 
            false, 
            "Box2dWorldRef::createGearJoint() - \
            Couldn't register jointRef." 
            );

        return NULL;
    }
    
    b2GearJointDef jointDef;

    this->readJointBaseData( def, &jointDef, jointRef );

    static const StringTableEntry stJoint1 = StringTable->insert( "joint1" );
    static const StringTableEntry stJoint2 = StringTable->insert( "joint2" );
    static const StringTableEntry stRatio = StringTable->insert( "ratio" );
        
    // joint1
    const char * value = def->getDataField( stJoint1, NULL );
    if ( notEmpty( value ) )
    {
        Box2dJointRef *jointRef1 = 
            dynamic_cast<Box2dJointRef*>( Sim::findObject( value ) );
        
        AssertFatal( dynamic_cast<Box2dRevoluteJointRef*>( jointRef1 ) ||
            dynamic_cast<Box2dRevoluteJointRef*>( jointRef1 ),
            "Box2dWorldRef::createGearJoint() - Joint 1 is neither a revolute \
             joint nor a prismatic joint." );

        jointDef.joint1 = jointRef1->getJoint();
    }

    // joint2
    value = def->getDataField( stJoint2, NULL );
    if ( notEmpty( value ) )
    {
        Box2dJointRef *jointRef2 = 
            dynamic_cast<Box2dJointRef*>( Sim::findObject( value ) );
        
        AssertFatal( dynamic_cast<Box2dRevoluteJointRef*>( jointRef2 ) ||
            dynamic_cast<Box2dRevoluteJointRef*>( jointRef2 ),
            "Box2dWorldRef::createGearJoint() - Joint 2 is neither a revolute \
             joint nor a prismatic joint." );

        jointDef.joint2 = jointRef2->getJoint();
    }

    // ratio
    value = def->getDataField( stRatio, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.ratio = strToFloat( value );
    }

    b2Joint * joint = this->mWorld->CreateJoint( &jointDef );
    jointRef->setJoint( joint );

    return jointRef;
}


//=----------------------------------------------------------------------------
// Box2dWorldRef::createPulleyJoint()
//=----------------------------------------------------------------------------
Box2dJointRef* Box2dWorldRef::createPulleyJoint( SimObject *def )
{
    AssertFatal( this->mWorld != NULL, "" );
    AssertFatal( def != NULL, "" );
    AssertFatal( dStricmp( def->getDataField( 
        StringTable->insert( "jointType" ), NULL ), "e_pulleyJoint" ) == 0, 
        "" 
        );

    Box2dPulleyJointRef *jointRef = new Box2dPulleyJointRef();

    if ( !jointRef->registerObject() )
    {
        AssertFatal( 
            false, 
            "Box2dWorldRef::createGearJoint() - \
            Couldn't register jointRef." 
            );

        return NULL;
    }
    
    b2PulleyJointDef jointDef;

    this->readJointBaseData( def, &jointDef, jointRef );

    static const StringTableEntry stGroundAnchor1 = 
        StringTable->insert( "groundAnchor1" );
    static const StringTableEntry stGroundAnchor2 = 
        StringTable->insert( "groundAnchor2" );
    static const StringTableEntry stLocalAnchor1 = 
        StringTable->insert( "localAnchor1" );
    static const StringTableEntry stLocalAnchor2 = 
        StringTable->insert( "localAnchor2" );
    static const StringTableEntry stLength1 = 
        StringTable->insert( "length1" );
    static const StringTableEntry stLength2 = 
        StringTable->insert( "length2" );
    static const StringTableEntry stMaxLength1 = 
        StringTable->insert( "maxLength1" );
    static const StringTableEntry stMaxLength2 = 
        StringTable->insert( "maxLength2" );
    static const StringTableEntry stRatio = 
        StringTable->insert( "ratio" );
    
    // groundAnchor1
    const char * value = def->getDataField( stGroundAnchor1, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.groundAnchor1 = Box2dUtil::strTob2Vec2( value );
    }

    // groundAnchor2
    value = def->getDataField( stGroundAnchor2, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.groundAnchor2 = Box2dUtil::strTob2Vec2( value );
    }

    // localAnchor1
    value = def->getDataField( stLocalAnchor1, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.localAnchor1 = Box2dUtil::strTob2Vec2( value );
    }

    // localAnchor2
    value = def->getDataField( stLocalAnchor2, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.localAnchor2 = Box2dUtil::strTob2Vec2( value );
    }

    // length1
    value = def->getDataField( stLength1, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.length1 = Box2dUtil::strToFloat( value );
    }

    // length2
    value = def->getDataField( stLength2, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.length2 = Box2dUtil::strToFloat( value );
    }

    // maxLength1
    value = def->getDataField( stMaxLength1, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.maxLength1 = Box2dUtil::strToFloat( value );
    }

    // maxLength2
    value = def->getDataField( stMaxLength2, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.maxLength2 = Box2dUtil::strToFloat( value );
    }

    // ratio
    value = def->getDataField( stRatio, NULL );
    if ( notEmpty( value ) )
    {
        jointDef.ratio = strToFloat( value );
    }

    b2Joint * joint = this->mWorld->CreateJoint( &jointDef );
    jointRef->setJoint( joint );

    return jointRef;
}
