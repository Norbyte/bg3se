#pragma once

BEGIN_NS(esv::templates)

DEFINE_ONEFRAME_TAG_COMPONENT(esv::templates, TemplateChangedOneFrameComponent, ServerTemplateChangedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::templates, TemplateTransformedOneFrameComponent, ServerTemplateTransformedEvent)

END_NS()

BEGIN_NS(esv::tags)

struct TagsChangedEventOneFrameComponent : public BaseComponent
{
	DEFINE_COMPONENT(TagsChangedEvent, "esv::tags::TagsChangedEventOneFrameComponent")

	Array<Guid> AddedTags;
	Array<Guid> RemovedTags;
};

END_NS()

BEGIN_NS(esv::summon)

struct SpawnCreatedEventOneFrameComponent : public BaseComponent
{
	DEFINE_COMPONENT(SummonCreatedEvent, "esv::summon::SpawnCreatedEventOneFrameComponent")

	EntityHandle Entity;
};

END_NS()
