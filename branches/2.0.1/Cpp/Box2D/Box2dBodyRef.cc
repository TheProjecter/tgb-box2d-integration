//=============================================================================
// Box2dBodyRef.cc
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

#include "./Box2dBodyRef.h"
#include "./Box2dUtil.h"
#include "./Box2dShapeRef.h"
#include "./Box2dWorldRef.h"
#include "./Joints/Box2dJointRef.h"
#include "./Joints/Box2dJointEdgeRef.h"

#include "T2D/t2dSceneObject.h"
#include "T2D/t2dTileMap.h"
#include "console/console.h"

#include "Box2D.h"

using namespace Box2dUtil;

IMPLEMENT_CONOBJECT(Box2dBodyRef);

//=----------------------------------------------------------------------------
// Box2dBodyRef::Box2dBodyRef()
//=----------------------------------------------------------------------------
Box2dBodyRef::Box2dBodyRef():
    mSceneObject( NULL ),
    mBody( NULL ),
    mWorld( NULL ),
    mAutoSetMassFromShapes( false ),
    mAutoMassDirty( false ),
    mNext( NULL ),
    mUserData( NULL )
{
    DEBUG_INC(Box2dBodyRefsCreated);
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::~Box2dBodyRef()
//=----------------------------------------------------------------------------
Box2dBodyRef::~Box2dBodyRef()
{
    DEBUG_INC(Box2dBodyRefsDestroyed);
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::createShape()
//=----------------------------------------------------------------------------
Box2dShapeRef * Box2dBodyRef::createShape( t2dSceneObject * def )
{
    AssertFatal( def != NULL, 
        "Box2dBodyRef::createShape() - No def sceneObject given" );
    
    BehaviorInstance * shapeBehavior = 
        def->getBehavior( StringTable->insert( "Box2dShapeBehavior" ) );

    AssertFatal( shapeBehavior != NULL,
        "Box2dBodyRef::createShape() -\
         def must own an instance of Box2dShapeBehavior" );
    
    AssertFatal( 
        Box2dShapeRef::isSupported( 
            static_cast<t2dSceneObject*>( shapeBehavior->getBehaviorOwner() ) 
            ),
        "Box2dWorldRef::createBody() - This t2dSceneObject is not supported" 
        );

    Box2dShapeRef * shapeRef = new Box2dShapeRef();

    if ( ! shapeRef->registerObject() ) {
        AssertFatal( false, 
            "Box2dBodyRef::createShape() - Couldn't register Box2dShapeRef" );
        delete shapeRef;
        return NULL;
    }

    t2dSceneObject * sceneObject = def;
    t2dSceneObject * bodySceneObject = this->getSceneObject();

    AssertFatal( bodySceneObject != NULL, 
        "Box2dBodyRef::createShape() - No body sceneobject given" );
    
    if ( sceneObject->getSceneGraph() != bodySceneObject->getSceneGraph() )
    {
        bodySceneObject->getSceneGraph()->addToScene( sceneObject );
    }

    if ( sceneObject != bodySceneObject )
    {
        sceneObject->dismount();

        sceneObject->mount( 
            bodySceneObject,
            bodySceneObject->getLocalPoint( sceneObject->getPosition() ),
            0.0f,
            true,
            true,
            true,
            false
            );
    }

    // StringTableEntries for getDataField queries
    static const StringTableEntry stType = 
            StringTable->insert( "Type" );    
    static const StringTableEntry stDensity = 
        StringTable->insert( "Density" );
    static const StringTableEntry stFriction = 
        StringTable->insert( "Friction" );
    static const StringTableEntry stIsSensor = 
        StringTable->insert( "IsSensor" );
    static const StringTableEntry stRestitution = 
        StringTable->insert( "Restitution" );
    static const StringTableEntry stFilterGroupIndex = 
        StringTable->insert( "FilterGroupIndex" );
    static const StringTableEntry stFilterCategoryBits = 
        StringTable->insert( "FilterCategoryBits" );
    static const StringTableEntry stFilterMaskBits = 
        StringTable->insert( "FilterMaskBits" );
    static const StringTableEntry stShapeRef = 
        StringTable->insert( "shapeRef" );


    // Determine if this is a circle or a polygon shape
    const char * shapeTypeString = 
        shapeBehavior->getDataField( stType, NULL );

    if ( dStricmp( shapeTypeString, "POLYGON" ) == 0 )
    {
        b2PolygonDef polyDef;

        // Density
        polyDef.density = 
            strToFloat( shapeBehavior->getDataField( stDensity, NULL ) );

        // Friction
        polyDef.friction = 
            strToFloat( shapeBehavior->getDataField( stFriction, NULL ) );

        // IsSensor
        polyDef.isSensor = 
            dAtob( shapeBehavior->getDataField( stIsSensor, NULL ) );

        // Restitution
        polyDef.restitution = 
            strToFloat( shapeBehavior->getDataField( stRestitution, NULL ) );

        // Filter.GroupIndex
        polyDef.filter.groupIndex = 
            dAtoi( shapeBehavior->getDataField( stFilterGroupIndex, NULL ) );

        // Filter.CategoryBits
        polyDef.filter.categoryBits = strTo16BitMask( 
            shapeBehavior->getDataField( stFilterCategoryBits, NULL ) 
            );

        // Filter.maskBits
        polyDef.filter.maskBits = strTo16BitMask( 
            shapeBehavior->getDataField( stFilterMaskBits, NULL ) 
            );

        // Let the user data point to the shapeRef
        polyDef.userData = shapeRef;

        // Read the collision polygon from the t2dSceneObject
        polyDef.vertexCount = sceneObject->getCollisionPolyCount();

        const t2dVector bodyHalfSize = bodySceneObject->getHalfSize();

        for ( U32 i = 0; i < sceneObject->getCollisionPolyCount(); ++i )
        {
            const t2dVector localVertex =
                sceneObject->getCollisionPolyArray()[i];

            const t2dVector rotatedLocalVertex =
                ( sceneObject->getMountedParent() != NULL ) ?
                localVertex * bodySceneObject->getRotationMatrix()
                :
                localVertex;

            const t2dVector worldVertex = 
                sceneObject->getWorldPoint( rotatedLocalVertex );

            const t2dVector bodyLocalVertex = 
                bodySceneObject->getLocalPoint( worldVertex );

            polyDef.vertices[i].Set( bodyLocalVertex.mX * bodyHalfSize.mX, 
                                     bodyLocalVertex.mY * bodyHalfSize.mY );
        }
        
        // Create the b2Shape
        b2Shape * shape = this->mBody->CreateShape( &polyDef );

        // Configure the shapeRef
        shapeRef->setShape( shape );
        shapeRef->setBody( this );
        shapeRef->setSceneObject( sceneObject );
    }
    else
    {
        AssertFatal( dStricmp( shapeTypeString, "CIRCLE" ) == 0, 
            "Box2dBodyRef::createShape() - Invalid shape type." );

        AssertFatal( 
            mIsEqualRange( sceneObject->getSize().mX, 
                           sceneObject->getSize().mY, 0.05f ),
            "Box2dBodyRef::createShape() - Ellipses are not supported." 
            );

        b2CircleDef circleDef;

        // Density
        circleDef.density = 
            strToFloat( shapeBehavior->getDataField( stDensity, NULL ) );
        
        // Friction
        circleDef.friction = 
            strToFloat( shapeBehavior->getDataField( stFriction, NULL ) );

        // IsSensor
        circleDef.isSensor = 
            dAtob( shapeBehavior->getDataField( stIsSensor, NULL ) );

        // Restitution
        circleDef.restitution = 
            strToFloat( shapeBehavior->getDataField( stRestitution, NULL ) );

        // Filter.GroupIndex
        circleDef.filter.groupIndex = 
            dAtoi( shapeBehavior->getDataField( stFilterGroupIndex, NULL ) );

        // Filter.CategoryBits
        circleDef.filter.categoryBits = strTo16BitMask( 
            shapeBehavior->getDataField( stFilterCategoryBits, NULL ) 
            );

        // Filter.MaskBits
        circleDef.filter.maskBits = strTo16BitMask( 
            shapeBehavior->getDataField( stFilterMaskBits, NULL ) 
            );

        // Let the user data point to the shapeRef
        circleDef.userData = shapeRef;

        // Set the local position of the circle
        const t2dVector localPosition = 
            sceneObject->getPosition() - bodySceneObject->getPosition();

        circleDef.localPosition.Set( localPosition.mX, localPosition.mY );

        // Set the circle's radius
        sceneObject->setCollisionCircleSuperscribed( false );
        circleDef.radius = sceneObject->getCollisionCircleRadii();

        // Create the b2Shape
        b2Shape * shape = this->mBody->CreateShape( &circleDef );

        // Configure the shapeRef
        shapeRef->setShape( shape );
        shapeRef->setBody( this );
        shapeRef->setSceneObject( sceneObject );
    }

    if ( this->mAutoSetMassFromShapes )
    {
        this->setMassFromShapes();
    }

    sceneObject->setDataField( stShapeRef, NULL, shapeRef->getIdString() );

    return shapeRef;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::createShape() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, createShape, S32, 3, 3, "" )
{
    t2dSceneObject * def = 
        dynamic_cast<t2dSceneObject*>( Sim::findObject( argv[2] ) );

    AssertFatal( def != NULL, 
        "Box2dBodyRef::createShape() - Invalid BehaviorInstance." );

    Box2dShapeRef * shapeRef = object->createShape( def );

    return shapeRef ? shapeRef->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::destroyShape()
//=----------------------------------------------------------------------------
void Box2dBodyRef::destroyShape( Box2dShapeRef * shapeRef )
{
    AssertFatal( shapeRef != NULL, 
        "Box2dBodyRef::destroyShape() - Shape must not be NULL." );
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::destroyShape() - No b2Body is referenced" );
    AssertFatal( this->mBody == shapeRef->getBody()->getBody(), 
        "Box2dBodyRef::destroyShape() - Shape owner mismatch." );
    
    b2Shape *shape = shapeRef->getShape();    
    this->mBody->DestroyShape( shape );
    shapeRef->destroy();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::destroyShape() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, destroyShape, void, 3, 3, "" )
{
    Box2dShapeRef * shapeRef = 
        dynamic_cast<Box2dShapeRef*>( Sim::findObject( argv[2] ) );

    AssertFatal( shapeRef != NULL, 
        "Box2dBodyRef::destroyShape() - Invalid Box2dShapeRef." );

    object->destroyShape( shapeRef );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setMass()
//=----------------------------------------------------------------------------
void Box2dBodyRef::setMass( const b2MassData* massData )
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::setMass() - No b2Body is referenced" );

    this->mBody->SetMass( massData );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setMass() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, setMass, void, 5, 5, 
              "Box2dBodyRef::setMass( %center, %mass, %inertialMoment )" )
{
    b2MassData massData;
    
    const t2dVector center = t2dSceneObject::getStringElementVector( argv[2] );
    massData.center.Set( center.mX, center.mY );

    massData.mass = dAtof( argv[3] );
    massData.I = dAtof( argv[4] );

    object->setMass( &massData );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setMassFromShapes()
//=----------------------------------------------------------------------------
void Box2dBodyRef::setMassFromShapes()
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::setMassFromShapes() - No b2Body is referenced" );

    this->mBody->SetMassFromShapes();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setMassFromShapes() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, setMassFromShapes, void, 2, 2, "" )
{
    object->setMassFromShapes();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setXForm()
//=----------------------------------------------------------------------------
bool Box2dBodyRef::setXForm( const b2Vec2& position, float32 angle )
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::setXForm() - No b2Body is referenced" );

    return this->mBody->SetXForm( position, angle );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setXForm() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, setXForm, bool, 4, 4, "" )
{
    AssertFatal( isValid2dVector(argv[2]), 
        "Box2dBodyRef::setXForm() - Encountered invalid 2d-vector" );

    const b2Vec2 position = strTob2Vec2( argv[2] );
    const F32 angle = dAtof( argv[3] );

    return object->setXForm( position, angle );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getXForm()
//=----------------------------------------------------------------------------
const b2XForm& Box2dBodyRef::getXForm() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getXForm() - No b2Body is referenced" );
    
    return this->mBody->GetXForm();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getXForm() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef ,getXForm, const char*, 2, 2, 
"returns: \"posX posY rotMatrixCol1X rotMatrixCol1Y \
rotMatrixCol2X rotMatrixCol2Y\"" 
)
{
    const b2XForm & xForm = object->getXForm();

    const int BUFFER_SIZE = 64;
    char * returnBuffer = Con::getReturnBuffer( BUFFER_SIZE );

    dSprintf( 
        returnBuffer, 
        BUFFER_SIZE, 
        "%f %f %f %f %f %f", 
        xForm.position.x, 
        xForm.position.y,
        xForm.R.col1.x,
        xForm.R.col1.y,
        xForm.R.col2.x,
        xForm.R.col2.y );

    return returnBuffer;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getPosition()
//=----------------------------------------------------------------------------
const b2Vec2& Box2dBodyRef::getPosition() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getPosition() - No b2Body is referenced" );

    return this->mBody->GetPosition();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getPosition() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getPosition, const char*, 2, 2, "returns \"x y\"" )
{
    return b2Vec2ToReturnBuffer( object->getPosition() );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getAngle()
//=----------------------------------------------------------------------------
float32 Box2dBodyRef::getAngle() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getAngle() - No b2Body is referenced" );

    return this->mBody->GetAngle();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getAngle() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getAngle, F32, 2, 2, "" )
{
    return object->getAngle();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getWorldCenter()
//=----------------------------------------------------------------------------
const b2Vec2& Box2dBodyRef::getWorldCenter() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getWorldCenter() - No b2Body is referenced" );

    return this->mBody->GetWorldCenter();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getWorldCenter() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getWorldCenter, const char*, 2, 2, "" )
{
    return b2Vec2ToReturnBuffer( object->getWorldCenter() );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLocalCenter()
//=----------------------------------------------------------------------------
const b2Vec2& Box2dBodyRef::getLocalCenter() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getLocalCenter() - No b2Body is referenced" );

    return this->mBody->GetLocalCenter();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLocalCenter() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getLocalCenter, const char*, 2, 2, "" )
{
    return b2Vec2ToReturnBuffer( object->getLocalCenter() );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setLinearVelocity()
//=----------------------------------------------------------------------------
void Box2dBodyRef::setLinearVelocity(const b2Vec2& v)
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::setLinearVelocity() - No b2Body is referenced" );

    this->mBody->SetLinearVelocity( v );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setLinearVelocity() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, setLinearVelocity, void, 3, 3, "" )
{
    object->setLinearVelocity( strTob2Vec2( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLinearVelocity()
//=----------------------------------------------------------------------------
b2Vec2 Box2dBodyRef::getLinearVelocity() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getLinearVelocity() - No b2Body is referenced" );

    return this->mBody->GetLinearVelocity();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLinearVelocity() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getLinearVelocity, const char*, 2, 2, "" )
{
    return b2Vec2ToReturnBuffer( object->getLinearVelocity() );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setAngularVelocity()
//=----------------------------------------------------------------------------
void Box2dBodyRef::setAngularVelocity( float32 omega )
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::setAngularVelocity() - No b2Body is referenced" );

    this->mBody->SetAngularVelocity( omega );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setAngularVelocity() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, setAngularVelocity, void, 3, 3, "" )
{
    AssertFatal( isValidFloat( argv[2] ), 
        "Box2dBodyRef::setAngularVelocity() - Encountered invalid float." );

    object->setAngularVelocity( dAtof( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getAngularVelocity()
//=----------------------------------------------------------------------------
float32 Box2dBodyRef::getAngularVelocity() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getAngularVelocity() - No b2Body is referenced" );

    return this->mBody->GetAngularVelocity();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getAngularVelocity() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getAngularVelocity, F32, 2, 2, "" )
{
    return object->getAngularVelocity();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::applyForce()
//=----------------------------------------------------------------------------
void Box2dBodyRef::applyForce(const b2Vec2& force, const b2Vec2& point)
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::applyForce() - No b2Body is referenced" );

    this->mBody->ApplyForce( force, point );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::applyForce() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, applyForce, void, 4, 4, "" )
{
    const b2Vec2 force = strTob2Vec2( argv[2] );
    const b2Vec2 point = strTob2Vec2( argv[3] );

    object->applyForce( force, point );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::applyTorque()
//=----------------------------------------------------------------------------
void Box2dBodyRef::applyTorque( float32 torque )
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::applyTorque() - No b2Body is referenced" );

    this->mBody->ApplyTorque( torque );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::applyTorque() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, applyTorque, void, 3, 3, "" )
{
    AssertFatal( isValidFloat( argv[2] ), 
        "Box2dBodyRef::applyTorque() - Encountered invalid float." );

    object->applyTorque( dAtof( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::applyImpulse()
//=----------------------------------------------------------------------------
void Box2dBodyRef::applyImpulse( const b2Vec2& impulse, const b2Vec2& point )
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::applyImpulse() - No b2Body is referenced" );

    this->mBody->ApplyImpulse( impulse, point );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::applyImpulse() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, applyImpulse, void, 4, 4, "" )
{
    const b2Vec2 impulse = strTob2Vec2( argv[2] );
    const b2Vec2 point = strTob2Vec2( argv[3] );

    object->applyImpulse( impulse, point );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getMass()
//=----------------------------------------------------------------------------
float32 Box2dBodyRef::getMass() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getMass() - No b2Body is referenced" );

    return this->mBody->GetMass();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getMass() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getMass, F32, 2, 2, "" )
{
    return object->getMass();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getInertia()
//=----------------------------------------------------------------------------
float32 Box2dBodyRef::getInertia() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getInertia() - No b2Body is referenced" );

    return this->mBody->GetInertia();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getInertia() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getInertia, F32, 2, 2, "" )
{
    return object->getInertia();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getWorldPoint()
//=----------------------------------------------------------------------------
b2Vec2 Box2dBodyRef::getWorldPoint( const b2Vec2& localPoint ) const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getWorldPoint() - No b2Body is referenced" );

    return this->mBody->GetWorldPoint( localPoint );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getWorldPoint() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getWorldPoint, const char*, 3, 3, "" )
{
    const b2Vec2 localPoint = strTob2Vec2( argv[2] );
    const b2Vec2 worldPoint = object->getWorldPoint( localPoint );

    return b2Vec2ToReturnBuffer( worldPoint );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getWorldVector()
//=----------------------------------------------------------------------------
b2Vec2 Box2dBodyRef::getWorldVector( const b2Vec2& localVector ) const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getWorldVector() - No b2Body is referenced" );

    return this->mBody->GetWorldVector( localVector );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getWorldVector() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getWorldVector, const char*, 3, 3, "" )
{
    const b2Vec2 localVector = strTob2Vec2( argv[2] );
    const b2Vec2 worldVector = object->getWorldVector( localVector );

    return b2Vec2ToReturnBuffer( worldVector );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLocalPoint()
//=----------------------------------------------------------------------------
b2Vec2 Box2dBodyRef::getLocalPoint(const b2Vec2& worldPoint) const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getLocalPoint() - No b2Body is referenced" );

    return this->mBody->GetLocalPoint( worldPoint );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLocalPoint() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getLocalPoint, const char*, 3, 3, "" )
{
    const b2Vec2 worldPoint = strTob2Vec2( argv[2] );
    const b2Vec2 localPoint = object->getLocalPoint( worldPoint );

    return b2Vec2ToReturnBuffer( localPoint );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLocalVector()
//=----------------------------------------------------------------------------
b2Vec2 Box2dBodyRef::getLocalVector(const b2Vec2& worldVector) const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getLocalVector() - No b2Body is referenced" );

    return this->mBody->GetLocalVector( worldVector );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLocalVector() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getLocalVector, const char*, 3, 3, "" )
{
    const b2Vec2 worldVector = strTob2Vec2( argv[2] );
    const b2Vec2 localVector = object->getLocalVector( worldVector );

    return b2Vec2ToReturnBuffer( localVector );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLinearVelocityFromWorldPoint()
//=----------------------------------------------------------------------------
b2Vec2 Box2dBodyRef::getLinearVelocityFromWorldPoint( 
    const b2Vec2& worldPoint ) const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getLinearVelocityFromWorldPoint() -\
        No b2Body is referenced" );

    return this->mBody->GetLinearVelocityFromWorldPoint( worldPoint );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLinearVelocityFromWorldPoint() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getLinearVelocityFromWorldPoint, const char*, 
               3, 3, "" )
{
    const b2Vec2 worldPoint = strTob2Vec2( argv[2] );
    const b2Vec2 v = object->getLinearVelocityFromWorldPoint( worldPoint );

    return b2Vec2ToReturnBuffer( v );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLinearVelocityFromLocalPoint()
//=----------------------------------------------------------------------------
b2Vec2 Box2dBodyRef::getLinearVelocityFromLocalPoint( 
    const b2Vec2& localPoint ) const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getLinearVelocityFromLocalPoint() -\
        No b2Body is referenced" );

    return this->mBody->GetLinearVelocityFromLocalPoint( localPoint );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getLinearVelocityFromLocalPoint() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getLinearVelocityFromLocalPoint, const char*, 
               3, 3, "" )
{
    const b2Vec2 localPoint = strTob2Vec2( argv[2] );
    const b2Vec2 v = object->getLinearVelocityFromLocalPoint( localPoint );

    return b2Vec2ToReturnBuffer( v );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isBullet()
//=----------------------------------------------------------------------------
bool Box2dBodyRef::isBullet() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::isBullet() - No b2Body is referenced" );

    return this->mBody->IsBullet(); 
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isBullet() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, isBullet, bool, 2, 2, "" )
{
    return object->isBullet();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setBullet()
//=----------------------------------------------------------------------------
void Box2dBodyRef::setBullet( bool flag )
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::setBullet() - No b2Body is referenced" );

    this->mBody->SetBullet( flag ); 
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setBullet() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, setBullet, void, 3, 3, "" )
{
    object->setBullet( dAtob( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isStatic()
//=----------------------------------------------------------------------------
bool Box2dBodyRef::isStatic() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::isStatic() - No b2Body is referenced" );

    return this->mBody->IsStatic(); 
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isStatic() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, isStatic, bool, 2, 2, "" )
{
    return object->isStatic();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isDynamic()
//=----------------------------------------------------------------------------
bool Box2dBodyRef::isDynamic() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::isDynamic() - No b2Body is referenced" );

    return this->mBody->IsDynamic(); 
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isDynamic() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, isDynamic, bool, 2, 2, "" )
{
    return object->isDynamic();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isFrozen()
//=----------------------------------------------------------------------------
bool Box2dBodyRef::isFrozen() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::isFrozen() - No b2Body is referenced" );

    return this->mBody->IsFrozen(); 
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isFrozen() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, isFrozen, bool, 2, 2, "" )
{
    return object->isFrozen();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isSleeping()
//=----------------------------------------------------------------------------
bool Box2dBodyRef::isSleeping() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::isSleeping() - No b2Body is referenced" );

    return this->mBody->IsSleeping(); 
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isSleeping() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, isSleeping, bool, 2, 2, "" )
{
    return object->isSleeping();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::allowSleeping()
//=----------------------------------------------------------------------------
void Box2dBodyRef::allowSleeping( bool flag )
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::allowSleeping() - No b2Body is referenced" );

    this->mBody->AllowSleeping( flag ); 
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::allowSleeping() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, allowSleeping, void, 3, 3, "" )
{
    object->allowSleeping( dAtob( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::wakeUp()
//=----------------------------------------------------------------------------
void Box2dBodyRef::wakeUp()
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::wakeUp() - No b2Body is referenced" );
    
    this->mBody->WakeUp(); 
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::wakeUp() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, wakeUp, void, 2, 2, "" )
{
    object->wakeUp();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::putToSleep()
//=----------------------------------------------------------------------------
void Box2dBodyRef::putToSleep()
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::putToSleep() - No b2Body is referenced" );

    this->mBody->PutToSleep(); 
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::putToSleep() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, putToSleep, void, 2, 2, "" )
{
    object->putToSleep();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getShapeList()
//=----------------------------------------------------------------------------
Box2dShapeRef *Box2dBodyRef::getShapeList() 
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getShapeList() - No b2Body is referenced" );

    b2Shape *shape = this->mBody->GetShapeList();
    return ( shape != NULL ? 
        static_cast<Box2dShapeRef*>( shape->GetUserData() ) : NULL );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getShapeList() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getShapeList, S32, 2, 2, "" )
{
    Box2dShapeRef * shapeRef = object->getShapeList();
    return shapeRef != NULL ? shapeRef->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getJointList()
//=----------------------------------------------------------------------------
Box2dJointEdgeRef *Box2dBodyRef::getJointList() 
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getJointList() - No b2Body is referenced" );

    b2JointEdge *edge = this->mBody->GetJointList();
    
    Box2dJointRef *jointRef = 
        static_cast<Box2dJointRef*>( edge->joint->GetUserData() );

    if ( this == jointRef->getJointEdgeRef1()->getOther() )
    {
        return jointRef->getJointEdgeRef2();
    }
    else
    {
        return jointRef->getJointEdgeRef1();
    }
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getJointList() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getJointList, S32, 2, 2, "" )
{
    Box2dJointEdgeRef * jointEdgeRef = object->getJointList();

    return jointEdgeRef != NULL ? jointEdgeRef->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getNext()
//=----------------------------------------------------------------------------
Box2dBodyRef *Box2dBodyRef::getNext() const
{
    AssertFatal( this->mBody != NULL, 
        "Box2dBodyRef::getNext() - No b2Body is referenced" );

    return this->mNext;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getNext() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getNext, S32, 2, 2, "" )
{
    Box2dBodyRef *next = object->getNext();
    return next != NULL ? next->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getWorld()
//=----------------------------------------------------------------------------
Box2dWorldRef *Box2dBodyRef::getWorld() const
{
    return this->mWorld;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::getWorld() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, getWorld, S32, 2, 2, "" )
{
    return object->getWorld() != NULL ? object->getWorld()->getId() : 0;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setAutoSetMassFromShapes()
//=----------------------------------------------------------------------------
void Box2dBodyRef::setAutoSetMassFromShapes( const bool flag )
{
    this->mAutoSetMassFromShapes = flag;
    this->mAutoMassDirty = flag;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setAutoSetMassFromShapes() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, setAutoSetMassFromShapes, void, 3, 3, "(%bool)" )
{
    object->setAutoSetMassFromShapes( dAtob( argv[2] ) );
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isAutoSetMassFromShapes()
//=----------------------------------------------------------------------------
bool Box2dBodyRef::isAutoSetMassFromShapes() const
{
    return this->mAutoSetMassFromShapes;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isAutoSetMassFromShapes() - ConsoleMethod
//=----------------------------------------------------------------------------
ConsoleMethod( Box2dBodyRef, isAutoSetMassFromShapes, bool, 2, 2, "" )
{
    return object->isAutoSetMassFromShapes();
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::onShapeDestroyedCallback()
//=----------------------------------------------------------------------------
void Box2dBodyRef::onShapeDestroyedCallback( Box2dShapeRef * shapeRef )
{
    this->mAutoMassDirty = true;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::updateAutoMass()
//=----------------------------------------------------------------------------
void Box2dBodyRef::updateAutoMass()
{ 
    if ( this->mAutoMassDirty && this->mAutoSetMassFromShapes )
    {
        this->setMassFromShapes(); 
        this->mAutoMassDirty = false;
    }
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::isSupported()
//=----------------------------------------------------------------------------
bool Box2dBodyRef::isSupported( const t2dSceneObject* sceneObject ) 
{
    const bool isTileMap = ( dynamic_cast<const t2dTileMap*>( sceneObject ) != NULL );
    const bool isTileLayer = ( dynamic_cast<const t2dTileLayer*>( sceneObject ) != NULL );

    return !isTileMap && !isTileLayer;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setBody()
//=----------------------------------------------------------------------------
void Box2dBodyRef::setBody( b2Body * body )
{
    AssertFatal( this->mBody == NULL, 
        "Box2dBodyRef::setBody() - Must only be called once on creation." );

    this->mBody = body;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setSceneObject()
//=----------------------------------------------------------------------------
void Box2dBodyRef::setSceneObject( t2dSceneObject * sceneObject )
{
    AssertFatal( this->mSceneObject == NULL, 
        "Box2dBodyRef::setSceneObject() -\
        Must only be called once on creation." );

    this->mSceneObject = sceneObject;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setWorld()
//=----------------------------------------------------------------------------
void Box2dBodyRef::setWorld( Box2dWorldRef * world )
{
    AssertFatal( this->mWorld == NULL, 
        "Box2dBodyRef::setWorld() - Must only be called once on creation." );

    this->mWorld = world;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::setNext()
//=----------------------------------------------------------------------------
void Box2dBodyRef::setNext( Box2dBodyRef * bodyRef )
{
    this->mNext = bodyRef;
}


//=----------------------------------------------------------------------------
// Box2dBodyRef::destroy()
//=----------------------------------------------------------------------------
void Box2dBodyRef::destroy()
{
    AssertFatal( this->mSceneObject || this->mWorld->getGroundBody() == this, 
        "Box2dBodyRef::destroy() - No SceneObject" );
    AssertFatal( this->mBody != NULL, "Box2dBodyRef::destroy() - No b2Body" );

    if ( this->mSceneObject )
    {
        this->mSceneObject->setDataField( 
            StringTable->insert( "box2d_can_be_removed" ), NULL, "1" 
            );

        this->mSceneObject->safeDelete();
        this->mSceneObject = NULL;
    }

    this->mBody = NULL;

    this->deleteObject();
}
