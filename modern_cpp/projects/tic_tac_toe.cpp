#include <iostream>
#include <variant>
#include <array>
#include <vector>
#include <algorithm>

class Player
{
public:
	explicit Player(char ticker) : m_ticker{ticker}
	{}

	char GetTicker() const
	{
		return m_ticker;
	}

	int ChooseSquare()
	{
		std::cout << "Please choose a square to place your next move:\n";
		int input;
		std::cin >> input;

		return input;
	}

private:
	char m_ticker;
};

class Board
{
public:
	Board()
	{
		for (auto& it : m_boardArr)
		{
			it = ' ';
		}
	}

	void DisplayBoard() const
	{
		std::cout << std::endl;
		std::cout << m_boardArr[0] << " | " << m_boardArr[1] << " | " << m_boardArr[2] << "\n";
		std::cout << "--" << " " << "--" << "  " << "--" << "\n";
		std::cout << m_boardArr[3] << " | " << m_boardArr[4] << " | " << m_boardArr[5] << "\n";
		std::cout << "--" << " " << "--" << "  " << "--" << "\n";
		std::cout << m_boardArr[6] << " | " << m_boardArr[7] << " | " << m_boardArr[8] << "\n";
		std::cout << std::endl;
	}

	bool CheckMoveValid(const int square_idx) const
	{
		bool nStatus = (square_idx < static_cast<int>(m_boardArr.size()) 
						&& m_boardArr[square_idx] == ' ');

		return nStatus;
	}

	void PlaceMove(const char ticker, const int square_idx)
	{
		m_boardArr[square_idx] = ticker;
	}

	bool IsWin(const char ticker)
	{
		auto it = std::find_if(m_winningCombos.begin(), m_winningCombos.end(),
						[this, ticker](const auto& combo){
							return m_boardArr[combo[0]] == ticker && 
									m_boardArr[combo[1]] == ticker && 
									m_boardArr[combo[2]] == ticker;
						}
					);

		return it != m_winningCombos.end();
	}

private:
	std::array<char, 9> m_boardArr;
	const std::vector<std::vector<int>> m_winningCombos
	{
		{0,1,2},
		{3,4,5},
		{6,7,8},
		{0,3,6},
		{1,4,7},
		{2,5,8},
		{0,4,8},
		{2,4,6}
	};
};


namespace state
{
	struct DisplayBoard 
	{
	public:
		DisplayBoard() = default;
		DisplayBoard(Board* board) : m_board(board) {}

		void handle() const
		{
			m_board->DisplayBoard();
		}

	private:
		Board* m_board;
	};


	struct MakeMove
	{
	public:
		MakeMove() = default;
		MakeMove(Board* board, Player* player) : m_board{board}, m_player{player}
		{}

		void handle() const
		{
			bool validMove = false;
			int nSquare = -1;
			char tick = m_player->GetTicker();
			
			while (!validMove)
			{
				nSquare = m_player->ChooseSquare();
				validMove = m_board->CheckMoveValid(nSquare);
			}

			m_board->PlaceMove(tick, nSquare);
		}

	private:
		Board* m_board; 
		Player* m_player;

	};


	struct CheckWin
	{
	public:
		CheckWin() = default;
		CheckWin(Board* board, Player* player) : m_board{board}, m_player{player}
		{}

		bool handle() const
		{
			return m_board->IsWin(m_player->GetTicker());
		}
	private:
		Board* m_board; 
		Player* m_player;
	};


	struct GameOver
	{
	public:
		GameOver() = default;
		GameOver(Board* board) : m_board{board}
		{}

		void handle() const
		{
			m_board->DisplayBoard();
		}
	private:
		Board* m_board;
	};

	struct Nothing
	{

	};
} // namespace state



using GameState = std::variant<state::DisplayBoard,
								state::MakeMove,
								state::CheckWin,
								state::GameOver>;


class GameStateMachine;


class GameStateMachine
{
public:
	GameStateMachine() : m_gameOn{true}, m_board{}, m_turnId{0}, m_turnCounter{0}
	{
		m_state = state::DisplayBoard{&m_board};
		m_player.emplace_back('x');
		m_player.emplace_back('o');
	}

	void UpdateTurn()
	{
		m_turnId ^= 1;
		++m_turnCounter;
	}

	void RunGame()
	{
		while (m_gameOn && m_turnCounter < 9)
		{
			m_state = std::visit([this](const auto& state)
								{
									return this->Handle(state);
								},
								m_state);
		}
	}

protected:
	GameState Handle(const state::DisplayBoard& state);
	GameState Handle(const state::MakeMove& state);
	GameState Handle(const state::CheckWin& state);
	GameState Handle(const state::GameOver& state);

private:
	bool m_gameOn;
	Board m_board;
	std::vector<Player> m_player;
	unsigned int m_turnId;
	unsigned int m_turnCounter;
	GameState m_state;
};


GameState GameStateMachine::Handle(const state::DisplayBoard& state)
{
	state.handle();
	return state::MakeMove{&m_board, &m_player[m_turnId]};
}

GameState GameStateMachine::Handle(const state::MakeMove& state)
{
	state.handle();
	return state::CheckWin{&m_board, &m_player[m_turnId]};
}

GameState GameStateMachine::Handle(const state::CheckWin& state)
{
	if (state.handle() == true)
	{
		std::cout << "Player " << m_player[m_turnId].GetTicker() << " has won!!!\n";
		return state::GameOver{&m_board};
	}

	if (m_turnCounter == 8)
	{
		//If it is the last turn and no one has won
		std::cout << "It's a Draw!\n";
		return state::GameOver{&m_board};
	}

	this->UpdateTurn();
	return state::DisplayBoard{&m_board};
}

GameState GameStateMachine::Handle(const state::GameOver& state)
{
	state.handle();
	m_gameOn = false;
	return state::DisplayBoard{&m_board};
}


int main()
{
	GameStateMachine game;
	game.RunGame();
}