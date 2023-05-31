echo "------------------------------------------------------------------ cache ------------------------------------------------------------------"
wrk -d 10 -t 1 -c 1 --latency -s ./cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 1 -c 10 --latency -s ./cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 1 -c 100 --latency -s ./cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 5 -c 10 --latency -s ./cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 5 -c 100 --latency -s ./cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 10 -c 10 --latency -s ./cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 10 -c 100 --latency -s ./cache.lua http://localhost:8082/ \ 

echo "----------------------------------------------------------------- no-cache -----------------------------------------------------------------"

wrk -d 10 -t 1 -c 1 --latency -s ./no_cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 1 -c 10 --latency -s ./no_cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 1 -c 100 --latency -s ./no_cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 5 -c 10 --latency -s ./no_cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 5 -c 100 --latency -s ./no_cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 10 -c 10 --latency -s ./no_cache.lua http://localhost:8082/ \ 
wrk -d 10 -t 10 -c 100 --latency -s ./no_cache.lua http://localhost:8082/ \ 