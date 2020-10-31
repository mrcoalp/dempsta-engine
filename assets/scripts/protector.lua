function OnInit(data)
end

function OnUpdate(data, delta)
end

function OnEvent(data, event, action)
end

function OnMessage(data, msg_id, sender, message)
    if msg_id == "protect" then
        this.PlaySound()
    end
end
