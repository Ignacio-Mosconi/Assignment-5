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
		int size;
	public:
		HighScore(const int& size);											
		~HighScore();											
		void addHighScore(const int& points, const string& name);							
		void showHighScore(const int& pos, int& points, string& name);	
		void clearScore();
		void resize(const int& newSize);
		int getCurrentSize();
	};
}
