#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>  
#include <errno.h>
#include <mosquitto.h>

pthread_t mqtt;
int pubsub_callback (char *message, int len)
{
    if (message != NULL) {
        printf("%s:%d len:%d message:%s\n", __func__, __LINE__, len, message);
    //    json_parse_message(message);
    }
}

void connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    printf("connect!\n");
}

void disconnect_callback(struct mosquitto *mosq, void *obj, int result)
{
    printf("disconnect, result:%d!\n", result);
}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    pubsub_callback(msg->payload, msg->payloadlen);
}

void *pubsub_func(void *arg) 
{
    int mid = 0;
    int rc;
    struct mosquitto *mosq;
    char *topic = "avm/#";

    /* set mqtt */
    mosq = mosquitto_new("client", true, NULL);
    mosquitto_connect_callback_set(mosq, connect_callback);
    mosquitto_disconnect_callback_set(mosq, disconnect_callback);
    mosquitto_message_callback_set(mosq, message_callback);
    mosquitto_connect(mosq, "127.0.0.1", 1883, 600);

    /* subscribe topics */
    mosquitto_subscribe(mosq, &mid, topic, 0);

    while(1) {
        rc = mosquitto_loop(mosq, 1, 10);
        if (rc != MOSQ_ERR_SUCCESS) {
            printf("result is %d\n", rc);
        }
    }
}


int main(int argc, char *argv[])
{
    int ret = 0;

    //parse_opts(argc, argv);
    mosquitto_lib_init();
    ret = pthread_create(&mqtt, NULL, pubsub_func, NULL);
    if (ret) {
        printf("create mqtt thread failed!\n");
        return ret;
    }

    printf("[%s]Waiting for mqtt msg...\n", __func__);

    /* block threads */
    pthread_join(mqtt, NULL);

    printf("[%s]quit!\n", __func__);

    return ret;
}
