#include <chrono>
#include "../src/NDDGamesMinimax.hpp"

int main()
{
    int8_t player;
    int8_t score = 2;
    int8_t bestMove;
    uint16_t position;
    std::cout << "Player: ";
    std::cin >> player;
    player = player == 'X';
    TicTacToe game = TicTacToe(player);
    while (score == 2)
    {
        game.PrintBoard();
        std::cout << std::endl;
        if (game.CurrentPlayer() == player)
        {
            do
            {
                std::cout << std::endl
                          << "Your move: ";
                std::cin >> position;
                score = game.Move(position - 1, player);
            } while (score == 4);
        }
        else
        {
            auto start = std::chrono::steady_clock::now();
            bestMove = game.BestMove();
            auto end = std::chrono::steady_clock::now();

            std::cout << std::endl
                      << "Best move: " << bestMove + 1 << " ("
                      << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1000
                      << "μs)" << std::endl;

            score = game.Move(bestMove, game.CurrentPlayer());
        }
    }
    game.PrintBoard();
    std::cout << std::endl
              << std::endl;
    if (score == -1)
        std::cout << "You win." << std::endl;
    else if (score == 1)
        std::cout << "You lose." << std::endl;
    else
        std::cout << "Draw." << std::endl;
    return 0;
}