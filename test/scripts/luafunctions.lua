function OnUpdate(table, index)
    local s = table[index]
    return s
end

function Maths(a, b, c)
    return a * b + c
end

function Object(object)
    object.m_prop = 0
    object.Setter(object.m_prop + 1)
end

function VecTest(a, b, c)
    local t = {a, b, c}
    return t
end
