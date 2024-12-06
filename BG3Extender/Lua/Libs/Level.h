#pragma once

/// <lua_module>Level</lua_module>
BEGIN_NS(lua::level)

class PathfindingSystem
{
public:
    PathfindingSystem(LevelManager& levelManager);
    ~PathfindingSystem();

    AiPath* CreatePathRequest(LuaDelegate<void(AiPath*)>&& callback);
    AiPath* CreatePathRequestImmediate();
    void ReleasePath(AiPath* path);
    void Update();

private:
    struct PathRequest
    {
        AiPath* path_;
        LuaDelegate<void (AiPath*)> callback_;
        bool immediate_;

        void Release(LevelManager& levelManager);
    };

    LevelManager& levelManager_;
    Array<PathRequest> pendingRequests_;
    DeferredLuaDelegateQueue eventQueue_;

    bool ProcessRequest(PathRequest& request);
};

END_NS()
