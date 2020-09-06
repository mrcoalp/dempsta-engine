function OnInit(data)
    data.dirX = 0
    data.dirY = 0
    data.speed = 20
end

function OnUpdate(data, delta)
    local x = this.x
    local y = this.y
    data.dirX = 0
    data.dirY = 0

    if IsKeyPressed(KEY_W) then
        data.dirY = 1
    elseif IsKeyPressed(KEY_S) then
        data.dirY = -1
    end

    if IsKeyPressed(KEY_A) then
        data.dirX = -1
    elseif IsKeyPressed(KEY_D) then
        data.dirX = 1
    end

    x = x + data.dirX * data.speed * delta
    y = y + data.dirY * data.speed * delta

    this.x = x
    this.y = y

    local buffer = DataBuffer()
    buffer.x = x
    buffer.y = y
    this.SendMessage('barrel_position', buffer)
end

function OnEvent(data, event, action)
end

function OnMessage(data, msgId, message, sender)
    if msgId == "welcome" then
        print(this.name .. " knows " .. sender .. " chasing state: " .. tostring(message.chasing))
    end
end
