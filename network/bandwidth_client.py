import socket
import time
import statistics  

HOST = "192.168.1.137"
FILE_SIZE = 1024  
PORT = 8090

bandwidth_results = []  


skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
skt.connect((HOST, PORT))


fileData = '1' * int(FILE_SIZE) * 1024 * 1024  
fileData = fileData.encode()

for i in range(11):  
    print(f"Start sending {len(fileData)} bytes data (Iteration {i + 1})")
    start = time.time()  
    skt.sendall(fileData)  
    end = time.time()  

    elapsed_time = end - start  
    bandwidth = (len(fileData) * 8) / (elapsed_time * 1e6)  

    if i > 0:  # Skip the first measurement
        bandwidth_results.append(bandwidth)


skt.close()


mean_bandwidth = statistics.mean(bandwidth_results)
std_dev_bandwidth = statistics.stdev(bandwidth_results)

print(f"Mean Bandwidth: {mean_bandwidth:.2f} Mbps")
print(f"Standard Deviation: {std_dev_bandwidth:.2f} Mbps")

