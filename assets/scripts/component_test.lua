function OnInit(data)
    data.delta = 0
    data.speedX = 5
    data.speedY = 2
    data.currentColor = -1
    data.playing = true
end

function OnUpdate(data, delta)
    if not data.playing then
        return
    end

    local x = this.GetX()
    local y = this.y

    if x > 7 or x < -7 then
        data.speedX = data.speedX * -1
    end

    if y > 4 or y < -4 then
        data.speedY = data.speedY * -1
    end

    x = x + data.speedX * delta
    y = y + data.speedY * delta

    this.x = x
    this.SetY(y)

    if (data.delta >= 1) then
        handleSecondElapsed(data)
        data.delta = 0
    else
        data.delta = data.delta + delta
    end
end

function handleSecondElapsed(data)
    data.currentColor = (data.currentColor + 1) % 4
    changeColor(data.currentColor)
    print("Entity '" .. this.name .. "': pos: x: " .. this.x .. ", y: " .. this.y .. ", z: " .. this.z)
    print("Color: " .. this.color .. ", alpha: " .. this.alpha)
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
