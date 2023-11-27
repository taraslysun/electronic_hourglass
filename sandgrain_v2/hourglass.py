class Hourglass:
    def __init__(self, size):
        self.size = size
        self.grid = []
        self.grains = []
        self.create_grid()
        self.fill_grid()

    def create_grid(self):
        for i in range(self.size):
            self.grid.append([])
            for j in range(self.size):
                self.grid[i].append(' ')

    def fill_grid(self):
        offset = 0
        for i in range(offset, self.size-offset):
            for j in range(offset, self.size-offset):
                self.grid[i][j] = '_'
            offset += 1
        offset = 0
        for i in range(offset, self.size-offset):
            for j in range(offset, self.size-offset):
                self.grid[-i-1][-j-1] = '_'
            offset += 1

    def print_grid(self):
        for i in range(self.size):
            for j in range(self.size):
                print(self.grid[i][j], end=" ")
            print()

    def one_epoch(self):
        for i in range(self.size):
            for j in range(self.size):
                if isinstance(self.grid[-i][j], Grain) and self.grid[-i][j]:
                    if self.grid[-i][j].fallen_on_this_epoch:
                        continue
                    self.grid[-i][j].fall()

        for grain in self.grains:
            grain.fallen_on_this_epoch = False

    def add_grain(self, grain, row, col):
        self.grid[row][col] = grain
        self.grains.append(grain)


class Grain:
    def __init__(self, row, col, grid: Hourglass):
        self.row = row
        self.col = col
        self.grid = grid
        self.fallen_on_this_epoch = False

    def find_position_to_fall(self):
        if self.row == self.grid.size-1:
            return (self.row, self.col)
        
        elif self.grid.grid[self.row+1][self.col] == '_':
            return (self.row+1, self.col)
        elif self.grid.grid[self.row+1][self.col-1] == '_':
            return (self.row+1, self.col-1)
        elif self.grid.grid[self.row+1][self.col+1] == '_':
            return (self.row+1, self.col+1)
        else:
            return (self.row, self.col)

    def fall(self):
        row, col = self.find_position_to_fall()
        self.grid.grid[self.row][self.col] = '_'
        self.grid.grid[row][col] = self
        self.row = row
        self.col = col
        self.fallen_on_this_epoch = True


    def __str__(self):
        return 'G'


import pygame
import sys
import time

# Initialize Pygame
pygame.init()

# Constants
GRID_SIZE = 71
CELL_SIZE = 10
WINDOW_SIZE = (GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE)
WHITE = (255, 255, 255)
YELLOW = (255, 255, 0)
GRAY = (128, 128, 128)

# Create an Hourglass object
hourglass = Hourglass(GRID_SIZE)

# Add grains to the Hourglass
for i in range(0, 25):
    for j in range(30,35):
        hourglass.add_grain(Grain(i, j, hourglass), i, j)

# Pygame setup
screen = pygame.display.set_mode(WINDOW_SIZE)
pygame.display.set_caption("Hourglass Simulation")

def draw_grid():
    for i in range(GRID_SIZE):
        for j in range(GRID_SIZE):
            cell_color = WHITE if hourglass.grid[i][j] == ' ' else GRAY
            if isinstance(hourglass.grid[i][j], Grain):
                cell_color = YELLOW
            pygame.draw.rect(screen, cell_color, (j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE))

def main():
    running = True
    epoch = 0
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Clear the screen
        screen.fill((0, 0, 0))

        # Update the Hourglass for one epoch
        hourglass.one_epoch()

        # Draw the grid
        draw_grid()

        # Update the display
        pygame.display.flip()

        # Delay to control the speed of the simulation
        time.sleep(0.1)

        epoch += 1

        # Exit the simulation after a specified number of epochs
        if epoch >= 275:
            running = False

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
