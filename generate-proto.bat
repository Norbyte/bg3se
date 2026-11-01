pushd .

cd BG3Extender

..\External\protoc\bin\protoc --cpp_out=. Extender\Shared\ExtenderProtocol.proto
..\External\protoc\bin\protoc --cpp_out=. Lua\Debugger\LuaDebug.proto
..\External\protoc\bin\protoc --cpp_out=. Osiris\Debugger\osidebug.proto

popd
