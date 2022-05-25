all:
	echo running...
	$(MAKE) -C eth-core-infrastructure all
	$(MAKE) -C eth-core-infrastructure install
	$(MAKE) -C tc8-protocols-coverage all
	$(MAKE) -C tc8-protocols-coverage install
	$(MAKE) -C frontend-com-module all
clean:
	echo clean..
	$(MAKE) -C eth-core-infrastructure clean
	$(MAKE) -C frontend-com-module clean
	$(MAKE) -C tc8-protocols-coverage clean
install:
	echo install...
	$(MAKE) -C frontend-com-module install
DUT:
	echo Updating DUT...
	sshpass -p rootroot ssh -o StrictHostKeyChecking=no -l pi 169.254.159.220 "sudo pkill DUT; rm -rf /home/pi/control-channel"
	sshpass -p "rootroot" scp -r ./eth-core-infrastructure/control-channel pi@169.254.159.220:/home/pi
	sshpass -p rootroot ssh -o StrictHostKeyChecking=no -l pi 169.254.159.220 "cd /home/pi/control-channel ; make clean ; make ; make install ; DUT&"
	echo Please check that everything is ok

clone:
	git clone git@gitlab.focus.com.tn:rbt/ethernet-tool/tc8-protocols-coverage.git
	git clone git@gitlab.focus.com.tn:rbt/ethernet-tool/frontend-com-module.git
	git clone git@gitlab.focus.com.tn:rbt/ethernet-tool/eth-core-infrastructure.git
	git clone git@gitlab.focus.com.tn:rbt/ethernet-tool/ethernet-tool.git
remove:
	echo removing old files
	rm -rf tc8-protocols-coverage
	rm -rf frontend-com-module
	rm -rf eth-core-infrastructure
	rm -rf ethernet-tool
