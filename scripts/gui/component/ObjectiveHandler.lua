local GUI = require "gui/GUI"
local Label = require "gui/component/TextLabel"

local Objective = require "gui/component/objective_handler/Objective"

local AnchorPlacer = require "gui/placement/AnchorPlacer"
local EastPlacer = require "gui/placement/EastPlacer"
local NorthPlacer = require "gui/placement/NorthPlacer"
local WestPlacer = require "gui/placement/WestPlacer"
local NorthWestPlacer = require "gui/placement/NorthWestPlacer"
local CenterPlacer = require "gui/placement/CenterPlacer"

local ObjectiveHandler = GUI:new{
        show = true,
        showObjectives = true,
    }

function ObjectiveHandler:new(o)
   
    o.objectives = {}
    o.objectiveComponents = {}
    o.state = ""
    o.subGUI = GUI:new({xoffset=10,yoffset=10})

    o = GUI.new( self, o )

    o:addComponent(o.subGUI)
    o:addPlacementHandler( NorthWestPlacer )
    o.subGUI:addPlacementHandler( NorthWestPlacer )
    o.subGUI:addComponent( Label:new
    {
        label="(-) Objectives",
        onClick = function( self )
            o:setObjectivesShow( not o.showObjectives )
            
            if o.showObjectives then
                self:setLabel( "(-) Objectives" )
            else
                self:setLabel( "(+) Objectives" )
            end
        end
    } )

    self.cwin = function()
        o:win()
    end
     
    win = self.cwin

    self.closs = function()
        o:loss() 
    end
   
    loss = self.closs

    return o
end

function ObjectiveHandler:destroy()
    if win == self.cwin then
        win = nil
    end

    if loss == self.closs then
        loss = nil
    end

    GUI.destroy( self )
end

function ObjectiveHandler:setObjectivesShow( state )
    for i,v in pairs(self.objectiveComponents) do
        v:setShow( state and self.show ) 
    end
    self.showObjectives = state
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

function ObjectiveHandler:setShow( state )
    self.show = state
    self.subGUI:setShow( state )

    self:setObjectivesShow( self.showObjectives )
end

function ObjectiveHandler:isWin()
    return self.state == "success"
end

function ObjectiveHandler:isLoss()
    return self.state == "fail"
end

function ObjectiveHandler:isEnd()
    return self:isWin() or self:isLoss()
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
            if o.state ~= "success" and not o.bonus then
                won = false  
            end
            has = true
        end
        if won and has then
            self.state = "success" 
        end
        
        local fail = true
        for i,o in pairs( self.objectives ) do
            if o.state ~= "fail" and not o.bonus then
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
