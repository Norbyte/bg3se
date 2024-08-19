pushd .

cd BG3Extender\Extender\Shared
..\..\..\External\protobuf\tools\protobuf\protoc --cpp_out=. ExtenderProtocol.proto

cd ..\..\Lua\Debugger
..\..\..\External\protobuf\tools\protobuf\protoc --cpp_out=. LuaDebug.proto

cd ..\..\Osiris\Debugger
..\..\..\External\protobuf\tools\protobuf\protoc --cpp_out=. osidebug.proto

popd
pause