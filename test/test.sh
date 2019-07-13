
#!/bin/bash

DARKRED="\033[2;31m"
DARKGREEN="\033[2;32m"
GREEN="\033[32m"
RED="\033[31m"
YELLOW="\033[33m"
CYAN="\033[36m"
DARKGREY="\033[90m"
NONE="\033[0m"

function clean_dirs()
{
	rm -rf /tmp/test/*
	rm -rf /tmp/test2/*
}

function compile()
{
	echo -e $DARKGREY"\nCompile and create directories"$NONE
	clean_dirs

	mkdir -p /tmp/test /tmp/test2
	make
	make -C ..
}

function check_signature()
{
	strings $1 | grep __UNICORNS_OF_THE_APOCALYPSE__ 1>/dev/null
}

function test_infect_1_without_deamon()
{
	clean_dirs

	echo -e $YELLOW"\n\t\t-- infect one without deamon --\n"$NONE
	echo -e $DARKGREY"    [deamon]: "$DARKRED"stopped"$NONE
	killall deamon 2>/dev/null
	echo -e $DARKGREY"    copy sane binary"$NONE
	cp /bin/ls /tmp/test
	echo -e $DARKGREY"    run Pestilence"$NONE
	../Pestilence
	echo -e $DARKGREY"    run infected binary"$NONE
	/tmp/test/ls
	sleep 1

	if check_signature "/tmp/test/ls"; then
		echo -e $CYAN"    infect one without deamon: "$GREEN"[ OK ]"$NONE
	else
		echo -e $CYAN"    infect one without deamon: "$RED"[ KO ]"$NONE
	fi
}

function test_infect_1_with_deamon()
{
	clean_dirs

	echo -e $YELLOW"\n\t\t-- infect one with deamon --\n"$NONE
	echo -e $DARKGREY"    [deamon]: "$DARKGREEN"running"$NONE
	./deamon 1>/dev/null
	echo -e $DARKGREY"    copy sane binary"$NONE
	cp /bin/ls /tmp/test
	echo -e $DARKGREY"    run Pestilence"$NONE
	../Pestilence
	echo -e $DARKGREY"    run sane binary"$NONE
	/tmp/test/ls
	sleep 1

	if check_signature "/tmp/test/ls"; then
		echo -e $CYAN"    infect one with deamon: "$RED"[ KO ]"$NONE
	else
		echo -e $CYAN"    infect one with deamon: "$GREEN"[ OK ]"$NONE
	fi
}

function test_spread_2_without_deamon()
{
	clean_dirs

	echo -e $YELLOW"\n\t\t-- spread two without deamon --\n"$NONE
	echo -e $DARKGREY"    [deamon]: "$DARKRED"stopped"$NONE
	killall deamon 2>/dev/null
	echo -e $DARKGREY"    copy first sane binary"$NONE
	cp /bin/ls /tmp/test
	echo -e $DARKGREY"    run Pestilence"$NONE
	../Pestilence
	echo -e $DARKGREY"    copy second sane binary"$NONE
	cp /bin/pwd /tmp/test2
	echo -e $DARKGREY"    run first infected binary"$NONE
	/tmp/test/ls
	sleep 1
	echo -e $DARKGREY"    run second infected binary"$NONE
	/tmp/test2/pwd
	sleep 1

	if check_signature "/tmp/test/ls" && check_signature "/tmp/test2/pwd"; then
		echo -e $CYAN"    spread two without deamon: "$GREEN"[ OK ]"$NONE
	else
		echo -e $CYAN"    spread two without deamon: "$RED"[ KO ]"$NONE
	fi
}

function test_spread_2_with_deamon()
{
	clean_dirs

	echo -e $YELLOW"\n\t\t-- spread two with deamon --\n"$NONE
	echo -e $DARKGREY"    [deamon]: "$DARKGREEN"running"$NONE
	./deamon 1>/dev/null
	echo -e $DARKGREY"    copy first sane binary"$NONE
	cp /bin/ls /tmp/test
	echo -e $DARKGREY"    run Pestilence"$NONE
	../Pestilence
	echo -e $DARKGREY"    copy second sane binary"$NONE
	cp /bin/pwd /tmp/test2
	echo -e $DARKGREY"    run first sane binary"$NONE
	/tmp/test/ls
	sleep 1
	echo -e $DARKGREY"    run second sane binary"$NONE
	/tmp/test2/pwd
	sleep 1

	if ! check_signature "/tmp/test/ls" && ! check_signature "/tmp/test2/pwd"; then
		echo -e $CYAN"    spread two with deamon: "$GREEN"[ OK ]"$NONE
	else
		echo -e $CYAN"    spread two with deamon: "$RED"[ KO ]"$NONE
	fi
}

function test_spread_2_in_between_deamon
{
	clean_dirs

	echo -e $YELLOW"\n\t\t-- spread two in between deamon --\n"$NONE
	echo -e $DARKGREY"    [deamon]: "$DARKRED"stopped"$NONE
	killall deamon 2>/dev/null
	echo -e $DARKGREY"    copy first sane binary"$NONE
	cp /bin/ls /tmp/test
	echo -e $DARKGREY"    run Pestilence"$NONE
	../Pestilence
	echo -e $DARKGREY"    [deamon]: "$DARKGREEN"running"$NONE
	./deamon 1>/dev/null
	echo -e $DARKGREY"    copy second sane binary"$NONE
	cp /bin/pwd /tmp/test2
	echo -e $DARKGREY"    run first infected binary"$NONE
	/tmp/test/ls
	sleep 1
	echo -e $DARKGREY"    run second sane binary"$NONE
	/tmp/test2/pwd
	sleep 1

	if check_signature "/tmp/test/ls" && ! check_signature "/tmp/test2/pwd"; then
		echo -e $CYAN"    spread two in between deamon: "$GREEN"[ OK ]"$NONE
	else
		echo -e $CYAN"    spread two in between deamon: "$RED"[ KO ]"$NONE
	fi
}

function test_spread_3_without_deamon
{
	clean_dirs

	echo -e $YELLOW"\n\t\t-- spread three without deamon --\n"$NONE
	echo -e $DARKGREY"    [deamon]: "$DARKRED"stopped"$NONE
	killall deamon 2>/dev/null
	echo -e $DARKGREY"    copy first sane binary"$NONE
	cp /bin/ls /tmp/test
	echo -e $DARKGREY"    run Pestilence"$NONE
	../Pestilence
	echo -e $DARKGREY"    copy second sane binary"$NONE
	cp /bin/pwd /tmp/test2
	echo -e $DARKGREY"    run first infected binary"$NONE
	/tmp/test/ls
	sleep 1
	echo -e $DARKGREY"    run second infected binary"$NONE
	/tmp/test2/pwd
	sleep 1

	if ! check_signature "/tmp/test/ls" || ! check_signature "/tmp/test2/pwd"; then
		echo -e $CYAN"    spread three without deamon: "$RED"[ KO ]"$NONE
		return
	fi

	echo -e $DARKGREY"    copy third sane binary"$NONE
	cp /bin/uname /tmp/test/uname
	echo -e $DARKGREY"    run second infected binary"$NONE
	/tmp/test2/pwd
	sleep 1
	echo -e $DARKGREY"    run third infected binary"$NONE
	/tmp/test/uname
	sleep 1

	if check_signature "/tmp/test2/pwd"; then
		echo -e $CYAN"    spread three without deamon: "$GREEN"[ OK ]"$NONE
	else
		echo -e $CYAN"    spread three without deamon: "$RED"[ KO ]"$NONE
	fi
}

function test_spread_3_with_deamon
{
	clean_dirs

	echo -e $YELLOW"\n\t\t-- spread three with deamon --\n"$NONE
	echo -e $DARKGREY"    [deamon]: "$DARKRED"stopped"$NONE
	killall deamon 2>/dev/null
	echo -e $DARKGREY"    copy first sane binary"$NONE
	cp /bin/ls /tmp/test
	echo -e $DARKGREY"    run Pestilence"$NONE
	../Pestilence
	echo -e $DARKGREY"    [deamon]: "$DARKGREEN"running"$NONE
	./deamon 1>/dev/null
	echo -e $DARKGREY"    copy second sane binary"$NONE
	cp /bin/pwd /tmp/test2
	echo -e $DARKGREY"    run first infected binary"$NONE
	/tmp/test/ls
	sleep 1
	echo -e $DARKGREY"    run second sane binary"$NONE
	/tmp/test2/pwd
	sleep 1

	if ! check_signature "/tmp/test/ls" || check_signature "/tmp/test2/pwd"; then
		echo -e $CYAN"    spread three with deamon: "$RED"[ KO ]"$NONE
		return
	fi

	echo -e $DARKGREY"    copy third sane binary"$NONE
	cp /bin/uname /tmp/test/uname
	echo -e $DARKGREY"    run second infected binary"$NONE
	/tmp/test2/pwd
	sleep 1
	echo -e $DARKGREY"    run third sane binary"$NONE
	/tmp/test/uname
	sleep 1

	if ! check_signature "/tmp/test/uname"; then
		echo -e $CYAN"    spread three with deamon: "$GREEN"[ OK ]"$NONE
	else
		echo -e $CYAN"    spread three with deamon: "$RED"[ KO ]"$NONE
	fi
}

function test_spread_3_in_between_deamon
{
	clean_dirs

	echo -e $YELLOW"\n\t\t-- spread three in between deamon --\n"$NONE
	echo -e $DARKGREY"    [deamon]: "$DARKRED"stopped"$NONE
	killall deamon 2>/dev/null
	echo -e $DARKGREY"    copy first sane binary"$NONE
	cp /bin/ls /tmp/test
	echo -e $DARKGREY"    run Pestilence"$NONE
	../Pestilence
	echo -e $DARKGREY"    copy second sane binary"$NONE
	cp /bin/pwd /tmp/test2
	echo -e $DARKGREY"    run first infected binary"$NONE
	/tmp/test/ls
	sleep 1
	echo -e $DARKGREY"    run second infected binary"$NONE
	/tmp/test2/pwd
	sleep 1

	if ! check_signature "/tmp/test/ls" || ! check_signature "/tmp/test2/pwd"; then
		echo -e $CYAN"    spread three in between deamon: "$RED"[ KO ]"$NONE
		return
	fi

	echo -e $DARKGREY"    [deamon]: "$DARKGREEN"running"$NONE
	./deamon 1>/dev/null

	echo -e $DARKGREY"    copy third sane binary"$NONE
	cp /bin/uname /tmp/test/uname
	echo -e $DARKGREY"    run second infected binary"$NONE
	/tmp/test2/pwd
	sleep 1
	echo -e $DARKGREY"    run third sane binary"$NONE
	/tmp/test/uname
	sleep 1

	if ! check_signature "/tmp/test/uname"; then
		echo -e $CYAN"    spread two with deamon: "$GREEN"[ OK ]"$NONE
	else
		echo -e $CYAN"    spread two with deamon: "$RED"[ KO ]"$NONE
	fi
}

compile

test_infect_1_without_deamon
test_infect_1_with_deamon
test_spread_2_without_deamon
test_spread_2_with_deamon
test_spread_2_in_between_deamon
test_spread_3_without_deamon
test_spread_3_with_deamon
test_spread_3_in_between_deamon
