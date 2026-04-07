BEGIN_SE()

resource::Resource* ResourceBank::GetResource(ResourceBankType type, FixedString const& resource)
{
    if (type >= ResourceBankType::Sentinel) return nullptr;

    return Container.Banks[(unsigned)type]->Resources.get_or_default(resource);
}

END_SE()
