import matplotlib.pyplot as plt

# thread_vs_time_size
threads = [1, 2, 4, 8]
sizes = [131, 395, 662, 984, 1376]
tries = [8, 16, 32, 64]

times = []

for thread in threads:
    for size in sizes:
        time_number_all = 0
        for i in range(1,4):
            file = open(f'thread_vs_time_size/{i}/{thread}_{size}.out')
            data = file.read()
            data_splitted = data.split(' ')
            data_splitted[0] = data_splitted[0].strip()
            data_splitted[1] = data_splitted[1].strip()

            time_number = float(data_splitted[0])
            time_number_all = time_number_all + time_number

        time_number_all = time_number_all / 3
        # time is index 0. It is in ms
        times.append([thread, size, round(time_number_all, 2)])

# times list contains in index 0 the number of threads, in index 1 the size of the problem and in index 2 the time in ms
# the objective is to plot the number of threads vs the time for each size of the problem. Each size would be a different line
for size in sizes:
    x = []
    y = []
    for time in times:
        if time[1] == size:
            x.append(time[0])
            y.append(time[2])
    plt.plot(x, y, label=f'size {size}')

plt.xlabel('Number of threads')
plt.ylabel('Time (ms)')
plt.title('Time vs number of threads for different sizes')
plt.legend()
plt.tight_layout()
plt.savefig('thread_vs_time.png')