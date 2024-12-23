#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "ProcessNumbers.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <string>  
#include <filesystem>
#include <cstdio>
#include <cstdlib>


using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental::listener;



void handle_options(http_request request) {
    http_response response(status_codes::OK);
    response.headers().add("Access-Control-Allow-Origin", "*");
    response.headers().add("Access-Control-Allow-Methods", "POST, OPTIONS");
    response.headers().add("Access-Control-Allow-Headers", "Content-Type");
    request.reply(response);
}

void handle_post(http_request request) {
    
  request.extract_json().then([=](json::value request_data) {
    if (request_data.is_null()) {
      std::cout << "Тело запроса пусто!" << std::endl;
      http_response response(status_codes::BadRequest);
      response.set_body(json::value::string(U("Невозможно обработать пустой запрос")));
      request.reply(response);
    } else {
      if (request_data.has_field(U("numbers"))) {
        auto numbers_array = request_data[U("numbers")].as_array();

        std::vector<int> numbers;
        for (const auto& value : numbers_array) {
          numbers.push_back(value.as_integer());
        }

        json::value response_data;
        response_data[U("message")] = json::value::string(U("Запрос успешно обработан"));

        char filename[] = "/tmp/file_XXXXXX";

        int fd = mkstemp(filename);
        fchmod(fd, 0777);
        if (fd == -1) {
          response_data[U("err_mkstemp")] = json::value::string("Ошибка при создании временного файла");
          http_response response(status_codes::OK);
          response.set_body(response_data);
          request.reply(response);
          return;
        }

        FILE* tmpf = fdopen(fd, "w+");
        if (tmpf == nullptr) {
          response_data[U("fdopen_error")] = json::value::string("Ошибка при создании временного файла fopen");
          http_response response(status_codes::OK);
          response.set_body(response_data);
          request.reply(response);
          return;
        }
        // if (request_data.has_field(U("onCuda"))) {
        //   auto onCuda = request_data[U("onCuda")].as_integer();
        // }
        process_numbers(tmpf, numbers);
        std::stringstream buffer;
        buffer << std::filesystem::read_symlink(
             std::filesystem::path("/proc/self/fd") / std::to_string(fileno(tmpf))
         ) << '\n';
        response_data[U("file")] = json::value::string(buffer.str());
        fclose(tmpf);

        http_response response(status_codes::OK);
        response.set_body(response_data);
        request.reply(response);
      } else {
        http_response response(status_codes::BadRequest);
        response.set_body(json::value::string(U("Не найдено поле 'numbers' в запросе")));
        request.reply(response);
      }
    }
  }).wait();

}

int main() {
    http_listener listener(U("http://0.0.0.0:8080/upload"));


    listener.support(methods::POST, handle_post);
    listener.support(methods::OPTIONS, handle_options);

    try {
        listener
          .open()
          .then([](){ std::wcout << L"Сервер запущен" << std::endl; })
          .wait();

        std::string line;
        std::getline(std::cin, line);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    while (true) {
      pause();
    }

    return 0;
}
