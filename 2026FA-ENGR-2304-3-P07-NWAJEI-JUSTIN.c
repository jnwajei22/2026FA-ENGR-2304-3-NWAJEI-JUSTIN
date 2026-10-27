/* 2026FA-ENGR-2304-3-P07-NWAJEI-JUSTIN */
/* ENGR 2304 - Program P07: Tic-Tac-Toe (ChatGPT-assisted) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

/* Get a whole number safely without leaving bad input in stdin. */
int read_choice(const char *message, int low, int high)
{
    char line[80];
    char *end;
    long number;
    for (;;) {
        printf("%s", message);
        if (!fgets(line, sizeof line, stdin)) return -1;
        number = strtol(line, &end, 10);
        while (isspace((unsigned char)*end)) end++;
        if (end != line && *end == '\0' && number >= low && number <= high)
            return (int)number;
        printf("Invalid choice. Enter a number from %d to %d.\n", low, high);
    }
}

void show_board(const char board[9])
{
    int i;
    printf("\n");
    for (i = 0; i < 9; i++) {
        char symbol = board[i] == ' ' ? (char)('1' + i) : board[i];
        printf(" %c ", symbol);
        if (i % 3 != 2) printf("|");
        else if (i != 8) printf("\n---+---+---\n");
    }
    printf("\n\n");
}

/* Returns X, O, D for a draw, or a blank for an unfinished game. */
char game_result(const char board[9])
{
    const int wins[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, {0,3,6},
        {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}
    };
    int i;
    for (i = 0; i < 8; i++) {
        int a = wins[i][0], b = wins[i][1], c = wins[i][2];
        if (board[a] != ' ' && board[a] == board[b] && board[b] == board[c])
            return board[a];
    }
    for (i = 0; i < 9; i++)
        if (board[i] == ' ') return ' ';
    return 'D';
}

/* Minimax: find the best available move for the computer. */
int minimax(char board[9], char computer, char human, int computer_turn, int depth)
{
    int i, score, best;
    char result = game_result(board);
    if (result == computer) return 10 - depth;
    if (result == human) return depth - 10;
    if (result == 'D') return 0;

    best = computer_turn ? -1000 : 1000;
    for (i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = computer_turn ? computer : human;
            score = minimax(board, computer, human, !computer_turn, depth + 1);
            board[i] = ' ';
            if (computer_turn && score > best) best = score;
            if (!computer_turn && score < best) best = score;
        }
    }
    return best;
}

int best_computer_move(char board[9], char computer, char human)
{
    int i, score, highest = -1000, move = -1;
    for (i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = computer;
            score = minimax(board, computer, human, 0, 1);
            board[i] = ' ';
            if (score > highest || (score == highest && rand() % 2 == 0)) {
                highest = score;
                move = i;
            }
        }
    }
    return move;
}

/* A short, standard-C delay before the computer or next player acts. */
void pause_briefly(void)
{
    clock_t start = clock();
    while ((double)(clock() - start) / CLOCKS_PER_SEC < 0.4) {
        /* Wait about 0.4 seconds. */
    }
}

int human_move(char board[9], const char *name, char mark)
{
    int position;
    for (;;) {
        char prompt[100];
        snprintf(prompt, sizeof prompt, "%s (%c), choose a square (1-9): ", name, mark);
        position = read_choice(prompt, 1, 9);
        if (position == -1) return 0;
        if (board[position - 1] == ' ') {
            board[position - 1] = mark;
            return 1;
        }
        puts("That square is occupied. Choose a different one.");
    }
}

int main(void)
{
    int again = 1;
    srand((unsigned int)time(NULL));
    puts("ENGR 2304 - TIC-TAC-TOE");

    while (again == 1) {
        char board[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};
        int mode, symbol_choice, first, move;
        char player1, player2, current, result;
        int i;

        mode = read_choice("\n1. One player vs computer\n2. Two players\nSelect mode: ", 1, 2);
        if (mode == -1) break;
        symbol_choice = read_choice("Choose your mark: 1. X   2. O: ", 1, 2);
        if (symbol_choice == -1) break;
        player1 = symbol_choice == 1 ? 'X' : 'O';
        player2 = player1 == 'X' ? 'O' : 'X';
        first = rand() % 2;
        current = first == 0 ? player1 : player2;
        printf("\nCoin toss: %s goes first (%c).\n",
               first == 0 ? "Player 1" : (mode == 1 ? "Computer" : "Player 2"),
               current);
        puts("Use the numbers 1-9 to choose a square.");
        show_board(board);

        result = ' ';
        for (i = 0; i < 9 && result == ' '; i++) {
            if (current == player1) {
                if (!human_move(board, "Player 1", player1)) return 0;
            } else if (mode == 2) {
                puts("Player 2's turn...");
                pause_briefly();
                if (!human_move(board, "Player 2", player2)) return 0;
            } else {
                puts("Computer is thinking...");
                pause_briefly();
                move = best_computer_move(board, player2, player1);
                if (move >= 0) {
                    board[move] = player2;
                    printf("Computer chose square %d.\n", move + 1);
                }
            }
            show_board(board);
            result = game_result(board);
            current = current == player1 ? player2 : player1;
        }

        if (result == 'D') puts("It's a draw!");
        else if (result == player1) puts("Player 1 wins!");
        else if (mode == 1) puts("Computer wins!");
        else puts("Player 2 wins!");

        again = read_choice("Play again? 1. Yes   2. No: ", 1, 2);
        if (again == -1) break;
    }
    puts("Thanks for playing!");
    return 0;
}

/*
CHATGPT QUERY / DEVELOPMENT RECORD

Verbatim user prompts from the ENGR 2304 ChatGPT conversation:
1. "im gonna say fuck it and submit the rest of the a series hw. then we can do the rest of the pseries."
2. "alright. only a11 remains. lets go p series"

Assignment specification supplied to ChatGPT in the course materials:
Programming Assignment 07: Tic-Tac-Toe Using Artificial Intelligence P07.
The program must (a) allow one player vs the computer or two players,
(b) let the user pick X or O, (c) choose who goes first randomly,
(d) update the displayed board after each move, (e) delay briefly before
computer or second-player moves, (f) detect a win or draw, and
(g) ask whether to play another game.

ChatGPT's implementation decisions:
- The computer opponent uses minimax to choose a move.
- Inputs are checked, and filled squares cannot be overwritten.
- Programmer-written functions handle input, drawing, game result,
  computer decisions, pauses, and human moves.

Compilation and execution check (GCC, C11):
  gcc -std=c11 -Wall -Wextra -Wpedantic -O2 \
      2026FA-ENGR-2304-3-P07-NWAJEI-JUSTIN.c -o p07
Test runs: one-player and two-player completed without errors.

This record distinguishes actual user prompts from the supplied assignment
specification. No additional user prompts are invented.
*/
