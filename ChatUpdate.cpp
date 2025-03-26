#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstring>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <termios.h>
#include <fcntl.h>

using namespace std;

// ANSI color codes
#define RESET   "\033[0m"
#define SKYBLUE "\033[36m" // Sky blue (Cyan) for Server
#define LIGHTGREEN "\033[92m" // Light green for Client

// Profile structure
struct Profile {
    string name;
    string email;
    string mac;
};

// Global profile variable
Profile user_profile;

// Function prototypes
void run_server();
void run_client();
void create_or_update_profile();
void load_profile();
void save_profile();
void scan_network(vector<pair<string, string>>& devices);
void messaging_server();
void messaging_client();
string get_mac_from_arp(const string& ip);
void ping_ip(const string& ip);
void print_with_typing(const string& message, const string& color);
void receive_messages(int sock, const string& sender_color, atomic<bool>& running, mutex& display_mutex);
void set_nonblocking_input();
string get_input(atomic<bool>& running);

// Mutex for synchronizing display
mutex display_mutex;

// Main function
int main() {
    int mode;
    load_profile();

    while (true) {
        cout << "=====================\n";
        cout << "   Network Tool\n";
        cout << "=====================\n";
        cout << "1. Profile\n";
        cout << "2. Server Mode (Send first)\n";
        cout << "3. Client Mode (Listen only)\n";
        cout << "Select mode (1/2/3): ";
        cin >> mode;
        cin.ignore();

        if (mode == 1) {
            create_or_update_profile();
        } else if (mode == 2) {
            if (user_profile.name.empty()) {
                cout << "Please set your profile first!\n";
                continue;
            }
            run_server();
        } else if (mode == 3) {
            if (user_profile.name.empty()) {
                cout << "Please set your profile first!\n";
                continue;
            }
            run_client();
        } else {
            cout << "Invalid input!\n";
        }
    }
    return 0;
}

// Server mode
void run_server() {
    vector<pair<string, string>> devices;
    int choice;

    while (true) {
        cout << "\n=====================\n";
        cout << "   Server Menu\n";
        cout << "=====================\n";
        cout << "1. Scan Network\n";
        cout << "2. Messaging\n";
        cout << "Select option (1/2): ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            scan_network(devices);
        } else if (choice == 2) {
            messaging_server();
        } else {
            cout << "Invalid input!\n";
        }
    }
}

// Client mode
void run_client() {
    cout << "Client mode activated. Listening for incoming messages...\n";
    messaging_client();
}

// Load profile from file
void load_profile() {
    ifstream file("profile.txt");
    if (file.is_open()) {
        getline(file, user_profile.name);
        getline(file, user_profile.email);
        getline(file, user_profile.mac);
        file.close();
    }
}

// Save profile to file
void save_profile() {
    ofstream file("profile.txt");
    if (file.is_open()) {
        file << user_profile.name << endl;
        file << user_profile.email << endl;
        file << user_profile.mac << endl;
        file.close();
    }
}

// Create or update profile function
void create_or_update_profile() {
    cout << "Current Profile:\n";
    if (!user_profile.name.empty()) {
        cout << "Name: " << user_profile.name << endl;
        cout << "Email: " << user_profile.email << endl;
        cout << "MAC Address: " << user_profile.mac << endl;
    } else {
        cout << "No profile set yet.\n";
    }

    cout << "\nEnter your name: ";
    getline(cin, user_profile.name);
    cout << "Enter your email address: ";
    getline(cin, user_profile.email);

    user_profile.mac = "00:1A:2B:3C:4D:5E";
    cout << "Your device MAC Address: " << user_profile.mac << endl;
    cout << "Profile updated successfully!\n";

    save_profile();
}

// Network scan function (ARP-based)
void scan_network(vector<pair<string, string>>& devices) {
    devices.clear();
    cout << "Scanning network... (This may take a few seconds)\n";

    string base_ip = "192.168.1.";
    vector<thread> threads;

    for (int i = 1; i <= 254; i++) {
        string ip = base_ip + to_string(i);
        threads.push_back(thread(ping_ip, ip));
    }

    for (auto& t : threads) {
        t.join();
    }

    for (int i = 1; i <= 254; i++) {
        string ip = base_ip + to_string(i);
        string mac = get_mac_from_arp(ip);
        if (!mac.empty() && mac != "00:00:00:00:00:00") {
            devices.push_back({ip, mac});
        }
    }

    cout << "\n+-------------------------+\n";
    cout << "| IP Address    | MAC Address    |\n";
    cout << "+-------------------------+\n";
    for (const auto& device : devices) {
        cout << "| " << setw(13) << left << device.first << " | " << device.second << " |\n";
    }
    cout << "+-------------------------+\n";
    if (devices.empty()) {
        cout << "No devices found! Try adjusting the base IP or check network connectivity.\n";
    } else {
        cout << "Scan completed. " << devices.size() << " device(s) found.\n";
    }
}

