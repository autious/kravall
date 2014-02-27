local GUI = require "gui/GUI"

local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local TextSelectList = require "gui/component/TextSelectList"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local Image = require "gui/component/Image"

local AnchorPlacer = require "gui/placement/AnchorPlacer"
local EastPlacer = require "gui/placement/EastPlacer"
local NorthPlacer = require "gui/placement/NorthPlacer"
local WestPlacer = require "gui/placement/WestPlacer"
local CenterPlacer = require "gui/placement/CenterPlacer"

local Util = require "gui/placement/util"

local PrepInterface = 
{
    policeTypes = {},
    onSelectCurrentSquad = function(squadDef) end,
    onFinished = function() end,
    onSelectCreatedSquad = function (squadInstance) end,
    onRemoveSelected = function() end
}

function PrepInterface:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self

    o.gui = GUI:new()


    o.totalCash = TextLabel:new{ label="Cash: $" .. o.cashLimit , xoffset=20,  anchor="West" }
    o.title = TextLabel:new{ label="\\/ Purchase menu \\/", xoffset=20, anchor="West" }
    o.selectedCost = TextLabel:new{ label ="Cost: $0", xoffset=20,  anchor="West" }

    o.unitSelection = TextSelectList:new
    { 
        xoffset=30,
        anchor="West", 
        elements=o.policeTypes,
        onSelect = function( squadDef )
            if squadDef then
                o.selectedCost:setLabel( "Cost: $" .. squadDef.cost )
            else
                o.selectedCost:setLabel( "Cost: $0" )
            end

            o.onSelectCurrentSquad(squadDef)
        end
    }

    o.unitPurchasedTitle = TextLabel:new{ label="\\/ Bought \\/", xoffset=20,yoffset=-5, anchor="West" }

    o.unitPurchased = TextSelectList:new
    { 
        xoffset=30,
        anchor="West", 
        elements= o.createdSquads,
        onSelect = o.onSelectCreatedSquad,
    }

    o.removeSelectedButton = Button:new
    { 
        xoffset = 20,
        yoffset = -20,
        anchor = "SouthWest",
        matReleased = "assets/texture/ui/remove-button-release.material",
        matPressed = "assets/texture/ui/remove-button-press.material",
        matHover = "assets/texture/ui/remove-button-hover.material",
        
        onClick = o.onRemoveSelected,
    }

    o.doneButton = Button:new{ anchor = "SouthEast", xoffset=-10, yoffset=-10, onClick = o.onFinished }

    local comps = {}
    table.insert( comps, o.totalCash )
    table.insert( comps, o.title )
    table.insert( comps, o.selectedCost )
    table.insert( comps, o.unitSelection )
    table.insert( comps, o.unitPurchasedTitle )

    o.background = Image:new
    { 
        mat = "assets/texture/ui/prep-background_00.material",
        ignoreConstrict = true,
        yoffset = -10,
    }
    
    local width,height = Util.getTotalDimHeight( comps, 10,10 )
    o.menuSubGUI = GUI:new{ width = width, height = height, anchor="West" }

    table.insert( comps, o.background )

    o.menuSubGUI:addComponents( comps )
    o.menuSubGUI:addPlacementHandler( AnchorPlacer )
    
    o.gui:addComponent( o.menuSubGUI  )
    o.gui:addComponent( o.unitPurchased )
    o.gui:addComponent( o.doneButton )

    o.gui:addComponent( o.removeSelectedButton )

    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function PrepInterface:setSelected( squad )
   self.unitSelection:setActive( squad )
end

function PrepInterface:setRemainingMoney( value )
    self.totalCash:setLabel( "Cash: $" .. value )
end

function PrepInterface:updatePurchasedList()
    self.unitPurchased:updateList()
end

function PrepInterface:destroy()
    self.gui:destroy()  
    self.gui = nil
end

return PrepInterface
