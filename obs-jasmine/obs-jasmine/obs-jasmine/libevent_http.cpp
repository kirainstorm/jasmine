//----------------------------------------
// kirainstorm
// https://github.com/kirainstorm
//----------------------------------------
#include "pch.h"
#include "libevent_http.h"
#include <string.h>
#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp  _strnicmp 
#endif

namespace libevent
{
	inline char* XMallocBuffer(int32_t in_size, int32_t& out_size)
	{
		out_size = (int32_t)(ceil(in_size * 1.0 / 1024) * 1024);
		char* buffer = (char*)malloc(out_size);
		if (nullptr == buffer)
		{
			//XLoggerError("[%s][%d][%s]:%s", __FILENAME__, __LINE__, __FUNCTION__, "XMallocBuffer");
		}
		assert(buffer);
		memset(buffer, 0, out_size);
		return buffer;
	}
	inline void XFreeBuffer(void* buffer)
	{
		if (buffer)
		{
			free(buffer);
			buffer = nullptr;
		}
		else
		{
			//XLoggerError("[%s][%d][%s]:%s", __FILENAME__, __LINE__, __FUNCTION__, "XFreeBuffer");
		}
	}

#define HTTP_BUF_MAX (2*1024*1024)
	std::mutex mutex_;

	//16进制数转化为10进制, return 0不会出现
	int hexit(char c)
	{
		if (c >= '0' && c <= '9')
			return c - '0';
		if (c >= 'a' && c <= 'f')
			return c - 'a' + 10;
		if (c >= 'A' && c <= 'F')
			return c - 'A' + 10;

		return 0;
	}
	/*
	 * 这里的内容是处理%20之类的东西！是"解码"过程。
	 * %20 URL编码中的‘ ’(space)
	 * %21 '!' %22 '"' %23 '#' %24 '$'
	 * %25 '%' %26 '&' %27 ''' %28 '('......
	 * 相关知识html中的‘ ’(space)是&nbsp
	 */
	void strdecode(char* to, char* from)
	{
		for (; *from != '\0'; ++to, ++from)
		{
			if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
			{
				// 依次判断from中 %20 三个字符
				*to = hexit(from[1]) * 16 + hexit(from[2]);
				// 移过已经处理的两个字符(%21指针指向1),表达式3的++from还会再向后移一个字符
				from += 2;
			}
			else
			{
				*to = *from;
			}
		}
		*to = '\0';
	}

