/*
 * Copyright (c) 2014-2021 Kevin Huck
 * Copyright (c) 2014-2021 University of Oregon
 *
 * Distributed under the BSD 2-Clause Software License. (See accompanying
 * file LICENSE)
 */

#include <stddef.h>
#include "tasktimer.h"

uint64_t new_guid(void) {
    static uint64_t count = 0;
    return count++;
}

void A(uint64_t);
void B(uint64_t, uint64_t);
void C(uint64_t, uint64_t);

void A(uint64_t parent) {
    uint64_t parents[] = {parent};
    uint64_t myguid = new_guid();
    // both address and name
    TASKTIMER_CREATE(&A, "A", myguid, parents, 1, tt_A);
    TASKTIMER_SCHEDULE(tt_A, NULL, 0);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = 0;
    TASKTIMER_START(tt_A, &resource);
    B(parent, myguid);
    C(parent, myguid);
    TASKTIMER_STOP(tt_A);
}

void B(uint64_t parent1, uint64_t parent2) {
    uint64_t parents[] = {parent1, parent2};
    uint64_t myguid = new_guid();
    // both address and name
    TASKTIMER_CREATE(&B, "B", myguid, parents, 2, tt_B);
    TASKTIMER_SCHEDULE(tt_B, NULL, 0);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = 0;
    TASKTIMER_START(tt_B, &resource);
    TASKTIMER_STOP(tt_B);
}

void C(uint64_t parent1, uint64_t parent2) {
    uint64_t parents[] = {parent1, parent2};
    uint64_t myguid = new_guid();
    // no name, just address
    TASKTIMER_CREATE(&C, NULL, myguid, parents, 2, tt_C);
    TASKTIMER_SCHEDULE(tt_C, NULL, 0);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = 0;
    TASKTIMER_START(tt_C, &resource);
    TASKTIMER_STOP(tt_C);
}

int main(int argc, char * argv[]) {
    uint64_t myguid = new_guid();
    // no address, just name
    TASKTIMER_INITIALIZE();
    TASKTIMER_CREATE(NULL, "main", myguid, NULL, 0, tt);
    TASKTIMER_SCHEDULE(tt, NULL, 0);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = 0;
    TASKTIMER_START(tt, &resource);
    TASKTIMER_YIELD(tt);
    A(myguid);
    TASKTIMER_RESUME(tt, &resource);
    TASKTIMER_STOP(tt);
    TASKTIMER_FINALIZE();
    return 0;
}
