#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <unistd.h>
#include <mosquitto.h>

#define TOPIC_LEN 30

char topic[TOPIC_LEN];
static void print_usage(const char *prog)
{
    printf("Usage: %s [options]\n", prog);
    puts(   "  -t --topic               specify topics to subscribe\n"
            "  -h --help                print help info \n");
}

static int parse_opts(int argc, char *argv[])
{
    int c;
    int ret = -1;

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
                ret = 0;
                break;

            case '?':
            case 'h':
            default:
                print_usage(argv[0]);
                break;
        }
    }

    return ret;
}

void mqtt_connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    printf("rc: %d\n", rc);
}

void mqtt_disconnect_callback(struct mosquitto *mosq, void *obj, int result)
{
    printf("disconnect: %d\n", result);
}

void mqtt_publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
    printf("[%s]enter!\n", __func__);
}

int main(int argc, char *argv[])
{
    int ret;
    struct mosquitto *mosq;

    ret = parse_opts(argc, argv);
    if (ret) {
        print_usage(argv[0]);
        return ret;
    }

    /* init lib */
    mosquitto_lib_init();
    mosq = mosquitto_new("master_avm", true, NULL);
    if (!mosq) {
        printf("alloc mosq failed!\n");
        return -1;
    }

    mosquitto_connect_callback_set(mosq, mqtt_connect_callback);
    mosquitto_disconnect_callback_set(mosq, mqtt_disconnect_callback);
    mosquitto_publish_callback_set(mosq, mqtt_publish_callback);

    ret = mosquitto_connect(mosq, "127.0.0.1", 1883, 600);
    if (ret != MOSQ_ERR_SUCCESS) {
        printf("connect failed!\n");
        return ret;
    }


    ret = mosquitto_publish(mosq, NULL, topic, 5, "hello", 0, false);
    if (ret != MOSQ_ERR_SUCCESS) {
        printf("publish failed!\n");
        return ret;
    }

    usleep(100);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
