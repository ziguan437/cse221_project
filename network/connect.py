import socket
import time
import statistics

HOST = "192.168.1.137"  
PORT = 8090         

setup_times = []
teardown_times = []

for i in range(100):
    skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    start = time.time()
    skt.connect((HOST, PORT))
    end = time.time()
    setup_duration = end - start
    if setup_duration > 0:  
        setup_times.append(setup_duration)
    

    start = time.time()
    skt.close()
    end = time.time()
    teardown_duration = end - start
    if teardown_duration > 0:  
        teardown_times.append(teardown_duration)

print(setup_times)
setup_avg = statistics.mean(setup_times) if setup_times else 0
teardown_avg = statistics.mean(teardown_times) if teardown_times else 0
setup_stddev = statistics.stdev(setup_times) if len(setup_times) > 1 else 0
teardown_stddev = statistics.stdev(teardown_times) if len(teardown_times) > 1 else 0

print("Setup: Avg = {:.6f} ms, StdDev = {:.6f} ms".format(setup_avg * 1000, setup_stddev * 1000))
print("Teardown: Avg = {:.6f} ms, StdDev = {:.6f} ms".format(teardown_avg * 1000, teardown_stddev * 1000))




