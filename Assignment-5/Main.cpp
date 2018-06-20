#include <windows.h>
#include <string>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <HS.h>

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	using namespace nana;
	using namespace HS;

	HighScore* leaderboard = new HighScore();
	leaderboard->addHighScore(30, "Peter");
	leaderboard->addHighScore(50, "Monica");
	leaderboard->addHighScore(40, "Chandler");
	leaderboard->addHighScore(70, "Rachel");

	int highestScore;
	string bestPlayer;
	leaderboard->showHighScore(0, highestScore, bestPlayer);

	form fm;
	
	label lab{ fm, "Highest Score: " + bestPlayer };
	lab.format(true);

	button btn{ fm, "Quit" };
	btn.events().click([&fm] {
		fm.close();
	});

	fm.div("vert <><<><weight=80% text><>><><weight=24<><button><>><>");
	fm["text"] << lab;
	fm["button"] << btn;
	fm.collocate();

	fm.show();

	exec();
}