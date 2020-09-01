function OnInit(data)
    data.seconds = 1
    data.delta = 0
    data.double = 3.14
    data.bool = true
    data.string = "Seconds elapsed: "
    data.direction = 1
end

function OnUpdate(data, delta)
    local x = this.GetX()
    local y = this.y

    if x > 7 then
        data.direction = -1
    elseif x < -7 then
        data.direction = 1
    end

    x = x + 5 * data.direction * delta
    y = y + 2 * data.direction * delta

    this.SetX(x)
    this.y = y

    local seconds = data.seconds

    if (data.delta >= 1) then
        print("Entity x pos: " .. x)
        print(data.string .. seconds .. "s")

        data.seconds = seconds + 1

        print("Saved data -> ", data.double, data.bool, data.inexistent_key)

        data.bool = not data.bool
        data.double = data.double + (data.seconds / 3)
        data.delta = 0
    else
        data.delta = data.delta + delta
    end
end
