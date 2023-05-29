@startuml
!include https://raw.githubusercontent.com/plantuml-stdlib/C4-PlantUML/master/C4_Container.puml


Person(admin, "Администратор")
Person(user, "Пользователь")

System(conference_site, "Веб приложения для заказа услуг", "Сайт для предоставления услуг пользователям")


Rel(admin, conference_site, "Просмотр, добавление и редактирование информации о пользователях, услугах и заказах")
Rel(user, conference_site, "Регистрация, Выбор услуг, создание/изменение заказа на оказание услуг")



@enduml
