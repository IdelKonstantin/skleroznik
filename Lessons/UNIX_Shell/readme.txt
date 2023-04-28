Команды UNIX: 

0) Открыть терминал по дефолтному пути: 

	Ctrl + Alt + T (Alt + T в некоторых дистрибутивах)

   Закрыть: 
	$exit

   Справка:
	$man <имя команды или системного вызова>
	$apropos <имя команды или действия>

1) Навигация:

	$pwd
	$cd (пути /, ../, ./, -, ~)

2) Информация о файлах и их содержимом:

	$file
	$ls
	$cat
	$head
	$tail (-f)
	$grep
	$locate
	$find
	$stat

3) Манипуляции с файлами и каталогами:

	$cp
	$mv
	$rm (-r)
	$mkdir
	$touch <имя файла>

4) Перенаправление в файл:

	> со стиранием содержимого
	>> с добавлением к содержимому
	> 2>&1
	echo

5) Конвейер:

	команда 1 | команда 2

6) Системное/вспомогательное:

	$clear
	$history (!<номер команды в истории>)
	$df -h
	$free
	$su/sudo
	$chmod
	$ps/top
	$kill/killall
	$reboot/shutdown

	$printenv/set/export

	$apt install <имя пакета>

7) Сеть: 

	$ping 
	$traceroute
	$hostname
	$ip a	
	$netstat
	$ftp
	$wget
	$scp/ssh

8) Редакторы:
	mc
	nano
	vim
