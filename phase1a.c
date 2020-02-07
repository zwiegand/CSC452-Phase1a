#include "phase1Int.h"
#include "usloss.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

extern  USLOSS_PTE  *P3_AllocatePageTable(int cid);
extern  void        P3_FreePageTable(int cid);

typedef struct Context {
    void            (*startFunc)(void *);
    void            *startArg;
    USLOSS_Context  context;
    // you'll need more stuff here
    Boolean         isFree;
    int             cid;
    USLOSS_PTE      pageTable;
} Context;

static Context   contexts[P1_MAXPROC];

static int currentCid = -1;

/*
 * Helper function to call func passed to P1ContextCreate with its arg.
 */
static void launch(void)
{
    assert(contexts[currentCid].startFunc != NULL);
    contexts[currentCid].startFunc(contexts[currentCid].startArg);
}

void P1ContextInit(void)
{
    //This function was given to us empty, all code I added, and
    // therefore could be wrong.
    int i;
    // ZACK
    // I just went through and said all the contexts in the array
    // of contexts defined are free. 
    // I was thinking as P1ContextCreate takes contexts in the 
    // array 'contexts', it can mark isFree = false.
    for(i = 0; i < P1_MAXPROC; i++){
        contexts[i].isFree = true;
    }
}

int P1ContextCreate(void (*func)(void *), void *arg, int stacksize, int *cid) {
    int result = P1_SUCCESS;
    // Above is given
    // find a free context and initialize it
    // allocate the stack, specify the startFunc, etc.
    int i;
    for (i = 0; i < P1_MAXPROC; i++){
        if (contexts[i].isFree == true){
            //Code in here is executed if a free context is found.

            // Checking if stacksize passed in is min size.
            if (stacksize < USLOSS_MIN_STACK){
                result = P1_INVALID_STACK;
                // We exit before we mark the free context false.
                // We didn't fill the context,
                //  there is no data in the context[i]
                // so we exit and leave isFree = true.
                return result;
            }
            char newStack[stacksize];

            //Specifying startFunc and its startArg
            contexts[i].startArg = arg;
            contexts[i].startFunc = func;
            //Letting the context know its own cid.
            contexts[i].cid = cid;
            // The P3_allocatePageTable asks for a (int pid) as
            //  parameter. Not sure if the Process ID (pid) is the same
            //  as a Context ID (cid)? But i'm making the assumption for 
            // now. There is no other purpose for the cid here, so I think
            //  it really is right.
            contexts[i].pageTable = P3_AllocatePageTable(cid);
            // So first parameter is (USLOSS_Context *context).
            // In phase0 and examples you would put &myContext as that parameter,
            // i'm not sure exactly if I did that right here with (contexts[i]->context).

            USLOSS_ContextInit(contexts[i].context, newStack, sizeof(newStack),
            contexts[i].pageTable, contexts[i].startFunc);
            
            // We filled context so it is not free.
            contexts[i].isFree = false;

            // result is still P1_Success
            return result;
        
        }
    }
    // If the code gets to here without returning, then there were
    //  no free contexts to be found. Return value will reflect that.
    result = P1_TOO_MANY_CONTEXTS;
    // Below is given
    return result;
}

// I have not touched anything below here - Zack

int P1ContextSwitch(int cid) {
    int result = P1_SUCCESS;
    // switch to the specified context
    return result;
}

int P1ContextFree(int cid) {
    int result = P1_SUCCESS;
    // free the stack and mark the context as unused
    return result;
}


void 
P1EnableInterrupts(void) 
{
    // set the interrupt bit in the PSR
}

/*
 * Returns true if interrupts were enabled, false otherwise.
 */
int 
P1DisableInterrupts(void) 
{
    int enabled = FALSE;
    // set enabled to TRUE if interrupts are already enabled
    // clear the interrupt bit in the PSR
    return enabled;
}
