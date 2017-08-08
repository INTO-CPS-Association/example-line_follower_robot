This is a manual implementation of SUT.

Commands:
sut_build
	c:/python27/python.exe <PROJECT-LOCAL-PATH>/sut/sut-build.py	

trace-result
	<ENV-RESOLVED-BY-SYSTEM:RTTDIR>/bin/rtt-trace-result.py --trace-req

TP-LTL:
	Finally ([sensorLeftVal == 150 && sensorRightVal == 150])
	Finally ([sensorLeftVal < 150 && sensorRightVal < 150])
	Finally ([sensorLeftVal < 150 && sensorRightVal > 155])
	Finally ([sensorLeftVal > 155 && sensorRightVal < 150])
	Finally ([sensorLeftVal > 155 && sensorRightVal > 155])
