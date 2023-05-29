sudo rm -R serving_service
sudo rm -R user_service
sudo rm -R build
sudo rm -R order_service
docker rm -vf $(docker ps -aq)