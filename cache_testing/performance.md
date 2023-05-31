------------------------------------------------------------------ cache ------------------------------------------------------------------
Running 10s test @ http://localhost:8082/
  1 threads and 1 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    86.50ms   62.99ms 402.35ms   76.09%
    Req/Sec    13.84      9.84    49.00     84.00%
  Latency Distribution
     50%   76.74ms
     75%  111.14ms
     90%  160.27ms
     99%  321.65ms
  125 requests in 10.09s, 34.30KB read
Requests/sec:     12.39
Transfer/sec:      3.40KB
Running 10s test @ http://localhost:8082/
  1 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   283.11ms  187.38ms   1.10s    70.03%
    Req/Sec    39.39     28.30   121.00     61.33%
  Latency Distribution
     50%  255.47ms
     75%  386.94ms
     90%  521.96ms
     99%  928.85ms
  366 requests in 10.12s, 101.29KB read
Requests/sec:     36.16
Transfer/sec:     10.01KB
Running 10s test @ http://localhost:8082/
  1 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   447.71ms  263.45ms   1.76s    75.16%
    Req/Sec    35.97     27.92   117.00     60.29%
  Latency Distribution
     50%  390.45ms
     75%  570.09ms
     90%  780.28ms
     99%    1.38s 
  319 requests in 10.13s, 88.78KB read
  Socket errors: connect 0, read 1899, write 0, timeout 1
Requests/sec:     31.50
Transfer/sec:      8.77KB
Running 10s test @ http://localhost:8082/
  5 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   306.43ms  257.08ms   1.57s    89.42%
    Req/Sec     9.36      5.34    28.00     56.30%
  Latency Distribution
     50%  229.72ms
     75%  345.48ms
     90%  550.50ms
     99%    1.30s 
  373 requests in 10.13s, 103.53KB read
Requests/sec:     36.81
Transfer/sec:     10.22KB
Running 10s test @ http://localhost:8082/
  5 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   289.24ms  140.10ms 747.80ms   68.51%
    Req/Sec    52.62     36.09   205.00     72.84%
  Latency Distribution
     50%  273.66ms
     75%  370.66ms
     90%  498.65ms
     99%  616.77ms
  505 requests in 10.13s, 138.72KB read
  Socket errors: connect 0, read 2148, write 0, timeout 0
Requests/sec:     49.87
Transfer/sec:     13.70KB
Running 10s test @ http://localhost:8082/
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   238.77ms  147.39ms   1.01s    84.43%
    Req/Sec     5.23      2.90    19.00     66.82%
  Latency Distribution
     50%  206.85ms
     75%  285.10ms
     90%  383.16ms
     99%  903.30ms
  440 requests in 10.10s, 121.02KB read
Requests/sec:     43.58
Transfer/sec:     11.99KB
Running 10s test @ http://localhost:8082/
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   299.19ms  154.13ms   1.00s    70.97%
    Req/Sec    17.69     17.26    83.00     82.79%
  Latency Distribution
     50%  269.81ms
     75%  379.35ms
     90%  516.06ms
     99%  756.90ms
  496 requests in 10.09s, 139.70KB read
  Socket errors: connect 0, read 1736, write 0, timeout 0
Requests/sec:     49.18
Transfer/sec:     13.85KB
----------------------------------------------------------------- no-cache -----------------------------------------------------------------
Running 10s test @ http://localhost:8082/
  1 threads and 1 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    73.62ms  112.63ms 770.53ms   94.07%
    Req/Sec    22.56     15.91    77.00     82.93%
  Latency Distribution
     50%   48.12ms
     75%   83.49ms
     90%  137.14ms
     99%  673.08ms
  206 requests in 10.04s, 57.94KB read
Requests/sec:     20.52
Transfer/sec:      5.77KB
Running 10s test @ http://localhost:8082/
  1 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   182.40ms   86.77ms 544.38ms   70.61%
    Req/Sec    54.11     31.52   161.00     68.97%
  Latency Distribution
     50%  171.92ms
     75%  236.48ms
     90%  292.50ms
     99%  438.92ms
  529 requests in 10.06s, 148.92KB read
Requests/sec:     52.56
Transfer/sec:     14.80KB
Running 10s test @ http://localhost:8082/
  1 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   309.06ms  164.81ms   1.17s    72.22%
    Req/Sec    48.84     29.11   125.00     61.73%
  Latency Distribution
     50%  277.29ms
     75%  398.06ms
     90%  519.41ms
     99%  866.50ms
  468 requests in 10.02s, 128.43KB read
  Socket errors: connect 0, read 1793, write 0, timeout 0
Requests/sec:     46.70
Transfer/sec:     12.82KB
Running 10s test @ http://localhost:8082/
  5 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   287.43ms  203.39ms   1.34s    83.87%
    Req/Sec     8.86      4.98    29.00     76.81%
  Latency Distribution
     50%  240.49ms
     75%  338.10ms
     90%  499.26ms
     99%    1.08s 
  375 requests in 10.12s, 103.17KB read
  Socket errors: connect 0, read 3, write 0, timeout 0
Requests/sec:     37.07
Transfer/sec:     10.20KB
Running 10s test @ http://localhost:8082/
  5 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   263.04ms  135.07ms 963.74ms   69.73%
    Req/Sec    40.39     36.51   190.00     67.23%
  Latency Distribution
     50%  241.87ms
     75%  346.95ms
     90%  456.60ms
     99%  619.88ms
  565 requests in 10.08s, 155.43KB read
  Socket errors: connect 0, read 2383, write 0, timeout 0
Requests/sec:     56.03
Transfer/sec:     15.41KB
Running 10s test @ http://localhost:8082/
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   258.22ms  148.85ms 940.09ms   76.47%
    Req/Sec     4.88      3.24    35.00     70.73%
  Latency Distribution
     50%  230.22ms
     75%  322.45ms
     90%  456.53ms
     99%  770.05ms
  399 requests in 10.12s, 110.12KB read
Requests/sec:     39.43
Transfer/sec:     10.88KB
Running 10s test @ http://localhost:8082/
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   258.73ms  150.05ms 839.83ms   70.05%
    Req/Sec    28.88     18.32    99.00     64.60%
  Latency Distribution
     50%  234.12ms
     75%  341.67ms
     90%  456.27ms
     99%  740.01ms
  571 requests in 10.13s, 157.46KB read
  Socket errors: connect 0, read 2078, write 0, timeout 0
Requests/sec:     56.36
Transfer/sec:     15.54KB