// Read MAC address from ARP table
string get_mac_from_arp(const string& ip) {
    ifstream arp_file("/proc/net/arp");
    string line, arp_ip, mac;
    while (getline(arp_file, line)) {
        istringstream iss(line);
        iss >> arp_ip >> hex >> mac >> mac >> mac;
        if (arp_ip == ip) {
            arp_file.close();
            return mac;
        }
    }
    arp_file.close();
    return "";
}

// Ping an IP (to populate ARP table)
void ping_ip(const string& ip) {
    string command = "ping -c 1 " + ip + " > /dev/null 2>&1";
    system(command.c_str());
}

// Typing animation function
void print_with_typing(const string& message, const string& color) {
    cout << color;
    for (char c : message) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    cout << RESET << endl;
}

// Set terminal to non-blocking input
void set_nonblocking_input() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~(ICANON | ECHO);
    ttystate.c_cc[VMIN] = 0;
    ttystate.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

// Get input non-blocking (Updated)
string get_input(atomic<bool>& running) {
    string input;
    char c;
    cout << "Enter Message> ";
    cout.flush();
    while (running) {
        if (read(STDIN_FILENO, &c, 1) > 0) {
            if (c == '\n') {
                if (!input.empty()) {
                    cout << endl; // এন্টার চাপার পর লাইন ব্রেক
                    return input;
                }
                cout << endl << "Enter Message> " << flush; // খালি এন্টার দিলে নতুন প্রম্পট
            } else if (c == 127) { // Backspace
                if (!input.empty()) {
                    input.pop_back();
                    cout << "\b \b" << flush;
                }
            } else {
                input += c;
                cout << c << flush;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    return "";
}

// Receive messages in a separate thread
void receive_messages(int sock, const string& sender_color, atomic<bool>& running, mutex& display_mutex) {
    char buffer[1024] = {0};
    while (running) {
        int valread = read(sock, buffer, 1024);
        if (valread <= 0) {
            lock_guard<mutex> lock(display_mutex);
            cout << "Disconnected.\n";
            cout << "Enter Message> ";
            cout.flush();
            running = false;
            break;
        }
        {
            lock_guard<mutex> lock(display_mutex);
            print_with_typing(string(buffer), sender_color);
            cout << "Enter Message> ";
            cout.flush();
        }
        memset(buffer, 0, 1024);
    }
}

// Messaging for server (initiates connection)
void messaging_server() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    atomic<bool> running(true);

    string target_ip;
    cout << "Enter destination IP address: ";
    getline(cin, target_ip);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, target_ip.c_str(), &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection failed! Check IP or client status.\n";
        close(sock);
        return;
    }

    cout << "Connected to " << target_ip << " successfully!\n";
    set_nonblocking_input();
    thread recv_thread(receive_messages, sock, LIGHTGREEN, ref(running), ref(display_mutex));
    recv_thread.detach();

    while (running) {
        string message = get_input(running);
        if (!message.empty()) {
            if (message == "exit") {
                running = false;
                break;
            }
            string full_message = user_profile.name + ": " + message;
            send(sock, full_message.c_str(), full_message.length(), 0);
        }
    }
    close(sock);
}

// Messaging for client (listens only)
void messaging_client() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    atomic<bool> running(true);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            cout << "Failed to accept connection!\n";
            continue;
        }
        cout << "Server connected from " << inet_ntoa(address.sin_addr) << "\n";
        set_nonblocking_input();
        thread recv_thread(receive_messages, new_socket, SKYBLUE, ref(running), ref(display_mutex));
        recv_thread.detach();

        while (running) {
            string reply = get_input(running);
            if (!reply.empty()) {
                if (reply == "exit") {
                    running = false;
                    break;
                }
                string full_reply = user_profile.name + ": " + reply;
                send(new_socket, full_reply.c_str(), full_reply.length(), 0);
            }
        }
        close(new_socket);
        running = true;
    }
    close(server_fd);
}