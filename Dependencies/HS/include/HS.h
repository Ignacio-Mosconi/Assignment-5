#include <iostream>
#include <list>
using namespace std;

namespace HS 
{
	class HighScore 
	{
	private:
		struct Score 
		{																
			int points;
			string name;
			bool operator<(const Score &rhs) const { return  points > rhs.points; }
		};
		list<Score>* listHighScore;
	public:
		HighScore();											
		~HighScore();											
		void addHighScore(int, string);							
		void showHighScore(int pos, int& points, string& name);	
		void clearScore();
	};
}
