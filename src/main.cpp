#include "pch.h"
#include "KeyPress.h"
#include <OBSE/OBSE.h>
#include <OBSE/CommandTable.h>
#include <OBSE/PluginAPI.h>
#include <RE/Oblivion.h>

namespace {

bool IsKeyPressedPapyrus(OBSE::StaticFunctionTag*, std::uint32_t keyCode) {
	return CommandsInput::IsKeyPressed(keyCode);
}

bool IsKeyPressedExPapyrus(OBSE::StaticFunctionTag*, std::uint32_t keyCode, bool checkShift, bool checkCtrl, bool checkAlt) {
	return CommandsInput::IsKeyPressedEx(keyCode, checkShift, checkCtrl, checkAlt);
}

void RegisterPapyrusFunctions() {
	OBSE::RegisterFunction("IsKeyPressed", "CommandsInput", IsKeyPressedPapyrus);
	OBSE::RegisterFunction("IsKeyPressedEx", "CommandsInput", IsKeyPressedExPapyrus);
}

} // namespace

// Plugin information
#define PLUGIN_NAME "CommandsInput"
#define PLUGIN_VERSION 0x0100

// Plugin initialization
bool OBSEPlugin_Query(const OBSEInterface* obse, PluginInfo* info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = PLUGIN_NAME;
	info->version = PLUGIN_VERSION;
	
	if(obse->isEditor)
		return false;
	
	return true;
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* obse) {
	OBSE::Init(obse, { .trampoline = true, .trampolineSize = 64 });
	RegisterPapyrusFunctions();
	RegisterCommands();
	REX::INFO("CommandsInput loaded");
	return true;
}

// Command registration
void RegisterCommands()
{
	// Register IsKeyPressed command
	OBSE::CommandInfo* cmdInfo = new OBSE::CommandInfo;
	cmdInfo->longName = "IsKeyPressed";
	cmdInfo->shortName = "ikp";
	cmdInfo->helpText = "Checks if a key is currently pressed";
	cmdInfo->isRefRequired = false;
	cmdInfo->numParams = 1;
	cmdInfo->params = new OBSE::ParamInfo[1];
	cmdInfo->params[0].type = OBSE::kParamType_Integer;
	cmdInfo->params[0].name = "keyCode";
	cmdInfo->params[0].isOptional = false;
	
	// Register IsKeyPressedEx command
	OBSE::CommandInfo* cmdInfoEx = new OBSE::CommandInfo;
	cmdInfoEx->longName = "IsKeyPressedEx";
	cmdInfoEx->shortName = "ikpe";
	cmdInfoEx->helpText = "Checks if a key is pressed with optional modifier keys";
	cmdInfoEx->isRefRequired = false;
	cmdInfoEx->numParams = 4;
	cmdInfoEx->params = new OBSE::ParamInfo[4];
	cmdInfoEx->params[0].type = OBSE::kParamType_Integer;
	cmdInfoEx->params[0].name = "keyCode";
	cmdInfoEx->params[0].isOptional = false;
	cmdInfoEx->params[1].type = OBSE::kParamType_Integer;
	cmdInfoEx->params[1].name = "checkShift";
	cmdInfoEx->params[1].isOptional = true;
	cmdInfoEx->params[2].type = OBSE::kParamType_Integer;
	cmdInfoEx->params[2].name = "checkCtrl";
	cmdInfoEx->params[2].isOptional = true;
	cmdInfoEx->params[3].type = OBSE::kParamType_Integer;
	cmdInfoEx->params[3].name = "checkAlt";
	cmdInfoEx->params[3].isOptional = true;
}
