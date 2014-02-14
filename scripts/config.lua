core.config = {
showSplash = false,
vsync = 0,
windowResizable = true,
initScreenWidth = 1280,
initScreenHeight = 720,
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
consoleFont = "assets/Fonts/ConsoleFont.font",
defaultNrFlowfields = 20,
debugRenderAreas = true,
debugRenderHoverTextBounding = false,
debugLightVolumes = false,
debugDrawGUIBorders = false,
animationQuality = 0, -- range 0 - 2, determines frames/second in animation
toggleConsoleButton = core.input.keyboard.key.GRAVE_ACCENT,
cameraScrollingSpeed = 0,
consoleOutputLevel = "warning", -- debug, warning, error, fatal
stickySelection = false,
boxSelectionGraceDistance = 0.5
}
