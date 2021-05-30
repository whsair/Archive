import time
import numpy as np
import matplotlib.pyplot as plt


# Function for printing the maze if you need it
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


# Return a clean copy of the maze
def get_new_maze():
    maze = [[2, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0],
            [0, 1, 0, 0, 0, 0, 1, 0],
            [0, 0, 0, 0, 1, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0],
            [0, 0, 1, 0, 0, 1, 0, 0],
            [0, 0, 0, 0, 0, 0, 1, 1],
            [0, 0, 0, 0, 1, 0, 0, 0]]
    return maze


def update_state(maze, current_location, direction):
    new_maze = maze
    y, x = current_location
    new_x = x + direction[1]
    new_y = y + direction[0]
    new_maze[y][x] = 0
    new_maze[new_y][new_x] = 2
    new_location = (new_y, new_x)
    return (new_maze, new_location)


# Set up important variables
actions = {'U': (-1, 0), 'D': (1, 0), 'L': (0, -1), 'R': (0, 1)}
start_location = (0, 0)
end_location = (7, 7)
alpha = 0.1
random_factor = 0.25
maze = get_new_maze()
allowed_states = {}
current_expectations = {}

# Construct allowed states
for y in range(len(maze)):
    for x in range(len(maze[y])):
        if (maze[y][x] == 1):
            continue
        allowed_states[(y, x)] = []
        for action in actions:
            new_x = x + actions[action][1]
            new_y = y + actions[action][0]
            if (new_x < 0 or new_x > 7 or new_y < 0 or new_y > 7):
                continue
            if maze[new_y][new_x] == 0 or maze[new_y][new_x] == 2:
                allowed_states[(y, x)].append(action)

# Initialize rewards
########################################

for key in allowed_states:
    current_expectations[key] = np.random.uniform(-1.0, -0.1)
# print(current_expectations)
# quit()

########################################

# Set up everything for rendering
fig = plt.figure(1)
# Uncomment next line for maximized window
# plt.get_current_fig_manager().window.state('zoomed') 
fig.show()
fig.canvas.draw()
x = []
y = []

# The training happens here
move_counts = []
for i in range(5000):
    if i % 1000 == 0:
        print(i)
    # Reset important variables
    moves = 0
    current_location = start_location
    state_history = [((0, 0), 0)]
    maze = get_new_maze()
    # Do a run through the maze
    while (current_location != end_location):
        # Choose next action
        ########################################
        next_move = None
        if np.random.random() < random_factor:
            explored_move = np.random.choice(allowed_states[current_location])
            next_move = explored_move

        else:
            max_E_R = -1e12
            for m in allowed_states[current_location]:

                next_state = tuple([sum(val) for val in zip(actions[m], current_location)])

                if next_state == end_location:
                    next_move = m
                    break
                # print(current_expectations[next_state])
                if current_expectations[next_state] > max_E_R:
                    max_E_R = current_expectations[next_state]
                    exploit_move = m
                    next_move = exploit_move
        # print(next_move)

        ########################################

        # Update the state (uncomment line after you figure out what the next move should be)
        # print(actions[next_move])

        maze, current_location = update_state(maze, current_location, actions[next_move])

        # print(moves)
        moves += 1

        # Get reward
        ########################################
        r = 0
        if current_location == end_location:
            r = 0
        else:
            r = -1

        ########################################

        # Update state history
        ########################################
        state_history.append((current_location, r))
        # print(state_history)
        ########################################

        # If robot takes too long, just end the run
        if (moves > 1000):
            current_location = end_location

    # Do the learning
    ########################################
    # current_E = 0.0
    # expected_history = []
    # current_E += max_E_R
    target_r = 0
    for i in range(len(state_history) - 1, -1, -1):
        state = state_history[i][0]
        reward = state_history[i][1]

        current_expectations[state] = current_expectations[state] + alpha * (target_r - current_expectations[state])
        target_r += reward

    random_factor = random_factor - 1e-4

    ########################################

    # Store number of moves for plotting
    move_counts.append(moves)

    # Every 250 iterations, visualize the paths of the agent
    for val in state_history:
        x.append(val[0][1])
        y.append(val[0][0])
    if (i % 250 == 0):
        heatmap, _, _ = np.histogram2d(x, y, bins=8)
        extent = [0, 8, 0, 8]
        plt.clf()
        plt.imshow(heatmap.T, extent=extent, origin='lower')
        plt.xlabel("Agent x-coordinate", fontsize=14)
        plt.ylabel("Agent y-coordinate", fontsize=14)
        plt.gca().invert_yaxis()
        plt.gca().xaxis.tick_top()
        plt.gca().xaxis.set_label_position('top')
        cb = plt.colorbar(ticks=[0, heatmap.max()])
        cb.ax.set_yticklabels(['Never', 'Frequent'])
        cb.ax.tick_params(labelsize=12)
        fig.canvas.draw()
        time.sleep(0.5)

        x = []
        y = []
    # print("end\n\n\n\n")
fig2 = plt.figure(2)
plt.semilogy(move_counts, 'b')
plt.xlabel("Iteration Number", fontsize=14)
plt.ylabel("Number of moves taken", fontsize=14)
plt.show()
