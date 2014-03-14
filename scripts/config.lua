core.config = {
locallySavedVars = {"fullscreen","gamma","cameraForce","lut","cameraRotationSpeed"}, --Variables to save into the users home folder.
lut = "identity",
defaultGamma = 2.2,
gamma = 2.2,
showSplash = false,
vsync = 0,
windowResizable = true,
--initScreenWidth = 2048,
--initScreenHeight = 1152,
initScreenWidth = 1280,
initScreenHeight = 720,
fullscreen = false,
defaultFullscreen = false,
shadowQuality = 1, -- 0: off, 1: 2 cascade, 2: 4 cascade
shadowResolution = 512, -- min 128, max 2048
mipmapQuality = 2, -- 0: no mipmap, 1: mipmap, 2: anisotropic filtering (max supported)
showSystems = false,
entityMemoryOutputLevel = "partial", -- full, short or partial
-- List of components to render debug data for.
entityMemoryOutputComponents = { "WorldPositionComponent", 
                                 "GraphicsComponent", 
                                 "NameComponent",
                                 "AreaComponent",
                                 "AttributeComponent",
                                 "DeadUnitComponent" },
showFramebuffers = 0,
initCameraFieldOfView = 45.0,
initCameraNearClipDistance = 1.0,
initCameraFarClipDistance = 500.0,
consoleFont = "assets/font/ConsoleFont.font",
--defaultNrFlowfields = 20,
debugRenderAreas = false,
debugRenderHoverTextBounding = false,
debugLightVolumes = false,
debugDrawGUIBorders = false, --Draws blue border lines for (most) GUI elements
animationQuality = 0, -- range 0 - 2, determines frames/second in animation
toggleConsoleButton = core.input.keyboard.key.GRAVE_ACCENT,
cameraScrollingSpeed = 0,
consoleOutputLevel = "warning", -- debug, warning, error, fatal
stickySelection = false,
doubleClickDelay = 0.2,
boxSelectionGraceDistance = 0.5,
numberOfSystemCoresToUse = 4,

blockingOfNavMesh_Checktimer = 5,
blockingOfNavMesh_StuckDistance = 3,
blockingOfNavMesh_StuckTimerBeforeNewPath = 10,

playerBindings = 
{
    aggressiveStance = core.input.keyboard.key["I"],
    defensiveStance = core.input.keyboard.key["O"],
    passiveStance = core.input.keyboard.key["P"],

    attackAbility = core.input.keyboard.key["R"],
    tearGasAbility = core.input.keyboard.key["F"],
    sprintAbility = core.input.keyboard.key["V"],
    --fleeAbility = core.input.keyboard.key["Z"],
	haltOrder = core.input.keyboard.key["X"],
	
    lineFormation = core.input.keyboard.key["L"],
    circleFormation = core.input.keyboard.key["C"],
    halfCircleFormation = core.input.keyboard.key["H"],
    
    rotateSquadSelection = core.input.keyboard.key.Tab, 

	moodHighlight = core.input.keyboard.key["M"], 
},
cameraRotationSpeed = 0.4,
defaultCameraRotationSpeed = 0.4, -- should be same as rotation speed.
maxCameraRotationSpeed = 1.0,
minCameraRotationSpeed = 0.05,
cameraForce = 5.0,
defaultCameraForce = 5.0, --should be same as camera force.
maxCameraForce = 14,
minCameraForce = 0.1,
}

