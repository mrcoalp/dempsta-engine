function OnInit(data)
end

function OnUpdate(data, delta)
end

function OnEvent(data, event, action)
end

function OnMessage(data, msgId, message, sender)
    if msgId == "protect" then
        this.PlaySound()
    end
end
