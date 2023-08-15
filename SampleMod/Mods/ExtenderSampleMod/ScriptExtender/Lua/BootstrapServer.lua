Ext.Utils.PrintWarning("testing???")

Ext.Osiris.RegisterListener("MessageBoxYesNoClosed", 3, "after", function (char, msgbox, option)
    Ext.Utils.PrintWarning("Selected option: " .. option)
end)

Ext.Osiris.RegisterListener("TurnEnded", 1, "after", function (char)
    if string.sub(char, -36) == GetHostCharacter() then
        Ext.Utils.PrintWarning("TurnEnded")
        Osi.OpenMessageBoxYesNo(Osi.GetHostCharacter(), "Is this thing working?")
    end
end)
