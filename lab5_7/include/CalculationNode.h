#pragma once

#include "ZMQFunctions.h"
#include "unistd.h"

class CalculationNode {
public:
    int id;
    int left_id = -2, right_id = -2, parent_id;
    int left_port, right_port, parent_port;
    zmq::context_t context;
    zmq::socket_t left, right, parent;

    CalculationNode(int id, int parent_port, int parent_id)
        : id(id), left_id(-2), right_id(-2), parent_id(parent_id),
          left_port(0), right_port(0), parent_port(parent_port),
          context(), left(context, ZMQ_REQ), right(context, ZMQ_REQ), parent(context, ZMQ_REP) {
        if (id != -1) {
            connect(parent, parent_port);
        }
    }

    std::string create(int child_id) {
        int port;
        bool isLeft = false;
        if (left_id == -2) {
            left_port = bind(left, child_id);
            left_id = child_id;
            port = left_port;
            isLeft = true;
        } else if (right_id == -2) {
            right_port = bind(right, child_id);
            right_id = child_id;
            port = right_port;
        } else {
            return "Error: Can not create the calculation node";
        }

        int fork_id = fork();
        if (fork_id == 0) {
            const char* pathToServer = getenv("PATH_TO_CALCULATION_NODE");
            if (execl(pathToServer, "server", std::to_string(child_id).c_str(),
                      std::to_string(port).c_str(), std::to_string(id).c_str(), 
                      (char*)NULL) == -1) {
                std::cerr << "Error: Can not run the execl-command" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else {
            try {
                zmq::socket_t& childSocket = isLeft ? left : right;
                childSocket.setsockopt(ZMQ_SNDTIMEO, 3000);
                send_message(childSocket, "pid");
                std::string child_pid = receive_message(childSocket);
                return "Ok: " + child_pid;
            } catch (const std::exception& e) {
                return "Error: Сan not connect to the child";
            }
        }
        return "Error: Unexpected error in create function";
    }

    std::string ping(int id) {
        std::string answer = "Ok: 0";
        if (this->id == id) {
            answer = "Ok: 1";
            return answer;
        }
        if (left_id == id) {
            std::string message = "ping " + std::to_string(id);
            send_message(left, message);
            try {
                message = receive_message(left);
                if (message == "Ok: 1") {
                    answer = message;
                }
            }
            catch(int){}
        } else if (right_id == id) {
            std::string message = "ping " + std::to_string(id);
            send_message(right, message);
            try {
                message = receive_message(right);
                if (message == "Ok: 1") {
                    answer = message;
                }
            }
            catch(int){}
        }
        return answer;
    }

    std::string sendstring(std::string string, int id) {
        std::string answer = "Error: Parent not found";
        if (left_id == -2 && right_id == -2) return answer;
        if (left_id == id) {
            if (ping(left_id) == "Ok: 1") {
                send_message(left, string);
                try {
                    answer = receive_message(left);
                }
                catch(int){}
            }
        } else if (right_id == id) {
            if (ping(right_id) == "Ok: 1") {
                send_message(right, string);
                try {
                    answer = receive_message(right);
                }
                catch(int){}
            }
        } else {
            if (ping(left_id) == "Ok: 1") {
                std::string message = "send " + std::to_string(id) + " " + string;
                send_message(left, message);
                try {
                    message = receive_message(left);
                }
                catch(int) {
                    message = "Error: Parent not found";
                }
                if (message != "Error: Parent not found") answer = message;
            }
            if (ping(right_id) == "Ok: 1") {
                std::string message = "send " + std::to_string(id) + " " + string;
                send_message(right, message);
                try {
                    message = receive_message(right);
                }
                catch(int) {
                    message = "Error: Parent not found";
                }
                if (message != "Error: Parent not found") answer = message;
            }
        }
        return answer;
    }

    std::string exec(std::string string) {
        std::istringstream string_thread(string);
        int result = 0;
        int amount, number;
        string_thread >> amount;
        for (int i = 0; i < amount; ++i) {
            string_thread >> number;
            result += number;
        }
        std::string answer = "Ok: " + std::to_string(id) + ": " + std::to_string(result);
        return answer;
    }

    std::string treeclear(int child) {
        if (left_id == child) {
            left_id = -2;
            unbind(left, left_port);
        }
        else {
            right_id = -2;
            unbind(right, right_port);
        }
        return "Ok";
    }

    std::string kill() { 
        if (left_id != -2){
            if (ping(left_id) == "Ok: 1") {
                std::string message = "kill";
                send_message(left, message);
                try {
                    message = receive_message(left);
                }
                catch(int){}
                unbind(left, left_port);
                left.close();
            }
        }
        if (right_id != -2) {
            if (ping(right_id) == "Ok: 1") {
                std::string message = "kill";
                send_message(right, message);
                try {
                    message = receive_message(right);
                }
                catch (int){}
                unbind(right, right_port);
                right.close();
            }
        }
        return std::to_string(parent_id);
    }

    ~CalculationNode() {}
};