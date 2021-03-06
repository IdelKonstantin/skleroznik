#include <memory>
#include <cstdint>
#include <iostream>
#include <map>
#include <unordered_map>
#include <evhttp.h>

const char* getRequestType(const evhttp_request* const request) {

  std::unordered_map<evhttp_cmd_type, const char*> requestType {

      {EVHTTP_REQ_GET, "GET"},
      {EVHTTP_REQ_POST, "POST"},
      {EVHTTP_REQ_HEAD, "HEAD"},
      {EVHTTP_REQ_PUT, "PUT"},
      {EVHTTP_REQ_DELETE, "DELETE"},
      {EVHTTP_REQ_OPTIONS, "OPTIONS"},
      {EVHTTP_REQ_TRACE, "TRACE"},
      {EVHTTP_REQ_CONNECT, "CONNECT"},
      {EVHTTP_REQ_PATCH, "PATCH"}
  };

  return requestType[evhttp_request_get_command(request)];
}

std::string getURI(const evhttp_request* const request) {

  return evhttp_request_get_uri(request);
}

using header_container_t = std::map<std::string, std::string>;

header_container_t getHeaders(evhttp_request* const request) {

  header_container_t headersData;

  struct evkeyvalq* headers = evhttp_request_get_input_headers(request); // Здесь должен быть shared_ptr
  struct evkeyval* header;

  for (header = headers->tqh_first; header; header = header->next.tqe_next) {
    headersData.insert(std::make_pair(header->key, header->value));
  }

  evhttp_clear_headers(headers);

  return headersData;
}

std::string getBody(evhttp_request* const request) {

  struct evbuffer* buf = evhttp_request_get_input_buffer(request); // + auto buffer = std::make_shared<evbuffer, decltype(&evbuffer_free)>(evbuffer_new(), &evbuffer_free);
  size_t len = evbuffer_get_length(buf);

  std::unique_ptr<char[]> body{new char[len]};

  evbuffer_copyout(buf, (char*)body.get(), len);
  return std::string{body.get()};
}

std::pair<std::string, std::string> getClientEndPoint(evhttp_request* const request) {

  char* clientIP = nullptr;
  unsigned short clientPort = 0;
  
  evhttp_connection_get_peer(evhttp_request_get_connection(request), &clientIP, &clientPort);

  if(clientIP && clientPort) {

    return std::make_pair(clientIP, std::to_string(clientPort));
  }
  else
  {
    return std::make_pair("", "");
  }
}

std::string getPath(evhttp_request* const request) {

  return{evhttp_uri_get_path(request->uri_elems)};
}


void OnReq (evhttp_request *req, void *) {

  auto clientData = getClientEndPoint(req);

  std::cout << "Client host: " << clientData.first << " client port: " 
  << clientData.second << std::endl;

  std::cout << "Request path: " << getPath(req) << std::endl;

  printf("Request method: %s\n", getRequestType(req));
  printf("Request URI: %s\n", getURI(req).c_str());
  printf("Request headers: \n");

  for(const auto& i : getHeaders(req)) {

    printf("\t%s : %s\n", i.first.c_str(), i.second.c_str());
  }

  std::cout << "Request body: " << getBody(req) << std::endl;

  auto *OutBuf = evhttp_request_get_output_buffer(req);
  evbuffer_add_printf(OutBuf, "empty URI (may be: /get_songlist or /get_by_uid?uid=SONG_FILE_UID)");
  //evbuffer_add_printf(OutBuf, "<html><body><center><h2>Hello world!</h2></center></body></html>");
  evhttp_send_reply(req, HTTP_OK, "", OutBuf);
}

//curl -XPOST http://127.0.0.1:5555/hellasdasdasdo -d 'json_takiy_json' -H 'Content-Type: json'

int main() {

  event_init(); //OBSOLETE?
  std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start("127.0.0.1", 5555), &evhttp_free);

  evhttp_set_gencb(Server.get(), OnReq, nullptr);
  event_dispatch();
  return 0;
}
