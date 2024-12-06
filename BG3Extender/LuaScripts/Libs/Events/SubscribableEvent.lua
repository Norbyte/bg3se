--- @class SubscribableEvent
--- @field Name string
local SubscribableEvent = {}

function SubscribableEvent:Instantiate(name)
    return {
        First = nil,
        NextIndex = 1,
        Name = name,
        PendingDeletions = {},
        EnterCount = 0
    }
end

function SubscribableEvent:Subscribe(handler, opts)
    opts = opts or {}
    local index = self.NextIndex
    self.NextIndex = self.NextIndex + 1

    local sub = {
        Handler = handler,
        Index = index,
        Priority = opts.Priority or 100,
        Once = opts.Once or false,
        Options = opts
    }

    self:DoSubscribe(sub)
    return index
end

function SubscribableEvent:DoSubscribeBefore(node, sub)
    sub.Prev = node.Prev
    sub.Next = node
    
    if node.Prev ~= nil then
        node.Prev.Next = sub
    else
        self.First = sub
    end

    node.Prev = sub
end

function SubscribableEvent:DoSubscribe(sub)
    if self.First == nil then 
        self.First = sub
        return
    end

    local cur = self.First
    local last

    while cur ~= nil do
        last = cur
        if sub.Priority > cur.Priority then
            self:DoSubscribeBefore(cur, sub)
            return
        end

        cur = cur.Next
    end

    last.Next = sub
    sub.Prev = last
end

function SubscribableEvent:RemoveNode(node)
    if node.Prev ~= nil then
        node.Prev.Next = node.Next
    end

    if node.Next ~= nil then
        node.Next.Prev = node.Prev
    end

    if self.First == node then
        self.First = node.Next
    end

    node.Prev = nil
    node.Next = nil
end

function SubscribableEvent:Unsubscribe(handlerIndex)
    if self.EnterCount == 0 then
        self:DoUnsubscribe(handlerIndex)
    else
        table.insert(self.PendingDeletions, handlerIndex)
    end
end

function SubscribableEvent:DoUnsubscribe(handlerIndex)
    local cur = self.First
    while cur ~= nil do
        if cur.Index == handlerIndex then
            self:RemoveNode(cur)
            return
        end

        cur = cur.Next
    end
    
    Ext.Log.PrintWarning("Attempted to remove subscriber ID " .. handlerIndex .. " for event '" .. self.Name .. "', but no such subscriber exists (maybe it was removed already?)")
end

function SubscribableEvent:ProcessUnsubscriptions()
    if self.EnterCount == 0 and #self.PendingDeletions > 0 then
        for i,handlerIndex in pairs(self.PendingDeletions) do
            self:DoUnsubscribe(handlerIndex)
        end

        self.PendingDeletions = {}
    end
end

function SubscribableEvent:Throw(event)
    self.EnterCount = self.EnterCount + 1

    local cur = self.First
    while cur ~= nil do
        if event.Stopped then
            break
        end

        local ok, result = xpcall(cur.Handler, debug.traceback, event)
        if not ok then
            Ext.Log.PrintError("Error while dispatching event " .. self.Name .. ": ", result)
        end

        if cur.Once then
            local last = cur
            cur = last.Next
            self:RemoveNode(last)
        else
            cur = cur.Next
        end
    end

    self.EnterCount = self.EnterCount - 1
    self:ProcessUnsubscriptions()
end

return Class.Create(SubscribableEvent)
