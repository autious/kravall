local S = {}
local assembly_loader = require "assembly_loader"

function S.new( )
    self = {}
    self.ranInit = false
    self.tickCallbacks = {}
    self.tcindex = 0 
    self.tccount = 0

    self.initCallbacks = {}
    self.icindex = 0
    self.iccount = 0
    self.asm = assembly_loader.loadPack({})


    setmetatable( self, {__index = S, __newindex = function() error"NO" end, __gc = S.destroy } )

    return self
end

function S:loadAssembly( asmtable )
    return self.asm:loadAssembly( asmtable )  
end

function S:doneLoading()
    return self.ranInit
end

function S:registerTickCallback( func )
    print "Regging tick callback"
    self.tickCallbacks[#self.tickCallbacks+1] = func
    self.tccount = #self.tickCallbacks
end

function S:registerInitCallback( func )
    print "Regging init callback"
    self.initCallbacks[#(self.initCallbacks)+1] = func
    self.iccount = #(self.initCallbacks)
    print( #(self.initCallbacks) )
end

function S:update( delta )
    -- If we finish loading all assets, run the init functions.
    if self.asm:isLoading() == false and self.ranInit == false then
        self.icindex = self.icindex + 1
        if self.icindex > self.iccount then
            self.ranInit = true
        else
            self.initCallbacks[self.icindex]()
        end
    else

        for k,v in ipairs( self.tickCallbacks ) do
            v()
        end
        -- Call one tick callback
       -- if self.tccount > 0 then
       --     self.tcindex = self.tcindex + 1
       --     if self.tcindex > self.tccount then
       --         self.tcindex = 1
       --     end
       --     self.tickCallbacks[self.tcindex]()
       -- end
    end
end

function S:destroy()
    self.asm:destroy()
    self.asm = nil
end

return S
