#pragma once

BEGIN_NS(lua)

struct EmptyEventParams : public lua::EventBase
{
};

struct DoConsoleCommandEventParams : public lua::EventBase
{
    [[bg3::readonly]] STDString Command;
};

struct TickEventParams : public lua::EventBase
{
    GameTime Time;
};

struct NetMessageEventParams : public lua::EventBase
{
    StringView Channel;
    StringView Payload;
    std::optional<Guid> Module;
    std::optional<net::RequestId> RequestId;
    UserId UserID;
    bool Binary{ false };
};

struct FindPathEventParams : public lua::EventBase
{
    AiPath* Path{ nullptr };
};

END_NS()
