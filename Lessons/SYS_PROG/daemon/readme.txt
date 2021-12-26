1) g++ daemon -o my_daemon
2) sudo su 
	cp my_daemon /usr/bin/
	cp custom_daemon.service /etc/systemd/system/
3) Запуск:
	systemctl enable custom_daemon.service
	systemctl start custom_daemon.service
	systemctl status custom_daemon.service
4) Останов:
	systemctl stop custom_daemon.service
5) Посмотреть в лог:
	tail -f /var/log/syslog


