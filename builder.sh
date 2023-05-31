export DB_HOST=localhost
export DB_PORT=3360
export DB_LOGIN=myyuser
export DB_PASSWORD=123
export DB_DATABASE=serv_store


mkdir -p build && cd build && cmake .. && make && cd ..
cp -r build serving_service
cp -r build user_service
cp -r build order_service