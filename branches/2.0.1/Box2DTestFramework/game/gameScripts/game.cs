//---------------------------------------------------------------------------------------------
// Torque Game Builder
// Copyright (C) GarageGames.com, Inc.
//---------------------------------------------------------------------------------------------

exec( "./Box2dScene.cs" );
exec( "./Box2dJointUtility.cs" );
exec( "./Box2dTest.cs" );
exec( "./Box2dTestFrameworkListboxCallbacks.cs" );
exec( "./Box2dTestFrameworkWindowCallbacks.cs" );
exec( "./Box2dTestFrameworkUtil.cs" );

exec( "./Box2dSimpleTest.cs" );
exec( "./Box2dBridgeTest.cs" );
exec( "./Box2dMotorsAndLimitsTest.cs" );
exec( "./Box2dPulleysTest.cs" );

//---------------------------------------------------------------------------------------------
// startGame
// All game logic should be set up here. This will be called by the level builder when you
// select "Run Game" or by the startup process of your game to load the first level.
//---------------------------------------------------------------------------------------------
function startGame(%level)
{
    Canvas.setContent(mainScreenGui);
    Canvas.setCursor(DefaultCursor);
   
    $Box2dTestFrameworkScene = new t2dSceneGraph()
    {
        class = "Box2dScene";
        gravity = "0 10";
    };

    new ActionMap(moveMap);
    moveMap.bind( mouse, zaxis, mousewheelfunction );
    moveMap.push();

    Box2dTestFrameworkWindow.setSceneGraph( $Box2dTestFrameworkScene );
}

//---------------------------------------------------------------------------------------------
// endGame
// Game cleanup should be done here.
//---------------------------------------------------------------------------------------------
function endGame()
{
   moveMap.pop();
   moveMap.delete();    
    
   Box2dTestFrameworkWindow.endLevel();
}

function mousewheelfunction( %value )
{
    if ( %value < 0 )
    {
        Box2dTestFrameworkWindow.setCurrentCameraZoom(
            Box2dTestFrameworkWindow.getCurrentCameraZoom() * 1.2
            );
    }
    
    if ( %value > 0 )
    {
        Box2dTestFrameworkWindow.setCurrentCameraZoom(
            Box2dTestFrameworkWindow.getCurrentCameraZoom() / 1.2
            );
    }
}
