#pragma once

BEGIN_NS(tadpole_tree)

struct TreeStateComponent : public BaseComponent
{
	DEFINE_COMPONENT(TadpoleTreeState, "eoc::tadpole_tree::TreeStateComponent")

	uint8_t State;
};


struct PowerContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(TadpolePowers, "eoc::tadpole_tree::PowerContainerComponent")

	Array<FixedString> Powers;
};


DEFINE_TAG_COMPONENT(eoc::tadpole_tree, TadpoledComponent, Tadpoled)
DEFINE_TAG_COMPONENT(eoc::tadpole_tree, HalfIllithidComponent, HalfIllithid)
DEFINE_TAG_COMPONENT(eoc::tadpole_tree, FullIllithidComponent, FullIllithid)

END_NS()
