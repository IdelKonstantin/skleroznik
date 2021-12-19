#include <memory>
#include <cstdint>
#include <iostream>
#include <evhttp.h>
int main()
{
  if (!event_init())
  {
    std::cerr << "Failed to init libevent." << std::endl;
    return -1;
  }
  char const SrvAddress[] = "127.0.0.1";
  std::uint16_t SrvPort = 5555;
  std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);
  if (!Server)
  {
    std::cerr << "Failed to init http server." << std::endl;
    return -1;
  }
  void (*OnReq)(evhttp_request *req, void *) = [] (evhttp_request *req, void *)
  {
    auto *OutBuf = evhttp_request_get_output_buffer(req);
    if (!OutBuf)
      return;

    const char *cmdtype;

    switch (evhttp_request_get_command(req)) {
      case EVHTTP_REQ_GET: cmdtype = "GET"; break;
      case EVHTTP_REQ_POST: cmdtype = "POST"; break;
      case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
      case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
      case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
      case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
      case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
      case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
      case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
      default: cmdtype = "unknown"; break;
    }

    const char* URI = evhttp_request_get_uri(req);

    struct evkeyvalq* headers = evhttp_request_get_input_headers(req);
    struct evkeyval *header;

    printf("Request type: %s\n", cmdtype);
    printf("Request URI: %s\n", URI);
    printf("Request headers: \n");

    for (header = headers->tqh_first; header; header = header->next.tqe_next) {
      printf("\t%s: %s\n", header->key, header->value);
    }

    struct evbuffer *buf = evhttp_request_get_input_buffer(req);

    char data[0xFFF];

    evbuffer_copyout(buf, data, evbuffer_get_length(buf));

    printf("Request body: %s\n", data);

    evbuffer_add_printf(OutBuf, "<html><body><center><h1>Hello world!</h1></center></body></html>");
    evhttp_send_reply(req, HTTP_OK, "", OutBuf);
  };
  evhttp_set_gencb(Server.get(), OnReq, nullptr);
  if (event_dispatch() == -1)
  {
    std::cerr << "Failed to run message loop." << std::endl;
    return -1;
  }
  return 0;
}
