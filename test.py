import matplotlib.pyplot as plt

# Read numbers from file
with open("out.txt", "r") as f:
    data = list(map(float, f.read().split()))

# Plot
plt.figure()
plt.plot(data)
plt.xlabel("Index")
plt.ylabel("Value")
plt.title("Plot of out.txt data")
plt.show()
