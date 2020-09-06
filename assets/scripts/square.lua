function OnInit(data)
    this.acquire_events = true
    data.delta = 0
    data.dirX = 0
    data.dirY = 0
    data.speed = 10
    data.currentColor = -1
    data.chasing = false
    this.scale = {x = 1.5, y = 1.5, z = 0}
end

local function changeColor(colorID)
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

local function handleSecondElapsed(data)
    data.currentColor = (data.currentColor + 1) % 4
    changeColor(data.currentColor)
    if this.scale.x == 1.5 then
        this.scale = {x = 2, y = 2, z = 0}
    else
        this.scale = {x = 1.5, y = 1.5, z = 0}
    end
end

function OnUpdate(data, delta)
    if not data.chasing then
        local x = this.GetX()
        local y = this.y
        data.dirX = 0
        data.dirY = 0
    
        if IsKeyPressed(KEY_UP) then
            data.dirY = 1
        elseif IsKeyPressed(KEY_DOWN) then
            data.dirY = -1
        end
    
        if IsKeyPressed(KEY_LEFT) then
            data.dirX = -1
        elseif IsKeyPressed(KEY_RIGHT) then
            data.dirX = 1
        end
    
        x = x + data.dirX * data.speed * delta
        y = y + data.dirY * data.speed * delta
    
        this.x = x
        this.SetY(y)
    end

    if (data.delta >= 1) then
        handleSecondElapsed(data)
        data.delta = 0
    else
        data.delta = data.delta + delta
    end
end

function OnEvent(data, event, action)
    if event == EVT_KEY_PRESSED and action == KEY_SPACE then
        data.chasing = not data.chasing
        local buffer = DataBuffer()
        buffer.msg = "Hello"
        buffer.chasing = data.chasing
        this.SendMessage("welcome", buffer)
    end
end

function OnMessage(data, msg_id, message, sender)
    if msg_id == 'barrel_position' and data.chasing then
        this.x = message.x
        this.y = message.y
    end
end
