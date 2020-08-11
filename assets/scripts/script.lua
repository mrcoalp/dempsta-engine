function OnInit(data)
    data.name = "Marco"
    data.counter = 0
end

function OnUpdate(data, delta)
    if (data.counter == 60) then
        print(data.name)
        data.counter = 0
    else
        data.counter = data.counter + 1
    end
end
