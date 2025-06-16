#include <semaphore.h>
#include <lib.h>
#include <processManager.h>
#include <PCBQueueADT.h>
#include <scheduler.h>
#define SEM_NAME_MATCH(name1, name2) (safe_strncpy((name1), (name2), MAX_NAME_LEN) == 0)


NamedSemaphore namedSemaphores[MAX_SEMAPHORES];

uint8_t sem_init(Semaphore* sem, uint8_t initial_value) {
    if (!sem || initial_value < 0)
        return -1;

    sem->value = initial_value;
    sem->waiters = CreatePCBQueueADT();  // Or zero/init static queue
    if (!sem->waiters) return -1;

    return 0;
}


uint8_t sem_open(const char* name, uint8_t initial_value) {
    if (!name) return -1;

    // First, search for existing
    for (uint8_t i = 0; i < MAX_SEMAPHORES; i++) {
        if (namedSemaphores[i].inUse && SEM_NAME_MATCH(namedSemaphores[i].name, name)) {
            return i;
        }
    }

    // Create new semaphore
    for (uint8_t i = 0; i < MAX_SEMAPHORES; i++) {
        if (!namedSemaphores[i].inUse) {
            namedSemaphores[i].inUse = 1;
            safe_strncpy(namedSemaphores[i].name, name, MAX_NAME_LEN);
            sem_init(&namedSemaphores[i].sem, initial_value);
            return i;
        }
    }

    return -1;  // No free slots
}

void sem_post(uint8_t id) {
    if (id < 0 || id >= MAX_SEMAPHORES || !namedSemaphores[id].inUse)
        return;

    Semaphore* sem = &namedSemaphores[id].sem;
    _cli();

    if (getPCBQueueSize(sem->waiters) > 0) {
        PCB* p = dequeueProcess(sem->waiters);
        unblockProcess(p); 
    } else {
        sem->value++;
    }

    _sti();
}


void sem_wait(uint8_t id) {
    if (id < 0 || id >= MAX_SEMAPHORES || !namedSemaphores[id].inUse)
        return;

    Semaphore* sem = &namedSemaphores[id].sem;
    _cli();

    if (sem->value > 0) {
        sem->value--;
    } else {
        PCB* current = getCurrentProcess();
        blockProcess(current);
        queueProcess(sem->waiters, current);
        _sti();
        yield();  // Voluntary context switch
        return;
    }

    _sti();
}

void sem_destroy(uint8_t id){
	namedSemaphores[id].inUse = 0;
}







