local function changeColor(colorID)
    if colorID == 0 then
        this.color = 'ff0000d9'
    elseif colorID == 1 then
        this.color = '#00ff00'
        this.alpha = 170
    elseif colorID == 2 then
        this.color = '0x0000ff'
        this.alpha = 100
    elseif colorID == 3 then
        this.color = '0x000000'
    else
        this.color = 'ffffff'
    end
end

function OnInit(data)
    GenerateRandomSeed()
    local color = RandomNumber(0, 4)
    local multiplier = RandomNumber(0, 1)
    if multiplier == 0 then
        multiplier = -1
    else
        multiplier = 1
    end
    data.dirX = (RandomNumber(0, 100) / 100) * multiplier
    multiplier = RandomNumber(0, 1)
    if multiplier == 0 then
        multiplier = -1
    else
        multiplier = 1
    end
    data.dirY = (RandomNumber(0, 100) / 100) * multiplier
    changeColor(color)
    data.speed = 20
end

function OnUpdate(data, delta)
    this.x = this.x + data.dirX * delta
    this.y = this.y + data.dirY * delta

    local buffer = DataBuffer()
    buffer.x = this.x
    buffer.y = this.y
    this.SendMessage('barrel_position', buffer)
end

function OnEvent(data, event, action)
end

function OnMessage(data, msgId, message, sender)
    if msgId == "welcome" then
        print(this.name .. " knows " .. sender .. " chasing state: " .. tostring(message.chasing))
    end
end
