#include "NDDGamesMinimax.hpp"
#include <chrono>

bool TicTacToe::CheckBoard(uint16_t board)
{
    if ((board & 0x1C0) == 0x1C0 || (board & 0x38) == 0x38 || (board & 0x7) == 0x7)
        return true;
    if ((board & 0x124) == 0x124 || (board & 0x92) == 0x92 || (board & 0x49) == 0x49)
        return true;
    if ((board & 0x111) == 0x111 || (board & 0x54) == 0x54)
        return true;
    return false;
}

int8_t TicTacToe::EvaluateBoard(uint16_t board, uint16_t mask)
{
    scoreValue = player ? -1 : 1;

    if (CheckBoard(board & mask))
        return scoreValue;

    if (CheckBoard(~board & mask))
        return -scoreValue;

    if (mask == 0x1FF)
        return 0;

    return 2;
}

int8_t TicTacToe::Minimax(uint16_t board, uint16_t mask, bool isMax, int8_t alpha, int8_t beta, int8_t depth)
{
    int8_t score = EvaluateBoard(board, mask);

    if (score != 2)
        return score * (16 - depth);

    int8_t bestScore;

    if (isMax)
    {
        bestScore = -16;
        for (int8_t i : explorationOrder)
            if (~mask & (1 << i))
            {
                bestScore = std::max(bestScore, Minimax(board | (!player << i), mask | (1 << i), false, alpha, beta, depth++));
                alpha = std::max(alpha, bestScore);
                if (beta <= alpha)
                    break;
            }
    }
    else
    {
        bestScore = 16;
        for (int8_t i : explorationOrder)
            if (~mask & (1 << i))
            {
                bestScore = std::min(bestScore, Minimax(board | (player << i), mask | (1 << i), true, alpha, beta, depth++));
                beta = std::min(beta, bestScore);
                if (beta <= alpha)
                    break;
            }
    }

    return bestScore;
}

TicTacToe::TicTacToe(int8_t player)
{
    board = 0;
    mask = 0;
    this->player = player;
    currentPlayer = 1;
}

int8_t TicTacToe::Player()
{
    return player;
}

int8_t TicTacToe::CurrentPlayer()
{
    return currentPlayer;
}

void TicTacToe::PrintBoard()
{
    for (int8_t i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
            std::cout << std::endl;

        if (mask & (1 << i))
        {
            if (board & (1 << i))
                std::cout << "X";
            else
                std::cout << "O";
        }
        else
            std::cout << "-";

        std::cout << " ";
    }
}

int8_t TicTacToe::Move(int8_t position, int8_t player)
{
    if (player ^ currentPlayer)
        return 3;
    if (!(~mask & (1 << position)))
        return 4;
    board |= player << position;
    mask |= 1 << position;
    currentPlayer = !currentPlayer;
    return EvaluateBoard(board, mask);
}

int8_t TicTacToe::BestMove()
{
    int8_t bestMove;
    int8_t score;
    int8_t bestScore = -16;
    uint16_t newBoard;
    uint16_t newMask;
    for (int8_t i : explorationOrder)
    {
        if (~mask & (1 << i))
        {
            newBoard = board | (!player << i);
            newMask = mask | (1 << i);
            if (EvaluateBoard(newBoard, newMask) == 1)
                return i;
            score = Minimax(board | (!player << i), mask | (1 << i), false, -16, 16, 1);
            if (score > bestScore)
            {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return bestMove;
}
