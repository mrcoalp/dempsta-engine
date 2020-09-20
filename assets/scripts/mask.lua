local function changeColor(colorID)
    if colorID == 0 then
        this.color = '#ff0000d9'
    elseif colorID == 1 then
        this.color = '#00ff00'
    elseif colorID == 2 then
        this.color = '#0000ff'
    elseif colorID == 3 then
        this.color = '#ffff00'
    else
        this.color = '#ffffff'
    end
end

function OnInit(data)
    GenerateRandomSeed()
    local color = RandomNumber(0, 4)
    local multiplier = (RandomNumber(0, 1) == 0 and -1 or 1)
    data.dirX = (RandomNumber(5, 100) / 100) * multiplier
    multiplier = (RandomNumber(0, 1) == 0 and -1 or 1)
    data.dirY = (RandomNumber(5, 100) / 100) * multiplier
    changeColor(color)
end

function OnUpdate(data, delta)
    this.x = this.x + data.dirX * delta
    this.y = this.y + data.dirY * delta

    -- local buffer = DataBuffer()
    -- buffer.x = this.x
    -- buffer.y = this.y
    -- this.SendMessage('barrel_position', buffer)
end

function OnEvent(data, event, action)
end

function OnMessage(data, msgId, message, sender)
    if msgId == "welcome" then
        -- trace(this.name .. " knows " .. sender .. " chasing state: " .. tostring(message.chasing))
    end
end
