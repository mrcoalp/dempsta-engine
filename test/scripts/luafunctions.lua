function OnUpdate(delta)
    return 'FPS in Lua:' .. delta;
end

function Maths(a, b, c)
    return a * b + c
end

function Object(object)
    object.m_prop = 0
    object.Setter(object.m_prop + 1)
end
