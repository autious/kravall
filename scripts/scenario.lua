local S = {}
local assembly_loader = require "assembly_loader"

function S.new( )
    local self = {}
    self.ranInit = false
    self.tickCallbacks = {}
    self.tcindex = 0 
    self.tccount = 0

    self.initCallbacks = {}
    self.icindex = 0
    self.iccount = 0

    self.updateCallbacks = {}
    self.ucindex = 0
    self.uccount = 0

    self.destroyCallbacks = {}
    self.dcindex = 0
    self.dccount = 0

    self.asm = assembly_loader.loadPack({})


    setmetatable( self, {__index = S, __gc = S.destroy } )

    return self
end

function S:loadAssembly( asmtable )
    return self.asm:loadAssembly( asmtable )  
end

function S:doneLoading()
    return self.ranInit
end

function S:registerTickCallback( func )
    if type( func ) ~= "function" then
        error( "Tick callback given is not a function" )
    end
    self.tickCallbacks[#self.tickCallbacks+1] = func
    self.tccount = #self.tickCallbacks
end

function S:registerInitCallback( func )
    if type( func ) ~= "function" then
        error( "Init callback given is not a function" )
    end
    self.initCallbacks[#(self.initCallbacks)+1] = func
    self.iccount = #(self.initCallbacks)
end

function S:registerUpdateCallback( func )
    if type( func ) ~= "function" then
        error( "Update callback given is not a function" )
    end
    self.updateCallbacks[#(self.updateCallbacks)+1] = func
    self.uccount = #(self.updateCallbacks)
end

function S:registerDestroyCallback( func )
    if type( func ) ~= "function" then
        error( "Destroy callback given is not a function" )
    end
    self.destroyCallbacks[#(self.destroyCallbacks)+1] = func
    self.dccount = #(self.destroyCallbacks)
end

function S:update( delta )
    -- If we finish loading all assets, run the init functions.
    if self.asm:isLoading() == false and self.ranInit == false then
        while self.icindex < self.iccount do
            self.icindex = self.icindex + 1
            self.initCallbacks[self.icindex]()
        end
        self.ranInit = true
    else

        for k,v in ipairs( self.updateCallbacks ) do
            v(delta)
        end

         --Call one tick callback
         if self.tccount > 0 then
             self.tcindex = self.tcindex + 1
             if self.tcindex > self.tccount then
                 self.tcindex = 1
             end
             self.tickCallbacks[self.tcindex]()
         end
    end
end

function S:destroy()
    for _,v in pairs(self.destroyCallbacks) do
        v()
    end 

    self.tickCallbacks = nil
    self.initCallbacks = nil
    self.updateCallbacks = nil
    self.destroyCallbacks = nil

    self.asm:destroy()
    self.asm = nil
end

return S
