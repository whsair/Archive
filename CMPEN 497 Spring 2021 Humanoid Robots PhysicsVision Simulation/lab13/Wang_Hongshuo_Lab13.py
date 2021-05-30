

def print_maze(maze):
    print("██████████")
    for row in maze:
        print("█", end='')
        for col in row:
            if (col == 0):
                print(' ', end='')
            elif (col == 1):
                print('█', end='')
            elif (col == 2):
                print('O', end='')
        print("█")
    print("██████████")


# Set up important variables
actions = {'U': (-1, 0), 'D': (1, 0), 'L': (0, -1), 'R': (0, 1)}
current_location = (0, 0)
end_location = (7, 7)
maze = [[2, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0, 1, 0],
        [0, 0, 0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 0, 1, 0, 0],
        [0, 0, 0, 0, 0, 0, 1, 1],
        [0, 0, 0, 0, 1, 0, 0, 0]]

# Construct allowed states
allowed_states = {}

for r in range(len(maze)):
    for c in range(len(maze[0])):
        value = []
        key = None
        if maze[r][c] != 1:
            key = (r, c)
            for sa, a in actions.items():
                s = list(key)
                s[0], s[1] = s[0] + a[0], s[1] + a[1]
                print(s)
                if 0 <= s[0] <= len(maze) - 1 and 0 <= s[1] <= len(maze[0]) - 1:
                    if maze[s[0]][s[1]] != 1:

                        value.append(sa)
            allowed_states[key] = value

# print(allowed_states)


# The main loop where you navigate to the end
score = 0
while (current_location != end_location):

    print_maze(maze)
    valid_states = allowed_states[current_location]
    print(f"Current valid actions: {allowed_states[current_location]}")
    print(f"Type q to quit loop")
    i = input("Please type actions you would like to perform ('U' = upper, 'D' = down, 'L' = left, and 'R' = Right)\n")
    i = str(i).upper()
    if i == "q":
        break
    if i in actions.keys():

        if i in valid_states:
            a = actions[i]
            r, c = current_location[0] + a[0], current_location[1] + a[1]
            maze[current_location[0]][current_location[1]] = 0
            maze[r][c] = 2
            current_location = (r, c)
            score += 1
            print("valid move")
            print(f'current_location: {current_location}\n')
        else:
            print("invalid input\n")
    else:
        print("error input\n")



print_maze(maze)
print(f"Final score: {score}")
print("Congratulations! You made it to the end of the maze!")
