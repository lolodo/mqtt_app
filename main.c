#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>  
#include <mosquitto.h>

pthread_t mqtt;
char topic[30];

static void print_usage(const char *prog)
{
    printf("Usage: %s [options]\n", prog);
    puts(   "  -t --topic               specify topics to subscribe\n"
            "  -h --help                print help info \n");
}

static void parse_opts(int argc, char *argv[])
{
    int c;

    while (1) {
        static const struct option lopts[] = {
            { "topic", 1, 0, 't' },
            { "help", 0, 0, 'h' },
            { NULL, 0, 0, 0 },
        };

        c = getopt_long(argc, argv, "t:h", lopts, NULL);

        if (c == -1)
            break;

        switch (c) {
            case 't':
                memcpy(topic, optarg, sizeof(optarg));
                topic[sizeof(optarg)] = '\0';
                break;

            case '?':
            case 'h':
            default:
                print_usage(argv[0]);
                break;

        }
    }

}

int pubsub_callback (char *message, int len)
{
    if (message != NULL) {
        printf("topic:%s, len:%d, message:%s\n", topic, len, message);
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

    parse_opts(argc, argv);
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
