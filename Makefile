test: test_txt.cpp image_processor/*.* packet_sender/*.*
	g++ -o test test_txt.cpp image_processor/image_processor.cpp packet_sender/e131_sender.cpp -le131
mac: test_txt.cpp image_processor/*.* packet_sender/*.*
	g++ -std=c++11 -o test test.cpp image_processor/image_processor.cpp packet_sender/e131_sender.cpp -I/opt/homebrew/include/opencv4 -L/opt/homebrew/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -le131

video: video_test.cpp image_processor/*.* packet_sender/*.*
	g++ -std=c++11 -o test video_test.cpp image_processor/image_processor.cpp packet_sender/e131_sender.cpp -I/opt/homebrew/include/opencv4 -L/opt/homebrew/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -le131

image: image_test.cpp image_processor/*.* packet_sender/*.*
	g++ -std=c++11 -stdlib=libc++ -o image_test image_test.cpp image_processor/image_processor.cpp packet_sender/e131_sender.cpp -I/opt/homebrew/include/opencv4 -L/opt/homebrew/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -le131
