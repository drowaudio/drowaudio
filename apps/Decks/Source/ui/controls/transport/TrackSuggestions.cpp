/*
  ==============================================================================

    TrackSuggestions.cpp
    Created: 6 Feb 2011 2:09:29pm
    Author:  David Rowland

  ==============================================================================
*/

#include "TrackSuggestions.h"

const char* TrackSuggestions::mixTypeNames[] = {
	"Any",
	"Plus One",
	"Minus One",
	"Mood Change",
	"Plus Two Boost"
};

TrackSuggestions::TrackSuggestions(ValueTree sourceToBaseSuggestionsOn, ValueTree libraryDatabase)
:	font (12.0f),
	numRows(0),
	currentSource(sourceToBaseSuggestionsOn),
	currentLibrary(libraryDatabase)
{
	addAndMakeVisible( mixTypeBox = new ComboBox() );
	for (int i = 0; i < numMixTypes; i++)
	{
		mixTypeBox->addItem(mixTypeNames[i], i+1);
	}
	mixTypeBox->setSelectedId(1);
	mixTypeBox->addListener(this);
	
	loadData();
	
	// Create our table component and add it to this component..
	addAndMakeVisible (table = new TableListBox ("suggestion table", this));
	table->setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2));
	table->setHeaderHeight(18);
	table->setRowHeight(16);
	table->getViewport()->setScrollBarThickness(10);
	
	// give it a border
	table->setColour (ListBox::outlineColourId, Colours::grey);
	table->setOutlineThickness (0);
	
	// Add some columns to the table header, based on the column list in our database..
	for (int i = 0; i < columnList.getNumChildren(); i++)
	{
		table->getHeader().addColumn (columnList.getChild(i)["name"],
									  columnList.getChild(i)["columnId"],
									  columnList.getChild(i)["width"],
									  50, 800,
									  TableHeaderComponent::defaultFlags);
	}	
	
	table->getHeader().setColumnWidth(Columns::Artist, 100);
	table->getHeader().setColumnWidth(Columns::Song, 150);
	
	table->getHeader().setColumnVisible (Columns::LibID, false);
	table->getHeader().setColumnVisible (Columns::Album, false);
	table->getHeader().setColumnVisible (Columns::ID, false);
	table->getHeader().setColumnVisible (Columns::Rating, false);
	table->getHeader().setColumnVisible (Columns::Length, false);
	table->getHeader().setColumnVisible (Columns::Added, false);
	table->getHeader().setColumnVisible (Columns::Location, false);
	
	table->getHeader().setSortColumnId(Columns::Score, false);
	
	setSourceTrack(currentSource, currentLibrary, mixTypeBox->getSelectedId()-1);
}

TrackSuggestions::~TrackSuggestions()
{
	deleteAllChildren();
}

void TrackSuggestions::resized()
{
	const int m = 2;
	
	mixTypeBox->setBounds(m, m, getWidth()-2*m, 20);
	table->setBounds(m, mixTypeBox->getBottom()+m, getWidth()-2*m, getHeight()-mixTypeBox->getBottom()-2*m);
}

void TrackSuggestions::paint(Graphics &g)
{
}

//==============================================================================
void TrackSuggestions::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == mixTypeBox)
	{
		setSourceTrack(currentSource, currentLibrary, mixTypeBox->getSelectedId()-1);
	}
}

