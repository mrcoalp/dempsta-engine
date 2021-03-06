function OnInit(data)
    this.acquire_events = true
    data.delta = 0
    data.speed = 10
    data.currentColor = -1
    data.moving = true
    data.mult = 1
    this.x = -2.5
    this.scale = {
        x = 1.5,
        y = 1.5,
        z = 1
    }
    data.changeColor = true
end

local function changeColor(colorID)
    if colorID == 0 then
        this.color = 'ff0000d9'
    elseif colorID == 1 then
        this.color = '#00ff00'
        this.alpha = 170
    elseif colorID == 2 then
        this.color = '#008080'
        this.alpha = 200
    else
        this.color = 'ffffff'
    end
end

local function handleSecondElapsed(data)
    data.currentColor = (data.currentColor + 1) % 4
    if data.changeColor then
        changeColor(data.currentColor)
    end
end

local function handleMovement(data, delta)
    local dirX = 0
    local dirY = 0

    if IsKeyPressed(KEY_UP) then
        dirY = 1
    elseif IsKeyPressed(KEY_DOWN) then
        dirY = -1
    end

    if IsKeyPressed(KEY_LEFT) then
        dirX = -1
    elseif IsKeyPressed(KEY_RIGHT) then
        dirX = 1
    end

    if dirX ~= 0 or dirY ~= 0 then
        this.x = this.x + dirX * data.speed * delta
        this.y = this.y + dirY * data.speed * delta
    end
end

function OnUpdate(data, delta)
    if data.moving then
        handleMovement(data, delta)

        if this.scale.x > 3 then
            data.mult = -1
        end
        if this.scale.x < 1.5 then
            data.mult = 1
        end

        this.scale = {
            x = this.scale.x + data.mult * delta,
            y = this.scale.y + data.mult * delta,
            z = this.scale.z
        }
    end

    if (data.delta >= 1) then
        handleSecondElapsed(data)
        data.delta = 0
    else
        data.delta = data.delta + delta
    end
end

function OnEvent(data, event, action)
    if event == EVT_KEY_PRESSED then
        if action == KEY_SPACE then
            data.moving = not data.moving
            data.changeColor = true
            this.SendMessage("moving", { moving = data.moving })
        end
        if action == KEY_P then
            data.moving = false
            data.changeColor = false
            this.color = '0xff0000'
            this.scale = {
                x = 5,
                y = 5,
                z = 0
            }
            this.SendMessage("protect")
        end
    end
end

function OnMessage(data, msg_id, sender, message)
end
