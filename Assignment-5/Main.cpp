#include <windows.h>
#include <string>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <HS.h>
#include "Definitions.h"

void loadDefaultScores(HS::HighScore* leaderboard)
{
	leaderboard->addHighScore(30, "Ross");
	leaderboard->addHighScore(50, "Monica");
	leaderboard->addHighScore(75, "Chandler");
	leaderboard->addHighScore(70, "Rachel");
	leaderboard->addHighScore(25, "Phoebe");
	leaderboard->addHighScore(40, "Joey");
}

void loadListbox(HS::HighScore* leaderboard, nana::listbox& scores)
{
	string name;
	int score;
	for (int i = 0; i < leaderboard->getCurrentSize(); i++)
	{
		leaderboard->showHighScore(i, score, name);
		scores.at(0).append({ name, to_string(score) });
	}
}

void changeSizeCountLabel(HS::HighScore* leaderboard, nana::label& sizeCount)
{
	string currentSize = to_string(leaderboard->getCurrentSize());
	string maxSize = to_string(leaderboard->getMaxSize());
	sizeCount.caption("Scores: " + currentSize + "/" + maxSize);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	using namespace nana;
	using namespace HS;

	// Creates an HS Library Leaderboard.
	HighScore* leaderboard = new HighScore(SCORES);
	loadDefaultScores(leaderboard);

	// Creates the main form of the program, as well as its parts.
	form leaderboardForm(API::make_center(WINDOW_WIDTH, WINDOW_HEIGHT), appear::decorate<>());
	leaderboardForm.caption("Leaderboards - Powered by Nana & HS");

	label title(leaderboardForm, "<bold color = 0x177717 size = 24 center> Highest Scores</>" );
	title.text_align(align::center);
	title.format(true);

	label sizeCount(leaderboardForm, "Scores: 6/6");
	sizeCount.text_align(align::center);

	listbox scores(leaderboardForm);

	scores.append_header("Name");
	scores.append_header("Score");
	scores.bgcolor(color_rgb(colors::dark_green));
	scores.fgcolor(color_rgb(colors::white));

	// Loads the previously created leaderboard into the form's listbox.
	loadListbox(leaderboard, scores);

	// Creates the secondary text box form, which is used to enter new scores to the leaderboard.
	form textBoxForm(API::make_center(300, 200), appearance(false, false, true, true, false, false, false));

	label textBoxFormTitle(textBoxForm, "<bold color = 0x177717 size = 12 center> New Score</>");
	textBoxFormTitle.text_align(align::center);
	textBoxFormTitle.format(true);

	textbox nameField(textBoxForm);
	textbox scoreField(textBoxForm);
	nameField.tip_string("Name:          ").multi_lines(false);
	scoreField.tip_string("Score:        ").multi_lines(false);

	// Defines the main form's buttons.
	button quitButton(leaderboardForm, "Quit");
	quitButton.events().click([&leaderboardForm, &textBoxForm] 
	{
		leaderboardForm.close();
		textBoxForm.close();
	});

	button clearButton(leaderboardForm, "Clear");
	clearButton.events().click([leaderboard, &scores, &sizeCount] 
	{
		leaderboard->clearScore();
		scores.clear();
		changeSizeCountLabel(leaderboard, sizeCount);
	});

	button addNewButton(leaderboardForm, "Add");
	addNewButton.events().click([&textBoxForm]
	{
		textBoxForm.show();
	});

	button increaseSizeButton(leaderboardForm, "+");
	increaseSizeButton.events().click([leaderboard, &sizeCount] 
	{
		if (leaderboard->getMaxSize() < MAX_SCORES)
		{
			leaderboard->resize(leaderboard->getMaxSize() + 1);
			changeSizeCountLabel(leaderboard, sizeCount);
		}
	});

	button decreaseSizeButton(leaderboardForm, "-");
	decreaseSizeButton.events().click([leaderboard, &scores, &sizeCount] 
	{
		if (leaderboard->getMaxSize() > 1)
		{
			leaderboard->resize(leaderboard->getMaxSize() - 1);
			scores.clear();
			loadListbox(leaderboard, scores);
			changeSizeCountLabel(leaderboard, sizeCount);
		}
	});

	// Defines the secondary form's buttons.
	button addButton(textBoxForm, "Add");
	addButton.events().click([leaderboard, &textBoxForm, &scores, &sizeCount, &nameField, &scoreField]
	{
		string newName;
		int newScore;
		nameField.getline(0, newName);
		newScore = scoreField.to_int();
		leaderboard->addHighScore(newScore, newName);
		scores.clear();
		loadListbox(leaderboard, scores);
		changeSizeCountLabel(leaderboard, sizeCount);
		textBoxForm.hide();
	});

	button cancelButton(textBoxForm, "Cancel");
	cancelButton.events().click([&textBoxForm]
	{
		textBoxForm.hide();
	});

	// Defines the layout of both forms.
	place layout(leaderboardForm);

	layout.div("vertical <weight = 15% title>"
		"<weight = 70% margin = 20 scores>"
		"<weight = 5% sizeCount>"
		"<weight = 10% margin = 10 gap = 15 buttons>");

	layout["title"] << title;
	layout["scores"] << scores;
	layout["sizeCount"] << sizeCount;
	layout["buttons"] << addNewButton << clearButton << increaseSizeButton << decreaseSizeButton << quitButton;
	layout.collocate();

	textBoxForm.div("vertical <weight = 25% title>"
		"<vertical weight = 50% gap = 10 margin = 10 arrange = [25, 25] textBoxes>"
		"<weight = 25% margin = 10 gap = 10 buttons>");
	textBoxForm["title"] << textBoxFormTitle;
	textBoxForm["textBoxes"] << nameField << scoreField;
	textBoxForm["buttons"] << addButton << cancelButton;
	textBoxForm.collocate();

	// Shows the main form.
	leaderboardForm.show();

	// Starts to receive input.
	exec();

	delete leaderboard;
}