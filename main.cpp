#include <iostream>
#include <stdexcept>
#include <array>
#include <vector>
#include <stdio.h>
#include <algorithm>
#define MIN 99999999
#define MAX -99999999

using namespace std;

const int MINIMAX_d = 5;

// toggles the board pieces according to the rule
void toggle(char (&board)[8][8], int r, int c, char current_player)
{
    vector< vector<int>> discs_to_toggle;

    char otherPlayer = (current_player == 'b') ? 'w' : 'b';

    // to find 8-neighbors
    int neibr_pos[8][2] = {{-1, -1}, {-1, 0}, {-1, 1},
        {0, -1}, {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    //progressing through a straight line
    for(auto neibr : neibr_pos)
    {
        int curr_row = r + neibr[0];
        int curr_col = c + neibr[1];

        // continue if outside of the board
        if(curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
            continue;

        char char_in_pos = board[curr_row][curr_col];
        bool toggle_this_direction = false;

        if(char_in_pos == otherPlayer)
        {
            while(char_in_pos == otherPlayer)
            {
                curr_row += neibr[0];
                curr_col += neibr[1];

                if(curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
                    break;
                char_in_pos = board[curr_row][curr_col];
            }

            // stop when player piece is reached
            if(char_in_pos == current_player)
                toggle_this_direction = true;

            if(toggle_this_direction)
            {
                curr_row = r + neibr[0];
                curr_col = c + neibr[1];
                char_in_pos = board[curr_row][curr_col];

                while(char_in_pos == otherPlayer)
                {
                    vector<int> disc = {curr_row, curr_col};
                    discs_to_toggle.push_back(disc);
                    curr_row += neibr[0];
                    curr_col += neibr[1];
                    char_in_pos = board[curr_row][curr_col];
                }

            }
        }
    }
    for(auto pos : discs_to_toggle)
        board[pos[0]][pos[1]] = current_player;
}


bool is_flippable(char board[8][8], int r, int c, char player)
{
    char otherPlayer = (player == 'b') ? 'w' : 'b';

    // 8-neighbors check
    int neibr_pos[8][2] = {{-1, -1}, {-1, 0}, {-1, 1},
        {0, -1}, {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    //traverse in a straight line
    for(auto neibr : neibr_pos)
    {
        if(r+neibr[0] > 7 || r+neibr[0] < 0 || c+neibr[1] > 7 || c+neibr[1] < 0)
        {
            continue;
        }

        char char_in_pos = board[r+neibr[0]][c+neibr[1]];

        if(char_in_pos == otherPlayer)
        {
            int curr_row = r + neibr[0];
            int curr_col = c + neibr[1];
            while(char_in_pos == otherPlayer)
            {
                curr_row += neibr[0];
                curr_col += neibr[1];
                if(curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_row < 0)
                    break;
                char_in_pos = board[curr_row][curr_col];
            }

            if(char_in_pos == player)
                return true;
        }
    }
    return false;
}

void make_move(char (&board)[8][8], int r, int c, char current_player)
{
    board[r][c] = current_player;
    toggle(board, r, c, current_player);
}

// returns the set of legal moves for player
vector<vector<int>> find_legal_moves(char board[8][8], char player)
{
    vector< vector<int>> m_list;

    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            if(board[i][j] == '-')
            {
                if(is_flippable(board, i, j, player))
                {
                    vector<int> move = {i, j};
                    m_list.push_back(move);
                }

            }
        }
    }

    return m_list;
}

//determines if a move made is legal or not
bool is_legal(char board[8][8],  vector< vector<int>> m_list, int r, int c, char player)
{
    vector<int> playermove = {r, c};
    if(board[r][c] != '-')
    {
        return false;
    }

    if ( find(m_list.begin(), m_list.end(), playermove) != m_list.end())
    {
        return true;
    }

    return false;
}

// returns black move list
vector< vector<int>> get_black_moves(char board[8][8])
{
    return find_legal_moves(board, 'b');
}

// return white legal moves list
vector<vector<int>> get_white_moves(char board[8][8])
{
    return find_legal_moves(board, 'w');
}

// for the passed-in player, print all legal moves (displayed on board update)
void show_legal_moves(char board[8][8], char player)
{
    if(player == 'b')
    {
        cout << "You have the following moves:\n";
        auto v = get_black_moves(board);
        for ( const auto &vec : v )
        {
            cout << "(" << vec[0]  << "," << vec[1] << ")  ";
        }
        cout <<  endl;
    }
    else
    {
        cout << "White legal moves:\n";
        auto x = get_white_moves(board);
        for ( const auto &vec : x )
        {
            cout << "(" << vec[0]  << "," << vec[1] << ")  ";
        }
        cout <<  endl;
    }
}

// pass in a generated move list to "pretty print" them
void show_legal_moves( vector< vector<int>> m_list)
{
    for ( const auto &vec : m_list )
    {
        cout << "(" << vec[0]  << "," << vec[1] << ")  ";
    }
    cout <<  endl;
}

// overload the << operator to "pretty print" the board
ostream& operator<<( ostream& os, const char board[8][8])
{
    cout << "   0  1  2  3  4  5  6  7\n";
    for(int i = 0; i < 8; ++i)
    {
        cout << (i) << "  ";
        for (int j = 0; j < 8; ++j)
        {
            cout << board[i][j] << "  ";
        }
        cout << '\n';
    }
    return os;
}

// used to determine if the game is ended, makes sure at least 1 player has a move to make
bool isgameover(char board[8][8])
{
    return get_black_moves(board).empty() && get_white_moves(board).empty();
}


int get_score(char board[8][8], char player)
{
    int total = 0;
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            if(board[i][j] == player)
                total += 1;
    return total;
}

// "pretty print" the winner of the game at the end of the game loop
void result(char (&board)[8][8])
{
    cout << "Total Black: " << get_score(board, 'b') << '\n';
    cout << "Total White: " << get_score(board, 'w') << '\n';
    if(get_score(board, 'b') == get_score(board, 'w'))
    {
        cout << "TIE GAME\n";
        return;
    }

    cout << ((get_score(board, 'b') > get_score(board, 'w')) ? "Black" : "White") << " wins!\n";
}

// to assign scores to the leaf nodes
int leaf_score(char board[8][8])
{

    int b_total = 0;
    int w_total = 0;

    b_total += get_black_moves(board).size();
    w_total += get_white_moves(board).size();

    b_total += get_score(board, 'b');
    w_total += get_score(board, 'w');

    if(board[0][0] == 'w')
    {
        w_total += 10;
    }
    if(board[0][0] == 'b')
    {
        b_total += 10;
    }
    if(board[7][0] == 'w')
    {
        w_total += 10;
    }
    if(board[7][0] == 'b')
    {
        b_total += 10;
    }
    if(board[0][7] == 'w')
    {
        w_total += 10;
    }
    if(board[0][7] == 'b')
    {
        b_total += 10;
    }
    if(board[7][7] == 'w')
    {
        w_total += 10;
    }
    if(board[7][7] == 'b')
    {
        b_total += 10;
    }

    return (b_total-w_total);
}

struct Node
{
    Node ** child;
    int c_count;
    vector< vector<int>> m_list;
    char state[8][8];
    int val;
};

Node * build_tree(char board[8][8], int d, char player)
{
    Node * node = new Node();
    node->m_list = (player == 'w') ? get_white_moves(board) : get_black_moves(board);
    node->c_count = node->m_list.size();
    memcpy(node->state, board, 8 * 8 * sizeof(char));
    char other_player = (player == 'w') ? 'b' : 'w';
    if (d > 0 && node->c_count > 0)
    {
        node->child = new Node * [node->c_count];
        for (int i = 0; i < node->c_count; ++i)
        {
            char tmp_board[8][8];
            memcpy(tmp_board, board, 8 * 8 * sizeof(char));
            make_move(tmp_board, node->m_list[i][0], node->m_list[i][1], player);
            node->child[i] = build_tree(tmp_board, d - 1, other_player);
        }
    }
    else
    {
        node->child = NULL;
    }

    return node;
}

int minimax(Node *pos, int d, int a, int b, bool max_player)
{
    if(d == 0 || isgameover(pos->state))
    {
        return leaf_score(pos->state);
    }
    if(max_player)
    {
        int max_val = MAX;
        for(int i = 0; i < pos->c_count; ++i)
        {
            int eval = minimax(pos->child[i], d - 1, a, b, false);
            max_val =  max(max_val, eval);
            a =  max(a, eval);

        }
        pos->val = max_val;
        return max_val;
    }
    else
    {
        int min_val = MIN;
        for(int i = 0; i < pos->c_count; ++i)
        {
            int eval = minimax(pos->child[i], d -1, a, b, true);
            min_val = min(min_val, eval);b = min(b, eval);
            if(b <= a)
                break;
        }
        pos->val = min_val;
        return min_val;
    }
}


int main()
{

    cout << "\t\t\t\t=================FLIP N WIN=================\n";

    char board[8][8];

    for(int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            board[i][j]='-';

    board[3][3] = 'w';
    board[3][4] = 'b';
    board[4][4] = 'b';
    board[4][5] = 'w';

    int moves_total = 0;
    char player = 'b'; // black always goes first
    cout << "Enter 'b' choose black or 'w' choose white: ";
    char player_piece;
    while(true)
    {
        cin>>player_piece;
        if(player_piece != 'w' && player_piece != 'b')
        {
            cout << "\nINPUT NOT ACCEPTED. PLEASE TRY AGAIN.\n";
            continue;
        }
        break;
    }
    cout << "You are now playing as " << ((player_piece == 'w') ? "white" : "black") << "!\n\n";

    char ai_piece = ((player_piece == 'w') ? 'b' : 'w');

    ///the game starts here
    while(!isgameover(board))
    {
        // to hold the list of legal moves
        vector<vector<int>> list_of_moves = find_legal_moves(board, player);
        if(list_of_moves.empty()) break;

        cout << "CURRENT SCORE\nBlack: " << get_score(board, 'b') << '\n';
        cout << "White: " << get_score(board, 'w') << '\n';

        cout << board;
        cout << '\n';
        if(player == player_piece)
        {
            show_legal_moves(board, player_piece);
            while(true)
            {
                cout << "Make your move: ";
                int r,c;
                cin>>r>>c;

                if((r<0 && r>7)&&(c<0 && c>7))
                {
                    cout << "\nINPUT NOT ACCEPTED!!\n Please provide input in the form of row <space> column.\n";
                    continue;
                }
                else
                {
                    try
                    {
                        // check if the move is legal
                        if(is_legal(board, list_of_moves, r, c, player))
                        {
                            make_move(board, r, c, player);
                        }
                        else
                        {
                            cout << "The move is ILLEGAl.\n Please insert from the list.\n";
                            continue;
                        }
                    }
                    catch( range_error& e)
                    {
                        cout << e.what() << "You are trying to get outside the board.";
                        return 1;
                    }
                    break;
                }

            }

        }
        else
        {
            auto minimax_tree = build_tree(board, MINIMAX_d, player);
            bool max_p = (player == 'b') ? true : false;
            // optimizing using alpha beta pruning
            int optim_val = minimax(minimax_tree, MINIMAX_d, MAX, MIN, max_p);
            for(int i = 0; i < minimax_tree->c_count; ++i)
            {
                if(minimax_tree->child[i]->val == optim_val)
                {
                    for(int j = 0; j < 7; ++j)
                    {
                        for(int k = 0; k < 7; ++k)
                        {
                            memcpy(board, minimax_tree->child[i]->state, 64 * sizeof(char));
                        }
                    }
                    break;
                }
            }
        }
        moves_total += 1;
        player = (player == 'w') ? 'b' : 'w';

    }
    result(board);
    return 0;
}
