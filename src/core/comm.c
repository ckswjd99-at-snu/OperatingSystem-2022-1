/********************************************************
 * Filename: core/comm.c
 *  
 * Author: jtlim, RTOSLab. SNU.
 * 
 * Description: message queue management. 
 ********************************************************/
#include <core/eos.h>

void eos_init_mqueue(eos_mqueue_t *mq, void *queue_start, int16u_t queue_size, int8u_t msg_size, int8u_t queue_type) {
  eos_semaphore_t* putsem = (eos_semaphore_t*)malloc(sizeof(eos_semaphore_t));
  eos_semaphore_t* getsem = (eos_semaphore_t*)malloc(sizeof(eos_semaphore_t));
  eos_init_semaphore(putsem, (int32u_t)queue_size, queue_type);
  eos_init_semaphore(getsem, 0, queue_type);

  mq->queue_start = queue_start;
  mq->queue_size = queue_size;
  mq->queue_end = (int8u_t *)(mq->queue_start) + mq->queue_size;
  mq->msg_size = msg_size;
  mq->queue_type = queue_type;
  mq->putsem = putsem;
  mq->getsem = getsem;
  mq->front = queue_start;
  mq->rear = (int8u_t *)queue_start - 1;
}

int8u_t eos_send_message(eos_mqueue_t *mq, void *message, int32s_t timeout) {
  int8u_t putsem_aquired = eos_acquire_semaphore(mq->putsem, timeout);
  if (putsem_aquired) { 
    for (int32u_t i=0; i < (mq->msg_size); i++) {
      mq->rear++;
      int8u_t m_char = ((int8u_t*)message)[i]
      *(int8u_t*)mq->rear = m_char;
      if (mq->queue_end <= mq->rear) {
        mq->rear = (int8u_t *)(mq->queue_start) - 1;
      }
    }
    eos_release_semaphore(mq->getsem);
  }
  return 0;
}

int8u_t eos_receive_message(eos_mqueue_t *mq, void *message, int32s_t timeout) {
  int8u_t getsem_aquired = eos_acquire_semaphore(mq->getsem, timeout);
  if (getsem_aquired) {
    for (int32u_t i=0; i < (mq->msg_size); i++) {
      mq->front++;
      ((int8u_t *)message)[i] = *((int8u_t *)mq->front);
      if (mq->queue_end < mq->front) {
        mq->front = mq->queue_start;
      }
    }
    eos_release_semaphore(mq->putsem);
  }
  return 0;
}
