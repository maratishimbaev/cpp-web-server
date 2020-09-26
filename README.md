# C++ Web Server
Multithreaded HTTP web server written in C++ based on thread pool architecture

## Installation
```
git clone https://github.com/maratishimbaev/cpp-web-server.git
```

## Usage
Build:
```
cmake --build ./cmake-build --target cpp-web-server -- -j 6
```

Run server:
```
cd ./cmake-build/cpp-web-server -p 8000
```

Run functional tests:
```
python2 httptest.py
```

## Configuration
httpd.conf
```conf
thread_limit 100 ; maximum thred count
document_root /usr/www/html ; server files directory
```

## Load test
Tested with Apache Bench compared with Nginx

This server (with 100 threads):
```
This is ApacheBench, Version 2.3 <$Revision: 1879490 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        cpp-web-server
Server Hostname:        localhost
Server Port:            8000

Document Path:          /httptest/wikipedia_russia.html
Document Length:        954825 bytes

Concurrency Level:      100
Time taken for tests:   7.954 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      9549730000 bytes
HTML transferred:       9548250000 bytes
Requests per second:    1257.22 [#/sec] (mean)
Time per request:       79.541 [ms] (mean)
Time per request:       0.795 [ms] (mean, across all concurrent requests)
Transfer rate:          1172471.59 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   13 110.9      1    1030
Processing:     1   66 183.4     63    6730
Waiting:        0   14 181.1      3    6643
Total:          1   79 260.0     65    7758

Percentage of the requests served within a certain time (ms)
  50%     65
  66%     74
  75%     76
  80%     78
  90%     80
  95%     84
  98%     90
  99%   1080
 100%   7758 (longest request)
```

Nginx:
```
This is ApacheBench, Version 2.3 <$Revision: 1879490 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        nginx/1.19.2
Server Hostname:        localhost
Server Port:            8080

Document Path:          /httptest/wikipedia_russia.html
Document Length:        954824 bytes

Concurrency Level:      100
Time taken for tests:   5.442 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      9550620000 bytes
HTML transferred:       9548240000 bytes
Requests per second:    1837.63 [#/sec] (mean)
Time per request:       54.418 [ms] (mean)
Time per request:       0.544 [ms] (mean, across all concurrent requests)
Transfer rate:          1713919.38 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.5      0       7
Processing:     5   54  16.5     51     162
Waiting:        0   34  17.6     34     127
Total:          6   54  16.6     52     163

Percentage of the requests served within a certain time (ms)
  50%     52
  66%     57
  75%     61
  80%     64
  90%     73
  95%     82
  98%     96
  99%    108
 100%    163 (longest request)
```