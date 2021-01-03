############################################################
# CMPSC442: Homework 7
############################################################

student_name = "Hongshuo Wang"


############################################################
# Imports
############################################################

# Include your imports here, if any are used.


############################################################
# Section 1: Sudoku
############################################################

def sudoku_cells():
    return [(i, j) for i in range(9) for j in range(9)]


def sudoku_arcs():
    output = []
    cells = sudoku_cells()
    for cell_1 in cells:
        for cell_2 in cells:
            if cell_1 != cell_2:
                if cell_1[0] == cell_2[0]:
                    output += [(cell_1, cell_2)]
                elif cell_1[1] == cell_2[1]:
                    output += [(cell_1, cell_2)]
                elif (cell_1[0] // 3) == (cell_2[0] // 3):
                    if (cell_1[1] // 3) == (cell_2[1] // 3):
                        output += [(cell_1, cell_2)]

    return output


def read_board(path):
    board = {}
    pos_range = {1, 2, 3, 4, 5, 6, 7, 8, 9}
    with open(path, 'r+') as file:
        input_data = [i.strip() for i in file]
        for i in range(len(input_data)):
            for j in range(len(input_data[i])):
                if input_data[i][j] == '*':
                    board[(i, j)] = pos_range
                else:
                    board[(i, j)] = {int(input_data[i][j])}
    return board


class Sudoku(object):
    CELLS = sudoku_cells()
    ARCS = sudoku_arcs()

    def __init__(self, board):
        self.board = board

    def get_values(self, cell):
        return self.board[cell]

    def remove_inconsistent_values(self, cell1, cell2):
        if (cell1, cell2) in Sudoku.ARCS:
            if len(self.board[cell1]) >= 1 and len(self.board[cell2]) == 1:
                if next(iter(self.board[cell2])) in self.board[cell1]:
                    self.board[cell1] = self.board[cell1].difference(self.board[cell2])
                    return True
        return False

    def update_ARC(self, ARCS, cell_1):
        for i in range(9):
            a = ((i, cell_1[1]), cell_1)
            b = ((cell_1[0], i), cell_1)
            if b != (cell_1, cell_1):
                ARCS.add(b)
            if a != (cell_1, cell_1):
                ARCS.add(a)

        for i in range(3):
            for j in range(3):
                c = ((cell_1[0] // 3 * 3 + i, cell_1[1] // 3 * 3 + j), cell_1)
                if c != (cell_1, cell_1):
                    ARCS.add(c)

    def infer_ac3(self):
        ARCS = set(Sudoku.ARCS.copy())
        while len(ARCS) != 0:
            cell_1, cell_2 = ARCS.pop()

            if self.remove_inconsistent_values(cell_1, cell_2):
                self.update_ARC(ARCS, cell_1)

    def print_board(self):
        for i in range(9):
            line = []
            for j in range(9):
                line.append(self.get_values((i, j)))
            print(line)

    def unique_in_row(self, value, cell):
        for i in range(9):
            if value in self.board[(i, cell[1])] and (i, cell[1]) != cell:
                return False
        return True

    def unique_in_col(self, value, cell):
        for i in range(9):
            if value in self.board[(cell[0], i)] and (cell[0], i) != cell:
                return False
        return

    def unique_in_block(self, value, cell):
        for i in range(3):
            for j in range(3):
                c = (cell[0] // 3 * 3 + i, cell[1] // 3 * 3 + j)
                if value in self.board[c] and c != cell:
                    return False
        return True

    def infer_improved(self):
        new_assignment = True
        while new_assignment:
            if self.solvable_detector():
                return
            self.infer_ac3()
            new_assignment = False
            for cell in self.CELLS:
                if len(self.board[cell]) > 1:
                    for value in self.board[cell]:
                        if self.unique_in_block(value, cell) or self.unique_in_col(value, cell) or self.unique_in_row(
                                value, cell):
                            self.board[cell] = {value}
                            new_assignment = True

    def solvable_detector(self):
        for cell in Sudoku.CELLS:
            if len(self.board[cell]) != 1:
                return False
        return True

    def infer_with_guessing(self):
        prev_board = self.deep_copy()
        self.infer_improved()
        for cell in self.CELLS:
            if len(self.board[cell]) > 1:
                for value in self.board[cell]:
                    self.board[cell] = {value}
                    self.infer_with_guessing()
                    if self.solvable_detector():
                        return
                    else:
                        self.board = prev_board

    def deep_copy(self):
        output = {}
        for i in range(9):
            for j in range(9):
                output[(i, j)] = self.board[(i, j)]
        return output


############################################################
# Section 2: Feedback
############################################################

feedback_question_1 = """
15 hours
"""

feedback_question_2 = """
the problem 4
It's hard to reduce the running time
"""

feedback_question_3 = """
Example is much clear. The picture is nice.
Still more test cases plz
"""
