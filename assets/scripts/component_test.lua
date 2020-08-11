function OnInit(data)
    data.seconds = 1
    data.delta = 0
    data.double = 3.14
    data.bool = true
    data.string = "Seconds elapsed: "
end

function OnUpdate(data, delta)
    local seconds = data.seconds
    if (data.delta >= 1) then
        print(data.string .. seconds .. "s")
        data.seconds = seconds + 1
        print("Saved data -> ", data.double, data.bool, data.inexistent_key)
        data.delta = 0
    else
        data.delta = data.delta + delta
    end
end
