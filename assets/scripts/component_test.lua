function OnInit(data)
    data.seconds = 1
    data.delta = 0
    data.double = 3.14
    data.bool = true
    data.string = "Seconds elapsed: "
    data.direction = 1
    data.currentColor = -1
    data.playing = true
end

function OnUpdate(data, delta)
    if not data.playing then
        return
    end

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

    if (data.delta >= 1) then
        handleSecondElapsed(data)
    else
        data.delta = data.delta + delta
    end
end

function handleSecondElapsed(data)
    data.currentColor = (data.currentColor + 1) % 4
    changeColor(data.currentColor)

    local seconds = data.seconds
    print("Entity '" .. this.name .. "': pos: x: " .. this.x .. ", y: " .. this.y .. ", z: " .. this.z)
    print("Color: " .. this.color .. ", alpha: " .. this.alpha)
    print(data.string .. seconds .. "s")

    data.seconds = seconds + 1

    print("Saved data -> ", data.double, data.bool, data.inexistent_key, "\n")

    data.bool = not data.bool
    data.double = data.double + (data.seconds / 3)
    data.delta = 0
end

function changeColor(colorID)
    if colorID == 0 then
        this.color = 'ff0000d9'
    elseif colorID == 1 then
        this.color = '#00ff00'
        this.alpha = 170
    elseif colorID == 2 then
        this.color = '0x0000ff'
        this.alpha = 100
    else
        this.color = 'ffffff'
    end
end
