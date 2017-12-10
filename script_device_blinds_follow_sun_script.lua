
t1 = os.time()

commandArray = {}

for deviceName,deviceValue in pairs(devicechanged) do
	if ((deviceName=='Sun Azimuth' or deviceName=='Sunscreen' or deviceName=='TV' or deviceName=='PC' or deviceName=='Lamellen Handmatig') and otherdevices['Lamellen Handmatig'] == 'Off') then
		sWatt = otherdevices_svalues['Zon']:match("([^;]+)")
		sWatt = tonumber(sWatt);
		
		function Remap (value, from1, to1, from2, to2)
			return (value - from1) / (to1 - from1) * (to2 - from2) + from2
		end
			
		function Turnblinds (target)
			if (target > 14) then target = 14 
			elseif (target < 1) then target = 1 end
			
			if (sLamellen ~= target) then 
				print ('LAMELLEN: ' .. 'Turning blinds to position ' .. target) 
				commandArray[#commandArray + 1]={['OpenURL']="http://"..localhost.."/json.htm?type=command&param=udevice&idx="..idxLammelenStudie.."&nvalue=0&svalue="..tostring(target) }
				elseif debug then  print ('LAMELLEN: ' .. 'Blinds are already in correct position') end
			end
		
		if (otherdevices['Lamellen DEBUG'] == 'On') then debug = true else debug = false end
		
		--Variables to customize ------------------------------------------------
			localhost = '127.0.0.1:8080'  -- Set your port. (Not the universal IP).
			sAzimuth = tonumber(otherdevices_svalues['Sun Azimuth'])
			sAltitude = tonumber(otherdevices_svalues['Sun Altitude'])
			sLamellen = tonumber (otherdevices_svalues['LamellenStudie'])
			idxLammelenStudie = 999
			sBegin, sMiddle, sEnd = 175, 245, 290 --Angles of the sun, at middle the vertical blind should rollover
			currentDate = os.date("*t");  -- sets up currentDate.[table]
		-- Below , edit at your own risk ------------------------------------------
		
		--print ('LAMELLEN: ' .. 'Azimuth is now', sAzimuth)
		--print ('LAMELLEN: ' .. 'Lammellen svalue is' .. sLamellen .. '')
		if debug then print('LAMELLEN: ' .. 'Sun Altitude is now ' .. sAltitude .. '') end
	    
	    if (sAzimuth >= sBegin and sAzimuth < sMiddle) then
			if debug then print ('LAMELLEN: Azimuth is between begin and middle, Sunscreen is ' .. otherdevices['Sunscreen'] .. ', Television is ' .. otherdevices['TV'] .. ', Sun power is ' .. sWatt) end
			if (otherdevices['Sunscreen'] == 'Closed') then sTest = 6 else sTest = math.floor(Remap (sAzimuth, sBegin, sMiddle, 6, 1)) end
			if (otherdevices['TV'] == 'On') then sTest = sTest -2 --als de tv aan staat doe lamellen iets meer dicht.
			elseif (otherdevices['PC'] == 'On') then sTest = sTest -1 end --als de pc aan staat doe lamellen iets meer dicht.
			if (sWatt < 150 and sTest < 5) then sTest = sTest +1 end --als de zon amper schijnt doe lamellen meer open
			if (sWatt == 0 and sTest < 5) then sTest = sTest +1 end
			if (sAltitude < 10 and sTest < 5) then sTest = sTest +1 end --als de zon onder is
			print ('LAMELLEN: ' .. sTest)
			Turnblinds(sTest)
			
		elseif (sAzimuth >= sMiddle and sAzimuth < sEnd and sAltitude > 0) then
			if debug then print ('LAMELLEN: Azimuth is between middle and end, Sunscreen is ' .. otherdevices['Sunscreen'] .. ', Television is ' .. otherdevices['TV'] .. ', Sun power is ' .. sWatt) end
			if (otherdevices['Sunscreen'] == 'Closed') then sTest = 9 else sTest = math.floor(Remap (sAzimuth, sMiddle, sEnd, 14, 9)) end
			if (otherdevices['TV'] == 'On') then sTest = sTest +3 --als de tv aan staat doe lamellen iets meer dicht.
			elseif (otherdevices['PC'] == 'On') then sTest = sTest +1 end --als de pc aan staat doe lamellen iets meer dicht.
			if (sWatt < 150 and sTest > 11) then sTest = sTest -2 end --als de zon amper schijnt doe lamellen meer open
			if (sWatt < 25 and sTest > 11) then sTest = sTest -1 end
			if (sAltitude < 10 and sTest > 10) then sTest = sTest -2 end
			print ('LAMELLEN: ' .. sTest)
			Turnblinds(sTest)
							
		else
			if debug then print ('LAMELLEN: ' .. "Azimuth not in range set 7") end
			Turnblinds(7)
			
		end   
	end
end
return commandArray



