#pragma once

BEGIN_NS(ecl)

// Installed during engine startup, before character tasks can run.
void InitializeKeyboardMovementOverride();
bool SetKeyboardMovementOverride(bool enabled);
bool GetKeyboardMovementOverride();

END_NS()
