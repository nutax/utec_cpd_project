import matplotlib.pyplot as plt

threads = [1, 2, 4, 8]
sizes = [131, 395, 662, 984, 1376]
tries = [1, 2, 4, 8, 16, 32, 64]

times = []

def get_real(size):
    if size == 131:
        return 564
    if size == 395:
        return 1281
    if size == 662:
        return 2513
    if size == 984:
        return 2797
    if size == 1376:
        return 4666
    raise Exception("Invalid size")

for trie in tries:
    for size in sizes:
        file = open(f'tries_vs_precision_size/{trie}_{size}.out')
        data = file.read()
        data_splitted = data.split(' ')
        data_splitted[0] = data_splitted[0].strip()
        data_splitted[1] = data_splitted[1].strip()

        precision = float(data_splitted[1])

        exact_precision = get_real(size)
        precision = (precision - exact_precision) / exact_precision

        # time is index 0. It is in ms
        times.append([trie, size, round(precision, 2)])


for size in sizes:
    x = []
    y = []
    for time in times:
        if time[1] == size:
            x.append(time[0])
            y.append(time[2])
    plt.plot(x, y, label=f'size {size}')

plt.xlabel('Number of tries')
plt.ylabel('Percentage error')
plt.title('Percentage error vs number of tries for different sizes')
plt.legend()
plt.tight_layout()
plt.savefig('tries_vs_precision.png')