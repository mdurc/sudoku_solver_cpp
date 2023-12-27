
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include<unistd.h> 


//NOTE: THIS INCLUDES OPTIONAL PRINT STATEMENTS WITHIN THE recursion to show the process but also that make solving
//difficult puzzles take forever. Remove/comment out the statements for an instant solve.
// by default are commented out.

/*
 * Optimized solution reduces the creation of new variables and elements
 * by using references as parameters.
 *      Also using const for the const parameters (the possible move values)
 * 
 *
 * NOTE: a function to check if it is a solution is not required.
 *      All we have to recognize is if there are no more '.' in the grid.
 *      Because this means that everything that was placed was a valid_move.
 *
 *      If we finish all the options for move values and none of them work.
 *      The solution fails due to an incorrect value earlier in the recursion.
 *
 *
 * if using references. the recursive calls can't return the reference because
 * then we'd have to assign the return value to a new variable to check if it
 * is a solution or not. But we are trying to avoid extra variables.
 *
 * Therefore our recursive return value should be a boolean.
 *      True if a solution is found
 *      False if a solution is not found and the board should go back.
 *          If false, the each location that results in the false recursive
 *          call should reset their position to '.'
 * 
 */
class Solution {
public:
    //Constructor
    bool m_print{};
    Solution(std::vector<std::vector<std::string>>& b, std::string& fname, bool print) {
        m_print = print;

        //For output visual only:
        int x{int(sqrt(b.size()))};
        std::vector<std::vector<std::string>> original{b};

        std::string horiz(b.size()*3 + x + 1, '-');
        if (x>9){
            clear();
            std::cerr<<"Error. File: "<<fname<<" is too big. Max size: 25"<<std::endl;
        }else{
            solveSudoku(b, horiz, x, fname);
            print_board(b, horiz, x);
            std::cout<<"Original:"<<std::endl;
            print_board(original,horiz, x);
            sleep(2);
        }
    }

    //For output visual only:
    void print_board(const std::vector<std::vector<std::string>>& board, const std::string &horiz, const int& x) {
        std::cout<<horiz<<std::endl;
        for (int r{};r<board.size();r++){
            for (int c{};c<board.size();c++){
                if (c%x==0){
                    std::cout<<"|";
                }
                if (board[r][c]=="."){
                    std::cout<<" . ";
                }else{
                    std::cout<<std::setw(2)<<board[r][c]<<" ";
                }
            }
            if ((r+1)%x==0){
                std::cout<<"| \n"<<horiz<<std::endl;
            }else{
                std::cout<<"| "<<std::endl;
            }
        }
    }

    //For output visual only:
    void clear() {
        // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
        std::cout << "\x1B[2J\x1B[H";
    }

    void solveSudoku(std::vector<std::vector<std::string>>& board, const std::string& horiz, const int& x, std::string& fname) {
        std::vector<std::string> vals(x*x);
        for (int i = 0; i < x*x; ++i) {
            vals[i] = std::to_string(i + 1);
        }

        //For output visual only:
        bool found_solution{solve(board, vals, horiz, x)};
        clear();
        std::cout<<fname<<std::endl;
        if (found_solution) {
            std::cout << "Solved:" << std::endl;
        } else {
            std::cout << "No Solution:" << std::endl;
        }
    }

    bool solve(std::vector<std::vector<std::string>>& board,
               const std::vector<std::string>& vals, const std::string& horiz, const int& x) {
        for (int r{}; r < (x*x); r++) {
            for (int c{}; c < (x*x); c++) {

                //For output visual only:
                // Uncomment to see a progression of the board being solved
                // recursively
                if (m_print){
                    clear();
                    std::cout << "updating\n";
                    print_board(board, horiz, x);
                }

                if (board[r][c] == ".") {  // Base case if statment
                    for (std::string possible : vals) {
                        if (is_valid_move(r, c, possible, board, x)) {
                            board[r][c] = possible;
                            if (solve(board, vals, horiz, x)) {
                                return true;
                            }
                            // When solve returns false the board should reset the
                            // current location that was just attempted.

                            // NOTE: Necessary so that every recursive change
                            //      to 'board' reverts and the board is reset
                            //      to try the next value that is up at the
                            //      beginning
                            board[r][c] = '.';
                        }
                    }

                    // Base case 1. There are no possible options from vals
                    // that lead to a solution. correctly solved.
                    // The if statment was triggered, but there are no values
                    // that lead to a solve.
                    return false;
                }
            }
        }
        // Base case 2.
        // Solution is found because there are no more '.' that trigger the
        // interior if statement.
        // If there are no more '.', this means that all values were placed
        // as a *is_valid_move*, which means the entire board is solved.
        return true;
    }

    bool is_valid_move(int r, int c, std::string num,
                       const std::vector<std::vector<std::string>>& board, const int& x) {

        //check row and column
        for (int i = 0; i < (x*x); ++i) {
            if (board[r][i] == num || board[i][c] == num) {
                return false;
            }
        }

        //check 3x3
        int sr{(r / x) * x};
        int sc{(c / x) * x};
        for (int nr{sr}; nr < sr + x; nr++) {
            for (int nc{sc}; nc < sc + x; nc++) {
                if (board[nr][nc] == num) {
                    return false;
                }
            }
        }

        return true;
    }
};

int main() {

    std::vector<std::string> files {"tiny.txt","easy1.txt", "easy2.txt", "medium.txt", "mid.txt",  "hard.txt", "extreme.txt", "huge.txt"};
    //std::vector<std::string> files {"huge.txt"};

    int count{};
    int board_length{};
    std::vector<std::vector<std::string>> input_board{};
    std::vector<std::string> row{};
    std::string next_val{};
    for (std::string fname : files){
        std::ifstream input_file{"tests/"+fname};

        if (!input_file.good()) {
            std::cerr << "Error accessing file: " << fname << std::endl;
            return 1;
        }
        std::cout<<"\n"<<fname<<":\n";

        input_file>>board_length;
        while (input_file) {
            input_file >> next_val;
            row.push_back(next_val);
            count++;
            if (count % board_length == 0) {
                input_board.push_back(row);
                row.clear();
            }
        }
        if (fname=="extreme.txt" ){
            // extreme.txt takes too long if you want to show the recursion.
            Solution board_one{input_board, fname, false};
            board_one.clear();
        }else{
            Solution board_one{input_board, fname, true};
            board_one.clear();
        }

        row.clear();
        input_board.clear();
        count=0;
    }
    return 0;
}
