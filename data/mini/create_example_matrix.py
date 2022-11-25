import random

# Function to create random distance matrix
def create_distance_matrix(n):
    matrix = [[0 for i in range(n)] for j in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            matrix[i][j] = matrix[j][i] = random.randint(1, 100)
    return matrix


# Function to save matrix to file
def save_matrix(matrix, filename):
    with open(filename, 'w') as file:
        for i in range(len(matrix)):
            file.write('{')
            for j in range(len(matrix[i])):
                file.write(str(matrix[i][j]) + ',')
            file.write('},\n')

matrix = create_distance_matrix(20)
save_matrix(matrix, 'example_matrix.txt')