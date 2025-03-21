#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>   // system() 사용
#include <opencv2/opencv.hpp>

using namespace std;

string getYouTubeStreamURL(const string& youtube_url) {
    string command = "yt-dlp -g " + youtube_url + " 2>/dev/null";  // 오류 출력을 숨김
    FILE* pipe = popen(command.c_str(), "r");  // yt-dlp 실행 결과를 읽기 위한 파이프
    if (!pipe) {
        cerr << "❌ Error: Failed to run yt-dlp." << endl;
        return "";
    }

    char buffer[1024];
    string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;  // 결과를 문자열로 저장
    }
    pclose(pipe);

    // 개행 문자 제거 (출력값이 여러 줄일 경우 첫 번째 줄만 사용)
    size_t pos = result.find('\n');
    if (pos != string::npos) {
        result = result.substr(0, pos);
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || string(argv[1]) != "-url") {
        cerr << "❌ Usage: " << argv[0] << " -url <YouTube URL>" << endl;
        return -1;
    }

    string youtube_url = argv[2];
    cout << "📡 Fetching stream URL for: " << youtube_url << endl;

    // YouTube 영상의 직접 URL 가져오기
    string stream_url = getYouTubeStreamURL(youtube_url);
    if (stream_url.empty()) {
        cerr << "❌ Error: Failed to get YouTube stream URL." << endl;
        return -1;
    }

    cout << "✅ Stream URL: " << stream_url << endl;

    // OpenCV로 YouTube 영상 스트리밍
    cv::VideoCapture cap(stream_url);
    if (!cap.isOpened()) {
        cerr << "❌ Error: Could not open video stream." << endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::imshow("YouTube Stream", frame);
        if (cv::waitKey(30) == 27) break;  // ESC 키로 종료
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
