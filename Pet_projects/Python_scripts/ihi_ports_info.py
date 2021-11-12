#!/usr/bin/env python3
#encoding: utf-8

###############################################################################
#
#    Этот скрипт считывает открытые порты для компонент ядра ИХИ
#
#    sudo ./ihi_ports_info.py
#
###############################################################################

import subprocess
import os
from collections import defaultdict

PORTS_INFO_FILE = "ports.txt"
CONNECTION_TYPE = "tcp "
IHI_DAEMONS = ["proxyHttp", "taskLaunch", "cronDaemon", "siuDaemon", "postgres", 
"ioNotify", "replication",  "zabbix_agentd", "zabbix_infr"]

#Запрос какие порты открыты
psqlCommand = "netstat -tulpn > " + PORTS_INFO_FILE

p = subprocess.Popen(psqlCommand, shell = True)
p.wait()

lines = []
with open(PORTS_INFO_FILE) as f:
    lines = f.readlines()

lines = [x.strip() for x in lines]

lines.pop(0)
lines.pop(0)

content = defaultdict(list)
proc = []

for line in lines:
    if(line.find(CONNECTION_TYPE) != -1):

        port = line.split(":")[1]
        proc = line.split(":")[2]
        port = port.split()[0][0:]
        proc = proc.split('/')[-1]
        content[proc].append(int(port))

print("====================================================")

content_ = defaultdict(list)

for daemon in IHI_DAEMONS:
    for k, v in content.items():
        if(k.find(daemon) != -1):

            content_[daemon] += v

for k, v in content_.items():
    print("\n" + k + " ports:")
    v.sort()
    print(v)

os.remove(PORTS_INFO_FILE)