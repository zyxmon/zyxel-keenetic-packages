# Планировщик cron. #

В состав пакета busybox, устанавливаемого вместе с системой установки opkg, входит планировщик задач cron. Для того, чтобы cron запускался автоматически следует переименовать файл K02cron в папке /media/DISK\_A1/system/etc/init.d в файл S02cron. Для создания заданий отредактируйте файл /media/DISK\_A1/system/etc/crontabs/root. Информацию о формате этого файла легко найти в сети. Например, если Вы хотите, чтобы планировщик заданий выключал в wi-fi в 23:45 и включал wi-fi в 6:45 этот файл должен быть таким
```
SHELL=/bin/sh
MOUNT="/media/DISK_A1/system"
PATH=$MOUNT/bin:$MOUNT/sbin:$MOUNT/usr/bin:$MOUNT/usr/sbin:/sbin:/usr/sbin:/bin:/usr/bin
MAILTO=""
HOME=/
# ---------- ---------- Default is Empty ---------- ---------- #
45 23 * * * /bin/cat /dev/null > /var/tmp/radio_off; /usr/sbin/iwpriv ra0 set RadioOn=0
45 6 * * * /usr/sbin/iwpriv ra0 set RadioOn=1; /bin/rm -f /var/tmp/radio_off
```