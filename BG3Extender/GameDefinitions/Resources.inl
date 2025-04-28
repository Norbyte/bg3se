BEGIN_SE()

resource::Resource* ResourceBank::GetResource(ResourceBankType type, FixedString const& resource)
{
    if (type >= ResourceBankType::Sentinel) return nullptr;

    return Container.Banks[(unsigned)type]->Resources.try_get(resource);
}

END_SE()