void TrackSuggestions::setSourceTrack(ValueTree newSource, ValueTree libraryDatabase, int mixType)
{
	if (!newSource.isValid()) {
		return;
	}
	
	dataList.removeAllChildren(0);
	
	const double sourceBpm = double(newSource.getProperty(Columns::columnNames[Columns::BPM], 0));
	const double minBpm = sourceBpm * 0.95;
	const double maxBpm = sourceBpm * 1.05;
	
	const String sourceGenre(newSource.getProperty(Columns::columnNames[Columns::Genre]).toString());
	StringArray sourceSubGenre;
	sourceSubGenre.addTokens(newSource.getProperty(Columns::columnNames[Columns::SubGenre]).toString(), true);
	
	const String sourceKey(newSource.getProperty(Columns::columnNames[Columns::Key]).toString().trim());
	const String sourceKeyLetter (sourceKey.getLastCharacters(1));
	int sourceKeyNumber = sourceKey.trimCharactersAtEnd(sourceKeyLetter).getIntValue();
			
	bool addTrack = false;
	float totalScore = 0.0f;
	
	for(int i = 0; i < libraryDatabase.getNumChildren(); i++)
	{
		addTrack = false;
		totalScore = 0.0f;
		
		// score bpm
		double bpm = libraryDatabase.getChild(i).getProperty(Columns::columnNames[Columns::BPM]);
		if (bpm >= minBpm && bpm <= maxBpm)
		{
			float bpmScore = 100.0f - (fabsf(sourceBpm - bpm) / (maxBpm - minBpm));
			totalScore += bpmScore * 0.3;
			
			addTrack = true;
		}
		
		// score genre
		String genre = libraryDatabase.getChild(i).getProperty(Columns::columnNames[Columns::Genre]);
		if (genre == sourceGenre)
		{
			totalScore += 20.0f;
		}

		// score sub genres
		StringArray subGenre;
		subGenre.addTokens(libraryDatabase.getChild(i).getProperty(Columns::columnNames[Columns::SubGenre]), false);
		int numMatches = 0;
		int numPossibleMatches = jmax(sourceSubGenre.size(), subGenre.size());
		if (numPossibleMatches > 0)
		{
			for (int c = 0; c < sourceSubGenre.size(); c++)
			{
				for (int s = 0; s < subGenre.size(); s++) {
					if (sourceSubGenre[c] == subGenre[s]) {
						numMatches++;
					}
				}
			}
			totalScore += (numMatches / (float)numPossibleMatches) * 20.0f;
		}
		
		// score key
		String key (libraryDatabase.getChild(i).getProperty(Columns::columnNames[Columns::Key]).toString());
		if (key.isNotEmpty())
		{
			String keyLetter (key.getLastCharacters(1));
			int keyNumber = key.trimCharactersAtEnd(keyLetter).getIntValue();
			bool keyMatch = false;

			if (mixType == any)
			{
				if (keyNumber == sourceKeyNumber
					&& keyLetter == sourceKeyLetter)
				{
					keyMatch = true;
				}					
			}	
			if (mixType == any || mixType == plusOne)
			{
				int nextNumber = (sourceKeyNumber) % 12 + 1;
				if (nextNumber == keyNumber
					&& keyLetter == sourceKeyLetter)
				{
					keyMatch = true;
				}					
			}	
			if (mixType == any || mixType == minusOne)
			{
				if (((sourceKeyNumber+10) % 12 + 1) == keyNumber
					&& keyLetter == sourceKeyLetter)
				{
					keyMatch = true;
				}										
			}	
			if (mixType == any || mixType == moodChange)
			{
				if (keyNumber == sourceKeyNumber)
				{
					if (sourceKeyLetter == String("A")) {
						if (keyLetter == String("B")) {
							keyMatch = true;
						}
					}
					else if (sourceKeyLetter == String("B")) {
						if (keyLetter == String("A")) {
							keyMatch = true;
						}
					}
				}
			}	
			if (mixType == any || mixType == plusTwoBoost)
			{
				if (((sourceKeyNumber+1) % 12 + 1) == keyNumber
					&& keyLetter == sourceKeyLetter)
				{
					keyMatch = true;
				}
			}	
			
			if (keyMatch) {
				totalScore += 30.0f;
			}
		}
		
		if (addTrack)
		{
			ValueTree newTree(libraryDatabase.getChild(i).createCopy());
			newTree.setProperty(Columns::columnNames[Columns::Score], totalScore, 0);
			dataList.addChild(newTree, -1, 0);
		}
	}
	
	numRows = dataList.getNumChildren();

	table->getHeader().reSortTable();
	table->updateContent();
}

//==============================================================================
int TrackSuggestions::getNumRows()
{
	return numRows;
}

void TrackSuggestions::paintRowBackground (Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		table->hasKeyboardFocus(true) ? g.fillAll (Colours::darkorange) : g.fillAll (Colour::greyLevel(0.6));
	else
		g.fillAll (Colour::greyLevel(0.2));
}

void TrackSuggestions::paintCell (Graphics& g,
								   int rowNumber,
								   int columnId,
								   int width, int height,
								   bool rowIsSelected)
{
	rowIsSelected ? g.setColour(Colour::greyLevel(0.2)) : g.setColour(Colour::greyLevel(0.9));
	g.setFont (font);
	
	ValueTree rowElement (dataList.getChild(rowNumber));
	
	if (rowElement.isValid())
	{
		String text;
		if(columnId == Columns::Length)
			text = secondsToTimeLength(rowElement[(Columns::columnNames[columnId])].toString().getIntValue());
		else
			text = (rowElement[Columns::columnNames[columnId]].toString());
		
		g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
	}
	
	g.setColour (Colour::greyLevel(0.9).withAlpha (0.2f));
	g.fillRect (width - 1, 0, 1, height);
	g.fillRect (0, height - 1, width, 1);
}

void TrackSuggestions::sortOrderChanged (int newSortColumnId, const bool isForwards)
{
	if (newSortColumnId != 0)
	{
		if (newSortColumnId == Columns::Length
			|| newSortColumnId == Columns::BPM
			|| newSortColumnId == Columns::LibID
			|| newSortColumnId == Columns::ID
			|| newSortColumnId == Columns::Score)
		{
			ValueTreeComparators::Numerical sorter (Columns::columnNames[newSortColumnId], isForwards);
			dataList.sort (sorter, 0, false);
		}
		else {
			ValueTreeComparators::Lexicographic sorter (Columns::columnNames[newSortColumnId], isForwards);
			dataList.sort (sorter, 0, false);
		}
		
		table->updateContent();
	}
}

int TrackSuggestions::getColumnAutoSizeWidth (int columnId)
{
	int widest = 32;
	
	// find the widest bit of text in this column..
	for (int i = getNumRows(); --i >= 0;)
	{
		const ValueTree rowElement = dataList.getChild (i);
		
		if (rowElement.isValid())
		{
			const String text (rowElement[Columns::columnNames[columnId]].toString());
			
			widest = jmax (widest, font.getStringWidth (text));
		}
	}
	
	return widest + 8;
}

const String TrackSuggestions::getDragSourceDescription (const SparseSet< int > &currentlySelectedRows)
{
	if(!currentlySelectedRows.isEmpty())
	{
		ScopedPointer<XmlElement> tracksToDrag (new XmlElement("ITEMS"));
		for(int i = 0; i < currentlySelectedRows.size(); i++)
		{
			XmlElement *item (new XmlElement(*dataList.getChild(currentlySelectedRows[i]).createXml()));
			tracksToDrag->addChildElement(item);
		}
		
		return tracksToDrag->createDocument("", false, false);
	}
	return String::empty;
}

//==============================================================================
