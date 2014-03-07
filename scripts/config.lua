core.config = {
locallySavedVars = {"fullscreen"}, --Variables to save into the users home folder.
showSplash = false,
vsync = 0,
windowResizable = true,
--initScreenWidth = 2048,
--initScreenHeight = 1152,
initScreenWidth = 1280,
initScreenHeight = 720,
fullscreen = false,
showSystems = false,
entityMemoryOutputLevel = "partial", -- full, short or partial
-- List of components to render debug data for.
entityMemoryOutputComponents = { "WorldPositionComponent", 
                                 "GraphicsComponent", 
                                 "NameComponent",
                                 "AreaComponent",
                                 "AttributeComponent" },
showFramebuffers = 0,
initCameraFieldOfView = 45.0,
initCameraNearClipDistance = 1.0,
initCameraFarClipDistance = 500.0,
consoleFont = "assets/font/ConsoleFont.font",
defaultNrFlowfields = 20,
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
numberOfSystemCoresToUse = 1,

blockingOfNavMesh_Checktimer = 5,
blockingOfNavMesh_StuckDistance = 5,
blockingOfNavMesh_StuckTimerBeforeNewPath = 5
}

