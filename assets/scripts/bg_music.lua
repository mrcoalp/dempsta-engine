function OnInit(data)
    this.acquire_events = true
    this.PlaySound()
    data.playing = true
end

function OnUpdate(data, delta)
end

function OnEvent(data, event, action)
    if event == EVT_KEY_PRESSED then
        if action == KEY_SPACE then
            if data.playing then
                this.PauseSound()
            else
                this.ResumeSound()
            end
            data.playing = not data.playing
        end
    end
end

function OnMessage(data, msgId, message, sender)
end
