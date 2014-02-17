local scenario = require "scenario"
local scenario = require "scenario"
local scen = scenario.new()

local TestGUI = require "gui/TestGUI"

scen.name = "Protype Area"
scen.description = "Test scenario area for you"

scen.gamemode = nil

scen:registerInitCallback( function() scen.gui = TestGUI:new() end )
scen:registerUpdateCallback( function(delta)  scen.gui:update(delta) end )
scen:registerDestroyCallback( function() scen.gui:destroy() end )

function scen:load()
end

return scen
