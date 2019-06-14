#!/bin/bash

serial_test()
{
	echo "-> Copy the first healthy binary"
	mkdir -p /tmp/test
	cp -v /bin/ls /tmp/test

	echo "-> Launch famine --------------------------------------"
	../famine

	echo "-> Copy the second healthy binary"
	mkdir -p /tmp/test2
	cp -v /bin/pwd /tmp/test2

	echo "-> Launch binary in test"
	/tmp/test/ls

	echo "-> Launch binary in test2"
	/tmp/test2/pwd

	sleep 1
}

# main
echo "===== WITH DEAMON ====="
make
rm -rf /tmp/test
rm -rf /tmp/test2

echo "-> Launch deamon"
./deamon

serial_test


echo "===== WITHOUT DEAMON ====="
rm -rf /tmp/test
rm -rf /tmp/test2

echo "-> Kill deamon"
killall deamon

serial_test

echo "===== WITH DEAMON ====="
./deamon

echo "-> Copy the second healthy binary"
rm -rf /tmp/test2
mkdir -p /tmp/test2
cp -v /bin/pwd /tmp/test2

echo "-> Launch binary in test"
/tmp/test/ls

echo "-> Launch binary in test2"
/tmp/test2/pwd
