#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>

int is_looping = 1;

static int callback_http(struct libwebsocket_context * that,
			 struct libwebsocket *wsi,
			 enum libwebsocket_callback_reasons reason,
			 void *user,
			 void *in,
			 size_t len)
{
	return 0;
}

static int callback_neo(struct libwebsocket_context * that,
                                   struct libwebsocket *wsi,
                                   enum libwebsocket_callback_reasons reason,
                                   void *user,
                                   void *in,
                                   size_t len)
{
	switch (reason) {
		case LWS_CALLBACK_ESTABLISHED:
			{
			int socket_id = libwebsocket_get_socket_fd(wsi);
			printf("<%d> connection established\n", socket_id);
			break;
			}
		case LWS_CALLBACK_CLOSED:
			{
				int socket_id = libwebsocket_get_socket_fd(wsi);
				printf("<%d> connection LWS_CALLBACK_CLOSED\n", socket_id);
				break;
			}
		case LWS_CALLBACK_RECEIVE:
			{
				size_t i;
				is_looping = strcmp(in,"quit");
				int socket_id = libwebsocket_get_socket_fd(wsi);
				unsigned char *buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + \
									     len + \
									     LWS_SEND_BUFFER_POST_PADDING);
				//invert data
				for (i=0; i < len; i++) {
					buf[LWS_SEND_BUFFER_PRE_PADDING + (len - 1) - i ] = ((char *) in)[i];
				}
				printf("received client[%d] data: %s, replying: %.*s\n", socket_id, (char *) in, (int) len,
									buf + LWS_SEND_BUFFER_PRE_PADDING);
				// send data
				libwebsocket_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], len, LWS_WRITE_TEXT);
				free(buf);
			}
			break;
		default:
			break;
	};
        return 0;
}

static struct libwebsocket_protocols protocols[] = {
/* first protocol must always be HTTP handler */
	{
		"http-only",   // name
		callback_http, // callback
		0              // per_session_data_size
	},
	{
                NULL, // protocol name - very important!
                callback_neo,   // callback
                0                          // we don't use any per session data
        },
	{
		NULL, NULL, 0   /* End of list */
	}
};

int main(void) 
{
	struct libwebsocket_context *context;
	struct lws_context_creation_info info;
	memset(&info, 0, sizeof info);
	info.port = 8000;
	info.gid = -1;
	info.uid = -1;
	info.protocols = protocols;
 	//create context
	context = libwebsocket_create_context(&info);
 
	if (context == NULL) {
		fprintf(stderr, "libwebsocket init failed\n");
	return -1;
	}

	printf("starting server...\n");

	while (is_looping) {
		libwebsocket_service(context, 50);
	}
	libwebsocket_context_destroy(context);
	return 0;
}
