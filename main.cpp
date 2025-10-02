#include <iostream>
#include <string>
#include <serial/serial.h>

int main() {

    std::cout << "[ports]\n";
    auto ports = serial::list_ports();
    for (auto port : ports) {
        std::cout << port.port << '\n';
    }
    serial::Serial arduino_serial("COM4", 9600, serial::Timeout::simpleTimeout(250));

    std::string send_buffer;
    std::string receive_buffer;
    std::cout << ">> Enter a message: \n";
    std::getline(std::cin, send_buffer);
    arduino_serial.write(send_buffer + '\n');
    receive_buffer = arduino_serial.readline(65536, "\n");
    std::cout << "\n>> Received message: (length: " << receive_buffer.length() << ")\n";
    std::cout << receive_buffer << '\n';

    arduino_serial.close();

}
