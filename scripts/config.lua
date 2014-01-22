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
showFramebuffers = 1,
initCameraFieldOfView = 45.0,
initCameraNearClipDistance = 1.0,
initCameraFarClipDistance = 1000.0,
consoleFont = "assets/Fonts/ConsoleFont.font",
defaultNrFlowfields = 20,
debugRenderAreas = true,
debugLightVolumes = false,
animationQuality = 0, -- range 0 - 2, determines frames/second in animation
toggleConsoleButton = core.input.keyboard.key.GRAVE_ACCENT,
cameraScrollingSpeed = 3

}
