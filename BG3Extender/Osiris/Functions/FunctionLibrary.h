#pragma once

namespace bg3se
{
	class OsirisExtender;
}

namespace bg3se::esv
{
	class CustomFunctionLibrary
	{
	public:
		CustomFunctionLibrary(OsirisExtender& osiris);

		void Startup();
		void Register();
		void RegisterHelperFunctions();
		void RegisterMathFunctions();
		void RegisterStatFunctions();
		void RegisterLuaFunctions();

	private:
		OsirisExtender& osiris_;
		bool loaded_{ false };
	};

}