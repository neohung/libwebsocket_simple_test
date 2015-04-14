#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>

int is_looping = 1;

static int callback_client_neo(struct libwebsocket_context * that,
                                   struct libwebsocket *wsi,
                                   enum libwebsocket_callback_reasons reason,
                                   void *user,
                                   void *in,
                                   size_t len)
{
	switch (reason) {
		case LWS_CALLBACK_CLIENT_ESTABLISHED: // just log message that someone is connecting
			{
				size_t buf_len = 256;
				size_t sent_len, ret_len;
				unsigned char *buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + \
									     			  buf_len + \
									     			  LWS_SEND_BUFFER_POST_PADDING);
				sent_len = sprintf((unsigned char *)&buf[LWS_SEND_BUFFER_PRE_PADDING], "client know established");
				printf("connection client established\n");
				ret_len = libwebsocket_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], sent_len, LWS_WRITE_TEXT);
				if (ret_len != sent_len){
					printf("client send message error!!!\n");
				}
				free(buf);
				break;
			}
		case LWS_CALLBACK_CLIENT_RECEIVE:
			{
				printf("clent received data: %s\n",(char *)in);
			}
			break;
		default:
			break;
	};
        return 0;
}

static struct libwebsocket_protocols protocols[] = {
	{
                NULL, // protocol name - very important!
                callback_client_neo,   // callback
                0                          // we don't use any per session data
        },
	{
		NULL, NULL, 0   /* End of list */
	}
};



int main()
{
	int rc = 0;
	struct lws_context_creation_info info;
	memset(&info, 0, sizeof info);
	info.port = CONTEXT_PORT_NO_LISTEN;
	info.iface = NULL;
	info.gid = -1;
	info.uid = -1;
	info.options = 0;
	info.extensions = libwebsocket_get_internal_extensions();
	int debug_level = 7;
	lws_set_log_level(debug_level, lwsl_emit_syslog);
	info.protocols = protocols;
	struct libwebsocket_context *context;
	context = libwebsocket_create_context(&info);
	struct libwebsocket *wsi;
	wsi = libwebsocket_client_connect(context, 
									  "127.0.0.1", 
									  8000, 
									  0,          //"ws:" (no ssl) 
									  "/", 		  //path
									  "127.0.0.1", //host name
									  "", //soket origin name
									  NULL, -1);


	while (rc >= 0 && is_looping) {
		//printf("ser\n");
		rc = libwebsocket_service(context, 50);
	}
}
