export DB_HOST=localhost\ 
export DB_PORT=3360\ 
export DB_LOGIN=myyuser\ 
export DB_PASSWORD=123\ 
export DB_DATABASE=serv_store\ 

docker rm -vf $(docker ps -aq)\ 
docker rmi `docker images --filter 'dangling=true' -q --no-trunc`\ 
docker-compose build | docker-compose up