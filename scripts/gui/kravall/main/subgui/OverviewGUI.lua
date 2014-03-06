local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"
local Tooltip = require "gui/tooltip/ToolTip"

local OverviewGUI = GUI:new({
    onUseOverview = function()

        core.log.error("onUseOverview has no function in OverviewGUI.")
    end,
})

function OverviewGUI:new(o)
    o = GUI.new(self, o)

    local function onClickAbility(self, value)
        print"asdasdasdasdqwe12"
        print( self.checked)
        o.onUseOverview()
    end

    o.localComponents = {}

    o.localComponents[#o.localComponents+1] = Checkbox:new(
        {
            checked = false,
            matOpen             = "assets/texture/ui/blitz-unselected_00.material",
            matSelected         = "assets/texture/ui/blitz-selected_00.material",
            matHoverOpen        = "assets/texture/ui/blitz-hover_00.material",
            matHoverSelected    = "assets/texture/ui/blitz-extra_00.material",
            onClick             = onClickAbility,
            toolTip				= Tooltip:new( { handler=toolTipHandler, text="Overview" } )
        })

    for _,v in pairs(o.localComponents) do
        o:addComponent(v)
    end

    o.width=0
    o.height=0
    return o
end

function OverviewGUI:setOverview(state)
    self.localComponents[1]:setChecked(state)
end

return OverviewGUI
