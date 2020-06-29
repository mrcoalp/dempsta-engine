local s = Script(20)
print(s.GetProp())
print(s.m_bool)
print(s.m_string)

function OnUpdate(delta)
    print('FPS in Lua:', 1 / delta);
end

function ReturnAlgo(asd)
    print(asd);
    return 24;
end
