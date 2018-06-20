#include <windows.h>
#include <string>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <HS.h>
#include "Definitions.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	using namespace nana;
	using namespace HS;

	HighScore* leaderboard = new HighScore(SCORES);
	leaderboard->addHighScore(30, "Ross");
	leaderboard->addHighScore(50, "Monica");
	leaderboard->addHighScore(75, "Chandler");
	leaderboard->addHighScore(70, "Rachel");
	leaderboard->addHighScore(25, "Phoebe");
	leaderboard->addHighScore(40, "Joey");

	form leaderboardForm(API::make_center(WINDOW_WIDTH, WINDOW_HEIGHT), appear::decorate<>());
	leaderboardForm.caption("Leaderboards - Powered by Nana & HS");

	label title(leaderboardForm, "<bold color = 0x177717 size = 24 center> Highest Scores</>" );
	title.text_align(align::center);
	title.format(true);

	listbox scores(leaderboardForm);

	scores.append_header("Name");
	scores.append_header("Score");

	string name;
	int score;
	for (int i = 0; i < leaderboard->getCurrentSize(); i++)
	{
		leaderboard->showHighScore(i, score, name);
		scores.at(0).append({ name, to_string(score)});
	}

	button quitButton(leaderboardForm, "Quit");
	quitButton.events().click([&leaderboardForm] {
		leaderboardForm.close();
	});

	button clearButton(leaderboardForm, "Clear");
	clearButton.events().click([leaderboard, &scores] {
		leaderboard->clearScore();
		scores.clear();
	});

	button addNewButton(leaderboardForm, "Add");
	addNewButton.events().click([leaderboard, &scores] {
		leaderboard->addHighScore(50, "James");
		scores.clear();
		string name;
		int score;
		for (int i = 0; i < leaderboard->getCurrentSize(); i++)
		{
			leaderboard->showHighScore(i, score, name);
			scores.at(0).append({ name, to_string(score) });
		}
	});

	button increaseSizeButton(leaderboardForm, "+");
	increaseSizeButton.events().click([leaderboard] {
		if (leaderboard->getMaxSize() < MAX_SCORES)
			leaderboard->resize(leaderboard->getMaxSize() + 1);
	});

	button decreaseSizeButton(leaderboardForm, "-");
	decreaseSizeButton.events().click([leaderboard, &scores] {
		if (leaderboard->getMaxSize() > 1)
			leaderboard->resize(leaderboard->getMaxSize() - 1);
		scores.clear();
		string name;
		int score;
		for (int i = 0; i < leaderboard->getCurrentSize(); i++)
		{
			leaderboard->showHighScore(i, score, name);
			scores.at(0).append({ name, to_string(score) });
		}
	});

	place layout(leaderboardForm);

	layout.div("vertical <title> <weight = 75% margin = 20 scores> <weight = 10% margin = [10, 10] gap = 15 buttons>");
	layout["title"] << title;
	layout["scores"] << scores;
	layout["buttons"] << addNewButton << clearButton << increaseSizeButton << decreaseSizeButton << quitButton;
	layout.collocate();

	leaderboardForm.show();

	exec();
}