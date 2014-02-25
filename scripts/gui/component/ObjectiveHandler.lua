local GUI = require "gui/GUI"
local Label = require "gui/component/TextLabel"

local Objective = require "gui/component/objective_handler/Objective"

local AnchorPlacer = require "gui/placement/AnchorPlacer"
local EastPlacer = require "gui/placement/EastPlacer"
local NorthPlacer = require "gui/placement/NorthPlacer"
local WestPlacer = require "gui/placement/WestPlacer"
local NorthWestPlacer = require "gui/placement/NorthWestPlacer"
local CenterPlacer = require "gui/placement/CenterPlacer"

local ObjectiveHandler = GUI:new()

function ObjectiveHandler:new(o)
    o = GUI.new( self, o )
   
    o.objectives = {}
    o.objectiveComponents = {}
    o.state = ""
    o.subGUI = GUI:new({xoffset=10,yoffset=10})

    o:addComponent(o.subGUI)
    o:addPlacementHandler( NorthWestPlacer )
    o.subGUI:addPlacementHandler( NorthWestPlacer )
    o.subGUI:addComponent( Label:new{label="Objectives"} )

    return o
end

function ObjectiveHandler:win()
    for _,v in pairs( self.objectives ) do
        v.state = "success"
    end 
    self.state = "success"
end

function ObjectiveHandler:loss()
    for _,v in pairs( self.objectives ) do
        v.state = "fail"
    end 
    self.state = "fail"
end

function ObjectiveHandler:isWin()
    return self.state == "success"
end

function ObjectiveHandler:isLoss()
    return self.state == "fail"
end

function ObjectiveHandler:updateObjective( obj )

    if obj.state == "success" then
        self.objectiveComponents[obj]:setLabel( "[/] " .. obj.title  )
    elseif obj.state == "fail" then                   
        self.objectiveComponents[obj]:setLabel( "[x] " .. obj.title  )
    else                                              
        self.objectiveComponents[obj]:setLabel( "[ ] " .. obj.title  )
    end

    if self.state == "" then
        local won = true
        local has = false
        for i,o in pairs( self.objectives ) do
            if o.state ~= "success" then
                won = false  
            end
            has = true
        end
        if won and has then
            self.state = "success" 
        end
        
        local fail = true
        for i,o in pairs( self.objectives ) do
            if o.state ~= "fail" then
                fail = false  
            end
            has = true
        end
        if fail and has then
            self.state = "fail"
        end
    end
end

function ObjectiveHandler:createObjective( )
    local objective = Objective:new({onObjectiveChange = function(obj) self:updateObjective(obj) end })
    self.objectives[#(self.objectives)+1] = objective

    local newLabel = Label:new{label = "UNSET OBJECTIVE"}
    self.objectiveComponents[objective] = newLabel

    self.subGUI:addComponent( newLabel )

    return objective
end

return ObjectiveHandler
