/********************************************************
 * Filename: core/comm.c
 *  
 * Author: jtlim, RTOSLab. SNU.
 * 
 * Description: message queue management. 
 ********************************************************/
#include <core/eos.h>

void eos_init_mqueue(eos_mqueue_t *mq, void *queue_start, int16u_t queue_size, int8u_t msg_size, int8u_t queue_type) {
  mq -> queue_size = queue_size;
  mq -> msg_size = msg_size;
  mq -> queue_start = queue_start;
  mq -> queue_type = queue_type;
  eos_init_semaphore(&(mq -> putsem), (int32u_t)queue_size, queue_type);
  eos_init_semaphore(&(mq -> getsem), 0, queue_type);
  mq -> front = queue_start;
  mq -> rear = (int8u_t *)queue_start - 1;
}

int8u_t eos_send_message(eos_mqueue_t *mq, void *message, int32s_t timeout) {
  if (eos_acquire_semaphore(&mq -> putsem, timeout)){ 
    // semaphore 획득 성공
    for(int8u_t i = 0; i < mq -> msg_size; i++){
      *(int8u_t *)++(mq -> rear) = ((int8u_t *)message)[i];
      if (mq -> rear >= (int8u_t *)(mq -> queue_start) + mq -> queue_size) {
        mq -> rear = (int8u_t *)mq -> queue_start - 1;
      }
    }
    eos_release_semaphore(&(mq -> getsem));
  } else { 
    // semaphore 획득 실패
  }
  return 0;
}

int8u_t eos_receive_message(eos_mqueue_t *mq, void *message, int32s_t timeout) {
  if (eos_acquire_semaphore(&mq->getsem, timeout))
  {
    // semaphore 획득 성공
    for (int8u_t i = 0; i < mq->msg_size; i++) {
      ((int8u_t *)message)[i] = *((int8u_t *)(mq->front)++);
      if (mq->front > (int8u_t *)(mq->queue_start) + mq->queue_size) {
        mq->front = mq->queue_start;
      }
    }
    eos_release_semaphore(&(mq->putsem));
  }
  else
  {
    // semaphore 획득 실패
  }
  return 0;
}
