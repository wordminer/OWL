import pygame
import numpy as np
import math
import sys

# Khởi tạo pygame
pygame.init()

# Kích thước cửa sổ
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("3D Rubik's Cube")

# Màu sắc
colors = [
    (255, 0, 0),    # Đỏ
    (0, 255, 0),    # Xanh lá
    (0, 0, 255),    # Xanh dương
    (255, 255, 0),  # Vàng
    (255, 165, 0),  # Cam
    (255, 255, 255) # Trắng
]

# Góc xoay ban đầu
angle_x, angle_y = 0, 0

# Các hình vuông trên mỗi mặt của Rubik
def create_cube():
    cube = []
    size = 50  # Kích thước mỗi ô vuông
    for x in range(3):
        for y in range(3):
            for z in range(3):
                # Mỗi ô có 6 mặt với màu khác nhau
                if x == 0:
                    cube.append([size * x - 100, size * y - 50, size * z - 50, colors[0]])
                elif x == 2:
                    cube.append([size * x - 100, size * y - 50, size * z - 50, colors[1]])
                elif y == 0:
                    cube.append([size * x - 100, size * y - 50, size * z - 50, colors[2]])
                elif y == 2:
                    cube.append([size * x - 100, size * y - 50, size * z - 50, colors[3]])
                elif z == 0:
                    cube.append([size * x - 100, size * y - 50, size * z - 50, colors[4]])
                elif z == 2:
                    cube.append([size * x - 100, size * y - 50, size * z - 50, colors[5]])
    return cube

# Hàm xoay trong không gian 3D
def rotate(point, angle_x, angle_y):
    x, y, z = point
    # Xoay theo trục y
    x, z = x * math.cos(angle_y) - z * math.sin(angle_y), x * math.sin(angle_y) + z * math.cos(angle_y)
    # Xoay theo trục x
    y, z = y * math.cos(angle_x) - z * math.sin(angle_x), y * math.sin(angle_x) + z * math.cos(angle_x)
    return x, y, z

# Vẽ hình vuông
def draw_square(square):
    points = []
    for x, y, z in [(0, 0, 0), (1, 0, 0), (1, 1, 0), (0, 1, 0)]:
        nx, ny, nz = square[0] + x * 50, square[1] + y * 50, square[2]
        nx, ny, nz = rotate((nx, ny, nz), angle_x, angle_y)
        # Điều chỉnh tọa độ lên màn hình
        scale = 400 / (400 + nz)
        nx, ny = nx * scale + width // 2, ny * scale + height // 2
        points.append((nx, ny))
    pygame.draw.polygon(screen, square[3], points)

# Chạy chương trình chính
cube = create_cube()
clock = pygame.time.Clock()

while True:
    # Xử lý sự kiện
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                angle_y -= 0.1
            elif event.key == pygame.K_RIGHT:
                angle_y += 0.1
            elif event.key == pygame.K_UP:
                angle_x -= 0.1
            elif event.key == pygame.K_DOWN:
                angle_x += 0.1

    # Vẽ khối Rubik
    screen.fill((0, 0, 0))
    for square in cube:
        draw_square(square)
    pygame.display.flip()
    clock.tick(30)
