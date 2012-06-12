/*
  ==============================================================================

    Common.h
    Created: 10 Jun 2012 7:30:05pm
    Author:  David Rowland

  ==============================================================================
*/

ParameterUpdater::ParameterUpdater (Listener& owner)
    : listener (owner)
{
}

ParameterUpdater::~ParameterUpdater()
{
}

void ParameterUpdater::addParameter (int index)
{
    paramtersToUpdate.addIfNotAlreadyThere (index);
}

void ParameterUpdater::dispatchParameters()
{
    while (paramtersToUpdate.size() > 0)
        listener.parameterUpdated (paramtersToUpdate.remove (paramtersToUpdate.size() - 1));
}