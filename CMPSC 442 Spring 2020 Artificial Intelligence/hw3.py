############################################################
# CMPSC 442: Homework 3
############################################################

student_name = "Hongshuo Wang"

############################################################
# Imports
############################################################


# Include your imports here, if any are used.
import random
import math
import queue


############################################################
# Section 1: Tile Puzzle
############################################################


def create_tile_puzzle(rows, cols):
    output = []
    index = 1
    i = 0
    j = 0
    for i in range(rows):
        temp = []
        for j in range(cols):
            temp += [index]
            index += 1
        output.append(temp)

    output[i][j] = 0
    return TilePuzzle(output)


class TilePuzzle(object):

    # Required
    def __init__(self, board):
        self.board = board
        self.rows = len(board)
        self.cols = len(board[0])
        self.zero_i = 0
        self.zero_j = 0

        for i in range(self.rows):
            for j in range(self.cols):
                if self.board[i][j] == 0:
                    self.zero_i = i
                    self.zero_j = j

    def get_board(self):
        return self.board

    def perform_move(self, direction):
        if self.board is not None:

            i = self.zero_i
            j = self.zero_j

            if direction == "up" and i > 0:
                self.board[i][j], self.board[i - 1][j] = self.board[i - 1][j], self.board[i][j]
                self.zero_i -= 1
                return True
            elif direction == "down" and i < self.rows - 1:
                self.board[i][j], self.board[i + 1][j] = self.board[i + 1][j], self.board[i][j]
                self.zero_i += 1
                return True
            elif direction == "left" and j > 0:
                self.board[i][j], self.board[i][j - 1] = self.board[i][j - 1], self.board[i][j]
                self.zero_j -= 1
                return True
            elif direction == "right" and j < self.cols - 1:
                self.board[i][j], self.board[i][j + 1] = self.board[i][j + 1], self.board[i][j]
                self.zero_j += 1
                return True
            else:
                return False
        return False

    def scramble(self, num_moves):
        move = ["up", "down", "left", "right"]
        for i in range(num_moves):
            self.perform_move(random.choice(move))

    def is_solved(self):
        if self.board[self.rows - 1][self.cols - 1] != 0:
            return False
        else:
            t = 1
            for i in range(self.rows):
                for j in range(self.cols):
                    if i == self.rows - 1 and j == self.cols - 1:
                        continue
                    if t != self.board[i][j]:
                        return False
                    t += 1
            return True

    def copy(self):
        return TilePuzzle([[j for j in i] for i in self.board])

    def successors(self):
        move = ["up", "down", "left", "right"]
        for i in move:
            t = self.copy()
            if t.perform_move(i):
                yield (i, t)

    # Required
    def find_solutions_iddfs(self):
        limit = 0
        found = False
        moves = []
        if self.is_solved():
            yield moves

        while not found:
            for solution in self.iddfs_helper(limit, moves):
                if solution:
                    found = True
                    yield solution

            limit += 1

    def iddfs_helper(self, limit, moves):
        if limit == 0:
            yield None
        else:
            for new_move, next_state in self.successors():
                if next_state.is_solved():
                    yield moves + [new_move]
                else:
                    for new_moves in next_state.iddfs_helper(limit - 1, moves + [new_move]):
                        yield new_moves

    # Required
    def find_solution_a_star(self):
        explored_state = set()
        frontier = queue.PriorityQueue()
        solution = []
        f = 0
        g = 0
        frontier.put((f, g, solution, self))
        while not frontier.empty():
            f, g, solution, current_state = frontier.get()
            explored_state.add(tuple(tuple(i) for i in current_state.get_board()))

            if current_state.is_solved():
                return solution

            for next_move, next_state in current_state.successors():
                next_state_tuple = tuple(tuple(i) for i in next_state.get_board())
                if next_state_tuple not in explored_state:
                    frontier.put((next_state.md_heuristic() + g, g + 1, solution + [next_move], next_state))

        return None

    def md_heuristic(self):
        output = 0
        for i in range(self.rows):
            for j in range(self.cols):
                if self.board[i][j] == 0:
                    output += abs(self.rows - 1 - i) + abs(self.cols - 1 - j)
                    continue
                output += abs((self.board[i][j] - 1) % self.cols - j) + abs((self.board[i][j] - 1) // self.cols - i)
        return output


############################################################
# Section 2: Grid Navigation
############################################################

def find_path(start, goal, scene):
    explored_state = set()
    frontier = queue.PriorityQueue()
    solution = [start]
    f = 0
    g = 0
    frontier.put((f, g, solution, start))
    while not frontier.empty():
        f, g, solution, current_state = frontier.get()
        explored_state.add(current_state)
        if current_state == goal:
            return solution
        print(current_state)
        for next_p in successors(current_state, scene):
            if next_p not in explored_state:
                frontier.put((ed_heuristic(next_p, goal) + g, g + 1, solution + [next_p], next_p))

    return None


def successors(current_p, scene):
    max_x = current_p[0] + 1
    min_x = current_p[0] - 1
    min_y = current_p[1] - 1
    max_y = current_p[1] + 1
    for x in range(min_x, max_x + 1):
        for y in range(min_y, max_y + 1):
            if x >= 0 and y >= 0 and x <= len(scene) - 1 and y <= len(scene[0]) - 1:
                if not scene[x][y]:
                    yield (x, y)


def ed_heuristic(current_p, goal_p):
    x = math.pow(current_p[0] - goal_p[0], 2)
    y = math.pow(current_p[1] - goal_p[1], 2)
    return math.sqrt(x + y)


############################################################
# Section 3: Linear Disk Movement, Revisited
############################################################

def solve_distinct_disks(length, n):
    if length == 0:
        return []
    elif length <= n:
        return None

    explored_state = set()
    frontier = queue.PriorityQueue()
    solution = []
    f = 0
    g = 0
    goal_state = [0 for a in range(length - n)] + [n - a for a in range(n)]
    init_state = [a + 1 for a in range(n)] + [0 for a in range(length - n)]
    frontier.put((f, g, solution, init_state))

    while not frontier.empty():
        f, g, solution, current_state = frontier.get()
        explored_state.add(tuple(current_state))
        if current_state == goal_state:
            print(goal_state)
            return solution

        for next_move, next_state in distinct_successors(length, current_state):
            if tuple(next_state) not in explored_state:
                frontier.put((heuristic(next_state, goal_state) + g, g + 1, solution + [next_move], next_state))
    return None


def distinct_successors(length, current_state):
    for i in range(length):
        if i + 1 < length and current_state[i + 1] == 0:
            current = current_state[:]
            current[i + 1] = current[i]
            current[i] = 0
            yield (i, i + 1), current

        elif i + 2 < length and current_state[i + 1] != 0 and current_state[i + 2] == 0:
            current = current_state[:]
            current[i + 2] = current[i]
            current[i] = 0
            yield (i, i + 2), current

        elif i - 2 >= 0 and current_state[i - 1] != 0 and current_state[i - 2] == 0:
            current = current_state[:]
            current[i - 2] = current[i]
            current[i] = 0
            yield (i, i - 2), current

        elif i - 1 >= 0 and current_state[i - 1] == 0:
            current = current_state[:]
            current[i - 1] = current[i]
            current[i] = 0
            yield (i, i - 1), current


def heuristic(goal, board):
    output = 0
    for index in range(len(board)):

        if board[index] > 0:
            output += abs(goal.index(board[index]) - index)
    return output


############################################################
# Section 4: Dominoes Game
############################################################

def create_dominoes_game(rows, cols):
    return DominoesGame([[False for _ in range(cols)] for _ in range(rows)])


class DominoesGame(object):

    # Required
    def __init__(self, board):
        self.board = board
        self.rows = len(board)
        self.cols = len(board[0])

    def get_board(self):
        return self.board

    def reset(self):
        for i in range(self.rows):
            for j in range(self.cols):
                self.board[i][j] = False

    def is_legal_move(self, row, col, vertical):
        if vertical:
            if row >= 0 and col >= 0 and row < self.rows - 1 and col <= self.cols - 1:
                return not self.board[row + 1][col] and not self.board[row][col]
            else:
                return False
        else:
            if row >= 0 and col >= 0 and row <= self.rows - 1 and col < self.cols - 1:
                return not self.board[row][col] and not self.board[row][col + 1]
            else:
                return False

    def legal_moves(self, vertical):
        for i in range(self.rows):
            for j in range(self.cols):
                if self.is_legal_move(i, j, vertical):
                    yield (i, j)

    def perform_move(self, row, col, vertical):
        if vertical:
            self.board[row][col], self.board[row + 1][col] = True, True
        else:
            self.board[row][col], self.board[row][col + 1] = True, True

    def game_over(self, vertical):
        for i in range(self.rows):
            for j in range(self.cols):
                if self.is_legal_move(i, j, vertical):
                    return False
        return True

    def copy(self):
        return DominoesGame([[self.board[i][j] for j in range(self.cols)] for i in range(self.rows)])

    def successors(self, vertical):
        for i in range(self.rows):
            for j in range(self.cols):
                if self.is_legal_move(i, j, vertical):
                    copy = self.copy()
                    copy.perform_move(i, j, vertical)
                    yield ((i, j), copy)

    def get_random_move(self, vertical):
        return random.choice(list(self.legal_moves(vertical)))

    # Required
    def get_best_move(self, vertical, limit):
        return self.max_value(vertical, limit, -math.inf, math.inf)

    def max_value(self, vertical, limit, a, b):
        if self.game_over(vertical) or limit <= 0:
            return (tuple(), self.utility(vertical), 1)

        v = -math.inf
        n_leaf = 0
        best_move = tuple()
        for move, new_game in self.successors(vertical):
            t = new_game.min_value(not vertical, limit - 1, a, b)
            if v < t[1]:
                v = t[1]
                best_move = move

            n_leaf += t[2]
            if v >= b:
                return (tuple(), v, n_leaf)
            a = max(a, v)

        return (best_move, v, n_leaf)

    def min_value(self, vertical, limit, a, b):
        if self.game_over(vertical) or limit <= 0:
            return (tuple(), self.utility(not vertical), 1)

        v = math.inf
        n_leaf = 0

        best_move = tuple()
        for move, new_game in self.successors(vertical):
            t = new_game.max_value(not vertical, limit - 1, a, b)
            n_leaf += t[2]
            if v > t[1]:
                v = t[1]
                best_move = move

            if v <= a:
                return (tuple(), v, n_leaf)
            b = min(b, v)
        return (best_move, v, n_leaf)

    def utility(self, vertical):
        return sum(1 for _ in self.legal_moves(vertical)) - sum(1 for _ in self.legal_moves(not vertical))



############################################################
# Section 5: Feedback
############################################################

feedback_question_1 = """
25 hours
"""

feedback_question_2 = """
problem 1
idds function and a*
It takes me lots of time to optimize them in order to reduce the running time
"""

feedback_question_3 = """
problem 2 and GUI
I would like to remove the iddfs function. I hate that.
"""
