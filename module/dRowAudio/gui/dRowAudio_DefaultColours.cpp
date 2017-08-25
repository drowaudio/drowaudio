DefaultColours::DefaultColours()
{
    fillDefaultColours();
}

DefaultColours& DefaultColours::getInstance()
{
    static DefaultColours instance;
    return instance;
}

//==============================================================================
Colour DefaultColours::findColour (const Component& source, const int colourId) const noexcept
{
    if (source.isColourSpecified (colourId))
        return source.findColour (colourId);
    else if (source.getLookAndFeel().isColourSpecified (colourId))
        return source.getLookAndFeel().findColour (colourId);

    return colourIds.contains (colourId) ? colours[colourIds.indexOf (colourId)]
                                         : Colours::black;
}

//==============================================================================
void DefaultColours::fillDefaultColours() noexcept
{
    static const uint32 standardColours[] =
    {
        MusicLibraryTable::backgroundColourId,                      Colour::greyLevel (0.2f).getARGB(),
        MusicLibraryTable::unfocusedBackgroundColourId,             Colour::greyLevel (0.2f).getARGB(),
        MusicLibraryTable::selectedBackgroundColourId,              0xffff8c00,//Colour (Colours::darkorange).getARGB(),
        MusicLibraryTable::selectedUnfocusedBackgroundColourId,     Colour::greyLevel (0.6f).getARGB(),
        MusicLibraryTable::textColourId,                            Colour::greyLevel (0.9f).getARGB(),
        MusicLibraryTable::selectedTextColourId,                    Colour::greyLevel (0.2f).getARGB(),
        MusicLibraryTable::unfocusedTextColourId,                   Colour::greyLevel (0.9f).getARGB(),
        MusicLibraryTable::selectedUnfocusedTextColourId,           Colour::greyLevel (0.9f).getARGB(),
        MusicLibraryTable::outlineColourId,                         Colour::greyLevel (0.9f).withAlpha (0.2f).getARGB(),
        MusicLibraryTable::selectedOutlineColourId,                 Colour::greyLevel (0.9f).withAlpha (0.2f).getARGB(),
        MusicLibraryTable::unfocusedOutlineColourId,                Colour::greyLevel (0.9f).withAlpha (0.2f).getARGB(),
        MusicLibraryTable::selectedUnfocusedOutlineColourId,        Colour::greyLevel (0.9f).withAlpha (0.2f).getARGB()
    };

    for (int i = 0; i < numElementsInArray (standardColours); i += 2)
    {
        colourIds.add ((int) standardColours [i]);
        colours.add (Colour ((uint32) standardColours [i + 1]));
    }
}
