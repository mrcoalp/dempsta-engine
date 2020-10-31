function OnInit(data)
end

function OnUpdate(data, delta)
    local dirX = 0
    local dirY = 0

    if IsKeyPressed(KEY_W) then
        dirY = 1
    elseif IsKeyPressed(KEY_S) then
        dirY = -1
    end

    if IsKeyPressed(KEY_A) then
        dirX = -1
    elseif IsKeyPressed(KEY_D) then
        dirX = 1
    end

    if dirX ~= 0 or dirY ~= 0 then
        this.x = this.x + dirX * 10 * delta
        this.y = this.y + dirY * 10 * delta
    end
end

function OnEvent(data, event, action)
end

function OnMessage(data, msg_id, sender, message)
end
