

## How do I enable Box2D simulation for a t2dSceneGraph? ##
For a t2dSceneGraph to support Box2D simulation it has to have the script class "Box2dScene". The class can be set in TGB's scene editor or - when creating the scene manually - by setting the class field at the time of creation:
```
%scene = new t2dSceneGraph()
{
    class = "Box2dScene";
};
```

## How do I create a body in Torquescript? ##
Every Box2D body in a wrapped Box2D simulation is represented by a t2dSceneObject in a t2dSceneGraph. To create a new body you have to provide the t2dSceneObject that will represent the newly created body. This t2dSceneObject must own a Box2dBodyBehavior that acts as the body definition that specifies the body's mass, inertia, linearDamping, and everything else that normally is contained in a [b2BodyDef](http://www.box2d.org/manual.html#d0e575) object. The Box2dWorldRef::createBody() method takes this t2dSceneObject and creates the underlying b2Body object in the Box2D simulation. Here is some example code:

```
// Create the t2dSceneObject that will represent the body.
%sceneObject = new t2dStaticSprite();

// Create Box2dBodyBehavior that acts as the body definition.
%bodyBehavior = Box2dBodyBehavior.createInstance();

// Set the definition as you need it.
%bodyBehavior.Mass = 2.0;
%bodyBehavior.FixedRotation = true;
%bodyBehavior.LinearDamping = 0.7;

// Add the Behavior to the scene object.
%sceneObject.addBehavior( %bodyBehavior );

// Retrieve a reference to the Box2dWorldRef that should 
// contain the newly created body.
%worldRef = %sceneGraph.worldRef;

// Create the body
%bodyRef = %worldRef.createBody( %sceneObject );
```

## How do I add a shape to a body in Torquescript? ##
Like a body every Box2D shape is represented by a t2dSceneObject. This t2dSceneObject together with a Box2dShapeBehavior again acts as the definition that is needed to create the shape. Like in original Box2D shapes are created through the body that will own them.
```
// Create the scene object that will represent the shape.
%shapeSceneObject = new t2dAnimatedSprite();

// Create a Box2dShapeBehavior that holds the shape definition
%shapeBehavior = Box2dShapeBehavior.createInstance();

// Set the definition as you need it.
%shapeBehavior.Type = POLYGON;
%shapeBehavior.Friction = 0.4;
%shapeBehavior.Restitution = 0.55;
%shapeBehavior.FilterGroupIndex = 2;

// Don't forget to add the Behavior to the shapeSceneObject.
%shapeSceneObject.addBehavior( %shapeBehavior );

// Mind that the collision polygon of the shape is not set 
// in the Box2dShapeBehavior but in the shape scene object
// itself.
%shapeSceneObject.setCollisionPolyPrimitive( 5 );

// Place the shapeSceneObject relative to the bodySceneObject
// to specify the shape's offset from the body.
%shapeSceneObject.Position = t2dVectorAdd( %bodySceneObject.Position, "0.5 0.0" );

// Create the shape through the bodyRef
%bodySceneObject.bodyRef.createShape( %shapeSceneObject );
```

## Do I really need two separate t2dSceneObject's to represent a simple box? ##
No, you do not. The t2dSceneObject that represents a Box2D body can at the same time represent one Box2D shape. Only additional shapes have to be represented by separate t2dSceneObject. Here is an example that shows how to create a simple box that only uses one t2dSceneObject:
```
// Create the t2dSceneObject that will represent the body AND the shape.
%sceneObject = new t2dStaticSprite();

// Set up the body as you normally would do.
%bodyBehavior = Box2dBodyBehavior.createInstance();
%bodyBehavior.Mass = 2.0;
%bodyBehavior.FixedRotation = true;
%bodyBehavior.LinearDamping = 0.7;
%sceneObject.addBehavior( %bodyBehavior );

// Retrieve a reference to the Box2dWorldRef that should 
// contain the newly created body.
%worldRef = %sceneGraph.worldRef;

// Create the body
%bodyRef = %worldRef.createBody( %sceneObject );

// Now create a Box2dShapeBehavior and initialize it as needed.
%shapeBehavior = Box2dShapeBehavior.createInstance();
%shapeBehavior.Type = POLYGON;
%shapeBehavior.Friction = 0.4;
%shapeBehavior.Restitution = 0.55;
%shapeBehavior.FilterGroupIndex = 2;

// Add the Behavior to the sceneobject.
%sceneObject.addBehavior( %shapeBehavior );

// Set the collision poly of the scene object.
// It will determine the shape's collision geometry.
%sceneObject.setCollisionPolyPrimitive( 4 );

// Now create the shape by passing the scene object to
// the Box2dBodyRef's createShape() method.
%sceneObject.bodyRef.createShape( %sceneObject );
```

## Setting up an object seems to be a lot of work. Isn't there an easier way? ##
Yes, there is: Just write yourself some helper functions that do the heavy lifting.

## Can I use the scene editor to create a Box2D scene? ##
Yes, you can. Just add a Box2dBodyBehavior or Box2dShapeBehavior to the scene objects you want to represent Box2D entities. The only additional thing to know here is that shapeSceneObjects have to be mounted to their bodySceneObjects (unless of course the shapeSceneObject is the bodySceneObject).
And don't forget to set the class of the t2dSceneGraph to "Box2dScene".

## How can I create a prismatic joint, a revolute joint, or some other joint? ##
As in normal Box2D you have to use a definition object to create a joint. In contrast to bodies and shapes, joints are not represented by a t2dSceneObject. Therefore the definition object of a joint is not a Behavior but a simple SimObject that uses dynamic fields to store the definition data. Here is some sample code:
```
%jointDef = new SimObject()
{
    jointType = e_revoluteJoint;
    body1 = %bodyRef1;
    body2 = %bodyRef2;
    localAnchor1 = %bodyRef1.getLocalPoint( %bodyRef2.getPosition() );
    localAnchor2 = %bodyRef2.getLocalPoint( %bodyRef2.getPosition() );
    referenceAngle = %bodyRef2.getAngle() - %bodyRef1.getAngle();     
};
    
%joint1 = %worldRef.createJoint( %jointDef );
```
The field names must be the same as the field names of the corresponding b2JointDef. Also note that the ´body1´ and ´body2´ fields must be set to the Box2dBodyRef object of a body, not the t2dSceneObject that represents the body or the Box2dBodyBehavior that acted as the body definition.