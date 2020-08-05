function OnInit(data)
    data.SetInt("seconds", 1)
    data.SetFloat("delta", 0)
    data.SetDouble("double", 3.14)
    data.SetBool("bool", true)
    data.SetString("string", "Seconds elapsed: ")
end

function OnUpdate(data, delta)
    local seconds = data.GetInt("seconds");
    if (data.GetFloat("delta") >= 1) then
        print(data.GetString("string") .. seconds .. "s")
        data.SetInt("seconds", seconds + 1)
        print("Saved data -> ", data.GetDouble("double"), data.GetBool("bool"), data.GetBool("inexistent_key"))
        data.SetFloat("delta", 0)
    else
        data.SetFloat("delta", data.GetFloat("delta") + delta)
    end
end
