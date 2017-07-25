package.cpath = package.cpath .. ";./build/?.so"
local heapq = require "heapq"
local floor = math.floor 
local sfmt = string.format

local session = 0
local function new_session()
    session = session + 1
    return session
end

local function rnd_value(sta, _end)
    if sta == _end then
        return sta
    end
    if sta > _end then
        sta,_end = _end,sta
    end
    local sub = _end - sta
    local mid = math.random(sub)
    return sta + mid/sub*(sub - math.random(2) + 1)
end

local function table_str(mt, max_floor, cur_floor)
    cur_floor = cur_floor or 1
    max_floor = max_floor or 5
    if max_floor and cur_floor > max_floor then
        return tostring(mt)
    end
    local str
    if cur_floor == 1 then
        str = string.format("%s{\n",string.rep("--",max_floor))
    else
        str = "{\n"
    end
    for k,v in pairs(mt) do
        if type(v) == 'table' then
            v = table_str(v, max_floor, cur_floor+1)
        else
            if type(v) == 'string' then
                v = "'" .. v .. "'"
            end
            v = tostring(v) .. "\n"
        end
        str = str .. string.format("%s[%s] = %s",string.rep("--",cur_floor),k,v)
    end
    str = str .. string.format("%s}\n",string.rep("--",cur_floor-1))
    return str
end


print(sfmt("test start!! %sM",collectgarbage("count")))
math.randomseed(os.clock()*1000000)
local value_map = {}
local cq = heapq.create(2)

local i = 15
while i >= 1 do
    local id = new_session()
    local val = rnd_value(1,30)
    value_map[id] = val
    print("push id:",id,"value:",val)
    cq:push(id, val)
    i = i - 1
end

print("-----------------")

while cq:size() > 6 do
    local id = cq:pop()
    print("pop id:", id, "value:", value_map[id])
end
local id = cq:pop()
print("pop id:", id, "value:", value_map[id])

i = 5
while i > 0 do
    local val = rnd_value(1,30)
    local top_id = cq:top()
    local id = cq:cmpltepop(val)
    print("rnd val:",val,"top val:",value_map[top_id],"cur top id:",top_id,"cmp lte pop id:",id)
    i = i - 1
end
print("now cq size:",cq:size())
cq:reset()
print("reset cq:",table_str(cq:query()))

i = 5
while i >= 1 do
    local id = new_session()
    local val = rnd_value(1,30)
    value_map[id] = val
    print("push id:",id,"value:",val)
    cq:push(id, val)
    i = i - 1
end

print("-----------------")

i = 5
while i > 0 do
    local newid = new_session()
    local val = rnd_value(1,30)
    value_map[newid] = val
    local top_id = cq:top()
    local id = cq:pushpop(newid, val)
    print("last top id:",top_id,"val:",value_map[top_id],"pushpop id:",newid,"val:",val,"pop id:",id)
    i = i -1
end
print("last cq status:",table_str(cq:query()))
cq = nil
collectgarbage("collect")
print(sfmt("test end!! %sM",collectgarbage("count")))