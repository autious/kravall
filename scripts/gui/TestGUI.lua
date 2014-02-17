local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local TextSelectList = require "gui/component/TextSelectList"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local AnchorPlacer = require "gui/placement/AnchorPlacer"

local TestGUI = GUI:new()

function TestGUI:new(o)
    o = GUI.new(self,o)

    
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="West"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="NorthWest"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="North"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="NorthEast"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="East"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="SouthEast"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="South"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="SouthWest"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="Center"}))

    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="West"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="NorthWest"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="North"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="NorthEast"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="East"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="SouthEast"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="South"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="SouthWest"}))
    o:addComponent(Button:new({onClick=function(source) print( source.anchor ) end, anchor="Center"}))

    o:addPlacementHandler( AnchorPlacer )

    return o
end

return TestGUI
