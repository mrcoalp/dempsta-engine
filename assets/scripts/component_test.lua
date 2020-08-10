function OnInit(data)
    data.Set("seconds", 1)
    data.Set("delta", 0)
    data.Set("double", 3.14)
    data.Set("bool", true)
    data.Set("string", "Seconds elapsed: ")
end

function OnUpdate(data, delta)
    local seconds = data.Get("seconds");
    if (data.Get("delta") >= 1) then
        print(data.Get("string") .. seconds .. "s")
        data.Set("seconds", seconds + 1)
        print("Saved data -> ", data.Get("double"), data.Get("bool"), data.Get("inexistent_key"))
        data.Set("delta", 0)
    else
        data.Set("delta", data.Get("delta") + delta)
    end
end