	// "编码"，用作回写浏览器的时候，将除字母数字及/_.-~以外的字符转义后回写。
	// strencode(encoded_name, sizeof(encoded_name), name);
	void strencode(char* to, size_t tosize, const char* from)
	{
		int tolen;

		for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from)
		{
			if (isalnum(*from) || strchr("/_.-~", *from) != (char*)0)
			{
				*to = *from;
				++to;
				++tolen;
			}
			else
			{
				sprintf(to, "%%%02x", (int)*from & 0xff);
				to += 3;
				tolen += 3;
			}
		}
		*to = '\0';
	}
	const char* get_file_type(char* name)
	{
		char* dot;
		dot = strrchr(name, '.');
		if (dot == (char*)0)
			return "text/plain";
		if (strcmp(dot, ".htm") == 0)
			return "text/html";
		if (strcmp(dot, ".html") == 0)
			return "text/html";
		if (strcmp(dot, ".php") == 0)
			return "text/html";
		if (strcmp(dot, ".css") == 0)
			return "text/css";
		if (strcmp(dot, ".txt") == 0)
			return "text/plain";
		if (strcmp(dot, ".js") == 0)
			return "application/javascript";
		if (strcmp(dot, ".json") == 0)
			return "application/json";
		if (strcmp(dot, ".xml") == 0)
			return "application/xml";
		if (strcmp(dot, ".swf") == 0)
			return "application/x-shockwave-flash";
		if (strcmp(dot, ".flv") == 0)
			return "video/x-flv";
		if (strcmp(dot, ".png") == 0)
			return "image/png";
		if (strcmp(dot, ".jpe") == 0)
			return "image/jpeg";
		if (strcmp(dot, ".jpeg") == 0)
			return "image/jpeg";
		if (strcmp(dot, ".jpg") == 0)
			return "image/jpeg";
		if (strcmp(dot, ".gif") == 0)
			return "image/gif";
		if (strcmp(dot, ".bmp") == 0)
			return "image/bmp";
		if (strcmp(dot, ".ico") == 0)
			return "image/vnd.microsoft.icon";
		if (strcmp(dot, ".tiff") == 0)
			return "image/tiff";
		if (strcmp(dot, ".tif") == 0)
			return "image/tiff";
		if (strcmp(dot, ".svg") == 0)
			return "image/svg+xml";
		if (strcmp(dot, ".svgz") == 0)
			return "image/svg+xml";
		return "application/text";
	}

	int read_whole_file(const char *filename, struct bufferevent *bev)
	{

		return 0;
	}

	int http_get_response_file(const char* filename, struct bufferevent* bev)
	{
		int buffer_size = 0;
		char* buffer = XMallocBuffer(HTTP_BUF_MAX, buffer_size);
		FILE * fd;
		fd = fopen(filename, "rb");
		if (fd && buffer)
		{
			int size = fread(buffer, 1, HTTP_BUF_MAX, fd);
			if (size > 0)
				bufferevent_write(bev, buffer, size);
			fclose(fd);
		}
		XFreeBuffer(buffer);
		return 0;
	}
	int http_get_response_header(struct bufferevent* bev, int no, const char* desp, const char* type, long len)
	{
		char buf[256] = { 0 };
		sprintf(buf, "HTTP/1.1 %d %s\r\n", no, desp);
		bufferevent_write(bev, buf, strlen(buf)); //HTTP/1.1 200 OK\r\n
		sprintf(buf, "Content-Type:%s\r\n", type);
		bufferevent_write(bev, buf, strlen(buf)); // 文件类型
		sprintf(buf, "Content-Length:%ld\r\n", len);
		bufferevent_write(bev, buf, strlen(buf));                                         // 文件大小
		bufferevent_write(bev, "Connection: close\r\n", strlen("Connection: close\r\n")); // Connection: close
		bufferevent_write(bev, "\r\n", 2);                                                //send \r\n
		return 0;
	}

	int http_get_response_file_error(struct bufferevent* bev)
	{
		char pf[128] = "www/404.html";
		struct stat sb;
		stat(pf, &sb);
		http_get_response_header(bev, 404, "File Not Found", "text/html", sb.st_size);
		http_get_response_file("www/404.html", bev);
		return 0;
	}
	int http_get_response(struct bufferevent* bev, const char* method, char* path)
	{
		if (strcasecmp("GET", method) == 0)
		{
			//get method ...
			strdecode(path, path);
			char* pf = &path[1];
			char file_path[256] = { 0 }; //"./www/";

			if (strcmp(path, "/") == 0 || strcmp(path, "/.") == 0)
			{
				sprintf(file_path, "%s", "www/index.html");
			}
			else
			{
				sprintf(file_path, "www/%s", pf);
			}

			printf("***** http Request Resource Path =  %s, file_path = %s\n", path, file_path);
			struct stat sb;
			if (stat(file_path, &sb) < 0)
			{
				perror("open file err:");
				http_get_response_file_error(bev);
				return 0;
			}
			// 			if (S_ISDIR(sb.st_mode)) //处理目录
			// 			{
			// 				//应该显示目录列表
			// 				//send_header(bev, 200, "OK", get_file_type(".html"), -1);
			// 				//send_dir(bev, pf);
			// 			}
			// 			else //处理文件
			{
				http_get_response_header(bev, 200, "OK", get_file_type(file_path), sb.st_size);
				http_get_response_file(file_path, bev);
			}
		}
		return 0;
	}
	void conn_read_callback(struct bufferevent* bev, void* user_data)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		//printf("******************** begin call %s.........\n", __FUNCTION__);
		char buf[8192] = { 0 };
		char method[32], path[1024], protocol[32];
		bufferevent_read(bev, buf, sizeof(buf));
		printf("buf[%s]\n", buf);
		sscanf(buf, "%[^ ] %[^ ] %[^ \r\n]", method, path, protocol);
		//printf("method[%s], path[%s], protocol[%s]\n", method, path, protocol);
		if (strcasecmp(method, "GET") == 0)
		{
			http_get_response(bev, method, path);
		}
		//Sleep(1);
	}
	void conn_event_callback(struct bufferevent* bev, short events, void* user_data)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (events & BEV_EVENT_EOF)
		{
			printf("Connection closed.\n");
		}
		else if (events & BEV_EVENT_ERROR)
		{
			printf("Got an error on the connection: %s\n", strerror(errno));
		}
		bufferevent_free(bev);
	}
	void listener_callback(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int socklen, void* event_base_instance)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		struct event_base* base = (event_base*)event_base_instance;
		struct bufferevent* bev;
		//printf("fd is %d\n", fd);
		bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
		if (!bev){
			//XLoggerError("[%s][%d][%s]:Error constructing bufferevent!=>event_base_loopbreak", __FILENAME__, __LINE__, __FUNCTION__);
			event_base_loopbreak(base);
			return;
		}
		bufferevent_flush(bev, EV_READ | EV_WRITE, BEV_NORMAL);
		bufferevent_setcb(bev, conn_read_callback, NULL, conn_event_callback, NULL);
		bufferevent_enable(bev, EV_READ | EV_WRITE);
	}
	http::http() {
		WORD wVersionRequested = MAKEWORD(2, 2);
		WSADATA wsaData;
		WSAStartup(wVersionRequested, &wsaData);

		thread_exit_ = true;
#ifdef _MSC_VER
		evthread_use_windows_threads();
#endif
		base = event_base_new();
		if (!base) {
			//XLoggerError("[%s][%d][%s]:event_base_new failed", __FILENAME__, __LINE__, __FUNCTION__);
		}

	};
	http::~http() {
		event_base_free(base);
	};
	void http::start()
	{
		if (thread_exit_){
			thread_exit_ = false;
			threads_.push_back(std::thread([this]() {
				worker();
				}));
		}
	};
	void http::stop()
	{
		if (!thread_exit_) {
			thread_exit_ = true;
			event_base_loopbreak(base);
			for (auto& thread : threads_) {
				thread.join();
			}
		}
	};
	void http::worker()
	{
		struct evconnlistener* listener;
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(732);
		listener = evconnlistener_new_bind(base, listener_callback, (void*)base,
			LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
			(struct sockaddr*)&sin, sizeof(sin));
		if (!listener) {
			//XLoggerError("[%s][%d][%s]:evconnlistener_new_bind failed", __FILENAME__, __LINE__, __FUNCTION__);
			return;
		}
		event_base_dispatch(base);
		evconnlistener_free(listener);
		return;
	}
}