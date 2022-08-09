#include <stdint.h>

#include "pmsis.h"
#include "network.h"

int8_t explorer()
{

    /* devices */
    struct pi_device cluster_dev;

    /* configs */
    struct pi_cluster_conf cluster_conf;

    /* task structure */
    struct pi_cluster_task cluster_task;
    struct pi_task task;

    /* First open the cluster */
    pi_cluster_conf_init(&conf);
    pi_open_from_conf(&cluster_dev, &conf);

    if (pi_cluster_open(&cluster_dev))
        return -1;

    /* offload an entry point. This will get executed on the cluster controller */
    pi_cluster_send_task_to_cl(&cluster_dev, pi_cluster_task(&cluster_task, cluster_entry, NULL));

    return 0;
}

void explorer_kickoff(void *arg)
{
    int ret = explorer();
    pmsis_exit(ret);
}

int main()
{
    return pmsis_kickoff((void *)explorer_kickoff);
}
