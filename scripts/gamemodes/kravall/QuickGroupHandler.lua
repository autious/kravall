local QuickGroupHandler = {}

local input = require "input" 

local PossibleHotkeys = 
{
    core.input.keyboard.key["1"],
    core.input.keyboard.key["2"],
    core.input.keyboard.key["3"],
    core.input.keyboard.key["4"],
    core.input.keyboard.key["5"],
    core.input.keyboard.key["6"],
    core.input.keyboard.key["7"],
    core.input.keyboard.key["8"],
    core.input.keyboard.key["9"],
    core.input.keyboard.key["0"],
}

function QuickGroupHandler:new(o)
    o = o or {}

    assert( o.onSelectGroup, "Missing onSelectGroup callback function" )
    assert( o.onAddGroup, "Missing onAddGroup callback function" )

    setmetatable( o, self )
    self.__index = self

    o.squadHotkeySelections = {}
    o.squadSelection = {}
 
    o.onKey = function( key, scancode, action, mods )
        if action == core.input.action.Press then 
            local hotkey
            for i,v in pairs( PossibleHotkeys ) do
                if v == key then
                    hotkey = v 
                end
            end 

            if hotkey then
                --Check if we want to assign a group
                if mods["and"](mods, core.input.keyboard.mod.Control) == core.input.keyboard.mod.Control then
                    print( "Assigned selection to hotkey:" .. hotkey )
                    o.squadHotkeySelections[hotkey] = o.squadSelection
                -- Check if we want to append selection
                elseif mods["and"](mods, core.input.keyboard.mod.Shift) == core.input.keyboard.mod.Shift then
                    print( "Appending group from hotkey:" .. hotkey )
                    if o.squadHotkeySelections[hotkey] then
                        o.onAddGroup( o.squadHotkeySelections[hotkey] )
                    end
                --We want to fetch a group.
                else 
                    print( "Setting group from hotkey:" .. hotkey )
                    if o.squadHotkeySelections[hotkey] then
                        o.onSelectGroup( o.squadHotkeySelections[hotkey] )
                    end
                end
            end
        end
    end
    input.registerOnKey( o.onKey )

    return o;
end

function QuickGroupHandler:setSquadSelection( squadList )
    self.squadSelection = {}
    for _,v in  pairs( squadList ) do
        table.insert( self.squadSelection, v )
    end
end

function QuickGroupHandler:destroy()
    input.deregisterOnKey( self.onKey )    
end

return QuickGroupHandler
