#include <iostream>

enum GameState
{
    Playing,
    Player1Won,
    Player2Won,
    Draw
};

enum CaseValue
{
    Cross = 1, 
    Circle = 2, 
    Empty = 0   
};

class Morpion
{
public:
    unsigned int m_matchStats;

public:
    Morpion()
    {
        m_matchStats = 0;
        ResetGame();
    }

    void ResetGame()
    {
        m_matchStats &= ~0b111111111111111111;  
        m_matchStats &= ~(0b1 << 18);          
    }

    void DisplayGrid()
    {
        system("cls");
        for (int i = 0; i < 9; i++)
        {
            if (i % 3 == 0) std::cout << std::endl;

            switch ((m_matchStats & (0b11 << (i * 2))) >> (i * 2))
            {
            case Empty:
                std::cout << ".";
                break;
            case Cross:
                std::cout << "x";
                break;
            case Circle:
                std::cout << "o";
                break;
            }
        }
        std::cout << std::endl;
    }

    bool PlayMove(int caseID)
    {
        unsigned int currentCellState = (m_matchStats >> (caseID * 2)) & 0b11;

        if (currentCellState != Empty)
        {
            return false;
        }

        bool isPlayer1 = ((m_matchStats >> 18) & 0b1) == 0;

        unsigned int newState = isPlayer1 ? Cross : Circle;

        m_matchStats &= ~(0b11 << (caseID * 2)); 
        m_matchStats |= (newState << (caseID * 2)); 

        m_matchStats ^= (0b1 << 18);

        return true;
    }

    bool CheckWin()
    {
        int winConditions[8][3] =
        {
            {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, 
            {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, 
            {0, 4, 8}, {2, 4, 6}           
        };

        for (auto& condition : winConditions)
        {
            unsigned int a = (m_matchStats >> (condition[0] * 2)) & 0b11;
            unsigned int b = (m_matchStats >> (condition[1] * 2)) & 0b11;
            unsigned int c = (m_matchStats >> (condition[2] * 2)) & 0b11;

            if (a != Empty && a == b && b == c)
            {
                return true;
            }
        }

        return false;
    }

    bool CheckDraw()
    {
        for (int i = 0; i < 9; i++)
        {
            if (((m_matchStats >> (i * 2)) & 0b11) == Empty)
                return false; 
        }
        return true;
    }

    void UpdateGameStats(GameState result)
    {
        switch (result)
        {
        case Player1Won:
            m_matchStats += (1 << 19);
            break;
        case Player2Won:
            m_matchStats += (1 << 24);
            break;
        case Draw:
            m_matchStats += (1 << 29);
            break;
        default:
            break;
        }
    }

    void PlayGame()
    {
        GameState m_State = Playing;

        while (m_State == Playing)
        {
            DisplayGrid();
            int caseID;
            std::cout << "Player " << (((m_matchStats >> 18) & 0b1) == 0 ? "1 (X)" : "2 (O)") << ", enter your move (1-9): ";
            std::cin >> caseID;

            caseID -= 1;

            if (caseID < 0 || caseID > 8 || !PlayMove(caseID))
            {
                std::cout << "Invalid move! Try again." << std::endl;
                continue;
            }

            if (CheckWin())
            {
                m_State = (((m_matchStats >> 18) & 0b1) == 1) ? Player1Won : Player2Won;
                UpdateGameStats(m_State);
            }
            else if (CheckDraw())
            {
                m_State = Draw;
                UpdateGameStats(m_State);
            }
        }

        DisplayGrid();
        if (m_State == Player1Won)
        {
            std::cout << "Player 1 (X) wins!" << std::endl;
        }
        else if (m_State == Player2Won)
        {
            std::cout << "Player 2 (O) wins!" << std::endl;
        }
        else
        {
            std::cout << "It's a draw!" << std::endl;
        }
    }

    void DisplayStatistics()
    {
        unsigned int player1Wins = (m_matchStats >> 19) & 0b11111;
        unsigned int player2Wins = (m_matchStats >> 24) & 0b11111;
        unsigned int draws = (m_matchStats >> 29);

        std::cout << "Player 1 Wins: " << player1Wins << std::endl;
        std::cout << "Player 2 Wins: " << player2Wins << std::endl;
        std::cout << "Draws: " << draws << std::endl;
    }
};

int main()
{
    Morpion morpion;
    char playAgain;

    do
    {
        morpion.PlayGame();
        morpion.DisplayStatistics();
        std::cout << "Do you want to play again? (y/n): ";
        std::cin >> playAgain;
        morpion.ResetGame();
    } while (playAgain == 'y' || playAgain == 'Y');

    return 0;
}
