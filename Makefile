test: test.cpp image_processor/*.* packet_sender/*.*
	g++ -o test test.cpp image_processor/image_processor.cpp packet_sender/e131_sender.cpp -le131
