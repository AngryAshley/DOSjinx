--if app.apiVersion < 1 then
--  return app.alert("This script requires Aseprite v1.2.10-beta3")
--end

local cel = app.activeCel
if not cel then
  return app.alert("There is no active image")
end

local img = cel.image
app.command.ChangePixelFormat{ ui=false, format="indexed", dithering="none" }

local dlg = Dialog()
dlg:entry{ id="tile", label="TileID:", text="" }
dlg:button{ id="ok", text="OK" }
dlg:button{ id="cancel", text="Cancel" }
dlg:show()
local data = dlg.data

local result=""
local tileX=(data.tile*16)%256
local tileY=(data.tile*16)%256 --//

if data.ok then
 for i=0,15 do
  for j=0,15 do
   if(img:getPixel(j+tileX,i+tileY)>256 or img:getPixel(j+tileX,i+tileY)==0) then
    result=result.."\\xfa"
   else
	result=result.."\\x"..string.format("%x",img:getPixel(j+tileX,i+tileY))
   end
  end
 end
end

local dlg = Dialog()
dlg:entry{ id="result", label="Result:", text=result }
dlg:button{ id="ok", text="Thank you!" }
dlg:show()


app.refresh()