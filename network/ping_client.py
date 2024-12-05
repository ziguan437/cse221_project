import socket
import time
import sys
import statistics  

HOST = "192.168.1.137"  
PORT = 8090  
skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
skt.connect((HOST, PORT))

data = "1" * 64
data = data.encode()

rtt_results = []  

for k in range(0, 10):
    total = 0

    for i in range(0, 100):
        start = time.time()
        skt.send(data)
        data = skt.recv(64)
        end = time.time()
        total += (end - start)

    result = total * 1000 / 100  
    rtt_results.append(result)  
    print("The round trip time is {} ms".format(result))

skt.close()

mean_rtt = statistics.mean(rtt_results)
stddev_rtt = statistics.stdev(rtt_results)

print("\nRTT Statistics:")
print(f"Mean RTT: {mean_rtt:.4f} ms")
print(f"Standard Deviation of RTT: {stddev_rtt:.4f} ms")
