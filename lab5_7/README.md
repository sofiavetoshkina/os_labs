## Лабораторная работа №5-7

### Вариант 35

4/1/2

Топология 4

Все вычислительные узлы хранятся в идеально сбалансированном бинарном дереве поиска. [parent] — является необязательным параметром. Каждый следующий узел должен добавляться в самое наименьшее поддерево.

Набор команд 1 (подсчет суммы n чисел)

Формат команды: exec id n k1 … kn
id – целочисленный идентификатор вычислительного узла, на который отправляется команда
n – количество складываемых чисел (от 1 до 108)
k1 … kn – складываемые числа
Пример:
> exec 10 3 1 2 3
Ok:10: 6

Команда проверки 2

Формат команды: ping id
Команда проверяет доступность конкретного узла. Если узла нет, то необходимо выводить ошибку: «Error: Not found»
Пример:
> ping 10
Ok: 1 // узел 10 доступен
> ping 17
Ok: 0 // узел 17 недоступен

Технология очередей ZeroMQ


