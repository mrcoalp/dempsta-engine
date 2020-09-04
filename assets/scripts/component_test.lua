function OnInit(data)
    data.delta = 0
    data.dirX = 0
    data.dirY = 0
    data.speed = 5
    data.currentColor = -1
    data.playing = true
end

function OnUpdate(data, delta)
    if not data.playing then
        return
    end

    local x = this.GetX()
    local y = this.y
    data.dirX = 0
    data.dirY = 0

    if IsKeyPressed(DE_KEY_UP) then
        data.dirX = -1
    elseif IsKeyPressed(DE_KEY_DOWN) then
        data.dirX = 1
    end

    if IsKeyPressed(DE_KEY_LEFT) then
        data.dirY = -1
    elseif IsKeyPressed(DE_KEY_RIGHT) then
        data.dirY = 1
    end

    x = x + data.dirX * data.speed * delta
    y = y + data.dirY * data.speed * delta

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
