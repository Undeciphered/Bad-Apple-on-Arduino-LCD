#include <iostream>
#include <string>
#include <serial/serial.h>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <chrono>

uint64_t get_milliseconds() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main() {

    
    cv::VideoCapture cap("C:\\Bad Apple.mp4");
    if (!cap.isOpened()) {
        std::cerr << "Failed to load video";
        return -1;
    }

    cv::Mat frame, resized;

    std::string arduino_port_name;
    auto ports = serial::list_ports();
    for (auto port : ports) { // automaticaly selects the port with arduino
        serial::Serial test_port(port.port, 57600, serial::Timeout::simpleTimeout(250));
        uint8_t send_buffer{ 6 };
        test_port.write(&send_buffer, 1);
        uint8_t receive_buffer{};
        test_port.read(&receive_buffer, 1);
        if (receive_buffer == 7) {
            arduino_port_name = port.port;
        }
        test_port.close();
    }
    if (arduino_port_name.empty()) {
        std::cerr << "Failed to connect to arduino";
        return -1;
    }
    serial::Serial arduino_serial(arduino_port_name, 57600, serial::Timeout::simpleTimeout(250));

    while (cap.read(frame)) {
        uint64_t previous_time{get_milliseconds()};

        cv::resize(frame, frame, cv::Size(23, 17), cv::INTER_NEAREST);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        cv::threshold(frame, frame, 127, 255, cv::THRESH_BINARY);
        cv::resize(frame, resized, cv::Size(230, 170));
        cv::imshow("Video", resized);

        uint8_t send_buffer{};
        int row_ofset{0};
        int column_ofset{0};

        for (int k = 0; k < 2; k++) { // sends the video frame data to arduino
            for (int j = 0; j < 4; j++) {
                for (int row = 0; row < 8; row++) {

                    for (int i = 0; i < 5; i++) {
                        send_buffer <<= 1;
                        send_buffer |= (frame.at<uint8_t>(row + column_ofset, i + row_ofset) > 0);
                    }
                    arduino_serial.write(&send_buffer, 1);
                    send_buffer = 0;
                }
                row_ofset += 6;
            }
            column_ofset += 8;
            row_ofset = 0;
        }

        uint64_t current_time{};
        double frame_time{1000.0 / 30.0};
        current_time = get_milliseconds();

        auto sleep_time = std::chrono::milliseconds(static_cast<long long>(frame_time - (current_time - previous_time)));
        std::this_thread::sleep_for(sleep_time);
    }
    arduino_serial.close();
}
