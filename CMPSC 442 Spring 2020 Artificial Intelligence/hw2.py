############################################################
# CMPSC 442: Homework 2
############################################################

student_name = "Hongshuo Wang"

############################################################
# Imports
############################################################

import copy
# Include your imports here, if any are used.
import math
import random
from collections import deque


############################################################
# Section 1: N-Queens
############################################################

def num_placements_all(n):
    return math.factorial(n * n) / (math.factorial(n) * math.factorial(n * n - n))


def num_placements_one_per_row(n):
    return n ** n


def n_queens_valid(board):
    checked = []
    for i in board:
        if i in checked:
            return False
        else:
            for j in checked:
                if abs(i - j) == abs(board.index(i) - checked.index(j)):
                    return False
            checked.append(i)
    return True


def n_queens_solutions(n):
    output = n_queens_helper(n, [])
    while output:
        t = output.pop()
        if len(t) == n:
            yield t

        output.extend(n_queens_helper(n, t))


def n_queens_helper(n, board):
    output = []
    for i in range(n):
        if n_queens_valid(board + [i]):
            output.append(board + [i])

    return output


############################################################
# Section 2: Lights Out
############################################################

class LightsOutPuzzle(object):

    def __init__(self, board):
        self.board = board
        self.rows = 0
        self.cols = 0
        if board is not None:
            self.rows = len(board)
            if self.rows >= 0:
                self.cols = len(board[0])

    def get_board(self):
        return self.board

    def perform_move(self, row, col):
        if self.board is not None:
            board = self.get_board()
            board[row][col] = not board[row][col]
            if row != (self.rows - 1):
                board[row + 1][col] = not board[row + 1][col]

            if row != 0:
                board[row - 1][col] = not board[row - 1][col]

            if col != 0:
                board[row][col - 1] = not board[row][col - 1]

            if col != (self.cols - 1):
                board[row][col + 1] = not board[row][col + 1]

    def scramble(self):
        for i in range(self.rows):
            for j in range(self.cols):
                if random.random() < 0.5:
                    self.perform_move(i, j)

    def is_solved(self):
        board = self.board
        for i in board:
            for j in i:
                if j:
                    return False
        return True

    def copy(self):

        return LightsOutPuzzle(copy.deepcopy(self.board))

    def successors(self):
        for i in range(self.rows):
            for j in range(self.cols):
                t = self.copy()
                t.perform_move(i, j)
                yield ((i, j), t)

    def find_solution(self):

        if self.board is None:
            return None
        if self.is_solved():
            return []

        explored_state = set()

        frontier = deque()
        states = []

        frontier.append(([], self))

        while len(frontier) != 0:

            current_move, current_state = frontier.popleft()

            explored_state.add(tuple(tuple(i) for i in current_state.get_board()))

            for next_move, next_state in current_state.successors():
                solution = current_move + [next_move]

                if next_state.is_solved():
                    return solution

                next_state_tuple = tuple(tuple(i) for i in next_state.get_board())

                if tuple(next_state.get_board()) not in states and next_state_tuple not in explored_state:
                    frontier.append((solution, next_state))
                    states.append(tuple(next_state.get_board()))

        return None


def create_puzzle(rows, cols):
    return LightsOutPuzzle([[False for j in range(cols)] for i in range(rows)])


############################################################
# Section 3: Linear Disk Movement
############################################################

def solve_identical_disks(length, n):
    if length == 0:
        return []
    elif length < n:
        return None
    elif length == n:
        return []

    explored_state = set()
    states = []

    frontier = deque()

    goal_state = [0 for a in range(length - n)] + [1 for a in range(n)]
    init_state = [1 for a in range(n)] + [0 for a in range(length - n)]

    if goal_state == init_state:
        return []

    frontier.append(([], init_state))
    states.append(tuple(init_state))

    solution = []
    while len(frontier) != 0:

        current_move, current_state = frontier.popleft()

        explored_state.add(tuple(current_state))

        for next_move, next_state in identical_successors(length, current_state):

            solution = current_move + [next_move]
            if next_state == goal_state:
                return solution
            next_state_tuple = tuple(next_state)

            if tuple(next_state) not in states and next_state_tuple not in explored_state:
                frontier.append((solution, next_state))
                states.append(tuple(next_state))

    return None


def identical_successors(length, current_state):
    for i in range(length):
        if current_state[i] == 0:
            continue
        copy = current_state[:]

        if i + 1 < length and current_state[i + 1] == 0:
            current_state[i] = 0
            current_state[i + 1] = 1
            yield ((i, i + 1), current_state)

            current_state = copy[:]
        if i + 2 < length and current_state[i + 1] == 1 and current_state[i + 2] == 0:
            current_state[i] = 0
            current_state[i + 2] = 1
            yield ((i, i + 2), current_state)

            current_state = copy[:]


def solve_distinct_disks(length, n):
    if length == 0:
        return []
    elif length <= n:
        return None

    explored_state = set()
    states = []

    frontier = deque()

    goal_state = [0 for a in range(length - n)] + [n - a for a in range(n)]
    init_state = [a + 1 for a in range(n)] + [0 for a in range(length - n)]

    if goal_state == init_state:
        return []

    frontier.append(([], init_state))
    states.append(tuple(init_state))

    solution = []
    while len(frontier) != 0:

        current_move, current_state = frontier.popleft()

        explored_state.add(tuple(current_state))

        for next_move, next_state in distinct_successors(length, current_state):

            solution = current_move + [next_move]
            if next_state == goal_state:
                return solution

            next_state_tuple = tuple(next_state)

            if tuple(next_state) not in states and next_state_tuple not in explored_state:
                frontier.append((solution, next_state))
                states.append(tuple(next_state))
                print(solution)

    return None


def distinct_successors(length, current_state):
    for i in range(length):
        if current_state[i] == 0:
            continue
        copy = current_state[:]

        if i + 1 < length and current_state[i + 1] == 0:
            current_state[i + 1] = current_state[i]
            current_state[i] = 0
            yield ((i, i + 1), current_state)
            current_state = copy[:]

        if i + 2 < length and current_state[i + 1] != 0 and current_state[i + 2] == 0:
            current_state[i + 2] = current_state[i]
            current_state[i] = 0
            yield ((i, i + 2), current_state)
            current_state = copy[:]

        if i - 2 >= 0 and current_state[i - 1] != 0 and current_state[i - 2] == 0:
            current_state[i - 2] = current_state[i]
            current_state[i] = 0
            yield ((i, i - 2), current_state)

            current_state = copy[:]

        if i - 1 >= 0 and current_state[i - 1] == 0:
            current_state[i - 1] = current_state[i]
            current_state[i] = 0
            yield ((i, i - 1), current_state)

            current_state = copy[:]


############################################################
# Section 4: Feedback
############################################################

feedback_question_1 = """
20 hours
"""

feedback_question_2 = """
problem last question. It's my first to implement BFS by using python and order of the each action is still tricky
That affect the running time of the function so much.
"""

feedback_question_3 = """
still problem 2. it's so nice that I can use gui to see what I did
can you do a GUI also shows how does problem three each steps looks like instead of just get a couples of tuples
"""
